/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023 Mior, Lucas; <lucasbuddemior@gmail.com>
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

#include "brn2.h"
#include "hash.h"
#include "util.h"

void brn2_normalize_names(FileList *list) {
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
                memmove(&name[j], &name[j + 1],
                        (*length - j) * sizeof (*name));
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

void brn2_copy_filename(FileName *file, char *name, uint32 length) {
    file->name = util_malloc(length + 1);
    memcpy(file->name, name, length + 1);
    file->length = length;
    return;
}

FileList *brn2_list_from_args(int argc, char **argv) {
    FileList *list;
    uint32 length = 0;

    list = util_malloc(STRUCT_ARRAY_SIZE(list, FileName, argc - 1));

    for (int i = 1; i < argc; i += 1) {
        char *name = argv[i];
        uint32 name_length;

        if (is_pwd_or_parent(name))
            continue;

        name_length = (uint32) strlen(name);
        brn2_copy_filename(&(list->files[length]), name, name_length);

        length += 1;
    }
    list->length = length;
    return list;
}

FileList *brn2_list_from_dir(char *directory) {
    FileList *list;
    struct dirent **directory_list;
    uint32 length = 0;

    int n = scandir(directory, &directory_list, NULL, versionsort);
    if (n < 0) {
        fprintf(stderr, "Error scanning \"%s\": %s\n",
                        directory, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (n <= 2) {
        fprintf(stderr, "Empty directory. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    list = util_malloc(STRUCT_ARRAY_SIZE(list, FileName, n - 2));

    for (int i = 0; i < n; i += 1) {
        char *name = directory_list[i]->d_name;
        uint32 name_length;

        if (is_pwd_or_parent(name)) {
            free(directory_list[i]);
            continue;
        }
        name_length = (uint32) strlen(name);
        brn2_copy_filename(&(list->files[length]), name, name_length);

        free(directory_list[i]);
        length += 1;
    }
    list->length = length;
    free(directory_list);
    return list;
}

FileList *brn2_list_from_lines(char *filename, uint32 capacity) {
    FileList *list;
    FILE *lines;
    uint32 length = 0;

    if ((lines = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Error opening \"%s\" for reading: %s\n",
                        filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (capacity == 0)
        capacity = 128;

    list = util_malloc(STRUCT_ARRAY_SIZE(list, FileName, capacity));
    while (!feof(lines)) {
        char buffer[PATH_MAX];
        uint32 last;

        if (length >= capacity) {
            capacity *= 2;
            list = util_realloc(list,
                                STRUCT_ARRAY_SIZE(list, FileName, capacity));
        }

        if (!fgets(buffer, sizeof (buffer), lines))
            continue;

        last = (uint32) strcspn(buffer, "\n");
        if (last == 0)
            continue;
        buffer[last] = '\0';

        if (is_pwd_or_parent(buffer))
            continue;
        brn2_copy_filename(&(list->files[length]), buffer, last);

        length += 1;
    }
    fclose(lines);

    if (length == 0) {
        fprintf(stderr, "Empty list. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    list = util_realloc(list, STRUCT_ARRAY_SIZE(list, FileName, length));
    list->length = length;

    return list;
}

bool is_pwd_or_parent(char *filename) {
    if (filename[0] == '.') {
        if ((filename[1] == '.') || (filename[1] == '\0'))
            return true;
        else if ((filename[1] == '/') && (filename[2] == '\0'))
            return true;
    }
    return false;
}

typedef struct Slice {
    FileList *list;
    uint32 *hashes;
    uint32 *indexes;
    uint32 start;
    uint32 end;
    uint32 map_capacity;
    uint32 unused;
} Slice;

static int brn2_create_hashes(void *arg) {
    Slice *slice = arg;

    for (uint32 i = slice->start; i < slice->end; i += 1) {
        FileName newfile = slice->list->files[i];
        slice->hashes[i] = hash_function(newfile.name);
        slice->indexes[i] = slice->hashes[i] % slice->map_capacity;
    }
    thrd_exit(0);
}

bool brn2_check_repeated(FileList *list) {
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

        for (uint32 i = 0; i < nthreads; i += 1) {
            slices[i].start = i*range;
            if (i == nthreads - 1)
                slices[i].end = list->length;
            else
                slices[i].end = (i + 1)*range;
            slices[i].list = list;
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
                fprintf(stderr, RED"\"%s\""RESET
                                " appears more than once in the buffer\n",
                                newfile.name);
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
                fprintf(stderr, RED"\"%s\""RESET
                                " appears more than once in the buffer\n",
                                newfile.name);
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
                    fprintf(stderr, RED"\"%s\""RESET
                                    " appears more than once in the buffer\n",
                                    file_i.name);
                    repeated = true;
                }
            }
        }
    }
    return repeated;
}

bool brn2_verify(FileList *old, FileList *new) {
    bool repeated = false;

    if (old->length != new->length) {
        fprintf(stderr, "You are renaming "RED"%u"RESET" file%.*s "
                        "but buffer contains "RED"%u"RESET" file name%.*s\n",
                        old->length, old->length != 1, "s",
                        new->length, new->length != 1, "s");
        return false;
    }

    repeated = brn2_check_repeated(new);
    return !repeated;
}

uint32 brn2_get_number_changes(FileList *old, FileList *new) {
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

static inline int noop(const char *restrict unused, ...) {
    (void) unused;
    return 0;
}

uint32 brn2_execute(FileList *old, FileList *new,
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
                SWAP(char *, file_j->name, *oldname);
                SWAP(uint32, file_j->length, *oldlength);
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
                        SWAP(char *, file_j->name, *oldname);
                        SWAP(uint32, file_j->length, *oldlength);
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
            fprintf(stderr, "Can't rename \"%s\" to \"%s\": "
                            "File already exists.\n", *oldname, *newname);
            continue;
        }
#endif
        renamed = rename(*oldname, *newname);
        if (renamed < 0) {
            fprintf(stderr, "Error renaming "RED"\"%s\""RESET
                             "to "RED"\"%s\""RESET":\n", *oldname, *newname);
            fprintf(stderr, "%s\n", strerror(errno));
            continue;
        } else {
            if (hash_set_insert(names_renamed, *oldname))
                number_renames += 1;
            print("%s -> "GREEN"%s"RESET"\n", *oldname, *newname);
        }
    }
    hash_map_destroy(indexes_exchange);
    hash_map_destroy(names_renamed);
    return number_renames;
}


void brn2_free_list(FileList *list) {
    for (uint32 i = 0; i < list->length; i += 1)
        free(list->files[i].name);
    free(list);
    return;
}

void brn2_usage(FILE *stream) {
    fprintf(stream, "usage: brn2 [OPTIONS] [<filename> | <file1> <file2> ...]\n");
    fprintf(stream, "Rename filenames based on provided arguments.\n");
    fprintf(stream, "\n");
    fprintf(stream, "Options:\n");
    fprintf(stream, "  -h, --help    : Display this help message.\n");
    fprintf(stream, "  -q, --quiet   : Quiet mode; suppress output messages.\n");
    fprintf(stream, "  -v, --verbose : Verbose mode (default); output messages.\n");
    fprintf(stream, "\n");
    fprintf(stream, "Arguments:\n");
    fprintf(stream, "  No arguments        : Rename filenames in the current working directory.\n");
    fprintf(stream, "  Single argument     : Rename filenames listed in this argument.\n");
    fprintf(stream, "  2 or more arguments : Rename filenames passed as arguments.\n");
    fprintf(stream, "\n");
    exit((int)(stream != stdout));
}
