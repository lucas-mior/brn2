// SPDX-License-Identifier: AGPL
// Copyright (c) 2026 Lucas Mior

#if !defined(GENERIC_H)
#define GENERIC_H

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_generic 1
#elif !defined(TESTING_generic)
#define TESTING_generic 0
#endif

#include "platform_detection.h"
#include "primitives.h"
#include "base_macros.h"
#include "libc.h"

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

static int strlen2(char *string) {
    return (int)strlen(string);
}

static inline int
fprint_0(FILE *restrict fp, ... /* strings, NULL */) {
    int count = 0;
    char *s;

    va_list ap;
    va_start(ap, fp);

    while ((s = (char *)va_arg(ap, char *))) {
        int64 slen;
        if (fputs(s, fp) == EOF) {
            va_end(ap);
            return -1;
        }

        slen = strlen2(s);
        if ((int64)INT_MAX - (int64)count < slen) {
            count = INT_MAX;
        } else {
            count += (int32)slen;
        }
    }

    va_end(ap);
    return count;
}

static inline int
snprint_0(char *restrict buf, int64 bufSize, ... /* strings, NULL */) {
    va_list ap;
    int64 remainingLen;
    int64 requiredLen = 0;
    char *dst = buf;
    char *s;

    assert(bufSize >= 0);
    if (bufSize) {
        remainingLen = bufSize - 1;
    } else {
        remainingLen = 0;
    }

    va_start(ap, bufSize);

    if (buf && bufSize) {
        buf[0] = '\0';
    }

    while ((s = va_arg(ap, char *))) {
        int64 sLen = strlen2(s);
        requiredLen += sLen;

        if (dst && remainingLen) {
            int64 copyLen;
            if (remainingLen < sLen) {
                copyLen = remainingLen;
            } else {
                copyLen = sLen;
            }
            memcpy(dst, s, (size_t)copyLen);
            dst += copyLen;
            remainingLen -= copyLen;
            *dst = '\0';
        }
    }

    va_end(ap);
    if (requiredLen > (int64)INT_MAX) {
        return INT_MAX;
    }
    return (int)requiredLen;
}

/* Like snprintf but returns a pointer to the buffer. */
static inline char *
toString(char *restrict buf, int64 bufSize, char *restrict fmt, ...) {
    va_list ap;

    assert(buf);
    assert(bufSize > 0);
    assert(fmt);

    va_start(ap, fmt);
    vsnprintf(buf, (size_t)bufSize, fmt, ap);
    va_end(ap);
    return buf;
}

static double UNUSED
double_from_voidp(void* x) {
    (void)x;
    TRAP();
    return 0.0;  // NOLINT
}
static double UNUSED
double_from_charp(char* x) {
    (void)x;
    TRAP();
    return 0.0;  // NOLINT
}
static double UNUSED
double_from_bool(bool x) {
    (void)x;
    TRAP();
    return 0.0;  // NOLINT
}
static double UNUSED
double_from_char(char x) {
    (void)x;
    TRAP();
    return 0.0;  // NOLINT
}
static void
check_integer_fits_in_double(llong x) {
    if (x > (1ll << DBL_MANT_DIG)) {
        TRAP();
    }
    if (x < -(1ll << DBL_MANT_DIG)) {
        TRAP();
    }
    return;
}
static double UNUSED
double_from_schar(schar x) {
    return (double)x;
}
static double UNUSED
double_from_short(short x) {
    return (double)x;
}
static double UNUSED
double_from_int(int x) {
    return (double)x;
}
static double UNUSED
double_from_long(long x) {
    check_integer_fits_in_double((llong)x);
    return (double)x;
}
static double UNUSED
double_from_llong(llong x) {
    check_integer_fits_in_double(x);
    return (double)x;
}
static double UNUSED
double_from_uchar(uchar x) {
    return (double)x;
}
static double UNUSED
double_from_ushort(ushort x) {
    return (double)x;
}
static double UNUSED
double_from_uint(uint x) {
    return (double)x;
}
static double UNUSED
double_from_ulong(ulong x) {
#if ULONG_MAX >= LLONG_MAX
    if ((ullong)x >= (ullong)LLONG_MAX) {
        TRAP();
    }
#endif
    check_integer_fits_in_double((llong)x);
    return (double)x;
}
static double UNUSED
double_from_ullong(ullong x) {
    if (x >= (ullong)LLONG_MAX) {
        TRAP();
    }
    check_integer_fits_in_double((llong)x);
    return (double)x;
}
static double UNUSED
double_from_float(float x) {
    return (double)x;
}
static double UNUSED
double_from_double(double x)  {
    return (double)x;
}

static llong
typebits(enum Type type) {
    llong size = 0;
    union Primitive primitive;
    void **pointer;

    switch (type) {
    case TYPE_VOIDP:
        pointer = &(primitive.avoidp);
        size = ((char*)(pointer + 1)) - (char*)pointer;
        break;
    case TYPE_CHARP:
        pointer = (void*)&(primitive.acharp);
        size = ((char*)(pointer + 1)) - (char*)pointer;
        break;
    case TYPE_BOOL:    size = SIZEOF(bool);    break;
    case TYPE_CHAR:    size = SIZEOF(char);    break;
    case TYPE_SCHAR:   size = SIZEOF(schar);   break;
    case TYPE_SHORT:   size = SIZEOF(short);   break;
    case TYPE_INT:     size = SIZEOF(int);     break;
    case TYPE_LONG:    size = SIZEOF(long);    break;
    case TYPE_LLONG:   size = SIZEOF(llong);   break;
    case TYPE_UCHAR:   size = SIZEOF(uchar);   break;
    case TYPE_USHORT:  size = SIZEOF(ushort);  break;
    case TYPE_UINT:    size = SIZEOF(uint);    break;
    case TYPE_ULONG:   size = SIZEOF(ulong);   break;
    case TYPE_ULLONG:  size = SIZEOF(ullong);  break;
    case TYPE_FLOAT:   size = SIZEOF(float);   break;
    case TYPE_DOUBLE:  size = SIZEOF(double);  break;
    case TYPE_OTHER:
    default: TRAP();
    }
    return size*CHAR_BIT;
}

static char *
typename(enum Type type) {
    switch (type) {
    case TYPE_VOIDP:  return "void*";
    case TYPE_CHARP:  return "char*";
    case TYPE_BOOL:   return "bool";
    case TYPE_CHAR:   return "char";
    case TYPE_SCHAR:  return "schar";
    case TYPE_SHORT:  return "short";
    case TYPE_INT:    return "int";
    case TYPE_LONG:   return "long";
    case TYPE_LLONG:  return "llong";
    case TYPE_UCHAR:  return "uchar";
    case TYPE_USHORT: return "ushort";
    case TYPE_UINT:   return "uint";
    case TYPE_ULONG:  return "ulong";
    case TYPE_ULLONG: return "ullong";
    case TYPE_FLOAT:  return "float";
    case TYPE_DOUBLE: return "double";
    case TYPE_OTHER:
    default:           return "unknown type";
    }
}

static double
double_get(union Primitive var, enum Type type) {
    switch (type) {
    case TYPE_VOIDP:
        TRAP();
        break;  // NOLINT
    case TYPE_CHARP:
        TRAP();
        break;  // NOLINT
    case TYPE_BOOL:
        TRAP();
        break;  // NOLINT
    case TYPE_CHAR:
        TRAP();
        break;  // NOLINT
    case TYPE_SCHAR:
        return (double)var.aschar;
    case TYPE_SHORT:
        return (double)var.ashort;
    case TYPE_INT:
        return (double)var.aint;
    case TYPE_LONG:
        check_integer_fits_in_double(var.along);
        return (double)var.along;
    case TYPE_LLONG:
        check_integer_fits_in_double(var.allong);
        return (double)var.allong;
    case TYPE_UCHAR:
        return (double)var.auchar;
    case TYPE_USHORT:
        return (double)var.aushort;
    case TYPE_UINT:
        return (double)var.auint;
    case TYPE_ULONG:
#if ULONG_MAX >= LLONG_MAX
        if (var.aulong >= (ullong)LLONG_MAX) {
            TRAP();
        }
#endif
        check_integer_fits_in_double((llong)var.aulong);
        return (double)var.aulong;
    case TYPE_ULLONG:
        if (var.aullong >= (ullong)LLONG_MAX) {
            TRAP();
        }
        check_integer_fits_in_double((llong)var.aullong);
        return (double)var.aullong;
    case TYPE_FLOAT:
        return (double)var.afloat;
    case TYPE_DOUBLE:
        return (double)var.adouble;
    case TYPE_OTHER:
    default:
        TRAP();
        break;  // NOLINT
    }
    return (double)0.0;
}

#if 0 == TESTING_generic
static inline void
generic_functions_sink(void) {
    (void)generic_functions_sink;
    (void)typebits;
    (void)typename;
    (void)double_get;
    return;
}
#endif

void UNSUPPORTED_TYPE_FOR_DOUBLE_GET_GENERIC(void);

#if !defined(S_BSZ)
#define S_BSZ 64
#endif

#define fprint(FP, ...)        fprint_0((FP),          __VA_ARGS__, (char *)0)
#define snprint(BUF, BSZ, ...) snprint_0((BUF), (BSZ), __VA_ARGS__, (char *)0)
#define print0(...)            fprint_0(stdout,        __VA_ARGS__, (char *)0)

#define S_(X) \
toString((char[S_BSZ]){ "" }, S_BSZ, _Generic((X),      \
    void *:  "%p",                                      \
    char *:  "%s",                                      \
    bool:    "%i",                                      \
    char:    "%c",                                      \
    schar:   "%hhd",                                    \
    short:   "%hd",                                     \
    int:     "%d",                                      \
    long:    "%ld",                                     \
    llong:   "%lld",                                    \
    uchar:   "%hhu",                                    \
    ushort:  "%hu",                                     \
    uint:    "%u",                                      \
    ulong:   "%lu",                                     \
    ullong:  "%llu",                                    \
    float:   "%." QUOTE(FLT_DIG) "g",                   \
    double:  "%." QUOTE(DBL_DIG) "g",                   \
    default: "%p"                                       \
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

#define TYPEID(VAR)            \
_Generic((VAR),                \
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

#define TYPEBITS(VAR) (SIZEOF(VAR)*CHAR_BIT)

#define DOUBLE_GET(x)                                           \
_Generic((x),                                                   \
    void*:   double_from_voidp,                                 \
    char*:   double_from_charp,                                 \
    bool:    double_from_bool,                                  \
    char:    double_from_char,                                  \
    schar:   double_from_schar,                                 \
    short:   double_from_short,                                 \
    int:     double_from_int,                                   \
    long:    double_from_long,                                  \
    llong:   double_from_llong,                                 \
    uchar:   double_from_uchar,                                 \
    ushort:  double_from_ushort,                                \
    uint:    double_from_uint,                                  \
    ulong:   double_from_ulong,                                 \
    ullong:  double_from_ullong,                                \
    float:   double_from_float,                                 \
    double:  double_from_double,                                \
    default: UNSUPPORTED_TYPE_FOR_DOUBLE_GET_GENERIC            \
)(x)

#if CC_GCC || CC_CLANG
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
                  typename(TYPE), typebits(TYPE), #VAR, (CAST)(uintptr)(VAR))

#define PRINT_(VAR) \
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
    float:   PRINT_DOUBLE(VAR,   TYPE_FLOAT),                          \
    double:  PRINT_DOUBLE(VAR,   TYPE_DOUBLE),                         \
    default: 0                                                         \
)

#if CC_GCC || CC_CLANG
#define PRINT_DIAGNOSTIC_PUSH() do {                          \
    _Pragma("GCC diagnostic push")                            \
    _Pragma("GCC diagnostic ignored \"-Wpedantic\"")          \
} while (0)
#define PRINT_DIAGNOSTIC_POP() do {                           \
    _Pragma("GCC diagnostic pop")                             \
} while (0)
#define PRINT(VAR) do {                                       \
    PRINT_DIAGNOSTIC_PUSH();                                  \
    PRINT_(VAR);                                              \
    PRINT_DIAGNOSTIC_POP();                                   \
} while (0)
#else
#define PRINT(VAR) PRINT_(VAR)
#endif

#define PRINTLN(VAR) do {                                         \
    fprintf(stderr, "%s:%d %s():", __FILE__, __LINE__, __func__); \
    PRINT(VAR);                                                   \
    fprintf(stderr, "\n");                                        \
} while (0)

#endif /* GENERIC_H */
