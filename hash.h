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

#include <stdbool.h>

typedef struct HashSet HashSet;

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

HashSet *hash_set_create(uint32);
void hash_set_destroy(HashSet *);
uint32 hash_function(char *);
bool hash_set_insert(HashSet *, char *, const uint32);
bool hash_set_insert_pre_calc(HashSet *, char *, const uint32, const uint32, const uint32);
bool hash_set_remove(HashSet *, char *);
bool hash_set_remove_pre_calc(HashSet *, char *, const uint32, const uint32);
void hash_set_print_summary(HashSet *);
void hash_set_print(HashSet *, bool);
uint32 *hash_set_lookup(HashSet *, char *);
uint32 *hash_set_lookup_pre_calc(HashSet *, char *, const uint32, const uint32);
uint32 hash_set_capacity(HashSet *);
uint32 hash_set_length(HashSet *);
uint32 hash_set_collisions(HashSet *);
uint32 hash_set_expected_collisions(HashSet *);
