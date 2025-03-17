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

#ifndef HASH_C
#define HASH_C

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "util.h"

#pragma push_macro("TESTING_THIS_FILE")
#define TESTING_THIS_FILE 0

#include "util.c"

#pragma pop_macro("TESTING_THIS_FILE")

#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

typedef struct Bucket {
    char *key;
    uint32 hash;
    uint32 value;
    struct Bucket *next;
} Bucket;

struct HashMap {
    uint32 capacity;
    uint32 bitmask;
    uint32 collisions;
    uint32 length;
    Bucket array[];
};

HashMap *
hash_map_create(uint32 length) {
    HashMap *map;
    uint32 size;
    uint32 capacity = 1;
    uint32 power = 0;

    if (length > (UINT32_MAX/2))
        length = UINT32_MAX/2;

    while (capacity < length) {
        capacity *= 2;
        power += 1;
    }
    capacity *= 2;
    power += 1;

    size = sizeof(*map) + capacity*sizeof(map->array[0]);

    map = xmalloc(size);
    memset(map, 0, size);
    map->capacity = capacity;
    map->bitmask = (1 << power) - 1;
    return map;
}

HashMap *
hash_map_balance(HashMap *old_map) {
    HashMap *new_map;
    uint32 size;
    uint32 capacity;
    uint32 bitmask;

    if (old_map->capacity < (UINT32_MAX/2)) {
        capacity = old_map->capacity*2;
        bitmask = (old_map->bitmask << 1) + 1;
    } else if (old_map->capacity >= UINT32_MAX) {
        fprintf(stderr, "Error balancing hash map. Too big table.\n");
        return old_map;
    } else {
        capacity = UINT32_MAX;
        bitmask = UINT32_MAX;
    }

    size = sizeof(*new_map) + capacity*sizeof(new_map->array[0]);

    new_map = xmalloc(size);
    memset(new_map, 0, size);
    new_map->capacity = capacity;
    new_map->bitmask = bitmask;

    for (uint32 i = 0; i < old_map->capacity; i += 1) {
        Bucket *iterator = &(old_map->array[i]);

        if (iterator->key) {
            uint32 hash = iterator->hash;
            uint32 index = hash_normal(new_map, hash);
            hash_map_insert_pre_calc(new_map, iterator->key,
                                     hash, index, iterator->value);
        }
        iterator = iterator->next;

        while (iterator) {
            void *aux;
            uint32 hash = iterator->hash;
            uint32 index = hash_normal(new_map, hash);
            hash_map_insert_pre_calc(new_map, iterator->key,
                                     hash, index, iterator->value);

            aux = iterator;
            iterator = iterator->next;
            free(aux);
        }
    }

    free(old_map);
    return new_map;
}

void
hash_map_free_keys(HashMap *map) {
    for (uint32 i = 0; i < hash_map_capacity(map); i += 1) {
        Bucket *iterator = &(map->array[i]);
        while (iterator) {
            free(iterator->key);
            iterator = iterator->next;
        }
    }
    return;
}

void
hash_map_destroy(HashMap *map) {
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
    return;
}

uint32
hash_function(char *key, uint32 key_size) {
    BRN2_ASSUME_ALIGNED(key);
    uint32 hash = 5381;
    for (uint32 i = 0; i < key_size; i += 1)
        hash = ((hash << 5) + hash) + (uint32)key[i];
    return hash;
}

uint32
hash_normal(HashMap *map, uint32 hash) {
    // capacity has to be power of 2
    uint32 normal = hash & map->bitmask;
    return normal;
}

bool
hash_map_insert(HashMap *map, char *key, uint32 key_size, uint32 value) {
    uint32 hash = hash_function(key, key_size);
    uint32 index = hash_normal(map, hash);
    return hash_map_insert_pre_calc(map, key, hash, index, value);
}

bool
hash_map_insert_pre_calc(HashMap *map, char *key, uint32 hash,
				         uint32 index, uint32 value) {
    Bucket *iterator = &(map->array[index]);

    if (iterator->key == NULL) {
        iterator->key = key;
        iterator->hash = hash;
        iterator->value = value;
        map->length += 1;
        return true;
    }

    while (true) {
        if ((hash == iterator->hash) && !strcmp(iterator->key, key))
            return false;

        if (iterator->next)
            iterator = iterator->next;
        else
            break;
    }

    map->collisions += 1;
    iterator->next = xcalloc(1, sizeof(*iterator));
    iterator->next->key = key;
    iterator->next->hash = hash;
    iterator->next->value = value;
    map->length += 1;

    return true;
}

void *
hash_map_lookup(HashMap *map, char *key, uint32 key_size) {
    uint32 hash = hash_function(key, key_size);
    uint32 index = hash_normal(map, hash);
    return hash_map_lookup_pre_calc(map, key, hash, index);
}

void *
hash_map_lookup_pre_calc(HashMap *map, char *key, uint32 hash, uint32 index) {
    Bucket *iterator = &(map->array[index]);

    if (iterator->key == NULL)
        return NULL;

    while (true) {
        if ((hash == iterator->hash) && !strcmp(iterator->key, key))
            return &(iterator->value);

        if (iterator->next)
            iterator = iterator->next;
        else
            break;
    }

    return NULL;
}

bool
hash_map_remove(HashMap *map, char *key, uint32 key_size) {
    uint32 hash = hash_function(key, key_size);
    uint32 index = hash_normal(map, hash);
    return hash_map_remove_pre_calc(map, key, hash, index);
}

bool
hash_map_remove_pre_calc(HashMap *map, char *key, uint32 hash, uint32 index) {
    Bucket *iterator = &(map->array[index]);

    if (iterator->key == NULL)
        return false;

    if ((hash == iterator->hash) && !strcmp(iterator->key, key)) {
        if (iterator->next) {
            void *aux = iterator->next;
            memmove(iterator, iterator->next, sizeof(*iterator));
            free(aux);
            map->collisions -= 1;
        } else {
            memset(iterator, 0, sizeof(*iterator));
        }
        map->length -= 1;
        return true;
    }

    while (iterator->next) {
        Bucket *previous = iterator;
        iterator = iterator->next;

        if ((hash == iterator->hash) && !strcmp(iterator->key, key)) {
             previous->next = iterator->next;
             free(iterator);
             map->length -= 1;
             map->collisions -= 1;
             return true;
        }
    }

    return false;
}

void
hash_map_print_summary(HashMap *map, char *name) {
    printf("HashMap %s {\n", name);
    printf("  capacity: %u\n", map->capacity);
    printf("  length: %u\n", map->length);
    printf("  collisions: %u\n", map->collisions);
    printf("  expected collisions: %u\n", hash_map_expected_collisions(map));
    printf("}\n");
    return;
}

void
hash_map_print(HashMap *map, bool verbose) {
    HASH_MAP_PRINT_SUMMARY(map);

    for (uint32 i = 0; i < map->capacity; i += 1) {
        Bucket *iterator = &(map->array[i]);

        if (iterator->key || verbose)
            printf("\n%03u:", i);

        while (iterator && iterator->key) {
            printf(GREEN" %s=%u"RESET" ->", iterator->key, iterator->value);
            iterator = iterator->next;
        }
    }
    printf("\n");
    return;
}

uint32
hash_map_capacity(HashMap *map) {
    return map->capacity;
}

uint32
hash_map_length(HashMap *map) {
    return map->length;
}

uint32
hash_map_collisions(HashMap *map) {
    return map->collisions;
}

uint32
hash_map_expected_collisions(HashMap *map) {
    long double n = map->length;
    long double m = map->capacity;
    long double result = n - m * (1 - powl((m - 1)/m, n));
    return (uint32)(roundl(result));
}

#ifndef TESTING_THIS_FILE
#define TESTING_THIS_FILE 0
#endif

#if TESTING_THIS_FILE
#include <assert.h>

static char *
random_string(void) {
    int length = 10 + rand() % 60;
    const char characters[] = "abcdefghijklmnopqrstuvwxyz1234567890";
    char *string = xmalloc((usize)length + 1);

    for (int i = 0; i < length; i += 1) {
        int c = rand() % ((int)sizeof(characters) - 1);
        string[i] = characters[c];
    }
    string[length] = '\0';

    return string;
}

// flags: -lm
#define NSTRINGS 500000
int main(void) {
    struct timespec t0, t1;
    HashMap *original_map; 
    HashMap *balanced_map; 

    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);

    original_map = hash_map_create(NSTRINGS);
    assert(original_map);
    assert(hash_map_capacity(original_map) >= NSTRINGS);

    assert(hash_map_insert(original_map, "a", strlen("a"), 0));
    assert(!hash_map_insert(original_map, "a", strlen("a"), 1));
    assert(hash_map_insert(original_map, "b", strlen("b"), 2));

    srand((uint)t0.tv_nsec);

    for (int i = 0; i < NSTRINGS; i += 1) {
        char *key = random_string();
        uint32 value = (uint32)rand();
        assert(hash_map_insert(original_map, key, strlen(key), value));
    }

    HASH_MAP_PRINT_SUMMARY(original_map);

    {
        uint32 collisions_before = hash_map_collisions(original_map);
        uint32 expected_collisions = hash_map_expected_collisions(original_map);
        double ratio = (double)collisions_before / (double)expected_collisions;
        assert(ratio <= 1.2);
        balanced_map = hash_map_balance(original_map);

        HASH_MAP_PRINT_SUMMARY(balanced_map);
        assert(collisions_before > hash_map_collisions(balanced_map));
    }

    assert(hash_map_length(balanced_map) == (2 + NSTRINGS));
    assert(*(uint32 *)hash_map_lookup(balanced_map, "a", strlen("a")) == 0);
    assert(!hash_map_lookup(balanced_map, "c", strlen("c")));

    assert(!hash_map_remove(balanced_map, "c", strlen("c")));
    assert(hash_map_remove(balanced_map, "b", strlen("b")));

    assert(hash_map_length(balanced_map) == (1 + NSTRINGS));

    assert(hash_map_remove(balanced_map, "a", strlen("a")));

    hash_map_free_keys(balanced_map);
    hash_map_destroy(balanced_map);

    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    {
        long seconds = t1.tv_sec - t0.tv_sec;
        long nanos = t1.tv_nsec - t0.tv_nsec;
        double total_seconds = (double)seconds + (double)nanos/1.0e9;
        printf("\ntime elapsed (%s): %gs\n\n", __FILE__, total_seconds);
    }
    exit(0);
}
#endif

#endif
