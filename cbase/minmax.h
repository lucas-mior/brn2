#if !defined(MINMAX_H)
#define MINMAX_H

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

#include "minmax_macros.h"

#endif /* MINMAX_H */
