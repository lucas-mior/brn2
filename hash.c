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

#include "util.h"
#include "hash.h"

typedef struct SameHash {
    char *key;
    size_t hash;
    struct SameHash *next;
} SameHash;

struct HashTable {
    size_t length;
    SameHash array[];
};


size_t hash_function(char *str, size_t length) {
    /* djb2 hash function */
    size_t hash = 5381;
    for (size_t i = 0; i < length; i += 1)
        hash = ((hash << 5) + hash) + (size_t) str[i];
    return hash;
}

bool hash_insert_pre_calc(HashTable *table, char *newkey, size_t hash, size_t hash_rest) {
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

bool hash_insert(HashTable *table, char *newkey, size_t length) {
    size_t hash, hash_rest;
    SameHash *iterator;

    hash = hash_function(newkey, length);
    hash_rest = hash % table->length;
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

HashTable *hash_table_create(size_t length) {
    HashTable *table;
    size_t size;

    if (length > (SIZE_MAX/4))
        length = (SIZE_MAX/4);
    length *= 4;

    size = sizeof (*table) + length * sizeof (table->array[0]);

    table = util_realloc(NULL, size);
    memset(table, 0, size);
    table->length = length;
    return table;
}

size_t hash_table_length(HashTable *table) {
    return table->length;
}


void hash_table_destroy(HashTable *table) {
    for (size_t i = 0; i < table->length; i += 1) {
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
