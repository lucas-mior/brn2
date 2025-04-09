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

#ifndef ARENA_C
#define ARENA_C

#ifdef __WIN32__
#include <windows.h>
#else
#include <sys/mman.h>
#include "unistd.h"
#endif

#include "string.h"
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "assert.h"
#include "stdbool.h"
#include "stdint.h"

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
  #define ALIGNMENT 16lu
#endif
#if !defined(ALIGN)
  #define ALIGN(x) ARENA_ALIGN(x, ALIGNMENT)
#endif

#ifdef __linux__
  #define TRY_HUGE_PAGES 1
#else
  #define TRY_HUGE_PAGES 0
#endif

#ifndef INTEGERS
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

static Arena *arena_alloc(size_t);
static void *arena_malloc(size_t *);
static void arena_destroy(Arena *);
static void *arena_push(Arena *, uint32);
static uint32 arena_push_index32(Arena *, uint32);
static void *arena_reset(Arena *);
static void *arena_reset_zero(Arena *);

static size_t page_size = 0;

Arena *
arena_alloc(size_t size) {
    void *p;
    Arena *arena;

    size += ALIGN(sizeof(*arena));
    p = arena_malloc(&size);

    arena = p;
    arena->begin = (char *)arena + ALIGN(sizeof(*arena));
    arena->size = size;
    arena->pos = arena->begin;
    arena->next = NULL;
    arena_push(arena, ALIGNMENT);

    return arena;
}

#ifndef __WIN32__
void *
arena_malloc(size_t *size) {
    void *p;

    if (page_size == 0) {
        long aux;
        if ((aux = sysconf(_SC_PAGESIZE)) <= 0) {
            fprintf(stderr, "Error getting page size: %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        page_size = (size_t)aux;
    }

    do {
        if ((*size >= SIZEMB(2)) && TRY_HUGE_PAGES) {
            p = mmap(NULL, *size,
                     PROT_READ|PROT_WRITE,
                     MAP_ANON|MAP_PRIVATE|MAP_HUGETLB|MAP_HUGE_2MB,
                     -1, 0);
            if (p != MAP_FAILED) {
                *size = ARENA_ALIGN(*size, SIZEMB(2));
                break;
            }
        }
        p = mmap(NULL, *size,
                 PROT_READ|PROT_WRITE,
                 MAP_ANON|MAP_PRIVATE,
                 -1, 0);
        *size = ARENA_ALIGN(*size, page_size);
    } while (0);

    if (p == MAP_FAILED) {
        fprintf(stderr, "Error in mmap(%zu): %s.\n", *size, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return p;
}
void
arena_destroy(Arena *arena) {
    if (munmap(arena, arena->size) < 0) {
        fprintf(stderr, "Error in munmap(%p, %zu): %s.\n",
                        (void *)arena, arena->size, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return;
}
#else 
void *
arena_malloc(size_t *size) {
    void *p;

    if (page_size == 0) {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        page_size = si.dwPageSize;
        if (page_size <= 0) {
            fprintf(stderr, "Error getting page size.\n");
            exit(EXIT_FAILURE);
        }
    }

    p = VirtualAlloc(NULL, *size,
                           MEM_COMMIT|MEM_RESERVE,
                           PAGE_READWRITE);
    if (p == NULL) {
        fprintf(stderr, "Error in VirtualAlloc(%zu): %lu.\n",
                        *size, GetLastError());
        exit(EXIT_FAILURE);
    }
    *size = ARENA_ALIGN(*size, page_size);
    return p;
}
void
arena_destroy(Arena *arena) {
    if (!VirtualFree(arena, 0, MEM_RELEASE)) {
        fprintf(stderr, "Error in VirtualFree(%p): %lu.\n",
                        arena, GetLastError());
        exit(EXIT_FAILURE);
    }
    return;
}
#endif

void *
arena_push(Arena *arena, uint32 size) {
    void *before;

    while ((char *)arena->pos >= ((char *)arena + arena->size - (size_t)size)) {
        if (!arena->next)
            arena->next = arena_alloc(arena->size - ALIGN(sizeof(*arena)));

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
    arena->pos = arena->begin;
    return arena->begin;
}

void *
arena_reset_zero(Arena *arena) {
    size_t size = (size_t)((char *)arena->pos - (char *)arena->begin);
    memset(arena->begin, 0, size);
    arena->pos = arena->begin;
    return arena->begin;
}

#ifdef TESTING_arena
int
main(void) {
    Arena *arena;
    assert((arena = arena_alloc(UINT32_MAX)));
    assert(arena_push(arena, UINT32_MAX - 1));
    arena_destroy(arena);
    exit(EXIT_SUCCESS);
}
#endif

#endif
