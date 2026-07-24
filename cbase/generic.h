#if !defined(GENERIC_H)
#define GENERIC_H

#include <stdbool.h>
#include <stdio.h>

#include "primitives.h"

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

static double double_from_bool(bool);
static double double_from_char(char);
static double double_from_charp(char *);
static double double_from_double(double);
static double double_from_float(float);
static double double_from_int(int);
static double double_from_llong(llong);
static double double_from_long(long);
static double double_from_schar(schar);
static double double_from_short(short);
static double double_from_uchar(uchar);
static double double_from_uint(uint);
static double double_from_ullong(ullong);
static double double_from_ulong(ulong);
static double double_from_ushort(ushort);
static double double_from_voidp(void *);
static double double_get(union Primitive, enum Type);
static int fprint_0(FILE *restrict, ...);
static void generic_functions_sink(void);
static int snprint_0(char *restrict, int64, ...);
static int strlen2(char *);
static char *toString(char *restrict, int64, char *restrict, ...);
static llong typebits(enum Type);
static char *typename(enum Type);
void UNSUPPORTED_TYPE_FOR_DOUBLE_GET_GENERIC(void);

#include "generic_macros.h"

#endif /* GENERIC_H */
