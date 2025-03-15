#include "brn2.h"
#include <sys/mman.h>
#include "util.c"

#define PushArray(arena, type, count) (type *)ArenaPush((arena), sizeof(type)*(count))
#define PushArrayZero(arena, type, count) (type *)ArenaPushZero((arena), sizeof(type)*(count))
#define PushStruct(arena, type) PushArray((arena), (type), 1)
#define PushStructZero(arena, type) PushArrayZero((arena), (type), 1)

Arena *
arena_alloc(size_t size) {
    Arena *arena;

    arena = xmalloc(sizeof(*arena));
    arena->size = size;
    arena->begin = mmap(NULL, arena->size,
                 PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    if (arena->begin == MAP_FAILED) {
        error("Error in mmap: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    arena->pos = arena->begin;
    return arena;
}

#if 1
void *
arena_push(Arena *arena, uint32 size) {
    void *before = arena->pos;
    arena->pos += size;
    return before;
}
#else
void *arena_push(Arena *arena, uint32 size) {
    return xmalloc(size);
}
#endif

void *
arena_reset(Arena *arena) {
    arena->pos = arena->begin;
    return arena->begin;
}

void *
arena_reset_zero(Arena *arena) {
    memset(arena->begin, 0, arena->pos - arena->begin);
    arena->pos = arena->begin;
    return arena->begin;
}

void
arena_release(Arena *arena) {
    munmap(arena->begin, arena->size);
    free(arena);
    return;
}
