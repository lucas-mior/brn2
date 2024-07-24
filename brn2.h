/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023-2024 Mior, Lucas; <lucasbuddemior@gmail.com>
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

#ifndef BRN2_H
#define BRN2_H

#ifdef __linux__
#define _GNU_SOURCE
#endif
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <threads.h>
#include <unistd.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

#define USE_HASH_MAP_THRESHOLD 128
#define USE_THREADS_THRESHOLD 524288

#define STRUCT_ARRAY_SIZE(Struct, ArrayType, array_length) \
    sizeof (*Struct) + (usize)(array_length) * sizeof (ArrayType)
#define SWAP(x, y) do { __typeof__(x) SWAP = x; x = y; y = SWAP; } while (0)
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(*x))

#ifndef INTEGERS
#define INTEGERS
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

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

typedef struct File {
    char name[PATH_MAX-12];
    int fd;
    FILE *stream;
} File;

typedef struct FileName {
    char *name;
    uint32 length;
    uint32 unused;
} FileName;

typedef struct FileList {
    char *map;
    uint32 map_size;
    uint32 length;
    FileName files[];
} FileList;

extern char *program;

FileList *brn2_list_from_dir(char *, bool);
FileList *brn2_list_from_lines(char *, uint32, bool);
FileList *brn2_list_from_args(int, char **, bool);
void brn2_copy_filename(FileName *, char *, uint32);
void brn2_normalize_names(FileList *);
bool brn2_verify(FileList *, FileList *);
uint32 brn2_get_number_changes(FileList *, FileList *);
uint32 brn2_execute(FileList *, FileList *, const uint32, bool);
void brn2_free_lines_list(FileList *);
void brn2_usage(FILE *) __attribute__((noreturn));
void error(char *, ...);

#endif
