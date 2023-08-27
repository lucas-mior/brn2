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
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>

#include "util.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define USE_HASH_TABLE_THRESHOLD 128

#define STRUCT_ARRAY_SIZE(StructType, ArrayType, array_length) \
    sizeof (StructType) + (size_t)(array_length) * sizeof (ArrayType)
#define SWAP(Type, x, y) do { Type SWAP = x; x = y; y = SWAP; } while (0)
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#pragma clang diagnostic ignored "-Wpadded"
typedef struct File {
    char name[PATH_MAX];
    FILE *stream;
    int fd;
} File;

typedef struct FileName {
    char *name;
    uint32 length;
} FileName;

typedef struct FileList {
    uint32 length;
    FileName files[];
} FileList;
