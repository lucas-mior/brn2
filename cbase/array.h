#if !defined(ARRAY_H)
#define ARRAY_H

typedef struct GenericArrayHeader {
    ldouble alignment;
    int32 count;
    int32 cap;
    int64 padding;
} GenericArrayHeader;

static void *generic_array_init(int32, int64);
static void *generic_array_grow(void *, int64);
static void array_sink(void);

#define ARRAY_HEADER(ARRAY) ((GenericArrayHeader *)(ARRAY) - 1)
#define ARRAY_LEN(ARRAY) ((ARRAY) ? ARRAY_HEADER(ARRAY)->count : 0)
#define ARRAY_CLEAR(ARRAY) do { \
    if (ARRAY) { \
        ARRAY_HEADER(ARRAY)->count = 0; \
    } \
} while (0)
#define ARRAY_FREE(ARRAY) do { \
    if (ARRAY) { \
        GenericArrayHeader *array_header_ = ARRAY_HEADER(ARRAY); \
        free2(array_header_, SIZEOF(*array_header_) \
              + array_header_->cap*SIZEOF(*(ARRAY))); \
        (ARRAY) = NULL; \
    } \
} while (0)
#define ARRAY_PUSH(ARRAY, ...) \
    ((ARRAY) = generic_array_grow((ARRAY), SIZEOF(*(ARRAY))), \
     (ARRAY)[ARRAY_HEADER(ARRAY)->count++] = (__VA_ARGS__))
#define ARRAY_INIT(ARRAY, CAPACITY) \
    ((ARRAY) = generic_array_init((CAPACITY), SIZEOF(*(ARRAY))))

#endif /* ARRAY_H */
