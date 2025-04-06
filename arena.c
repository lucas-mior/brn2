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
#endif

#include "string.h"
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "assert.h"
#include "stdint.h"

typedef struct Arena {
    char *name;
    char *begin;
    void *pos;
    size_t size;
} Arena;

#define SIZE2MB (2u*1024u*1024u)
#define SIZE4GB (1u*1024u*1024u*1024u)

#define ARENA_ALIGN(x, alignment) ((x) + ((alignment) - ((x) % (alignment))))
#if !defined(ALIGNMENT)
  #define ALIGNMENT 16
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

static void *arena_malloc(size_t size);
static void arena_destroy(Arena *arena);
static Arena *arena_alloc(char *name, size_t size);
static void *arena_push(Arena *arena, uint32 size);
static uint32 arena_push_index32(Arena *arena, uint32 size);
static int64 arena_push_index(Arena *arena, uint32 size);
static void * arena_reset(Arena *arena);
static void * arena_reset_zero(Arena *arena);

#ifndef __WIN32__
void *
arena_malloc(size_t size) {
    void *p;
    do {
        if ((size >= SIZE2MB) && TRY_HUGE_PAGES) {
            p = mmap(NULL, size,
                     PROT_READ|PROT_WRITE,
                     MAP_ANON|MAP_PRIVATE|MAP_HUGETLB|MAP_HUGE_2MB,
                     -1, 0);
            if (p != MAP_FAILED) {
                size = ARENA_ALIGN(size, SIZE2MB);
                break;
            }
        }
        p = mmap(NULL, size,
                 PROT_READ|PROT_WRITE,
                 MAP_ANON|MAP_PRIVATE,
                 -1, 0);
    } while (0);

    if (p == MAP_FAILED) {
        fprintf(stderr, "Error in mmap(%zu): %s.\n",
                        size, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return p;
}
void
arena_destroy(Arena *arena) {
    if (munmap(arena, arena->size) < 0)
        fprintf(stderr, "Error in munmap(%p, %zu): %s.\n",
                        (void *)arena, arena->size, strerror(errno));
    return;
}
#else 
void *
arena_malloc(size_t size) {
    void *p;

    p = VirtualAlloc(NULL, size,
                           MEM_COMMIT|MEM_RESERVE,
                           PAGE_READWRITE);
    if (p == NULL) {
        fprintf(stderr, "Error in VirtualAlloc(%zu): %lu.\n",
                        size, GetLastError());
        exit(EXIT_FAILURE);
    }
    return p;
}
void
arena_destroy(Arena *arena) {
    if (!VirtualFree(arena, 0, MEM_RELEASE)) {
        fprintf(stderr, "Error in VirtualFree(%p): %lu.\n",
                        arena, GetLastError());
    }
    return;
}
#endif

Arena *
arena_alloc(char *name, size_t size) {
    void *p;
    Arena *arena;

    size += ALIGN(sizeof(*arena));

    p = arena_malloc(size);

    arena = p;
    arena->name = name;
    arena->begin = (char *)arena + ALIGN(sizeof(*arena));
    arena->size = size;
    arena->pos = arena->begin;
    return arena;
}

void *
arena_push(Arena *arena, uint32 size) {
    void *before = arena->pos;
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

int64
arena_push_index(Arena *arena, uint32 size) {
    void *before = arena->pos;
    arena->pos = (char *)arena->pos + size;
    return (int64)((char *)before - (char *)arena->begin);
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
    exit(EXIT_SUCCESS);
}
#endif

#endif
