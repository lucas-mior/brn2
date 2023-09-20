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

#include "util.h"
#include "hash.h"

#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

typedef struct Bucket {
    char *key;
    uint32 hash;
    uint32 value;
    struct Bucket *next;
} Bucket;

struct HashSet {
    uint32 capacity;
    uint32 unused;
    uint32 collisions;
    uint32 length;
    Bucket array[];
};

HashSet *hash_set_create(uint32 length) {
    HashSet *set;
    uint32 size;

    if (length > (UINT32_MAX/4))
        length = (UINT32_MAX/4);
    length *= 4;

    size = sizeof (*set) + length * sizeof (set->array[0]);

    set = util_malloc(size);
    memset(set, 0, size);
    set->capacity = length;
    return set;
}

void hash_set_destroy(HashSet *set) {
    for (uint32 i = 0; i < set->capacity; i += 1) {
        Bucket *iterator = &(set->array[i]);
        iterator = iterator->next;
        while (iterator) {
            void *aux = iterator;
            iterator = iterator->next;
            free(aux);
        }
    }
    free(set);
    return;
}

uint32 hash_function(char *str) {
    /* djb2 hash function */
    uint32 hash = 5381;
    char c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + (uint32) c;
    }
    return hash;
}

bool hash_set_insert(HashSet *set, char *key, const uint32 value) {
    uint32 hash = hash_function(key);
    uint32 index = hash % set->capacity;
    return hash_set_insert_pre_calc(set, key, hash, index, value);
}

uint32 *hash_set_lookup(HashSet *set, char *key) {
    uint32 hash = hash_function(key);
    uint32 index = hash % set->capacity;
    return hash_set_lookup_pre_calc(set, key, hash, index);
}

uint32 *hash_set_lookup_pre_calc(HashSet *set, char *key, const uint32 hash, const uint32 index) {
    Bucket *iterator = &(set->array[index]);

    if (iterator->key == NULL)
        return NULL;

    do {
        if ((hash == iterator->hash) && !strcmp(iterator->key, key))
            return &(iterator->value);

        if (iterator->next)
            iterator = iterator->next;
        else
            break;
    } while (true);

    return NULL;
}

bool hash_set_insert_pre_calc(HashSet *set, char *key,
                              const uint32 hash, const uint32 index, const uint32 value) {
    Bucket *iterator = &(set->array[index]);

    if (iterator->key == NULL) {
        iterator->key = key;
        iterator->hash = hash;
        iterator->value = value;
        set->length += 1;
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

    set->collisions += 1;
    iterator->next = util_calloc(1, sizeof (*iterator));
    iterator->next->key = key;
    iterator->next->hash = hash;
    iterator->next->value = value;
    set->length += 1;

    return true;
}

bool hash_set_remove(HashSet *set, char *key) {
    uint32 hash = hash_function(key);
    uint32 index = hash % set->capacity;
    return hash_set_remove_pre_calc(set, key, hash, index);
}

bool hash_set_remove_pre_calc(HashSet *set, char *key, const uint32 hash, const uint32 index) {
    Bucket *iterator = &(set->array[index]);
    Bucket *previous;

    if (iterator->key == NULL)
        return false;

    if ((hash == iterator->hash) && !strcmp(iterator->key, key)) {
        if (iterator->next) {
            void *aux = iterator->next;
            memmove(iterator, iterator->next, sizeof (*iterator));
            free(aux);
            set->collisions -= 1;
        } else {
            memset(iterator, 0, sizeof (*iterator));
        }
        set->length -= 1;
        return true;
    }

    while (iterator->next) {
        previous = iterator;
        iterator = iterator->next;

        if ((hash == iterator->hash) && !strcmp(iterator->key, key)) {
             previous->next = iterator->next;
             free(iterator);
             set->length -= 1;
             set->collisions -= 1;
             return true;
        }
    }

    return false;
}

void hash_set_print_summary(HashSet *set) {
    printf("Hash Set {\n");
    printf("  capacity: %u\n", set->capacity);
    printf("  length: %u\n", set->length);
    printf("  collisions: %u\n", set->collisions);
    printf("  expected collisions: %u\n", hash_set_expected_collisions(set));
    printf("}\n");
    return;
}

void hash_set_print(HashSet *set, bool verbose) {
    hash_set_print_summary(set);

    for (uint32 i = 0; i < set->capacity; i += 1) {
        Bucket *iterator = &(set->array[i]);
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
    return;
}

uint32 hash_set_capacity(HashSet *set) {
    return set->capacity;
}

uint32 hash_set_length(HashSet *set) {
    return set->length;
}

uint32 hash_set_collisions(HashSet *set) {
    return set->collisions;
}

uint32 hash_set_expected_collisions(HashSet *set) {
    long double n = set->length;
    long double m = set->capacity;
    long double result = n - m * (1 - powl((m - 1)/m, n));
    return (uint32) (roundl(result));
}
