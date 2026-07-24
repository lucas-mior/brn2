/*
 * Copyright (C) 2025 Mior, Lucas;
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the*License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(ASSERT_C)
#define ASSERT_C

#include <assert.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_assert 1
#elif !defined(TESTING_assert)
#define TESTING_assert 0
#endif

#include "cbase.h"

#include "primitives.h"

static void
assert_file_contains(char *file, int32 line, char *func,
                     char *path, char *needle) {
    FILE *file_handle;
    char buffer[4096];
    bool found = false;

    if ((file_handle = fopen(path, "r")) == NULL) {
        error_impl(file, line, func,
                   "Error opening %s for reading: %s.\n",
                   path, strerror(errno));
        fatal(EXIT_FAILURE);
    }
    while (fgets(buffer, SIZEOF(buffer), file_handle)) {
        if (strstr(buffer, needle)) {
            found = true;
            break;
        }
    }
    if (fclose(file_handle)) {
        error_impl(file, line, func,
                   "Error closing %s: %s.\n", path, strerror(errno));
    }
    if (!found) {
        error_impl(file, line, func,
                   "Needle '%s' not found in '%s'.\n", needle, path);
        fatal(EXIT_FAILURE);
    }
    return;
}

static void
assert_contains(char *file, int32 line, char *func,
                char *haystack, int32 haystack_len, char *needle) {
    int32 needle_len = strlen32(needle);
    if (memmem64(haystack, haystack_len, needle, needle_len) == NULL) {
        error_impl(file, line, func,
                   "expected to find substring:\n%.*s\n--- in ---\n%.*s",
                   needle_len, needle, haystack_len, haystack);
        fatal(EXIT_FAILURE);
    }
}

static void
assert_not_contains(char *file, int32 line, char *func,
                    char *haystack, int32 haystack_len, char *needle) {
    int32 needle_len = strlen32(needle);
    if (memmem64(haystack, haystack_len, needle, needle_len)) {
        error_impl(file, line, func,
                   "did not expect to find substring:\n%.*s\n--- in ---\n%.*s",
                   needle_len, needle, haystack_len, haystack);
        fatal(EXIT_FAILURE);
    }
}

#define GENERATE_ASSERT_STRINGS(MODE, SYMBOL)                            \
static void                                                              \
a_strings_##MODE(char *file, uint line, char *func,                      \
                 char *name1, char *name2,                               \
                 char *var1, char *var2) {                               \
    if (var1 == NULL) {                                                  \
        error2("\nError in assertion at %s:%u:%s\n", file, line, func);  \
        error2("%s is NULL.\n", name1);                                   \
        TRAP();                                                          \
    }                                                                    \
    if (var2 == NULL) {                                                  \
        error2("\nError in assertion at %s:%u:%s\n", file, line, func);  \
        error2("%s is NULL.\n", name2);                                   \
        TRAP();                                                          \
    }                                                                    \
    if (!(strcmp(var1, var2) SYMBOL 0)) {                                \
        error2("\nError in assertion at %s:%u:%s\n", file, line, func);  \
        error2("%s = %s " #SYMBOL " %s = %s\n",                          \
               name1, var1, var2, name2);                                \
        TRAP();                                                          \
    }                                                                    \
    return;                                                              \
}

GENERATE_ASSERT_STRINGS(less, <)
GENERATE_ASSERT_STRINGS(less_equal, <=)
GENERATE_ASSERT_STRINGS(equal, ==)
GENERATE_ASSERT_STRINGS(not_equal, !=)
GENERATE_ASSERT_STRINGS(more, >)
GENERATE_ASSERT_STRINGS(more_equal, >=)

#undef GENERATE_ASSERT_STRINGS

#define GENERATE_ASSERT_POINTERS(MODE, SYMBOL)                           \
static void                                                              \
a_pointers_##MODE(char *file, uint line, char *func,                     \
                  char *name1, char *name2,                              \
                  void *var1, void *var2) {                              \
    if (!((uintptr_t)var1 SYMBOL (uintptr_t)var2)) {                     \
        if (!DEBUGGING) {                                                \
            UNREACHABLE();                                               \
        }                                                                \
        error2("\nAssertion failed at %s:%u:%s\n", file, line, func);    \
        error2("%s = %p " #SYMBOL " %p = %s\n",                          \
               name1, var1, var2, name2);                                \
        TRAP();                                                          \
    }                                                                    \
    return;                                                              \
}

GENERATE_ASSERT_POINTERS(less, <)
GENERATE_ASSERT_POINTERS(less_equal, <=)
GENERATE_ASSERT_POINTERS(equal, ==)
GENERATE_ASSERT_POINTERS(not_equal, !=)
GENERATE_ASSERT_POINTERS(more, >)
GENERATE_ASSERT_POINTERS(more_equal, >=)

#undef GENERATE_ASSERT_POINTERS

#define GENERATE_ASSERT_INTEGERS_SAME_SIGN(TYPE, FORMAT, SYMBOL, MODE)      \
static void                                                                 \
a_both_##TYPE##_##MODE(char *file, uint line, char *func,                   \
                       char *name1, char *name2,                            \
                       char *type1, char *type2,                            \
                       llong bits1, llong bits2,                            \
                       TYPE long long var1, TYPE long long var2) {          \
    if (!(var1 SYMBOL var2)) {                                              \
        if (!DEBUGGING) {                              \
            UNREACHABLE(); \
        } \
        error2("\nAssertion failed at %s:%u:%s\n", file, line, func);       \
        error2("[%s%lld]%s = "FORMAT" " #SYMBOL " "FORMAT" = %s[%s%lld]\n", \
               type1, bits1, name1, var1, var2, name2, type2, bits2);       \
        TRAP();                                                             \
    }                                                                       \
    return;                                                                 \
}

GENERATE_ASSERT_INTEGERS_SAME_SIGN(signed, "%lld", ==, equal)
GENERATE_ASSERT_INTEGERS_SAME_SIGN(unsigned, "%llx", ==, equal)
GENERATE_ASSERT_INTEGERS_SAME_SIGN(signed, "%lld", !=, not_equal)
GENERATE_ASSERT_INTEGERS_SAME_SIGN(unsigned, "%llx", !=, not_equal)
GENERATE_ASSERT_INTEGERS_SAME_SIGN(signed, "%lld", <, less)
GENERATE_ASSERT_INTEGERS_SAME_SIGN(unsigned, "%llx", <, less)
GENERATE_ASSERT_INTEGERS_SAME_SIGN(signed, "%lld", <=, less_equal)
GENERATE_ASSERT_INTEGERS_SAME_SIGN(unsigned, "%llx", <=, less_equal)
GENERATE_ASSERT_INTEGERS_SAME_SIGN(signed, "%lld", >, more)
GENERATE_ASSERT_INTEGERS_SAME_SIGN(unsigned, "%llx", >, more)
GENERATE_ASSERT_INTEGERS_SAME_SIGN(signed, "%lld", >=, more_equal)
GENERATE_ASSERT_INTEGERS_SAME_SIGN(unsigned, "%llx", >=, more_equal)

#undef GENERATE_ASSERT_INTEGERS_SAME_SIGN

static int
compare_sign_with_unsign(llong s, ullong u) {
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

#define GENERATE_ASSERT_SIGNED_UNSIGNED(MODE, SYMBOL)                   \
static void                                                             \
a_signed_unsigned##MODE(char *file, uint line, char *func,              \
                        char *name1, char *name2,                       \
                        char *type1, char *type2,                       \
                        llong bits1, llong bits2,                       \
                        llong var1, ullong var2) {                      \
    if (!(compare_sign_with_unsign(var1, var2) SYMBOL 0)) {             \
        if (!DEBUGGING) {                              \
            UNREACHABLE(); \
        } \
        error2("\nAssertion failed at %s:%u:%s\n", file, line, func);   \
        error2("[%s%lld]%s = %lld " #SYMBOL " %llu = %s[%s%lld]\n",     \
               type1, bits1, name1, var1, var2, name2, type2, bits2);   \
        TRAP();                                                         \
    }                                                                   \
    return;                                                             \
}

GENERATE_ASSERT_SIGNED_UNSIGNED(equal, ==)
GENERATE_ASSERT_SIGNED_UNSIGNED(not_equal, !=)
GENERATE_ASSERT_SIGNED_UNSIGNED(less, <)
GENERATE_ASSERT_SIGNED_UNSIGNED(less_equal, <=)
GENERATE_ASSERT_SIGNED_UNSIGNED(more, >)
GENERATE_ASSERT_SIGNED_UNSIGNED(more_equal, >=)

#undef GENERATE_ASSERT_SIGNED_UNSIGNED

#define GENERATE_ASSERT_UNSIGNED_SIGNED(MODE, SYMBOL)                   \
static void                                                             \
a_unsigned_signed_##MODE(char *file, uint line, char *func,             \
                         char *name1, char *name2,                      \
                         char *type1, char *type2,                      \
                         llong bits1, llong bits2,                      \
                         ullong var1, llong var2) {                     \
    if (!((-compare_sign_with_unsign(var2, var1)) SYMBOL 0)) {          \
        if (!DEBUGGING) {                              \
            UNREACHABLE(); \
        } \
        error2("\nAssertion failed at %s:%u:%s\n", file, line, func);   \
        error2("[%s%lld]%s = %llu " #SYMBOL " %lld = %s[%s%lld]\n",     \
               type1, bits1, name1, var1, var2, name2, type2, bits2);   \
        TRAP();                                                         \
    }                                                                   \
    return;                                                             \
}

GENERATE_ASSERT_UNSIGNED_SIGNED(equal, ==)
GENERATE_ASSERT_UNSIGNED_SIGNED(not_equal, !=)
GENERATE_ASSERT_UNSIGNED_SIGNED(less, <)
GENERATE_ASSERT_UNSIGNED_SIGNED(less_equal, <=)
GENERATE_ASSERT_UNSIGNED_SIGNED(more, >)
GENERATE_ASSERT_UNSIGNED_SIGNED(more_equal, >=)

#undef GENERATE_ASSERT_UNSIGNED_SIGNED

static double
assert_double_abs(double x) {
    if (x < (double)0) {
        return -x;
    }
    return x;
}

static bool
assert_double_is_infinite(double x) {
    return (x > (double)DBL_MAX) || (x < -(double)DBL_MAX);
}

static int
assert_fp_common_kind(int kind1, int kind2) {
    if ((kind1 == ASSERT_FP_KIND_FLOAT) || (kind2 == ASSERT_FP_KIND_FLOAT)) {
        return ASSERT_FP_KIND_FLOAT;
    }
    if ((kind1 == ASSERT_FP_KIND_DOUBLE) || (kind2 == ASSERT_FP_KIND_DOUBLE)) {
        return ASSERT_FP_KIND_DOUBLE;
    }
    return ASSERT_FP_KIND_DOUBLE;
}

static double
assert_fp_rel_tol(int common_kind) {
    if (common_kind == ASSERT_FP_KIND_FLOAT) {
        return ASSERT_FLOAT_REL_TOL;
    }
    if (common_kind == ASSERT_FP_KIND_DOUBLE) {
        return ASSERT_DOUBLE_REL_TOL;
    }
    return ASSERT_DOUBLE_REL_TOL;
}

static double
assert_fp_abs_tol(int common_kind) {
    if (common_kind == ASSERT_FP_KIND_FLOAT) {
        return ASSERT_FLOAT_ABS_TOL;
    }
    if (common_kind == ASSERT_FP_KIND_DOUBLE) {
        return ASSERT_DOUBLE_ABS_TOL;
    }
    return ASSERT_DOUBLE_ABS_TOL;
}

static bool
assert_double_almost_equal(double var1, double var2,
                           int kind1, int kind2,
                           double *diff_out,
                           double *abs_tol_out,
                           double *rel_tol_out) {
    int common_kind;
    double diff;
    double abs1;
    double abs2;
    double scale;
    double abs_tol;
    double rel_tol;

    common_kind = assert_fp_common_kind(kind1, kind2);
    abs_tol = assert_fp_abs_tol(common_kind);
    rel_tol = assert_fp_rel_tol(common_kind);

    if (abs_tol < (double)0) {
        abs_tol = -abs_tol;
    }
    if (rel_tol < (double)0) {
        rel_tol = -rel_tol;
    }

    if (diff_out != NULL) {
        *diff_out = (double)0;
    }
    if (abs_tol_out != NULL) {
        *abs_tol_out = abs_tol;
    }
    if (rel_tol_out != NULL) {
        *rel_tol_out = rel_tol;
    }

    /* Exact equality catches identical finite values and matching infinities.
     */
    if (var1 == var2) {
        return true;
    }

    /* NaNs are not approximately equal to anything, including themselves. */
    if ((var1 != var1) || (var2 != var2)) {
        if (diff_out != NULL) {
            *diff_out = var1 - var2;
        }
        return false;
    }

    /* Mismatched infinities are ordered but never approximately equal. */
    if (assert_double_is_infinite(var1) || assert_double_is_infinite(var2)) {
        if (diff_out != NULL) {
            *diff_out = assert_double_abs(var1 - var2);
        }
        return false;
    }

    diff = assert_double_abs(var1 - var2);
    if (diff_out != NULL) {
        *diff_out = diff;
    }

    if (diff <= abs_tol) {
        return true;
    }

    abs1 = assert_double_abs(var1);
    abs2 = assert_double_abs(var2);
    if (abs1 > abs2) {
        scale = abs1;
    } else {
        scale = abs2;
    }

    return diff <= rel_tol*scale;
}

static bool
assert_double_less(double var1, double var2, int kind1, int kind2) {
    return (var1 < var2)
           && !assert_double_almost_equal(var1, var2, kind1, kind2,
                                           NULL, NULL, NULL);
}

static bool
assert_double_more(double var1, double var2, int kind1, int kind2) {
    return (var1 > var2)
           && !assert_double_almost_equal(var1, var2, kind1, kind2,
                                           NULL, NULL, NULL);
}

static void __attribute((noreturn))
assert_double_failure(char *file, uint line, char *func,
                      char *name1, char *name2,
                      char *type1, char *type2,
                      llong bits1, llong bits2,
                      double var1, double var2, char *symbol,
                      double diff, double abs_tol, double rel_tol) {
    if (!DEBUGGING) {
        UNREACHABLE();
    }
    error2("\nAssertion failed at %s:%u:%s\n", file, line, func);
    error2("[%s%lld]%s = %f %s %f = %s[%s%lld]\n",
           type1, bits1, name1, var1, symbol, var2, name2, type2, bits2);
    error2("floating diff = %f, abs_tol = %f, rel_tol = %f\n",
           diff, abs_tol, rel_tol);
    TRAP();
    exit(EXIT_FAILURE);
}

static void
a_double_equal(char *file, uint line, char *func,
               char *name1, char *name2,
               char *type1, char *type2,
               llong bits1, llong bits2,
               int kind1, int kind2,
               double var1, double var2) {
    double diff;
    double abs_tol;
    double rel_tol;
    if (!assert_double_almost_equal(var1, var2, kind1, kind2,
                                     &diff, &abs_tol, &rel_tol)) {
        assert_double_failure(file, line, func, name1, name2,
                               type1, type2, bits1, bits2,
                               var1, var2, "~=", diff, abs_tol, rel_tol);
    }
    return;
}

static void
a_double_not_equal(char *file, uint line, char *func,
                   char *name1, char *name2,
                   char *type1, char *type2,
                   llong bits1, llong bits2,
                   int kind1, int kind2,
                   double var1, double var2) {
    double diff;
    double abs_tol;
    double rel_tol;
    if (assert_double_almost_equal(var1, var2, kind1, kind2,
                                    &diff, &abs_tol, &rel_tol)) {
        assert_double_failure(file, line, func, name1, name2,
                               type1, type2, bits1, bits2,
                               var1, var2, "!~=", diff, abs_tol, rel_tol);
    }
    return;
}

static void
a_double_less(char *file, uint line, char *func,
              char *name1, char *name2,
              char *type1, char *type2,
              llong bits1, llong bits2,
              int kind1, int kind2,
              double var1, double var2) {
    double diff;
    double abs_tol;
    double rel_tol;
    (void)assert_double_almost_equal(var1, var2, kind1, kind2,
                                      &diff, &abs_tol, &rel_tol);
    if (!assert_double_less(var1, var2, kind1, kind2)) {
        assert_double_failure(file, line, func, name1, name2,
                               type1, type2, bits1, bits2,
                               var1, var2, "<", diff, abs_tol, rel_tol);
    }
    return;
}

static void
a_double_less_equal(char *file, uint line, char *func,
                    char *name1, char *name2,
                    char *type1, char *type2,
                    llong bits1, llong bits2,
                    int kind1, int kind2,
                    double var1, double var2) {
    double diff;
    double abs_tol;
    double rel_tol;
    if (!((var1 < var2) || assert_double_almost_equal(var1, var2, kind1, kind2,
                                                       &diff, &abs_tol,
                                                       &rel_tol))) {
        (void)assert_double_almost_equal(var1, var2, kind1, kind2,
                                          &diff, &abs_tol, &rel_tol);
        assert_double_failure(file, line, func, name1, name2,
                               type1, type2, bits1, bits2,
                               var1, var2, "<=", diff, abs_tol, rel_tol);
    }
    return;
}

static void
a_double_more(char *file, uint line, char *func,
              char *name1, char *name2,
              char *type1, char *type2,
              llong bits1, llong bits2,
              int kind1, int kind2,
              double var1, double var2) {
    double diff;
    double abs_tol;
    double rel_tol;
    (void)assert_double_almost_equal(var1, var2, kind1, kind2,
                                      &diff, &abs_tol, &rel_tol);
    if (!assert_double_more(var1, var2, kind1, kind2)) {
        assert_double_failure(file, line, func, name1, name2,
                               type1, type2, bits1, bits2,
                               var1, var2, ">", diff, abs_tol, rel_tol);
    }
    return;
}

static void
a_double_more_equal(char *file, uint line, char *func,
                    char *name1, char *name2,
                    char *type1, char *type2,
                    llong bits1, llong bits2,
                    int kind1, int kind2,
                    double var1, double var2) {
    double diff;
    double abs_tol;
    double rel_tol;
    if (!((var1 > var2) || assert_double_almost_equal(var1, var2, kind1, kind2,
                                                       &diff, &abs_tol,
                                                       &rel_tol))) {
        (void)assert_double_almost_equal(var1, var2, kind1, kind2,
                                          &diff, &abs_tol, &rel_tol);
        assert_double_failure(file, line, func, name1, name2,
                               type1, type2, bits1, bits2,
                               var1, var2, ">=", diff, abs_tol, rel_tol);
    }
    return;
}

#define GENERATE_ASSERT_BOOLS(MODE, SYMBOL)                            \
static void                                                            \
a_bool_##MODE(char *file, uint line, char *func,                       \
              char *name1, char *name2,                                \
              char *type1, char *type2,                                \
              llong bits1, llong bits2,                                \
              bool var1, bool var2) {                                  \
    if (!(var1 SYMBOL var2)) {                                         \
        char *s1 = "false";                                            \
        char *s2 = "false";                                            \
        if (!DEBUGGING) {                                              \
            UNREACHABLE();                                   \
        }                                                              \
        if (var1) {                                                    \
            s1 = "true";                                               \
        }                                                              \
        if (var2) {                                                    \
            s2 = "true";                                               \
        }                                                              \
        error2("\nAssertion failed at %s:%u:%s\n", file, line, func);  \
        error2("[%s%lld]%s = %s " #SYMBOL " %s = %s[%s%lld]\n",        \
               type1, bits1, name1, s1, s2, name2, type2, bits2);      \
        TRAP();                                                        \
    }                                                                  \
    return;                                                            \
}

GENERATE_ASSERT_BOOLS(equal, ==)
GENERATE_ASSERT_BOOLS(not_equal, !=)
GENERATE_ASSERT_BOOLS(less, <)
GENERATE_ASSERT_BOOLS(less_equal, <=)
GENERATE_ASSERT_BOOLS(more, >)
GENERATE_ASSERT_BOOLS(more_equal, >=)

#undef GENERATE_ASSERT_BOOLS

#if 0 == TESTING_assert
static inline void
assert_functions_sink(void) {
    (void)a_strings_less;
    (void)a_strings_less_equal;
    (void)a_strings_equal;
    (void)a_strings_not_equal;
    (void)a_strings_more;
    (void)a_strings_more_equal;

    (void)a_pointers_less;
    (void)a_pointers_less_equal;
    (void)a_pointers_equal;
    (void)a_pointers_not_equal;
    (void)a_pointers_more;
    (void)a_pointers_more_equal;

    (void)a_both_signed_less;
    (void)a_both_signed_less_equal;
    (void)a_both_signed_equal;
    (void)a_both_signed_not_equal;
    (void)a_both_signed_more;
    (void)a_both_signed_more_equal;

    (void)a_both_unsigned_less;
    (void)a_both_unsigned_less_equal;
    (void)a_both_unsigned_equal;
    (void)a_both_unsigned_not_equal;
    (void)a_both_unsigned_more;
    (void)a_both_unsigned_more_equal;

    (void)a_signed_unsignedless;
    (void)a_signed_unsignedless_equal;
    (void)a_signed_unsignedequal;
    (void)a_signed_unsignednot_equal;
    (void)a_signed_unsignedmore;
    (void)a_signed_unsignedmore_equal;

    (void)a_unsigned_signed_less;
    (void)a_unsigned_signed_less_equal;
    (void)a_unsigned_signed_equal;
    (void)a_unsigned_signed_not_equal;
    (void)a_unsigned_signed_more;
    (void)a_unsigned_signed_more_equal;

    (void)a_double_less;
    (void)a_double_less_equal;
    (void)a_double_equal;
    (void)a_double_not_equal;
    (void)a_double_more;
    (void)a_double_more_equal;
    (void)assert_file_contains;
    (void)assert_contains;
    (void)assert_not_contains;
    (void)a_bool_less;
    (void)a_bool_less_equal;
    (void)a_bool_more;
    (void)a_bool_more_equal;

    (void)a_bool_equal;
    (void)a_bool_not_equal;
    return;
}
#endif

#if TESTING_assert
#define CBASE_IMPLEMENT
#include "cbase.h"
#include <setjmp.h>
#include <signal.h>

static sig_atomic_t assertion_failed = false;
static sigjmp_buf assert_env;

static void __attribute__((noreturn))
handler_failed_assertion(int unused) {
    (void)unused;
    assertion_failed = true;
    siglongjmp(assert_env, 1);
}

int
main(void) {
    {
        char *string = NULL;
        void *pointer = NULL;
        ASSERT_EQUAL(string, pointer);
        ASSERT_NULL(string);
    }
    {
        int a = 1;
        int b = 1;
        ASSERT_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE_EQUAL(a, b);
    } {
        int a = 1;
        uint b = 1;
        ASSERT_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE_EQUAL(a, b);
    } {
        int a = 1;
        uint b = 2;
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    } {
        long a = -1;
        ulong b = 0;
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    } {
        long a = MINOF(a);
        ulong b = MAXOF(b);
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    } {
        ulong a = MINOF(a);
        long b = MAXOF(b);
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    } {
        char *a = "aaa";
        char *b = "aaa";
        ASSERT_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE_EQUAL(b, a);
    } {
        char *a = "aaa";
        char *b = "bbb";
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    } {
        long a = -1;
        ASSERT_NOT_EQUAL(a, 0);
        ASSERT_LESS(a, 0);
        ASSERT_LESS_EQUAL(a, 0);
        ASSERT_MORE(0, a);
        ASSERT_MORE_EQUAL(0, a);
    } {
        double a = 0.123;
        ASSERT_NOT_EQUAL(a, 0.123000001);
        ASSERT_LESS(a, 0.123000001);
        ASSERT_LESS_EQUAL(a, 0.123000001);
        ASSERT_MORE(0.123000001, a);
        ASSERT_MORE_EQUAL(0.123000001, a);
    } {
        double a = 0.1 + 0.2;
        double b = 0.3;
        ASSERT_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE_EQUAL(a, b);
        ASSERT_NOT_EQUAL(a, b + 1.0e-9);
    } {
        float a = 0.1f + 0.2f;
        double b = 0.3;
        ASSERT_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE_EQUAL(a, b);
    } {
        long a = -1;
        double b = -1;
        ASSERT_EQUAL(a, b);
        ASSERT_EQUAL(b, b);
        ASSERT_MORE_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
    } {
        double a = -1;
        long b = -1;
        ASSERT_EQUAL(a, b);
        ASSERT_EQUAL(b, b);
        ASSERT_MORE_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
    } {
        double a = -1;
        double b = 0;
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    } {
        float a = -1;
        double b = 1;
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    } {
        llong a = 1;
        double b = 1;
        ASSERT_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE_EQUAL(b, a);
    } {
        void *a = NULL;
        void *b = &a;
        ASSERT_NOT_EQUAL(a, b);
    } {
        int array[100];
        void *a = &array[0];
        void *b = &array[1];
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_LESS(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE(b, a);
        ASSERT_MORE_EQUAL(b, a);
    } {
        bool a = true;
        bool b = true;
        ASSERT_EQUAL(a, b);
        ASSERT_LESS_EQUAL(a, b);
        ASSERT_MORE_EQUAL(a, b);
    } {
        bool a = true;
        bool b = false;
        ASSERT_NOT_EQUAL(a, b);
        ASSERT_MORE(a, b);
        ASSERT_MORE_EQUAL(a, b);
        ASSERT_LESS(b, a);
        ASSERT_LESS_EQUAL(b, a);
    } {
        // uncomment to trigger linking error
        /* double x = 0.1; */
        /* void *a = NULL; */
        /* ASSERT_MORE_EQUAL(x, a); */
        /* ASSERT_MORE_EQUAL(a, x); */
        /* bool b = true; */
        /* ASSERT_EQUAL(b, 1); */
    } {
        int a = 0;
        double b = 1;
        float array[10] = {0};
        char *string_null = NULL;
        char *string_some = "some";

        struct sigaction signal_action;
        signal_action.sa_handler = handler_failed_assertion;
        sigemptyset(&signal_action.sa_mask);
        signal_action.sa_flags = SA_RESTART;

        if (sigaction(SIGILL, &signal_action, NULL) != 0) {
            error2("Error in sigaction: %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        error2("\nThe following assertions are supposed to fail");
        error2("\nand then check the 'assertion_failed' variable\n");

        if (sigsetjmp(assert_env, 1) == 0) {
            ASSERT_EQUAL(a, b);
        }
        ASSERT(assertion_failed);
        assertion_failed = false;

        if (sigsetjmp(assert_env, 1) == 0) {
            ASSERT_EQUAL(string_null, string_some);
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

        if (sigsetjmp(assert_env, 1) == 0) {
            ASSERT_LESS((void *)&array[1], (void *)&array[0]);
        }
        ASSERT(assertion_failed);
        assertion_failed = false;

        if (sigsetjmp(assert_env, 1) == 0) {
            ASSERT_EQUAL(true, false);
        }
        ASSERT(assertion_failed);
        assertion_failed = false;
    }
    ASSERT(true);
    ASSERT(!false);
    ASSERT_EQUAL(true, true);
    ASSERT_EQUAL(0 < 1, 1 < 2);
    exit(EXIT_SUCCESS);
}
#endif

#endif /* ASSERT_C */
