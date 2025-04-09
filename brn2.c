/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023-2024 Mior, Lucas; 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BRN2_C
#define BRN2_C

#include "brn2.h"
#include "hash.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "hash.c"
#include "util.c"
#include "arena.c"

static void *brn2_threads_work_hashes(void *);
static void *brn2_threads_work_normalization(void *);
static void *brn2_threads_work_changes(void *);
static inline bool brn2_is_invalid_name(char *);
static void brn2_slash_add(FileName *);

int
brn2_compare(const void *a, const void *b) {
    const FileName *file_a = a;
    const FileName *file_b = b;
    return strcmp(file_a->name, file_b->name);
}

void
brn2_print_list(FileList *list) {
    for (uint32 i = 0; i < list->length; i += 1) {
        printf("[%u] = %s\n", i, list->files[i].name);
    }
    return;
}

FileList *
brn2_list_from_args(int argc, char **argv) {
    FileList *list;
    uint32 length = 0;

    list = xmalloc(STRUCT_ARRAY_SIZE(list, FileName, argc));
    memset(list, 0, sizeof(*list));
    list->arena = arena_old;

    for (int i = 0; i < argc; i += 1) {
        char *name = argv[i];
        FileName *file = &(list->files[length]);
        uint32 size;

        if (brn2_is_invalid_name(name))
            continue;

        file->length = (uint16)strlen(name);
        size = ALIGN(file->length+2);
        file->name = arena_push(list->arena, size);
        BRN2_ASSUME_ALIGNED(file->name);
        memcpy(file->name, name, size);
        memset(&file->name[file->length], 0, size - file->length);

        length += 1;
    }
    list->length = length;

    return list;
}

#ifdef __WIN32__
int scandir(const char *dir, struct dirent ***namelist,
            void *filter, void *compar) {
    WIN32_FIND_DATAA find_data;
    HANDLE hFind;
    char buffer[MAX_PATH];
    char *path;
    size_t count = 0;
    size_t capacity = 16;
    (void) filter;
    (void) compar;

    struct dirent **list = xmalloc(capacity*sizeof(*list));

    path = SNPRINTF(buffer, "%s/*", dir);
    hFind = FindFirstFileA(path, &find_data);
    if (hFind == INVALID_HANDLE_VALUE) {
        free(list);
        return -1;
    }

    do {
        struct dirent *ent = xmalloc(sizeof(*ent));

        strncpy(ent->d_name, find_data.cFileName, MAX_PATH);
        if (count >= capacity) {
            capacity *= 2;
            list = xrealloc(list, capacity*sizeof(*list));
        }
        list[count++] = ent;
    } while (FindNextFileA(hFind, &find_data));
    FindClose(hFind);

    *namelist = list;
    return (int)count;
}
#endif

FileList *
brn2_list_from_dir(char *directory) {
    FileList *list;
    struct dirent **directory_list;
    uint32 length = 0;
    uint16 directory_length;
    int number_files;

    if (strcmp(directory, "."))
       directory_length = (uint16)strlen(directory);
    else
       directory_length = 0;

    number_files = scandir(directory, &directory_list, NULL, NULL);
    if (number_files < 0) {
        error("Error scanning '%s': %s.\n", directory, strerror(errno));
        exit(EXIT_FAILURE);
    }

    list = xmalloc(STRUCT_ARRAY_SIZE(list, FileName, number_files));
    memset(list, 0, sizeof(*list));
    list->arena = arena_old;

    for (int i = 0; i < number_files; i += 1) {
        FileName *file = &(list->files[length]);
        char *name = directory_list[i]->d_name;
        uint16 name_length = (uint16)strlen(name);
        uint32 size;

        if (brn2_is_invalid_name(name)) {
            if (BRN2_DEBUG)
                free(directory_list[i]);
            continue;
        }

        if (directory_length) {
            file->length = directory_length + 1 + name_length;
            size = ALIGN(file->length + 2);
            file->name = arena_push(list->arena, size);
            BRN2_ASSUME_ALIGNED(file->name);

            memcpy(file->name, directory, directory_length);
            file->name[directory_length] = '/';
            memcpy(file->name + directory_length + 1, name, name_length + 1);
        } else {
            file->length = name_length;
            size = ALIGN(file->length + 2);
            file->name = arena_push(list->arena, size);
            BRN2_ASSUME_ALIGNED(file->name);

            memcpy(file->name, name, file->length + 1);
        }
        memset(&file->name[file->length], 0, size - file->length);

        if (BRN2_DEBUG)
            free(directory_list[i]);
        length += 1;
    }
    free(directory_list);
    list->length = length;
    return list;
}

#ifndef __WIN32__
FileList *
brn2_list_from_dir_recurse(char *directory) {
    FileList *list;
    char* const paths[] = { directory, NULL };
    FTS *file_system = NULL;
    FTSENT *ent = NULL;
    uint32 length = 0;
    uint32 capacity = 64;
    uint32 size;

    list = xmalloc(STRUCT_ARRAY_SIZE(list, FileName, capacity));
    memset(list, 0, sizeof(*list));
    list->arena = arena_old;

    file_system = fts_open(paths, FTS_PHYSICAL|FTS_NOSTAT, NULL);
    if (file_system == NULL) {
        error("Error opening '%s' for traversal: %s.\n",
              directory, strerror(errno));
        exit(EXIT_FAILURE);
    }
    errno = 0;

    while ((ent = fts_read(file_system))) {
        switch (ent->fts_info) {
        case FTS_ERR:
            error("Error in fts_read(%s): %s.\n",
                  directory, strerror(ent->fts_errno));
            exit(EXIT_FAILURE);
        case FTS_D:
            // fallthrough
        case FTS_NSOK: {
            char *name = ent->fts_path;
            FileName *file;

            if (brn2_is_invalid_name(name))
                continue;

            if (length >= capacity) {
                capacity *= 2;
                list = xrealloc(list,
                                STRUCT_ARRAY_SIZE(list, FileName, capacity));
            }

            file = &(list->files[length]);
            file->length = ent->fts_pathlen;
            size = ALIGN(file->length+2);
            file->name = arena_push(list->arena, size);
            BRN2_ASSUME_ALIGNED(file->name);
            memcpy(file->name, name, size);
            memset(&file->name[file->length], 0, size - file->length);

            length += 1;
            break;
        }
        default:
            break;
        }
    }
    if (errno)
        error("Error in fts_read(%s): %s.\n", directory, strerror(errno));
    if (fts_close(file_system) < 0)
        error("Error in fts_close(%s): %s.\n", directory, strerror(errno));

    if (length == 0) {
        error("Empty list. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    list = xrealloc(list, STRUCT_ARRAY_SIZE(list, FileName, length));
    list->length = length;
    return list;
}
#endif

void
brn2_free_list(FileList *list) {
    arena_reset(list->arena);
    free(list);
    return;
}

#ifdef __linux__
FileList *
brn2_list_from_lines(char *filename, bool is_old) {
    FileList *list;
    char *map;
    char *begin;
    char *pointer;
    size_t left;
    uint32 length = 0;
    uint32 map_size;
    uint32 padding;
    uint32 capacity;
    int fd;

    if ((fd = open(filename, O_RDWR)) < 0) {
        error("Error opening '%s' for reading: %s.\n",
              filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    {
        struct stat lines_stat;
        if (fstat(fd, &lines_stat) < 0) {
            error("Error in fstat(%s): %s.\n", filename, strerror(errno));
            exit(EXIT_FAILURE);
        }
        map_size = (uint32)lines_stat.st_size;
        if (map_size <= 0) {
            error("map_size: %u\n", map_size);
            exit(EXIT_FAILURE);
        }
    }
    padding = BRN2_ALIGNMENT - (map_size % BRN2_ALIGNMENT);
    map_size += padding;
    if (ftruncate(fd, map_size) < 0) {
        error("Error in ftruncate(%s): %s.\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    capacity = map_size/2;
    list = xmalloc(STRUCT_ARRAY_SIZE(list, FileName, capacity));
    memset(list, 0, sizeof(*list));
    if (is_old)
        list->arena = arena_old;
    else
        list->arena = arena_new;

    map = mmap(NULL, map_size,
                     PROT_READ | PROT_WRITE, MAP_PRIVATE,
                     fd, 0);

    if (map == MAP_FAILED) {
        error("Error mapping history file to memory: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    begin = pointer = map;
    left = map_size - padding;

    while ((left > 0) && (pointer = memchr(pointer, '\n', left))) {
        FileName *file = &(list->files[length]);
        uint32 size;

        *pointer = '\0';
        if (is_old && brn2_is_invalid_name(begin)) {
            begin = pointer + 1;
            continue;
        }
        if (begin == pointer) {
            error("Empty line in file. Exiting.\n");
            exit(EXIT_FAILURE);
        }

        file->length = (uint16)(pointer - begin);
        size = ALIGN(file->length+2);
        file->name = arena_push(list->arena, size);
        BRN2_ASSUME_ALIGNED(file->name);
        memcpy(file->name, begin, size);
        memset(&file->name[file->length], 0, size - file->length);

        begin = pointer + 1;
        pointer += 1;
        length += 1;
        left -= (file->length + 1);
    }

    if (length == 0) {
        error("Empty list. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    list = xrealloc(list, STRUCT_ARRAY_SIZE(list, FileName, length));
    list->length = length;
    munmap(map, map_size);

    if (ftruncate(fd, map_size - padding) < 0) {
        error("Error truncating '%s': %s.\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (close(fd) < 0) {
        error("Error closing '%s': %s.\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return list;
}
#else
FileList *
brn2_list_from_lines(char *filename, bool is_old) {
    size_t length = 0;
    char buffer[BRN2_PATH_MAX];
    FileList *list;
    uint32 cap = 128;
    FILE *lines;
    
    if ((lines = fopen(filename, "r")) == NULL) {
        error("Error opening '%s': %s.\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    list = xmalloc(STRUCT_ARRAY_SIZE(list, FileName, cap));

    if (is_old)
        list->arena = arena_old;
    else
        list->arena = arena_new;

    while (!feof(lines)) {
        FileName *file;
        if (length >= cap) {
            cap *= 2;
            list = xrealloc(list, STRUCT_ARRAY_SIZE(list, FileName, cap));
        }
        file = &(list->files[length]);

        if (!fgets(buffer, sizeof(buffer), lines))
            continue;

        file->length = strcspn(buffer, "\n");
        buffer[file->length] = '\0';
        file->name = arena_push(list->arena, file->length + 1);
        memcpy(file->name, buffer, file->length + 1);
        length += 1;
    }
    fclose(lines);
    list->length = length;
    return list;
}
#endif

bool
brn2_is_invalid_name(char *filename) {
    while (*filename) {
        if ((*filename != '.') && (*filename != '/'))
            return false;
        filename += 1;
    }
    return true;
}

typedef struct Slice {
    FileList *old_list;
    FileList *new_list;
    uint32 start;
    uint32 end;
    uint32 map_capacity;
    uint32 unused;
    uint32 *partial;
} Slice;

#ifndef __WIN32__
void *
brn2_threads_work_normalization(void *arg) {
    Slice *slice = arg;
    FileList *list;
    bool old_list;

    if (slice->new_list) {
        list = slice->new_list;
        old_list = false;
    } else {
        list = slice->old_list;
        old_list = true;
    }

    for (uint32 i = slice->start; i < slice->end; i += 1) {
        FileName *file = &(list->files[i]);
        char *p;
        uint32 off = 0;

        while ((p = memmem(file->name + off, file->length - off, "//", 2))) {
            off = (uint32)(p - file->name);

            memmove(&p[0], &p[1], file->length - off);
            file->length -= 1;
        }

        while (file->name[0] == '.' && file->name[1] == '/') {
            memmove(&file->name[0], &file->name[2], file->length - 1);
            file->length -= 2;
        }

        off = 0;
        while ((p = memmem(file->name + off, file->length - off, "/./", 3))) {
            off = (uint32)(p - file->name);

            memmove(&p[1], &p[3], file->length - off - 2);
            file->length -= 2;
        }

        if (old_list) {
            struct stat file_stat;
            if (stat(file->name, &file_stat) < 0) {
                if (errno != ENOENT) {
                    error("Error in stat('%s'): %s.\n",
                          file->name, strerror(errno));
                }
                slice->old_list->files[i].type = TYPE_ERR;
                continue;
            }
            if (S_ISDIR(file_stat.st_mode)) {
                slice->old_list->files[i].type = TYPE_DIR;
                brn2_slash_add(file);
            } else {
                slice->old_list->files[i].type = TYPE_FILE;
            }
        } else {
            if (slice->old_list->files[i].type == TYPE_DIR)
                brn2_slash_add(file);
        }
    }
    return 0;
}
#else
void *
brn2_threads_work_normalization(void *arg) {
    (void) arg;
    return 0;
}
#endif

void
brn2_slash_add(FileName *file) {
    if (file->name[file->length - 1] != '/') {
        file->name[file->length] = '/';
        file->name[file->length+1] = '\0';
        file->length += 1;
    }
    return;
}

void *
brn2_threads_work_sort(void *arg) {
    Slice *slice = arg;
    FileName *files = &(slice->old_list->files[slice->start]);
    qsort(files, slice->end - slice->start, sizeof (*files), brn2_compare);
    return 0;
}

void *
brn2_threads_work_hashes(void *arg) {
    Slice *slice = arg;

    for (uint32 i = slice->start; i < slice->end; i += 1) {
        FileList *list = slice->old_list;
        FileName *newfile = &(list->files[i]);
        newfile->hash = hash_function(newfile->name, newfile->length);
        list->indexes[i] = newfile->hash % slice->map_capacity;
    }
    return 0;
}

void *
brn2_threads_work_changes(void *arg) {
    Slice *slice = arg;

    for (uint32 i = slice->start; i < slice->end; i += 1) {
        FileName oldfile = slice->old_list->files[i];
        FileName newfile = slice->new_list->files[i];
        if (oldfile.length == newfile.length) {
            if (!memcmp(oldfile.name, newfile.name, oldfile.length))
                continue;
        }
        *(slice->partial) += 1;
    }
    return 0;
}

void
brn2_timings(char *name,
             struct timespec t0, struct timespec t1, uint32 length) {
    long seconds = t1.tv_sec - t0.tv_sec;
    long nanos = t1.tv_nsec - t0.tv_nsec;

    double total_seconds = (double)seconds + (double)nanos/1.0e9;
    double micros_per_item = 1e6*(total_seconds/(double)(length));

    printf("\ntime elapsed %s\n", name);
    printf("%gs = %gus per item.\n\n", total_seconds, micros_per_item);
    return;
}

void
brn2_normalize_names(FileList *old, FileList *new) {
    brn2_threads(brn2_threads_work_normalization, old, new, NULL, 0);
    return;
}

void
brn2_create_hashes(FileList *list, uint32 map_capacity) {
    brn2_threads(brn2_threads_work_hashes,
                 list, NULL, NULL, map_capacity);
    return;
}

uint32
brn2_get_number_changes(FileList *old, FileList *new) {
    uint32 total = 0;
    uint32 numbers[BRN2_MAX_THREADS] = {0};
    brn2_threads(brn2_threads_work_changes, old, new, numbers, 0);

    for (uint32 i = 0; i < BRN2_MAX_THREADS; i += 1)
        total += numbers[i];
    return total;
}

#ifndef __WIN32__
uint32
brn2_threads(void *(*function)(void *),
             FileList *old, FileList *new,
             uint32 *numbers, uint32 map_size) {
    pthread_t threads[BRN2_MAX_THREADS];
    Slice slices[BRN2_MAX_THREADS];
    uint32 range;
    uint32 length;

    if (old) {
        length = old->length;
    } else {
        length = new->length;
    }

    if (nthreads*2 >= length)
        nthreads = 1;
    if (length <= BRN2_MIN_PARALLEL)
        nthreads = 1;

    range = length / nthreads;

    for (uint32 i = 0; i < (nthreads - 1); i += 1) {
        slices[i].start = i*range;
        slices[i].end = (i + 1)*range;
        slices[i].old_list = old;
        slices[i].new_list = new;
        slices[i].partial = numbers ? &numbers[i] : NULL;
        slices[i].map_capacity = map_size;
        pthread_create(&threads[i], NULL, function, (void *)&slices[i]);
    }{
        uint32 i = nthreads - 1;
        slices[i].start = i*range;
        slices[i].end = length;
        slices[i].old_list = old;
        slices[i].new_list = new;
        slices[i].partial = numbers ? &numbers[i] : NULL;
        slices[i].map_capacity = map_size;
        pthread_create(&threads[i], NULL, function, (void *)&slices[i]);
    }

    for (uint32 i = 0; i < nthreads; i += 1)
        pthread_join(threads[i], NULL);
    return nthreads;
}
#else
uint32
brn2_threads(void *(*function)(void *),
             FileList *old, FileList *new,
             uint32 *numbers, uint32 map_size) {
    Slice slices[1];
    uint32 length;

    if (old) {
        length = old->length;
    } else {
        length = new->length;
    }

    slices[0].start = 0;
    slices[0].end = length;
    slices[0].old_list = old;
    slices[0].new_list = new;
    slices[0].partial = numbers ? &numbers[0] : NULL;
    slices[0].map_capacity = map_size;
    function((void *)&slices[0]);

    return 1;
}
#endif

bool
brn2_verify(FileList *new, HashSet *repeated_set, uint32 *hashes_new) {
    bool failed = false;

    for (uint32 i = 0; i < new->length; i += 1) {
        FileName newfile = new->files[i];

        if (newfile.length >= BRN2_PATH_MAX) {
            error("Error: filename on line %u is longer than %u bytes",
                  i + 1, BRN2_PATH_MAX);
            failed = true;
            if (brn2_options_fatal)
                exit(EXIT_FAILURE);
        }

        if (!hash_set_insert_pre_calc(repeated_set, newfile.name,
                                      newfile.hash, hashes_new[i])) {
            error("Error: "RED"'%s'"RESET " repeats on line %u.\n",
                  newfile.name, i + 1);
            failed = true;
            if (brn2_options_fatal)
                exit(EXIT_FAILURE);
        }
    }

    return !failed;
}

static inline int
noop(const char *unused, ...) {
    (void) unused;
    return 0;
}

void
brn2_execute2(FileList *old, FileList *new,
              HashMap *oldlist_map, HashSet *names_renamed,
              uint32 i, uint32 *number_renames) {
    int renamed;
    uint32 *newname_index_on_oldlist;
    bool newname_exists;
    char **oldname = &(old->files[i].name);
    char *newname = new->files[i].name;

    uint16 *oldlength = &(old->files[i].length);

    uint32 newhash = new->files[i].hash;
    uint32 newindex = new->indexes[i];

    uint32 oldhash = old->files[i].hash;
    uint32 oldindex = old->indexes[i];

    if (newhash == oldhash) {
        if (!strcmp(*oldname, newname))
            return;
    }
    newname_index_on_oldlist = hash_map_lookup_pre_calc(oldlist_map,
                                                        newname,
                                                        newhash, newindex);
    newname_exists = !access(newname, F_OK);
#ifdef __linux__
    if (newname_exists
        && !newname_index_on_oldlist
        && !brn2_options_implicit) {
        error("Error renaming "RED"'%s'"RESET" to "RED"'%s'"RESET":\n",
              *oldname, newname);
        error(RED"'%s'"RESET" already exists,"
              " but it was not given in the list of"
              " files to rename, and --implict option is off.\n", newname);
        if (brn2_options_fatal)
            exit(EXIT_FAILURE);
        return;
    }
    if (newname_exists) {
        renamed = renameat2(AT_FDCWD, *oldname,
                            AT_FDCWD, newname, RENAME_EXCHANGE);
        if (renamed >= 0) {
            if (hash_set_insert_pre_calc(names_renamed,
                                         *oldname, oldhash, oldindex)) {
                *number_renames += 1;
            }
            if (hash_set_insert_pre_calc(names_renamed,
                                         newname, newhash, newindex)) {
                *number_renames += 1;
            }
            print(GREEN"%s"RESET" <-> "GREEN"%s"RESET"\n",
                  *oldname, newname);

            if (newname_index_on_oldlist) {
                uint32 next = *newname_index_on_oldlist;
                FileName *file_j = &(old->files[next]);

                hash_map_remove_pre_calc(oldlist_map,
                                         newname, newhash, newindex);
                hash_map_remove_pre_calc(oldlist_map,
                                         *oldname, oldhash, oldindex);

                hash_map_insert_pre_calc(oldlist_map,
                                         newname, newhash, newindex, i);
                hash_map_insert_pre_calc(oldlist_map,
                                         *oldname, oldhash, oldindex, next);

                SWAP(file_j->name, *oldname);
                SWAP(file_j->length, *oldlength);
                SWAP(file_j->hash, old->files[i].hash);
                SWAP(old->indexes[i], old->indexes[next]);
            } else {
                error("Warning: '%s' was swapped with '%s', even though"
                      " '%s' was not in the list of files to rename.\n",
                      newname, *oldname, newname);
                error("To disable this behaviour,"
                      " don't pass the --implict option.\n");
                hash_map_insert_pre_calc(oldlist_map,
                                         newname, newhash, newindex, i);
            }
            return;
        } else if (errno != ENOENT) {
            error("Error swapping "RED"'%s'"RESET
                  " and "RED"'%s'"RESET": %s.\n",
                  *oldname, newname, strerror(errno));
            if (brn2_options_fatal)
                exit(EXIT_FAILURE);
        }
    }
#else
    (void) oldlength;
    (void) newname_index_on_oldlist;
    if (newname_exists) {
        error("Error renaming "RED"'%s'"RESET" to '%s': File already exists.\n",
              *oldname, newname);
        if (brn2_options_fatal)
            exit(EXIT_FAILURE);
        return;
    }
#endif
    renamed = rename(*oldname, newname);
    if (renamed < 0) {
        error("Error renaming "RED"'%s'"RESET " to "RED"'%s'"RESET": %s.\n",
              *oldname, newname, strerror(errno));
        if (brn2_options_fatal)
            exit(EXIT_FAILURE);
        return;
    } else {
        if (hash_set_insert_pre_calc(names_renamed,
                                     *oldname, oldhash, oldindex)) {
            *number_renames += 1;
        }
        print("%s -> "GREEN"%s"RESET"\n", *oldname, newname);
    }
    return;
}

void
brn2_usage(FILE *stream) {
    fprintf(stream,
    "usage: brn2 [OPTIONS] -- <file1> <file2> ...\n"
    "usage: brn2 [OPTIONS] -f <filename>\n"
    "usage: brn2 [OPTIONS] -d <dir>\n"
    "usage: brn2 [OPTIONS] -r <dir>\n"
    "Rename filenames based on provided arguments.\n"
    "\n"
    "Options:\n"
    "  -F, --fatal   : Exit on first renaming error.\n"
    "  -c, --check   : Ignored.\n"
    "  -e, --explict : Only rename files given in the list (default).\n"
    "  -h, --help    : Display this help message and exit.\n"
    "  -i, --implict : Rename files not given in the list of files to rename.\n"
    "  -q, --quiet   : Quiet mode; suppress output messages.\n"
    "  -s, --sort    : Disable sorting of original list.\n"
    "  -v, --verbose : Verbose mode (default); output messages.\n"
    "\n"
    "Arguments:\n"
    "  No arguments              : Rename files of current working directory.\n"
    "  1 or more arguments       : Rename filenames passed as arguments.\n"
    "  -d <dir>, --dir=<dir>     : Rename files in directory.\n"
    "  -f <file>, --file=<file>  : Rename filenames listed in this argument.\n"
    "  -r <dir>, --recurse=<dir> : Recursively find files to rename.\n");
    exit((int)(stream != stdout));
}

#ifdef TESTING_brn2
#include <assert.h>

bool brn2_options_fatal = false;
bool brn2_options_implicit = false;
bool brn2_options_quiet = false;
uint32 nthreads = 1;
Arena *arena_old;
Arena *arena_new;

static bool
contains_filename(FileList *list, FileName file, bool verbose) {
    for (uint32 i = 0; i < list->length; i += 1) {
        if (list->files[i].length != file.length)
            continue;
        if (!memcmp(list->files[i].name, file.name, file.length)) {
            printf(GREEN "%s == %s\n" RESET, file.name, list->files[i].name);
            if (i < (list->length - 1)) {
                list->length -= 1;
                memmove(&list->files[i], &list->files[i+1],
                        (list->length - i)*sizeof (*(list->files)));
            }
            return true;
        }
        if (verbose) {
            printf("%u / %u | %s != %s \n",
                   i+1, list->length, list->files[i].name, file.name);
        }
    }
    return false;
}
// flags: -lm

int (*print)(const char *, ...);

int main(void) {
    FileList *list1;
    FileList *list2;

    char *command = "ls -a > /tmp/brn2test";
    char *file = command + 8;

    arena_old = arena_alloc(BRN2_ARENA_SIZE);
    arena_new = arena_alloc(BRN2_ARENA_SIZE);

    system(command);
    list1 = brn2_list_from_dir(".");
    list2 = brn2_list_from_lines(file, true);

    brn2_normalize_names(list1, NULL);
    brn2_normalize_names(list2, NULL);

    assert(list1->length == list2->length);

    for (uint32 i = 0; i < list1->length; i += 1) {
        printf(RED"%u / %u\n"RESET, i+1, list1->length);
        assert(contains_filename(list2, list1->files[i], list1->length < 9999));
    }

    brn2_free_list(list1);
    brn2_free_list(list2);

    unlink(file);
    exit(0);
}
#endif

#endif
