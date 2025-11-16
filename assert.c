#if !defined(ASSERT_C)
#define ASSERT_C

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <assert.h>

#define error2(...) fprintf(stderr, __VA_ARGS__)

#if defined(__GNUC__) || defined(__clang__)
#define trap(...) __builtin_trap()
#elif defined(_MSC_VER)
#define trap(...) __debugbreak()
#else
#define trap(...) *(volatile int *)0 = 0
#endif

#define ASSERT(C) do { \
    if (!(C)) { \
        error2("Assertion '%s' failed at %s:%d\n", #C, __FILE__, __LINE__); \
        trap(); \
    } \
} while (0)

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_assert 1
#elif !defined(TESTING_assert)
#define TESTING_assert 0
#endif

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;

typedef signed char schar;
typedef long long llong;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef union DoubleUnion {
    double adouble;
    float afloat;
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
    char *stupid;
} DoubleUnion;

// clang-format off

#define STRING_COMPARE(MODE, SYMBOL) \
static void \
assert_strings_##MODE(char *file, uint line, \
                      char *name1, char *name2, \
                      char *var1, char *var2) { \
    if (!(strcmp(var1, var2) SYMBOL 0)) { \
        error2("\n%s: Assertion failed at %s:%u\n", __func__, file, line); \
        error2("%s = %s " #SYMBOL " %s = %s\n", \
               name1, var1, var2, name2); \
        trap(); \
    } \
}

STRING_COMPARE(less,       <)
STRING_COMPARE(less_equal, <=)
STRING_COMPARE(equal,      ==)
STRING_COMPARE(not_equal,  !=)
STRING_COMPARE(more,       >)
STRING_COMPARE(more_equal, >=)

#undef STRING_COMPARE

#define INTEGER_SAME_SIGN_COMPARE(TYPE, FORMAT, SYMBOL, MODE) \
static void \
assert_##TYPE##_##MODE(char *file, uint line, \
                       char *name1, char *name2, \
                       TYPE long long var1, TYPE long long var2) { \
    if (!(var1 SYMBOL var2)) { \
        error2("\n%s: Assertion failed at %s:%u\n", __func__, file, line); \
        error2("%s = "FORMAT" " #SYMBOL " "FORMAT" = %s\n", \
               name1, var1, var2, name2); \
        trap(); \
    } \
}

INTEGER_SAME_SIGN_COMPARE(signed,   "%lld", ==, equal)
INTEGER_SAME_SIGN_COMPARE(unsigned, "%llu", ==, equal)
INTEGER_SAME_SIGN_COMPARE(signed,   "%lld", !=, not_equal)
INTEGER_SAME_SIGN_COMPARE(unsigned, "%llu", !=, not_equal)
INTEGER_SAME_SIGN_COMPARE(signed,   "%lld", <,  less)
INTEGER_SAME_SIGN_COMPARE(unsigned, "%llu", <,  less)
INTEGER_SAME_SIGN_COMPARE(signed,   "%lld", <=, less_equal)
INTEGER_SAME_SIGN_COMPARE(unsigned, "%llu", <=, less_equal)
INTEGER_SAME_SIGN_COMPARE(signed,   "%lld", >,  more)
INTEGER_SAME_SIGN_COMPARE(unsigned, "%llu", >,  more)
INTEGER_SAME_SIGN_COMPARE(signed,   "%lld", >=, more_equal)
INTEGER_SAME_SIGN_COMPARE(unsigned, "%llu", >=, more_equal)

#undef INTEGER_SAME_SIGN_COMPARE

// clang-format on

static int
compare_unsign_with_sign(ullong u, llong s) {
    ullong saux;
    if (s < 0) {
        return 1;
    }
    saux = (ullong)s;
    if (saux < u) {
        return 1;
    } else if (saux == u) {
        return 0;
    } else {
        return -1;
    }
}

// clang-format off
#define COMPARE_SIGN_UNSIGN(MODE, SYMBOL) \
static void \
assert_si_un_##MODE(char *file, uint line, \
                    char *name1, char *name2, \
                    llong var1, ullong var2) { \
    if (!(compare_unsign_with_sign(var2, var1) SYMBOL 0)) { \
        error2("\n%s: Assertion failed at %s:%u\n", __func__, file, line); \
        error2("%s = %llu " #SYMBOL " %lld = %s\n", \
               name2, var2, var1, name1); \
        trap(); \
    } \
}

COMPARE_SIGN_UNSIGN(equal, ==)
COMPARE_SIGN_UNSIGN(not_equal, !=)
COMPARE_SIGN_UNSIGN(less, >)
COMPARE_SIGN_UNSIGN(less_equal, >=)
COMPARE_SIGN_UNSIGN(more, <)
COMPARE_SIGN_UNSIGN(more_equal, <=)

#define COMPARE_DOUBLE(MODE, SYMBOL) \
static void \
assert_float_##MODE(char *file, uint line, \
                    char *name1, char *name2, \
                    double var1, double var2) { \
    if (!(var1 SYMBOL var2)) { \
        error2("\n%s: Assertion failed at %s:%u\n", __func__, file, line); \
        error2("%s = %f " #SYMBOL " %f = %s\n", \
               name2, var2, var1, name1); \
        trap(); \
    } \
}

COMPARE_DOUBLE(equal, ==)
COMPARE_DOUBLE(not_equal, !=)
COMPARE_DOUBLE(less, >)
COMPARE_DOUBLE(less_equal, >=)
COMPARE_DOUBLE(more, <)
COMPARE_DOUBLE(more_equal, <=)

#undef COMPARE_SIGN_UNSIGN

#define COMPARE_UNSIGN_SIGN(MODE, SYMBOL) \
static void \
assert_un_si_##MODE(char *file, uint line, \
                    char *name1, char *name2, \
                    ullong var1, llong var2) { \
    if (!(compare_unsign_with_sign(var1, var2) SYMBOL 0)) { \
        error2("\n%s: Assertion failed at %s:%u\n", __func__, file, line); \
        error2("%s = %llu " #SYMBOL " %lld = %s\n", name1, var1, var2, name2); \
        trap(); \
    } \
}

COMPARE_UNSIGN_SIGN(equal, ==)
COMPARE_UNSIGN_SIGN(not_equal, !=)
COMPARE_UNSIGN_SIGN(less, <)
COMPARE_UNSIGN_SIGN(less_equal, <=)
COMPARE_UNSIGN_SIGN(more, >)
COMPARE_UNSIGN_SIGN(more_equal, >=)

#undef COMPARE_UNSIGN_SIGN

// clang-format on

// clang-format off
#define COMPARE_BOTH_SIGNED(MODE, VAR1, VAR2) \
  assert_signed_##MODE(__FILE__, __LINE__, \
                       #VAR1, #VAR2, \
                       (llong)(VAR1), (llong)(VAR2))

#define COMPARE_SI_UN(MODE, VAR1, VAR2) \
  assert_si_un_##MODE(__FILE__, __LINE__, \
                      #VAR1, #VAR2, \
                      (llong)(VAR1), (ullong)(VAR2))

static void unsupported_type_for_generic(void);

#define COMPARE_FIRST_IS_SIGNED(MODE, VAR1, VAR2) \
_Generic((VAR2), \
  schar:  COMPARE_BOTH_SIGNED(MODE, VAR1, VAR2), \
  short:  COMPARE_BOTH_SIGNED(MODE, VAR1, VAR2), \
  int:    COMPARE_BOTH_SIGNED(MODE, VAR1, VAR2), \
  long:   COMPARE_BOTH_SIGNED(MODE, VAR1, VAR2), \
  llong:  COMPARE_BOTH_SIGNED(MODE, VAR1, VAR2), \
  uchar:  COMPARE_SI_UN(MODE, VAR1, VAR2), \
  ushort: COMPARE_SI_UN(MODE, VAR1, VAR2), \
  uint:   COMPARE_SI_UN(MODE, VAR1, VAR2), \
  ulong:  COMPARE_SI_UN(MODE, VAR1, VAR2), \
  ullong: COMPARE_SI_UN(MODE, VAR1, VAR2), \
  default: (void)unsupported_type_for_generic \
)

#define COMPARE_BOTH_UNSIGNED(MODE, VAR1, VAR2) \
  assert_unsigned_##MODE(__FILE__, __LINE__, \
                         #VAR1, #VAR2, \
                         (ullong)(VAR1), (ullong)(VAR2))

#define COMPARE_UN_SI(MODE, VAR1, VAR2) \
  assert_un_si_##MODE(__FILE__, __LINE__, \
                      #VAR1, #VAR2, \
                      (ullong)(VAR1), (llong)(VAR2))

#define COMPARE_FIRST_IS_UNSIGNED(MODE, VAR1, VAR2) \
_Generic((VAR2), \
  uchar:  COMPARE_BOTH_UNSIGNED(MODE, VAR1, VAR2), \
  ushort: COMPARE_BOTH_UNSIGNED(MODE, VAR1, VAR2), \
  uint:   COMPARE_BOTH_UNSIGNED(MODE, VAR1, VAR2), \
  ulong:  COMPARE_BOTH_UNSIGNED(MODE, VAR1, VAR2), \
  ullong: COMPARE_BOTH_UNSIGNED(MODE, VAR1, VAR2), \
  schar:  COMPARE_UN_SI(MODE, VAR1, VAR2), \
  short:  COMPARE_UN_SI(MODE, VAR1, VAR2), \
  int:    COMPARE_UN_SI(MODE, VAR1, VAR2), \
  long:   COMPARE_UN_SI(MODE, VAR1, VAR2), \
  llong:  COMPARE_UN_SI(MODE, VAR1, VAR2), \
  default: unsupported_type_for_generic \
)

typedef enum DoubleType {
    DOUBLE_DOUBLE,
    DOUBLE_FLOAT,
    DOUBLE_SCHAR,
    DOUBLE_SHORT, 
    DOUBLE_INT,
    DOUBLE_LONG,
    DOUBLE_LLONG,
    DOUBLE_UCHAR,
    DOUBLE_USHORT,
    DOUBLE_UINT,
    DOUBLE_ULONG,
    DOUBLE_ULLONG,
    DOUBLE_CHAR_P,
} DoubleType;

static double
double_get(DoubleUnion var, DoubleType type) {
    switch (type) {
    case DOUBLE_DOUBLE:
        return var.adouble;
    case DOUBLE_FLOAT:
        return (double)var.afloat;
    case DOUBLE_SCHAR:
        return (double)var.aschar;
    case DOUBLE_SHORT: 
        return (double)var.ashort;
    case DOUBLE_INT:
        return (double)var.aint;
    case DOUBLE_LONG:
        return (double)var.along;
    case DOUBLE_LLONG:
        return (double)var.allong;
    case DOUBLE_UCHAR:
        return (double)var.auchar;
    case DOUBLE_USHORT:
        return (double)var.aushort;
    case DOUBLE_UINT:
        return (double)var.auint;
    case DOUBLE_ULONG:
        return (double)var.along;
    case DOUBLE_ULLONG:
        return (double)var.allong;
    case DOUBLE_CHAR_P:
        return 0.0;
    default:
        return 0.0;
    }
}

#define COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, VAR1_TYPE, VAR2_TYPE) \
  assert_float_##MODE(__FILE__, __LINE__, \
                      #VAR1, #VAR2, \
                      double_get((DoubleUnion)VAR1, VAR1_TYPE), \
                      double_get((DoubleUnion)VAR2, VAR2_TYPE))

#define COMPARE_FIRST_IS_DOUBLE(MODE, VAR1, VAR2) \
_Generic((VAR2), \
  double: COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_DOUBLE), \
  float:  COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_FLOAT), \
  schar:  COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_SCHAR), \
  short:  COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_SHORT), \
  int:    COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_INT), \
  long:   COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_LONG), \
  llong:  COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_LLONG), \
  uchar:  COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_UCHAR), \
  ushort: COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_USHORT), \
  uint:   COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_UINT), \
  ulong:  COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_ULONG), \
  ullong: COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_ULLONG), \
  char *: COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_DOUBLE, DOUBLE_CHAR_P) \
)

#define COMPARE_FIRST_IS_FLOAT(MODE, VAR1, VAR2) \
_Generic((VAR2), \
  double: COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_DOUBLE), \
  float:  COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_FLOAT), \
  schar:  COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_SCHAR), \
  short:  COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_SHORT), \
  int:    COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_INT), \
  long:   COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_LONG), \
  llong:  COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_LLONG), \
  uchar:  COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_UCHAR), \
  ushort: COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_USHORT), \
  uint:   COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_UINT), \
  ulong:  COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_ULONG), \
  ullong: COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_ULLONG), \
  char *: COMPARE_BOTH_DOUBLE(MODE, VAR1, VAR2, DOUBLE_FLOAT, DOUBLE_CHAR_P) \
)

#define ASSERT_COMPARE(MODE, VAR1, VAR2) \
_Generic((VAR1), \
  char *: _Generic((VAR2), \
    char *: assert_strings_##MODE(__FILE__, __LINE__, \
                                  #VAR1, #VAR2, \
                                  (char *)(uintptr_t)(VAR1), \
                                  (char *)(uintptr_t)(VAR2)), \
      default: unsupported_type_for_generic \
  ), \
  schar:  COMPARE_FIRST_IS_SIGNED(MODE, VAR1, VAR2), \
  short:  COMPARE_FIRST_IS_SIGNED(MODE, VAR1, VAR2), \
  int:    COMPARE_FIRST_IS_SIGNED(MODE, VAR1, VAR2), \
  long:   COMPARE_FIRST_IS_SIGNED(MODE, VAR1, VAR2), \
  llong:  COMPARE_FIRST_IS_SIGNED(MODE, VAR1, VAR2), \
  uchar:  COMPARE_FIRST_IS_UNSIGNED(MODE, VAR1, VAR2), \
  ushort: COMPARE_FIRST_IS_UNSIGNED(MODE, VAR1, VAR2), \
  uint:   COMPARE_FIRST_IS_UNSIGNED(MODE, VAR1, VAR2), \
  ulong:  COMPARE_FIRST_IS_UNSIGNED(MODE, VAR1, VAR2), \
  ullong: COMPARE_FIRST_IS_UNSIGNED(MODE, VAR1, VAR2), \
  double: COMPARE_FIRST_IS_DOUBLE(MODE, VAR1, VAR2), \
  float:  COMPARE_FIRST_IS_FLOAT(MODE, VAR1, VAR2) \
)

#define ASSERT_EQUAL(VAR1, VAR2)      ASSERT_COMPARE(equal, VAR1, VAR2)
#define ASSERT_NOT_EQUAL(VAR1, VAR2)  ASSERT_COMPARE(not_equal, VAR1, VAR2)
#define ASSERT_LESS(VAR1, VAR2)       ASSERT_COMPARE(less, VAR1, VAR2)
#define ASSERT_LESS_EQUAL(VAR1, VAR2) ASSERT_COMPARE(less_equal, VAR1, VAR2)
#define ASSERT_MORE(VAR1, VAR2)       ASSERT_COMPARE(more, VAR1, VAR2)
#define ASSERT_MORE_EQUAL(VAR1, VAR2) ASSERT_COMPARE(more_equal, VAR1, VAR2)

// clang-format on

#if TESTING_assert

#if !defined(MINOF)

#define MINOF(VARIABLE) \
_Generic((VARIABLE), \
  schar:       SCHAR_MIN, \
  short:       SHRT_MIN,  \
  int:         INT_MIN,   \
  long:        LONG_MIN,  \
  uchar:       0,         \
  ushort:      0,         \
  uint:        0u,        \
  ulong:       0ul,       \
  ullong:      0ull,      \
  char:        CHAR_MIN,  \
  bool:        0,         \
  float:       -FLT_MAX,  \
  double:      -DBL_MAX,  \
  long double: -LDBL_MAX  \
)

#endif

#if !defined(MAXOF)

#define MAXOF(VARIABLE) \
_Generic((VARIABLE), \
  schar:       SCHAR_MAX,  \
  short:       SHRT_MAX,   \
  int:         INT_MAX,    \
  long:        LONG_MAX,   \
  uchar:       UCHAR_MAX,  \
  ushort:      USHRT_MAX,  \
  uint:        UINT_MAX,   \
  ulong:       ULONG_MAX,  \
  ullong:      ULLONG_MAX, \
  char:        CHAR_MAX,   \
  bool:        1,          \
  float:       FLT_MAX,    \
  double:      DBL_MAX,    \
  long double: LDBL_MAX    \
)

#endif

int
main(void) {
    {
        int a = 1;
        int b = 1;
        ASSERT_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE_EQUAL(a, b);
    }
    {
        int a = 1;
        uint b = 1;
        ASSERT_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE_EQUAL(a, b);
    }
    {
        int a = 1;
        uint b = 2;
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    }
    {
        long a = -1;
        ulong b = 0;
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    }
    {
        long a = MINOF(a);
        ulong b = MAXOF(b);
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    }
    {
        ulong a = MINOF(a);
        long b = MAXOF(b);
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    }
    {
        long a = -2;
        long b = -1;
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    }
    {
        char *a = "aaa";
        char *b = "aaa";
        ASSERT_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE_EQUAL(b, a);
    }
    {
        char *a = "aaa";
        char *b = "bbb";
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    }
    {
        long a = -1;
        ASSERT_NOT_EQUAL(a, 0);
        ASSERT_LESS(a, 0);
        ASSERT_LESS_EQUAL(a, 0);
        ASSERT_MORE(0, a);
        ASSERT_MORE_EQUAL(0, a);
    }
    {
        long a = -1;
        double b = -1;
        ASSERT_EQUAL(a, b);
        ASSERT_EQUAL(b, b);
    }
    ASSERT(true);
    exit(EXIT_SUCCESS);
}
#endif

#endif
