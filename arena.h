#ifndef ARENA_H
#define ARENA_H

typedef struct Arena {
    void *begin;
    void *pos;
    size_t size;
} Arena;

Arena *arena_alloc(size_t);
void *arena_push(Arena *arena, uint32);
void *arena_reset(Arena *arena);
void *arena_reset_zero(Arena *arena);
void arena_destroy(Arena *arena);

#endif
