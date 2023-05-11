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

static FileList main_file_list_from_dir(char *);
static FileList main_file_list_from_lines(char *, size_t);
static bool main_repeated_name_hash(FileList *);
static bool main_repeated_name_naive(FileList *);
static bool main_verify(FileList *, FileList *);
static size_t main_get_number_renames(FileList *, FileList *);
static size_t main_execute(FileList *, FileList *);
static void main_usage(FILE *) __attribute__((noreturn));
static void main_free_file_list(FileList *);
static char *EDITOR;
static const char *tempdir = "/tmp";

int main(int argc, char *argv[]) {
    File buffer;
    FileList old;
    FileList new;
    bool status = 0;

    if (argc >= 3) {
        main_usage(stderr);
    } else if (argc == 2) {
        if (!strncmp(argv[1], "--help", 6)) {
            main_usage(stdout);
        } else if (!strncmp(argv[1], "-h", 2)) {
            main_usage(stdout);
        } else {
            old = main_file_list_from_lines(argv[1], 0);
        }
    } else {
        old = main_file_list_from_dir(".");
    }

    if (!(EDITOR = getenv("EDITOR"))) {
        fprintf(stderr, "EDITOR variable is not set. Using vim by default.\n");
        EDITOR = "vim";
    }

    snprintf(buffer.name, sizeof (buffer.name), "%s/%s", tempdir, "brn2.XXXXXX");

    if ((buffer.fd = mkstemp(buffer.name)) < 0) {
        fprintf(stderr, "Error opening %s: %s\n", buffer.name, strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < old.length; i += 1)
        dprintf(buffer.fd, "%s\n", old.files[i].name);
    close(buffer.fd);
    buffer.fd = -1;

    {
        size_t number_changes;
        size_t number_renames;

        char *args[] = { EDITOR, buffer.name, NULL };
        while (!status) {
            util_command(args);
            new = main_file_list_from_lines(buffer.name, old.length);
            if ((status = main_verify(&old, &new))) {
                break;
            } else {
                main_free_file_list(&new);
                printf("Fix your renames. Press control-c to cancel or press"
                       " ENTER to open the file list editor again.\n");
                getc(stdin);
                continue;
            }
        }

        number_changes = main_get_number_renames(&old, &new);
        number_renames = 0;

        if (number_changes)
            number_renames = main_execute(&old, &new);
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

    main_free_file_list(&old);
    main_free_file_list(&new);
    unlink(buffer.name);
    exit(!status);
}

FileList main_file_list_from_dir(char *dir) {
    FileList file_list;
    struct dirent **directory_list;
    int length = 0;

    int n = scandir(dir, &directory_list, NULL, versionsort);
    if (n < 0) {
        fprintf(stderr, "Error scanning %s: %s\n", dir, strerror(errno));
        exit(EXIT_FAILURE);
    }

    file_list.files = util_realloc(NULL, (size_t) n * sizeof (FileName));

    for (int i = 0; i < n; i += 1) {
        char *name = directory_list[i]->d_name;
        if (!strcmp(name, ".") || !strcmp(name, "..")) {
            free(directory_list[i]);
            continue;
        }

        file_list.files[length].name = strdup(name);
        file_list.files[length].length = strlen(name);
        free(directory_list[i]);
        length += 1;
    }
    if (length == 0) {
        fprintf(stderr, "Empty directory. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    free(directory_list);
    file_list.length = (size_t) length;
    return file_list;
}

FileList main_file_list_from_lines(char *filename, size_t capacity) {
    FileList file_list;
    FILE *file = fopen(filename, "r");
    size_t length = 0;

    if (!file) {
        fprintf(stderr, "Error opening %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (capacity == 0)
        capacity = 128;

    file_list.files = util_realloc(NULL, capacity * sizeof (FileName));

    while (!feof(file)) {
        char buffer[PATH_MAX];
        size_t last;
        if (length >= capacity) {
            capacity *= 2;
            file_list.files = util_realloc(file_list.files, capacity * sizeof (FileName));
        }

        if (!fgets(buffer, sizeof(buffer), file))
            continue;
        if (!strcmp(buffer, ".") || !strcmp(buffer, ".."))
            continue;

        last = strcspn(buffer, "\n");
        buffer[last] = '\0';
        file_list.files[length].name = strdup(buffer);
        file_list.files[length].length = last;
        length += 1;
    }
    if (length == 0) {
        fprintf(stderr, "Empty filelist. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    fclose(file);
    file_list.files = util_realloc(file_list.files, length * sizeof (FileName));
    file_list.length = length;
    return file_list;
}

bool main_repeated_name_hash(FileList *new) {
    bool repeated;
    size_t table_size;
    SameHash *table;

    repeated = false;
    table_size = new->length > MIN_HASH_TABLE_SIZE ? new->length : MIN_HASH_TABLE_SIZE;

    table = util_calloc(table_size, sizeof(SameHash));
    for (size_t i = 0; i < new->length; i += 1) {
        char *name = new->files[i].name;
        size_t h = hash_function(name);
        if (!hash_insert(table, h % table_size, name)) {
            fprintf(stderr, RED"\"%s\""RESET
                            " appears more than once in the buffer\n", name);
            repeated = true;
        }
    }
    hash_free(table, table_size);
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

    if (new->length > 100)
        repeated = main_repeated_name_hash(new);
    else
        repeated = main_repeated_name_naive(new);

    return !repeated;
}

size_t main_get_number_renames(FileList *old, FileList *new) {
    size_t num = 0;
    for (size_t i = 0; i < old->length; i += 1) {
        if (strcmp(old->files[i].name, new->files[i].name))
            num += 1;
    }
    return num;
}

size_t main_execute(FileList *old, FileList *new) {
    size_t length;
    SameHash *names_renamed;
    size_t number_renames = 0;

    length = old->length;
    names_renamed = util_calloc(length, sizeof(SameHash));

    for (size_t i = 0; i < length; i += 1) {
        int renamed;
        char *oldname = old->files[i].name;
        char *newname = new->files[i].name;

        if (!strcmp(oldname, newname))
            continue;

        renamed = renameat2(AT_FDCWD, oldname, 
                      AT_FDCWD, newname, RENAME_EXCHANGE);
        if (renamed >= 0) {
            size_t h1 = hash_function(oldname);
            size_t h2 = hash_function(newname);

            if (hash_insert(names_renamed, h1 % length, oldname))
                number_renames += 1;
            if (hash_insert(names_renamed, h2 % length, newname))
                number_renames += 1;

            printf(GREEN"%s"RESET" <-> "GREEN"%s"RESET"\n", oldname, newname);
            for (size_t j = i + 1; j < old->length; j += 1) {
                if (old->files[j].length != new->files[i].length)
                    continue;
                if (!strcmp(old->files[j].name, newname))
                    strcpy(old->files[j].name, oldname);
            }
            continue;
        } else {
            renamed = rename(oldname, newname);
        }

        if (renamed < 0) {
            printf("Error renaming "
                    RED"%s"RESET" to "RED"%s"RESET":\n", 
                    oldname, newname);
            printf("%s\n", strerror(errno));
            continue;
        } else {
            size_t h1 = hash_function(oldname);
            if (hash_insert(names_renamed, h1 % length, oldname))
                number_renames += 1;
            printf("%s -> "GREEN"%s"RESET"\n", oldname, newname);
        }
    }
    hash_free(names_renamed, length);
    return number_renames;
}

void main_usage(FILE *stream) {
    fprintf(stream, "usage: brn2 [-h | --help | <filename>]\n");
    fprintf(stream, "Without arguments, rename files in current dir.\n");
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
    free(file_list->files);
    return;
}
