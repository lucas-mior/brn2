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

#include <sys/mman.h>
#include "brn2.h"
#include "util.c"

Arena *
arena_alloc(size_t size) {
    void *p;
    Arena *arena;

    size += ALIGN(sizeof(*arena));

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

    arena = p;
    arena->begin = (char *)arena + ALIGN(sizeof(*arena));
    arena->size = size;
    arena->pos = arena->begin;
    return arena;
}

void
arena_destroy(Arena *arena) {
    if (munmap(arena, arena->size) < 0) {
        error("Error in %s:\n", __func__);
        error("Error in munmap(%p, %zu): %s.\n",
              arena, arena->size, strerror(errno));
    }
    return;
}

void *
arena_push(Arena *arena, uint32 size) {
    void *before = arena->pos;
    arena->pos = (char *)arena->pos + size;
    return before;
}

uint32
arena_push_index(Arena *arena, uint32 size) {
    void *before = arena->pos;
    arena->pos = (char *)arena->pos + size;
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

#endif
