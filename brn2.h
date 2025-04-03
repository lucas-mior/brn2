/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023-2024 Mior, Lucas; 
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
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <threads.h>
#include <unistd.h>

#include "hash.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

#define BRN2_MAX_THREADS 64
#define BRN2_ALIGNMENT 16
#define MIN(a,b) (a) < (b) ? (a) : (b)
#define MAX(a,b) (a) > (b) ? (a) : (b)
#define ALIGN(x) BRN2_ALIGN(x, BRN2_ALIGNMENT)
#define BRN2_ALIGN(x,alignment) (((x) + ((alignment) - 1)) & ~(alignment - 1))
#define SIZE2MB (2*1024*1024)

#ifdef __GNUC__
# define BRN2_ASSUME_ALIGNED(BRN2_ASSUME_ALIGNED) do { \
    BRN2_ASSUME_ALIGNED \
    = __builtin_assume_aligned(BRN2_ASSUME_ALIGNED, BRN2_ALIGNMENT); \
    } while (0)
#else
#define BRN2_ASSUME_ALIGNED(BRN2_ASSUME_ALIGNED) do {} while (0)
#endif

#ifndef BRN2_DEBUG
#define BRN2_DEBUG 0
#endif

#if 0
#define BRN2_INLINE __attribute__((noinline))
#else
#define BRN2_INLINE inline __attribute__((always_inline))
#endif

#define SNPRINTF(BUFFER, FORMAT, ...) \
    snprintf2(BUFFER, sizeof(BUFFER), FORMAT, __VA_ARGS__)

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

typedef struct Arena {
    char *begin;
    void *pos;
    size_t size;
} Arena;

Arena *arena_alloc(size_t);
void *arena_push(Arena *, uint32);
void *arena_reset(Arena *);
void *arena_reset_zero(Arena *);
void arena_destroy(Arena *);

typedef struct File {
    char name[20];
    int fd;
    FILE *stream;
} File;

enum {
    TYPE_DIR = 0,
    TYPE_FILE = 1,
    TYPE_ERR = 2,
};

typedef struct FileName {
    char *name;
    uint32 hash;
    uint16 length;
    uint8 type;
    uint8 unused;
} FileName;

typedef struct FileList {
    uint32 length;
    uint32 unused;
    Arena *arena;
    FileName files[];
} FileList;

extern char *program;
extern bool brn2_options_fatal;
extern bool brn2_options_implicit;
extern bool brn2_options_quiet;
extern bool brn2_options_sort;
extern uint32 nthreads;

extern Arena *arena_old;
extern Arena *arena_new;

int brn2_compare(const void *, const void *);
FileList *brn2_list_from_dir(char *);
FileList *brn2_list_from_dir_recurse(char *);
FileList *brn2_list_from_lines(char *, bool);
FileList *brn2_list_from_args(int, char **);
void brn2_normalize_names(FileList *, FileList *);
uint32 *brn2_create_hashes(FileList *, uint32 *, uint32);
bool brn2_verify(FileList *, HashMap *, uint32 *);
uint32 brn2_get_number_changes(FileList *, FileList *);
uint32 brn2_execute(FileList *, FileList *, HashMap *, uint32 *, uint32 *);
void brn2_slash_add(FileName *);
void brn2_free_list(FileList *);

void brn2_usage(FILE *) __attribute__((noreturn));
void error(char *, ...);

void *xmalloc(const usize);
void *xmmap(usize size);
void xmunmap(void *, usize);
void *xrealloc(void *, const usize);
void *xcalloc(const usize, const usize);
char *xstrdup(char *);
void *xmemdup(void *, usize);
void *snprintf2(char *, size_t, char *, ...);
void util_command(const int, char **);
void error(char *, ...);

Arena *arena_alloc(size_t);
void *arena_push(Arena *, uint32);
uint32 arena_push_index(Arena *, uint32);
void *arena_reset(Arena *);
void *arena_reset_zero(Arena *);
void arena_destroy(Arena *);

#endif
