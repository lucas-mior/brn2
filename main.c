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
#include <malloc.h>

static FileList *main_file_list_from_dir(char *);
static FileList *main_file_list_from_lines(char *, uint32);
static FileList *main_file_list_from_args(int, char **);
static inline bool is_pwd_or_parent(char *filename);
static bool main_verify(FileList *, FileList *);
static uint32 main_get_number_changes(FileList *, FileList *);
static uint32 main_execute(FileList *, FileList *, const uint32);
static void main_free_file_list(FileList *);
static void main_usage(FILE *) __attribute__((noreturn));
static char *EDITOR;
static const char *tempdir = "/tmp";

int main(int argc, char **argv) {
    File buffer;
    FileList *old;
    FileList *new;
    bool status = true;

    if (argc >= 3) {
        old = main_file_list_from_args(argc, argv);
    } else if (argc == 2) {
        if (!strcmp(argv[1], "--help")) {
            main_usage(stdout);
        } else if (!strcmp(argv[1], "-h")) {
            main_usage(stdout);
        } else {
            old = main_file_list_from_lines(argv[1], 0);
        }
    } else {
        old = main_file_list_from_dir(".");
    }

    if (!(EDITOR = getenv("EDITOR"))) {
        EDITOR = "vim";
        fprintf(stderr, "EDITOR variable is not set. "
                        "Using %s by default.\n", EDITOR);
    }

    {
        char buffer2[BUFSIZ];

        snprintf(buffer.name, sizeof (buffer.name),
                "%s/%s", tempdir, "brn2.XXXXXX");

        if ((buffer.fd = mkstemp(buffer.name)) < 0) {
            fprintf(stderr, "Error opening %s: %s\n",
                            buffer.name, strerror(errno));
            exit(EXIT_FAILURE);
        }
        if ((buffer.stream = fdopen(buffer.fd, "w")) == NULL) {
            fprintf(stderr, "Error opening %s: %s\n",
                            buffer.name, strerror(errno));
            exit(EXIT_FAILURE);
        }

        setvbuf(buffer.stream, buffer2, _IOFBF, BUFSIZ);
        for (uint32 i = 0; i < old->length; i += 1) {
            uint32 length = old->files[i].length;
            old->files[i].name[length] = '\n';
            fwrite(old->files[i].name, 1, length+1, buffer.stream);
            old->files[i].name[length] = '\0';
        }
        fclose(buffer.stream);
        close(buffer.fd);
        buffer.fd = -1;
        buffer.stream = NULL;
    }

    {
        char *args[] = { EDITOR, buffer.name, NULL };

        while (true) {
            util_command(ARRAY_LENGTH(args), args);
            new = main_file_list_from_lines(buffer.name, old->length);
            if (!main_verify(old, new)) {
                main_free_file_list(new);
                printf("Fix your renames. Press control-c to cancel or press"
                       " ENTER to open the file list editor again.\n");
                getc(stdin);
                continue;
            } else {
                break;
            }
        }
    }

    {
        uint32 number_changes = main_get_number_changes(old, new);
        uint32 number_renames = 0;

        if (number_changes)
            number_renames = main_execute(old, new, number_changes);
        if (number_changes != number_renames) {
            fprintf(stderr, "%u name%.*s changed but %u file%.*s renamed. "
                            "Check your files.\n", 
                            number_changes, number_changes != 1, "s",
                            number_renames, number_renames != 1, "s");
            status = false;
        } else {
            fprintf(stdout, "%u file%.*s renamed\n",
                            number_renames, number_renames != 1, "s");
        }
    }

    main_free_file_list(old);
    main_free_file_list(new);
    unlink(buffer.name);
    exit(!status);
}

FileList *main_file_list_from_args(int argc, char **argv) {
    FileList *file_list;
    uint32 length = 0;

    file_list = util_malloc(STRUCT_ARRAY_SIZE(FileList, FileName, argc - 1));

    for (int i = 1; i < argc; i += 1) {
        char *name = argv[i];
        uint32 name_length;
        FileName *file;

        if (is_pwd_or_parent(name))
            continue;
        name_length = (uint32) strlen(name);

        file = &(file_list->files[length]);
        file->name = util_malloc(name_length+1);
        memcpy(file->name, name, name_length+1);
        file->length = name_length;

        length += 1;
    }
    file_list->length = length;
    return file_list;
}

FileList *main_file_list_from_dir(char *directory) {
    FileList *file_list;
    struct dirent **directory_list;
    uint32 length = 0;

    int n = scandir(directory, &directory_list, NULL, versionsort);
    if (n < 0) {
        fprintf(stderr, "Error scanning %s: %s\n", directory, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (n <= 2) {
        fprintf(stderr, "Empty directory. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    file_list = util_malloc(STRUCT_ARRAY_SIZE(FileList, FileName, n - 2));

    for (int i = 0; i < n; i += 1) {
        char *name = directory_list[i]->d_name;
        uint32 name_length;
        FileName *file;

        if (is_pwd_or_parent(name)) {
            free(directory_list[i]);
            continue;
        }
        name_length = (uint32) strlen(name);

        file = &(file_list->files[length]);
        file->name = util_malloc(name_length+1);
        memcpy(file->name, name, name_length+1);
        file->length = name_length;

        free(directory_list[i]);
        length += 1;
    }
    file_list->length = length;
    free(directory_list);
    return file_list;
}

FileList *main_file_list_from_lines(char *filename, uint32 capacity) {
    FileList *file_list;
    FILE *lines;
    uint32 length = 0;

    if ((lines = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Error opening %s for reading: %s\n",
                        filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (capacity == 0) {
        capacity = 128;
    }

    file_list = util_malloc(STRUCT_ARRAY_SIZE(FileList, FileName, capacity));
    while (!feof(lines)) {
        char buffer[PATH_MAX];
        uint32 last;
        FileName *file;

        if (length >= capacity) {
            capacity *= 2;
            file_list = util_realloc(file_list,
                        STRUCT_ARRAY_SIZE(FileList, FileName, capacity));
        }

        if (!fgets(buffer, sizeof (buffer), lines))
            continue;

        last = (uint32) strcspn(buffer, "\n");
        if (last == 0)
            continue;

        if (is_pwd_or_parent(buffer))
            continue;

        buffer[last] = '\0';

        file = &(file_list->files[length]);
        file->name = util_malloc(last + 1);
        memcpy(file->name, buffer, last + 1);
        file->length = last;
        length += 1;
    }
    fclose(lines);

    if (length == 0) {
        fprintf(stderr, "Empty filelist. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    file_list = util_realloc(file_list,
                             STRUCT_ARRAY_SIZE(FileList, FileName, capacity));
    file_list->length = length;

    return file_list;
}

bool is_pwd_or_parent(char *filename) {
    return filename[0] == '.' 
        && (filename[1] == '.' || filename[1] == '\0');
}

typedef struct Slice {
    FileList *filelist;
    uint32 start;
    uint32 end;
    uint32 table_capacity;
    uint32 *hashes;
    uint32 *indexes;
} Slice;

static int create_hashes(void *arg) {
    Slice *slice = arg;

    for (uint32 i = slice->start; i < slice->end; i += 1) {
        FileName newfile = slice->filelist->files[i];
        slice->hashes[i] = hash_function(newfile.name, newfile.length);
        slice->indexes[i] = slice->hashes[i] % slice->table_capacity;
    }
    thrd_exit(0);
}

bool main_verify(FileList *old, FileList *new) {
    bool repeated = false;
    long number_threads;

    if (old->length != new->length) {
        fprintf(stderr, "You are renaming "RED"%u"RESET" file%.*s "
                        "but buffer contains "RED"%u"RESET" file name%.*s\n", 
                        old->length, old->length != 1, "s",
                        new->length, new->length != 1, "s");
        return false;
    }

    number_threads = sysconf(_SC_NPROCESSORS_ONLN);
    if ((new->length >= USE_THREADS_THRESHOLD) && (number_threads >= 2)) {
        uint32 nthreads = (uint32) number_threads;

        uint32 *hashes = util_malloc(new->length * sizeof (*hashes));
        uint32 *indexes = util_malloc(new->length * sizeof (*indexes));
        thrd_t *threads = util_malloc(nthreads * sizeof (*threads));
        Slice *slices = util_malloc(nthreads * sizeof (*slices));

        HashTable *repeated_table = hash_table_create(new->length);
        uint32 range = new->length / nthreads;

        for (uint32 i = 0; i < nthreads; i += 1) {
            slices[i].start = i*range;
            if (i == nthreads - 1) {
                slices[i].end = new->length;
            } else {
                slices[i].end = (i + 1)*range;
            }
            slices[i].filelist = new;
            slices[i].hashes = hashes;
            slices[i].indexes = indexes;
            slices[i].table_capacity = hash_table_capacity(repeated_table);
            thrd_create(&threads[i], create_hashes, (void *) &slices[i]);
        }

        for (uint32 i = 0; i < nthreads; i += 1)
            thrd_join(threads[i], NULL);

        for (uint32 i = 0; i < new->length; i += 1) {
            FileName newfile = new->files[i];

            if (!hash_table_insert_pre_calc(repeated_table, newfile.name,
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
        hash_table_destroy(repeated_table);
    } else if (new->length > USE_HASH_TABLE_THRESHOLD) {
        HashTable *repeated_table = hash_table_create(new->length);

        for (uint32 i = 0; i < new->length; i += 1) {
            FileName newfile = new->files[i];

            if (!hash_table_insert(repeated_table, newfile.name, newfile.length)) {
                fprintf(stderr, RED"\"%s\""RESET
                                " appears more than once in the buffer\n",
                                newfile.name);
                repeated = true;
            }
        }
        printf("\nbefore removal:\n");
        hash_table_print(repeated_table, false);
        printf("\n");
        for (uint32 i = 0; i < new->length; i += 1) {
            hash_table_remove(repeated_table, new->files[i].name, new->files[i].length);
        }
        printf("\nafter removal:\n");
        hash_table_print(repeated_table, false);
        hash_table_destroy(repeated_table);
    } else {
        for (uint32 i = 0; i < new->length; i += 1) {
            FileName file_i = new->files[i];
            for (uint32 j = i+1; j < new->length; j += 1) {
                FileName file_j = new->files[j];

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

    return !repeated;
}

uint32 main_get_number_changes(FileList *old, FileList *new) {
    uint32 number = 0;

    for (uint32 i = 0; i < old->length; i += 1) {
        FileName oldfile = old->files[i];
        FileName newfile = new->files[i];
        if (oldfile.length != newfile.length) {
            number += 1;
        } else if (memcmp(oldfile.name, newfile.name, oldfile.length)) {
            number += 1;
        }
    }
    return number;
}

uint32 main_execute(FileList *old, FileList *new, const uint32 number_changes) {
    uint32 length;
    HashTable *names_renamed;
    uint32 number_renames = 0;

    length = old->length;
    names_renamed = hash_table_create(number_changes);

    for (uint32 i = 0; i < length; i += 1) {
        int renamed;
        char **oldname = &(old->files[i].name);
        char **newname = &(new->files[i].name);
        uint32 *oldlength = &(old->files[i].length);
        uint32 *newlength = &(new->files[i].length);

        if (!strcmp(*oldname, *newname))
            continue;

        renamed = renameat2(AT_FDCWD, *oldname, 
                            AT_FDCWD, *newname, RENAME_EXCHANGE);
        if (renamed >= 0) {
            if (hash_table_insert(names_renamed, *oldname, *oldlength))
                number_renames += 1;
            if (hash_table_insert(names_renamed, *newname, *newlength))
                number_renames += 1;

            printf(GREEN"%s"RESET" <-> "GREEN"%s"RESET"\n", *oldname, *newname);
            for (uint32 j = i + 1; j < length; j += 1) {
                FileName *file_j = &(old->files[j]);
                if (file_j->length != *newlength)
                    continue;
                if (!memcmp(file_j->name, *newname, *newlength)) {
                    SWAP(char *, file_j->name, *oldname);
                    SWAP(uint32, file_j->length, *oldlength);
                    break;
                }
            }
            continue;
        }

        renamed = rename(*oldname, *newname);
        if (renamed < 0) {
            printf("Error renaming "
                    RED"%s"RESET" to "RED"%s"RESET":\n", 
                    *oldname, *newname);
            printf("%s\n", strerror(errno));
            continue;
        } else {
            if (hash_table_insert(names_renamed, *oldname, *oldlength))
                number_renames += 1;
            printf("%s -> "GREEN"%s"RESET"\n", *oldname, *newname);
        }
    }
    hash_table_destroy(names_renamed);
    return number_renames;
}


void main_free_file_list(FileList *file_list) {
    for (uint32 i = 0; i < file_list->length; i += 1)
        free(file_list->files[i].name);
    free(file_list);
    return;
}

void main_usage(FILE *stream) {
    fprintf(stream, "usage: brn2 [-h | --help | <filename> | file1 file2 ... ]\n");
    fprintf(stream, "No arguments        : rename filenames in current working directory.\n");
    fprintf(stream, "Single argument     : rename filenames listed in this argument.\n");
    fprintf(stream, "2 or more arguments : rename filenames passed as arguments.\n");
    fprintf(stream, "-h | --help         : display this help message.\n");
    exit((int) (stream != stdout));
}
