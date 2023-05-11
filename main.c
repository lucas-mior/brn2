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

int main(int argc, char *argv[]) {
    char *EDITOR;
    if (!(EDITOR = getenv("EDITOR")))
        EDITOR = getenv("VISUAL");
    if (!EDITOR) {
        fprintf(stderr, "$EDITOR and $VISUAL "
                        "are both not set in the environment\n");
        exit(EXIT_FAILURE);
    }

    char *tempdir = getenv("TMPDIR");
    if (!tempdir)
        tempdir = "/tmp";

    char tempfile[PATH_MAX];
    snprintf(tempfile, sizeof (tempfile), "%s/%s", tempdir, "brn2.XXXXXX");

    int fd = mkstemp(tempfile);
    close(fd);

    FileList old;
    switch (argc) {
    case 2:
        if (!strncmp(argv[1], "--help", 6)) {
            main_usage(stdout);
        } else if (!strncmp(argv[1], "-h", 2)) {
            main_usage(stdout);
        } else {
            old = main_file_list_from_lines(argv[1], 0);
        }
        break;
    case 1:
        old = main_file_list_from_dir(".");
        break;
    default:
        main_usage(stderr);
        break;
    }

    FILE *file = fopen(tempfile, "r+");
    if (!file) {
        fprintf(stderr, "Error opening %s: %s\n", tempfile, strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < old.length; i += 1) {
        fprintf(file, "%s\n", old.files[i].name);
    }
    fclose(file);

    char *args[] = { EDITOR, tempfile, NULL };

    bool status = 0;
    FileList new;
    while (!status) {
        util_cmd(args);
        new = main_file_list_from_lines(tempfile, old.length);
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

    size_t n_changes = main_get_number_renames(&old, &new);
    size_t n_renames = 0;
    if (n_changes)
        n_renames = main_execute(&old, &new);
    if (n_changes != n_renames) {
        fprintf(stderr, "%zu name%.*s changed but %zu file%.*s renamed. "
                        "Check your files.\n", 
                        n_changes, n_changes != 1, "s",
                        n_renames, n_renames != 1, "s");
        status = false;
    } else {
        fprintf(stdout, "%zu file%.*s renamed\n",
                        n_renames, n_renames != 1, "s");
    }
    main_free_file_list(&old);
    main_free_file_list(&new);
    unlink(tempfile);
    exit(!status);
}

FileList main_file_list_from_dir(char *dir) {
    struct dirent **directory_list;
    int n = scandir(dir, &directory_list, NULL, versionsort);
    if (n < 0) {
        fprintf(stderr, "Error scanning %s: %s\n", dir, strerror(errno));
        exit(EXIT_FAILURE);
    }

    FileList file_list;
    file_list.files = util_realloc(NULL, n * sizeof (FileName));

    int length = 0;
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
    file_list.length = length;
    return file_list;
}

FileList main_file_list_from_lines(char *filename, size_t cap) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (cap == 0)
        cap = 128;

    FileList file_list;
    file_list.files = util_realloc(NULL, cap * sizeof (FileName));

    char buffer[PATH_MAX];
    size_t length = 0;
    while (!feof(file)) {
        if (length >= cap) {
            cap *= 2;
            file_list.files = util_realloc(file_list.files, cap * sizeof (FileName));
        }

        if (!fgets(buffer, sizeof(buffer), file))
            continue;
        if (!strcmp(buffer, ".") || !strcmp(buffer, ".."))
            continue;

        size_t last = strcspn(buffer, "\n");
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
    bool repeated = false;
    size_t bsize = new->length > MIN_HASH_TABLE_SIZE ? new->length : MIN_HASH_TABLE_SIZE;
    SameHash *table = util_calloc(bsize, sizeof(SameHash));
    for (size_t i = 0; i < new->length; i += 1) {
        char *name = new->files[i].name;
        size_t h = hash_function(name);
        if (hash_insert(table, h % bsize, name)) {
            fprintf(stderr, RED"\"%s\""RESET
                            " appears more than once in the buffer\n", name);
            repeated = true;
        }
    }
    hash_free(table, bsize);
    return repeated;
}

bool main_repeated_name_naive(FileList *new) {
    bool rep = false;
    for (size_t i = 0; i < new->length; i += 1) {
        char *name = new->files[i].name;
        size_t length = new->files[i].length;
        for (size_t j = i+1; j < new->length; j += 1) {
            if (length != new->files[j].length)
                continue;
            if (!strcmp(name, new->files[j].name)) {
                fprintf(stderr, RED"\"%s\""RESET 
                                " repeated in the buffer\n", name);
                rep = true;
            }
        }
    }
    return rep;
}

bool main_verify(FileList *old, FileList *new) {
    if (old->length != new->length) {
        fprintf(stderr, "You are renaming %zu file%.*s "
                        "but buffer contains %zu file name%.*s\n", 
                        old->length, old->length != 1, "s",
                        new->length, new->length != 1, "s");
        return false;
    }

    bool rep = false;
    if (new->length > 100)
        rep = main_repeated_name_hash(new);
    else
        rep = main_repeated_name_naive(new);

    return !rep;
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
    size_t length = old->length;

    SameHash *names_renamed = util_calloc(length, sizeof(SameHash));

    size_t n_renames = 0;
    for (size_t i = 0; i < length; i += 1) {
        char *oldname = old->files[i].name;
        char *newname = new->files[i].name;

        if (!strcmp(oldname, newname))
            continue;

        int r;
        r = renameat2(AT_FDCWD, oldname, 
                      AT_FDCWD, newname, RENAME_EXCHANGE);
        if (r >= 0) {
            size_t h1 = hash_function(oldname);
            size_t h2 = hash_function(newname);

            if (!hash_insert(names_renamed, h1 % length, oldname))
                n_renames += 1;
            if (!hash_insert(names_renamed, h2 % length, newname))
                n_renames += 1;

            printf(GREEN"%s"RESET" <-> "GREEN"%s"RESET"\n", oldname, newname);
            for (size_t j = i + 1; j < old->length; j += 1) {
                if (old->files[j].length != new->files[i].length)
                    continue;
                if (!strcmp(old->files[j].name, newname))
                    strcpy(old->files[j].name, oldname);
            }
            continue;
        } else {
            r = rename(oldname, newname);
        }

        if (r < 0) {
            printf("Error renaming "
                    RED"%s"RESET" to "RED"%s"RESET":\n", 
                    oldname, newname);
            printf("%s\n", strerror(errno));
            continue;
        } else {
            size_t h1 = hash_function(oldname);
            if (!hash_insert(names_renamed, h1 % length, oldname))
                n_renames += 1;
            printf("%s -> "GREEN"%s"RESET"\n", oldname, newname);
        }
    }
    hash_free(names_renamed, length);
    return n_renames;
}

void main_usage(FILE *stream) {
    fprintf(stream, "usage: brn2 [-h | --help | <filename>]\n");
    fprintf(stream, "Without arguments, rename files in current dir.\n");
    fprintf(stream, "<filename>, rename files listed in <filename>.\n");
    fprintf(stream, "--help : display this help message.\n");
    fprintf(stream, "Be sure to have EDITOR or VISUAL "
                    "environment variables properly set.\n");
    exit((int) (stream != stdout));
    return;
}

void main_free_file_list(FileList *file_list) {
    for (size_t i = 0; i < file_list->length; i += 1)
        free(file_list->files[i].name);
    free(file_list->files);
    return;
}
