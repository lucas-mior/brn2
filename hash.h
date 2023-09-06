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

typedef struct HashMap HashMap;

uint32 hash_function(char *, const uint32);
bool hash_map_insert_pre_calc(HashMap *, char *, const uint32, const uint32);
bool hash_map_insert(HashMap *, char *, const uint32);
bool hash_map_remove(HashMap *, char *, const uint32);
HashMap *hash_map_create(uint32);
uint32 hash_map_capacity(HashMap *);
uint32 hash_map_length(HashMap *);
uint32 hash_map_collisions(HashMap *);
uint32 hash_map_expected_collisions(HashMap *);
void hash_map_destroy(HashMap *);
void hash_map_print(HashMap *, bool);
void hash_map_print_summary(HashMap *);
