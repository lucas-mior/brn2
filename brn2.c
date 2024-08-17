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
#include <time.h>

#pragma push_macro("TESTING_THIS_FILE")
#define TESTING_THIS_FILE 0

#include "hash.c"
#include "util.c"

#pragma pop_macro("TESTING_THIS_FILE")

static int brn2_work_hashes(void *);
static int brn2_work_normalization(void *);
static int brn2_work_changes(void *);
static inline bool brn2_is_pwd_or_parent(char *);
static uint32 brn2_threads(int (*)(void *),
                           FileList *, FileList *,
                           Hash *, uint32 *, uint32);

int
brn2_compare(const void *a, const void *b) {
    const FileName *file_a = a;
    const FileName *file_b = b;
    return strcmp(file_a->name, file_b->name);
}

FileList *
brn2_list_from_args(int argc, char **argv) {
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

    return list;
}

FileList *
brn2_list_from_dir(char *directory) {
    FileList *list;
    struct dirent **directory_list;
    uint32 length = 0;
    int n;

    n = scandir(directory, &directory_list, NULL, NULL);
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
        file->name = util_malloc(file->length + 1);
        memcpy(file->name, name, file->length + 1);
        free(directory_list[i]);

        length += 1;
    }
    free(directory_list);
    list->length = length;
    return list;
}

void
brn2_free_dir_list(FileList *list) {
    for (uint32 i = 0; i < list->length; i += 1) {
        FileName *file = &(list->files[i]);
        free(file->name);
    }
    free(list);
    return;
}

void
brn2_free_lines_list(FileList *list) {
    munmap(list->map, list->map_size);
    free(list);
    return;
}

FileList *
brn2_list_from_lines(char *filename, uint32 capacity) {
    FileList *list;
    char *begin;
    char *pointer;
    size_t left;
    uint32 length = 0;
    bool is_old = capacity == 0;
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
        list->map_size += (MEMCHR_BYTES - (list->map_size % MEMCHR_BYTES));
    }

    list->map = mmap(NULL, list->map_size, 
                     PROT_READ | PROT_WRITE, MAP_PRIVATE,
                     fd, 0);

    if (list->map == MAP_FAILED) {
        error("Error mapping history file to memory: %s"
              "History will start empty.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (close(fd) < 0) {
        error("Error closing %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    begin = pointer = list->map;
    left = list->map_size;

    while ((left > 0) && (pointer = memchr(pointer, '\n', left))) {
        FileName *file;
        if (length >= capacity) {
            capacity *= 2;
            list = util_realloc(list,
                                STRUCT_ARRAY_SIZE(list, FileName, capacity));
        }

        file = &(list->files[length]);
        *pointer = '\0';
        if (is_old && brn2_is_pwd_or_parent(begin)) {
            begin = pointer + 1;
            continue;
        }
        if (begin == pointer) {
            error("Empty line in file. Exiting.\n");
            exit(EXIT_FAILURE);
        }

        file->name = begin;
        file->length = (uint32) (pointer - begin);
        begin = pointer + 1;
        length += 1;
        left -= (file->length + 1);
    }

    if (length == 0) {
        error("Empty list. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    list = util_realloc(list, STRUCT_ARRAY_SIZE(list, FileName, length));
    list->length = length;

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

typedef struct Slice {
    FileList *old_list;
    FileList *new_list;
    Hash *hashes;
    uint32 start;
    uint32 end;
    uint32 map_capacity;
    uint32 unused;
    uint32 *partial;
} Slice;

int
brn2_work_normalization(void *arg) {
    Slice *slice = arg;
    FileList *list = slice->old_list;

    for (uint32 i = slice->start; i < slice->end; i += 1) {
        FileName *file = &(list->files[i]);
        uint32 j = 0;

        while (file->name[file->length - 1] == '/') {
            file->name[file->length - 1] = '\0';
            file->length -= 1;
        }

        while (file->name[j] != '\0') {
            while (file->name[j] == '/' && file->name[j + 1] == '/') {
                file->length -= 1;
                memmove(&(file->name[j]), &(file->name[j + 1]),
                        (file->length - j)*sizeof(*(file->name)));
            }
            j += 1;
        }

        while (file->name[0] == '.' && file->name[1] == '/') {
            file->name += 2;
            file->length -= 2;
        }
    }
    thrd_exit(0);
}

int
brn2_work_hashes(void *arg) {
    Slice *slice = arg;

    for (uint32 i = slice->start; i < slice->end; i += 1) {
        FileList *list = slice->old_list;
        FileName *newfile = &(list->files[i]);
        newfile->hash = hash_function(newfile->name);
        slice->hashes[i].mod = newfile->hash % slice->map_capacity;
    }
    thrd_exit(0);
}

int brn2_work_changes(void *arg) {
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
    thrd_exit(0);
}

void
brn2_normalize_names(FileList *list) {
    brn2_threads(brn2_work_normalization, list, NULL, NULL, NULL, 0);
    return;
}

Hash *
brn2_create_hashes(FileList *list, uint32 map_capacity) {
    Hash *hashes = util_malloc(list->length*sizeof(*hashes));
    brn2_threads(brn2_work_hashes, list, NULL, hashes, NULL, map_capacity);
    return hashes;
}

uint32
brn2_get_number_changes(FileList *old, FileList *new) {
    uint32 total = 0;
    uint32 numbers[MAX_THREADS] = {0};
    brn2_threads(brn2_work_changes, old, new, NULL, numbers, 0);

    for (uint32 i = 0; i < MAX_THREADS; i += 1)
        total += numbers[i];
    return total;
}

uint32 brn2_threads(int (*function)(void *),
                    FileList *old, FileList *new,
                    Hash *hashes, uint32 *numbers,
                    uint32 map_size) {
    thrd_t threads[MAX_THREADS];
    Slice slices[MAX_THREADS];
    uint32 range;

    if (nthreads >= old->length)
        nthreads = 1;

    range = old->length / nthreads;

    for (uint32 i = 0; i < (nthreads - 1); i += 1) {
        slices[i].start = i*range;
        slices[i].end = (i + 1)*range;
        slices[i].old_list = old;
        slices[i].new_list = new;
        slices[i].hashes = hashes;
        slices[i].partial = &numbers[i];
        slices[i].map_capacity = map_size;
        thrd_create(&threads[i], function, (void *) &slices[i]);
    }{
        uint32 i = nthreads - 1;
        slices[i].start = i*range;
        slices[i].end = old->length;
        slices[i].old_list = old;
        slices[i].new_list = new;
        slices[i].hashes = hashes;
        slices[i].partial = &numbers[i];
        slices[i].map_capacity = map_size;
        thrd_create(&threads[i], function, (void *) &slices[i]);
    }

    for (uint32 i = 0; i < nthreads; i += 1)
        thrd_join(threads[i], NULL);
    return nthreads;
}

bool
brn2_verify(FileList *old, FileList *new,
            HashMap *repeated_map, Hash *hashes_new) {
    bool repeated = false;
    char *repeated_format = RED"\"%s\""RESET " (line %d)"
                            " appears more than once in the buffer\n";

    if (old->length != new->length) {
        error("You are renaming "RED"%u"RESET" file%.*s "
              "but buffer contains "RED"%u"RESET" file name%.*s\n",
              old->length, old->length != 1, "s",
              new->length, new->length != 1, "s");
        return false;
    }

    for (uint32 i = 0; i < new->length; i += 1) {
        FileName newfile = new->files[i];

        if (!hash_map_insert_pre_calc(repeated_map, newfile.name,
                                      newfile.hash, hashes_new[i].mod, i)) {
            fprintf(stderr, repeated_format, newfile.name, i + 1);
            repeated = true;
            if (brn2_fatal || BRN2_DEBUG)
                exit(EXIT_FAILURE);
        }
    }

    return !repeated;
}

static inline int
noop(const char *unused, ...) {
    (void) unused;
    return 0;
}

uint32
brn2_execute(FileList *old, FileList *new,
             HashMap *oldlist_map,
             Hash *hashes_old, Hash *hashes_new, bool quiet) {
    uint32 number_renames = 0;
    uint32 length = old->length;
    int (*print)(const char *, ...);
    HashSet *names_renamed = hash_set_create(old->length);

    if (quiet)
        print = noop;
    else
        print = printf;

    for (uint32 i = 0; i < length; i += 1) {
        int renamed;
        char **oldname = &(old->files[i].name);
        char *newname = new->files[i].name;

        uint32 *oldlength = &(old->files[i].length);

        uint32 newhash = new->files[i].hash;
        uint32 newindex = hashes_new[i].mod;

        uint32 oldhash = old->files[i].hash;
        uint32 oldindex = hashes_old[i].mod;

        if (newhash == oldhash) {
            if (!strcmp(*oldname, newname))
                continue;
        }

#ifdef __linux__
        renamed = renameat2(AT_FDCWD, *oldname,
                            AT_FDCWD, newname, RENAME_EXCHANGE);
        if (renamed >= 0) {
            uint32 *index;

            if (hash_set_insert_pre_calc(names_renamed, *oldname,
                                         oldhash, oldindex))
                number_renames += 1;
            if (hash_set_insert_pre_calc(names_renamed, newname,
                                         newhash, newindex))
                number_renames += 1;
            print(GREEN"  %s"RESET" <-> "GREEN"%s"RESET"\n", *oldname, newname);

            index = hash_map_lookup_pre_calc(oldlist_map, newname,
                                             newhash, newindex);
            if (index) {
                uint32 next = *index;
                FileName *file_j = &(old->files[next]);

                hash_map_remove_pre_calc(oldlist_map, newname,
                                         newhash, newindex);
                hash_map_remove_pre_calc(oldlist_map, *oldname,
                                         oldhash, oldindex);

                hash_map_insert_pre_calc(oldlist_map, newname,
                                         newhash, newindex, i);
                hash_map_insert_pre_calc(oldlist_map, *oldname,
                                         oldhash, oldindex, next);

                SWAP(file_j->name, *oldname);
                SWAP(file_j->length, *oldlength);
                SWAP(file_j->hash, old->files[i].hash);
                SWAP(hashes_old[i], hashes_old[next]);
            } else {
                error("Warning: \"%s\" was swapped with \"%s\", even though"
                      " \"%s\" was not in the list of files to rename.\n",
                      newname, *oldname, newname);
                hash_map_insert_pre_calc(oldlist_map, newname,
                                         newhash, newindex, i);
            }
            continue;
        } else if (errno != ENOENT) {
            error("Error swapping "RED"\"%s\""RESET" and "RED"\"%s\""RESET": %s\n",
                  *oldname, newname, strerror(errno));
            if (brn2_fatal || BRN2_DEBUG)
                exit(EXIT_FAILURE);
        }
#else
        (void) newlength;
        if (!access(newname, F_OK)) {
            error("Can't rename \"%s\" to \"%s\": " "File already exists.\n",
                  *oldname, newname);
            if (brn2_fatal || BRN2_DEBUG)
                exit(EXIT_FAILURE);
            continue;
        }
#endif
        renamed = rename(*oldname, newname);
        if (renamed < 0) {
            error("Error renaming "RED"\"%s\""RESET " to "RED"\"%s\""RESET":\n",
                  *oldname, newname);
            error("%s\n", strerror(errno));
            if (brn2_fatal || BRN2_DEBUG)
                exit(EXIT_FAILURE);
            continue;
        } else {
            if (hash_set_insert_pre_calc(names_renamed, *oldname,
                                         oldhash, oldindex))
                number_renames += 1;
            print("  %s -> "GREEN"%s"RESET"\n", *oldname, newname);
        }
    }
    if (BRN2_DEBUG)
        hash_set_destroy(names_renamed);
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
            "  -F, --fatal   : Exit on first renaming error.\n"
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

bool brn2_fatal = false;
uint32 nthreads = 1;

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
            printf("%d / %d | %s != %s \n",
                   i+1, list->length, list->files[i].name, file.name);
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
    list1 = brn2_list_from_dir(".");
    list2 = brn2_list_from_lines(file, 0);

    assert(list1->length == list2->length);

    for (uint32 i = 0; i < list1->length; i += 1) {
        printf(RED"%u / %u\n"RESET, i+1, list1->length);
        assert(contains_filename(list2, list1->files[i], list1->length < 9999));
    }

    unlink(file);
    exit(0);
}
#endif

#endif
