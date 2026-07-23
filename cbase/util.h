#if !defined(UTIL_H)
#define UTIL_H

#if !defined(DEBUGGING)
#define DEBUGGING 0
#endif

#ifndef RELEASING
#define RELEASING 0
#endif

#if !defined(ERROR_NOTIFY)
#define ERROR_NOTIFY 0
#endif

#include <stdbool.h>
#include <time.h>
#include "primitives.h"
#include "memory.h"

static void __attribute__((format(printf, 4, 5)))
error_impl(char *file, int32 line, char *func, char *format, ...);
#define error(...) error_impl(__FILE__, __LINE__, (char *)__func__, __VA_ARGS__)
static int32 snprintf2(char *buffer, int64 size, char *format, ...);
static int32 optional_strlen32(char *string);
static int strncmp32(char *left, char *right, int64 size);
static int xclose(char *file, int line, int *fd, char *fd_var_name, char *filename);

static int64 read64(int fd, void *buf, int64 n);
static int64 write64(int fd, void *buf, int64 n);
static void xpipe(int array[2]);
static void xdup2(int fd1, int fd2);
static void xkill(pid_t pid, int signum);

static bool is_ident_start_char(char c);
static bool is_ident_char(char c);

static int32 random_ascii_string(char *buffer, int32 capacity, int32 min_len);

static void fatal(int status);

static void memset64(void *buffer, int value, int64 size);
static int32 strlen32(char *string);
static char *xstrdup(char *string);
static void *xmmap_commit(int64 *size);
static void xmunmap(void *p, int64 size);
static int memcmp64(void *left, void *right, int64 size);
static void
print_timings(char *file, int32 line, char *func,
              int64 nitems, struct timespec t0, struct timespec t1);
static bool strequal(char *s1, char *s2);
static bool strequal2(char *a, int32 a_len, char *b, int32 b_len);
static void *xmalloc(int64 size, bool zero);
static void xpthread_mutex_lock(pthread_mutex_t *mutex);
static void xpthread_mutex_unlock(pthread_mutex_t *mutex);
static void xpthread_cond_destroy(pthread_cond_t *cond);
static void xpthread_mutex_destroy(pthread_mutex_t *mutex);
static void xpthread_create(pthread_t *thread, pthread_attr_t *attr,
                void *(*function)(void *), void *arg);
static void xpthread_join(pthread_t *thread, void **thread_return);

static void normalize(char *restrict path, int32 *restrict length);
static bool write_entire_file(char *path, char *text, int64 text_len);

INLINE void * memchr64(void *pointer, int32 value, int64 size);
static int util_command(int argc, char **argv);

#define SNPRINTF(BUFFER, FORMAT, ...) \
    snprintf2(BUFFER, sizeof(BUFFER), FORMAT, __VA_ARGS__)
#define STRFTIME(BUFFER, FORMAT, TIME) \
    strftime2(BUFFER, sizeof(BUFFER), FORMAT, TIME)

#define STRUCT_ARRAY_SIZE(struct_object, ArrayType, array_length) \
    (int64)(SIZEOF(*(struct_object)) + ((array_length)*SIZEOF(ArrayType)))

#define XCLOSE_1(FD) xclose(__FILE__, __LINE__, FD, #FD, NULL)
#define XCLOSE_2(FD, NAME) xclose(__FILE__, __LINE__, FD, #FD, NAME)
#define XCLOSE(...) SELECT_ON_NUM_ARGS(XCLOSE_, __VA_ARGS__)

#define SB_APPEND_2(BUILER, STRING) \
        sb_append(BUILER, STRING, strlen32(STRING))
#define SB_APPEND_3(BUILER, STRING, LEN) \
        sb_append(BUILER, STRING, (int32)LEN)
#define SB_APPEND(...) SELECT_ON_NUM_ARGS(SB_APPEND_, __VA_ARGS__)

#define strequal2_3(A, A_LEN, B) \
        strequal2(A, A_LEN, B, strlen32(B))
#define strequal2_4(A, A_LEN, B, B_LEN) \
        strequal2(A, A_LEN, B, B_LEN)
#define STREQUAL(...) SELECT_ON_NUM_ARGS(strequal2_, __VA_ARGS__)

static void memcpy64(void *dest, void *source, int64 n);
static void memmove64(void *dest, void *source, int64 n);

static ullong here_counter = 0;

#define HERE do { \
    char HEREbuffer[4096]; \
    fprintf(stderr, "\n===== HERE(%llu): %s:%d (%s)\n", \
                    here_counter++, __FILE__, __LINE__, __func__); \
    SNPRINTF(HEREbuffer, "%s:%d:%s\n", __FILE__, __LINE__, __func__); \
    switch (fork()) { \
    case -1: \
        error("Error forking: %s.\n", strerror(errno)); \
        fatal(EXIT_FAILURE); \
    case 0: \
        execlp("dunstify", "dunstify", program, HEREbuffer, NULL); \
        error("Error executing dunstify: %s.\n", strerror(errno)); \
        exit(EXIT_FAILURE); \
    default: \
        break; \
    } \
} while (0)

#define NCALLS(INTERVAL) do { \
    static int64 ncalls_ncalls = 1; \
    if ((ncalls_ncalls % INTERVAL) == 0) { \
        fprintf(stderr, "%s:%d:%s: called %lld times\n", \
                        __FILE__, __LINE__, __func__, (llong)ncalls_ncalls); \
    } \
    ncalls_ncalls += 1; \
} while (0)

#define MEM_FREED 0xDC
#define MEM_MALLOCED_UNINITIALIZED 0xCD
#define MEM_DONT_READ 0xBD

#define PRINT_TIMINGS_3(N, T0, T1) \
        print_timings(__FILE__, __LINE__, (char *)__func__, N, T0, T1)
#define PRINT_TIMINGS_4(N, T0, T1, NAME) \
        print_timings(__FILE__, __LINE__, NAME, N, T0, T1)
#define PRINT_TIMINGS(...) SELECT_ON_NUM_ARGS(PRINT_TIMINGS_, __VA_ARGS__)

typedef struct StrBuilder {
    char *data;
    int32 len;
    int32 cap;
} StrBuilder;

typedef struct StrBuilderArray {
    StrBuilder *items;
    int32 len;
    int32 cap;
} StrBuilderArray;

static void sb_init(StrBuilder *str_builder);
static void sb_free(StrBuilder *str_builder);
static void sb_clear(StrBuilder *str_builder);
static bool sb_copy(StrBuilder *dest, StrBuilder *source);
static void sb_move(StrBuilder *dest, StrBuilder *source);
static bool sb_set(StrBuilder *str_builder, char *data, int32 data_len);
static void sb_reserve(StrBuilder *str_builder, int32 extra);
static void sb_append(StrBuilder *str_builder, char *data, int32 data_len);
static void sb_append_byte(StrBuilder *str_builder, char byte);
static void sb_printf(StrBuilder *str_builder, char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
static char *sb_steal(StrBuilder *str_builder, int32 *len, int32 *cap);
static char *sb_steal_exact(StrBuilder *str_builder, int32 *len);

static void str_builder_array_init(StrBuilderArray *array);
static void str_builder_array_clear(StrBuilderArray *array);
static void str_builder_array_destroy(StrBuilderArray *array);
static bool str_builder_array_copy(StrBuilderArray *dest,
                                   StrBuilderArray *source);
static void str_builder_array_move(StrBuilderArray *dest,
                                   StrBuilderArray *source);
static void str_builder_array_swap(StrBuilderArray *left,
                                   StrBuilderArray *right);
static bool str_builder_array_reserve(StrBuilderArray *array, int32 extra);
static StrBuilder *str_builder_array_append(StrBuilderArray *array);
static bool str_builder_array_append_copy(StrBuilderArray *array,
                                          StrBuilder *item);

static void *memmem64(void *haystack, int64 hay_len,
                      void *needle, int64 needle_len);

#endif /* UTIL_H */
