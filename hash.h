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

typedef struct HashTable HashTable;

size_t hash_function(char *, size_t);
bool hash_insert_pre_calc(HashTable *table, char *, size_t, size_t);
bool hash_insert(HashTable *, char *, size_t);
HashTable *hash_table_create(size_t);
size_t hash_table_length(HashTable *);
void hash_table_destroy(HashTable *);
