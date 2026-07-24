#if !defined(UTIL_H)
#define UTIL_H

#include <dirent.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#if !defined(MAX_FILES_COPY)
#define MAX_FILES_COPY 256
#endif

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

typedef struct UtilCopyFilesAsync {
    struct pollfd pipes[MAX_FILES_COPY];
    int dests[MAX_FILES_COPY];
    int32 nfds;
    int32 unused;
} UtilCopyFilesAsync;

static void error_impl(char *, int32, char *, char *, ...)
    __attribute__((format(printf, 4, 5)));
static void fatal(int32) __attribute__((noreturn));
static void util_segv_handler(int32) __attribute__((noreturn));
static int32 itoa2(char *, int32, llong);
static long atoi2(char *);
static char *basename2(char *, int32 *, int32 *);
static char *begins_with(char *, int32, char *, int32);
static int32 bytes_pretty(char *, int64);
static void catfile(int, char *);
static double deg2rad(double);
static int32 dirname2(char *, char *, int32 *);
static char *ends_with(char *, int32, char *, int32);
static void error_async_safe(char *);
static bool is_ident_char(char);
static bool is_ident_start_char(char);
static void *memchr64(void *, int32, int64);
static int memcmp64(void *, void *, int64);
static void *memmem64(void *, int64, void *, int64);
static void *memrchr64(void *, int32, int64);
static void normalize(char *restrict, int32 *restrict);
static int32 optional_strlen32(char *);
static bool parse_option(char **, char *, char *);
static char *path_basename(char *, int32);
static void print_timings(
    char *,
    int32,
    char *,
    int64,
    struct timespec,
    struct timespec
);
static void qsort64(void *, int64, int64, int (*)(void *, void *));
static double rad2deg(double);
static int32 random_ascii_string(char *, int32, int32);
static char *read_entire_file(char *, int32 *);
static char *remove_escape_sequences(char *, int32 *);
static void sb_append(StrBuilder *, char *, int32);
static void sb_append_byte(StrBuilder *, char);
static void sb_clear(StrBuilder *);
static bool sb_copy(StrBuilder *, StrBuilder *);
static void sb_free(StrBuilder *);
static void sb_init(StrBuilder *);
static void sb_move(StrBuilder *, StrBuilder *);
static void sb_printf(StrBuilder *, char *, ...);
static void sb_reserve(StrBuilder *, int32);
static bool sb_set(StrBuilder *, char *, int32);
static char *sb_steal(StrBuilder *, int32 *, int32 *);
static char *sb_steal_exact(StrBuilder *, int32 *);
static void send_signal(char *, int32);
static int32 snprintf2(char *, int64, char *, ...);
static StrBuilder *str_builder_array_append(StrBuilderArray *);
static bool str_builder_array_append_copy(StrBuilderArray *, StrBuilder *);
static void str_builder_array_clear(StrBuilderArray *);
static bool str_builder_array_copy(StrBuilderArray *, StrBuilderArray *);
static void str_builder_array_destroy(StrBuilderArray *);
static void str_builder_array_init(StrBuilderArray *);
static void str_builder_array_move(StrBuilderArray *, StrBuilderArray *);
static bool str_builder_array_reserve(StrBuilderArray *, int32);
static void str_builder_array_swap(StrBuilderArray *, StrBuilderArray *);
static int32 string_from_strings(char *, int32, char *, char **, int32);
static int32 string_from_doubles(char *, int32, char *, double *, int32);
static double clamp_double(double, double, double);
static double square_double(double);
static int64 clamp_int64(int64, int64, int64);
static int64 square_int64(int64);
static bool strequal(char *, char *);
static bool strequal2(char *, int32, char *, int32);
static int64 strftime2(char *, int64, char *, struct tm *);
static int32 strlen32(char *);
static int strncmp32(char *, char *, int64);
static char *strncpy32(char *, char *, int64);
static double timediff(struct timespec, struct timespec);
static void timezone_init(void);
static int util_command(int, char **);
static int util_command_launch(int, char **);
static int32 util_copy_file_async(char *, char *, int *);
static void util_copy_file_async_parsed(UtilCopyFilesAsync *);
static void *util_copy_file_async_thread(void *);
static int32 util_copy_file_sync(char *, char *);
static void util_die_notify(char *, char *, ...);
static bool util_equal_files(char *, char *);
static bool util_file_exists(char *);
static int32 util_filename_from(char *, int64, int);
static void util_functions_sink(void);
static int32 util_nthreads(void);
static int32 util_string_int32(int32 *, char *);
static void warn(char *, ...);
static int64 read64(int32, void *, int64);
static int64 write64(int32, void *, int64);
static int64 fread64(void *, int64, int64, FILE *);
static int64 fwrite64(void *, int64, int64, FILE *);
static void write_all(int, char *, int64);
static bool write_entire_file(char *, char *, int64);
static int xclose(char *, int, int *, char *, char *);
static int xclosedir(DIR *, char *);
static void xdup2(int, int);
static int xfclose(char *, int32, char *, FILE *, char *);
static FILE *xfopen(char *, int32, char *, char *, char *);
static void xkill(pid_t, int);
static void xpipe(int [2]);
static void xpthread_cond_destroy(pthread_cond_t *);
static void xpthread_create(
    pthread_t *,
    pthread_attr_t *,
    void *(*)(void *),
    void *
);
static void xpthread_join(pthread_t *, void **);
static void xpthread_mutex_destroy(pthread_mutex_t *);
static void xpthread_mutex_lock(pthread_mutex_t *);
static void xpthread_mutex_unlock(pthread_mutex_t *);
static int xunlink(char *);
static void here_impl(char *, int32, char *);

#define error(...) \
    error_impl(__FILE__, __LINE__, (char *)__func__, __VA_ARGS__)
#define error2(...) fprintf(stderr, __VA_ARGS__)

#define STRING_FROM_ARRAY(BUFFER, SEP, ARRAY, LENGTH) \
_Generic((ARRAY), \
    double *: string_from_doubles, \
    char **: string_from_strings \
)(BUFFER, SIZEOF(BUFFER), SEP, ARRAY, LENGTH)

#define CLAMP(VAR, VMIN, VMAX) \
_Generic((VAR), \
    float: clamp_double, \
    double: clamp_double, \
    default: clamp_int64 \
)(VAR, VMIN, VMAX)

#define SQUARE(VAR) \
_Generic((VAR), \
    float: square_double, \
    double: square_double, \
    default: square_int64 \
)(VAR)

#define MEMMEM_3(LONG, LONG_LEN, SHORT) \
    memmem64(LONG, LONG_LEN, SHORT, strlen32(SHORT))
#define MEMMEM_4(LONG, LONG_LEN, SHORT, LEN) \
    memmem64(LONG, LONG_LEN, SHORT, LEN)
#define MEMMEM(...) SELECT_ON_NUM_ARGS(MEMMEM_, __VA_ARGS__)

#define BEGINS_WITH_3(STRING, STRING_LEN, PREFIX) \
    begins_with(STRING, STRING_LEN, PREFIX, strlen32(PREFIX))
#define BEGINS_WITH_4(STRING, STRING_LEN, PREFIX, PREFIX_LEN) \
    begins_with(STRING, STRING_LEN, PREFIX, PREFIX_LEN)
#define BEGINS_WITH(...) SELECT_ON_NUM_ARGS(BEGINS_WITH_, __VA_ARGS__)

#define ENDS_WITH_3(STRING, STRING_LEN, SUFFIX) \
    ends_with(STRING, STRING_LEN, SUFFIX, strlen32(SUFFIX))
#define ENDS_WITH_4(STRING, STRING_LEN, SUFFIX, SUFFIX_LEN) \
    ends_with(STRING, STRING_LEN, SUFFIX, SUFFIX_LEN)
#define ENDS_WITH(...) SELECT_ON_NUM_ARGS(ENDS_WITH_, __VA_ARGS__)

#if OS_WINDOWS || OS_MAC
#define basename basename2
#endif
#define ITOA(BUFFER, NUM) itoa2(BUFFER, SIZEOF(BUFFER), NUM)

#define SNPRINTF(BUFFER, FORMAT, ...) \
    snprintf2(BUFFER, SIZEOF(BUFFER), FORMAT, __VA_ARGS__)
#define STRFTIME(BUFFER, FORMAT, TIME) \
    strftime2(BUFFER, SIZEOF(BUFFER), FORMAT, TIME)

#define STRUCT_ARRAY_SIZE(STRUCT_OBJECT, ARRAY_TYPE, ARRAY_LENGTH) \
    (SIZEOF(*(STRUCT_OBJECT)) + (ARRAY_LENGTH)*SIZEOF(ARRAY_TYPE))

#define XCLOSE_1(FD) xclose(__FILE__, __LINE__, FD, #FD, NULL)
#define XCLOSE_2(FD, NAME) xclose(__FILE__, __LINE__, FD, #FD, NAME)
#define XCLOSE(...) SELECT_ON_NUM_ARGS(XCLOSE_, __VA_ARGS__)

#define XFOPEN(FILENAME, MODE) \
    xfopen(__FILE__, __LINE__, FUNC__, FILENAME, MODE)
#define XFCLOSE(F, FILENAME) \
    xfclose(__FILE__, __LINE__, FUNC__, F, FILENAME)

#define SB_APPEND_2(BUILDER, STRING) \
    sb_append(BUILDER, STRING, strlen32(STRING))
#define SB_APPEND_3(BUILDER, STRING, LEN) \
    sb_append(BUILDER, STRING, (int32)(LEN))
#define SB_APPEND(...) SELECT_ON_NUM_ARGS(SB_APPEND_, __VA_ARGS__)

#define strequal2_3(A, A_LEN, B) strequal2(A, A_LEN, B, strlen32(B))
#define strequal2_4(A, A_LEN, B, B_LEN) strequal2(A, A_LEN, B, B_LEN)
#define STREQUAL(...) SELECT_ON_NUM_ARGS(strequal2_, __VA_ARGS__)

#define HERE here_impl(__FILE__, __LINE__, (char *)__func__)

#define NCALLS(INTERVAL) do { \
    static int64 ncalls_ncalls = 1; \
    if ((ncalls_ncalls % (INTERVAL)) == 0) { \
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

#define GETENV(VAR) do { \
    if (((VAR) = getenv(#VAR)) == NULL) { \
        if (DEBUGGING) { \
            error_impl(__FILE__, __LINE__, FUNC__, \
                       RED("%s") " is not defined.", #VAR); \
        } \
    } else { \
        int32 getenv_len_ = strlen32(VAR); \
        char *getenv_copy_ = malloc2(getenv_len_ + 1); \
        memcpy64(getenv_copy_, VAR, getenv_len_ + 1); \
        (VAR) = getenv_copy_; \
    } \
} while (0)

#define PARSE_OPTION(ARG, NAME) \
    if (parse_option(&(NAME), ARG, #NAME)) { \
        continue; \
    }

#endif /* UTIL_H */
