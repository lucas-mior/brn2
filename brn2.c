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

#define _USE_GNU
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/syscall.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

typedef struct FileName {
    char *name;
    size_t len;
} FileName;

typedef struct FileList {
    FileName *files;
    size_t len;
} FileList;

typedef struct SameHash {
    char *key;
    struct SameHash *next;
} SameHash;

void *ealloc(void *old, size_t size) {
    void *p;
    if ((p = realloc(old, size)) == NULL) {
        fprintf(stderr, "Failed to allocate %zu bytes.\n", size);
        exit(1);
    }
    return p;
}

void *ecalloc(size_t nmemb, size_t size) {
    void *p;
    if ((p = calloc(nmemb, size)) == NULL) {
        fprintf(stderr, "Failed to allocate %zu members of %zu bytes each.\n", 
                        nmemb, size);
        exit(1);
    }
    return p;
}

size_t hash(char *str) {
    /* djb2 hash function */
    size_t hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

void cmd(char **argv) {
    switch (fork()) {
    case 0:
        execvp(argv[0], argv);
        fprintf(stderr, "Error running `%s`: %s\n", argv[0], strerror(errno));
        exit(1);
        break;
    case -1:
        fprintf(stderr, "Error forking: %s\n", strerror(errno));
        exit(1);
        break;
    default:
        if (wait(NULL) < 0) {
            fprintf(stderr, "Error waiting for the forked child: %s\n", 
                            strerror(errno));
            exit(1);
        }
        break;
    }
}

FileList flist_from_dir(char *dir) {
    struct dirent **namelist;
    int n = scandir(dir, &namelist, NULL, versionsort);
    if (n < 0) {
        fprintf(stderr, "Error scanning %s: %s\n", dir, strerror(errno));
        exit(1);
    }

    FileList flist;
    flist.files = ealloc(NULL, n * sizeof (FileName));

    int len = 0;
    for (int i = 0; i < n; i += 1) {
        char *name = namelist[i]->d_name;
        if (!strcmp(name, ".") || !strcmp(name, "..")) {
            free(namelist[i]);
            continue;
        }

        flist.files[len].name = strdup(name);
        flist.files[len].len = strlen(name);
        free(namelist[i]);
        len += 1;
    }
    if (len == 0) {
        fprintf(stderr, "Empty directory. Exiting.\n");
        exit(1);
    }
    free(namelist);
    flist.len = len;
    return flist;
}

FileList flist_from_lines(char *filename, size_t cap) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening %s: %s\n", filename, strerror(errno));
        exit(1);
    }

    if (cap == 0)
        cap = 128;

    FileList flist;
    flist.files = ealloc(NULL, cap * sizeof (FileName));

    char buffer[PATH_MAX];
    size_t len = 0;
    while (!feof(file)) {
        if (len >= cap) {
            cap *= 2;
            flist.files = ealloc(flist.files, cap * sizeof (FileName));
        }

        if (!fgets(buffer, sizeof(buffer), file))
            continue;
        if (!strcmp(buffer, ".") || !strcmp(buffer, ".."))
            continue;

        size_t last = strcspn(buffer, "\n");
        buffer[last] = '\0';
        flist.files[len].name = strdup(buffer);
        flist.files[len].len = last;
        len += 1;
    }
    if (len == 0) {
        fprintf(stderr, "Empty filelist. Exiting.\n");
        exit(1);
    }
    fclose(file);
    flist.files = ealloc(flist.files, len * sizeof (FileName));
    flist.len = len;
    return flist;
}

bool check_insert(SameHash *sh, size_t h, char *newkey) {
    SameHash *it = &sh[h];

    if (it->key == NULL) {
        it->key = newkey;
        return false;
    }

    do {
        if (!strcmp(it->key, newkey))
            return true;

        if (it->next)
            it = it->next;
        else
            break;
    } while (true);

    it->next = ecalloc(1, sizeof (SameHash));
    it->next->key = newkey;

    return false;
}

bool dup_check_hash(FileList *new) {
    bool rep = false;
    size_t bsize = new->len > 256 ? new->len : 256;
    SameHash *strings = ecalloc(bsize, sizeof(SameHash));
    for (size_t i = 0; i < new->len; i += 1) {
        char *name = new->files[i].name;
        size_t h = hash(name);
        if (check_insert(strings, h % bsize, name)) {
            fprintf(stderr, RED"\"%s\""RESET
                            " appears more than once in the buffer\n", name);
            rep = true;
        }
    }
    for (size_t i = 0; i < bsize; i += 1) {
        SameHash *it = &strings[i];
        it = it->next;
        while (it) {
            void *aux = it;
            it = it->next;
            free(aux);
        }
    }
    free(strings);
    return rep;
}

bool dup_check_naive(FileList *new) {
    bool rep = false;
    for (size_t i = 0; i < new->len; i += 1) {
        char *name = new->files[i].name;
        size_t len = new->files[i].len;
        for (size_t j = i+1; j < new->len; j += 1) {
            if (len != new->files[j].len)
                continue;
            if (!strcmp(name, new->files[j].name)) {
                fprintf(stderr, RED"\"%s\""RESET 
                                " appears more than once in the buffer\n", name);
                rep = true;
            }
        }
    }
    return rep;
}

bool verify(FileList *old, FileList *new) {
    if (old->len != new->len) {
        fprintf(stderr, "You are renaming %zu file%.*s "
                        "but buffer contains %zu file name%.*s\n", 
                        old->len, old->len != 1, "s",
                        new->len, new->len != 1, "s");
        return false;
    }

    bool rep = false;
    if (new->len > 100)
        rep = dup_check_hash(new);
    else
        rep = dup_check_naive(new);

    return !rep;
}

size_t get_num_renames(FileList *old, FileList *new) {
    size_t num = 0;
    for (size_t i = 0; i < old->len; i += 1) {
        if (strcmp(old->files[i].name, new->files[i].name))
            num += 1;
    }
    return num;
}

size_t execute(FileList *old, FileList *new) {
    size_t len = old->len;

    size_t n_renames = 0;
    for (size_t i = 0; i < len; i += 1) {
        char *oldname = old->files[i].name;
        char *newname = new->files[i].name;

        if (!strcmp(oldname, newname))
            continue;

        int r;
        r = renameat2(AT_FDCWD, oldname, 
                      AT_FDCWD, newname, RENAME_EXCHANGE);
        if (r < 0) {
            r = rename(oldname, newname);
        } else {
            n_renames += 1;
            printf("%s -> "GREEN"%s"RESET"\n", newname, oldname);
        }

        if (r < 0) {
            printf("Error renaming "
                    RED"%s"RESET" to "RED"%s"RESET":\n", 
                    oldname, newname);
            printf("%s\n", strerror(errno));
            continue;
        } else {
            printf("%s -> "GREEN"%s"RESET"\n", oldname, newname);
            n_renames += 1;
        }

        for (size_t j = i + 1; j < old->len; j += 1) {
            if (old->files[j].len != new->files[i].len)
                continue;
            if (!strcmp(old->files[j].name, newname))
                strcpy(old->files[j].name, oldname);
        }
    }
    return n_renames;
}

void usage(FILE *stream) {
    fprintf(stream, "usage: brn2 [--help | <filename>]\n");
    fprintf(stream, "Without arguments, rename files in current dir.\n");
    fprintf(stream, "<filename>, rename files listed in <filename>.\n");
    fprintf(stream, "--help : display this help message.\n");
    fprintf(stream, "Be sure to have EDITOR or VISUAL environment variables properly set.\n");
    exit((int) (stream != stdout));
    return;
}

void free_flist(FileList *f) {
    for (size_t i = 0; i < f->len; i += 1)
        free(f->files[i].name);
    free(f->files);
    return;
}

int main(int argc, char *argv[]) {
    char *editor_cmd;
    if (!(editor_cmd = getenv("EDITOR")))
        editor_cmd = getenv("VISUAL");
    if (!editor_cmd) {
        fprintf(stderr, "$EDITOR and $VISUAL are both not set in the environment\n");
        exit(1);
    }

    char *tempdir = getenv("TMPDIR");
    if (!tempdir)
        tempdir = "/tmp";

    char tempfile[PATH_MAX];
    strcpy(tempfile, tempdir);
    strcat(tempfile, "/brn2.XXXXXX");

    int fd = mkstemp(tempfile);
    close(fd);

    FileList old;
    switch (argc) {
    case 2:
        if (!strncmp(argv[1], "--help", 6)) {
            usage(stdout);
        } else {
            old = flist_from_lines(argv[1], 0);
        }
        break;
    case 1:
        old = flist_from_dir(".");
        break;
    default:
        usage(stderr);
        break;
    }

    FILE *file = fopen(tempfile, "r+");
    if (!file) {
        fprintf(stderr, "Error opening %s: %s\n", tempfile, strerror(errno));
        exit(1);
    }

    for (size_t i = 0; i < old.len; i += 1) {
        fprintf(file, "%s\n", old.files[i].name);
    }
    fclose(file);

    char *args[] = { editor_cmd, tempfile, NULL };

    bool status = 0;
    FileList new;
    while (!status) {
        cmd(args);
        new = flist_from_lines(tempfile, old.len);
        if ((status = verify(&old, &new))) {
            break;
        } else {
            free_flist(&new);
            printf("Fix your renames. Press control-c to cancel or press"
                   " ENTER to open vim again.\n");
            getc(stdin);
            continue;
        }
    }

    size_t n_changes = get_num_renames(&old, &new);
    size_t n_renames = 0;
    if (n_changes)
        n_renames = execute(&old, &new);
    if (n_changes != n_renames) {
        fprintf(stderr, "%zu name%.*s changed but %zu file%.*s renamed. Check your files.\n", 
                        n_changes, n_changes != 1, "s",
                        n_renames, n_renames != 1, "s");
        status = false;
    } else {
        fprintf(stdout, "%zu file%.*s renamed\n", n_renames, n_renames != 1, "s");
    }
    free_flist(&old);
    free_flist(&new);
    unlink(tempfile);
    exit(!status);
}
