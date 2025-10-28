/*
 * Copyright (C) 2025 Mior, Lucas;
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

#if !defined(ARENA_C)
#define ARENA_C

#if defined(__linux__)
#define OS_LINUX 1
#define OS_MAC 0
#define OS_BSD 0
#define OS_WINDOWS 0
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_LINUX 0
#define OS_MAC 1
#define OS_BSD 0
#define OS_WINDOWS 0
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#define OS_LINUX 0
#define OS_MAC 0
#define OS_BSD 1
#define OS_WINDOWS 0
#elif defined(_WIN32) || defined(_WIN64)
#define OS_LINUX 0
#define OS_MAC 0
#define OS_BSD 0
#define OS_WINDOWS 1
#else
#error "Unsupported OS.\n"
#endif

#define OS_UNIX (OS_LINUX || OS_MAC || OS_BSD)

#if OS_WINDOWS
#include <windows.h>
#endif

#if OS_UNIX
#include <sys/mman.h>
#include <unistd.h>
#endif

#if defined(__INCLUDE_LEVEL__) && __INCLUDE_LEVEL__ == 0
#define TESTING_arena 1
#elif !defined(TESTING_arena)
#define TESTING_arena 0
#endif

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct Arena {
    char *name;
    char *begin;
    void *pos;
    size_t size;
    struct Arena *next;
} Arena;

#if !defined(SIZEKB)
#define SIZEKB(X) ((size_t)(X)*1024ul)
#define SIZEMB(X) ((size_t)(X)*1024ul*1024ul)
#define SIZEGB(X) ((size_t)(X)*1024ul*1024ul*1024ul)
#endif

#define ARENA_ALIGN(S, A) (((S) + ((A) - 1)) & ~((A) - 1))
#if !defined(ALIGNMENT)
#define ALIGNMENT 16ul
#endif
#if !defined(ALIGN)
#define ALIGN(x) ARENA_ALIGN(x, ALIGNMENT)
#endif

#if OS_LINUX
#define FLAGS_HUGE_PAGES MAP_HUGETLB | MAP_HUGE_2MB
#else
#define FLAGS_HUGE_PAGES 0
#endif

#if !defined(INTEGERS)
#define INTEGERS
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
#endif

static Arena *arena_create(size_t);
static void *arena_allocate(size_t *);
static void arena_destroy(Arena *);
static void arena_free(Arena *);
static void *arena_push(Arena *, uint32);
static uint32 arena_push_index32(Arena *, uint32);
static void *arena_reset(Arena *);

static size_t arena_page_size = 0;

Arena *
arena_create(size_t size) {
    void *p;
    Arena *arena;

    p = arena_allocate(&size);

    arena = p;
    arena->begin = (char *)arena + ALIGN(sizeof(*arena));
    arena->size = size;
    arena->pos = arena->begin;
    arena->next = NULL;
    arena_push(arena, ALIGNMENT);

    return arena;
}

#if OS_UNIX
void *
arena_allocate(size_t *size) {
    void *p;

    if (arena_page_size == 0) {
        long aux;
        if ((aux = sysconf(_SC_PAGESIZE)) <= 0) {
            fprintf(stderr, "Error getting page size: %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        arena_page_size = (size_t)aux;
    }

    do {
        if ((*size >= SIZEMB(2)) && FLAGS_HUGE_PAGES) {
            p = mmap(NULL, *size, PROT_READ | PROT_WRITE,
                     MAP_ANON | MAP_PRIVATE | FLAGS_HUGE_PAGES, -1, 0);
            if (p != MAP_FAILED) {
                *size = ARENA_ALIGN(*size, SIZEMB(2));
                break;
            }
        }
        p = mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE,
                 -1, 0);
        *size = ARENA_ALIGN(*size, arena_page_size);
    } while (0);

    if (p == MAP_FAILED) {
        fprintf(stderr, "Error in mmap(%zu): %s.\n", *size, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return p;
}
void
arena_free(Arena *arena) {
    if (munmap(arena, arena->size) < 0) {
        fprintf(stderr, "Error in munmap(%p, %zu): %s.\n", (void *)arena,
                arena->size, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return;
}
#else
void *
arena_allocate(size_t *size) {
    void *p;

    if (arena_page_size == 0) {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        arena_page_size = si.dwPageSize;
        if (arena_page_size <= 0) {
            fprintf(stderr, "Error getting page size.\n");
            exit(EXIT_FAILURE);
        }
    }

    p = VirtualAlloc(NULL, *size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (p == NULL) {
        fprintf(stderr, "Error in VirtualAlloc(%zu): %lu.\n", *size,
                GetLastError());
        exit(EXIT_FAILURE);
    }
    *size = ARENA_ALIGN(*size, arena_page_size);
    return p;
}
void
arena_free(Arena *arena) {
    if (!VirtualFree(arena, 0, MEM_RELEASE)) {
        fprintf(stderr, "Error in VirtualFree(%p): %lu.\n", arena,
                GetLastError());
        exit(EXIT_FAILURE);
    }
    return;
}
#endif

void
arena_destroy(Arena *arena) {
    Arena *next;

    do {
        next = arena->next;
        arena_free(arena);
    } while ((arena = next));

    return;
}

void *
arena_push(Arena *arena, uint32 size) {
    void *before;

    if (size > (arena->size - ALIGN(sizeof(*arena)))) {
        fprintf(stderr, "Error pushing %u bytes into arena of size %zu.\n",
                size, arena->size - ALIGN(sizeof(*arena)));
        exit(EXIT_FAILURE);
    }

    while ((char *)arena->pos >= (arena->begin + arena->size - size)) {
        if (!arena->next) {
            arena->next = arena_create(arena->size);
        }

        arena = arena->next;
    }

    before = arena->pos;
    arena->pos = (char *)arena->pos + size;
    return before;
}

uint32
arena_push_index32(Arena *arena, uint32 size) {
    void *before = arena->pos;
    arena->pos = (char *)arena->pos + size;
    assert(arena->size < UINT32_MAX);
    return (uint32)((char *)before - (char *)arena->begin);
}

void *
arena_reset(Arena *arena) {
    Arena *first = arena;

    do {
        arena->pos = arena->begin;
    } while ((arena = arena->next));

    return first->begin;
}

#if TESTING_arena
#include "assert.h"
int
main(void) {
    Arena *arena;
    void *begin;

    assert((arena = arena_create(SIZEMB(1))));
    begin = arena->begin;

    assert(arena_push(arena, 10));
    assert(arena_push(arena, 100));
    assert(arena_push(arena, 1000));
    assert(arena_push(arena, 10000));

    assert(arena_reset(arena));
    assert(arena->begin == begin);

    assert(arena_push(arena, 100000));
    assert(arena_push(arena, 1000000));

    arena_reset(arena);
    assert(arena_push(arena, SIZEMB(1) - ALIGN(sizeof(*arena))));
    for (int i = 0; i < 10; i += 1) {
        assert(arena_push(arena, 1000000));
    }

    arena_destroy(arena);
    exit(EXIT_SUCCESS);
}
#endif

#endif
