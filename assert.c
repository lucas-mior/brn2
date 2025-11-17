#if !defined(ASSERT_C)
#define ASSERT_C

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <assert.h>
#include <signal.h>

#define error2(...) fprintf(stderr, __VA_ARGS__)

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_assert 1
#elif !defined(TESTING_assert)
#define TESTING_assert 0
#endif

#if TESTING_assert
#define trap(...) raise(SIGILL)
#else
#if defined(__GNUC__) || defined(__clang__)
#define trap(...) __builtin_trap()
#elif defined(_MSC_VER)
#define trap(...) __debugbreak()
#else
#define trap(...) *(volatile int *)0 = 0
#endif
#endif

#define ASSERT(C) do { \
    if (!(C)) { \
        error2("Assertion '%s' failed at %s:%d\n", #C, __FILE__, __LINE__); \
        trap(); \
    } \
} while (0)

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;

typedef signed char schar;
typedef long long llong;
typedef long double ldouble;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

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

#define POINTER_COMPARE(MODE, SYMBOL) \
static void \
assert_pointers_##MODE(char *file, uint line, \
                       char *name1, char *name2, \
                       void *var1, void *var2) { \
    if (!((uintptr_t)var1 SYMBOL (uintptr_t)var2)) { \
        error2("\n%s: Assertion failed at %s:%u\n", __func__, file, line); \
        error2("%s = %p " #SYMBOL " %p = %s\n", \
               name1, var1, var2, name2); \
        trap(); \
    } \
}

POINTER_COMPARE(less,       <)
POINTER_COMPARE(less_equal, <=)
POINTER_COMPARE(equal,      ==)
POINTER_COMPARE(not_equal,  !=)
POINTER_COMPARE(more,       >)
POINTER_COMPARE(more_equal, >=)

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

#define COMPARE_LDOUBLE(MODE, SYMBOL) \
static void \
assert_ldouble_##MODE(char *file, uint line, \
                      char *name1, char *name2, \
                      ldouble var1, ldouble var2) { \
    if (!(var1 SYMBOL var2)) { \
        error2("\n%s: Assertion failed at %s:%u\n", __func__, file, line); \
        error2("%s = %Lf " #SYMBOL " %Lf = %s\n", \
               name1, var1, var2, name2); \
        trap(); \
    } \
}

COMPARE_LDOUBLE(equal, ==)
COMPARE_LDOUBLE(not_equal, !=)
COMPARE_LDOUBLE(less, <)
COMPARE_LDOUBLE(less_equal, <=)
COMPARE_LDOUBLE(more, >)
COMPARE_LDOUBLE(more_equal, >=)

#undef COMPARE_LDOUBLE

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

void unsupported_type_for_generic(void);

#define COMPARE_FIRST_IS_SIGNED(MODE, VAR1, VAR2) \
_Generic((VAR2), \
  schar:   COMPARE_BOTH_SIGNED(MODE, VAR1, VAR2), \
  short:   COMPARE_BOTH_SIGNED(MODE, VAR1, VAR2), \
  int:     COMPARE_BOTH_SIGNED(MODE, VAR1, VAR2), \
  long:    COMPARE_BOTH_SIGNED(MODE, VAR1, VAR2), \
  llong:   COMPARE_BOTH_SIGNED(MODE, VAR1, VAR2), \
  uchar:   COMPARE_SI_UN(MODE, VAR1, VAR2), \
  ushort:  COMPARE_SI_UN(MODE, VAR1, VAR2), \
  uint:    COMPARE_SI_UN(MODE, VAR1, VAR2), \
  ulong:   COMPARE_SI_UN(MODE, VAR1, VAR2), \
  ullong:  COMPARE_SI_UN(MODE, VAR1, VAR2), \
  ldouble: COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  double:  COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  float:   COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  default: unsupported_type_for_generic() \
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
  uchar:   COMPARE_BOTH_UNSIGNED(MODE, VAR1, VAR2), \
  ushort:  COMPARE_BOTH_UNSIGNED(MODE, VAR1, VAR2), \
  uint:    COMPARE_BOTH_UNSIGNED(MODE, VAR1, VAR2), \
  ulong:   COMPARE_BOTH_UNSIGNED(MODE, VAR1, VAR2), \
  ullong:  COMPARE_BOTH_UNSIGNED(MODE, VAR1, VAR2), \
  schar:   COMPARE_UN_SI(MODE, VAR1, VAR2), \
  short:   COMPARE_UN_SI(MODE, VAR1, VAR2), \
  int:     COMPARE_UN_SI(MODE, VAR1, VAR2), \
  long:    COMPARE_UN_SI(MODE, VAR1, VAR2), \
  llong:   COMPARE_UN_SI(MODE, VAR1, VAR2), \
  ldouble: COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  double:  COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  float:   COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  default: unsupported_type_for_generic() \
)

static ldouble ldouble_from_ldouble(ldouble x) { return x;           }
static ldouble ldouble_from_double(double x)   { return (ldouble)x;    }
static ldouble ldouble_from_float(float x)     { return (ldouble)x;    }
static ldouble ldouble_from_schar(schar x)     { return (ldouble)x;    }
static ldouble ldouble_from_short(short x)     { return (ldouble)x;    }
static ldouble ldouble_from_int(int x)         { return (ldouble)x;    }
static ldouble ldouble_from_long(long x)       { return (ldouble)x;    }
static ldouble ldouble_from_llong(llong x)     { return (ldouble)x;    }
static ldouble ldouble_from_uchar(uchar x)     { return (ldouble)x;    }
static ldouble ldouble_from_ushort(ushort x)   { return (ldouble)x;    }
static ldouble ldouble_from_uint(uint x)       { return (ldouble)x;    }
static ldouble ldouble_from_ulong(ulong x)     { return (ldouble)x;    }
static ldouble ldouble_from_ullong(ullong x)   { return (ldouble)x;    }
static ldouble ldouble_from_charp(char *x)     { (void)x; return 0.0l; }
static ldouble ldouble_from_voidp(void *x)     { (void)x; return 0.0l; }
static ldouble ldouble_from_bool(bool x)       { (void)x; return 0.0l; }
static ldouble ldouble_from_char(char x)       { (void)x; return 0.0l; }

#define LDOUBLE_GET(x) \
_Generic((x), \
  ldouble: ldouble_from_ldouble, \
  double:  ldouble_from_double, \
  float:   ldouble_from_float, \
  schar:   ldouble_from_schar, \
  short:   ldouble_from_short, \
  int:     ldouble_from_int, \
  long:    ldouble_from_long, \
  llong:   ldouble_from_llong, \
  uchar:   ldouble_from_uchar, \
  ushort:  ldouble_from_ushort, \
  uint:    ldouble_from_uint, \
  ulong:   ldouble_from_ulong, \
  ullong:  ldouble_from_ullong, \
  char *:  ldouble_from_charp, \
  void *:  ldouble_from_voidp, \
  bool:    ldouble_from_bool, \
  char:    ldouble_from_char \
)(x)

#define COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2) \
    assert_ldouble_##MODE(__FILE__, __LINE__, \
                          #VAR1, #VAR2, \
                          LDOUBLE_GET(VAR1), LDOUBLE_GET(VAR2))

#define COMPARE_FIRST_IS_LDOUBLE(MODE, VAR1, VAR2) \
_Generic((VAR2), \
  ldouble: COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  double:  COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  float:   COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  schar:   COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  short:   COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  int:     COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  long:    COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  llong:   COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  uchar:   COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  ushort:  COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  uint:    COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  ulong:   COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  ullong:  COMPARE_BOTH_LDOUBLE(MODE, VAR1, VAR2), \
  default: unsupported_type_for_generic() \
)

#define COMPARE_POINTERS(MODE, VAR1, VAR2) \
    assert_pointers_##MODE(__FILE__, __LINE__, \
                           #VAR1, #VAR2, \
                           (char *)(uintptr_t)(VAR1), \
                           (char *)(uintptr_t)(VAR2))

#define ASSERT_COMPARE(MODE, VAR1, VAR2) \
_Generic((VAR1), \
  char *: _Generic((VAR2), \
    char *: assert_strings_##MODE(__FILE__, __LINE__, \
                                  #VAR1, #VAR2, \
                                  (char *)(uintptr_t)(VAR1), \
                                  (char *)(uintptr_t)(VAR2)), \
    default: unsupported_type_for_generic() \
  ), \
  void *: _Generic((VAR2), \
    char *: COMPARE_POINTERS(MODE, VAR1, VAR2), \
    void *: COMPARE_POINTERS(MODE, VAR1, VAR2), \
    default: unsupported_type_for_generic() \
  ), \
  schar:   COMPARE_FIRST_IS_SIGNED(MODE, VAR1, VAR2), \
  short:   COMPARE_FIRST_IS_SIGNED(MODE, VAR1, VAR2), \
  int:     COMPARE_FIRST_IS_SIGNED(MODE, VAR1, VAR2), \
  long:    COMPARE_FIRST_IS_SIGNED(MODE, VAR1, VAR2), \
  llong:   COMPARE_FIRST_IS_SIGNED(MODE, VAR1, VAR2), \
  uchar:   COMPARE_FIRST_IS_UNSIGNED(MODE, VAR1, VAR2), \
  ushort:  COMPARE_FIRST_IS_UNSIGNED(MODE, VAR1, VAR2), \
  uint:    COMPARE_FIRST_IS_UNSIGNED(MODE, VAR1, VAR2), \
  ulong:   COMPARE_FIRST_IS_UNSIGNED(MODE, VAR1, VAR2), \
  ullong:  COMPARE_FIRST_IS_UNSIGNED(MODE, VAR1, VAR2), \
  ldouble: COMPARE_FIRST_IS_LDOUBLE(MODE, VAR1, VAR2), \
  double:  COMPARE_FIRST_IS_LDOUBLE(MODE, VAR1, VAR2), \
  float:   COMPARE_FIRST_IS_LDOUBLE(MODE, VAR1, VAR2) \
)

#define ASSERT_EQUAL(VAR1, VAR2)      ASSERT_COMPARE(equal, VAR1, VAR2)
#define ASSERT_NOT_EQUAL(VAR1, VAR2)  ASSERT_COMPARE(not_equal, VAR1, VAR2)
#define ASSERT_LESS(VAR1, VAR2)       ASSERT_COMPARE(less, VAR1, VAR2)
#define ASSERT_LESS_EQUAL(VAR1, VAR2) ASSERT_COMPARE(less_equal, VAR1, VAR2)
#define ASSERT_MORE(VAR1, VAR2)       ASSERT_COMPARE(more, VAR1, VAR2)
#define ASSERT_MORE_EQUAL(VAR1, VAR2) ASSERT_COMPARE(more_equal, VAR1, VAR2)

// clang-format on

#if TESTING_assert

#include <signal.h>
#include <setjmp.h>

static volatile sig_atomic_t assertion_failed = false;
static sigjmp_buf assert_env;

static void __attribute__((noreturn))
handler_failed_assertion(int unused) {
    (void)unused;
    assertion_failed = true;
    siglongjmp(assert_env, 1);
}

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
        double a = 0.123;
        ASSERT_NOT_EQUAL(a, 0.123000001);
        ASSERT_LESS(a, 0.123000001);
        ASSERT_LESS_EQUAL(a, 0.123000001);
        ASSERT_MORE(0.123000001, a);
        ASSERT_MORE_EQUAL(0.123000001, a);
    }
    {
        long a = -1;
        double b = -1;
        ASSERT_EQUAL(a, b);
        ASSERT_EQUAL(b, b);
        ASSERT_MORE_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
    }
    {
        double a = -1;
        long b = -1;
        ASSERT_EQUAL(a, b);
        ASSERT_EQUAL(b, b);
        ASSERT_MORE_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
    }
    {
        double a = -1;
        double b = 0;
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    }
    {
        float a = -1;
        double b = 1;
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    }
    {
        llong a = 1;
        ldouble b = 1;
        ASSERT_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE_EQUAL(b, a);
    }
    {
        llong a = 1;
        ldouble b = 2;
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    }
    {
        int c;
        void *a = NULL;
        void *b = &c;
        ASSERT_NOT_EQUAL(a, b);
    }
    {
        int array[100];
        void *a = &array[0];
        void *b = &array[1];
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    }
    {
        int a = 0;
        double b = 1;
        struct sigaction signal_action;
        signal_action.sa_handler = handler_failed_assertion;
        sigemptyset(&signal_action.sa_mask);
        signal_action.sa_flags = SA_RESTART;

        if (sigaction(SIGILL, &signal_action, NULL) != 0) {
            error2("Error in sigaction: %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (sigsetjmp(assert_env, 1) == 0) {
            ASSERT_EQUAL(a, b);
        }
        ASSERT(assertion_failed);
        assertion_failed = false;

        if (sigsetjmp(assert_env, 1) == 0) {
            ASSERT_MORE(a, b);
        }
        ASSERT(assertion_failed);
        assertion_failed = false;

        if (sigsetjmp(assert_env, 1) == 0) {
            ASSERT_LESS(b, a);
        }
        ASSERT(assertion_failed);
        assertion_failed = false;

        if (sigsetjmp(assert_env, 1) == 0) {
            ASSERT_MORE_EQUAL(a, b);
        }
        ASSERT(assertion_failed);
        assertion_failed = false;

        if (sigsetjmp(assert_env, 1) == 0) {
            ASSERT_LESS_EQUAL(b, a);
        }
        ASSERT(assertion_failed);
        assertion_failed = false;
    }
    ASSERT(true);
    exit(EXIT_SUCCESS);
}
#endif

#endif
