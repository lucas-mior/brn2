#ifndef ARENA_H
#define ARENA_H

Arena *arena_alloc(size_t);
void *arena_push(Arena *arena, uint32);
void *arena_reset(Arena *arena);
void *arena_reset_zero(Arena *arena);
void arena_release(Arena *arena);

#endif
