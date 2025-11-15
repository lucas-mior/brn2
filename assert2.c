#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

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

static void
assert_strings_less(char *file, uint line, char *name1, char *name2, char *var1,
                    char *var2) {
    if (!(strcmp(var1, var2) < 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %s "
                "<"
                "%s = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}

static void
assert_strings_less_equal(char *file, uint line, char *name1, char *name2,
                          char *var1, char *var2) {
    if (!(strcmp(var1, var2) <= 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %s "
                "<="
                "%s = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_strings_equal(char *file, uint line, char *name1, char *name2,
                     char *var1, char *var2) {
    if (!(strcmp(var1, var2) == 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %s "
                "=="
                "%s = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_strings_not_equal(char *file, uint line, char *name1, char *name2,
                         char *var1, char *var2) {
    if (!(strcmp(var1, var2) != 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %s "
                "!="
                "%s = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_strings_more(char *file, uint line, char *name1, char *name2, char *var1,
                    char *var2) {
    if (!(strcmp(var1, var2) > 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %s "
                ">"
                "%s = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_strings_more_equal(char *file, uint line, char *name1, char *name2,
                          char *var1, char *var2) {
    if (!(strcmp(var1, var2) >= 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %s "
                ">="
                "%s = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_signed_less(char *file, uint32 line, char *name1, char *name2,
                   signed long long var1, signed long long var2) {
    if (!(var1 < var2)) {
        fprintf(stderr, "Assertion failed at %s:%u\n", file, line);
        fprintf(stderr,
                "%s = "
                "%lld"
                " "
                "<"
                " "
                "%lld"
                " = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_unsigned_less(char *file, uint32 line, char *name1, char *name2,
                     unsigned long long var1, unsigned long long var2) {
    if (!(var1 < var2)) {
        fprintf(stderr, "Assertion failed at %s:%u\n", file, line);
        fprintf(stderr,
                "%s = "
                "%llu"
                " "
                "<"
                " "
                "%llu"
                " = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_signed_less_equal(char *file, uint32 line, char *name1, char *name2,
                         signed long long var1, signed long long var2) {
    if (!(var1 <= var2)) {
        fprintf(stderr, "Assertion failed at %s:%u\n", file, line);
        fprintf(stderr,
                "%s = "
                "%lld"
                " "
                "<="
                " "
                "%lld"
                " = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_unsigned_less_equal(char *file, uint32 line, char *name1, char *name2,
                           unsigned long long var1, unsigned long long var2) {
    if (!(var1 <= var2)) {
        fprintf(stderr, "Assertion failed at %s:%u\n", file, line);
        fprintf(stderr,
                "%s = "
                "%llu"
                " "
                "<="
                " "
                "%llu"
                " = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_signed_equal(char *file, uint32 line, char *name1, char *name2,
                    signed long long var1, signed long long var2) {
    if (!(var1 == var2)) {
        fprintf(stderr, "Assertion failed at %s:%u\n", file, line);
        fprintf(stderr,
                "%s = "
                "%lld"
                " "
                "=="
                " "
                "%lld"
                " = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_unsigned_equal(char *file, uint32 line, char *name1, char *name2,
                      unsigned long long var1, unsigned long long var2) {
    if (!(var1 == var2)) {
        fprintf(stderr, "Assertion failed at %s:%u\n", file, line);
        fprintf(stderr,
                "%s = "
                "%llu"
                " "
                "=="
                " "
                "%llu"
                " = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_signed_not_equal(char *file, uint32 line, char *name1, char *name2,
                        signed long long var1, signed long long var2) {
    if (!(var1 != var2)) {
        fprintf(stderr, "Assertion failed at %s:%u\n", file, line);
        fprintf(stderr,
                "%s = "
                "%lld"
                " "
                "!="
                " "
                "%lld"
                " = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_unsigned_not_equal(char *file, uint32 line, char *name1, char *name2,
                          unsigned long long var1, unsigned long long var2) {
    if (!(var1 != var2)) {
        fprintf(stderr, "Assertion failed at %s:%u\n", file, line);
        fprintf(stderr,
                "%s = "
                "%llu"
                " "
                "!="
                " "
                "%llu"
                " = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_signed_more(char *file, uint32 line, char *name1, char *name2,
                   signed long long var1, signed long long var2) {
    if (!(var1 > var2)) {
        fprintf(stderr, "Assertion failed at %s:%u\n", file, line);
        fprintf(stderr,
                "%s = "
                "%lld"
                " "
                ">"
                " "
                "%lld"
                " = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_unsigned_more(char *file, uint32 line, char *name1, char *name2,
                     unsigned long long var1, unsigned long long var2) {
    if (!(var1 > var2)) {
        fprintf(stderr, "Assertion failed at %s:%u\n", file, line);
        fprintf(stderr,
                "%s = "
                "%llu"
                " "
                ">"
                " "
                "%llu"
                " = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_signed_more_equal(char *file, uint32 line, char *name1, char *name2,
                         signed long long var1, signed long long var2) {
    if (!(var1 >= var2)) {
        fprintf(stderr, "Assertion failed at %s:%u\n", file, line);
        fprintf(stderr,
                "%s = "
                "%lld"
                " "
                ">="
                " "
                "%lld"
                " = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_unsigned_more_equal(char *file, uint32 line, char *name1, char *name2,
                           unsigned long long var1, unsigned long long var2) {
    if (!(var1 >= var2)) {
        fprintf(stderr, "Assertion failed at %s:%u\n", file, line);
        fprintf(stderr,
                "%s = "
                "%llu"
                " "
                ">="
                " "
                "%llu"
                " = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}

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
static void
assert_si_un_equal(char *file, uint line, char *name1, char *name2, llong var1,
                   ullong var2) {
    if (!(compare_unsign_with_sign(var2, var1) == 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %lld "
                "=="
                " %llu = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_si_un_not_equal(char *file, uint line, char *name1, char *name2,
                       llong var1, ullong var2) {
    if (!(compare_unsign_with_sign(var2, var1) != 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %lld "
                "!="
                " %llu = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_si_un_less(char *file, uint line, char *name1, char *name2, llong var1,
                  ullong var2) {
    if (!(compare_unsign_with_sign(var2, var1) > 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %lld "
                ">"
                " %llu = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_si_un_less_equal(char *file, uint line, char *name1, char *name2,
                        llong var1, ullong var2) {
    if (!(compare_unsign_with_sign(var2, var1) >= 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %lld "
                ">="
                " %llu = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_si_un_more(char *file, uint line, char *name1, char *name2, llong var1,
                  ullong var2) {
    if (!(compare_unsign_with_sign(var2, var1) < 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %lld "
                "<"
                " %llu = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_si_un_more_equal(char *file, uint line, char *name1, char *name2,
                        llong var1, ullong var2) {
    if (!(compare_unsign_with_sign(var2, var1) <= 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %lld "
                "<="
                " %llu = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_un_si_equal(char *file, uint line, char *name1, char *name2, ullong var1,
                   llong var2) {
    if (!(compare_unsign_with_sign(var1, var2) == 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %llu "
                "=="
                " %lld = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_un_si_not_equal(char *file, uint line, char *name1, char *name2,
                       ullong var1, llong var2) {
    if (!(compare_unsign_with_sign(var1, var2) != 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %llu "
                "!="
                " %lld = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_un_si_less(char *file, uint line, char *name1, char *name2, ullong var1,
                  llong var2) {
    if (!(compare_unsign_with_sign(var1, var2) < 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %llu "
                "<"
                " %lld = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_un_si_less_equal(char *file, uint line, char *name1, char *name2,
                        ullong var1, llong var2) {
    if (!(compare_unsign_with_sign(var1, var2) <= 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %llu "
                "<="
                " %lld = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_un_si_more(char *file, uint line, char *name1, char *name2, ullong var1,
                  llong var2) {
    if (!(compare_unsign_with_sign(var1, var2) > 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %llu "
                ">"
                " %lld = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
static void
assert_un_si_more_equal(char *file, uint line, char *name1, char *name2,
                        ullong var1, llong var2) {
    if (!(compare_unsign_with_sign(var1, var2) >= 0)) {
        fprintf(stderr, "%s Assertion failed at %s:%u\n", __func__, file, line);
        fprintf(stderr,
                "%s = %llu "
                ">="
                " %lld = %s\n",
                name1, var1, var2, name2);
        __builtin_trap();
    }
}
int
main(void) {
    {
        int a = 1;
        int b = 1;
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_equal("assert.c", 300, "a", "b",
                                             (char *)(uintptr_t)(a),
                                             (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 300,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 300, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 300,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 300, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 300,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 300, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 300,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 300, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 300,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 300, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 300, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 300, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 300,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 300, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 300, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 300, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 300, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 300, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 300, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 300, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 300,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 300, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 300, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 300, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 300, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 300, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 300, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 300, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 300,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 300, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 300, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 300, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 300, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 300, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 300, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 300, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 300,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 300, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 300, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 300, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 300, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 300, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 300, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 300, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 300,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 300, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 300, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 300, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 300, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 300, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 300, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 300, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 300, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 300,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less_equal("assert.c", 301, "a", "b",
                                                  (char *)(uintptr_t)(a),
                                                  (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 301,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 301, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 301,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 301, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 301,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 301, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 301,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 301, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 301,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 301, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 301, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 301,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 301,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 301,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 301,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 301,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 301, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 301, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 301,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_more_equal("assert.c", 302, "a", "b",
                                                  (char *)(uintptr_t)(a),
                                                  (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 302,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_more_equal("assert.c", 302, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 302,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_more_equal("assert.c", 302, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 302,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_more_equal("assert.c", 302, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 302,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_more_equal("assert.c", 302, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 302,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_more_equal("assert.c", 302, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_more_equal("assert.c", 302, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 302,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 302,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 302,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 302,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 302,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_more_equal("assert.c", 302, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_more_equal("assert.c", 302, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 302,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
    }
    {
        int a = 1;
        uint b = 1;
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_equal("assert.c", 307, "a", "b",
                                             (char *)(uintptr_t)(a),
                                             (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 307,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 307, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 307,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 307, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 307,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 307, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 307,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 307, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 307,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 307, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 307, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 307, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 307,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 307, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 307, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 307, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 307, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 307, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 307, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 307, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 307,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 307, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 307, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 307, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 307, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 307, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 307, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 307, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 307,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 307, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 307, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 307, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 307, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 307, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 307, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 307, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 307,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 307, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 307, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 307, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 307, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 307, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 307, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 307, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 307,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 307, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 307, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 307, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 307, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 307, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 307, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 307, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 307, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 307,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less_equal("assert.c", 308, "a", "b",
                                                  (char *)(uintptr_t)(a),
                                                  (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 308,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 308, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 308,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 308, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 308,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 308, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 308,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 308, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 308,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 308, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 308, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 308,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 308,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 308,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 308,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 308,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 308, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 308, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 308,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_more_equal("assert.c", 309, "a", "b",
                                                  (char *)(uintptr_t)(a),
                                                  (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 309,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_more_equal("assert.c", 309, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 309,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_more_equal("assert.c", 309, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 309,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_more_equal("assert.c", 309, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 309,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_more_equal("assert.c", 309, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 309,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_more_equal("assert.c", 309, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_more_equal("assert.c", 309, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 309,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 309,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 309,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 309,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 309,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_more_equal("assert.c", 309, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_more_equal("assert.c", 309, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 309,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
    }
    {
        int a = 1;
        uint b = 2;
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_not_equal("assert.c", 314, "a", "b",
                                                 (char *)(uintptr_t)(a),
                                                 (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 314,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 314, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 314,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 314, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 314,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 314, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 314,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 314, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 314,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 314, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 314, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 314,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 314,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 314,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 314,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 314,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 314, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 314, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 314,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less("assert.c", 315, "a", "b",
                                            (char *)(uintptr_t)(a),
                                            (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 315,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 315,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 315,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 315,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 315,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 315, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 315, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 315,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 315, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 315, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 315, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 315, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 315, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 315,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 315, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 315, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 315, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 315, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 315, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 315,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 315, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 315, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 315, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 315, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 315, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 315,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 315, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 315, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 315, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 315, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 315, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 315,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 315, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 315, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 315, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 315, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 315, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 315, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 315,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less_equal("assert.c", 316, "a", "b",
                                                  (char *)(uintptr_t)(a),
                                                  (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 316,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 316, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 316,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 316, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 316,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 316, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 316,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 316, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 316,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 316, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 316, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 316,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 316,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 316,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 316,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 316,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 316, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 316, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 316,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more("assert.c", 317, "b", "a",
                                            (char *)(uintptr_t)(b),
                                            (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 317,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 317,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 317,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 317,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 317,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 317, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 317, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 317,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 317, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 317, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 317, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 317, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 317, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 317,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 317, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 317, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 317, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 317, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 317, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 317,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 317, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 317, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 317, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 317, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 317, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 317,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 317, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 317, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 317, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 317, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 317, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 317,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 317, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 317, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 317, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 317, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 317, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 317, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 317,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more_equal("assert.c", 318, "b", "a",
                                                  (char *)(uintptr_t)(b),
                                                  (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 318,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 318, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 318,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 318, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 318,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 318, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 318,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 318, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 318,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 318, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 318, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 318,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 318,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 318,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 318,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 318,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 318, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 318, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 318,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
    }
    {
        long a = -1;
        ulong b = 0;
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_not_equal("assert.c", 323, "a", "b",
                                                 (char *)(uintptr_t)(a),
                                                 (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 323,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 323, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 323,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 323, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 323,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 323, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 323,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 323, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 323,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 323, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 323, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 323,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 323,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 323,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 323,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 323,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 323, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 323, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 323,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less("assert.c", 324, "a", "b",
                                            (char *)(uintptr_t)(a),
                                            (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 324,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 324,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 324,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 324,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 324,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 324, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 324, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 324,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 324, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 324, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 324, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 324, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 324, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 324,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 324, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 324, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 324, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 324, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 324, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 324,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 324, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 324, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 324, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 324, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 324, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 324,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 324, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 324, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 324, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 324, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 324, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 324,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 324, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 324, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 324, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 324, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 324, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 324, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 324,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less_equal("assert.c", 325, "a", "b",
                                                  (char *)(uintptr_t)(a),
                                                  (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 325,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 325, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 325,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 325, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 325,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 325, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 325,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 325, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 325,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 325, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 325, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 325,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 325,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 325,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 325,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 325,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 325, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 325, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 325,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more("assert.c", 326, "b", "a",
                                            (char *)(uintptr_t)(b),
                                            (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 326,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 326,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 326,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 326,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 326,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 326, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 326, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 326,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 326, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 326, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 326, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 326, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 326, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 326,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 326, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 326, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 326, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 326, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 326, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 326,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 326, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 326, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 326, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 326, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 326, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 326,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 326, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 326, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 326, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 326, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 326, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 326,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 326, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 326, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 326, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 326, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 326, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 326, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 326,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more_equal("assert.c", 327, "b", "a",
                                                  (char *)(uintptr_t)(b),
                                                  (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 327,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 327, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 327,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 327, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 327,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 327, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 327,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 327, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 327,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 327, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 327, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 327,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 327,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 327,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 327,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 327,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 327, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 327, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 327,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
    }
    {
        long a = _Generic((a),
            schar: (-0x7f - 1),
            short: (-0x7fff - 1),
            int: (-0x7fffffff - 1),
            long: (-0x7fffffffffffffffL - 1L),
            uchar: 0,
            ushort: 0,
            uint: 0u,
            ulong: 0ul,
            ullong: 0ull,
            char: (-0x7f - 1),
            bool: 0,
            float: -3.40282346638528859811704183484516925e+38F,
            double: -((double)1.79769313486231570814527423731704357e+308L),
            long double: -1.18973149535723176502126385303097021e+4932L);
        ulong b = _Generic((b),
            schar: 0x7f,
            short: 0x7fff,
            int: 0x7fffffff,
            long: 0x7fffffffffffffffL,
            uchar: (0x7f*2 + 1),
            ushort: (0x7fff*2 + 1),
            uint: (0x7fffffff*2U + 1U),
            ulong: (0x7fffffffffffffffL*2UL + 1UL),
            ullong: (0x7fffffffffffffffLL*2ULL + 1ULL),
            char: 0x7f,
            bool: 1,
            float: 3.40282346638528859811704183484516925e+38F,
            double: ((double)1.79769313486231570814527423731704357e+308L),
            long double: 1.18973149535723176502126385303097021e+4932L);
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_not_equal("assert.c", 332, "a", "b",
                                                 (char *)(uintptr_t)(a),
                                                 (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 332,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 332, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 332,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 332, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 332,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 332, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 332,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 332, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 332,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 332, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 332, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 332,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 332,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 332,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 332,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 332,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 332, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 332, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 332,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less("assert.c", 333, "a", "b",
                                            (char *)(uintptr_t)(a),
                                            (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 333,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 333,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 333,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 333,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 333,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 333, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 333, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 333,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 333, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 333, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 333, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 333, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 333, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 333,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 333, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 333, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 333, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 333, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 333, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 333,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 333, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 333, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 333, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 333, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 333, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 333,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 333, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 333, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 333, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 333, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 333, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 333,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 333, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 333, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 333, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 333, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 333, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 333, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 333,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less_equal("assert.c", 334, "a", "b",
                                                  (char *)(uintptr_t)(a),
                                                  (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 334,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 334, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 334,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 334, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 334,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 334, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 334,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 334, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 334,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 334, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 334, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 334,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 334,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 334,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 334,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 334,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 334, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 334, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 334,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more("assert.c", 335, "b", "a",
                                            (char *)(uintptr_t)(b),
                                            (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 335,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 335,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 335,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 335,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 335,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 335, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 335, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 335,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 335, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 335, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 335, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 335, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 335, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 335,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 335, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 335, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 335, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 335, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 335, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 335,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 335, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 335, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 335, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 335, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 335, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 335,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 335, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 335, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 335, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 335, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 335, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 335,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 335, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 335, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 335, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 335, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 335, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 335, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 335,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more_equal("assert.c", 336, "b", "a",
                                                  (char *)(uintptr_t)(b),
                                                  (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 336,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 336, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 336,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 336, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 336,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 336, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 336,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 336, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 336,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 336, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 336, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 336,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 336,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 336,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 336,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 336,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 336, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 336, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 336,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
    }
    {
        ulong a = _Generic((a),
            schar: (-0x7f - 1),
            short: (-0x7fff - 1),
            int: (-0x7fffffff - 1),
            long: (-0x7fffffffffffffffL - 1L),
            uchar: 0,
            ushort: 0,
            uint: 0u,
            ulong: 0ul,
            ullong: 0ull,
            char: (-0x7f - 1),
            bool: 0,
            float: -3.40282346638528859811704183484516925e+38F,
            double: -((double)1.79769313486231570814527423731704357e+308L),
            long double: -1.18973149535723176502126385303097021e+4932L);
        long b = _Generic((b),
            schar: 0x7f,
            short: 0x7fff,
            int: 0x7fffffff,
            long: 0x7fffffffffffffffL,
            uchar: (0x7f*2 + 1),
            ushort: (0x7fff*2 + 1),
            uint: (0x7fffffff*2U + 1U),
            ulong: (0x7fffffffffffffffL*2UL + 1UL),
            ullong: (0x7fffffffffffffffLL*2ULL + 1ULL),
            char: 0x7f,
            bool: 1,
            float: 3.40282346638528859811704183484516925e+38F,
            double: ((double)1.79769313486231570814527423731704357e+308L),
            long double: 1.18973149535723176502126385303097021e+4932L);
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_not_equal("assert.c", 341, "a", "b",
                                                 (char *)(uintptr_t)(a),
                                                 (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 341,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 341, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 341,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 341, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 341,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 341, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 341,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 341, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 341,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 341, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 341, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 341,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 341,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 341,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 341,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 341,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 341, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 341, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 341,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less("assert.c", 342, "a", "b",
                                            (char *)(uintptr_t)(a),
                                            (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 342,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 342,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 342,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 342,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 342,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 342, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 342, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 342,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 342, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 342, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 342, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 342, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 342, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 342,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 342, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 342, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 342, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 342, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 342, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 342,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 342, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 342, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 342, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 342, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 342, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 342,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 342, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 342, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 342, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 342, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 342, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 342,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 342, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 342, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 342, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 342, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 342, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 342, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 342,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less_equal("assert.c", 343, "a", "b",
                                                  (char *)(uintptr_t)(a),
                                                  (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 343,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 343, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 343,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 343, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 343,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 343, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 343,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 343, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 343,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 343, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 343, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 343,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 343,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 343,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 343,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 343,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 343, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 343, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 343,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more("assert.c", 344, "b", "a",
                                            (char *)(uintptr_t)(b),
                                            (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 344,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 344,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 344,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 344,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 344,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 344, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 344, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 344,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 344, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 344, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 344, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 344, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 344, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 344,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 344, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 344, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 344, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 344, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 344, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 344,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 344, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 344, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 344, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 344, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 344, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 344,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 344, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 344, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 344, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 344, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 344, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 344,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 344, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 344, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 344, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 344, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 344, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 344, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 344,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more_equal("assert.c", 345, "b", "a",
                                                  (char *)(uintptr_t)(b),
                                                  (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 345,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 345, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 345,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 345, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 345,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 345, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 345,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 345, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 345,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 345, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 345, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 345,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 345,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 345,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 345,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 345,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 345, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 345, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 345,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
    }
    {
        long a = -2;
        long b = -1;
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_not_equal("assert.c", 350, "a", "b",
                                                 (char *)(uintptr_t)(a),
                                                 (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 350,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 350, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 350,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 350, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 350,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 350, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 350,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 350, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 350,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 350, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 350, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 350,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 350,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 350,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 350,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 350,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 350, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 350, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 350,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less("assert.c", 351, "a", "b",
                                            (char *)(uintptr_t)(a),
                                            (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 351,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 351,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 351,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 351,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 351,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 351, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 351, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 351,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 351, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 351, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 351, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 351, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 351, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 351,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 351, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 351, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 351, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 351, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 351, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 351,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 351, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 351, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 351, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 351, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 351, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 351,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 351, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 351, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 351, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 351, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 351, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 351,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 351, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 351, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 351, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 351, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 351, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 351, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 351,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less_equal("assert.c", 352, "a", "b",
                                                  (char *)(uintptr_t)(a),
                                                  (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 352,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 352, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 352,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 352, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 352,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 352, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 352,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 352, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 352,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 352, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 352, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 352,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 352,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 352,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 352,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 352,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 352, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 352, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 352,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more("assert.c", 353, "b", "a",
                                            (char *)(uintptr_t)(b),
                                            (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 353,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 353,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 353,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 353,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 353,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 353, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 353, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 353,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 353, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 353, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 353, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 353, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 353, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 353,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 353, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 353, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 353, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 353, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 353, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 353,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 353, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 353, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 353, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 353, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 353, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 353,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 353, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 353, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 353, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 353, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 353, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 353,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 353, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 353, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 353, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 353, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 353, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 353, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 353,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more_equal("assert.c", 354, "b", "a",
                                                  (char *)(uintptr_t)(b),
                                                  (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 354,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 354, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 354,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 354, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 354,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 354, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 354,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 354, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 354,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 354, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 354, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 354,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 354,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 354,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 354,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 354,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 354, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 354, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 354,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
    }
    {
        char *a = "aaa";
        char *b = "aaa";
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_equal("assert.c", 359, "a", "b",
                                             (char *)(uintptr_t)(a),
                                             (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 359,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 359, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 359,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 359, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 359,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 359, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 359,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 359, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 359,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                short: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                int: assert_signed_equal("assert.c", 359, "a", "b", (llong)(a),
                                         (llong)(b)),
                long: assert_signed_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (llong)(b)),
                llong: assert_signed_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (llong)(b)),
                uchar: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ushort: assert_si_un_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (ullong)(b)),
                uint: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ulong: assert_si_un_equal("assert.c", 359, "a", "b", (llong)(a),
                                          (ullong)(b)),
                ullong: assert_si_un_equal("assert.c", 359, "a", "b",
                                           (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 359,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 359, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 359, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 359, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 359, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 359, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 359, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 359, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 359,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 359, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 359, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 359, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 359, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 359, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 359, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 359, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 359,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 359, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 359, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 359, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 359, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 359, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 359, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 359, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 359,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 359, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 359, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 359, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 359, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 359, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 359, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 359, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 359,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_equal("assert.c", 359, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_equal("assert.c", 359, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_equal("assert.c", 359, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_equal("assert.c", 359, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_equal("assert.c", 359, "a", "b",
                                              (ullong)(a), (ullong)(b)),
                schar: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                short: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                int: assert_un_si_equal("assert.c", 359, "a", "b", (ullong)(a),
                                        (llong)(b)),
                long: assert_un_si_equal("assert.c", 359, "a", "b", (ullong)(a),
                                         (llong)(b)),
                llong: assert_un_si_equal("assert.c", 359, "a", "b",
                                          (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 359,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less_equal("assert.c", 360, "a", "b",
                                                  (char *)(uintptr_t)(a),
                                                  (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 360,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 360, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 360,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 360, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 360,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 360, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 360,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 360, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 360,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 360, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 360, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 360,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 360,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 360,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 360,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 360,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 360, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 360, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 360,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more_equal("assert.c", 361, "b", "a",
                                                  (char *)(uintptr_t)(b),
                                                  (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 361,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 361, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 361,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 361, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 361,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 361, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 361,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 361, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 361,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 361, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 361, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 361,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 361,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 361,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 361,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 361,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 361, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 361, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 361,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
    }
    {
        char *a = "aaa";
        char *b = "bbb";
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_not_equal("assert.c", 366, "a", "b",
                                                 (char *)(uintptr_t)(a),
                                                 (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 366,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 366, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 366,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 366, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 366,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 366, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 366,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 366, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 366,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                short: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                int: assert_signed_not_equal("assert.c", 366, "a", "b",
                                             (llong)(a), (llong)(b)),
                long: assert_signed_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (llong)(b)),
                llong: assert_signed_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (llong)(b)),
                uchar: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ushort: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (ullong)(b)),
                uint: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                             (llong)(a), (ullong)(b)),
                ulong: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ullong: assert_si_un_not_equal("assert.c", 366, "a", "b",
                                               (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 366,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 366,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 366,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 366,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 366,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_not_equal("assert.c", 366, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                schar: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                short: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                int: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                            (ullong)(a), (llong)(b)),
                long: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                             (ullong)(a), (llong)(b)),
                llong: assert_un_si_not_equal("assert.c", 366, "a", "b",
                                              (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 366,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less("assert.c", 367, "a", "b",
                                            (char *)(uintptr_t)(a),
                                            (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 367,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 367,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 367,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 367,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 367,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                short: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                int: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                        (llong)(b)),
                long: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                         (llong)(b)),
                llong: assert_signed_less("assert.c", 367, "a", "b", (llong)(a),
                                          (llong)(b)),
                uchar: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ushort: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                          (ullong)(b)),
                uint: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                        (ullong)(b)),
                ulong: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                         (ullong)(b)),
                ullong: assert_si_un_less("assert.c", 367, "a", "b", (llong)(a),
                                          (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 367,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 367, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 367, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 367, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 367, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 367, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 367,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 367, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 367, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 367, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 367, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 367, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 367,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 367, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 367, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 367, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 367, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 367, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 367,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 367, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 367, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 367, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 367, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 367, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 367,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less("assert.c", 367, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less("assert.c", 367, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less("assert.c", 367, "a", "b",
                                           (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less("assert.c", 367, "a", "b",
                                            (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less("assert.c", 367, "a", "b",
                                             (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                short: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                int: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                       (llong)(b)),
                long: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                        (llong)(b)),
                llong: assert_un_si_less("assert.c", 367, "a", "b", (ullong)(a),
                                         (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 367,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((b),
                char *: assert_strings_less_equal("assert.c", 368, "a", "b",
                                                  (char *)(uintptr_t)(a),
                                                  (char *)(uintptr_t)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 368,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 368, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 368,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 368, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 368,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 368, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 368,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 368, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 368,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((b),
                schar: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                short: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                int: assert_signed_less_equal("assert.c", 368, "a", "b",
                                              (llong)(a), (llong)(b)),
                long: assert_signed_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (llong)(b)),
                llong: assert_signed_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (llong)(b)),
                uchar: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ushort: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (ullong)(b)),
                uint: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                              (llong)(a), (ullong)(b)),
                ulong: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                               (llong)(a), (ullong)(b)),
                ullong: assert_si_un_less_equal("assert.c", 368, "a", "b",
                                                (llong)(a), (ullong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 368,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 368,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 368,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 368,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 368,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((b),
                uchar: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ushort: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                uint: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                 (ullong)(a), (ullong)(b)),
                ulong: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                  (ullong)(a), (ullong)(b)),
                ullong: assert_unsigned_less_equal("assert.c", 368, "a", "b",
                                                   (ullong)(a), (ullong)(b)),
                schar: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                short: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                int: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                             (ullong)(a), (llong)(b)),
                long: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                              (ullong)(a), (llong)(b)),
                llong: assert_un_si_less_equal("assert.c", 368, "a", "b",
                                               (ullong)(a), (llong)(b)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 368,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more("assert.c", 369, "b", "a",
                                            (char *)(uintptr_t)(b),
                                            (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 369,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 369,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 369,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 369,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 369,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 369, "b", "a", (llong)(b),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 369, "b", "a", (llong)(b),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 369,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 369, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 369, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 369, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 369, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 369, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 369,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 369, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 369, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 369, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 369, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 369, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 369,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 369, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 369, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 369, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 369, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 369, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 369,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 369, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 369, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 369, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 369, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 369, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 369,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 369, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 369, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 369, "b", "a",
                                           (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 369, "b", "a",
                                            (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 369, "b", "a",
                                             (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 369, "b", "a", (ullong)(b),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 369,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((b),
            char *: _Generic((a),
                char *: assert_strings_more_equal("assert.c", 370, "b", "a",
                                                  (char *)(uintptr_t)(b),
                                                  (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 370,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 370, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 370,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 370, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 370,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 370, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 370,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 370, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 370,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 370, "b", "a",
                                              (llong)(b), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                              (llong)(b), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                               (llong)(b), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 370, "b", "a",
                                                (llong)(b), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 370,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 370,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 370,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 370,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 370,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                 (ullong)(b), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                  (ullong)(b), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 370, "b", "a",
                                                   (ullong)(b), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                             (ullong)(b), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                              (ullong)(b), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 370, "b", "a",
                                               (ullong)(b), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 370,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
    }
    {
        long a = -1;
        _Generic((a),
            char *: _Generic((0),
                char *: assert_strings_not_equal("assert.c", 374, "a", "0",
                                                 (char *)(uintptr_t)(a),
                                                 (char *)(uintptr_t)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 374,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((0),
                schar: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                short: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                int: assert_signed_not_equal("assert.c", 374, "a", "0",
                                             (llong)(a), (llong)(0)),
                long: assert_signed_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (llong)(0)),
                llong: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                uchar: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ushort: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (ullong)(0)),
                uint: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                             (llong)(a), (ullong)(0)),
                ulong: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ullong: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 374,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((0),
                schar: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                short: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                int: assert_signed_not_equal("assert.c", 374, "a", "0",
                                             (llong)(a), (llong)(0)),
                long: assert_signed_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (llong)(0)),
                llong: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                uchar: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ushort: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (ullong)(0)),
                uint: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                             (llong)(a), (ullong)(0)),
                ulong: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ullong: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 374,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((0),
                schar: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                short: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                int: assert_signed_not_equal("assert.c", 374, "a", "0",
                                             (llong)(a), (llong)(0)),
                long: assert_signed_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (llong)(0)),
                llong: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                uchar: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ushort: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (ullong)(0)),
                uint: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                             (llong)(a), (ullong)(0)),
                ulong: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ullong: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 374,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((0),
                schar: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                short: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                int: assert_signed_not_equal("assert.c", 374, "a", "0",
                                             (llong)(a), (llong)(0)),
                long: assert_signed_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (llong)(0)),
                llong: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                uchar: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ushort: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (ullong)(0)),
                uint: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                             (llong)(a), (ullong)(0)),
                ulong: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ullong: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 374,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((0),
                schar: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                short: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                int: assert_signed_not_equal("assert.c", 374, "a", "0",
                                             (llong)(a), (llong)(0)),
                long: assert_signed_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (llong)(0)),
                llong: assert_signed_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (llong)(0)),
                uchar: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ushort: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (ullong)(0)),
                uint: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                             (llong)(a), (ullong)(0)),
                ulong: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ullong: assert_si_un_not_equal("assert.c", 374, "a", "0",
                                               (llong)(a), (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 374,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((0),
                uchar: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                schar: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                short: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                int: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                            (ullong)(a), (llong)(0)),
                long: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                             (ullong)(a), (llong)(0)),
                llong: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 374,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((0),
                uchar: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                schar: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                short: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                int: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                            (ullong)(a), (llong)(0)),
                long: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                             (ullong)(a), (llong)(0)),
                llong: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 374,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((0),
                uchar: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                schar: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                short: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                int: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                            (ullong)(a), (llong)(0)),
                long: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                             (ullong)(a), (llong)(0)),
                llong: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 374,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((0),
                uchar: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                schar: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                short: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                int: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                            (ullong)(a), (llong)(0)),
                long: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                             (ullong)(a), (llong)(0)),
                llong: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 374,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((0),
                uchar: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_not_equal("assert.c", 374, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                schar: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                short: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                int: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                            (ullong)(a), (llong)(0)),
                long: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                             (ullong)(a), (llong)(0)),
                llong: assert_un_si_not_equal("assert.c", 374, "a", "0",
                                              (ullong)(a), (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 374,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((0),
                char *: assert_strings_less("assert.c", 375, "a", "0",
                                            (char *)(uintptr_t)(a),
                                            (char *)(uintptr_t)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 375,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((0),
                schar: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                short: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                int: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                        (llong)(0)),
                long: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                         (llong)(0)),
                llong: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                uchar: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                         (ullong)(0)),
                ushort: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                          (ullong)(0)),
                uint: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                        (ullong)(0)),
                ulong: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                         (ullong)(0)),
                ullong: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                          (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 375,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((0),
                schar: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                short: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                int: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                        (llong)(0)),
                long: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                         (llong)(0)),
                llong: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                uchar: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                         (ullong)(0)),
                ushort: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                          (ullong)(0)),
                uint: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                        (ullong)(0)),
                ulong: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                         (ullong)(0)),
                ullong: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                          (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 375,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((0),
                schar: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                short: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                int: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                        (llong)(0)),
                long: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                         (llong)(0)),
                llong: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                uchar: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                         (ullong)(0)),
                ushort: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                          (ullong)(0)),
                uint: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                        (ullong)(0)),
                ulong: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                         (ullong)(0)),
                ullong: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                          (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 375,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((0),
                schar: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                short: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                int: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                        (llong)(0)),
                long: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                         (llong)(0)),
                llong: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                uchar: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                         (ullong)(0)),
                ushort: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                          (ullong)(0)),
                uint: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                        (ullong)(0)),
                ulong: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                         (ullong)(0)),
                ullong: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                          (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 375,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((0),
                schar: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                short: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                int: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                        (llong)(0)),
                long: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                         (llong)(0)),
                llong: assert_signed_less("assert.c", 375, "a", "0", (llong)(a),
                                          (llong)(0)),
                uchar: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                         (ullong)(0)),
                ushort: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                          (ullong)(0)),
                uint: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                        (ullong)(0)),
                ulong: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                         (ullong)(0)),
                ullong: assert_si_un_less("assert.c", 375, "a", "0", (llong)(a),
                                          (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 375,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((0),
                uchar: assert_unsigned_less("assert.c", 375, "a", "0",
                                            (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_less("assert.c", 375, "a", "0",
                                             (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_less("assert.c", 375, "a", "0",
                                           (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_less("assert.c", 375, "a", "0",
                                            (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_less("assert.c", 375, "a", "0",
                                             (ullong)(a), (ullong)(0)),
                schar: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                short: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                int: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                       (llong)(0)),
                long: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                        (llong)(0)),
                llong: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 375,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((0),
                uchar: assert_unsigned_less("assert.c", 375, "a", "0",
                                            (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_less("assert.c", 375, "a", "0",
                                             (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_less("assert.c", 375, "a", "0",
                                           (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_less("assert.c", 375, "a", "0",
                                            (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_less("assert.c", 375, "a", "0",
                                             (ullong)(a), (ullong)(0)),
                schar: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                short: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                int: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                       (llong)(0)),
                long: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                        (llong)(0)),
                llong: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 375,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((0),
                uchar: assert_unsigned_less("assert.c", 375, "a", "0",
                                            (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_less("assert.c", 375, "a", "0",
                                             (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_less("assert.c", 375, "a", "0",
                                           (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_less("assert.c", 375, "a", "0",
                                            (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_less("assert.c", 375, "a", "0",
                                             (ullong)(a), (ullong)(0)),
                schar: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                short: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                int: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                       (llong)(0)),
                long: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                        (llong)(0)),
                llong: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 375,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((0),
                uchar: assert_unsigned_less("assert.c", 375, "a", "0",
                                            (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_less("assert.c", 375, "a", "0",
                                             (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_less("assert.c", 375, "a", "0",
                                           (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_less("assert.c", 375, "a", "0",
                                            (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_less("assert.c", 375, "a", "0",
                                             (ullong)(a), (ullong)(0)),
                schar: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                short: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                int: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                       (llong)(0)),
                long: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                        (llong)(0)),
                llong: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 375,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((0),
                uchar: assert_unsigned_less("assert.c", 375, "a", "0",
                                            (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_less("assert.c", 375, "a", "0",
                                             (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_less("assert.c", 375, "a", "0",
                                           (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_less("assert.c", 375, "a", "0",
                                            (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_less("assert.c", 375, "a", "0",
                                             (ullong)(a), (ullong)(0)),
                schar: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                short: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                int: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                       (llong)(0)),
                long: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                        (llong)(0)),
                llong: assert_un_si_less("assert.c", 375, "a", "0", (ullong)(a),
                                         (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 375,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((a),
            char *: _Generic((0),
                char *: assert_strings_less_equal("assert.c", 376, "a", "0",
                                                  (char *)(uintptr_t)(a),
                                                  (char *)(uintptr_t)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 376,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((0),
                schar: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                short: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                int: assert_signed_less_equal("assert.c", 376, "a", "0",
                                              (llong)(a), (llong)(0)),
                long: assert_signed_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (llong)(0)),
                llong: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                uchar: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (ullong)(0)),
                ushort: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (ullong)(0)),
                uint: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ulong: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (ullong)(0)),
                ullong: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 376,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((0),
                schar: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                short: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                int: assert_signed_less_equal("assert.c", 376, "a", "0",
                                              (llong)(a), (llong)(0)),
                long: assert_signed_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (llong)(0)),
                llong: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                uchar: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (ullong)(0)),
                ushort: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (ullong)(0)),
                uint: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ulong: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (ullong)(0)),
                ullong: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 376,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((0),
                schar: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                short: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                int: assert_signed_less_equal("assert.c", 376, "a", "0",
                                              (llong)(a), (llong)(0)),
                long: assert_signed_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (llong)(0)),
                llong: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                uchar: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (ullong)(0)),
                ushort: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (ullong)(0)),
                uint: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ulong: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (ullong)(0)),
                ullong: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 376,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((0),
                schar: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                short: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                int: assert_signed_less_equal("assert.c", 376, "a", "0",
                                              (llong)(a), (llong)(0)),
                long: assert_signed_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (llong)(0)),
                llong: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                uchar: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (ullong)(0)),
                ushort: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (ullong)(0)),
                uint: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ulong: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (ullong)(0)),
                ullong: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 376,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((0),
                schar: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                short: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                int: assert_signed_less_equal("assert.c", 376, "a", "0",
                                              (llong)(a), (llong)(0)),
                long: assert_signed_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (llong)(0)),
                llong: assert_signed_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (llong)(0)),
                uchar: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (ullong)(0)),
                ushort: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (ullong)(0)),
                uint: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                              (llong)(a), (ullong)(0)),
                ulong: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                               (llong)(a), (ullong)(0)),
                ullong: assert_si_un_less_equal("assert.c", 376, "a", "0",
                                                (llong)(a), (ullong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 376,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((0),
                uchar: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                   (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                   (ullong)(a), (ullong)(0)),
                schar: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                short: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                int: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                             (ullong)(a), (llong)(0)),
                long: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                              (ullong)(a), (llong)(0)),
                llong: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 376,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((0),
                uchar: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                   (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                   (ullong)(a), (ullong)(0)),
                schar: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                short: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                int: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                             (ullong)(a), (llong)(0)),
                long: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                              (ullong)(a), (llong)(0)),
                llong: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 376,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((0),
                uchar: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                   (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                   (ullong)(a), (ullong)(0)),
                schar: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                short: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                int: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                             (ullong)(a), (llong)(0)),
                long: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                              (ullong)(a), (llong)(0)),
                llong: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 376,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((0),
                uchar: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                   (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                   (ullong)(a), (ullong)(0)),
                schar: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                short: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                int: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                             (ullong)(a), (llong)(0)),
                long: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                              (ullong)(a), (llong)(0)),
                llong: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 376,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((0),
                uchar: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                ushort: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                   (ullong)(a), (ullong)(0)),
                uint: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                 (ullong)(a), (ullong)(0)),
                ulong: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                  (ullong)(a), (ullong)(0)),
                ullong: assert_unsigned_less_equal("assert.c", 376, "a", "0",
                                                   (ullong)(a), (ullong)(0)),
                schar: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                short: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                int: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                             (ullong)(a), (llong)(0)),
                long: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                              (ullong)(a), (llong)(0)),
                llong: assert_un_si_less_equal("assert.c", 376, "a", "0",
                                               (ullong)(a), (llong)(0)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 376,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((0),
            char *: _Generic((a),
                char *: assert_strings_more("assert.c", 377, "0", "a",
                                            (char *)(uintptr_t)(0),
                                            (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 377,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 377,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 377,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 377,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 377,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                short: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                int: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                        (llong)(a)),
                long: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                         (llong)(a)),
                llong: assert_signed_more("assert.c", 377, "0", "a", (llong)(0),
                                          (llong)(a)),
                uchar: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                         (ullong)(a)),
                ushort: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                          (ullong)(a)),
                uint: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                        (ullong)(a)),
                ulong: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                         (ullong)(a)),
                ullong: assert_si_un_more("assert.c", 377, "0", "a", (llong)(0),
                                          (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 377,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 377, "0", "a",
                                            (ullong)(0), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 377, "0", "a",
                                             (ullong)(0), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 377, "0", "a",
                                           (ullong)(0), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 377, "0", "a",
                                            (ullong)(0), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 377, "0", "a",
                                             (ullong)(0), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 377,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 377, "0", "a",
                                            (ullong)(0), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 377, "0", "a",
                                             (ullong)(0), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 377, "0", "a",
                                           (ullong)(0), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 377, "0", "a",
                                            (ullong)(0), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 377, "0", "a",
                                             (ullong)(0), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 377,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 377, "0", "a",
                                            (ullong)(0), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 377, "0", "a",
                                             (ullong)(0), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 377, "0", "a",
                                           (ullong)(0), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 377, "0", "a",
                                            (ullong)(0), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 377, "0", "a",
                                             (ullong)(0), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 377,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 377, "0", "a",
                                            (ullong)(0), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 377, "0", "a",
                                             (ullong)(0), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 377, "0", "a",
                                           (ullong)(0), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 377, "0", "a",
                                            (ullong)(0), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 377, "0", "a",
                                             (ullong)(0), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 377,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more("assert.c", 377, "0", "a",
                                            (ullong)(0), (ullong)(a)),
                ushort: assert_unsigned_more("assert.c", 377, "0", "a",
                                             (ullong)(0), (ullong)(a)),
                uint: assert_unsigned_more("assert.c", 377, "0", "a",
                                           (ullong)(0), (ullong)(a)),
                ulong: assert_unsigned_more("assert.c", 377, "0", "a",
                                            (ullong)(0), (ullong)(a)),
                ullong: assert_unsigned_more("assert.c", 377, "0", "a",
                                             (ullong)(0), (ullong)(a)),
                schar: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                short: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                int: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                       (llong)(a)),
                long: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                        (llong)(a)),
                llong: assert_un_si_more("assert.c", 377, "0", "a", (ullong)(0),
                                         (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 377,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
        _Generic((0),
            char *: _Generic((a),
                char *: assert_strings_more_equal("assert.c", 378, "0", "a",
                                                  (char *)(uintptr_t)(0),
                                                  (char *)(uintptr_t)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 378,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            schar: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 378, "0", "a",
                                              (llong)(0), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                              (llong)(0), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 378,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            short: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 378, "0", "a",
                                              (llong)(0), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                              (llong)(0), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 378,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            int: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 378, "0", "a",
                                              (llong)(0), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                              (llong)(0), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                              if (false)
                                  ;
                              else {
                                  __assert_fail(
                                      "false", "assert.c", 378,
                                      __extension__ __PRETTY_FUNCTION__);
                              }
                          }))),
            long: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 378, "0", "a",
                                              (llong)(0), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                              (llong)(0), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 378,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            llong: _Generic((a),
                schar: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                short: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                int: assert_signed_more_equal("assert.c", 378, "0", "a",
                                              (llong)(0), (llong)(a)),
                long: assert_signed_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (llong)(a)),
                llong: assert_signed_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (llong)(a)),
                uchar: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (ullong)(a)),
                ushort: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (ullong)(a)),
                uint: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                              (llong)(0), (ullong)(a)),
                ulong: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                               (llong)(0), (ullong)(a)),
                ullong: assert_si_un_more_equal("assert.c", 378, "0", "a",
                                                (llong)(0), (ullong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 378,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            uchar: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                  (ullong)(0), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                   (ullong)(0), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                 (ullong)(0), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                  (ullong)(0), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                   (ullong)(0), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                             (ullong)(0), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                              (ullong)(0), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 378,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ushort: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                  (ullong)(0), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                   (ullong)(0), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                 (ullong)(0), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                  (ullong)(0), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                   (ullong)(0), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                             (ullong)(0), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                              (ullong)(0), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 378,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))),
            uint: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                  (ullong)(0), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                   (ullong)(0), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                 (ullong)(0), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                  (ullong)(0), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                   (ullong)(0), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                             (ullong)(0), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                              (ullong)(0), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                               if (false)
                                   ;
                               else {
                                   __assert_fail(
                                       "false", "assert.c", 378,
                                       __extension__ __PRETTY_FUNCTION__);
                               }
                           }))),
            ulong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                  (ullong)(0), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                   (ullong)(0), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                 (ullong)(0), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                  (ullong)(0), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                   (ullong)(0), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                             (ullong)(0), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                              (ullong)(0), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                if (false)
                                    ;
                                else {
                                    __assert_fail(
                                        "false", "assert.c", 378,
                                        __extension__ __PRETTY_FUNCTION__);
                                }
                            }))),
            ullong: _Generic((a),
                uchar: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                  (ullong)(0), (ullong)(a)),
                ushort: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                   (ullong)(0), (ullong)(a)),
                uint: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                 (ullong)(0), (ullong)(a)),
                ulong: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                  (ullong)(0), (ullong)(a)),
                ullong: assert_unsigned_more_equal("assert.c", 378, "0", "a",
                                                   (ullong)(0), (ullong)(a)),
                schar: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                short: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                int: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                             (ullong)(0), (llong)(a)),
                long: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                              (ullong)(0), (llong)(a)),
                llong: assert_un_si_more_equal("assert.c", 378, "0", "a",
                                               (ullong)(0), (llong)(a)),
                default: ((void)sizeof((false) ? 1 : 0), __extension__({
                                 if (false)
                                     ;
                                 else {
                                     __assert_fail(
                                         "false", "assert.c", 378,
                                         __extension__ __PRETTY_FUNCTION__);
                                 }
                             }))));
    }
    do {
        if (!(true)) {
            fprintf(stderr, "Assertion '%s' failed at %s:%d\n", "true",
                    "assert.c", 380);
            __builtin_trap();
        }
    } while (0);
    exit(0);
}
