/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023-2025 Mior, Lucas;
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

#if !defined(BRN2_C)
#define BRN2_C

#include "brn2.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "util.c"
#include "arena.c"

#if defined(__INCLUDE_LEVEL__) && __INCLUDE_LEVEL__ == 0
#define TESTING_brn2 1
#elif !defined(TESTING_brn2)
#define TESTING_brn2 0
#endif

#if OS_WINDOWS
#include "windows_functions.c"
#endif

static void *brn2_threads_work_hashes(void *);
static void *brn2_threads_work_normalization(void *);
static void *brn2_threads_work_changes(void *);
static inline bool brn2_is_invalid_name(char *);
static void brn2_slash_add(FileName *);
static void brn2_list_from_lines(FileList *, char *, bool);

#if OS_UNIX
static pthread_mutex_t brn2_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t brn2_done_work = PTHREAD_COND_INITIALIZER;

pthread_t thread_pool[BRN2_MAX_THREADS];
static uint32 work_pending = 0;
pthread_cond_t brn2_new_work = PTHREAD_COND_INITIALIZER;
#endif

typedef struct Work {
    void *(*function)(void *);
    FileList *old_list;
    FileList *new_list;
    uint32 start;
    uint32 end;
    uint32 map_capacity;
    uint32 unused;
    uint32 *partial;
} Work;

static struct WorkQueue {
    struct Work *items[BRN2_MAX_THREADS];
    uint32 head;
    uint32 tail;
    uint32 count;
    uint32 padding;
} work_queue = {0};

bool stop_threads = false;

static void *
xarena_push(Arena *arena, uint32 size) {
    void *p;

    if ((p = arena_push(arena, size)) == NULL) {
        error("%u bytes is too large object for arena.\n", size);
        exit(EXIT_FAILURE);
    }
    return p;
}

int
brn2_compare(const void *a, const void *b) {
    FileName *const *file_a = a;
    FileName *const *file_b = b;
    return strcmp((*file_a)->name, (*file_b)->name);
}

void
brn2_list_from_args(FileList *list, int argc, char **argv) {
    uint32 length = 0;

    list->files = xmalloc(argc*SIZEOF(*(list->files)));

    for (int i = 0; i < argc; i += 1) {
        char *name = argv[i];
        uint16 name_length = (uint16)strlen64(name);
        FileName **file_pointer = &(list->files[length]);
        FileName *file;
        uint32 size;

        if (brn2_is_invalid_name(name)) {
            continue;
        }

        size = STRUCT_ARRAY_SIZE(*file_pointer, char, name_length + 2);
        *file_pointer = xarena_push(list->arena, ALIGN(size));
        file = *file_pointer;

        file->length = (uint16)strlen64(name);
        memcpy64(file->name, name, name_length + 1);

        length += 1;
    }
    list->length = length;

    return;
}

#if OS_UNIX
static void
brn2_enqueue(Work *work) {
    xpthread_mutex_lock(&brn2_mutex);

    if (work_queue.count >= LENGTH(work_queue.items)) {
        error("Error: work queue is full.\n");
        exit(EXIT_FAILURE);
    }
    work_queue.items[work_queue.tail] = work;
    work_queue.tail = (work_queue.tail + 1) % LENGTH(work_queue.items);
    work_queue.count += 1;
    work_pending += 1;

    pthread_cond_signal(&brn2_new_work);
    xpthread_mutex_unlock(&brn2_mutex);
    return;
}

static void *
brn2_threads_function(void *arg) {
    (void)arg;

    while (true) {
        Work *work;

        xpthread_mutex_lock(&brn2_mutex);
        while (work_queue.count == 0 && !stop_threads) {
            pthread_cond_wait(&brn2_new_work, &brn2_mutex);
        }

        if (stop_threads) {
            xpthread_mutex_unlock(&brn2_mutex);
            pthread_exit(NULL);
        }

        if (work_queue.count == 0) {
            work = NULL;
        } else {
            work = work_queue.items[work_queue.head];
            work_queue.head = (work_queue.head + 1) % LENGTH(work_queue.items);
            work_queue.count -= 1;
        }

        xpthread_mutex_unlock(&brn2_mutex);

        if (work) {
            work->function(work);
            xpthread_mutex_lock(&brn2_mutex);
            work_pending -= 1;
            if (work_pending == 0 && (work_queue.count == 0)) {
                pthread_cond_signal(&brn2_done_work);
            }
            xpthread_mutex_unlock(&brn2_mutex);
        }
    }
}
#endif

void
brn2_list_from_dir(FileList *list, char *directory) {
    struct dirent **directory_list;
    uint32 length = 0;
    uint16 directory_length;
    int number_files;

    if (strcmp(directory, ".")) {
        directory_length = (uint16)strlen64(directory);
    } else {
        directory_length = 0;
    }

    number_files = scandir(directory, &directory_list, NULL, NULL);
    if (number_files < 0) {
        error("Error scanning '%s': %s.\n", directory, strerror(errno));
        fatal(EXIT_FAILURE);
    }

    list->files = xmalloc(number_files*SIZEOF(*(list->files)));

    for (int i = 0; i < number_files; i += 1) {
        FileName **file_pointer = &(list->files[length]);
        FileName *file;
        char *name = directory_list[i]->d_name;
        uint16 name_length = (uint16)strlen64(name);
        uint32 size;

        if (brn2_is_invalid_name(name)) {
            if (DEBUGGING) {
                free(directory_list[i]);
            }
            continue;
        }

        if (directory_length) {
            size = STRUCT_ARRAY_SIZE(*file_pointer, char,
                                     directory_length + 1 + name_length + 2);
            *file_pointer = xarena_push(list->arena, ALIGN(size));
            file = *file_pointer;

            file->length = directory_length + 1 + name_length;
            memcpy64(file->name, directory, directory_length);
            file->name[directory_length] = '/';
            memcpy64(file->name + directory_length + 1, name, name_length + 1);
        } else {
            size = STRUCT_ARRAY_SIZE(*file_pointer, char, name_length + 2);
            *file_pointer = xarena_push(list->arena, ALIGN(size));
            file = *file_pointer;

            file->length = name_length;
            memcpy64(file->name, name, file->length + 1);
        }

        if (DEBUGGING) {
            free(directory_list[i]);
        }
        length += 1;
    }
    free(directory_list);
    list->length = length;
    return;
}

void
brn2_free_list(FileList *list) {
    free(list->files);
    arena_reset(list->arena);
    return;
}

#if defined(__linux__)
void
brn2_list_from_file(FileList *list, char *filename, bool is_old) {
    char *map;
    uint32 length = 0;
    uint32 map_size;
    uint32 padding;
    int fd;

    if (!strcmp(filename, "-") || !strcmp(filename, "/dev/stdin")) {
        error("Reading from stdin...\n");
        brn2_list_from_lines(list, filename, is_old);
        return;
    }
    if ((fd = open(filename, O_RDWR)) < 0) {
        error("Error opening '%s' for reading: %s.\n", filename,
              strerror(errno));
        fatal(EXIT_FAILURE);
    }

    {
        struct stat lines_stat;
        if (fstat(fd, &lines_stat) < 0) {
            error("Error in fstat(%s): %s.\n", filename, strerror(errno));
            fatal(EXIT_FAILURE);
        }
        if (lseek(fd, 0, SEEK_CUR) < 0 && errno == ESPIPE) {
            error("Error getting file names: File is not seekable.\n");
            fatal(EXIT_FAILURE);
        }
        if (!S_ISREG(lines_stat.st_mode)) {
            error("Error getting file names: Not a regular file.\n");
            fatal(EXIT_FAILURE);
        }
        if (lines_stat.st_size <= 0) {
            error("Error getting file names: File size = %lld.\n",
                  (llong)lines_stat.st_size);
            fatal(EXIT_FAILURE);
        }
        if (sizeof(lines_stat.st_size) > sizeof(map_size)) {
            if (lines_stat.st_size >= (int64)UINT32_MAX) {
                error("Error: File size = %lld.\n", (llong)lines_stat.st_size);
                fatal(EXIT_FAILURE);
            }
        }
        map_size = (uint32)lines_stat.st_size;
    }
    padding = BRN2_ALIGNMENT - (map_size % BRN2_ALIGNMENT);
    map_size += padding;
    if (ftruncate(fd, map_size) < 0) {
        error("Error in ftruncate(%s, %zu): %s.\n", filename, strerror(errno),
              map_size);
        fatal(EXIT_FAILURE);
    }

    map = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        error("Error mapping history file to memory: %s.\n", strerror(errno));
        fatal(EXIT_FAILURE);
    }

    {
        uint32 capacity = map_size / 2;
        list->files = xmalloc(capacity*sizeof(*(list->files)));
    }

    {
        char *begin = map;
        char *pointer = map;
        int64 left = map_size - padding;

        while ((left > 0) && (pointer = memchr64(pointer, '\n', left))) {
            FileName **file_pointer = &(list->files[length]);
            FileName *file;
            uint32 size;
            uint16 name_length;

            *pointer = '\0';
            if (is_old && brn2_is_invalid_name(begin)) {
                begin = pointer + 1;
                continue;
            }
            if (begin == pointer) {
                error("Empty line in file. Exiting.\n");
                fatal(EXIT_FAILURE);
            }

            name_length = (uint16)(pointer - begin);
            size = STRUCT_ARRAY_SIZE(file_pointer, char, name_length + 2);
            *file_pointer = xarena_push(list->arena, ALIGN(size));

            file = *file_pointer;
            file->length = name_length;
            memcpy64(file->name, begin, name_length + 1);

            begin = pointer + 1;
            pointer += 1;
            length += 1;
            left -= (file->length + 1);
        }
    }

    if (length == 0) {
        error("Empty list. Exiting.\n");
        fatal(EXIT_FAILURE);
    }
    list->files = xrealloc(list->files, length*sizeof(*(list->files)));
    list->length = length;
    munmap(map, map_size);

    if (ftruncate(fd, map_size - padding) < 0) {
        error("Error in ftruncate(%s, %s): %s.\n", filename, strerror(errno),
              map_size - padding);
        fatal(EXIT_FAILURE);
    }
    if (close(fd) < 0) {
        error("Error closing '%s': %s.\n", filename, strerror(errno));
    }

    return;
}
#else
void
brn2_list_from_file(FileList *list, char *filename, bool is_old) {
    brn2_list_from_lines(list, filename, is_old);
    return;
}
#endif

void
brn2_list_from_lines(FileList *list, char *filename, bool is_old) {
    uint32 length = 0;
    char buffer[BRN2_PATH_MAX];
    uint32 capacity = 128;
    FILE *lines;

    if (!strcmp(filename, "-")) {
        lines = stdin;
    } else {
        if ((lines = fopen(filename, "r")) == NULL) {
            error("Error opening '%s': %s.\n", filename, strerror(errno));
            fatal(EXIT_FAILURE);
        }
    }

    list->files = xmalloc(capacity*sizeof(*(list->files)));

    errno = 0;
    while (fgets(buffer, sizeof(buffer), lines)) {
        FileName **file_pointer;
        FileName *file;
        uint16 name_length;
        uint32 size;

        name_length = (uint16)strcspn(buffer, "\n");
        buffer[name_length] = '\0';
        if (is_old && brn2_is_invalid_name(buffer)) {
            continue;
        }

        if (length >= capacity) {
            capacity *= 2;
            list->files
                = xrealloc(list->files, capacity*sizeof(*(list->files)));
        }

        size = STRUCT_ARRAY_SIZE(*file_pointer, char, name_length + 2);

        file_pointer = &(list->files[length]);
        *file_pointer = xarena_push(list->arena, ALIGN(size));
        file = *file_pointer;

        file->length = name_length;
        memcpy64(file->name, buffer, file->length + 1);

        length += 1;
        errno = 0;
    }
    if (errno) {
        error("Error reading from %s: %s.\n", filename, strerror(errno));
        fatal(EXIT_FAILURE);
    }
    if (length == 0) {
        error("No files to rename.\n");
        fatal(EXIT_FAILURE);
    }
    if (fclose(lines)) {
        error("Error closing file %s: %s.\n", strerror(errno));
    }
    list->files = xrealloc(list->files, length*sizeof(*(list->files)));
    list->length = length;
    return;
}

bool
brn2_is_invalid_name(char *filename) {
    while (*filename) {
        if ((*filename != '.') && (*filename != '/')) {
            return false;
        }
        filename += 1;
    }
    return true;
}

void *
brn2_threads_work_normalization(void *arg) {
    Work *work = arg;
    FileList *list;
    bool old_list;

    if (work->new_list) {
        list = work->new_list;
        old_list = false;
    } else {
        list = work->old_list;
        old_list = true;
    }

    for (uint32 i = work->start; i < work->end; i += 1) {
        FileName *file = list->files[i];
        char *p;
        uint32 off = 0;

        while ((p = memmem64(file->name + off, file->length - off, "//", 2))) {
            off = (uint32)(p - file->name);

            memmove64(&p[0], &p[1], file->length - off);
            file->length -= 1;
        }

        while (file->name[0] == '.' && file->name[1] == '/') {
            memmove64(&file->name[0], &file->name[2], file->length - 1);
            file->length -= 2;
        }

        off = 0;
        while ((p = memmem64(file->name + off, file->length - off, "/./", 3))) {
            off = (uint32)(p - file->name);

            memmove64(&p[1], &p[3], file->length - off - 2);
            file->length -= 2;
        }

        if (old_list) {
            struct stat file_stat;
            if (lstat(file->name, &file_stat) < 0) {
                if (errno != ENOENT) {
                    error("Error in stat('%s'): %s.\n", file->name,
                          strerror(errno));
                }
                work->old_list->files[i]->type = TYPE_ERR;
                continue;
            }
            if (S_ISDIR(file_stat.st_mode)) {
                work->old_list->files[i]->type = TYPE_DIR;
                brn2_slash_add(file);
            } else {
                work->old_list->files[i]->type = TYPE_FILE;
            }
        } else {
            if (work->old_list->files[i]->type == TYPE_DIR) {
                brn2_slash_add(file);
            }
        }
    }
    return 0;
}

void
brn2_slash_add(FileName *file) {
    if (file->name[file->length - 1] != '/') {
        file->name[file->length] = '/';
        file->name[file->length + 1] = '\0';
        file->length += 1;
    }
    return;
}

void *
brn2_threads_work_sort(void *arg) {
    Work *work = arg;
    FileName **files = &(work->old_list->files[work->start]);
    qsort(files, work->end - work->start, sizeof(*files), brn2_compare);
    return 0;
}

void *
brn2_threads_work_hashes(void *arg) {
    Work *work = arg;

    for (uint32 i = work->start; i < work->end; i += 1) {
        FileList *list = work->old_list;
        FileName *newfile = list->files[i];
        newfile->hash = hash_function(newfile->name, newfile->length);
        list->indexes[i] = newfile->hash % work->map_capacity;
    }
    return 0;
}

void *
brn2_threads_work_changes(void *arg) {
    Work *work = arg;

    for (uint32 i = work->start; i < work->end; i += 1) {
        FileName *oldfile = work->old_list->files[i];
        FileName *newfile = work->new_list->files[i];
        if (oldfile->length == newfile->length) {
            if (!memcmp64(oldfile->name, newfile->name, oldfile->length)) {
                continue;
            }
        }
        *(work->partial) += 1;
    }
    return 0;
}

void
brn2_timings(char *name, struct timespec t0, struct timespec t1,
             uint32 length) {
    long seconds = t1.tv_sec - t0.tv_sec;
    long nanos = t1.tv_nsec - t0.tv_nsec;

    double total_seconds = (double)seconds + (double)nanos / 1.0e9;
    double micros_per_item = 1e6*(total_seconds / (double)(length));

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
    brn2_threads(brn2_threads_work_hashes, list, NULL, NULL, map_capacity);
    return;
}

uint32
brn2_get_number_changes(FileList *old, FileList *new) {
    uint32 total = 0;
    uint32 numbers[BRN2_MAX_THREADS] = {0};
    brn2_threads(brn2_threads_work_changes, old, new, numbers, 0);

    for (uint32 i = 0; i < BRN2_MAX_THREADS; i += 1) {
        total += numbers[i];
    }
    return total;
}

#if OS_UNIX && (BRN2_MAX_THREADS > 1)
uint32
brn2_threads(void *(*function)(void *), FileList *old, FileList *new,
             uint32 *numbers, uint32 map_size) {
    static Work slices[BRN2_MAX_THREADS];
    uint32 range;
    uint32 length;

    if (old) {
        length = old->length;
    } else {
        length = new->length;
    }

    range = length / nthreads;

    for (uint32 i = 0; i < (nthreads - 1); i += 1) {
        slices[i].start = i*range;
        slices[i].end = (i + 1)*range;
        slices[i].old_list = old;
        slices[i].new_list = new;
        slices[i].partial = numbers ? &numbers[i] : NULL;
        slices[i].map_capacity = map_size;
        slices[i].function = function;

        brn2_enqueue(&slices[i]);
    }
    {
        uint32 i = nthreads - 1;
        slices[i].start = i*range;
        slices[i].end = length;
        slices[i].old_list = old;
        slices[i].new_list = new;
        slices[i].partial = numbers ? &numbers[i] : NULL;
        slices[i].map_capacity = map_size;
        slices[i].function = function;

        brn2_enqueue(&slices[i]);
    }

    xpthread_mutex_lock(&brn2_mutex);
    while (work_pending > 0 || (work_queue.count != 0)) {
        pthread_cond_wait(&brn2_done_work, &brn2_mutex);
    }
    pthread_cond_signal(&brn2_new_work);
    xpthread_mutex_unlock(&brn2_mutex);

    return nthreads;
}
#else
uint32
brn2_threads(void *(*function)(void *), FileList *old, FileList *new,
             uint32 *numbers, uint32 map_size) {
    Work slices[1];
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
    slices[0].function = function;
    function((void *)&slices[0]);

    return 1;
}
#endif

bool
brn2_verify(FileList *new, FileList *old, HashSet *repeated_set,
            uint32 *hashes_new) {
    bool failed = false;

    for (uint32 i = 0; i < new->length; i += 1) {
        FileName *newfile = new->files[i];

        if (newfile->length >= BRN2_PATH_MAX) {
            error("Error: filename on line %u is longer than %u bytes", i + 1,
                  BRN2_PATH_MAX);
            failed = true;
            if (brn2_options_fatal) {
                fatal(EXIT_FAILURE);
            }
        }

        if (!hash_insert_pre_calc_set(repeated_set, newfile->name,
                                      newfile->hash, hashes_new[i])) {
            FileName *oldfile = old->files[i];
            char *diff[] = {
                "diff", "-q", newfile->name, oldfile->name, NULL,
            };
            error("Error: " RED "'%s'" RESET " repeats on line %u. ",
                  newfile->name, i + 1);

            if (util_command(LENGTH(diff), diff) == 0) {
                error("Old (%s) and new name (%s)"
                      " have exactly the same content.\n",
                      oldfile->name, newfile->name);
                if (brn2_options_autosolve) {
                    error("--autosolve is enabled: Deleting old file...\n");
                    if (unlink(newfile->name) < 0) {
                        error("Error deleting %s: %s.\n", newfile->name,
                              strerror(errno));
                        fatal(EXIT_FAILURE);
                    }
                    continue;
                }
            } else {
                error("\n");
            }

            failed = true;
            if (brn2_options_fatal) {
                fatal(EXIT_FAILURE);
            }
        }
    }

    return !failed;
}

static inline int
noop(const char *unused, ...) {
    (void)unused;
    return 0;
}

void
brn2_execute2(FileList *old, FileList *new, HashMap *oldlist_map,
              HashSet *names_renamed, uint32 i, uint32 *number_renames) {
    int renamed;
    uint32 *newname_index_on_oldlist;
    bool newname_exists;

    FileName **oldfile = &(old->files[i]);

    char *oldname = old->files[i]->name;
    char *newname = new->files[i]->name;

    uint32 newhash = new->files[i]->hash;
    uint32 newindex = new->indexes[i];

    uint32 oldhash = old->files[i]->hash;
    uint32 oldindex = old->indexes[i];

    if (newhash == oldhash) {
        if (!strcmp(oldname, newname)) {
            return;
        }
    }
    newname_index_on_oldlist
        = hash_lookup_pre_calc_map(oldlist_map, newname, newhash, newindex);
    newname_exists = !access(newname, F_OK);
#if defined(_GNU_SOURCE)
    if (newname_exists && !newname_index_on_oldlist && !brn2_options_implicit) {
        error("Error renaming " RED "'%s'" RESET " to " RED "'%s'" RESET ":\n",
              oldname, newname);
        error(RED "'%s'" RESET " already exists,"
                  " but it was not given in the list of"
                  " files to rename, and --implict option is off.\n",
              newname);
        if (brn2_options_fatal) {
            fatal(EXIT_FAILURE);
        }
        return;
    }
    if (newname_exists) {
        renamed
            = renameat2(AT_FDCWD, oldname, AT_FDCWD, newname, RENAME_EXCHANGE);
        if (renamed >= 0) {
            if (hash_insert_pre_calc_set(names_renamed, oldname, oldhash,
                                         oldindex)) {
                *number_renames += 1;
            }
            if (hash_insert_pre_calc_set(names_renamed, newname, newhash,
                                         newindex)) {
                *number_renames += 1;
            }
            print(GREEN "%s" RESET " <-> " GREEN "%s" RESET "\n", oldname,
                  newname);

            if (newname_index_on_oldlist) {
                uint32 next = *newname_index_on_oldlist;
                FileName **file_j = &(old->files[next]);

                hash_remove_pre_calc_map(oldlist_map, newname, newhash,
                                         newindex);
                hash_remove_pre_calc_map(oldlist_map, oldname, oldhash,
                                         oldindex);

                hash_insert_pre_calc_map(oldlist_map, newname, newhash,
                                         newindex, i);
                hash_insert_pre_calc_map(oldlist_map, oldname, oldhash,
                                         oldindex, next);

                SWAP(*file_j, *oldfile);
                SWAP(old->indexes[i], old->indexes[next]);
            } else {
                error("Warning: '%s' was swapped with '%s', even though"
                      " '%s' was not in the list of files to rename.\n",
                      newname, oldname, newname);
                error("To disable this behaviour,"
                      " don't pass the --implict option.\n");
                hash_insert_pre_calc_map(oldlist_map, newname, newhash,
                                         newindex, i);
            }
            return;
        } else if (errno != ENOENT) {
            error("Error swapping " RED "'%s'" RESET " and " RED "'%s'" RESET
                  ": %s.\n",
                  oldname, newname, strerror(errno));
            if (brn2_options_fatal) {
                fatal(EXIT_FAILURE);
            }
        }
    }
#else
    (void)newname_index_on_oldlist;
    (void)oldfile;
    if (newname_exists) {
        error("Error renaming " RED "'%s'" RESET
              " to '%s': File already exists.\n",
              oldname, newname);
        if (brn2_options_fatal) {
            fatal(EXIT_FAILURE);
        }
        return;
    }
#endif
    renamed = rename(oldname, newname);
    if (renamed < 0) {
        error("Error renaming " RED "'%s'" RESET " to " RED "'%s'" RESET
              ": %s.\n",
              oldname, newname, strerror(errno));
        if (brn2_options_fatal) {
            fatal(EXIT_FAILURE);
        }
        return;
    } else {
        if (hash_insert_pre_calc_set(names_renamed, oldname, oldhash,
                                     oldindex)) {
            *number_renames += 1;
        }
        print("%s -> " GREEN "%s" RESET "\n", oldname, newname);
    }
    return;
}

void
brn2_usage(FILE *stream) {
    fprintf(
        stream,
        "usage: brn2 [OPTIONS] -- <file1> <file2> ...\n"
        "usage: brn2 [OPTIONS] -f <filename>\n"
        "usage: brn2 [OPTIONS] -d <dir>\n"
        "Rename filenames based on provided arguments.\n"
        "\n"
        "Options:\n"
        "  -F, --fatal     : Exit on first renaming error.\n"
        "  -a, --autosolve : Auto solve name conflicts for equal files.\n"
        "  -e, --explict   : Only rename files given in the list (default).\n"
        "  -h, --help      : Display this help message and exit.\n"
        "  -i, --implict   : Rename files not given in the list of files to "
        "rename.\n"
        "  -q, --quiet     : Quiet mode; suppress output messages.\n"
        "  -s, --sort      : Disable sorting of original list.\n"
        "  -v, --verbose   : Verbose mode (default); output messages.\n"
        "\n"
        "Arguments:\n"
        "  No arguments             : Rename files of current working "
        "directory.\n"
        "  1 or more arguments      : Rename filenames passed as arguments.\n"
        "  -d <dir>, --dir=<dir>    : Rename files in directory.\n"
        "  -f <file>, --file=<file> : Rename filenames listed in this "
        "argument.\n");
    exit((int)(stream != stdout));
}

#if TESTING_brn2
#include <assert.h>

bool brn2_options_fatal = false;
bool brn2_options_implicit = false;
bool brn2_options_quiet = false;
bool brn2_options_autosolve = false;
uint32 nthreads = 1;

static bool
contains_filename(FileList *list, FileName *file, bool verbose) {
    for (uint32 i = 0; i < list->length; i += 1) {
        if (list->files[i]->length != file->length) {
            continue;
        }
        if (!memcmp64(list->files[i]->name, file->name, file->length)) {
            printf(GREEN "%s == %s\n" RESET, file->name, list->files[i]->name);
            if (i < (list->length - 1)) {
                list->length -= 1;
                memmove64(&list->files[i], &list->files[i + 1],
                          (list->length - i)*sizeof(*(list->files)));
            }
            return true;
        }
        if (verbose) {
            printf("%u / %u | %s != %s \n", i + 1, list->length,
                   list->files[i]->name, file->name);
        }
    }
    return false;
}
// flags: -lm

int (*print)(const char *, ...);

int
main(void) {
    FileList list1_stack = {0};
    FileList list2_stack = {0};
    FileList *list1 = &list1_stack;
    FileList *list2 = &list2_stack;

    char *command = "ls -a > /tmp/brn2test";
    char *file = command + 8;

    list1->arena = arena_create(BRN2_ARENA_SIZE);
    list2->arena = arena_create(BRN2_ARENA_SIZE);

    system(command);
    brn2_list_from_dir(list1, ".");
    brn2_list_from_file(list2, file, true);

    for (uint32 i = 0; i < nthreads; i += 1) {
        pthread_create(&thread_pool[i], NULL, brn2_threads_function, NULL);
    }

    brn2_normalize_names(list1, NULL);
    brn2_normalize_names(list2, NULL);

    assert(list1->length == list2->length);

    for (uint32 i = 0; i < list1->length; i += 1) {
        printf(RED "%u / %u\n" RESET, i + 1, list1->length);
        assert(contains_filename(list2, list1->files[i], list1->length < 9999));
    }

    brn2_free_list(list1);
    brn2_free_list(list2);

    unlink(file);
    exit(0);
}
#endif

#endif
