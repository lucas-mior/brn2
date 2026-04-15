/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023-2025 Mior, Lucas;
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the*License,
 * or (at your option) any later version.
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

#include "generic.c"

#define BRN2_PATH_MAX 4096
#define BRN2_ARENA_SIZE SIZEGB(1)
#define BRN2_MIN_PARALLEL 64

#define HASH_KEY_TYPE char
#define HASH_VALUE_TYPE int32
#define HASH_TYPE map
#include "hash.c"

#define HASH_KEY_TYPE char
#define HASH_PADDING_TYPE uint32
#define HASH_TYPE set
#include "hash.c"

#if !defined(DEBUGGING)
#define DEBUGGING 0
#endif

#if !defined(BRN2_BENCHMARK)
#define BRN2_BENCHMARK 0
#endif

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
    int32 length;
    uint32 type;
    char name[];
} FileName;

typedef struct FileList {
    Arena *arenas[BRN2_MAX_THREADS];
    uint32 *indexes;
    int64 indexes_size;
    int32 length;
    uint32 unused;
    FileName **files;
} FileList;

extern bool brn2_options_fatal;
extern bool brn2_options_implicit;
extern bool brn2_options_quiet;
extern bool brn2_options_sort;
extern bool brn2_options_autosolve;
extern bool brn2_options_vim_split;
extern int32 nthreads;

#if OS_UNIX
extern pthread_t thread_pool[BRN2_MAX_THREADS];
extern uint32 ids[BRN2_MAX_THREADS];
extern bool stop_threads;
extern pthread_cond_t brn2_new_work;
#endif

extern int (*print)(const char *, ...);

INLINE int brn2_compare(const void *, const void *);
void brn2_list_from_dir(FileList *, char *);
void brn2_list_from_file(FileList *, char *, bool);
void brn2_list_from_args(FileList *, int, char **);
void brn2_normalize_names(FileList *, FileList *);
void brn2_create_hashes(FileList *, uint32);
bool brn2_verify(FileList *, FileList *, struct Hash_set *, uint32 *);
int32 brn2_get_number_changes(FileList *, FileList *);
void brn2_free_list(FileList *);
void brn2_print_list(FileList *);
void brn2_execute2(FileList *, FileList *,
                   struct Hash_map *, struct Hash_set *,
                   int32, int32 *);

void brn2_usage(FILE *) __attribute((noreturn));

#endif
