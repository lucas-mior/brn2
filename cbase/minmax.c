// SPDX-License-Identifier: AGPL
// Copyright (c) 2026 Lucas Mior

#if !defined(MINMAX_C)
#define MINMAX_C

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_minmax 1
#elif !defined(TESTING_minmax)
#define TESTING_minmax 0
#endif

#include "libc.h"
#include "primitives.h"
#include "base_macros.h"
#include "platform_detection.h"

#define GENERATE_COMPARE_POINTERS(MODE, SYMBOL) \
static void * \
get_pointer_##MODE(void *var1, void *var2) { \
    if ((uintptr)var1 SYMBOL (uintptr)var2) { \
        return var1; \
    } else { \
        return var2; \
    } \
}

GENERATE_COMPARE_POINTERS(min, <)
GENERATE_COMPARE_POINTERS(max, >)

#undef GENERATE_COMPARE_POINTERS

#define GENERATE_COMPARE_INTEGERS_SAME_SIGN(TYPE, SYMBOL, MODE) \
static TYPE long long \
get_both_##TYPE##_##MODE(TYPE long long var1, TYPE long long var2) { \
    if (var1 SYMBOL var2) { \
        return var1; \
    } else { \
        return var2; \
    } \
}

GENERATE_COMPARE_INTEGERS_SAME_SIGN(signed,   <,  min)
GENERATE_COMPARE_INTEGERS_SAME_SIGN(unsigned, <,  min)
GENERATE_COMPARE_INTEGERS_SAME_SIGN(signed,   >,  max)
GENERATE_COMPARE_INTEGERS_SAME_SIGN(unsigned, >,  max)

#undef GENERATE_COMPARE_INTEGERS_SAME_SIGN

#define GENERATE_COMPARE_INTEGERS(TYPE, NAME, SYMBOL, MODE) \
static TYPE \
get_both_##NAME##_##MODE(TYPE var1, TYPE var2) { \
    if (var1 SYMBOL var2) { \
        return var1; \
    } else { \
        return var2; \
    } \
}

GENERATE_COMPARE_INTEGERS(char,   char,   <, min)
GENERATE_COMPARE_INTEGERS(schar,  schar,  <, min)
GENERATE_COMPARE_INTEGERS(short,  short,  <, min)
GENERATE_COMPARE_INTEGERS(int,    int,    <, min)
GENERATE_COMPARE_INTEGERS(long,   long,   <, min)
GENERATE_COMPARE_INTEGERS(llong,  llong,  <, min)
GENERATE_COMPARE_INTEGERS(uchar,  uchar,  <, min)
GENERATE_COMPARE_INTEGERS(ushort, ushort, <, min)
GENERATE_COMPARE_INTEGERS(uint,   uint,   <, min)
GENERATE_COMPARE_INTEGERS(ulong,  ulong,  <, min)
GENERATE_COMPARE_INTEGERS(ullong, ullong, <, min)
GENERATE_COMPARE_INTEGERS(char,   char,   >, max)
GENERATE_COMPARE_INTEGERS(schar,  schar,  >, max)
GENERATE_COMPARE_INTEGERS(short,  short,  >, max)
GENERATE_COMPARE_INTEGERS(int,    int,    >, max)
GENERATE_COMPARE_INTEGERS(long,   long,   >, max)
GENERATE_COMPARE_INTEGERS(llong,  llong,  >, max)
GENERATE_COMPARE_INTEGERS(uchar,  uchar,  >, max)
GENERATE_COMPARE_INTEGERS(ushort, ushort, >, max)
GENERATE_COMPARE_INTEGERS(uint,   uint,   >, max)
GENERATE_COMPARE_INTEGERS(ulong,  ulong,  >, max)
GENERATE_COMPARE_INTEGERS(ullong, ullong, >, max)

#undef GENERATE_COMPARE_INTEGERS

static int
minmax_compare_sign_with_unsign(llong s, ullong u) {
    ullong saux;
    if (s < 0) {
        return -1;
    }
    saux = (ullong)s;
    if (saux < u) {
        return -1;
    } else if (saux == u) {
        return 0;
    } else {
        return +1;
    }
}

#define GENERATE_COMPARE_SIGNED_UNSIGNED(MODE, SYMBOL) \
static llong \
get_signed_unsigned_##MODE(llong var1, ullong var2) { \
    if ((minmax_compare_sign_with_unsign(var1, var2) SYMBOL 0)) { \
        return var1; \
    } else { \
        if (var2 > LLONG_MAX) { \
            fprintf(stderr, "You are working with a too large number.\n"); \
            TRAP(); \
        } \
        return (llong)var2; \
    } \
}

GENERATE_COMPARE_SIGNED_UNSIGNED(min, <)
GENERATE_COMPARE_SIGNED_UNSIGNED(max, >)

#undef GENERATE_COMPARE_SIGNED_UNSIGNED

#define GENERATE_COMPARE_UNSIGNED_SIGNED(MODE, SYMBOL) \
static llong \
get_unsigned_signed_##MODE(ullong var1, llong var2) { \
    if (((-minmax_compare_sign_with_unsign(var2, var1)) SYMBOL 0)) { \
        if (var1 > LLONG_MAX) { \
            fprintf(stderr, "You are working with a too large number.\n"); \
            TRAP(); \
        } \
        return (llong)var1; \
    } else { \
        return var2; \
    } \
}

GENERATE_COMPARE_UNSIGNED_SIGNED(min, <)
GENERATE_COMPARE_UNSIGNED_SIGNED(max, >)

#undef GENERATE_COMPARE_UNSIGNED_SIGNED

#define GENERATE_COMPARE_DOUBLE(MODE, SYMBOL) \
static double \
get_double_##MODE(double var1, double var2) { \
    if (var1 SYMBOL var2) { \
        return var1; \
    } else { \
        return var2; \
    } \
}

GENERATE_COMPARE_DOUBLE(min, <)
GENERATE_COMPARE_DOUBLE(max, >)

#undef GENERATE_COMPARE_DOUBLE

#if CC_GCC || CC_CLANG
enum MinmaxType {
    MINMAX_TYPE_VOIDP = 1,
    MINMAX_TYPE_CHARP,
    MINMAX_TYPE_CHAR,
    MINMAX_TYPE_SCHAR,
    MINMAX_TYPE_SHORT,
    MINMAX_TYPE_INT,
    MINMAX_TYPE_LONG,
    MINMAX_TYPE_LLONG,
    MINMAX_TYPE_UCHAR,
    MINMAX_TYPE_USHORT,
    MINMAX_TYPE_UINT,
    MINMAX_TYPE_ULONG,
    MINMAX_TYPE_ULLONG,
    MINMAX_TYPE_FLOAT,
    MINMAX_TYPE_DOUBLE,
    MINMAX_TYPE_OTHER = 0,
};

union MinmaxPrimitive {
    void *avoidp;
    char *acharp;
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

static bool
minmax_type_is_signed(enum MinmaxType type) {
    switch (type) {
#if CHAR_MIN < 0
    case MINMAX_TYPE_CHAR:
#endif
    case MINMAX_TYPE_SCHAR:
    case MINMAX_TYPE_SHORT:
    case MINMAX_TYPE_INT:
    case MINMAX_TYPE_LONG:
    case MINMAX_TYPE_LLONG:
        return true;
    case MINMAX_TYPE_UCHAR:
    case MINMAX_TYPE_USHORT:
    case MINMAX_TYPE_UINT:
    case MINMAX_TYPE_ULONG:
    case MINMAX_TYPE_ULLONG:
    case MINMAX_TYPE_OTHER:
    case MINMAX_TYPE_CHARP:
    case MINMAX_TYPE_VOIDP:
    case MINMAX_TYPE_FLOAT:
    case MINMAX_TYPE_DOUBLE:
    default:
        return false;
    }
}

static bool
minmax_type_is_unsigned(enum MinmaxType type) {
    switch (type) {
#if CHAR_MIN >= 0
    case MINMAX_TYPE_CHAR:
#endif
    case MINMAX_TYPE_UCHAR:
    case MINMAX_TYPE_USHORT:
    case MINMAX_TYPE_UINT:
    case MINMAX_TYPE_ULONG:
    case MINMAX_TYPE_ULLONG:
        return true;
    case MINMAX_TYPE_SCHAR:
    case MINMAX_TYPE_SHORT:
    case MINMAX_TYPE_INT:
    case MINMAX_TYPE_LONG:
    case MINMAX_TYPE_LLONG:
    case MINMAX_TYPE_VOIDP:
    case MINMAX_TYPE_CHARP:
    case MINMAX_TYPE_OTHER:
    case MINMAX_TYPE_DOUBLE:
    case MINMAX_TYPE_FLOAT:
#if CHAR_MIN < 0
    case MINMAX_TYPE_CHAR:
#endif
    default:
        return false;
    }
}

static void *
minmax_pointer_value(union MinmaxPrimitive var, enum MinmaxType type) {
    switch (type) {
    case MINMAX_TYPE_VOIDP:
        return var.avoidp;
    case MINMAX_TYPE_CHARP:
        return var.acharp;
    default:
        TRAP();
    }
    return 0;
}

static llong
minmax_signed_value(union MinmaxPrimitive var, enum MinmaxType type) {
    switch (type) {
#if CHAR_MIN < 0
    case MINMAX_TYPE_CHAR:
        return (llong)var.achar;
#endif
    case MINMAX_TYPE_SCHAR:
        return (llong)var.aschar;
    case MINMAX_TYPE_SHORT:
        return (llong)var.ashort;
    case MINMAX_TYPE_INT:
        return (llong)var.aint;
    case MINMAX_TYPE_LONG:
        return (llong)var.along;
    case MINMAX_TYPE_LLONG:
        return var.allong;
    default:
        TRAP();
    }
    return 0;
}

static ullong
minmax_unsigned_value(union MinmaxPrimitive var, enum MinmaxType type) {
    switch (type) {
#if CHAR_MIN >= 0
    case MINMAX_TYPE_CHAR:
        return (ullong)var.achar;
#endif
    case MINMAX_TYPE_UCHAR:
        return (ullong)var.auchar;
    case MINMAX_TYPE_USHORT:
        return (ullong)var.aushort;
    case MINMAX_TYPE_UINT:
        return (ullong)var.auint;
    case MINMAX_TYPE_ULONG:
        return (ullong)var.aulong;
    case MINMAX_TYPE_ULLONG:
        return var.aullong;
    default:
        TRAP();
    }
    return 0;
}

static void
minmax_check_integer_fits_in_double(llong x) {
    if (x > (1ll << DBL_MANT_DIG)) {
        TRAP();
    }
    if (x < -(1ll << DBL_MANT_DIG)) {
        TRAP();
    }
    return;
}

static double
minmax_double_value(union MinmaxPrimitive var, enum MinmaxType type) {
    switch (type) {
    case MINMAX_TYPE_SCHAR:
        return (double)var.aschar;
    case MINMAX_TYPE_SHORT:
        return (double)var.ashort;
    case MINMAX_TYPE_INT:
        return (double)var.aint;
    case MINMAX_TYPE_LONG:
        minmax_check_integer_fits_in_double(var.along);
        return (double)var.along;
    case MINMAX_TYPE_LLONG:
        minmax_check_integer_fits_in_double(var.allong);
        return (double)var.allong;
    case MINMAX_TYPE_UCHAR:
        return (double)var.auchar;
    case MINMAX_TYPE_USHORT:
        return (double)var.aushort;
    case MINMAX_TYPE_UINT:
        return (double)var.auint;
    case MINMAX_TYPE_ULONG:
        if ((ullong)var.aulong >= (ullong)LLONG_MAX) {
            TRAP();
        }
        minmax_check_integer_fits_in_double((llong)var.aulong);
        return (double)var.aulong;
    case MINMAX_TYPE_ULLONG:
        if (var.aullong >= (ullong)LLONG_MAX) {
            TRAP();
        }
        minmax_check_integer_fits_in_double((llong)var.aullong);
        return (double)var.aullong;
    case MINMAX_TYPE_FLOAT:
        return (double)var.afloat;
    case MINMAX_TYPE_DOUBLE:
        return var.adouble;
    default:
        TRAP();
    }
    return 0.0;
}

#define GENERATE_MINMAX_PRIMITIVE(MODE) \
static union MinmaxPrimitive \
minmax_primitive_##MODE( \
    union MinmaxPrimitive var1, \
    enum MinmaxType type1, \
    union MinmaxPrimitive var2, \
    enum MinmaxType type2, \
    enum MinmaxType result_type \
) { \
    union MinmaxPrimitive result; \
    switch (result_type) { \
    case MINMAX_TYPE_VOIDP: \
        result.avoidp = get_pointer_##MODE( \
            minmax_pointer_value(var1, type1), \
            minmax_pointer_value(var2, type2)); \
        break; \
    case MINMAX_TYPE_CHAR: \
        result.achar = get_both_char_##MODE(var1.achar, var2.achar); \
        break; \
    case MINMAX_TYPE_SCHAR: \
        result.aschar = get_both_schar_##MODE(var1.aschar, var2.aschar); \
        break; \
    case MINMAX_TYPE_SHORT: \
        result.ashort = get_both_short_##MODE(var1.ashort, var2.ashort); \
        break; \
    case MINMAX_TYPE_INT: \
        result.aint = get_both_int_##MODE(var1.aint, var2.aint); \
        break; \
    case MINMAX_TYPE_LONG: \
        result.along = get_both_long_##MODE(var1.along, var2.along); \
        break; \
    case MINMAX_TYPE_LLONG: \
        if (minmax_type_is_signed(type1) && minmax_type_is_unsigned(type2)) { \
            result.allong = get_signed_unsigned_##MODE( \
                minmax_signed_value(var1, type1), \
                minmax_unsigned_value(var2, type2)); \
        } else if (minmax_type_is_unsigned(type1) \
                   && minmax_type_is_signed(type2)) { \
            result.allong = get_unsigned_signed_##MODE( \
                minmax_unsigned_value(var1, type1), \
                minmax_signed_value(var2, type2)); \
        } else { \
            result.allong = get_both_signed_##MODE( \
                minmax_signed_value(var1, type1), \
                minmax_signed_value(var2, type2)); \
        } \
        break; \
    case MINMAX_TYPE_UCHAR: \
        result.auchar = get_both_uchar_##MODE(var1.auchar, var2.auchar); \
        break; \
    case MINMAX_TYPE_USHORT: \
        result.aushort = get_both_ushort_##MODE(var1.aushort, var2.aushort); \
        break; \
    case MINMAX_TYPE_UINT: \
        result.auint = get_both_uint_##MODE(var1.auint, var2.auint); \
        break; \
    case MINMAX_TYPE_ULONG: \
        result.aulong = get_both_ulong_##MODE(var1.aulong, var2.aulong); \
        break; \
    case MINMAX_TYPE_ULLONG: \
        result.aullong = get_both_unsigned_##MODE( \
            minmax_unsigned_value(var1, type1), \
            minmax_unsigned_value(var2, type2)); \
        break; \
    case MINMAX_TYPE_DOUBLE: \
        result.adouble = get_double_##MODE( \
            minmax_double_value(var1, type1), \
            minmax_double_value(var2, type2)); \
        break; \
    case MINMAX_TYPE_CHARP: \
    case MINMAX_TYPE_FLOAT: \
    case MINMAX_TYPE_OTHER: \
    default: \
        TRAP(); \
    } \
    return result; \
}

GENERATE_MINMAX_PRIMITIVE(min)
GENERATE_MINMAX_PRIMITIVE(max)

#undef GENERATE_MINMAX_PRIMITIVE
#endif

#if 0 == TESTING_minmax
static inline void
minmax_functions_sink(void) {
    (void)get_pointer_min;
    (void)get_pointer_max;
    (void)get_both_signed_min;
    (void)get_both_signed_max;
    (void)get_both_unsigned_min;
    (void)get_both_unsigned_max;
    (void)get_both_char_min;
    (void)get_both_char_max;
    (void)get_both_schar_min;
    (void)get_both_schar_max;
    (void)get_both_short_min;
    (void)get_both_short_max;
    (void)get_both_int_min;
    (void)get_both_int_max;
    (void)get_both_long_min;
    (void)get_both_long_max;
    (void)get_both_llong_min;
    (void)get_both_llong_max;
    (void)get_both_uchar_min;
    (void)get_both_uchar_max;
    (void)get_both_ushort_min;
    (void)get_both_ushort_max;
    (void)get_both_uint_min;
    (void)get_both_uint_max;
    (void)get_both_ulong_min;
    (void)get_both_ulong_max;
    (void)get_both_ullong_min;
    (void)get_both_ullong_max;
    (void)get_signed_unsigned_min;
    (void)get_signed_unsigned_max;
    (void)get_unsigned_signed_min;
    (void)get_unsigned_signed_max;
    (void)get_double_min;
    (void)get_double_max;
#if CC_GCC || CC_CLANG
    (void)minmax_type_is_signed;
    (void)minmax_type_is_unsigned;
    (void)minmax_pointer_value;
    (void)minmax_signed_value;
    (void)minmax_unsigned_value;
    (void)minmax_check_integer_fits_in_double;
    (void)minmax_double_value;
    (void)minmax_primitive_min;
    (void)minmax_primitive_max;
#endif
    return;
}
#endif

void UNSUPPORTED_TYPE_FOR_GENERIC_FIRST_SIGNED(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_FIRST_UNSIGNED(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_FIRST_DOUBLE(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_MINMAX_COMPARE_VOIDP(void);
void UNSUPPORTED_TYPE_FOR_GENERIC_MINMAX_COMPARE(void);

#define BOTH_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2) \
    get_both_signed_##MODE((llong)(VAR1), (llong)(VAR2))

#define MINMAX_CHAR(VAR) _Generic((VAR), char: (VAR), default: (char)0)
#define MINMAX_SCHAR(VAR) _Generic((VAR), schar: (VAR), default: (schar)0)
#define MINMAX_SHORT(VAR) _Generic((VAR), short: (VAR), default: (short)0)
#define MINMAX_INT(VAR) _Generic((VAR), int: (VAR), default: (int)0)
#define MINMAX_LONG(VAR) _Generic((VAR), long: (VAR), default: (long)0)
#define MINMAX_LLONG(VAR) _Generic((VAR), llong: (VAR), default: (llong)0)
#define MINMAX_UCHAR(VAR) _Generic((VAR), uchar: (VAR), default: (uchar)0)
#define MINMAX_USHORT(VAR) \
    _Generic((VAR), ushort: (VAR), default: (ushort)0)
#define MINMAX_UINT(VAR) _Generic((VAR), uint: (VAR), default: (uint)0)
#define MINMAX_ULONG(VAR) _Generic((VAR), ulong: (VAR), default: (ulong)0)
#define MINMAX_ULLONG(VAR) \
    _Generic((VAR), ullong: (VAR), default: (ullong)0)

#define BOTH_CHAR(MODE, VAR1, VAR2) \
    get_both_char_##MODE(MINMAX_CHAR(VAR1), MINMAX_CHAR(VAR2))

#define BOTH_SCHAR(MODE, VAR1, VAR2) \
    get_both_schar_##MODE(MINMAX_SCHAR(VAR1), MINMAX_SCHAR(VAR2))

#define BOTH_SHORT(MODE, VAR1, VAR2) \
    get_both_short_##MODE(MINMAX_SHORT(VAR1), MINMAX_SHORT(VAR2))

#define BOTH_INT(MODE, VAR1, VAR2) \
    get_both_int_##MODE(MINMAX_INT(VAR1), MINMAX_INT(VAR2))

#define BOTH_LONG(MODE, VAR1, VAR2) \
    get_both_long_##MODE(MINMAX_LONG(VAR1), MINMAX_LONG(VAR2))

#define BOTH_LLONG(MODE, VAR1, VAR2) \
    get_both_llong_##MODE(MINMAX_LLONG(VAR1), MINMAX_LLONG(VAR2))

#define BOTH_UCHAR(MODE, VAR1, VAR2) \
    get_both_uchar_##MODE(MINMAX_UCHAR(VAR1), MINMAX_UCHAR(VAR2))

#define BOTH_USHORT(MODE, VAR1, VAR2) \
    get_both_ushort_##MODE(MINMAX_USHORT(VAR1), MINMAX_USHORT(VAR2))

#define BOTH_UINT(MODE, VAR1, VAR2) \
    get_both_uint_##MODE(MINMAX_UINT(VAR1), MINMAX_UINT(VAR2))

#define BOTH_ULONG(MODE, VAR1, VAR2) \
    get_both_ulong_##MODE(MINMAX_ULONG(VAR1), MINMAX_ULONG(VAR2))

#define BOTH_ULLONG(MODE, VAR1, VAR2) \
    get_both_ullong_##MODE(MINMAX_ULLONG(VAR1), MINMAX_ULLONG(VAR2))

#define SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2) \
    get_signed_unsigned_##MODE((llong)(VAR1), (ullong)(VAR2))

#if CHAR_MIN < 0
#define CHAR_FOR_SIGNED(MODE, VAR1, VAR2, TYPE1) \
    BOTH_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR)

#define CHAR_FOR_UNSIGNED(MODE, VAR1, VAR2, TYPE1) \
    UNSIGNED_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR)

#define FIRST_CHAR(MODE, VAR1, VAR2) \
    FIRST_SIGNED(MODE, VAR1, VAR2, TYPE_CHAR)
#else
#define CHAR_FOR_SIGNED(MODE, VAR1, VAR2, TYPE1) \
    SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR)

#define CHAR_FOR_UNSIGNED(MODE, VAR1, VAR2, TYPE1) \
    BOTH_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR)

#define FIRST_CHAR(MODE, VAR1, VAR2) \
    FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_CHAR)
#endif

#define FIRST_SIGNED(MODE, VAR1, VAR2, TYPE1) \
_Generic((VAR2), \
    char:    CHAR_FOR_SIGNED(MODE, VAR1, VAR2, TYPE1), \
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
    char:    CHAR_FOR_UNSIGNED(MODE, VAR1, VAR2, TYPE1), \
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
    char:    BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR   ), \
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
    get_pointer_##MODE((void *)(uintptr)(VAR1), (void *)(uintptr)(VAR2))

#define MINMAX_COMPARE(MODE, VAR1, VAR2) \
_Generic((VAR1), \
    void *: _Generic((VAR2), \
        char *: POINTERS(MODE, VAR1, VAR2), \
        void *: POINTERS(MODE, VAR1, VAR2), \
        default: UNSUPPORTED_TYPE_FOR_GENERIC_MINMAX_COMPARE_VOIDP() \
    ), \
    char: _Generic((VAR2), \
        char:    BOTH_CHAR(MODE, VAR1, VAR2), \
        default: FIRST_CHAR(MODE, VAR1, VAR2) \
    ), \
    schar: _Generic((VAR2), \
        schar:   BOTH_SCHAR(MODE, VAR1, VAR2), \
        default: FIRST_SIGNED(MODE, VAR1, VAR2, TYPE_SCHAR) \
    ), \
    short: _Generic((VAR2), \
        short:   BOTH_SHORT(MODE, VAR1, VAR2), \
        default: FIRST_SIGNED(MODE, VAR1, VAR2, TYPE_SHORT) \
    ), \
    int: _Generic((VAR2), \
        int:     BOTH_INT(MODE, VAR1, VAR2), \
        default: FIRST_SIGNED(MODE, VAR1, VAR2, TYPE_INT) \
    ), \
    long: _Generic((VAR2), \
        long:    BOTH_LONG(MODE, VAR1, VAR2), \
        default: FIRST_SIGNED(MODE, VAR1, VAR2, TYPE_LONG) \
    ), \
    llong: _Generic((VAR2), \
        llong:   BOTH_LLONG(MODE, VAR1, VAR2), \
        default: FIRST_SIGNED(MODE, VAR1, VAR2, TYPE_LLONG) \
    ), \
    uchar: _Generic((VAR2), \
        uchar:   BOTH_UCHAR(MODE, VAR1, VAR2), \
        default: FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_UCHAR) \
    ), \
    ushort: _Generic((VAR2), \
        ushort:  BOTH_USHORT(MODE, VAR1, VAR2), \
        default: FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_USHORT) \
    ), \
    uint: _Generic((VAR2), \
        uint:    BOTH_UINT(MODE, VAR1, VAR2), \
        default: FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_UINT) \
    ), \
    ulong: _Generic((VAR2), \
        ulong:   BOTH_ULONG(MODE, VAR1, VAR2), \
        default: FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_ULONG) \
    ), \
    ullong: _Generic((VAR2), \
        ullong:  BOTH_ULLONG(MODE, VAR1, VAR2), \
        default: FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_ULLONG) \
    ), \
    float:   FIRST_DOUBLE(MODE,  VAR1, VAR2, TYPE_FLOAT  ), \
    double:  FIRST_DOUBLE(MODE,  VAR1, VAR2, TYPE_DOUBLE ), \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_MINMAX_COMPARE() \
)

#if CC_GCC || CC_CLANG
#define MINMAX_PRIMITIVE(VAR) ((union MinmaxPrimitive)(VAR))

#define MINMAX_TYPEID(VAR) \
_Generic((VAR), \
    void *:  MINMAX_TYPE_VOIDP, \
    char *:  MINMAX_TYPE_CHARP, \
    char:    MINMAX_TYPE_CHAR, \
    schar:   MINMAX_TYPE_SCHAR, \
    short:   MINMAX_TYPE_SHORT, \
    int:     MINMAX_TYPE_INT, \
    long:    MINMAX_TYPE_LONG, \
    llong:   MINMAX_TYPE_LLONG, \
    uchar:   MINMAX_TYPE_UCHAR, \
    ushort:  MINMAX_TYPE_USHORT, \
    uint:    MINMAX_TYPE_UINT, \
    ulong:   MINMAX_TYPE_ULONG, \
    ullong:  MINMAX_TYPE_ULLONG, \
    float:   MINMAX_TYPE_FLOAT, \
    double:  MINMAX_TYPE_DOUBLE, \
    default: MINMAX_TYPE_OTHER \
)

#if CHAR_MIN < 0
#define MINMAX_TYPE_IS_SIGNED_INTEGER(TYPE) \
    ((TYPE) == MINMAX_TYPE_CHAR \
     || (TYPE) == MINMAX_TYPE_SCHAR \
     || (TYPE) == MINMAX_TYPE_SHORT \
     || (TYPE) == MINMAX_TYPE_INT \
     || (TYPE) == MINMAX_TYPE_LONG \
     || (TYPE) == MINMAX_TYPE_LLONG)
#define MINMAX_TYPE_IS_UNSIGNED_INTEGER(TYPE) \
    ((TYPE) == MINMAX_TYPE_UCHAR \
     || (TYPE) == MINMAX_TYPE_USHORT \
     || (TYPE) == MINMAX_TYPE_UINT \
     || (TYPE) == MINMAX_TYPE_ULONG \
     || (TYPE) == MINMAX_TYPE_ULLONG)
#else
#define MINMAX_TYPE_IS_SIGNED_INTEGER(TYPE) \
    ((TYPE) == MINMAX_TYPE_SCHAR \
     || (TYPE) == MINMAX_TYPE_SHORT \
     || (TYPE) == MINMAX_TYPE_INT \
     || (TYPE) == MINMAX_TYPE_LONG \
     || (TYPE) == MINMAX_TYPE_LLONG)
#define MINMAX_TYPE_IS_UNSIGNED_INTEGER(TYPE) \
    ((TYPE) == MINMAX_TYPE_CHAR \
     || (TYPE) == MINMAX_TYPE_UCHAR \
     || (TYPE) == MINMAX_TYPE_USHORT \
     || (TYPE) == MINMAX_TYPE_UINT \
     || (TYPE) == MINMAX_TYPE_ULONG \
     || (TYPE) == MINMAX_TYPE_ULLONG)
#endif

#define MINMAX_TYPE_IS_INTEGER(TYPE) \
    (MINMAX_TYPE_IS_SIGNED_INTEGER(TYPE) \
     || MINMAX_TYPE_IS_UNSIGNED_INTEGER(TYPE))

#define MINMAX_TYPE_IS_DOUBLE(TYPE) \
    ((TYPE) == MINMAX_TYPE_FLOAT || (TYPE) == MINMAX_TYPE_DOUBLE)

#define MINMAX_TYPE_IS_POINTERS(TYPE1, TYPE2) \
    ((TYPE1) == MINMAX_TYPE_VOIDP \
     && ((TYPE2) == MINMAX_TYPE_VOIDP || (TYPE2) == MINMAX_TYPE_CHARP))

#define MINMAX_RESULT_TYPE(TYPE1, TYPE2) \
    (MINMAX_TYPE_IS_POINTERS(TYPE1, TYPE2) ? MINMAX_TYPE_VOIDP : \
     MINMAX_TYPE_IS_DOUBLE(TYPE1) || MINMAX_TYPE_IS_DOUBLE(TYPE2) \
         ? MINMAX_TYPE_DOUBLE : \
     ((TYPE1) == (TYPE2) && MINMAX_TYPE_IS_INTEGER(TYPE1)) ? (TYPE1) : \
     MINMAX_TYPE_IS_SIGNED_INTEGER(TYPE1) \
         && MINMAX_TYPE_IS_SIGNED_INTEGER(TYPE2) ? MINMAX_TYPE_LLONG : \
     MINMAX_TYPE_IS_UNSIGNED_INTEGER(TYPE1) \
         && MINMAX_TYPE_IS_UNSIGNED_INTEGER(TYPE2) ? MINMAX_TYPE_ULLONG : \
     MINMAX_TYPE_IS_INTEGER(TYPE1) && MINMAX_TYPE_IS_INTEGER(TYPE2) \
         ? MINMAX_TYPE_LLONG : \
     MINMAX_TYPE_OTHER)

#define MINMAX_FAST_RESULT(PRIMITIVE, TYPE) \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_VOIDP, (PRIMITIVE).avoidp, \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_CHAR, (PRIMITIVE).achar, \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_SCHAR, (PRIMITIVE).aschar, \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_SHORT, (PRIMITIVE).ashort, \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_INT, (PRIMITIVE).aint, \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_LONG, (PRIMITIVE).along, \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_LLONG, (PRIMITIVE).allong, \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_UCHAR, (PRIMITIVE).auchar, \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_USHORT, (PRIMITIVE).aushort, \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_UINT, (PRIMITIVE).auint, \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_ULONG, (PRIMITIVE).aulong, \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_ULLONG, (PRIMITIVE).aullong, \
__builtin_choose_expr((TYPE) == MINMAX_TYPE_DOUBLE, (PRIMITIVE).adouble, \
UNSUPPORTED_TYPE_FOR_GENERIC_MINMAX_COMPARE())))))))))))))

#define MINMAX_COMPARE_FAST(MODE, VAR1, VAR2) ({ \
    __auto_type minmax_var1 = (VAR1); \
    __auto_type minmax_var2 = (VAR2); \
    enum MinmaxType minmax_type1 = MINMAX_TYPEID(minmax_var1); \
    enum MinmaxType minmax_type2 = MINMAX_TYPEID(minmax_var2); \
    enum MinmaxType minmax_result_type = (enum MinmaxType)MINMAX_RESULT_TYPE( \
        MINMAX_TYPEID(minmax_var1), MINMAX_TYPEID(minmax_var2)); \
    union MinmaxPrimitive minmax_result = minmax_primitive_##MODE( \
        MINMAX_PRIMITIVE(minmax_var1), minmax_type1, \
        MINMAX_PRIMITIVE(minmax_var2), minmax_type2, \
        minmax_result_type); \
    MINMAX_FAST_RESULT( \
        minmax_result, \
        MINMAX_RESULT_TYPE(MINMAX_TYPEID(minmax_var1), \
                           MINMAX_TYPEID(minmax_var2))); \
})
#endif

#if defined(MIN)
#undef MIN
#endif
#if defined(MAX)
#undef MAX
#endif

#define MIN(A, B) MIN_IMPL(A, B)
#define MAX(A, B) MAX_IMPL(A, B)

#if CC_GCC || CC_CLANG
#define MIN_IMPL(VAR1, VAR2) MINMAX_COMPARE_FAST(min, VAR1, VAR2)
#define MAX_IMPL(VAR1, VAR2) MINMAX_COMPARE_FAST(max, VAR1, VAR2)
#else
#define MIN_IMPL(VAR1, VAR2) MINMAX_COMPARE(min, VAR1, VAR2)
#define MAX_IMPL(VAR1, VAR2) MINMAX_COMPARE(max, VAR1, VAR2)
#endif

#if TESTING_minmax
#define CBASE_IMPLEMENT
#include "cbase.h"

#define ASSERT_TYPE(EXPR, TYPE) \
    _Static_assert(_Generic((EXPR), TYPE: 1, default: 0), "unexpected type")

int
main(void) {
    {
        ASSERT_TYPE(MIN((char){ 0 }, (char){ 1 }), char);
        ASSERT_TYPE(MAX((schar){ 0 }, (schar){ 1 }), schar);
        ASSERT_TYPE(MIN((short){ 0 }, (short){ 1 }), short);
        ASSERT_TYPE(MAX((int){ 0 }, (int){ 1 }), int);
        ASSERT_TYPE(MIN((long){ 0 }, (long){ 1 }), long);
        ASSERT_TYPE(MAX((llong){ 0 }, (llong){ 1 }), llong);
        ASSERT_TYPE(MIN((uchar){ 0 }, (uchar){ 1 }), uchar);
        ASSERT_TYPE(MAX((ushort){ 0 }, (ushort){ 1 }), ushort);
        ASSERT_TYPE(MIN((uint){ 0 }, (uint){ 1 }), uint);
        ASSERT_TYPE(MAX((ulong){ 0 }, (ulong){ 1 }), ulong);
        ASSERT_TYPE(MIN((ullong){ 0 }, (ullong){ 1 }), ullong);
        ASSERT_TYPE(MIN((int){ 0 }, (uint){ 1 }), llong);
        ASSERT_TYPE(MAX((void *)0, (void *)0), void *);
    } {
        long min01 = MIN(0, 1);
        long min11 = MIN(1, 1);
        long max11 = MAX(1, 1);
        long max01 = MAX(0, 1);

        ASSERT_EQUAL(min01, 0);
        ASSERT_EQUAL(min11, 1);
        ASSERT_EQUAL(max11, 1);
        ASSERT_EQUAL(max01, 1);
    } {
        int a = 1;
        int b = 1;
        int min = MIN(a, b);
        int max = MAX(a, b);
        ASSERT_EQUAL(min, a);
        ASSERT_EQUAL(max, a);
    } {
        int a = 1;
        uint b = 2;
        long min = MIN(a, b);
        long max = MAX(a, b);
        ASSERT_EQUAL(min, a);
        ASSERT_EQUAL(max, b);
    } {
        long a = -1;
        ulong b = 0;
        long min = MIN(a, b);
        long max = MAX(a, b);
        ASSERT_EQUAL(max, b);
        ASSERT_EQUAL(min, a);
    } {
        long a = MINOF(a);
        ulong b = MAXOF(a);
        long min = MIN(a, b);
        ullong max = (ullong)MAX(a, b);
        ASSERT_EQUAL((long)min, a);
        ASSERT_EQUAL(max, b);
    } {
        ulong a = MINOF(a);
        long b = MAXOF(b);
        long min = MIN(a, b);
        long max = MAX(a, b);
        ASSERT_EQUAL(min, a);
        ASSERT_EQUAL(max, b);
    } {
        long a = -1;
        long min = MIN(a, 0);
        long max = MAX(a, 0);
        ASSERT_EQUAL(min, -1);
        ASSERT_EQUAL(max, 0);
    } {
        double a = 0.123;
        double min = MIN(a, 0);
        double max = MAX(a, 0);
        ASSERT_EQUAL(min, 0.0);
        ASSERT_EQUAL(max, a);
    } {
        char a = 1;
        int b = 0;
        long min = MIN(a, b);
        long max = MAX(a, b);
        ASSERT_EQUAL(min, b);
        ASSERT_EQUAL(max, a);
    } {
        char a = 1;
        uint b = 2;
        long min = MIN(a, b);
        long max = MAX(a, b);
        ASSERT_EQUAL(min, a);
        ASSERT_EQUAL(max, b);
    } {
        int array[100];
        void *a = &array[0];
        void *b = &array[1];
        void *min = MIN(a, b);
        void *max = MAX(a, b);
        ASSERT_EQUAL(min, a);
        ASSERT_EQUAL(max, b);
    }
    exit(EXIT_SUCCESS);
}
#endif

#endif /* MINMAX_C */
