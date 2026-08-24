// SPDX-License-Identifier: AGPL
// Copyright (c) 2026 Lucas Mior

#if !defined(ARENA_H)
#define ARENA_H

#include "primitives.h"
#include "base_macros.h"

typedef struct Arena {
    char *name;
    char *begin;
    void *pos;
    int64 size;
    int64 npushed;
    struct Arena *next;
} Arena;

enum ArenaErrors {
    EARENA_INVALID = 2000000,
    EARENA_INVALID_OBJECT,
    EARENA_OBJECT_SIZE,
    EARENA_MORE_THAN_4GB,
    EARENA_LINKED,
    EARENA_SIZE,
};

Arena *arena_create(int64, char *);
int64 arena_data_size(Arena *);
bool arena_decr(Arena *, void *);
void arena_destroy(Arena *);
int32 arena_nlinked(Arena *);
Arena *arena_of(Arena *, void *);
void arena_print(Arena *);
void *arena_push(Arena *, int64);
uint32 arena_push_index32(Arena *, uint32);
void *arena_reset(Arena *);
char *arena_strerror(int);
Arena *arena_with_space(Arena *, int64);
void arenas_destroy(Arena **, int32);
bool arenas_pop(Arena **, int32, void *);
void *arenas_push(Arena **, int32, int64);
void *arenas_reset(Arena **, int32);
void *xarena_push(Arena *, int64);
void *xarenas_push(Arena **, int32, int64);

#endif /* ARENA_H */
