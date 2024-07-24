/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023-2024 Mior, Lucas; <lucasbuddemior@gmail.com>
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
#include "util.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#pragma push_macro("TESTING_THIS_FILE")
#define TESTING_THIS_FILE 0

#include "hash.c"
#include "util.c"

#pragma pop_macro("TESTING_THIS_FILE")

static int brn2_create_hashes(void *arg);
static bool brn2_is_pwd_or_parent(char *);
static bool brn2_check_repeated(FileList *);

int
brn2_compare(const void *a, const void *b) {
    const FileName *file_a = a;
    const FileName *file_b = b;
    return strcmp(file_a->name, file_b->name);
}

FileList *
brn2_list_from_args(int argc, char **argv, bool sort_list) {
    FileList *list;
    uint32 length = 0;

    list = util_malloc(STRUCT_ARRAY_SIZE(list, FileName, argc));

    for (int i = 0; i < argc; i += 1) {
        char *name = argv[i];
        FileName *file = &(list->files[length]);

        if (brn2_is_pwd_or_parent(name))
            continue;

        file->length = (uint32) strlen(name);
        file->name = name;

        length += 1;
    }
    list->length = length;

    if (sort_list)
        qsort(list->files, list->length, sizeof (*(list->files)), brn2_compare);
    return list;
}

FileList *
brn2_list_from_dir(char *directory, bool sort_list) {
    FileList *list;
    struct dirent **directory_list;
    uint32 length = 0;
    int (*sort)(const struct dirent **, const struct dirent **);

    if (sort_list)
        sort = versionsort;
    else
        sort = NULL;

    int n = scandir(directory, &directory_list, NULL, sort);
    if (n < 0) {
        error("Error scanning \"%s\": %s\n", directory, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (n <= 2) {
        error("Empty directory. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    list = util_malloc(STRUCT_ARRAY_SIZE(list, FileName, n - 2));

    for (int i = 0; i < n; i += 1) {
        char *name = directory_list[i]->d_name;
        FileName *file = &(list->files[length]);

        if (brn2_is_pwd_or_parent(name)) {
            free(directory_list[i]);
            continue;
        }
        file->length = (uint32) strlen(name);
        file->name = name;

        length += 1;
    }
    list->length = length;
    return list;
}

void
brn2_free_lines_list(FileList *list) {
    munmap(list->map, list->map_size);
    free(list);
    return;
}

FileList *
brn2_list_from_lines(char *filename, uint32 capacity, bool sort_list) {
    FileList *list;
    char *begin;
    uint32 length = 0;
    int fd;

    if (capacity == 0)
        capacity = 128;
    list = util_malloc(STRUCT_ARRAY_SIZE(list, FileName, capacity));

    if ((fd = open(filename, O_RDWR)) < 0) {
        error("Error opening file for reading: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    {
        struct stat lines_stat;
        if (fstat(fd, &lines_stat) < 0) {
            error("Error getting file information: %s\n"
                  "History will start empty.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        list->map_size = (uint32) lines_stat.st_size;
        if (list->map_size <= 0) {
            error("list->map_size: %zu\n", list->map_size);
            exit(EXIT_FAILURE);
        }
    }

    list->map = mmap(NULL, list->map_size, 
                     PROT_READ | PROT_WRITE, MAP_PRIVATE,
                     fd, 0);

    if (list->map == MAP_FAILED) {
        error("Error mapping history file to memory: %s"
              "History will start empty.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    begin = list->map;
    for (char *p = list->map; p < list->map + list->map_size; p += 1) {
        if (length >= capacity) {
            capacity *= 2;
            list = util_realloc(list,
                                STRUCT_ARRAY_SIZE(list, FileName, capacity));
        }
        if (*p == '\n') {
            *p = '\0';
            if (brn2_is_pwd_or_parent(begin)) {
                begin = p + 1;
                continue;
            }

            list->files[length].name = begin;
            list->files[length].length = (uint32) (p - begin);
            begin = p + 1;
            length += 1;
        }
    }
    close(fd);

    if (length == 0) {
        error("Empty list. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    list = util_realloc(list, STRUCT_ARRAY_SIZE(list, FileName, length));
    list->length = length;
    list->map_size = list->map_size;
    if (sort_list)
        qsort(list->files, list->length, sizeof (*(list->files)), brn2_compare);

    return list;
}

bool
brn2_is_pwd_or_parent(char *filename) {
    if (filename[0] == '.') {
        if ((filename[1] == '.') || (filename[1] == '\0'))
            return true;
        else if ((filename[1] == '/') && (filename[2] == '\0'))
            return true;
    }
    return false;
}

void
brn2_normalize_names(FileList *list) {
    for (uint32 i = 0; i < list->length; i += 1) {
        FileName *file = &(list->files[i]);
        char *name = file->name;
        uint32 *length = &(file->length);
        uint32 j = 0;

        while (name[*length - 1] == '/') {
            name[*length - 1] = '\0';
            *length -= 1;
        }

        while (name[j] != '\0') {
            while (name[j] == '/' && name[j + 1] == '/') {
                *length -= 1;
                memmove(&name[j], &name[j + 1], (*length - j) * sizeof (*name));
            }
            j += 1;
        }

        while (name[0] == '.' && name[1] == '/') {
            memmove(&(name[0]), &(name[2]), (*length - 1) * sizeof (*name));
            *length -= 2;
        }
    }
    return;
}

void
brn2_copy_filename(FileName *file, char *name, uint32 length) {
    file->name = util_malloc(length + 1);
    memcpy(file->name, name, length + 1);
    file->length = length;
    return;
}

typedef struct Slice {
    FileName *files;
    uint32 *hashes;
    uint32 *indexes;
    uint32 start;
    uint32 end;
    uint32 map_capacity;
    uint32 unused;
} Slice;

int
brn2_create_hashes(void *arg) {
    Slice *slice = arg;

    for (uint32 i = slice->start; i < slice->end; i += 1) {
        FileName newfile = slice->files[i];
        slice->hashes[i] = hash_function(newfile.name);
        slice->indexes[i] = slice->hashes[i] % slice->map_capacity;
    }
    thrd_exit(0);
}

bool
brn2_check_repeated(FileList *list) {
    char *repeated_format = RED"\"%s\""RESET
                            " appears more than once in the buffer\n";
    bool repeated = false;
    long number_threads = sysconf(_SC_NPROCESSORS_ONLN);
    if ((list->length >= USE_THREADS_THRESHOLD) && (number_threads >= 2)) {
        uint32 nthreads = (uint32) number_threads;

        uint32 *hashes = util_malloc(list->length * sizeof (*hashes));
        uint32 *indexes = util_malloc(list->length * sizeof (*indexes));
        thrd_t *threads = util_malloc(nthreads * sizeof (*threads));
        Slice *slices = util_malloc(nthreads * sizeof (*slices));

        HashMap *repeated_map = hash_map_create(list->length);
        uint32 range = list->length / nthreads;

        for (uint32 i = 0; i < (nthreads - 1); i += 1) {
            slices[i].start = i*range;
            slices[i].end = (i + 1)*range;
            slices[i].files = list->files;
            slices[i].hashes = hashes;
            slices[i].indexes = indexes;
            slices[i].map_capacity = hash_map_capacity(repeated_map);
            thrd_create(&threads[i], brn2_create_hashes, (void *) &slices[i]);
        }
        {
            uint32 i = nthreads - 1;
            slices[i].start = i*range;
            slices[i].end = list->length;
            slices[i].files = list->files;
            slices[i].hashes = hashes;
            slices[i].indexes = indexes;
            slices[i].map_capacity = hash_map_capacity(repeated_map);
            thrd_create(&threads[i], brn2_create_hashes, (void *) &slices[i]);
        }

        for (uint32 i = 0; i < nthreads; i += 1)
            thrd_join(threads[i], NULL);

        for (uint32 i = 0; i < list->length; i += 1) {
            FileName newfile = list->files[i];

            if (!hash_set_insert_pre_calc(repeated_map, newfile.name,
                                          hashes[i], indexes[i])) {
                fprintf(stderr, repeated_format, newfile.name);
                repeated = true;
            }
        }

        free(hashes);
        free(indexes);
        free(slices);
        free(threads);
        hash_map_destroy(repeated_map);
    } else if (list->length > USE_HASH_MAP_THRESHOLD) {
        HashMap *repeated_map = hash_map_create(list->length);

        for (uint32 i = 0; i < list->length; i += 1) {
            FileName newfile = list->files[i];

            if (!hash_set_insert(repeated_map, newfile.name)) {
                fprintf(stderr, repeated_format, newfile.name);
                repeated = true;
            }
        }
        hash_map_destroy(repeated_map);
    } else {
        for (uint32 i = 0; i < list->length; i += 1) {
            FileName file_i = list->files[i];
            for (uint32 j = i + 1; j < list->length; j += 1) {
                FileName file_j = list->files[j];

                if (file_i.length != file_j.length)
                    continue;
                if (!memcmp(file_i.name, file_j.name, file_i.length)) {
                    fprintf(stderr, repeated_format, file_i.name);
                    repeated = true;
                }
            }
        }
    }
    return repeated;
}

bool
brn2_verify(FileList *old, FileList *new) {
    bool repeated = false;

    if (old->length != new->length) {
        error("You are renaming "RED"%u"RESET" file%.*s "
              "but buffer contains "RED"%u"RESET" file name%.*s\n",
              old->length, old->length != 1, "s",
              new->length, new->length != 1, "s");
        return false;
    }

    repeated = brn2_check_repeated(new);
    return !repeated;
}

uint32
brn2_get_number_changes(FileList *old, FileList *new) {
    uint32 number = 0;

    for (uint32 i = 0; i < old->length; i += 1) {
        FileName oldfile = old->files[i];
        FileName newfile = new->files[i];
        if (oldfile.length != newfile.length)
            number += 1;
        else if (memcmp(oldfile.name, newfile.name, oldfile.length))
            number += 1;
    }
    return number;
}

static inline int
noop(const char *restrict unused, ...) {
    (void) unused;
    return 0;
}

uint32
brn2_execute(FileList *old, FileList *new,
             const uint32 number_changes, bool quiet) {
    uint32 number_renames = 0;
    uint32 length = old->length;
    int (*print)(const char *restrict, ...);
    HashMap *names_renamed = hash_map_create(number_changes);
    HashMap *indexes_exchange = hash_map_create(number_changes);

    if (quiet)
        print = noop;
    else
        print = printf;

    for (uint32 i = 0; i < length; i += 1) {
        int renamed;
        char **oldname = &(old->files[i].name);
        char **newname = &(new->files[i].name);
        uint32 *oldlength = &(old->files[i].length);
        uint32 *newlength = &(new->files[i].length);
        uint32 newhash = hash_function(*newname);
        uint32 oldhash = hash_function(*oldname);
        uint32 newindex = newhash % hash_map_capacity(indexes_exchange);
        uint32 oldindex = oldhash % hash_map_capacity(indexes_exchange);

        if (!strcmp(*oldname, *newname))
            continue;

#ifdef __linux__
        renamed = renameat2(AT_FDCWD, *oldname,
                            AT_FDCWD, *newname, RENAME_EXCHANGE);
        if (renamed >= 0) {
            uint32 *index;

            if (hash_set_insert_pre_calc(names_renamed, *oldname,
                                         oldhash, oldindex))
                number_renames += 1;
            if (hash_set_insert_pre_calc(names_renamed, *newname,
                                         newhash, newindex))
                number_renames += 1;
            print(GREEN"%s"RESET" <-> "GREEN"%s"RESET"\n", *oldname, *newname);

            index = hash_map_lookup_pre_calc(indexes_exchange, *newname,
                                             newhash, newindex);
            if (index) {
                FileName *file_j = &(old->files[*index]);
                SWAP(file_j->name, *oldname);
                SWAP(file_j->length, *oldlength);
                hash_map_remove_pre_calc(indexes_exchange, *newname,
                                         newhash, newindex);
                hash_map_insert_pre_calc(indexes_exchange, *oldname,
                                         oldhash, oldindex, *index);
                continue;
            }
            for (uint32 j = i + 1; j < length; j += 1) {
                FileName *file_j = &(old->files[j]);
                if (file_j->length == *newlength) {
                    if (!memcmp(file_j->name, *newname, *newlength)) {
                        SWAP(file_j->name, *oldname);
                        SWAP(file_j->length, *oldlength);
                        hash_map_insert(indexes_exchange, *oldname, j);
                        break;
                    }
                }
                hash_map_insert(indexes_exchange, file_j->name, j);
            }
            continue;
        }
#else
        (void) newlength;
        if (!access(*newname, F_OK)) {
            error("Can't rename \"%s\" to \"%s\": " "File already exists.\n",
                  *oldname, *newname);
            continue;
        }
#endif
        renamed = rename(*oldname, *newname);
        if (renamed < 0) {
            error("Error renaming "RED"\"%s\""RESET "to "RED"\"%s\""RESET":\n",
                  *oldname, *newname);
            error("%s\n", strerror(errno));
            continue;
        } else {
            if (hash_set_insert(names_renamed, *oldname))
                number_renames += 1;
            print("%s -> "GREEN"%s"RESET"\n", *oldname, *newname);
        }
    }
#ifndef BRN2_DEBUG
#define BRN2_DEBUG 0
#endif
    if (BRN2_DEBUG) {
        hash_map_destroy(indexes_exchange);
        hash_map_destroy(names_renamed);
    }
    return number_renames;
}


void
brn2_usage(FILE *stream) {
    fprintf(stream,
            "usage: brn2 [OPTIONS] -- <file1> <file2> ...\n"
            "usage: brn2 [OPTIONS] -f <filename>\n"
            "Rename filenames based on provided arguments.\n"
            "\n"
            "Options:\n"
            "  -h, --help    : Display this help message and exit.\n"
            "  -q, --quiet   : Quiet mode; suppress output messages.\n"
            "  -v, --verbose : Verbose mode (default); output messages.\n"
            "  -c, --check   : Check if original file names exist.\n"
            "  -s, --sort    : Disable sorting of original list.\n"
            "\n"
            "Arguments:\n"
            "  No arguments"
            " : Rename filenames in the current working directory.\n"
            "  1 or more arguments"
            " : Rename filenames passed as arguments.\n"
            "  -f <file>, --file=<file>"
            " : Rename filenames listed in this argument.\n");
    exit((int)(stream != stdout));
}

#ifndef TESTING_THIS_FILE
#define TESTING_THIS_FILE 0
#endif

#if TESTING_THIS_FILE
#include <assert.h>

static bool
contains_filename(FileList *list, FileName file) {
    for (uint32 i = 0; i < list->length; i += 1) {
        if (!strcmp(list->files[i].name, file.name)) {
            if (i < (list->length - 1)) {
                list->length -= 1;
                memmove(&list->files[i], &list->files[i+1], (list->length - i) * sizeof (list->files[0]));
            }
            return true;
        }
    }
    return false;
}
// flags: -lm

int main(void) {
    FileList *list1;
    FileList *list2;
    char *command = "ls -a > /tmp/brn2test";
    char *file = command + 8;

    system(command);
    list1 = brn2_list_from_dir(".", true);
    list2 = brn2_list_from_lines(file, 0, true);

    assert(list1->length == list2->length);

    for (uint32 i = 0; i < list1->length; i += 1) {
        assert(contains_filename(list2, list1->files[i]));
    }

    unlink(file);
    exit(0);
}
#endif

#endif
