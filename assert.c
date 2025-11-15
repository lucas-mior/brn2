#if !defined(ASSERT_C)
#define ASSERT_C

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_assert 1
#elif !defined(TESTING_assert)
#define TESTING_assert 0
#endif

#if !defined(INTEGERS)
#define INTEGERS
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;

typedef long long llong;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
#endif

#define error2(...) fprintf(stderr, __VA_ARGS__)

static void
assert_strings_equal(char *file, uint32 line,
                     char *name1, char *name2, char *var1, char *var2) {
    if (strcmp(var1, var2)) {
        error2("Assertion failed at %s:%u\n", file, line);
        error2("%s = %s != %s = %s\n", name1, var1, var2, name2);
        abort();
    }
}

static void
assert_strings_less(char *file, uint32 line,
                     char *name1, char *name2, char *var1, char *var2) {
    if (strcmp(var1, var2) >= 0) {
        error2("Assertion failed at %s:%u\n", file, line);
        error2("%s = %s >= %s = %s\n", name1, var1, var2, name2);
        abort();
    }
}

static void
assert_strings_less_equal(char *file, uint32 line,
                          char *name1, char *name2, char *var1, char *var2) {
    if (strcmp(var1, var2) > 0) {
        error2("Assertion failed at %s:%u\n", file, line);
        error2("%s = %s > %s = %s\n", name1, var1, var2, name2);
        abort();
    }
}

#define INTEGER_LESS(TYPE, FORMAT) \
    static void \
    assert_##TYPE##_less(char *file, uint32 line, \
                               char *name1, char *name2, \
                               TYPE long long var1, TYPE long long var2) { \
    if (var1 >= var2) { \
        error2("Assertion failed at %s:%u\n", file, line); \
        error2("%s = "FORMAT" >= "FORMAT" = %s\n", name1, var1, var2, name2); \
        abort(); \
    } \
}

#define INTEGER_LESS_EQUAL(TYPE, FORMAT) \
    static void \
    assert_##TYPE##_less_equal(char *file, uint32 line, \
                               char *name1, char *name2, \
                               TYPE long long var1, TYPE long long var2) { \
    if (var1 > var2) { \
        error2("Assertion failed at %s:%u\n", file, line); \
        error2("%s = "FORMAT" > "FORMAT" = %s\n", name1, var1, var2, name2); \
        abort(); \
    } \
}

#define INTEGER_EQUAL(TYPE, FORMAT) \
    static void \
    assert_##TYPE##_equal(char *file, uint32 line, \
                             char *name1, char *name2, \
                             TYPE long long var1, TYPE long long var2) { \
    if (var1 != var2) { \
        error2("Assertion failed at %s:%u\n", file, line); \
        error2("%s = "FORMAT" != "FORMAT" = %s\n", name1, var1, var2, name2); \
        abort(); \
    } \
}

INTEGER_LESS(signed, "%lld")
INTEGER_LESS(unsigned, "%llu")
INTEGER_LESS_EQUAL(signed, "%lld")
INTEGER_LESS_EQUAL(unsigned, "%llu")
INTEGER_EQUAL(signed, "%lld")
INTEGER_EQUAL(unsigned, "%llu")

#define COMPARE_SIGNED(VAR1, VAR2, MODE) \
_Generic((VAR2), \
  short: assert_signed_##MODE(__FILE__, __LINE__, #VAR1, #VAR2, \
                             (llong)(VAR1), (llong)(VAR2)), \
  int: assert_signed_##MODE(__FILE__, __LINE__, #VAR1, #VAR2, \
                           (llong)(VAR1), (llong)(VAR2)), \
  long: assert_signed_##MODE(__FILE__, __LINE__, #VAR1, #VAR2, \
                            (llong)(VAR1), (llong)(VAR2)), \
  llong: assert_signed_##MODE(__FILE__, __LINE__, #VAR1, #VAR2, \
                            (llong)(VAR1), (llong)(VAR2)), \
  default: assert(false) \
) \

#define COMPARE_UNSIGNED(VAR1, VAR2, MODE) \
_Generic((VAR2), \
  ushort: assert_unsigned_##MODE(__FILE__, __LINE__, #VAR1, #VAR2, \
                                (ullong)(VAR1), (ullong)(VAR2)), \
  uint: assert_unsigned_##MODE(__FILE__, __LINE__, #VAR1, #VAR2, \
                              (ullong)(VAR1), (ullong)(VAR2)), \
  ulong: assert_unsigned_##MODE(__FILE__, __LINE__, #VAR1, #VAR2, \
                               (ullong)(VAR1), (ullong)(VAR2)), \
  ullong: assert_unsigned_##MODE(__FILE__, __LINE__, #VAR1, #VAR2, \
                                (ullong)(VAR1), (ullong)(VAR2)), \
  default: assert(false) \
) \

#define ASSERT(WHAT, VAR1, VAR2) \
_Generic((VAR1), \
  char *: _Generic((VAR2), \
    char *: assert_strings_##WHAT(__FILE__, __LINE__, #VAR1, #VAR2, \
                                 (char *)(uintptr_t)(VAR1), \
                                 (char *)(uintptr_t)(VAR2)), \
    default: assert(false) \
  ), \
  short: COMPARE_SIGNED(VAR1, VAR2, WHAT), \
  int: COMPARE_SIGNED(VAR1, VAR2, WHAT), \
  long: COMPARE_SIGNED(VAR1, VAR2, WHAT), \
  llong: COMPARE_SIGNED(VAR1, VAR2, WHAT), \
  ushort: COMPARE_UNSIGNED(VAR1, VAR2, WHAT), \
  uint: COMPARE_UNSIGNED(VAR1, VAR2, WHAT), \
  ulong: COMPARE_UNSIGNED(VAR1, VAR2, WHAT), \
  ullong: COMPARE_UNSIGNED(VAR1, VAR2, WHAT) \
)

#define ASSERT_EQUAL(VAR1, VAR2) ASSERT(equal, VAR1, VAR2)
#define ASSERT_LESS(VAR1, VAR2) ASSERT(less, VAR1, VAR2)
#define ASSERT_LESS_EQUAL(VAR1, VAR2) ASSERT(less_equal, VAR1, VAR2)

#if TESTING_assert
int main(void) {
    int a = 0;
    int b = 1;
    int c = 2;
    ASSERT_EQUAL(a, a);
    ASSERT_LESS(b, c);
    ASSERT_LESS_EQUAL(a, b);
}
#endif

#endif
