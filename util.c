/*
 * Copyright (C) 2024 Mior, Lucas; 
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

#ifndef UTIL_C
#define UTIL_C

#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>

#include "brn2.h"

void *
xmalloc(const usize size) {
    void *p;
    if ((p = malloc(size)) == NULL) {
        error("Failed to allocate %zu bytes.\n", size);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *
xmmap(const usize size) {
    void *p;
    p = mmap(NULL, size,
             PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE|MAP_POPULATE,
             -1, 0);
    if (p == MAP_FAILED) {
        error("Failed to map memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return p;
}

void
xmunmap(void *p, usize size) {
    if (munmap(p, size) < 0)
        error("Error in munmap(%p, %zu): %s\n", (void *)p, strerror(errno));
    return;
}

void *
xrealloc(void *old, const usize size) {
    void *p;
    if ((p = realloc(old, size)) == NULL) {
        error("Failed to reallocate %zu bytes.\n", size);
        error("Reallocating from: %p\n", old);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *
xcalloc(const usize nmemb, const usize size) {
    void *p;
    if ((p = calloc(nmemb, size)) == NULL) {
        error("Error allocating %zu members of %zu bytes each.\n", nmemb, size);
        exit(EXIT_FAILURE);
    }
    return p;
}

char *
xstrdup(char *string) {
    char *p;
    size_t length;

    length = strlen(string) + 1;
    if ((p = malloc(length)) == NULL) {
        error("Error allocating %zi bytes to duplicate \"%s\".\n",
               length, string);
        exit(EXIT_FAILURE);
    }

    memcpy(p, string, length);
    return p;
}

void *
xmemdup(void *source, usize size) {
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
    pid_t child;
    switch (child = fork()) {
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
        if (waitpid(child, NULL, 0) < 0) {
            error("Error waiting for the forked child: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

void error(char *format, ...) {
    int n;
    ssize_t w;
    va_list args;
    char buffer[BUFSIZ];

    va_start(args, format);
    n = vsnprintf(buffer, sizeof(buffer) - 1, format, args);
    va_end(args);

    if (n < 0) {
        fprintf(stderr, "Error in vsnprintf()\n");
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    if ((w = write(STDERR_FILENO, buffer, (usize)n)) < n) {
        fprintf(stderr, "Error writing to STDERR_FILENO");
        if (w < 0)
            fprintf(stderr, ": %s", strerror(errno));
        fprintf(stderr, ".\n");
    }

#if BRN2_DEBUG
    char *notifiers[2] = { "dunstify", "notify-send" };
    switch (fork()) {
    case -1:
        fprintf(stderr, "Error forking: %s\n", strerror(errno));
        break;
    case 0:
        for (uint i = 0; i < ARRAY_LENGTH(notifiers); i += 1) {
            execlp(notifiers[i], notifiers[i], "-u", "critical", 
                                 program, buffer, NULL);
            fprintf(stderr, "Error trying to exec(%s).\n", notifiers[i]);
        }
        _exit(EXIT_FAILURE);
    default:
        break;
    }
#endif
}

#ifndef TESTING_THIS_FILE
#define TESTING_THIS_FILE 0
#endif

#if TESTING_THIS_FILE
#include <assert.h>

int main(void) {
    assert(true);
    exit(0);
}

#endif

#endif
