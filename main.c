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

static FileList *main_file_list_from_dir(char *);
static FileList *main_file_list_from_lines(char *, size_t);
static bool main_repeated_name_hash(FileList *);
static bool main_repeated_name_naive(FileList *);
static bool main_verify(FileList *, FileList *);
static size_t main_get_number_changes(FileList *, FileList *);
static size_t main_execute(FileList *, FileList *);
static void main_usage(FILE *) __attribute__((noreturn));
static void main_free_file_list(FileList *);
static char *EDITOR;
static const char *tempdir = "/tmp";

enum {
    CAPACITY_NONE = 0,
    CAPACITY_INITIAL_GUESS = 128,
};

int main(int argc, char *argv[]) {
    File buffer;
    FileList *old;
    FileList *new;
    bool status = true;

    if (argc >= 3) {
        main_usage(stderr);
    } else if (argc == 2) {
        if (!strncmp(argv[1], "--help", 6)) {
            main_usage(stdout);
        } else if (!strncmp(argv[1], "-h", 2)) {
            main_usage(stdout);
        } else {
            old = main_file_list_from_lines(argv[1], CAPACITY_NONE);
        }
    } else {
        old = main_file_list_from_dir(".");
    }

    if (!(EDITOR = getenv("EDITOR"))) {
        EDITOR = "vim";
        fprintf(stderr, "EDITOR variable is not set. Using %s by default.\n", EDITOR);
    }

    {
        snprintf(buffer.name, sizeof (buffer.name),
                "%s/%s", tempdir, "brn2.XXXXXX");

        if ((buffer.fd = mkstemp(buffer.name)) < 0) {
            fprintf(stderr, "Error opening %s: %s\n",
                            buffer.name, strerror(errno));
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < old->length; i += 1)
            dprintf(buffer.fd, "%s\n", old->files[i].name);
        close(buffer.fd);
        buffer.fd = -1;
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
        size_t number_changes;
        size_t number_renames;
        number_changes = main_get_number_changes(old, new);
        number_renames = 0;

        if (number_changes)
            number_renames = main_execute(old, new);
        if (number_changes != number_renames) {
            fprintf(stderr, "%zu name%.*s changed but %zu file%.*s renamed. "
                            "Check your files.\n", 
                            number_changes, number_changes != 1, "s",
                            number_renames, number_renames != 1, "s");
            status = false;
        } else {
            fprintf(stdout, "%zu file%.*s renamed\n",
                            number_renames, number_renames != 1, "s");
        }
    }

    main_free_file_list(old);
    main_free_file_list(new);
    unlink(buffer.name);
    exit(!status);
}

FileList *main_file_list_from_dir(char *directory) {
    FileList *file_list;
    struct dirent **directory_list;
    size_t length = 0;

    int n = scandir(directory, &directory_list, NULL, versionsort);
    if (n < 0) {
        fprintf(stderr, "Error scanning %s: %s\n", directory, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (n <= 2) {
        fprintf(stderr, "Empty directory. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    file_list = util_realloc(NULL, STRUCT_ARRAY_SIZE(FileList, FileName, n - 2));

    for (int i = 0; i < n; i += 1) {
        char *name = directory_list[i]->d_name;
        size_t name_length;
        if (!strcmp(name, ".") || !strcmp(name, "..")) {
            free(directory_list[i]);
            continue;
        }
        name_length = strlen(name);
        file_list->files[length].name = util_realloc(NULL, name_length+1);
        memcpy(file_list->files[length].name, name, name_length+1);
        file_list->files[length].length = name_length;

        free(directory_list[i]);
        length += 1;
    }
    file_list->length = length;
    free(directory_list);
    return file_list;
}

FileList *main_file_list_from_lines(char *filename, size_t capacity) {
    FileList *file_list;
    FILE *file = fopen(filename, "r");
    size_t length = 0;
    bool new_buffer = true;
    void (*func)(void);

    if (!file) {
        fprintf(stderr, "Error opening %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    void realoc(void) {
        if (length >= capacity) {
            capacity *= 2;
            file_list = util_realloc(file_list, STRUCT_ARRAY_SIZE(FileList, FileName, capacity));
        }
    }

    void nothing(void) {
        return;
    }

    if (capacity == CAPACITY_NONE) {
        capacity = CAPACITY_INITIAL_GUESS;
        new_buffer = false;
        func = realoc;
    } else {
        func = nothing;
    }

    file_list = util_realloc(NULL, STRUCT_ARRAY_SIZE(FileList, FileName, capacity));

    while (!feof(file)) {
        char buffer[PATH_MAX];
        size_t last;
        func();

        if (!fgets(buffer, sizeof(buffer), file))
            continue;

        last = strcspn(buffer, "\n");
        if (last == 0)
            continue;

        buffer[last] = '\0';
        if (!strcmp(buffer, ".") || !strcmp(buffer, ".."))
            continue;
        if (!new_buffer && access(buffer, F_OK))
            continue;

        file_list->files[length].name = util_realloc(NULL, last+1);
        memcpy(file_list->files[length].name, buffer, last+1);
        file_list->files[length].length = last;
        length += 1;
    }
    if (length == 0) {
        fprintf(stderr, "Empty filelist. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    fclose(file);
    file_list = util_realloc(file_list, STRUCT_ARRAY_SIZE(FileList, FileName, length));
    file_list->length = length;
    return file_list;
}

bool main_repeated_name_hash(FileList *new) {
    bool repeated;
    HashTable *repeated_table;

    repeated = false;
    repeated_table = hash_table_create(new->length);
    for (size_t i = 0; i < new->length; i += 1) {
        char *name = new->files[i].name;
        size_t length = new->files[i].length;

        if (!hash_insert(repeated_table, name, length)) {
            fprintf(stderr, RED"\"%s\""RESET
                            " appears more than once in the buffer\n", name);
            repeated = true;
        }
    }
    hash_table_destroy(repeated_table);
    return repeated;
}

bool main_repeated_name_naive(FileList *new) {
    bool repeated = false;
    for (size_t i = 0; i < new->length; i += 1) {
        char *name = new->files[i].name;
        size_t length = new->files[i].length;
        for (size_t j = i+1; j < new->length; j += 1) {
            if (length != new->files[j].length)
                continue;
            if (!strcmp(name, new->files[j].name)) {
                fprintf(stderr, RED"\"%s\""RESET 
                                " repeated in the buffer\n", name);
                repeated = true;
            }
        }
    }
    return repeated;
}

bool main_verify(FileList *old, FileList *new) {
    bool repeated = false;
    if (old->length != new->length) {
        fprintf(stderr, "You are renaming %zu file%.*s "
                        "but buffer contains %zu file name%.*s\n", 
                        old->length, old->length != 1, "s",
                        new->length, new->length != 1, "s");
        return false;
    }

    if (new->length > USE_HASH_TABLE_THRESHOLD)
        repeated = main_repeated_name_hash(new);
    else
        repeated = main_repeated_name_naive(new);

    return !repeated;
}

size_t main_get_number_changes(FileList *old, FileList *new) {
    size_t number = 0;
    for (size_t i = 0; i < old->length; i += 1) {
        if (strcmp(old->files[i].name, new->files[i].name))
            number += 1;
    }
    return number;
}

size_t main_execute(FileList *old, FileList *new) {
    size_t length;
    HashTable *names_renamed;
    size_t number_renames = 0;

    length = old->length;
    names_renamed = hash_table_create(length);

    for (size_t i = 0; i < length; i += 1) {
        int renamed;
        char **oldname = &(old->files[i].name);
        size_t *oldlength = &(old->files[i].length);
        char *newname = new->files[i].name;
        size_t newlength = new->files[i].length;

        if (!strcmp(*oldname, newname))
            continue;

        renamed = renameat2(AT_FDCWD, *oldname, 
                            AT_FDCWD, newname, RENAME_EXCHANGE);
        if (renamed >= 0) {
            if (hash_insert(names_renamed, *oldname, *oldlength))
                number_renames += 1;
            if (hash_insert(names_renamed, newname, newlength))
                number_renames += 1;

            printf(GREEN"%s"RESET" <-> "GREEN"%s"RESET"\n", *oldname, newname);
            for (size_t j = i + 1; j < length; j += 1) {
                if (old->files[j].length != new->files[i].length)
                    continue;
                if (!strcmp(old->files[j].name, newname)) {
                    SWAP(old->files[j].name, *oldname);
                    SWAP(old->files[j].length, *oldlength);
                    break;
                }
            }
            continue;
        }

        renamed = rename(*oldname, newname);
        if (renamed < 0) {
            printf("Error renaming "
                    RED"%s"RESET" to "RED"%s"RESET":\n", 
                    *oldname, newname);
            printf("%s\n", strerror(errno));
            continue;
        } else {
            if (hash_insert(names_renamed, *oldname, *oldlength))
                number_renames += 1;
            printf("%s -> "GREEN"%s"RESET"\n", *oldname, newname);
        }
    }
    hash_table_destroy(names_renamed);
    return number_renames;
}

void main_usage(FILE *stream) {
    fprintf(stream, "usage: brn2 [-h | --help | <filename>]\n");
    fprintf(stream, "Without arguments, rename files in current directory.\n");
    fprintf(stream, "<filename>, rename files listed in <filename>.\n");
    fprintf(stream, "-h     : display this help message.\n");
    fprintf(stream, "--help : display this help message.\n");
    fprintf(stream, "Be sure to have EDITOR or VISUAL "
                    "environment variables properly set.\n");
    exit((int) (stream != stdout));
}

void main_free_file_list(FileList *file_list) {
    for (size_t i = 0; i < file_list->length; i += 1)
        free(file_list->files[i].name);
    free(file_list);
    return;
}
