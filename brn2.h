/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/syscall.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define USE_HASH_TABLE_THRESHOLD 128

#define STRUCT_ARRAY_SIZE(StructType, ArrayType, array_length) \
    sizeof (StructType) + (size_t)(array_length)*sizeof (ArrayType)
#define SWAP(x, y) do { typeof(x) aux = x; x = y; y = aux; } while (0)

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef struct File {
    char name[PATH_MAX];
    int fd;
} File;

typedef struct FileName {
    char *name;
    size_t length;
} FileName;

typedef struct FileList {
    size_t length;
    FileName files[];
} FileList;

typedef struct SameHash {
    char *key;
    size_t hash;
    struct SameHash *next;
} SameHash;

typedef struct HashTable {
    size_t length;
    SameHash array[];
} HashTable;

void *util_realloc(void *, size_t);
void *util_calloc(size_t, size_t);
void util_command(char **);

bool hash_insert(HashTable *, char *, size_t);
HashTable *hash_table_create(size_t);
void hash_table_destroy(HashTable *);
