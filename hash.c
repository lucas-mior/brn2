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

#include "brn2.h"

size_t hash_function(char *str) {
    /* djb2 hash function */
    size_t hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

bool hash_insert(SameHash *sh, size_t h, char *newkey) {
    SameHash *iterator = &sh[h];

    if (iterator->key == NULL) {
        iterator->key = newkey;
        return false;
    }

    do {
        if (!strcmp(iterator->key, newkey))
            return true;

        if (iterator->next)
            iterator = iterator->next;
        else
            break;
    } while (true);

    iterator->next = util_calloc(1, sizeof (SameHash));
    iterator->next->key = newkey;

    return false;
}

void hash_free(SameHash *table, size_t length) {
    for (size_t i = 0; i < length; i += 1) {
        SameHash *iterator = &table[i];
        iterator = iterator->next;
        while (iterator) {
            void *aux = iterator;
            iterator = iterator->next;
            free(aux);
        }
    }
    free(table);
}