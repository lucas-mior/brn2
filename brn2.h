// SPDX-License-Identifier: AGPL
// Copyright (c) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
// Copyright (c) 2026 Mior, Lucas

#if !defined(BRN2_H)
#define BRN2_H

#include "cbase.h"

#if defined(__WIN32__)
#define BRN2_MAX_THREADS 1
#else
#define BRN2_MAX_THREADS 64
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

typedef struct File {
    char name[124];
    int32 fd;
    FILE *stream;
} File;

enum Brn2FileType {
    TYPE_DIR = 0,
    TYPE_FILE = 1,
    TYPE_ERR = 2,
};

typedef struct FileName {
    uint64 hash;
    int32 length;
    enum Brn2FileType type;
    char name[];
} FileName;

enum Brn2RenameExecutionMode {
    BRN2_RENAME_NORMAL,
    BRN2_RENAME_REPLACE_EQUAL_TARGET,
    BRN2_RENAME_SKIP_EQUAL_TARGET_OWNER,
};

typedef struct Brn2RenamePlan {
    enum Brn2RenameExecutionMode execution_mode;
    int32 conflicting_owner_index;
    int32 claimant_count;
} Brn2RenamePlan;

typedef struct FileList {
    Arena *arenas[BRN2_MAX_THREADS];
    uint32 *indexes;
    int64 indexes_size;
    Brn2RenamePlan *rename_plans;
    int64 rename_plans_size;
    int32 length;
    int32 capacity;
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

INLINE int32 brn2_compare(void *, void *);
void brn2_list_from_dir(FileList *, char *);
void brn2_list_from_file(FileList *, char *, bool);
void brn2_list_from_args(FileList *, int32, char **);
void brn2_normalize_names(FileList *, FileList *);
void brn2_create_hashes(FileList *, uint32);
bool brn2_verify(FileList *, FileList *, struct Hash_map *,
                 struct Hash_map *, uint32 *);
int32 brn2_get_number_changes(FileList *, FileList *);
void brn2_free_list(FileList *);
void brn2_print_list(FileList *);
void brn2_execute(FileList *, FileList *, struct Hash_map *,
                  struct Hash_set *, int32 *);
void brn2_execute2(FileList *, FileList *, struct Hash_map *,
                   struct Hash_set *, int32, int32 *);

void brn2_usage(FILE *) __attribute__((noreturn));

#endif
