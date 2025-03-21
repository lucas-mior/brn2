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
#include <sys/mman.h>

#include "hash.h"
#include "rapidhash.h"

#pragma push_macro("TESTING_THIS_FILE")
#define TESTING_THIS_FILE 0

#include "util.c"
#include "arena.c"

#pragma pop_macro("TESTING_THIS_FILE")

#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

#define HASH_IMPLEMENT(T) \
typedef struct Bucket##T { \
    char *key; \
    uint32 hash; \
    HASH_VALUE_FIELD \
    uint32 next; \
} Bucket##T; \
\
struct Hash##T { \
    uint32 capacity; \
    uint32 bitmask; \
    uint32 collisions; \
    uint32 length; \
    Arena *arena; \
    Bucket##T array[]; \
}; \
\
struct Hash##T * \
hash_##T##_create(uint32 length) { \
    struct Hash##T *map; \
    uint64 size; \
    uint32 capacity = 1; \
    uint32 power = 0; \
\
    if (length > (UINT32_MAX/2)) \
        length = UINT32_MAX/2; \
\
    while (capacity < length) { \
        capacity *= 2; \
        power += 1; \
    } \
    capacity *= 2; \
    power += 1; \
\
    size = sizeof(*map) + capacity*sizeof(map->array[0]); \
\
    map = xmmap(size); \
    map->arena = arena_alloc(capacity*sizeof(*(&map->array[0]))); \
    arena_push(map->arena, BRN2_ALIGNMENT); \
    map->capacity = capacity; \
    map->bitmask = (1 << power) - 1; \
    return map; \
} \
\
struct Hash##T * \
hash_##T##_balance(struct Hash##T *old_map) { \
    struct Hash##T *new_map; \
    usize size; \
    uint32 capacity; \
    uint32 bitmask; \
    usize old_size; \
\
    if (old_map->capacity < (UINT32_MAX/2)) { \
        capacity = old_map->capacity*2; \
        bitmask = (old_map->bitmask << 1) + 1; \
    } else if (old_map->capacity >= UINT32_MAX) { \
        fprintf(stderr, "Error balancing hash map. Too big table.\n"); \
        return old_map; \
    } else { \
        capacity = UINT32_MAX; \
        bitmask = UINT32_MAX; \
    } \
\
    size = sizeof(*new_map) + capacity*sizeof(new_map->array[0]); \
\
    new_map = xmmap(size); \
    new_map->arena = arena_alloc(capacity*sizeof(*(&new_map->array[0]))); \
    arena_push(new_map->arena, BRN2_ALIGNMENT); \
    new_map->capacity = capacity; \
    new_map->bitmask = bitmask; \
\
    for (uint32 i = 0; i < old_map->capacity; i += 1) { \
        Bucket##T *iterator = &(old_map->array[i]); \
\
        if (iterator->key) { \
            uint32 hash = iterator->hash; \
            uint32 index = hash_##T##_normal(new_map, hash); \
            hash_##T##_insert_pre_calc(new_map, iterator->key, \
                                     hash, index, HASH_ITERATOR_VALUE); \
        } \
\
        while (iterator->next) { \
            iterator = &(old_map->arena->begin[iterator->next]); \
            uint32 hash = iterator->hash; \
            uint32 index = hash_##T##_normal(new_map, hash); \
            hash_##T##_insert_pre_calc(new_map, iterator->key, \
                                     hash, index, HASH_ITERATOR_VALUE); \
\
        } \
    } \
\
    old_size = sizeof(*old_map) + old_map->capacity*sizeof(old_map->array[0]); \
    arena_destroy(old_map->arena); \
    xmunmap(old_map, old_size); \
    return new_map; \
} \
\
void \
hash_##T##_destroy(struct Hash##T *map) { \
    usize size = sizeof(*map) + map->capacity*sizeof(map->array[0]); \
    arena_destroy(map->arena); \
    xmunmap(map, size); \
    return; \
} \
\
uint32 BRN2_INLINE \
hash_##T##_function(char *key, uint32 key_size) { \
    uint32 hash; \
    hash = rapidhash(key, key_size); \
    return (uint32)hash; \
} \
\
uint32 \
hash_##T##_normal(struct Hash##T *map, uint32 hash) { \
    uint32 normal = hash & map->bitmask; \
    return normal; \
} \
\
bool \
hash_##T##_insert(struct Hash##T *map, char *key, uint32 key_size, uint32 value) { \
    uint32 hash = hash_##T##_function(key, key_size); \
    uint32 index = hash_##T##_normal(map, hash); \
    return hash_##T##_insert_pre_calc(map, key, hash, index, value); \
} \
\
bool \
hash_##T##_insert_pre_calc(struct Hash##T *map, char *key, uint32 hash, \
				         uint32 index, uint32 value) { \
    Bucket##T *iterator = &(map->array[index]); \
\
    if (iterator->key == NULL) { \
        iterator->key = key; \
        printf("inserted %s\n", iterator->key); \
        iterator->hash = hash; \
        iterator->next = 0; \
        HASH_ITERATOR_VALUE_ASSIGN; \
        map->length += 1; \
        return true; \
    } \
\
    while (true) { \
        if ((hash == iterator->hash) && !strcmp(iterator->key, key)) \
            return false; \
\
        if (iterator->next) \
            iterator = &(map->arena->begin[iterator->next]); \
        else \
            break; \
    } \
\
    map->collisions += 1; \
    iterator->next = arena_push_index(map->arena, sizeof(*iterator)); \
    uint32 aux = iterator->next; \
    iterator = &(map->arena->begin[iterator->next]); \
    iterator->key = key; \
    printf("inserted %s @ %u\n", iterator->key, aux); \
    iterator->hash = hash; \
    HASH_ITERATOR_VALUE_ASSIGN; \
    iterator->next = 0; \
    map->length += 1; \
\
    return true; \
} \
\
void * \
hash_##T##_lookup(struct Hash##T *map, char *key, uint32 key_size) { \
    uint32 hash = hash_##T##_function(key, key_size); \
    uint32 index = hash_##T##_normal(map, hash); \
    return hash_##T##_lookup_pre_calc(map, key, hash, index); \
} \
\
void * \
hash_##T##_lookup_pre_calc(struct Hash##T *map, char *key, uint32 hash, uint32 index) { \
    Bucket##T *iterator = &(map->array[index]); \
\
    if (iterator->key == NULL) \
        return NULL; \
\
    while (true) { \
        if ((hash == iterator->hash) && !strcmp(iterator->key, key)) \
            return HASH_ITERATOR_VALUE_RETURN; \
\
        if (iterator->next) \
            iterator = &(map->arena->begin[iterator->next]); \
        else \
            break; \
    } \
\
    return NULL; \
} \
\
bool \
hash_##T##_remove(struct Hash##T *map, char *key, uint32 key_size) { \
    uint32 hash = hash_##T##_function(key, key_size); \
    uint32 index = hash_##T##_normal(map, hash); \
    return hash_##T##_remove_pre_calc(map, key, hash, index); \
} \
\
bool \
hash_##T##_remove_pre_calc(struct Hash##T *map, char *key, uint32 hash, uint32 index) { \
    Bucket##T *iterator = &(map->array[index]); \
\
    if (iterator->key == NULL) \
        return false; \
\
    if ((hash == iterator->hash) && !strcmp(iterator->key, key)) { \
        if (iterator->next) { \
            memmove(iterator, &(map->arena->begin[iterator->next]), sizeof(*iterator)); \
            map->collisions -= 1; \
        } else { \
            memset(iterator, 0, sizeof(*iterator)); \
        } \
        map->length -= 1; \
        return true; \
    } \
\
    while (iterator->next) { \
        Bucket##T *previous = iterator; \
        iterator = &(map->arena->begin[iterator->next]); \
\
        if ((hash == iterator->hash) && !strcmp(iterator->key, key)) { \
             previous->next = iterator->next; \
             map->length -= 1; \
             map->collisions -= 1; \
             return true; \
        } \
    } \
\
    return false; \
} \
\
void \
hash_##T##_print_summary(struct Hash##T *map, char *name) { \
    printf("struct Hash%s %s {\n", QUOTE(T), name); \
    printf("  capacity: %u\n", map->capacity); \
    printf("  length: %u\n", map->length); \
    printf("  collisions: %u\n", map->collisions); \
    printf("  expected collisions: %u\n", hash_##T##_expected_collisions(map)); \
    printf("}\n"); \
    return; \
} \
\
void \
hash_##T##_print(struct Hash##T *map, bool verbose) { \
    HASH_##T##_PRINT_SUMMARY(map); \
\
    for (uint32 i = 0; i < map->capacity; i += 1) { \
        Bucket##T *iterator = &(map->array[i]); \
\
        if (iterator->key || verbose) \
            printf("\n%03u:", i); \
\
        while (iterator->key) { \
            printf(RED"'%s'"RESET"=%u ->", iterator->key, HASH_ITERATOR_VALUE); \
            if (iterator->next) \
                iterator = &(map->arena->begin[iterator->next]); \
            else { \
                break; \
            } \
        } \
    } \
    printf("\n"); \
    return; \
} \
\
uint32 \
hash_##T##_capacity(struct Hash##T *map) { \
    return map->capacity; \
} \
\
uint32 \
hash_##T##_length(struct Hash##T *map) { \
    return map->length; \
} \
\
uint32 \
hash_##T##_collisions(struct Hash##T *map) { \
    return map->collisions; \
} \
\
uint32 \
hash_##T##_expected_collisions(struct Hash##T *map) { \
    long double n = map->length; \
    long double m = map->capacity; \
    long double result = n - m * (1 - powl((m - 1)/m, n)); \
    return (uint32)(roundl(result)); \
} \

#define HASH_VALUE_FIELD uint32 value;
#define HASH_ITERATOR_VALUE iterator->value
#define HASH_ITERATOR_VALUE_ASSIGN iterator->value = value
#define HASH_ITERATOR_VALUE_RETURN &(iterator->value)
HASH_IMPLEMENT(map)
#undef HASH_VALUE_FIELD
#undef HASH_ITERATOR_VALUE
#undef HASH_ITERATOR_VALUE_ASSIGN
#undef HASH_ITERATOR_VALUE_RETURN

#define HASH_VALUE_FIELD
#define HASH_ITERATOR_VALUE 0
#define HASH_ITERATOR_VALUE_ASSIGN (void)value
#define HASH_ITERATOR_VALUE_RETURN NULL
HASH_IMPLEMENT(set)
#undef HASH_VALUE_FIELD
#undef HASH_ITERATOR_VALUE
#undef HASH_ITERATOR_VALUE_ASSIGN
#undef HASH_ITERATOR_VALUE_RETURN

#define hash_set_create(a)                   hash_set_create(a)
#define hash_set_balance(a)                  hash_set_balance(a)
#define hash_set_free_keys(a)                hash_set_free_keys(a)
#define hash_set_destroy(a)                  hash_set_destroy(a)
#define hash_set_insert(a, b)                hash_set_insert(a, b, 0)
#define hash_set_insert_pre_calc(a, b, c, d) hash_set_insert_pre_calc(a, b, c, d, 0)
#define hash_set_lookup(a, b)                hash_set_lookup(a, b)
#define hash_set_lookup_pre_calc(a, b, c, d) hash_set_lookup_pre_calc(a, b, c, d)
#define hash_set_remove(a, b)                hash_set_remove(a, b)
#define hash_set_remove_pre_calc(a, b, c, d) hash_set_remove_pre_calc(a, b, c, d)
#define hash_set_print_summary(a)            hash_set_print_summary(a)
#define hash_set_print(a, b)                 hash_set_print(a, b)
#define hash_set_capacity(a)                 hash_set_capacity(a)
#define hash_set_length(a)                   hash_set_length(a)
#define hash_set_collisions(a)               hash_set_collisions(a)
#define hash_set_expected_collisions(a)      hash_set_expected_collisions(a)
#define hash_set_normal(a, b)                hash_set_normal(a, b)
#define hash_set_function(a, b)              hash_set_function(a, b)


#ifndef TESTING_THIS_FILE
#define TESTING_THIS_FILE 0
#endif

#if TESTING_THIS_FILE
#include <assert.h>

#define NSTRINGS 5
#define NBYTES BRN2_ALIGNMENT

static char *
random_string(Arena *arena) {
    int length = NBYTES + rand() % BRN2_ALIGNMENT;
    int size = ALIGN(length + 1);
    const char allowed[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "abcdefghijklmnopqrstuvwxyz"
                           "!@#$%&*()[]-=_+<>,"
                           "0123456789";
    char *string = arena_push(arena, size);

    for (int i = 0; i < length; i += 1) {
        int c = rand() % ((int)sizeof(allowed) - 1);
        string[i] = allowed[c];
    }
    string[length] = '\0';

    return string;
}

// flags: -lm
int main(void) {
    struct timespec t0, t1;
    HashMap *original_map; 
    HashMap *balanced_map; 

    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);

    original_map = hash_map_create(NSTRINGS);
    Arena *arena = arena_alloc((usize)4096*NSTRINGS);
    arena_push(arena, BRN2_ALIGNMENT); // in order to set [0] as invalid
    assert(original_map);
    assert(hash_map_capacity(original_map) >= NSTRINGS);

    char *string1 = "aaaaaaaaaaaaaaaa";
    char *string2 = "bbbbbbbbbbbbbbbb";
    char *string3 = "cccccccccccccccc";

    assert(hash_map_insert(original_map, string1, strlen(string1), 0));
    assert(!hash_map_insert(original_map, string1, strlen(string1), 1));
    assert(hash_map_insert(original_map, string2, strlen(string2), 2));

    assert(hash_map_length(original_map) == 2);
    hash_map_print(original_map, true);

    srand(42);

    for (int i = 0; i < NSTRINGS; i += 1) {
        char *key = random_string(arena);
        uint32 value = (uint32)rand();
        assert(hash_map_insert(original_map, key, strlen(key), value));
    }

    if (NSTRINGS < 10)
        hash_map_print(original_map, true);
    else
        HASH_map_PRINT_SUMMARY(original_map);

    {
        uint32 collisions_before = hash_map_collisions(original_map);
        uint32 expected_collisions = hash_map_expected_collisions(original_map);
        double ratio = (double)collisions_before / (double)expected_collisions;
        assert(ratio <= 1.2);
        balanced_map = hash_map_balance(original_map);

        if (NSTRINGS < 10)
            hash_map_print(balanced_map, true);
        else
            HASH_map_PRINT_SUMMARY(balanced_map);

        assert(collisions_before > hash_map_collisions(balanced_map));
    }

    assert(hash_map_length(balanced_map) == (2 + NSTRINGS));
    uint32 *value = hash_map_lookup(balanced_map, string1, strlen(string1));
    assert(*value == 0);
    assert(!hash_map_lookup(balanced_map, string3, strlen(string3)));

    assert(!hash_map_remove(balanced_map, string3, strlen(string3)));
    assert(hash_map_remove(balanced_map, string2, strlen(string2)));

    assert(hash_map_length(balanced_map) == (1 + NSTRINGS));

    assert(hash_map_remove(balanced_map, string1, strlen(string1)));

    hash_map_destroy(balanced_map);

    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    {
        long seconds = t1.tv_sec - t0.tv_sec;
        long nanos = t1.tv_nsec - t0.tv_nsec;

        double total_seconds = (double)seconds + (double)nanos/1.0e9;
        double micros_per_str = 1e6*(total_seconds/(double)(NSTRINGS));
        double nanos_per_byte = 1e3*(micros_per_str/(double)(NBYTES));

        printf("\ntime elapsed (%s):\n", __FILE__);
        printf("%gs = %gus per string = %gns per byte\n\n",
               total_seconds, micros_per_str, nanos_per_byte);
    }
    exit(0);
}
#endif

#endif
