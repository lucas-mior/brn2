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

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_RESET "\x1b[0m"

typedef unsigned long ulong;

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
    if ((p = realloc(old, size))) {
        return p;
    } else {
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(1);
    }
}

void *ecalloc(size_t nmemb, size_t size) {
    void *p;
    if ((p = calloc(nmemb, size))) {
        return p;
    } else {
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(1);
    }
}

ulong hash(char *str, ulong max) {
    /* djb2 hash function */
    ulong hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash % max;
}

void cmd(char **argv) {
    switch (fork()) {
    case 0:
        execvp(argv[0], argv);
        fprintf(stderr, "Could not execute the child: %s\n", strerror(errno));
        exit(1);
        break;
    case -1:
        fprintf(stderr, "Could not fork a child: %s\n", strerror(errno));
        exit(1);
        break;
    default:
        if (wait(NULL) < 0) {
            fprintf(stderr, "Could not wait for the forked child: %s\n", strerror(errno));
            exit(1);
        }
        break;
    }
}

FileList flist_from_dir(char *dir) {
    struct dirent **namelist;
    int n = scandir(dir, &namelist, NULL, versionsort);
    if (n < 0) {
        fprintf(stderr, "Could not scan directory %s\n", dir);
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
    free(namelist);
    flist.len = len;
    return flist;
}

FileList flist_from_lines(char *filename, size_t cap) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
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

        buffer[strcspn(buffer, "\n")] = '\0';
        flist.files[len].name = strdup(buffer);
        flist.files[len].len = strlen(buffer);
        len += 1;
    }
    fclose(file);
    flist.files = ealloc(flist.files, len * sizeof (FileName));
    flist.len = len;
    return flist;
}

typedef struct args {
    FileList *flist;
    size_t begin;
    size_t end;
} args;

bool insert(SameHash *sh, ulong index, char *newkey) {
    SameHash *it = &sh[index];

    do {
        if (it->key == NULL)
            break;
        if (!strcmp(it->key, newkey)) {
            fprintf(stderr, "\"%s\" appears more than once in the buffer\n", newkey);
            return true;
        }
        it = it->next;
    } while (it->next);
    it->next = ecalloc(1, sizeof (SameHash));
    it->key = newkey;

    return false;
}

bool verify(FileList old, FileList new) {
    if (old.len != new.len) {
        fprintf(stderr, "You are renaming %zu files but buffer contains %zu file names\n", old.len, new.len);
        return false;
    }

    bool rep = false;
    if (new.len > 100) {
        SameHash *strings = ecalloc(new.len, sizeof(SameHash));
        for (size_t i = 0; i < new.len; i += 1) {
            char *name = new.files[i].name;
            ulong h = hash(name, new.len);
            rep = insert(strings, h, name) || rep;
        }
    } else {
        /* for short lists of filenames, use naive approach */
        for (size_t i = 0; i < new.len; i += 1) {
            char *name = new.files[i].name;
            size_t len = new.files[i].len;
            for (size_t j = i+1; j < new.len; j += 1) {
                if (len != new.files[j].len)
                    continue;
                if (!strcmp(name, new.files[j].name)) {
                    fprintf(stderr, "\"%s\" appears more than once in the buffer\n", name);
                    rep = true;
                }
            }
        }
    }

    return !rep;
}

size_t get_num_renames(FileList old, FileList new) {
    size_t num = 0;
    for (size_t i = 0; i < old.len; i += 1) {
        if (strcmp(old.files[i].name, new.files[i].name))
            num += 1;
    }
    return num;
}

void execute(FileList *old, FileList *new) {
    size_t len = old->len;

    for (size_t i = 0; i < len; i += 1) {
        char *oldname = old->files[i].name;
        char *newname = new->files[i].name;

        if (strcmp(oldname, newname) == 0)
            continue;

        int r;
        r = renameat2(AT_FDCWD, oldname, AT_FDCWD, newname, RENAME_EXCHANGE);
        if (r < 0) {
            r = rename(oldname, newname);
        } 
        if (r < 0) {
            printf("Error renaming "
                    ANSI_RED"%s"ANSI_RESET" to "ANSI_RED"%s"ANSI_RESET".\n", 
                    oldname, newname);
            printf("%s", strerror(errno));
        } else {
            printf("%s -> "ANSI_GREEN"%s"ANSI_RESET"\n", oldname, newname);
        }

        for (size_t j = i + 1; j < old->len; j += 1) {
            if (strcmp(old->files[j].name, newname) == 0) {
                strcpy(old->files[j].name, oldname);
            }
        }
    }
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
    /* not using on user provided strings so should be fine */
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
    }

    FILE *file = fopen(tempfile, "r+");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", tempfile);
        exit(1);
    }

    for (size_t i = 0; i < old.len; i += 1) {
        fprintf(file, "%s\n", old.files[i].name);
    }
    fclose(file);

    char *args[] = { editor_cmd, tempfile, NULL };
    cmd(args);

    FileList new = flist_from_lines(tempfile, old.len);

    bool status;

    if ((status = verify(old, new))) {
        size_t n_renames = get_num_renames(old, new);
        fprintf(stdout, "%zu files renamed\n", n_renames);
        if (n_renames)
            execute(&old, &new);
    }
    for (size_t i = 0; i < old.len; i += 1)
        free(old.files[i].name);
    free(old.files);
    for (size_t i = 0; i < new.len; i += 1)
        free(new.files[i].name);
    free(new.files);
    unlink(tempfile);
    exit(!status);
}