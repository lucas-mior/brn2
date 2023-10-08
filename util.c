/*
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "util.h"

void *util_malloc(const usize size) {
    void *p;
    if ((p = malloc(size)) == NULL) {
        fprintf(stderr, "Failed to allocate %zu bytes.\n", size);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *util_realloc(void *old, const usize size) {
    void *p;
    if ((p = realloc(old, size)) == NULL) {
        fprintf(stderr, "Failed to reallocate %zu bytes.\n", size);
        fprintf(stderr, "Reallocating from: %p\n", old);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *util_calloc(const usize nmemb, const usize size) {
    void *p;
    if ((p = calloc(nmemb, size)) == NULL) {
        fprintf(stderr, "Failed to allocate %zu members of %zu bytes each.\n",
                        nmemb, size);
        exit(EXIT_FAILURE);
    }
    return p;
}

char *util_strdup(char *string) {
    char *p;
    if ((p = strdup(string)) == NULL) {
        fprintf(stderr, "Failed to allocate duplicate %s.\n", string);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *util_memdup(void *source, usize size) {
    void *p;
    if ((p = malloc(size)) == NULL) {
        fprintf(stderr, "Failed to reallocate %zu bytes.\n", size);
        exit(EXIT_FAILURE);
    }
    memcpy(p, source, size);
    return p;
}

void util_command(const int argc, char **argv) {
    switch (fork()) {
    case 0:
        if (!freopen("/dev/tty", "r", stdin))
            fprintf(stderr, "Error reopening stdin: %s\n", strerror(errno));
        execvp(argv[0], argv);
        fprintf(stderr, "Error running '%s", argv[0]);
        for (int i = 1; i < argc; i += 1)
            fprintf(stderr, " %s", argv[i]);
        fprintf(stderr, "': %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    case -1:
        fprintf(stderr, "Error forking: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    default:
        if (wait(NULL) < 0) {
            fprintf(stderr, "Error waiting for the forked child: %s\n",
                            strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}
