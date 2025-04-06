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

#include "brn2.h"
#include "util.c"

Arena *
arena_alloc(char *name, size_t size) {
    void *p;
    Arena *arena;

    size += ALIGN(sizeof(*arena));

    p = util_alloc_huge(size);

    arena = p;
    arena->name = name;
    arena->begin = (char *)arena + ALIGN(sizeof(*arena));
    arena->size = size;
    arena->pos = arena->begin;
    return arena;
}

void
arena_destroy(Arena *arena) {
    util_free_huge(arena, arena->size);
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

#ifdef TESTING_arena
int
main(void) {
    exit(EXIT_SUCCESS);
}
#endif

#endif
