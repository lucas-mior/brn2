/*
 * Copyright (C) 2025 Mior, Lucas;
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
#include <time.h>

#include "hash.h"
#include "rapidhash.h"

#include "arena.c"
#include "util.c"

#if !defined(INLINE)
#define INLINE
#endif

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
    Arena *arena; \
    usize size; \
    uint32 capacity; \
    uint32 bitmask; \
    uint32 collisions; \
    uint32 length; \
    Bucket##T array[]; \
}; \
\
void \
hash_##T##_zero(struct Hash##T *map) { \
    map->collisions = 0; \
    map->length = 0; \
    arena_reset(map->arena); \
    memset(map->array, 0, map->capacity*sizeof(*(&map->array[0]))); \
    return; \
} \
struct Hash##T * \
hash_##T##_create(uint32 length) { \
    struct Hash##T *map; \
    size_t size; \
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
    size = sizeof(*map) + capacity*sizeof(*(&map->array[0])); \
\
    map = xmmap_commit(&size); \
    map->arena = arena_alloc(capacity*sizeof(*(&map->array[0]))); \
    map->capacity = capacity; \
    map->bitmask = (1 << power) - 1; \
    map->size = size; \
    return map; \
} \
\
struct Hash##T * \
hash_##T##_balance(struct Hash##T *old_map) { \
    struct Hash##T *new_map; \
    usize size; \
    uint32 capacity; \
    uint32 bitmask; \
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
    size = sizeof(*new_map) + capacity*sizeof(*(&new_map->array[0])); \
\
    new_map = xmmap_commit(&size); \
    new_map->arena = arena_alloc(capacity*sizeof(*(&new_map->array[0]))); \
    new_map->capacity = capacity; \
    new_map->bitmask = bitmask; \
    new_map->size = size; \
\
    for (uint32 i = 0; i < old_map->capacity; i += 1) { \
        Bucket##T *iterator = &(old_map->array[i]); \
        uint32 hash; \
        uint32 index; \
\
        if (iterator->key) { \
            hash = iterator->hash; \
            index = hash_normal(new_map, hash); \
            hash_##T##_insert_pre_calc(new_map, iterator->key, \
                                       hash, index, HASH_ITERATOR_VALUE); \
        } \
\
        while (iterator->next) { \
            iterator = (void *)(old_map->arena->begin + iterator->next); \
            hash = iterator->hash; \
            index = hash_normal(new_map, hash); \
            hash_##T##_insert_pre_calc(new_map, iterator->key, \
                                       hash, index, HASH_ITERATOR_VALUE); \
\
        } \
    } \
\
    arena_destroy(old_map->arena); \
    xmunmap(old_map, HASH_MAP_SIZE(old_map)); \
    return new_map; \
} \
\
void \
hash_##T##_destroy(struct Hash##T *map) { \
    arena_destroy(map->arena); \
    xmunmap(map, HASH_MAP_SIZE(map)); \
    return; \
} \
\
INLINE bool \
hash_##T##_insert(struct Hash##T *map, \
                  char *key, uint32 key_length, uint32 value) { \
    uint32 hash = hash_function(key, key_length); \
    uint32 index = hash_normal(map, hash); \
    return hash_##T##_insert_pre_calc(map, key, hash, index, value); \
} \
\
bool \
hash_##T##_insert_pre_calc(struct Hash##T *map, char *key, \
                           uint32 hash, uint32 index, uint32 value) { \
    Bucket##T *iterator = &(map->array[index]); \
\
    if (iterator->key == NULL) { \
        iterator->key = key; \
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
            iterator = (void *)(map->arena->begin + iterator->next); \
        else \
            break; \
    } \
\
    map->collisions += 1; \
    iterator->next = arena_push_index32(map->arena, sizeof(*iterator)); \
    iterator = (void *)(map->arena->begin + iterator->next); \
    iterator->key = key; \
    iterator->hash = hash; \
    HASH_ITERATOR_VALUE_ASSIGN; \
    iterator->next = 0; \
    map->length += 1; \
\
    return true; \
} \
\
INLINE void * \
hash_##T##_lookup(struct Hash##T *map, char *key, uint32 key_length) { \
    uint32 hash = hash_function(key, key_length); \
    uint32 index = hash_normal(map, hash); \
    return hash_##T##_lookup_pre_calc(map, key, hash, index); \
} \
\
void * \
hash_##T##_lookup_pre_calc(struct Hash##T *map, char *key, \
                           uint32 hash, uint32 index) { \
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
            iterator = (void *)(map->arena->begin + iterator->next); \
        else \
            break; \
    } \
\
    return NULL; \
} \
\
INLINE bool \
hash_##T##_remove(struct Hash##T *map, char *key, uint32 key_length) { \
    uint32 hash = hash_function(key, key_length); \
    uint32 index = hash_normal(map, hash); \
    return hash_##T##_remove_pre_calc(map, key, hash, index); \
} \
\
bool \
hash_##T##_remove_pre_calc(struct Hash##T *map, char *key, \
                           uint32 hash, uint32 index) { \
    Bucket##T *iterator = &(map->array[index]); \
\
    if (iterator->key == NULL) \
        return false; \
\
    if ((hash == iterator->hash) && !strcmp(iterator->key, key)) { \
        if (iterator->next) { \
            memmove(iterator, \
                    map->arena->begin + iterator->next, sizeof(*iterator)); \
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
        iterator = (void *)(map->arena->begin + iterator->next); \
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
    printf("  expected collisions: %u\n", hash_expected_collisions(map)); \
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
            printf("'%s'=%u ->", \
                   iterator->key, HASH_ITERATOR_VALUE); \
            if (iterator->next) \
                iterator = (void *)(map->arena->begin + iterator->next); \
            else { \
                break; \
            } \
        } \
    } \
    printf("\n"); \
    return; \
} \
\

#define HASH_VALUE_FIELD uint32 value; uint32 unused;
#define HASH_ITERATOR_VALUE iterator->value
#define HASH_ITERATOR_VALUE_ASSIGN iterator->value = value
#define HASH_ITERATOR_VALUE_RETURN &(iterator->value)
HASH_IMPLEMENT(map)
#undef HASH_VALUE_FIELD
#undef HASH_ITERATOR_VALUE
#undef HASH_ITERATOR_VALUE_ASSIGN
#undef HASH_ITERATOR_VALUE_RETURN

#define HASH_VALUE_FIELD
#define HASH_ITERATOR_VALUE 0u
#define HASH_ITERATOR_VALUE_ASSIGN (void)value
#define HASH_ITERATOR_VALUE_RETURN NULL
HASH_IMPLEMENT(set)
#undef HASH_VALUE_FIELD
#undef HASH_ITERATOR_VALUE
#undef HASH_ITERATOR_VALUE_ASSIGN
#undef HASH_ITERATOR_VALUE_RETURN

INLINE uint32
hash_function(char *key, uint32 key_length) {
    uint32 hash;
    hash = (uint32)rapidhash(key, key_length);
    return (uint32)hash;
}

INLINE uint32
hash_normal(void *map, uint32 hash) {
    HashMap *map2 = map;
    uint32 normal = hash & map2->bitmask;
    return normal;
}

uint32
hash_capacity(void *map) {
    HashMap *map2 = map;
    return map2->capacity;
}

uint32
hash_length(void *map) {
    HashMap *map2 = map;
    return map2->length;
}

uint32
hash_collisions(void *map) {
    HashMap *map2 = map;
    return map2->collisions;
}

uint32
hash_expected_collisions(void *map) { \
    HashMap *map2 = map;
    long double n = map2->length; \
    long double m = map2->capacity; \
    long double result = n - m * (1 - powl((m - 1)/m, n)); \
    return (uint32)(roundl(result)); \
}

#define hash_set_insert(a, b) \
        hash_set_insert(a, b, 0)
#define hash_set_insert_pre_calc(a, b, c, d) \
        hash_set_insert_pre_calc(a, b, c, d, 0)

#ifdef TESTING_hash
#include <assert.h>

#define NSTRINGS 500000
#define NBYTES 10*ALIGNMENT

typedef struct String {
    char *s;
    uint32 length;
    uint32 value;
} String;

static String
random_string(Arena *arena, uint32 nbytes) {
    const char characters[] = "abcdefghijklmnopqrstuvwxyz1234567890";
    String string;
    uint32 size;
    uint32 length;

    length = nbytes + (uint32)rand() % 16u;
    size = length + 1;
    string.s = arena_push(arena, size);

    for (uint32 i = 0; i < length; i += 1) {
        uint32 c = (uint32)rand() % ((int)sizeof(characters) - 1);
        string.s[i] = characters[c];
    }
    string.s[length] = '\0';
    string.length = length;
    string.value = (uint32)rand();

    return string;
}

static void
print_timings(char *name, struct timespec t0, struct timespec t1) {
    long seconds = t1.tv_sec - t0.tv_sec;
    long nanos = t1.tv_nsec - t0.tv_nsec;

    double total_seconds = (double)seconds + (double)nanos/1.0e9;
    double micros_per_str = 1e6*(total_seconds/(double)(NSTRINGS));
    double nanos_per_byte = 1e3*(micros_per_str/(double)(NBYTES));

    printf("\ntime elapsed %s:%s\n", __FILE__, name);
    printf("%gs = %gus per string = %gns per byte.\n\n",
           total_seconds, micros_per_str, nanos_per_byte);
    return;
}

// flags: -lm
int main(void) {
    struct timespec t0;
    struct timespec t1;
    HashMap *original_map;
    HashMap *balanced_map;
    Arena *arena;
    String str1 = { .s = "aaaaaaaaaaaaaaaa", .value = 0};
    String str2 = { .s = "bbbbbbbbbbbbbbbb", .value = 1};
    String str3 = { .s = "cccccccccccccccc", .value = 2};
    String *strings = xmalloc(NSTRINGS*sizeof(*strings));

    original_map = hash_map_create(NSTRINGS);
    arena = arena_alloc((usize)4096*NSTRINGS);

    assert(original_map);
    assert(hash_capacity(original_map) >= NSTRINGS);

    str1.length = (uint32)strlen(str1.s);
    str2.length = (uint32)strlen(str2.s);
    str3.length = (uint32)strlen(str3.s);

    assert(hash_map_insert(original_map, str1.s, str1.length, str1.value));
    assert(!hash_map_insert(original_map, str1.s, str1.length, 1));
    assert(hash_map_insert(original_map, str2.s, str2.length, str2.value));

    assert(hash_length(original_map) == 2);
    hash_map_print(original_map, false);

    srand(42);

    for (int i = 0; i < NSTRINGS; i += 1)
        strings[i] = random_string(arena, NBYTES);

    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
    for (int i = 0; i < NSTRINGS; i += 1) {
        assert(hash_map_insert(original_map,
                               strings[i].s,
                               strings[i].length,
                               strings[i].value));
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    print_timings("insertion", t0, t1);

    if (NSTRINGS < 10)
        hash_map_print(original_map, false);
    else
        HASH_map_PRINT_SUMMARY(original_map);

    {
        struct timespec tbalance0;
        struct timespec tbalance1;
        uint32 collisions_before = hash_collisions(original_map);
        uint32 expected_collisions = hash_expected_collisions(original_map);
        double ratio = (double)collisions_before / (double)expected_collisions;
        assert(ratio <= 1.2);

        clock_gettime(CLOCK_MONOTONIC_RAW, &tbalance0);
        balanced_map = hash_map_balance(original_map);
        clock_gettime(CLOCK_MONOTONIC_RAW, &tbalance1);
        print_timings("balancing", tbalance0, tbalance1);

        if (NSTRINGS < 10)
            hash_map_print(balanced_map, false);
        else
            HASH_map_PRINT_SUMMARY(balanced_map);

        if (collisions_before > 10)
            assert(collisions_before > hash_collisions(balanced_map));
    }

    assert(hash_length(balanced_map) == (2 + NSTRINGS));
    {
        uint32 *value = hash_map_lookup(balanced_map, str1.s, str1.length);
        assert(*value == 0);
    }
    assert(!hash_map_lookup(balanced_map, str3.s, str3.length));

    assert(!hash_map_remove(balanced_map, str3.s, str3.length));
    assert(hash_map_remove(balanced_map, str2.s, str2.length));

    assert(hash_length(balanced_map) == (1 + NSTRINGS));

    assert(hash_map_remove(balanced_map, str1.s, str1.length));

    hash_map_destroy(balanced_map);
    free(strings);

    exit(EXIT_SUCCESS);
}
#endif

#endif
