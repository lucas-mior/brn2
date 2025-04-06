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

#include "brn2.h"

#ifndef __WIN32__
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
    if ((w = write2(STDERR_FILENO, buffer, (usize)n)) < n) {
        fprintf(stderr, "Error writing to STDERR_FILENO");
        if (w < 0)
            fprintf(stderr, ": %s", strerror(errno));
        fprintf(stderr, ".\n");
    }
    return;
}
#endif

void
util_free_huge(void *p, usize size) {
#ifdef __linux__
    if (munmap(p, size) < 0)
        error("Error in munmap(%p, %zu): %s.\n", p, size, strerror(errno));
#else
    (void) size;
    free(p);
#endif
    return;
}

#ifdef __linux__
void *
util_alloc_huge(usize size) {
    void *p;
    do {
        if (size >= SIZE2MB) {
            p = mmap(NULL, size,
                     PROT_READ|PROT_WRITE,
                     MAP_ANONYMOUS|MAP_PRIVATE|MAP_HUGETLB|MAP_HUGE_2MB,
                     -1, 0);
            if (p != MAP_FAILED) {
                size = BRN2_ALIGN(size, SIZE2MB);
                break;
            }
        }
        p = mmap(NULL, size,
                 PROT_READ|PROT_WRITE,
                 MAP_ANONYMOUS|MAP_PRIVATE,
                 -1, 0);
    } while (0);

    if (p == MAP_FAILED) {
        error("Error in mmap(%zu): %s.\n", size, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return p;
}
#else 
void *
util_alloc_huge(usize size) {
    void *p;
    size = MIN(SIZE4GB, size);
    p = xmalloc(size);
    return p;
}
#endif

void *
xmalloc(const usize size) {
    void *p;
    if ((p = malloc(size)) == NULL) {
        error("Failed to allocate %zu bytes.\n", size);
        exit(EXIT_FAILURE);
    }
    return p;
}

#ifdef __linux__
void *
xmmap_commit(usize *size) {
    void *p;

    do {
        if (*size >= SIZE2MB) {
            p = mmap(NULL, *size,
                     PROT_READ|PROT_WRITE,
                     MAP_ANONYMOUS|MAP_POPULATE|MAP_PRIVATE
                     |MAP_HUGETLB|MAP_HUGE_2MB,
                     -1, 0);
            if (p != MAP_FAILED) {
                *size = BRN2_ALIGN(*size, SIZE2MB);
                break;
            }
        }
        p = mmap(NULL, *size,
                 PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE|MAP_POPULATE,
                 -1, 0);
    } while (0);
    if (p == MAP_FAILED) {
        error("Error in mmap(%zu): %s.\n", *size, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return p;
}
#else
void *
xmmap_commit(usize *size) {
    void *p;
    p = xmalloc(*size);
    return p;
}
#endif

void
xmunmap(void *p, usize size) {
#ifdef __linux__
    if (munmap(p, size) < 0)
        error("Error in munmap(%p, %zu): %s.\n", p, size, strerror(errno));
#else
    (void) size;
    free(p);
#endif
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

void *
snprintf2(char *buffer, size_t size, char *format, ...) {
    int n;
    va_list args;

    va_start(args, format);
    n = vsnprintf(buffer, size, format, args);
    va_end(args);

    if (n >= (int)size) {
        va_list args2;
        buffer = xmalloc((usize)n + 1);
        va_start(args, format);
        va_copy(args2, args);
        n = vsnprintf(buffer, (usize)n + 1, format, args);
        va_end(args);
    }
    if (n <= 0) {
        error("Error in snprintf.\n");
        exit(EXIT_FAILURE);
    }
    return buffer;
}

#ifdef __WIN32__
void util_command(const int argc, char **argv) {
    if (argc == 0 || argv == NULL) {
        error("Invalid arguments.\n");
        exit(EXIT_FAILURE);
    }

    size_t len = 1;
    for (int i = 0; i < argc - 1; ++i)
        len += strlen(argv[i]) + 3;
    char *cmdline = xmalloc(len);

    cmdline[0] = '\0';
    for (int i = 0; i < argc - 1; ++i) {
        strcat(cmdline, "\"");
        strcat(cmdline, argv[i]);
        strcat(cmdline, "\"");
        if (i < argc - 1)
            strcat(cmdline, " ");
    }

    FILE *tty = freopen("CONIN$", "r", stdin);
    if (!tty) {
        error("Error reopening stdin: %s.\n", strerror(errno));
        free(cmdline);
        exit(EXIT_FAILURE);
    }

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    BOOL success = CreateProcessA(
        NULL,
        cmdline,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );

    if (!success) {
        error("Error running '%s", argv[0]);
        for (int i = 1; i < argc; i++)
            error(" %s", argv[i]);
        error("': %lu.\n", GetLastError());
        free(cmdline);
        exit(EXIT_FAILURE);
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exit_code = 0;
    GetExitCodeProcess(pi.hProcess, &exit_code);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    free(cmdline);
    return;
}
#else
void
util_command(const int argc, char **argv) {
    pid_t child;
    switch (child = fork()) {
    case 0:
        if (!freopen("/dev/tty", "r", stdin))
            error("Error reopening stdin: %s.\n", strerror(errno));
        execvp(argv[0], argv);
        error("Error running '%s", argv[0]);
        for (int i = 1; i < argc; i += 1)
            error(" %s", argv[i]);
        error("': %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    case -1:
        error("Error forking: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    default:
        if (waitpid(child, NULL, 0) < 0) {
            error("Error waiting for the forked child: %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}
#endif

#ifdef TESTING_util
#include <assert.h>

int main(void) {
    void *p1 = xmalloc(1024*1024*1024);
    void *p2 = xcalloc(1024, 1024*1024);
    char *p3;
    char *string = __FILE__;
    memcpy(p1, string, strlen(string));
    memset(p2, 0, 1024*1024*1024);
    p3 = xstrdup(p1);
    error("%s == %s is working? %b\n", string, p3, !strcmp(string, p3));
    exit(0);
}

#endif

#endif
