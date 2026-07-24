// SPDX-License-Identifier: AGPL
// Copyright (c) 2026 Lucas Mior

#if !defined(CBASE_H)
#define CBASE_H

#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif

#if !defined(_DEFAULT_SOURCE)
#define _DEFAULT_SOURCE
#endif

#if !defined(TESTING_array)
#define TESTING_array 0
#endif
#if !defined(TESTING_arena)
#define TESTING_arena 0
#endif
#if !defined(TESTING_assert)
#define TESTING_assert 0
#endif
#if !defined(TESTING_command)
#define TESTING_command 0
#endif
#if !defined(TESTING_generic)
#define TESTING_generic 0
#endif
#if !defined(TESTING_hash)
#define TESTING_hash 0
#endif
#if !defined(TESTING_memory)
#define TESTING_memory 0
#endif
#if !defined(TESTING_meta_common)
#define TESTING_meta_common 0
#endif
#if !defined(TESTING_meta_generate)
#define TESTING_meta_generate 0
#endif
#if !defined(TESTING_meta_parse)
#define TESTING_meta_parse 0
#endif
#if !defined(TESTING_meta_tokenize)
#define TESTING_meta_tokenize 0
#endif
#if !defined(TESTING_minmax)
#define TESTING_minmax 0
#endif
#if !defined(TESTING_sort)
#define TESTING_sort 0
#endif
#if !defined(TESTING_utf8)
#define TESTING_utf8 0
#endif
#if !defined(TESTING_util)
#define TESTING_util 0
#endif
#if !defined(TESTING_xenums)
#define TESTING_xenums 0
#endif


#if !defined(DEBUGGING)
#define DEBUGGING 0
#endif

#if !defined(RELEASING)
#define RELEASING 0
#endif

#if !defined(ERROR_NOTIFY)
#define ERROR_NOTIFY 0
#endif

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "platform_detection.h"
#include "primitives.h"
#include "base_macros.h"
#include "i18n.h"
#include "memory.h"
#include <stdbool.h>
#include <stdio.h>

#include "primitives.h"

enum Type {
    TYPE_VOIDP = 1,
    TYPE_CHARP,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_SCHAR,
    TYPE_SHORT,
    TYPE_INT,
    TYPE_LONG,
    TYPE_LLONG,
    TYPE_UCHAR,
    TYPE_USHORT,
    TYPE_UINT,
    TYPE_ULONG,
    TYPE_ULLONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_OTHER = 0,
};

union Primitive {
    void *avoidp;
    char *acharp;
    bool abool;
    char achar;
    schar aschar;
    short ashort;
    int aint;
    long along;
    llong allong;
    uchar auchar;
    ushort aushort;
    uint auint;
    ulong aulong;
    ullong aullong;
    float afloat;
    double adouble;
};

static double double_from_bool(bool);
static double double_from_char(char);
static double double_from_charp(char *);
static double double_from_double(double);
static double double_from_float(float);
static double double_from_int(int);
static double double_from_llong(llong);
static double double_from_long(long);
static double double_from_schar(schar);
static double double_from_short(short);
static double double_from_uchar(uchar);
static double double_from_uint(uint);
static double double_from_ullong(ullong);
static double double_from_ulong(ulong);
static double double_from_ushort(ushort);
static double double_from_voidp(void *);
static double double_get(union Primitive, enum Type);
static int fprint_0(FILE *restrict, ...);
static void generic_functions_sink(void);
static int snprint_0(char *restrict, int64, ...);
static int strlen2(char *);
static char *toString(char *restrict, int64, char *restrict, ...);
static llong typebits(enum Type);
static char *typename(enum Type);
void UNSUPPORTED_TYPE_FOR_DOUBLE_GET_GENERIC(void);

#if !defined(GENERIC_S_BSZ)
#define GENERIC_S_BSZ 64
#endif

#if !defined(S_BSZ)
#define S_BSZ GENERIC_S_BSZ
#endif

#if !defined(MACRO_NAME)
#define MACRO_NAME(X) #X
#endif
#if !defined(MACRO_VALUE)
#define MACRO_VALUE(X) MACRO_NAME(X)
#endif

#define fprint(FP, ...) fprint_0((FP), __VA_ARGS__, (char *)0)
#define snprint(BUF, BSZ, ...) snprint_0((BUF), (BSZ), __VA_ARGS__, (char *)0)
#define print0(...) fprint_0(stdout, __VA_ARGS__, (char *)0)

#define S_(X) toString((char[S_BSZ]){ "" }, S_BSZ, _Generic((X), \
    void *: "%p", \
    char *: "%s", \
    bool: "%i", \
    char: "%c", \
    schar: "%hhi", \
    short: "%hi", \
    int: "%i", \
    long: "%li", \
    llong: "%lli", \
    uchar: "%hhu", \
    ushort: "%hu", \
    uint: "%u", \
    ulong: "%lu", \
    ullong: "%llu", \
    float: "%." MACRO_VALUE(FLT_DIG) "g", \
    double: "%." MACRO_VALUE(DBL_DIG) "g", \
    default: "%p" \
), (X))

#define V(X) "", S_(X), ""
#define W(X) "", (X), ""
#define SF(F, X) toString((char[S_BSZ]){ "" }, S_BSZ, (F), (X))
#define VF(F, X) "", SF((F), (X)), ""

#define TYPENAME(VAR)        \
_Generic((VAR),              \
    void*:   "void*",        \
    char*:   "char*",        \
    bool:    "bool",         \
    char:    "char",         \
    schar:   "schar",        \
    short:   "short",        \
    int:     "int",          \
    long:    "long",         \
    llong:   "llong",        \
    uchar:   "uchar",        \
    ushort:  "ushort",       \
    uint:    "uint",         \
    ulong:   "ulong",        \
    ullong:  "ullong",       \
    float:   "float",        \
    double:  "double",       \
    default: "unknown"       \
)

#define MINOF(VARIABLE)           \
_Generic((VARIABLE),              \
    schar:   SCHAR_MIN,           \
    short:   SHRT_MIN,            \
    int:     INT_MIN,             \
    long:    LONG_MIN,            \
    llong:   LLONG_MIN,           \
    uchar:   0,                   \
    ushort:  0,                   \
    uint:    0u,                  \
    ulong:   0ul,                 \
    ullong:  0ull,                \
    char:    CHAR_MIN,            \
    bool:    0,                   \
    float:   -FLT_MAX,            \
    double:  -DBL_MAX,            \
    default: 0                    \
)

#define MAXOF(VARIABLE)           \
_Generic((VARIABLE),              \
    schar:   SCHAR_MAX,           \
    short:   SHRT_MAX,            \
    int:     INT_MAX,             \
    long:    LONG_MAX,            \
    llong:   LLONG_MAX,           \
    uchar:   UCHAR_MAX,           \
    ushort:  USHRT_MAX,           \
    uint:    UINT_MAX,            \
    ulong:   ULONG_MAX,           \
    ullong:  ULLONG_MAX,          \
    char:    CHAR_MAX,            \
    bool:    1,                   \
    float:   FLT_MAX,             \
    double:  DBL_MAX,             \
    default: 1                    \
)

#define TYPEID(VAR) \
_Generic((VAR), \
    void*:   TYPE_VOIDP,       \
    char*:   TYPE_CHARP,       \
    bool:    TYPE_BOOL,        \
    char:    TYPE_CHAR,        \
    schar:   TYPE_SCHAR,       \
    short:   TYPE_SHORT,       \
    int:     TYPE_INT,         \
    long:    TYPE_LONG,        \
    llong:   TYPE_LLONG,       \
    uchar:   TYPE_UCHAR,       \
    ushort:  TYPE_USHORT,      \
    uint:    TYPE_UINT,        \
    ulong:   TYPE_ULONG,       \
    ullong:  TYPE_ULLONG,      \
    float:   TYPE_FLOAT,       \
    double:  TYPE_DOUBLE,      \
    default: TYPE_OTHER        \
)

#define TYPEBITS(VAR) (sizeof(VAR)*CHAR_BIT)

#define DOUBLE_GET(x) \
_Generic((x), \
    void*:   double_from_voidp,                                  \
    char*:   double_from_charp,                                  \
    bool:    double_from_bool,                                   \
    char:    double_from_char,                                   \
    schar:   double_from_schar,                                  \
    short:   double_from_short,                                  \
    int:     double_from_int,                                    \
    long:    double_from_long,                                   \
    llong:   double_from_llong,                                  \
    uchar:   double_from_uchar,                                  \
    ushort:  double_from_ushort,                                 \
    uint:    double_from_uint,                                   \
    ulong:   double_from_ulong,                                  \
    ullong:  double_from_ullong,                                 \
    float:   double_from_float,                                  \
    double:  double_from_double,                                 \
    default: UNSUPPORTED_TYPE_FOR_DOUBLE_GET_GENERIC            \
)(x)

#if defined(__GNUC__) || defined(__clang__)
#define DOUBLE_GET2(VAR, TYPE) double_get((union Primitive)(VAR), TYPE)
#else
#define DOUBLE_GET2(VAR, TYPE) DOUBLE_GET(VAR)
#endif

#define PRINT_SIGNED(VAR, TYPE) \
  fprintf(stderr, "["GREEN("%s%lld")"]%s = %lld ", \
                  typename(TYPE), typebits(TYPE), #VAR, (llong)(VAR))

#define PRINT_UNSIGNED(VAR, TYPE) \
  fprintf(stderr, "["GREEN("%s%lld")"]%s = %llu ", \
                  typename(TYPE), typebits(TYPE), #VAR, (ullong)(VAR))

#define PRINT_DOUBLE(VAR, TYPE) \
  fprintf(stderr, "["GREEN("%s%lld")"]%s = %f ", \
                  typename(TYPE), typebits(TYPE), #VAR, DOUBLE_GET2(VAR, TYPE))

#define PRINT_OTHER(VAR, TYPE, FORMAT, CAST) \
  fprintf(stderr, "["GREEN("%s%lld")"]%s = "FORMAT" ", \
                  typename(TYPE), typebits(TYPE), #VAR, (CAST)(uintptr_t)(VAR))

#define PRINT(VAR) \
_Generic((VAR), \
    void*:   PRINT_OTHER(VAR,    TYPE_VOIDP,   "%p",           void*), \
    char*:   PRINT_OTHER(VAR,    TYPE_CHARP,   RED("\"%s\""),  char*), \
    bool:    PRINT_OTHER(VAR,    TYPE_BOOL,    "%u",           bool),  \
    char:    PRINT_OTHER(VAR,    TYPE_CHAR,    YELLOW("'%c'"), char),  \
    schar:   PRINT_SIGNED(VAR,   TYPE_SCHAR),                          \
    short:   PRINT_SIGNED(VAR,   TYPE_SHORT),                          \
    int:     PRINT_SIGNED(VAR,   TYPE_INT),                            \
    long:    PRINT_SIGNED(VAR,   TYPE_LONG),                           \
    llong:   PRINT_SIGNED(VAR,   TYPE_LLONG),                          \
    uchar:   PRINT_UNSIGNED(VAR, TYPE_UCHAR),                          \
    ushort:  PRINT_UNSIGNED(VAR, TYPE_USHORT),                         \
    uint:    PRINT_UNSIGNED(VAR, TYPE_UINT),                           \
    ulong:   PRINT_UNSIGNED(VAR, TYPE_ULONG),                          \
    ullong:  PRINT_UNSIGNED(VAR, TYPE_ULLONG),                         \
    float:   PRINT_DOUBLE(VAR,  TYPE_FLOAT),                          \
    double:  PRINT_DOUBLE(VAR,  TYPE_DOUBLE),                         \
    default: 0                                                        \
)

#define PRINTLN(VAR) do { \
    fprintf(stderr, "%s:%d %s():", __FILE__, __LINE__, __func__); \
    PRINT(VAR); \
    fprintf(stderr, "\n"); \
} while (0)

static llong get_both_signed_max(llong, llong);
static llong get_both_signed_min(llong, llong);
static ullong get_both_unsigned_max(ullong, ullong);
static ullong get_both_unsigned_min(ullong, ullong);
static double get_double_max(double, double);
static double get_double_min(double, double);
static void *get_pointer_max(void *, void *);
static void *get_pointer_min(void *, void *);
static llong get_signed_unsigned_max(llong, ullong);
static llong get_signed_unsigned_min(llong, ullong);
static llong get_unsigned_signed_max(ullong, llong);
static llong get_unsigned_signed_min(ullong, llong);
static void minmax_functions_sink(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_FIRST_SIGNED(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_FIRST_UNSIGNED(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_FIRST_DOUBLE(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_MINMAX_COMPARE_VOIDP(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_MINMAX_COMPARE(void);

#define BOTH_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2) \
    get_both_signed_##MODE((llong)(VAR1), (llong)(VAR2))

#define SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2) \
    get_signed_unsigned_##MODE((llong)(VAR1), (ullong)(VAR2))

#define FIRST_SIGNED(MODE, VAR1, VAR2, TYPE1) \
_Generic((VAR2), \
    schar:   BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_SCHAR  ), \
    short:   BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_SHORT  ), \
    int:     BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_INT    ), \
    long:    BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_LONG   ), \
    llong:   BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_LLONG  ), \
    uchar:   SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_UCHAR  ), \
    ushort:  SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_USHORT ), \
    uint:    SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_UINT   ), \
    ulong:   SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_ULONG  ), \
    ullong:  SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_ULLONG ), \
    float:   BOTH_DOUBLE(MODE,    VAR1, VAR2, TYPE1, TYPE_FLOAT  ), \
    double:  BOTH_DOUBLE(MODE,    VAR1, VAR2, TYPE1, TYPE_DOUBLE ), \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_FIRST_SIGNED() \
)
#define BOTH_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2) \
    get_both_unsigned_##MODE((ullong)(VAR1), (ullong)(VAR2))

#define UNSIGNED_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2) \
    get_unsigned_signed_##MODE((ullong)(VAR1), (llong)(VAR2))

#define FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE1) \
_Generic((VAR2), \
    schar:   UNSIGNED_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_SCHAR  ), \
    short:   UNSIGNED_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_SHORT  ), \
    int:     UNSIGNED_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_INT    ), \
    long:    UNSIGNED_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_LONG   ), \
    llong:   UNSIGNED_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_LLONG  ), \
    uchar:   BOTH_UNSIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_UCHAR  ), \
    ushort:  BOTH_UNSIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_USHORT ), \
    uint:    BOTH_UNSIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_UINT   ), \
    ulong:   BOTH_UNSIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_ULONG  ), \
    ullong:  BOTH_UNSIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_ULLONG ), \
    float:   BOTH_DOUBLE(MODE,    VAR1, VAR2, TYPE1, TYPE_FLOAT  ), \
    double:  BOTH_DOUBLE(MODE,    VAR1, VAR2, TYPE1, TYPE_DOUBLE ), \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_FIRST_UNSIGNED() \
)
#define BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE2) \
    get_double_##MODE(DOUBLE_GET2(VAR1, TYPE1), DOUBLE_GET2(VAR2, TYPE2))

#define FIRST_DOUBLE(MODE, VAR1, VAR2, TYPE1) \
_Generic((VAR2), \
    schar:   BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_SCHAR  ), \
    short:   BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_SHORT  ), \
    int:     BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_INT    ), \
    long:    BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_LONG   ), \
    llong:   BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_LLONG  ), \
    uchar:   BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_UCHAR  ), \
    ushort:  BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_USHORT ), \
    uint:    BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_UINT   ), \
    ulong:   BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_ULONG  ), \
    ullong:  BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_ULLONG ), \
    float:   BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_FLOAT  ), \
    double:  BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_DOUBLE ), \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_FIRST_DOUBLE()        \
)
#define POINTERS(MODE, VAR1, VAR2) \
    get_pointer_##MODE((void *)(uintptr_t)(VAR1), (void *)(uintptr_t)(VAR2))

#define MINMAX_COMPARE(MODE, VAR1, VAR2) \
_Generic((VAR1), \
    void *: _Generic((VAR2), \
        char *: POINTERS(MODE, VAR1, VAR2), \
        void *: POINTERS(MODE, VAR1, VAR2), \
        default: UNSUPPORTED_TYPE_FOR_GENERIC_MINMAX_COMPARE_VOIDP() \
    ), \
    schar:   FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_SCHAR  ), \
    short:   FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_SHORT  ), \
    int:     FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_INT    ), \
    long:    FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_LONG   ), \
    llong:   FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_LLONG  ), \
    uchar:   FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_UCHAR  ), \
    ushort:  FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_USHORT ), \
    uint:    FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_UINT   ), \
    ulong:   FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_ULONG  ), \
    ullong:  FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_ULLONG ), \
    float:   FIRST_DOUBLE(MODE,  VAR1, VAR2, TYPE_FLOAT  ), \
    double:  FIRST_DOUBLE(MODE,  VAR1, VAR2, TYPE_DOUBLE ), \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_MINMAX_COMPARE() \
)

#if defined(MIN)
#undef MIN
#endif
#if defined(MAX)
#undef MAX
#endif

#define MIN(VAR1, VAR2) MINMAX_COMPARE(min, VAR1, VAR2)
#define MAX(VAR1, VAR2) MINMAX_COMPARE(max, VAR1, VAR2)

static void a_bool_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    bool,
    bool
);
static void a_bool_less(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    bool,
    bool
);
static void a_bool_less_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    bool,
    bool
);
static void a_bool_more(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    bool,
    bool
);
static void a_bool_more_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    bool,
    bool
);
static void a_bool_not_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    bool,
    bool
);
static void a_both_signed_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    long long,
    long long
);
static void a_both_signed_less(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    long long,
    long long
);
static void a_both_signed_less_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    long long,
    long long
);
static void a_both_signed_more(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    long long,
    long long
);
static void a_both_signed_more_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    long long,
    long long
);
static void a_both_signed_not_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    long long,
    long long
);
static void a_both_unsigned_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    unsigned long long,
    unsigned long long
);
static void a_both_unsigned_less(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    unsigned long long,
    unsigned long long
);
static void a_both_unsigned_less_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    unsigned long long,
    unsigned long long
);
static void a_both_unsigned_more(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    unsigned long long,
    unsigned long long
);
static void a_both_unsigned_more_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    unsigned long long,
    unsigned long long
);
static void a_both_unsigned_not_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    unsigned long long,
    unsigned long long
);
static void a_double_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    int,
    int,
    double,
    double
);
static void a_double_less(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    int,
    int,
    double,
    double
);
static void a_double_less_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    int,
    int,
    double,
    double
);
static void a_double_more(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    int,
    int,
    double,
    double
);
static void a_double_more_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    int,
    int,
    double,
    double
);
static void a_double_not_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    int,
    int,
    double,
    double
);
static void a_pointers_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    void *,
    void *
);
static void a_pointers_less(
    char *,
    uint,
    char *,
    char *,
    char *,
    void *,
    void *
);
static void a_pointers_less_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    void *,
    void *
);
static void a_pointers_more(
    char *,
    uint,
    char *,
    char *,
    char *,
    void *,
    void *
);
static void a_pointers_more_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    void *,
    void *
);
static void a_pointers_not_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    void *,
    void *
);
static void a_signed_unsignedequal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    llong,
    ullong
);
static void a_signed_unsignedless(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    llong,
    ullong
);
static void a_signed_unsignedless_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    llong,
    ullong
);
static void a_signed_unsignedmore(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    llong,
    ullong
);
static void a_signed_unsignedmore_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    llong,
    ullong
);
static void a_signed_unsignednot_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    llong,
    ullong
);
static void a_strings_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *
);
static void a_strings_less(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *
);
static void a_strings_less_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *
);
static void a_strings_more(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *
);
static void a_strings_more_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *
);
static void a_strings_not_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *
);
static void a_unsigned_signed_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    ullong,
    llong
);
static void a_unsigned_signed_less(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    ullong,
    llong
);
static void a_unsigned_signed_less_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    ullong,
    llong
);
static void a_unsigned_signed_more(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    ullong,
    llong
);
static void a_unsigned_signed_more_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    ullong,
    llong
);
static void a_unsigned_signed_not_equal(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    ullong,
    llong
);
static void assert_contains(char *, int32, char *, char *, int32, char *);
static double assert_double_abs(double);
static bool assert_double_almost_equal(
    double,
    double,
    int,
    int,
    double *,
    double *,
    double *
);
static void assert_double_failure(
    char *,
    uint,
    char *,
    char *,
    char *,
    char *,
    char *,
    llong,
    llong,
    double,
    double,
    char *,
    double,
    double,
    double
);
static bool assert_double_is_infinite(double);
static bool assert_double_less(double, double, int, int);
static bool assert_double_more(double, double, int, int);
static void assert_file_contains(char *, int32, char *, char *, char *);
static double assert_fp_abs_tol(int);
static int assert_fp_common_kind(int, int);
static double assert_fp_rel_tol(int);
static void assert_functions_sink(void);
static void assert_not_contains(char *, int32, char *, char *, int32, char *);
static int compare_sign_with_unsign(llong, ullong);
void UNSUPPORTED_TYPE_FOR_GENERIC_A_FIRST_SIGNED(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_A_FIRST_UNSIGNED(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_A_FIRST_DOUBLE(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_A_FIRST_BOOL(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_ASSERT_COMPARE_CHARP(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_ASSERT_COMPARE_VOIDP(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_ASSERT_COMPARE(void);

#define ASSERT(C) do {                                     \
    if (!(C)) {                                            \
        if (!DEBUGGING) {                                  \
            UNREACHABLE();                       \
        } else {                                           \
            error2("%s: Assertion '%s' failed at %s:%d\n", \
                   __func__, #C, __FILE__, __LINE__);      \
            TRAP();                                        \
        }                                                  \
    }                                                      \
} while (0)

#define ASSERT_FILE_CONTAINS(PATH, NEEDLE) \
    assert_file_contains(__FILE__, __LINE__, FUNC__, \
                         PATH, NEEDLE)

#define ASSERT_CONTAINS(HAYSTACK, HAYSTACK_LEN, NEEDLE) \
    assert_contains(__FILE__, __LINE__, FUNC__, \
                    HAYSTACK, HAYSTACK_LEN, NEEDLE)

#define ASSERT_NOT_CONTAINS(HAYSTACK, HAYSTACK_LEN, NEEDLE) \
    assert_not_contains(__FILE__, __LINE__, FUNC__, \
                        HAYSTACK, HAYSTACK_LEN, NEEDLE)

#define ASSERT_FP_KIND_NONE    0
#define ASSERT_FP_KIND_FLOAT   1
#define ASSERT_FP_KIND_DOUBLE  2

#define ASSERT_FP_KIND_EXPR(VAR) \
_Generic((VAR), \
    float:  ASSERT_FP_KIND_FLOAT,  \
    double: ASSERT_FP_KIND_DOUBLE, \
    default: ASSERT_FP_KIND_NONE   \
)

#if !defined(ASSERT_FLOAT_REL_TOL)
#define ASSERT_FLOAT_REL_TOL   (16.0*(double)FLT_EPSILON)
#endif
#if !defined(ASSERT_DOUBLE_REL_TOL)
#define ASSERT_DOUBLE_REL_TOL  (16.0*(double)DBL_EPSILON)
#endif

#if !defined(ASSERT_FLOAT_ABS_TOL)
#define ASSERT_FLOAT_ABS_TOL   (16.0*(double)FLT_EPSILON)
#endif
#if !defined(ASSERT_DOUBLE_ABS_TOL)
#define ASSERT_DOUBLE_ABS_TOL  (16.0*(double)DBL_EPSILON)
#endif

#define A_BOTH_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2)             \
    a_both_signed_##MODE(__FILE__, __LINE__, FUNC__,    \
                         #VAR1, #VAR2,                            \
                         typename(TYPE1), typename(TYPE2),        \
                         typebits(TYPE1), typebits(TYPE2),        \
                         (llong)(VAR1), (llong)(VAR2))

#define A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2)         \
    a_signed_unsigned##MODE(__FILE__, __LINE__, FUNC__, \
                            #VAR1, #VAR2,                         \
                            typename(TYPE1), typename(TYPE2),     \
                            typebits(TYPE1), typebits(TYPE2),     \
                            (llong)(VAR1), (ullong)(VAR2))

#if CHAR_MIN < 0
#define A_CHAR_FOR_SIGNED(MODE, VAR1, VAR2, TYPE1) \
    A_BOTH_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR)

#define A_CHAR_FOR_UNSIGNED(MODE, VAR1, VAR2, TYPE1) \
    A_UNSIGNED_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR)

#define A_FIRST_CHAR(MODE, VAR1, VAR2) \
    A_FIRST_SIGNED(MODE, VAR1, VAR2, TYPE_CHAR)
#else
#define A_CHAR_FOR_SIGNED(MODE, VAR1, VAR2, TYPE1) \
    A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR)

#define A_CHAR_FOR_UNSIGNED(MODE, VAR1, VAR2, TYPE1) \
    A_BOTH_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR)

#define A_FIRST_CHAR(MODE, VAR1, VAR2) \
    A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_CHAR)
#endif

#define A_FIRST_SIGNED(MODE, VAR1, VAR2, TYPE1)                        \
_Generic((VAR2),                                                       \
    char:    A_CHAR_FOR_SIGNED(MODE, VAR1, VAR2, TYPE1),               \
    schar:   A_BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_SCHAR  ), \
    short:   A_BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_SHORT  ), \
    int:     A_BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_INT    ), \
    long:    A_BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_LONG   ), \
    llong:   A_BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_LLONG  ), \
    uchar:   A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_UCHAR  ), \
    ushort:  A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_USHORT ), \
    uint:    A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_UINT   ), \
    ulong:   A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_ULONG  ), \
    ullong:  A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_ULLONG ), \
    float:   A_BOTH_DOUBLE(MODE,    VAR1, VAR2, TYPE1, TYPE_FLOAT  ),  \
    double:  A_BOTH_DOUBLE(MODE,    VAR1, VAR2, TYPE1, TYPE_DOUBLE ),  \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_A_FIRST_SIGNED()             \
)
#define A_BOTH_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2)             \
    a_both_unsigned_##MODE(__FILE__, __LINE__, FUNC__,    \
                           #VAR1, #VAR2,                            \
                           typename(TYPE1), typename(TYPE2),        \
                           typebits(TYPE1), typebits(TYPE2),        \
                           (ullong)(VAR1), (ullong)(VAR2))

#define A_UNSIGNED_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2)           \
    a_unsigned_signed_##MODE(__FILE__, __LINE__, FUNC__,  \
                             #VAR1, #VAR2,                          \
                             typename(TYPE1), typename(TYPE2),      \
                             typebits(TYPE1), typebits(TYPE2),      \
                             (ullong)(VAR1), (llong)(VAR2))

#define A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE1)                        \
_Generic((VAR2),                                                         \
    char:    A_CHAR_FOR_UNSIGNED(MODE, VAR1, VAR2, TYPE1),               \
    schar:   A_UNSIGNED_SIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_SCHAR  ), \
    short:   A_UNSIGNED_SIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_SHORT  ), \
    int:     A_UNSIGNED_SIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_INT    ), \
    long:    A_UNSIGNED_SIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_LONG   ), \
    llong:   A_UNSIGNED_SIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_LLONG  ), \
    uchar:   A_BOTH_UNSIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_UCHAR  ), \
    ushort:  A_BOTH_UNSIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_USHORT ), \
    uint:    A_BOTH_UNSIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_UINT   ), \
    ulong:   A_BOTH_UNSIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_ULONG  ), \
    ullong:  A_BOTH_UNSIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_ULLONG ), \
    float:   A_BOTH_DOUBLE(MODE,       VAR1, VAR2, TYPE1, TYPE_FLOAT  ), \
    double:  A_BOTH_DOUBLE(MODE,       VAR1, VAR2, TYPE1, TYPE_DOUBLE ), \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_A_FIRST_UNSIGNED()             \
)
#define A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE2)                      \
    a_double_##MODE(__FILE__, __LINE__, FUNC__,                  \
                     #VAR1, #VAR2,                                         \
                     typename(TYPE1), typename(TYPE2),                     \
                     typebits(TYPE1), typebits(TYPE2),                     \
                     ASSERT_FP_KIND_EXPR(VAR1), ASSERT_FP_KIND_EXPR(VAR2), \
                     DOUBLE_GET2(VAR1, TYPE1), DOUBLE_GET2(VAR2, TYPE2))

#define A_FIRST_DOUBLE(MODE, VAR1, VAR2, TYPE1) \
_Generic((VAR2), \
    char:    A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR   ),     \
    schar:   A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_SCHAR  ),     \
    short:   A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_SHORT  ),     \
    int:     A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_INT    ),     \
    long:    A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_LONG   ),     \
    llong:   A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_LLONG  ),     \
    uchar:   A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_UCHAR  ),     \
    ushort:  A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_USHORT ),     \
    uint:    A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_UINT   ),     \
    ulong:   A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_ULONG  ),     \
    ullong:  A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_ULLONG ),     \
    float:   A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_FLOAT  ),     \
    double:  A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_DOUBLE ),     \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_A_FIRST_DOUBLE()             \
)
#define A_FIRST_BOOL(MODE, VAR1, VAR2, TYPE1)                  \
_Generic((VAR2),                                               \
    bool: a_bool_##MODE(__FILE__, __LINE__, FUNC__,  \
                        #VAR1, #VAR2,                          \
                        typename(TYPE1), typename(TYPE_BOOL),  \
                        typebits(TYPE1), typebits(TYPE_BOOL),  \
                        (VAR1), (VAR2)),                       \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_A_FIRST_BOOL()       \
)
#define A_POINTERS(MODE, VAR1, VAR2)                           \
    a_pointers_##MODE(__FILE__, __LINE__, FUNC__,    \
                      #VAR1, #VAR2,                            \
                      (void *)(uintptr_t)(VAR1),               \
                      (void *)(uintptr_t)(VAR2))

#define ASSERT_COMPARE(MODE, VAR1, VAR2)                                \
_Generic((VAR1),                                                        \
    void *: _Generic((VAR2),                                            \
        char *: A_POINTERS(MODE, VAR1, VAR2),                           \
        void *: A_POINTERS(MODE, VAR1, VAR2),                           \
        default: UNSUPPORTED_TYPE_FOR_GENERIC_ASSERT_COMPARE_VOIDP()    \
    ),                                                                  \
    char *: _Generic((VAR2),                                            \
        char *: a_strings_##MODE(__FILE__, __LINE__, FUNC__,  \
                                 #VAR1, #VAR2,                          \
                                 (char *)(uintptr_t)(VAR1),             \
                                 (char *)(uintptr_t)(VAR2)),            \
        void *: A_POINTERS(MODE, VAR1, VAR2),                           \
        default: UNSUPPORTED_TYPE_FOR_GENERIC_ASSERT_COMPARE_CHARP()    \
    ),                                                                  \
    char:    A_FIRST_CHAR(MODE,     VAR1, VAR2),                        \
    schar:   A_FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_SCHAR  ),          \
    short:   A_FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_SHORT  ),          \
    int:     A_FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_INT    ),          \
    long:    A_FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_LONG   ),          \
    llong:   A_FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_LLONG  ),          \
    uchar:   A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_UCHAR  ),          \
    ushort:  A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_USHORT ),          \
    uint:    A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_UINT   ),          \
    ulong:   A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_ULONG  ),          \
    ullong:  A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_ULLONG ),          \
    float:   A_FIRST_DOUBLE(MODE,   VAR1, VAR2, TYPE_FLOAT  ),          \
    double:  A_FIRST_DOUBLE(MODE,   VAR1, VAR2, TYPE_DOUBLE ),          \
    bool:    A_FIRST_BOOL(MODE,     VAR1, VAR2, TYPE_BOOL),             \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_ASSERT_COMPARE()              \
)

#define ASSERT_EQUAL(VAR1, VAR2)      ASSERT_COMPARE(equal,      VAR1, VAR2)
#define ASSERT_NOT_EQUAL(VAR1, VAR2)  ASSERT_COMPARE(not_equal,  VAR1, VAR2)
#define ASSERT_LESS(VAR1, VAR2)       ASSERT_COMPARE(less,       VAR1, VAR2)
#define ASSERT_LESS_EQUAL(VAR1, VAR2) ASSERT_COMPARE(less_equal, VAR1, VAR2)
#define ASSERT_MORE(VAR1, VAR2)       ASSERT_COMPARE(more,       VAR1, VAR2)
#define ASSERT_MORE_EQUAL(VAR1, VAR2) ASSERT_COMPARE(more_equal, VAR1, VAR2)

#define ASSERT_NULL(VAR1) do {                                          \
    void *p = VAR1;                                                     \
    if (p != NULL) {                                                    \
        error2("\nAssertion failed at %s:%d:%s\n",                      \
               __FILE__, __LINE__, __func__);                           \
        error2("%s = %p == NULL\n", #VAR1, p);                          \
        TRAP();                                                         \
    }                                                                   \
} while (0)

#include "arena.h"
#define UTF_INVALID 0xFFFD

static int32 random_utf8_string(char *, int32, int32);
static int32 utf8_byte_position(char *, int32, int32);
static int32 utf8_capitalize_first_letters(char *, int32, char *, int32);
static int32 utf8_char_width(uint32);
static int32 utf8_characters(char *, int32);
static int32 utf8_cut_width(char *, int32, int32);
static int32 utf8_decode(char *, int32, uint32 *);
static uint32 utf8_decode_byte(char, int32 *);
static int32 utf8_decode_raw(char *, uint32 *, int32);
static int32 utf8_encode(uint32, char *, int32);
static char utf8_encode_byte(uint32, int32);
static int32 utf8_encode_raw(uint32, char *);
static void utf8_functions_sink(void);
static int32 utf8_next_position(char *, int32, int32);
static int32 utf8_suffix_width_position(char *, int32, int32);
static int32 utf8_validate(uint32 *, int32);
static int32 utf8_width(char *, int32);

#include <dirent.h>
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

#if OS_UNIX
typedef struct UtilCopyFilesAsync {
    struct pollfd pipes[MAX_FILES_COPY];
    int dests[MAX_FILES_COPY];
    int32 nfds;
    int32 unused;
} UtilCopyFilesAsync;

static int32 util_copy_file_async(char *, char *, int *);
static void util_copy_file_async_parsed(UtilCopyFilesAsync *);
static void *util_copy_file_async_thread(void *);
#endif

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

enum CommandFlag {
    COMMAND_FLAG_NONE = 0,
    COMMAND_FLAG_CAPTURE_STDOUT = 1 << 0,
    COMMAND_FLAG_CAPTURE_STDERR = 1 << 1,
    COMMAND_FLAG_MERGE_STDERR = 1 << 2,
    COMMAND_FLAG_ASYNC = 1 << 3,
    COMMAND_FLAG_DETACHED = 1 << 4,
    COMMAND_FLAG_NEW_SESSION = 1 << 5,
    COMMAND_FLAG_NEW_PROCESS_GROUP = 1 << 6,
    COMMAND_FLAG_STDIN_TTY = 1 << 7,
    COMMAND_FLAG_CLOSE_STDIN = 1 << 8,
    COMMAND_FLAG_LAST,
};

typedef struct CommandResult {
    char *output;
    char *stdout_output;
    char *stderr_output;

    int64 pid;

    int32 output_len;
    int32 stdout_len;
    int32 stderr_len;
    int32 status;
    int32 error_status;
    int32 exit_status;
    int32 term_signal;
    int32 stdout_fd;
    int32 stderr_fd;

    bool exited;
    bool signaled;
    bool stdout_fd_open;
    bool stderr_fd_open;
} CommandResult;

typedef struct Command {
    char **argv;
    char **env;
    char *cwd;

    int32 *argvs_lens;
    int32 *env_lens;
    int32 cwd_len;
    int32 argc;
    int32 env_len;
    int32 cap;
    int32 env_cap;
    int32 error_status;

    CommandResult result;
} Command;

static char *COMMAND_FLAG_str(enum CommandFlag);
static void COMMAND_FLAG_str_free(char *);
static enum CommandFlag COMMAND_FLAG_parse(char *);
static bool command_flag_token_equal(char *, int32, char *);
static void command_argv0_set(Command *, char *);
static void command_child_env_apply(Command *);
static void command_child_exec(
    Command *, enum CommandFlag, int [2], int [2]
) __attribute__((noreturn));
static void command_cwd_clear(Command *);
static void command_cwd_set(Command *, char *);
static void command_env_clear(Command *);
static void command_env_printf(Command *, char *, ...);
static void command_env_push(Command *, char *);
static void command_env_push_length(Command *, char *, int32);
static void command_error_set(Command *, int32);
static bool command_flags_capture(enum CommandFlag);
static bool command_flags_capture_stderr(enum CommandFlag);
static bool command_flags_capture_stdout(enum CommandFlag);
static enum CommandFlag command_flags_normalized(enum CommandFlag);
static void command_free(Command *);
static void command_print(Command *);
static void command_printf(Command *, char *, ...);
static void command_push(Command *, char *);
static void command_push_length(Command *, char *, int32);
static void command_push_owned_length(
    char ***,
    int32 **,
    int32 *,
    int32 *,
    char *,
    int32
);
static void command_push_split(Command *, char *, char *);
static void command_reset(Command *);
static void command_result_append(
    StrBuilder *,
    StrBuilder *,
    StrBuilder *,
    bool,
    char *,
    int32
);
static void command_result_file_descriptors_close(CommandResult *);
static void command_result_free(CommandResult *);
static void command_result_init(CommandResult *);
static void command_result_read_captured(Command *);
static bool command_run(Command *, enum CommandFlag);
static bool command_run_async(Command *, enum CommandFlag);
static bool command_run_capture(Command *, enum CommandFlag);
static bool command_run_capture_all(Command *);
static bool command_run_capture_combined(Command *);
static bool command_run_sync(Command *, int *);
static bool command_signal(Command *, int32, bool);
static bool command_start(Command *, enum CommandFlag);
static int32 command_status_from_wait(int, CommandResult *);
static char *command_str(Command *, int32 *);
static void command_vector_reserve(char ***, int32 **, int32 *, int32, int32);
static bool command_wait(Command *);

#define COMMAND_PUSH_2(A, B) command_push(A, B)
#define COMMAND_PUSH_3(A, B, B_LEN) command_push_length(A, B, B_LEN)
#define COMMAND_PUSH(...) SELECT_ON_NUM_ARGS(COMMAND_PUSH_, __VA_ARGS__)

#define COMMAND_ENV_PUSH_2(A, B) command_env_push(A, B)
#define COMMAND_ENV_PUSH_3(A, B, B_LEN) \
    command_env_push_length(A, B, B_LEN)
#define COMMAND_ENV_PUSH(...) \
    SELECT_ON_NUM_ARGS(COMMAND_ENV_PUSH_, __VA_ARGS__)

#if !defined(SORT_COMPARE)
#define SORT_COMPARE(A, B) compare_func(A, B)
#endif

#if !defined(MAX_NTHREADS)
#define MAX_NTHREADS 64
#endif

typedef struct HeapNode {
    void *value;
    int32 p_index;
    int32 unused;
} HeapNode;

static void sort_functions_sink(void);
static void sort_heapify(HeapNode *, int32, int32, int32 (*)(void *, void *));
static void sort_merge_subsorted(
    void *,
    int32,
    int32,
    int64,
    int32 (*)(void *, void *)
);
static void sort_shuffle(void *, int64, int64);

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

#include "meta.h"

#endif /* CBASE_H */

#if defined(CBASE_IMPLEMENT) && !defined(CBASE_IMPLEMENTED)
#define CBASE_IMPLEMENTED 1

#include "generic.c"
#include "assert.c"
#include "minmax.c"
#include "arena.c"
#include "memory.c"
#include "utf8.c"
#include "util.c"
#include "command.c"
#include "sort.c"
#include "array.c"
#include "meta_common.c"
#include "meta_tokenize.c"
#include "meta_parse.c"
#include "meta_generate.c"

#endif /* CBASE_IMPLEMENT && !CBASE_IMPLEMENTED */
