/*
 * Copyright (C) 2024 Mior, Lucas; <lucasbuddemior@gmail.com>
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

#ifndef ARENA_H
#define ARENA_H

typedef struct Arena {
    void *begin;
    void *pos;
    size_t size;
} Arena;

Arena *arena_alloc(size_t);
void *arena_push(Arena *arena, uint32);
void *arena_reset(Arena *arena);
void *arena_reset_zero(Arena *arena);
void arena_destroy(Arena *arena);

#endif
