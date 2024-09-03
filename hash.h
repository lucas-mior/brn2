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

#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct HashMap HashMap;
typedef struct HashMap HashSet;

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

typedef size_t usize;
typedef ssize_t isize;
#endif

HashMap *hash_map_create(uint32);
HashMap *hash_map_balance(HashMap *);
void hash_map_free_keys(HashMap *);
void hash_map_destroy(HashMap *);
uint32 hash_function(char *);
uint32 hash_normal(uint32, uint32);
bool hash_map_insert(HashMap *, char *, uint32);
bool hash_map_insert_pre_calc(HashMap *, char *, uint32, uint32, uint32);
void *hash_map_lookup(HashMap *, char *);
void *hash_map_lookup_pre_calc(HashMap *, char *, uint32, uint32);
bool hash_map_remove(HashMap *, char *);
bool hash_map_remove_pre_calc(HashMap *, char *, uint32, uint32);
void hash_map_print_summary(HashMap *);
void hash_map_print(HashMap *, bool);
uint32 hash_map_capacity(HashMap *);
uint32 hash_map_length(HashMap *);
uint32 hash_map_collisions(HashMap *);
uint32 hash_map_expected_collisions(HashMap *);

#define hash_set_create(a)                   hash_map_create(a)
#define hash_set_balance(a)                  hash_map_balance(a)
#define hash_set_free_keys(a)                hash_map_free_keys(a)
#define hash_set_destroy(a)                  hash_map_destroy(a)
#define hash_set_insert(a, b)                hash_map_insert(a, b, 0)
#define hash_set_insert_pre_calc(a, b, c, d) hash_map_insert_pre_calc(a, b, c, d, 0)
#define hash_set_lookup(a, b)                hash_map_lookup(a, b)
#define hash_set_lookup_pre_calc(a, b, c, d) hash_map_lookup_pre_calc(a, b, c, d)
#define hash_set_remove(a, b)                hash_map_remove(a, b)
#define hash_set_remove_pre_calc(a, b, c, d) hash_map_remove_pre_calc(a, b, c, d)
#define hash_set_print_summary(a)            hash_map_print_summary(a)
#define hash_set_print(a, b)                 hash_map_print(a, b)
#define hash_set_capacity(a)                 hash_map_capacity(a)
#define hash_set_length(a)                   hash_map_length(a)
#define hash_set_collisions(a)               hash_map_collisions(a)
#define hash_set_expected_collisions(a)      hash_map_expected_collisions(a)

#endif
