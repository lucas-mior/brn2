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

#pragma clang diagnostic ignored "-Wpadded"
typedef struct SameHash {
    char *key;
    uint32 hash;
    struct SameHash *next;
} SameHash;

struct HashTable {
    uint32 size;
    SameHash array[];
};

uint32 hash_function(char *str, uint32 length) {
    /* djb2 hash function */
    uint32 hash = 5381;
    for (uint32 i = 0; i < length; i += 1)
        hash = ((hash << 5) + hash) + (uint32) str[i];
    return hash;
}

bool hash_insert_pre_calc(HashTable *table, char *newkey, uint32 hash, uint32 hash_rest) {
    SameHash *iterator = &(table->array[hash_rest]);

    if (iterator->key == NULL) {
        iterator->key = newkey;
        iterator->hash = hash;
        return true;
    }

    do {
        if ((hash == iterator->hash) && !strcmp(iterator->key, newkey))
            return false;

        if (iterator->next)
            iterator = iterator->next;
        else
            break;
    } while (true);

    iterator->next = util_calloc(1, sizeof (*iterator));
    iterator->next->key = newkey;
    iterator->next->hash = hash;

    return true;
}

bool hash_insert(HashTable *table, char *newkey, uint32 key_length) {
    uint32 hash, hash_rest;
    SameHash *iterator;

    hash = hash_function(newkey, key_length);
    hash_rest = hash % table->size;
    iterator = &(table->array[hash_rest]);

    if (iterator->key == NULL) {
        iterator->key = newkey;
        iterator->hash = hash;
        return true;
    }

    do {
        if ((hash == iterator->hash) && !strcmp(iterator->key, newkey))
            return false;

        if (iterator->next)
            iterator = iterator->next;
        else
            break;
    } while (true);

    iterator->next = util_calloc(1, sizeof (*iterator));
    iterator->next->key = newkey;
    iterator->next->hash = hash;

    return true;
}

HashTable *hash_table_create(uint32 length) {
    HashTable *table;
    uint32 size;

    if (length > (UINT32_MAX/4))
        length = (UINT32_MAX/4);
    length *= 4;

    size = sizeof (*table) + length * sizeof (table->array[0]);

    table = util_realloc(NULL, size);
    memset(table, 0, size);
    table->size = length;
    return table;
}

uint32 hash_table_size(HashTable *table) {
    return table->size;
}

void hash_table_destroy(HashTable *table) {
    for (uint32 i = 0; i < table->size; i += 1) {
        SameHash *iterator = &(table->array[i]);
        iterator = iterator->next;
        while (iterator) {
            void *aux = iterator;
            iterator = iterator->next;
            free(aux);
        }
    }
    free(table);
}
