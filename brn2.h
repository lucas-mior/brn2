/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023-2025 Mior, Lucas; 
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

#if !defined(BRN2_H)
#define BRN2_H

#if defined(__WIN32__)
  #define BRN2_MAX_THREADS 1
  #include <windows.h>
#else
  #define BRN2_MAX_THREADS 64
  #include <pthread.h>
  #include <sys/mman.h>
  #include <sys/wait.h>
#endif

#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "generic.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

#define BRN2_PATH_MAX 4096u
#define BRN2_ARENA_SIZE SIZEGB(1)
#define BRN2_MIN_PARALLEL 64
#define BRN2_ALIGNMENT 16ul
#define ALIGNMENT BRN2_ALIGNMENT
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define ALIGN(x) BRN2_ALIGN(x, BRN2_ALIGNMENT)
#define BRN2_ALIGN(S, A) (((S) + ((A) - 1)) & ~((A) - 1))

#define HASH_VALUE_TYPE uint32
#define HASH_PADDING_TYPE uint32
#define HASH_TYPE map
#include "hash.c"

#define HASH_TYPE set
#include "hash.c"

#if defined(__GNUC__)
# define BRN2_ASSUME_ALIGNED(X) do { \
    X = __builtin_assume_aligned(X, BRN2_ALIGNMENT); \
    } while (0)
#else
#define BRN2_ASSUME_ALIGNED(X) do {} while (0)
#endif

#if !defined(DEBUGGING)
#define DEBUGGING 0
#endif

#if !defined(BRN2_BENCHMARK)
#define BRN2_BENCHMARK 0
#endif

#if 0
#define BRN2_INLINE __attribute__((noinline))
#else
#define BRN2_INLINE static inline __attribute__((always_inline))
#endif

#if !defined(INLINE)
#define INLINE BRN2_INLINE
#endif

#define STRUCT_ARRAY_SIZE(struct_object, ArrayType, array_length) \
    (uint32)(SIZEOF(*(struct_object)) + (array_length*SIZEOF(ArrayType)))

#define SWAP(x, y) do { __typeof__(x) SWAP = x; x = y; y = SWAP; } while (0)

#include "arena.c"

typedef struct File {
    char name[124];
    int fd;
    FILE *stream;
} File;

enum {
    TYPE_DIR = 0,
    TYPE_FILE = 1,
    TYPE_ERR = 2,
};

typedef struct FileName {
    uint64 hash;
    uint32 length;
    uint32 type;
    char name[];
} FileName;

typedef struct FileList {
    Arena *arenas[BRN2_MAX_THREADS];
    uint32 *indexes;
    int64 indexes_size;
    uint32 length;
    uint32 unused;
    FileName **files;
} FileList;

extern bool brn2_options_fatal;
extern bool brn2_options_implicit;
extern bool brn2_options_quiet;
extern bool brn2_options_sort;
extern bool brn2_options_autosolve;
extern uint32 nthreads;

#if OS_UNIX
extern pthread_t thread_pool[BRN2_MAX_THREADS];
extern uint32 ids[BRN2_MAX_THREADS];
extern bool stop_threads;
extern pthread_cond_t brn2_new_work;
#endif

extern int (*print)(const char *, ...);

int brn2_compare(const void *, const void *);
void brn2_list_from_dir(FileList *, char *);
void brn2_list_from_file(FileList *, char *, bool);
void brn2_list_from_args(FileList *, int, char **);
void brn2_normalize_names(FileList *, FileList *);
void brn2_create_hashes(FileList *, uint32);
bool brn2_verify(FileList *, FileList *, HashSet *, uint32 *);
uint32 brn2_get_number_changes(FileList *, FileList *);
void brn2_free_list(FileList *);
void brn2_timings(char *, struct timespec, struct timespec, uint32);
void brn2_print_list(FileList *);
void brn2_execute2(FileList *, FileList *,
                   HashMap *, HashSet *,
                   uint32, uint32 *);

void brn2_usage(FILE *) __attribute__((noreturn));

#endif
