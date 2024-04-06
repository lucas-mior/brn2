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

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "util.h"
#include "brn2.h"

void *
util_malloc(const usize size) {
    void *p;
    if ((p = malloc(size)) == NULL) {
        error("Failed to allocate %zu bytes.\n", size);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *
util_realloc(void *old, const usize size) {
    void *p;
    if ((p = realloc(old, size)) == NULL) {
        error("Failed to reallocate %zu bytes.\n", size);
        error("Reallocating from: %p\n", old);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *
util_calloc(const usize nmemb, const usize size) {
    void *p;
    if ((p = calloc(nmemb, size)) == NULL) {
        error("Error allocating %zu members of %zu bytes each.\n", nmemb, size);
        exit(EXIT_FAILURE);
    }
    return p;
}

char *
util_strdup(char *string) {
    char *p;
    if ((p = strdup(string)) == NULL) {
        error("Error allocating duplicate \"%s\".\n", string);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *
util_memdup(void *source, usize size) {
    void *p;
    if ((p = malloc(size)) == NULL) {
        error("Error reallocating %zu bytes.\n", size);
        exit(EXIT_FAILURE);
    }
    memcpy(p, source, size);
    return p;
}

void
util_command(const int argc, char **argv) {
    switch (fork()) {
    case 0:
        if (!freopen("/dev/tty", "r", stdin))
            error("Error reopening stdin: %s\n", strerror(errno));
        execvp(argv[0], argv);
        error("Error running '%s", argv[0]);
        for (int i = 1; i < argc; i += 1)
            error(" %s", argv[i]);
        error("': %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    case -1:
        error("Error forking: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    default:
        if (wait(NULL) < 0) {
            error("Error waiting for the forked child: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

void error(char *format, ...) {
    int n;
    va_list args;
    char buffer[BUFSIZ];

    va_start(args, format);
    n = vsnprintf(buffer, sizeof (buffer) - 1, format, args);
    va_end(args);

    if (n < 0) {
        fprintf(stderr, "Error in vsnprintf()\n");
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    (void) write(STDERR_FILENO, buffer, (usize) n);

#ifdef DEBUGGING
    switch (fork()) {
        char *notifiers[2] = { "dunstify", "notify-send" };
        case -1:
            fprintf(stderr, "Error forking: %s\n", strerror(errno));
            break;
        case 0:
            for (uint i = 0; i < LENGTH(notifiers); i += 1) {
                execlp(notifiers[i], notifiers[i], "-u", "critical", 
                                     program, buffer, NULL);
            }
            fprintf(stderr, "Error trying to exec dunstify.\n");
            break;
        default:
            break;
    }
    exit(EXIT_FAILURE);
#endif
}

#ifndef MAIN
#define MAIN 0
#endif

#if MAIN
int main(int argc, char **argv) {
    assert(true);
    exit(0);
}
#endif
