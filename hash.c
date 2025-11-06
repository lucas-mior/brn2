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

#if !defined(HASH_H)
#define HASH_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rapidhash.h"
#include "util.c"

#if defined(__INCLUDE_LEVEL__) && __INCLUDE_LEVEL__ == 0
#define TESTING_hash 1
#elif !defined(TESTING_hash)
#define TESTING_hash 0
#endif

#if TESTING_hash
#define HASH_VALUE_TYPE uint32
#define HASH_TYPE map
#endif

#define SLOT_FREE   0
#define SLOT_DELETED -1

#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

#if !defined(ALIGNMENT)
#define ALIGNMENT 16
#endif

uint32 hash_function(char *key, uint32 key_length);
uint32 hash_normal(void *map, uint32 hash);
uint32 hash_capacity(void *map);
uint32 hash_length(void *map);
uint32 hash_collisions(void *map);
uint32 hash_expected_collisions(void *map);

typedef struct Hash_map HashMap;
typedef struct Hash_set HashSet;

#if !defined(INTEGERS)
#define INTEGERS
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ulonglong;

typedef long long llong;

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

#define Q(x) #x
#define QUOTE(x) Q(x)
#define HASH_PRINT_SUMMARY_map(MAP) hash_print_summary_map(MAP, QUOTE(MAP))
#define HASH_PRINT_SUMMARY_set(MAP) hash_print_summary_set(MAP, QUOTE(MAP))
#define HASH_MAP_SIZE(map)                                                     \
    (sizeof(*map) + map->capacity*sizeof(*(&map->array[0])))

#define CAT_(a, b) a##b
#define CAT(a, b) CAT_(a, b)

#endif /* HASH_H */

#if !defined(HASH_TYPE)
#error HASH_TYPE is undefined
#endif

typedef struct CAT(Bucket_, HASH_TYPE) {
    char *key;
    uint32 hash;
#if defined(HASH_VALUE_TYPE)
    HASH_VALUE_TYPE value;
#endif
#if defined(HASH_PADDING_TYPE)
    HASH_PADDING_TYPE padding;
#endif
} CAT(Bucket_, HASH_TYPE);

struct CAT(Hash_, HASH_TYPE) {
    int64 size;
    uint32 capacity;
    uint32 bitmask;
    uint32 collisions;
    uint32 length;
    CAT(Bucket_, HASH_TYPE) array[];
};

static struct CAT(Hash_, HASH_TYPE)*CAT(hash_create_, HASH_TYPE)(uint32);
static bool CAT(hash_insert_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE) *, char *,
                                         uint32
#if defined(HASH_VALUE_TYPE)
                                         ,
                                         uint32
#endif
);

static bool CAT(hash_insert_pre_calc_,
                HASH_TYPE)(struct CAT(Hash_, HASH_TYPE) *, char *, uint32,
                           uint32
#if defined(HASH_VALUE_TYPE)
                           ,
                           uint32
#endif
);

static void *CAT(hash_lookup_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE) *,
                                          char *, uint32);
static void *CAT(hash_lookup_pre_calc_,
                 HASH_TYPE)(struct CAT(Hash_, HASH_TYPE) *, char *, uint32,
                            uint32);
static bool CAT(hash_remove_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE) *, char *,
                                         uint32);
static bool CAT(hash_remove_pre_calc_,
                HASH_TYPE)(struct CAT(Hash_, HASH_TYPE) *, char *, uint32,
                           uint32);
static void CAT(hash_print_summary_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE) *,
                                                char *);
static void CAT(hash_print_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE) *, bool);

static void
CAT(hash_zero_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE)*map) {
    map->collisions = 0;
    map->length = 0;
    memset(map->array, 0, map->capacity*sizeof(*(&map->array[0])));
    return;
}

struct CAT(Hash_, HASH_TYPE)*CAT(hash_create_, HASH_TYPE)(uint32 length) {
    struct CAT(Hash_, HASH_TYPE)*map;
    int64 size;
    uint32 capacity = 1;
    uint32 power = 0;

    if (length > (UINT32_MAX / 2)) {
        length = UINT32_MAX / 2;
    }

    while (capacity < length) {
        capacity *= 2;
        power += 1;
    }
    capacity *= 2;
    power += 1;

    size = sizeof(*map) + capacity*sizeof(*(&map->array[0]));

    map = xmmap_commit(&size);
    map->capacity = capacity;
    map->bitmask = (1 << power) - 1;
    map->size = size;
    return map;
}

static void
CAT(hash_destroy_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE)*map) {
    xmunmap(map, HASH_MAP_SIZE(map));
    return;
}

static bool
CAT(hash_insert_pre_calc_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE)*map,
                                      char *key, uint32 hash, uint32 index
#if defined(HASH_VALUE_TYPE)
                                      ,
                                      HASH_VALUE_TYPE value
#endif
) {
    uint32 capacity = map->capacity;
    uint32 i = 0;
    uint32 probe = index;
    int32_t first_tombstone = -1;

    while (i < capacity) {
        CAT(Bucket_, HASH_TYPE) *iterator = &map->array[probe];

        switch ((int64)iterator->key) {
        case SLOT_FREE: {
            CAT(Bucket_, HASH_TYPE) *target = (first_tombstone >= 0)
                                                  ? &map->array[first_tombstone]
                                                  : iterator;

            target->key = key;
            target->hash = hash;
#if defined(HASH_VALUE_TYPE)
            target->value = value;
#endif
            map->length += 1;
            return true;
        }
        case SLOT_DELETED:
            /* remember first tombstone but keep scanning for duplicate */
            if (first_tombstone < 0) {
                first_tombstone = (int32_t)probe;
            }
            break;
        default:
            /* check duplicate only on truly occupied slots */
            if (iterator->hash == hash && strcmp(iterator->key, key) == 0) {
                return false; /* already present */
            }
            /* real collision: occupied by different key */
            map->collisions += 1;
        }

        i += 1;
        probe = (index + i*i) % capacity;
    }

    /* table full: if we found a tombstone, reuse iterator now */
    if (first_tombstone >= 0) {
        CAT(Bucket_, HASH_TYPE) *target = &map->array[first_tombstone];
        target->key = key;
        target->hash = hash;
#if defined(HASH_VALUE_TYPE)
        target->value = value;
#endif
        map->length += 1;
        return true;
    }

    return false;
}

static bool
CAT(hash_insert_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE)*map, char *key,
                             uint32 key_length
#if defined(HASH_VALUE_TYPE)
                             ,
                             uint32 value
#endif
) {
    uint32 hash = hash_function(key, key_length);
    uint32 index = hash_normal(map, hash);
    return CAT(hash_insert_pre_calc_, HASH_TYPE)(map, key, hash, index
#if defined(HASH_VALUE_TYPE)
                                                 ,
                                                 value
#endif
    );
}

void *
CAT(hash_lookup_pre_calc_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE)*map,
                                      char *key, uint32 hash, uint32 index) {
    uint32 capacity = map->capacity;
    uint32 i = 0;
    uint32 probe = index;

    while (i < capacity) {
        CAT(Bucket_, HASH_TYPE) *iterator = &map->array[probe];

        switch ((int64)iterator->key) {
        case SLOT_FREE:
            return NULL;
        case SLOT_DELETED:
            break;
        default:
            if (iterator->hash == hash && strcmp(iterator->key, key) == 0) {
#if defined(HASH_VALUE_TYPE)
                return &(iterator->value);
#else
                return NULL;
#endif
            }
        }

        i += 1;
        probe = (index + i*i) % capacity;
    }

    return NULL;
}

void *
CAT(hash_lookup_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE)*map, char *key,
                             uint32 key_length) {
    uint32 hash = hash_function(key, key_length);
    uint32 index = hash_normal(map, hash);
    return CAT(hash_lookup_pre_calc_, HASH_TYPE)(map, key, hash, index);
}

static bool
CAT(hash_remove_pre_calc_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE)*map,
                                      char *key, uint32 hash, uint32 index) {
    uint32 capacity = map->capacity;
    uint32 i = 0;
    uint32 probe = index;

    while (i < capacity) {
        CAT(Bucket_, HASH_TYPE) *iterator = &map->array[probe];

        switch ((int64)iterator->key) {
        case SLOT_FREE:
            return false;
        case SLOT_DELETED:
            break;
        default:
            if (iterator->hash == hash && strcmp(iterator->key, key) == 0) {
                /* mark tombstone, keep key/hash as-is (optional) */
                iterator->key = (char *)SLOT_DELETED;
                map->length -= 1;
                return true;
            }
            break;
        }

        i += 1;
        probe = (index + i*i) % capacity;
    }

    return false;
}

static bool
CAT(hash_remove_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE)*map, char *key,
                             uint32 key_length) {
    uint32 hash = hash_function(key, key_length);
    uint32 index = hash_normal(map, hash);
    return CAT(hash_remove_pre_calc_, HASH_TYPE)(map, key, hash, index);
}

static void
CAT(hash_print_summary_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE)*map,
                                    char *name) {
    printf("struct Hash%s %s {\n", QUOTE(HASH_TYPE), name);
    printf("  capacity: %u\n", map->capacity);
    printf("  length: %u\n", map->length);
    printf("  collisions: %u\n", map->collisions);
    printf("  expected collisions: %u\n", hash_expected_collisions(map));
    printf("}\n");
    return;
}

static void
CAT(hash_print_, HASH_TYPE)(struct CAT(Hash_, HASH_TYPE)*map, bool verbose) {
    CAT(HASH_PRINT_SUMMARY_, HASH_TYPE)(map);

    for (uint32 i = 0; i < map->capacity; i += 1) {
        CAT(Bucket_, HASH_TYPE) *iterator = &map->array[i];

        if (!verbose) {
            if (iterator->key == (char *)SLOT_FREE) {
                continue;
            }
            if (iterator->key == (char *)SLOT_DELETED) {
                continue;
            }
        }

        printf("\n%03u: ", i);

        switch ((int64)iterator->key) {
        case SLOT_FREE:
            printf("[empty]");
            break;
        case SLOT_DELETED:
            printf("[deleted]");
            break;
        default:
            printf("'%s'", iterator->key);
#if defined(HASH_VALUE_TYPE)
            printf("=%u", iterator->value);
#endif
        }
    }

    printf("\n");
}
#undef HASH_VALUE_TYPE
#undef HASH_PADDING_TYPE
#undef HASH_TYPE

#if !defined(HASH_H2)
#define HASH_H2

uint32
hash_function(char *key, uint32 key_length) {
    uint32 hash;
    hash = (uint32)rapidhash(key, key_length);
    return (uint32)hash;
}

uint32
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
hash_expected_collisions(void *map) {
    HashMap *map2 = map;
    long double n = map2->length;
    long double m = map2->capacity;
    long double result = n - m*(1 - powl((m - 1) / m, n));
    return (uint32)(roundl(result));
}

#endif

#if TESTING_hash

#if !OS_UNIX
#error "hash.c tests only work on unix systems"
#endif

#include <assert.h>
#include "arena.c"

#define NSTRINGS 8
#define NBYTES 2*ALIGNMENT

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

    double total_seconds = (double)seconds + (double)nanos / 1.0e9;
    double micros_per_str = 1e6*(total_seconds / (double)(NSTRINGS));
    double nanos_per_byte = 1e3*(micros_per_str / (double)(NBYTES));

    printf("\ntime elapsed %s:%s\n", __FILE__, name);
    printf("%gs = %gus per string = %gns per byte.\n\n", total_seconds,
           micros_per_str, nanos_per_byte);
    return;
}

// flags: -lm
int
main(void) {
    struct timespec t0;
    struct timespec t1;
    HashMap *original_map;
    Arena *arena;
    String str1 = {.s = "aaaaaaaaaaaaaaaa", .value = 0};
    String str2 = {.s = "bbbbbbbbbbbbbbbb", .value = 1};
    String str3 = {.s = "cccccccccccccccc", .value = 2};
    String *strings = xmalloc(NSTRINGS*sizeof(*strings));

    original_map = hash_create_map(NSTRINGS);
    arena = arena_create((usize)4096*NSTRINGS);

    assert(original_map);
    assert(hash_capacity(original_map) >= NSTRINGS);

    str1.length = (uint32)strlen(str1.s);
    str2.length = (uint32)strlen(str2.s);
    str3.length = (uint32)strlen(str3.s);

    assert(hash_insert_map(original_map, str1.s, str1.length, str1.value));
    assert(!hash_insert_map(original_map, str1.s, str1.length, 1));
    assert(hash_insert_map(original_map, str2.s, str2.length, str2.value));

    assert(hash_length(original_map) == 2);
    hash_print_map(original_map, false);

    srand(42);

    for (int i = 0; i < NSTRINGS; i += 1) {
        strings[i] = random_string(arena, NBYTES);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
    for (int i = 0; i < NSTRINGS; i += 1) {
        assert(hash_insert_map(original_map, strings[i].s, strings[i].length,
                               strings[i].value));
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    print_timings("insertion", t0, t1);

    assert(hash_remove_map(original_map, strings[0].s, strings[0].length));

    if (NSTRINGS < 10) {
        hash_print_map(original_map, true);
    } else {
        HASH_PRINT_SUMMARY_map(original_map);
    }

    assert(hash_insert_map(original_map, strings[0].s, strings[0].length,
                           strings[0].value));

    for (int i = 0; i < NSTRINGS; i += 1) {
        assert(hash_remove_map(original_map, strings[i].s, strings[i].length));
    }

    free(strings);

    exit(EXIT_SUCCESS);
}
#endif
