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

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#include "util.h"
#include "hash.h"

#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

typedef struct Bucket {
    char *key;
    uint32 hash;
    struct Bucket *next;
} Bucket;

struct HashMap {
    uint32 capacity;
    uint32 collisions;
    uint32 length;
    Bucket array[];
};

uint32 hash_function(char *str, const uint32 length) {
    /* djb2 hash function */
    uint32 hash = 5381;
    for (uint32 i = 0; i < length; i += 1)
        hash = ((hash << 5) + hash) + (uint32) str[i];
    return hash;
}

bool hash_map_insert(HashMap *map, char *key, const uint32 key_length) {
    uint32 hash = hash_function(key, key_length);
    uint32 index = hash % map->capacity;
    return hash_map_insert_pre_calc(map, key, hash, index);
}

bool hash_map_insert_pre_calc(HashMap *map, char *key,
                                const uint32 hash, const uint32 index) {
    Bucket *iterator = &(map->array[index]);

    if (iterator->key == NULL) {
        iterator->key = key;
        iterator->hash = hash;
        map->length += 1;
        return true;
    }

    do {
        if ((hash == iterator->hash) && !strcmp(iterator->key, key))
            return false;

        if (iterator->next)
            iterator = iterator->next;
        else
            break;
    } while (true);

    map->collisions += 1;
    iterator->next = util_calloc(1, sizeof (*iterator));
    iterator->next->key = key;
    iterator->next->hash = hash;
    map->length += 1;

    return true;
}

bool hash_map_remove(HashMap *map, char *key, const uint32 key_length) {
    uint32 hash = hash_function(key, key_length);
    uint32 index = hash % map->capacity;
    Bucket *iterator = &(map->array[index]);
    Bucket *previous;

    if (iterator->key == NULL)
        return false;

    if ((hash == iterator->hash) && !strcmp(iterator->key, key)) {
        if (iterator->next) {
            void *aux = iterator->next;
            memmove(iterator, iterator->next, sizeof (*iterator));
            free(aux);
            map->collisions -= 1;
        } else {
            memset(iterator, 0, sizeof (*iterator));
        }
        map->length -= 1;
        return true;
    }

    while (iterator->next) {
        previous = iterator;
        iterator = iterator->next;

        if ((hash == iterator->hash) && !strcmp(iterator->key, key)) {
             previous->next = iterator->next;
             free(iterator);
             map->length -= 1;
             map->collisions -= 1;
             return true;
        }
    } while (true);

    return false;
}

HashMap *hash_map_create(uint32 length) {
    HashMap *map;
    uint32 size;

    if (length > (UINT32_MAX/4))
        length = (UINT32_MAX/4);
    length *= 4;

    size = sizeof (*map) + length * sizeof (map->array[0]);

    map = util_malloc(size);
    memset(map, 0, size);
    map->capacity = length;
    return map;
}

void hash_map_print_summary(HashMap *map) {
    printf("Hash Map {\n");
    printf("  capacity: %u\n", map->capacity);
    printf("  length: %u\n", map->length);
    printf("  collisions: %u\n", map->collisions);
    printf("  expected collisions: %u\n", hash_map_expected_collisions(map));
    printf("}\n");
}

void hash_map_print(HashMap *map, bool verbose) {
    hash_map_print_summary(map);

    for (uint32 i = 0; i < map->capacity; i += 1) {
        Bucket *iterator = &(map->array[i]);
        if (iterator->key || verbose) {
            printf("\n%03d:", i);
        } else {
            continue;
        }
        while (iterator && iterator->key) {
            printf(GREEN" %s"RESET" ->", iterator->key);
            iterator = iterator->next;
        }
    }
}

uint32 hash_map_capacity(HashMap *map) {
    return map->capacity;
}

uint32 hash_map_length(HashMap *map) {
    return map->length;
}

uint32 hash_map_collisions(HashMap *map) {
    return map->collisions;
}

uint32 hash_map_expected_collisions(HashMap *map) {
    long double n = map->length;
    long double m = map->capacity;
    long double result = n - m * (1 - powl((m-1)/m, n));
    return (uint32) (roundl(result));
}

void hash_map_destroy(HashMap *map) {
    for (uint32 i = 0; i < map->capacity; i += 1) {
        Bucket *iterator = &(map->array[i]);
        iterator = iterator->next;
        while (iterator) {
            void *aux = iterator;
            iterator = iterator->next;
            free(aux);
        }
    }
    free(map);
}
