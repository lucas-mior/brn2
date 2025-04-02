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

#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct Hashmap HashMap;
typedef struct Hashset HashSet;

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

uint32 hash_function(char *, uint32);
uint32 hash_normal(void *, uint32);
uint32 hash_capacity(void *);
uint32 hash_length(void *);
uint32 hash_collisions(void *);
uint32 hash_expected_collisions(void *);

HashMap *hash_map_create(uint32);
HashMap *hash_map_balance(HashMap *);
void hash_map_destroy(HashMap *);
bool hash_map_insert(HashMap *, char *, uint32, uint32);
bool hash_map_insert_pre_calc(HashMap *, char *, uint32, uint32, uint32, uint32);
void *hash_map_lookup(HashMap *, char *, uint32);
void *hash_map_lookup_pre_calc(HashMap *, char *, uint32, uint32, uint32);
bool hash_map_remove(HashMap *, char *, uint32);
bool hash_map_remove_pre_calc(HashMap *, char *, uint32, uint32, uint32);
void hash_map_print_summary(HashMap *, char *);
void hash_map_print(HashMap *, bool);

HashSet *hash_set_create(uint32);
HashSet *hash_set_balance(HashSet *);
void hash_set_destroy(HashSet *);
bool hash_set_insert(HashSet *, char *, uint32, uint32);
bool hash_set_insert_pre_calc(HashSet *, char *, uint32, uint32, uint32, uint32);
void *hash_set_lookup(HashSet *, char *, uint32);
void *hash_set_lookup_pre_calc(HashSet *, char *, uint32, uint32, uint32);
bool hash_set_remove(HashSet *, char *, uint32);
bool hash_set_remove_pre_calc(HashSet *, char *, uint32, uint32, uint32);
void hash_set_print_summary(HashSet *, char *);
void hash_set_print(HashSet *, bool);

#define Q(x) #x
#define QUOTE(x) Q(x)
#define HASH_map_PRINT_SUMMARY(MAP) hash_map_print_summary(MAP, QUOTE(MAP))
#define HASH_set_PRINT_SUMMARY(MAP) hash_set_print_summary(MAP, QUOTE(MAP))
#define HASH_MAP_SIZE(map) (sizeof(*map) + map->capacity*sizeof(map->array[0]))

#endif
