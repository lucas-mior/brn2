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

#if defined(MIN)
#undef MIN
#endif
#if defined(MAX)
#undef MAX
#endif

#define MIN(VAR1, VAR2) (VAR1 < VAR2 ? VAR1 : VAR2)
#define MAX(VAR1, VAR2) (VAR1 > VAR2 ? VAR1 : VAR2)

#if TESTING_minmax
#define CBASE_IMPLEMENT
#include "cbase.h"

int
main(void) {
    {
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
        long min = MIN(a, b);
        long max = MAX(a, b);
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
        double min = (double)MIN(a, b);
        double max = (double)MAX(a, b);
        ASSERT_EQUAL(min, a);
        ASSERT_EQUAL(max, b);
    } {
        long a = MINOF(a);
        ulong b = MAXOF(a);
        double min = (double)MIN(a, b);
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
