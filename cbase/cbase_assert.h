#if !defined(CBASE_ASSERT_H)
#define CBASE_ASSERT_H

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

#include "assert_macros.h"

#endif /* CBASE_ASSERT_H */
