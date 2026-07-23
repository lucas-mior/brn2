#if !defined(MEMORY_H)
#define MEMORY_H

#include "primitives.h"
#include "base_macros.h"
#include "util.h"

#if !defined(ALIGNMENT)
#define ALIGNMENT 16
#endif

#define MEMORY_PADDING ((int32)ALIGNMENT)

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_memory 1
#elif !defined(TESTING_memory)
#define TESTING_memory 0
#endif

#if TESTING_memory
#define DEBUGGING_MEMORY 1
#define MEMORY_CHECK_USE_AFTER_FREE 1
#define MEMORY_CHECK_DOUBLE_FREE 1
#endif

#if !defined(MEMORY_CHECK_USE_AFTER_FREE)
// this option makes every pointer leak and makes things extremely slow
#define MEMORY_CHECK_USE_AFTER_FREE 0
#endif

#if MEMORY_CHECK_USE_AFTER_FREE
// we are already leaking, might as well check double free
#define MEMORY_CHECK_DOUBLE_FREE 1
#endif

#if !defined(MEMORY_CHECK_DOUBLE_FREE)
// this option makes every pointer leak
#define MEMORY_CHECK_DOUBLE_FREE 1
#endif

#if !defined(DEBUGGING_MEMORY)
#define DEBUGGING_MEMORY DEBUGGING
#endif
#if DEBUGGING_MEMORY
#define malloc2_zero(size) \
    malloc_debug(__FILE__, __LINE__, FUNC__, \
                 size, true)
#define malloc2(size) \
    malloc_debug(__FILE__, __LINE__, FUNC__, \
                 size, false)
#define realloc2(old, old_capacity, new_capacity, obj_size) \
    realloc_debug(__FILE__, __LINE__, FUNC__, \
                  old, old_capacity, new_capacity, obj_size)
#define realloc_flex(old, old_capacity, new_capacity, obj_size) \
    realloc_flex_debug(__FILE__, __LINE__, FUNC__, \
                       old, SIZEOF(*old), old_capacity, \
                       new_capacity, obj_size)
#define free2(pointer, size) \
    free_debug(__FILE__, __LINE__, FUNC__, \
               pointer, size)
#else
#define malloc2_zero(size) \
    xmalloc(size, true)
#define malloc2(size) \
    xmalloc(size, false)
#define realloc2(old, old_capacity, new_capacity, obj_size) \
    realloc4(old, old_capacity, new_capacity, obj_size)
#define realloc_flex(old, old_capacity, new_capacity, obj_size) \
    xrealloc(old, SIZEOF(*old) + obj_size*new_capacity)
#define free2(pointer, size) \
    free2_(pointer, size)
#endif

static void free2_(void *pointer, int64 size);
static void *realloc4(void *old, int64 old_capacity, int64 new_capacity, int64 obj_size);
static void *malloc_debug(char *file, int32 line, char *func, int64 size, bool zero);
static void *realloc_debug(char *file, int32 line, char *func,
              void *old, int64 old_capacity, int64 new_capacity,
              int64 obj_size);
static void free_debug(char *file, int32 line, char *func, void *pointer, int64 size);
static void *realloc_flex_debug(char *file, int32 line, char *func,
                   void *old, int64 struct_size,
                   int64 old_capacity, int64 new_capacity, int64 obj_size);

#endif
