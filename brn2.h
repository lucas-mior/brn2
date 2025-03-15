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
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <threads.h>
#include <unistd.h>
#include "hash.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

#define USE_HASH_MAP_THRESHOLD 128
#define USE_THREADS_THRESHOLD 524288
#define MAX_THREADS 64
#define MEMCHR_BYTES 16
#define MIN(a,b) (a) < (b) ? (a) : (b)
#define MAX(a,b) (a) > (b) ? (a) : (b)
#define ALIGN(x) (x)

#ifndef BRN2_DEBUG
#define BRN2_DEBUG 0
#endif

#define STRUCT_ARRAY_SIZE(struct_object, ArrayType, array_length) \
    (sizeof(*struct_object) + (usize)(array_length)*sizeof(ArrayType))
#define SWAP(x, y) do { __typeof__(x) SWAP = x; x = y; y = SWAP; } while (0)
#define ARRAY_LENGTH(ARRAY_LENGTH) \
    (sizeof(ARRAY_LENGTH) / sizeof(*ARRAY_LENGTH))

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
    uint32 hash;
    uint16 length;
    bool dir;
    uint8 unused;
} FileName;

typedef struct Arena {
    void *begin;
    void *pos;
    size_t size;
} Arena;

typedef struct FileList {
    uint32 length;
    Arena *arena;
    FileName files[];
} FileList;

typedef struct Hash {
    uint32 mod;
} Hash;

extern char *program;
extern bool brn2_fatal;
extern bool brn2_implict;
extern bool brn2_quiet;
extern bool brn2_sort;
extern uint32 nthreads;

extern Arena *arena_old;
extern Arena *arena_new;

int brn2_compare(const void *, const void *);
FileList *brn2_list_from_dir_recurse(char *);
FileList *brn2_list_from_dir(char *);
FileList *brn2_list_from_lines(char *, uint32);
FileList *brn2_list_from_args(int, char **);
void brn2_normalize_names(FileList *, FileList *);
Hash *brn2_create_hashes(FileList *, uint32);
bool brn2_verify(FileList *, FileList *, HashMap *, Hash *);
uint32 brn2_get_number_changes(FileList *, FileList *);
uint32 brn2_execute(FileList *, FileList *, HashMap *, Hash *, Hash *);
void brn2_slash_add(FileName *file);
void brn2_free_list(FileList *list);

void brn2_usage(FILE *) __attribute__((noreturn));
void error(char *, ...);

#endif
