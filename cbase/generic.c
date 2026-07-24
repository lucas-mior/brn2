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

#if !defined(GENERIC_C)
#define GENERIC_C

#include <limits.h>
#include <float.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_generic 1
#define CBASE_IMPLEMENT
#elif !defined(TESTING_generic)
#define TESTING_generic 0
#endif

#include "cbase.h"

#include "primitives.h"
#include "base_macros.h"

#include <assert.h>
#include <stdarg.h>
#include <string.h>

static int strlen2(char *string) {
    return (int)strlen(string);
}

static inline int
fprint_0(FILE *restrict fp, ... /* strings, NULL */) {
    int count = 0;
    char *s;

    va_list ap;
    va_start(ap, fp);

    while ((s = (char *)va_arg(ap, char *))) {
        int64 slen;
        if (fputs(s, fp) == EOF) {
            va_end(ap);
            return -1;
        }

        slen = strlen2(s);
        if ((int64)INT_MAX - (int64)count < slen) {
            count = INT_MAX;
        } else {
            count += slen;
        }
    }

    va_end(ap);
    return count;
}

static inline int
snprint_0(char *restrict buf, int64 bufSize, ... /* strings, NULL */) {
    va_list ap;
    int64 remainingLen;
    int64 requiredLen = 0;
    char *dst = buf;
    char *s;

    assert(bufSize >= 0);
    if (bufSize) {
        remainingLen = bufSize - 1;
    } else {
        remainingLen = 0;
    }

    va_start(ap, bufSize);

    if (buf && bufSize) {
        buf[0] = '\0';
    }

    while ((s = va_arg(ap, char *))) {
        int64 sLen = strlen2(s);
        requiredLen += sLen;

        if (dst && remainingLen) {
            int64 copyLen;
            if (remainingLen < sLen) {
                copyLen = remainingLen;
            } else {
                copyLen = sLen;
            }
            memcpy(dst, s, (size_t)copyLen);
            dst += copyLen;
            remainingLen -= copyLen;
            *dst = '\0';
        }
    }

    va_end(ap);
    if (requiredLen > (int64)INT_MAX) {
        return INT_MAX;
    }
    return (int)requiredLen;
}

/* Like snprintf but returns a pointer to the buffer. */
static inline char *
toString(char *restrict buf, int64 bufSize, char *restrict fmt, ...) {
    va_list ap;

    assert(buf);
    assert(bufSize > 0);
    assert(fmt);

    va_start(ap, fmt);
    vsnprintf(buf, (size_t)bufSize, fmt, ap);
    va_end(ap);
    return buf;
}

static double
double_from_voidp(void* x) {
    (void)x;
    TRAP();
    return (double)0.0;
}
static double
double_from_charp(char* x) {
    (void)x;
    TRAP();
    return (double)0.0;
}
static double
double_from_bool(bool x) {
    (void)x;
    TRAP();
    return (double)0.0;
}
static double
double_from_char(char x) {
    (void)x;
    TRAP();
    return (double)0.0;
}
static double double_from_schar  (schar x)   {
    return (double)x;
}
static double double_from_short  (short x)   {
    return (double)x;
}
static double double_from_int    (int x)     {
    return (double)x;
}
static double double_from_long   (long x)    {
    return (double)x;
}
static double double_from_llong  (llong x)   {
    return (double)x;
}
static double double_from_uchar  (uchar x)   {
    return (double)x;
}
static double double_from_ushort (ushort x)  {
    return (double)x;
}
static double double_from_uint   (uint x)    {
    return (double)x;
}
static double double_from_ulong  (ulong x)   {
    return (double)x;
}
static double double_from_ullong (ullong x)  {
    return (double)x;
}
static double double_from_float  (float x)   {
    return (double)x;
}
static double double_from_double (double x)  {
    return (double)x;
}

static llong
typebits(enum Type type) {
    llong size = 0;
    union Primitive primitive;
    void **pointer;

    switch (type) {
    case TYPE_VOIDP:
        pointer = &(primitive.avoidp);
        size = ((char*)(pointer + 1)) - (char*)pointer;
        break;
    case TYPE_CHARP:
        pointer = (void*)&(primitive.acharp);
        size = ((char*)(pointer + 1)) - (char*)pointer;
        break;
    case TYPE_BOOL:    size = sizeof(bool);    break;
    case TYPE_CHAR:    size = sizeof(char);    break;
    case TYPE_SCHAR:   size = sizeof(schar);   break;
    case TYPE_SHORT:   size = sizeof(short);   break;
    case TYPE_INT:     size = sizeof(int);     break;
    case TYPE_LONG:    size = sizeof(long);    break;
    case TYPE_LLONG:   size = sizeof(llong);   break;
    case TYPE_UCHAR:   size = sizeof(uchar);   break;
    case TYPE_USHORT:  size = sizeof(ushort);  break;
    case TYPE_UINT:    size = sizeof(uint);    break;
    case TYPE_ULONG:   size = sizeof(ulong);   break;
    case TYPE_ULLONG:  size = sizeof(ullong);  break;
    case TYPE_FLOAT:   size = sizeof(float);   break;
    case TYPE_DOUBLE:  size = sizeof(double);  break;
    case TYPE_OTHER:
    default: TRAP();
    }
    return size*CHAR_BIT;
}

static char *
typename(enum Type type) {
    switch (type) {
    case TYPE_VOIDP:  return "void*";
    case TYPE_CHARP:  return "char*";
    case TYPE_BOOL:   return "bool";
    case TYPE_CHAR:   return "char";
    case TYPE_SCHAR:  return "schar";
    case TYPE_SHORT:  return "short";
    case TYPE_INT:    return "int";
    case TYPE_LONG:   return "long";
    case TYPE_LLONG:  return "llong";
    case TYPE_UCHAR:  return "uchar";
    case TYPE_USHORT: return "ushort";
    case TYPE_UINT:   return "uint";
    case TYPE_ULONG:  return "ulong";
    case TYPE_ULLONG: return "ullong";
    case TYPE_FLOAT:  return "float";
    case TYPE_DOUBLE: return "double";
    case TYPE_OTHER:
    default:           return "unknown type";
    }
}

static double
double_get(union Primitive var, enum Type type) {
    switch (type) {
    case TYPE_VOIDP:   TRAP(); break;
    case TYPE_CHARP:   TRAP(); break;
    case TYPE_BOOL:    TRAP(); break;
    case TYPE_CHAR:    TRAP(); break;
    case TYPE_SCHAR:   return (double)var.aschar;
    case TYPE_SHORT:   return (double)var.ashort;
    case TYPE_INT:     return (double)var.aint;
    case TYPE_LONG:    return (double)var.along;
    case TYPE_LLONG:   return (double)var.allong;
    case TYPE_UCHAR:   return (double)var.auchar;
    case TYPE_USHORT:  return (double)var.aushort;
    case TYPE_UINT:    return (double)var.auint;
    case TYPE_ULONG:   return (double)var.aulong;
    case TYPE_ULLONG:  return (double)var.aullong;
    case TYPE_FLOAT:   return (double)var.afloat;
    case TYPE_DOUBLE:  return (double)var.adouble;
    case TYPE_OTHER:
    default:           TRAP(); break;
    }
    return (double)0.0;
}

#if 0 == TESTING_generic
static inline void
generic_functions_sink(void) {
    (void)double_from_voidp;
    (void)double_from_charp;
    (void)double_from_bool;
    (void)double_from_char;
    (void)double_from_schar;
    (void)double_from_short;
    (void)double_from_int;
    (void)double_from_long;
    (void)double_from_llong;
    (void)double_from_uchar;
    (void)double_from_ushort;
    (void)double_from_uint;
    (void)double_from_ulong;
    (void)double_from_ullong;
    (void)double_from_float;
    (void)double_from_double;

    (void)typebits;
    (void)typename;
    (void)double_get;
    (void)generic_functions_sink;
    return;
}
#endif

#if TESTING_generic

#include <assert.h>
#include <string.h>
#include <stdio.h>

int
main(void) {
    union Primitive primitive;

    assert(MINOF(primitive.afloat)   == -FLT_MAX);
    assert(MINOF(primitive.aint)     == INT_MIN);
    assert(MINOF(primitive.allong)   == LLONG_MIN);
    assert(MINOF(primitive.along)    == LONG_MIN);
    assert(MINOF(primitive.aschar)   == SCHAR_MIN);
    assert(MINOF(primitive.ashort)   == SHRT_MIN);
    assert(MINOF(primitive.auchar)   == 0);
    assert(MINOF(primitive.auint)    == 0u);
    assert(MINOF(primitive.aullong)  == 0ull);
    assert(MINOF(primitive.aulong)   == 0ul);
    assert(MINOF(primitive.aushort)  == 0);

    assert(MAXOF(primitive.afloat)   == FLT_MAX);
    assert(MAXOF(primitive.aschar)   == SCHAR_MAX);
    assert(MAXOF(primitive.ashort)   == SHRT_MAX);
    assert(MAXOF(primitive.aint)     == INT_MAX);
    assert(MAXOF(primitive.along)    == LONG_MAX);
    assert(MAXOF(primitive.allong)   == LLONG_MAX);
    assert(MAXOF(primitive.auchar)   == UCHAR_MAX);
    assert(MAXOF(primitive.aushort)  == USHRT_MAX);
    assert(MAXOF(primitive.auint)    == UINT_MAX);
    assert(MAXOF(primitive.aulong)   == ULONG_MAX);
    assert(MAXOF(primitive.aullong)  == ULLONG_MAX);
    assert(MAXOF(primitive.abool)    == 1);

    assert(!strcmp(TYPENAME(primitive.avoidp),
                   typename(TYPEID(primitive.avoidp))));
    assert(!strcmp(TYPENAME(primitive.acharp),
                   typename(TYPEID(primitive.acharp))));
    assert(!strcmp(TYPENAME(primitive.abool),
                   typename(TYPEID(primitive.abool))));
    assert(!strcmp(TYPENAME(primitive.aschar),
                   typename(TYPEID(primitive.aschar))));
    assert(!strcmp(TYPENAME(primitive.ashort),
                   typename(TYPEID(primitive.ashort))));
    assert(!strcmp(TYPENAME(primitive.aint),
                   typename(TYPEID(primitive.aint))));
    assert(!strcmp(TYPENAME(primitive.along),
                   typename(TYPEID(primitive.along))));
    assert(!strcmp(TYPENAME(primitive.allong),
                   typename(TYPEID(primitive.allong))));
    assert(!strcmp(TYPENAME(primitive.auchar),
                   typename(TYPEID(primitive.auchar))));
    assert(!strcmp(TYPENAME(primitive.aushort),
                   typename(TYPEID(primitive.aushort))));
    assert(!strcmp(TYPENAME(primitive.auint),
                   typename(TYPEID(primitive.auint))));
    assert(!strcmp(TYPENAME(primitive.aulong),
                   typename(TYPEID(primitive.aulong))));
    assert(!strcmp(TYPENAME(primitive.aullong),
                   typename(TYPEID(primitive.aullong))));
    assert(!strcmp(TYPENAME(primitive.afloat),
                   typename(TYPEID(primitive.afloat))));
    assert(!strcmp(TYPENAME(primitive.adouble),
                   typename(TYPEID(primitive.adouble))));

    {
        int32 var_int32;
        uint32 var_uint32;
        int64 var_int64;
        uint64 var_uint64;

        assert(MAXOF(var_int32) == INT32_MAX);
        assert(MAXOF(var_int64) == INT64_MAX);
        assert(MAXOF(var_uint32) == UINT32_MAX);
        assert(MAXOF(var_uint64) == UINT64_MAX);

        assert(MINOF(var_int32) == INT32_MIN);
        assert(MINOF(var_int64) == INT64_MIN);
        assert(MINOF(var_uint32) == 0u);
        assert(MINOF(var_uint64) == 0ull);
    }

    {
        void* var_voidptr = NULL;
        char* var_string = "a nice string";
        char var_buffer[128] = "a nice buffer";
        bool var_bool = true;
        char var_char = 'c';
        int8 var_int8 = INT8_MAX;
        int16 var_int16 = INT16_MAX;
        int32 var_int32 = INT32_MAX;
        int var_int = INT_MAX;
        int64 var_int64 = INT64_MAX;
        uint8 var_uint8 = UINT8_MAX;
        uint16 var_uint16 = UINT16_MAX;
        uint32 var_uint32 = UINT32_MAX;
        uint var_uint = UINT_MAX;
        uint64 var_uint64 = UINT64_MAX;
        float var_float = FLT_MAX;
#if !defined(__CPROC__)
        // DBL_MAX is defined with L suffix in the gcc headers,
        // which means long double,
        // and cproc uses the gcc pre processor
        // and cproc uses the gcc headers (at least in this case),
        // and qbe does not support long double,
        // which means that cproc also does not support long double
        double var_double = DBL_MAX;
#else
        double var_double = 1e300;
#endif

        PRINTLN(var_voidptr);
        PRINTLN(var_string);
        PRINTLN(var_buffer);
        PRINTLN(var_bool);
        PRINTLN(var_char);
        PRINTLN(var_int8);
        PRINTLN(var_int16);
        PRINT(var_int32);
        PRINTLN(var_int);
        PRINTLN(var_int64);
        PRINTLN(var_uint8);
        PRINTLN(var_uint16);
        PRINTLN(var_uint32);
        PRINTLN(var_uint);
        PRINTLN(var_uint64);
        PRINTLN(var_float);
        PRINTLN(var_double);

        PRINTLN(*var_string);
        PRINTLN(var_uint - (uint)var_int);
    }

    {
        char a = 'i';
        char *b = "able";
        int c = 1;
        double d = (double)8.0;
        char *e = "a long string that won't fit in the compound literal "
                  "buffer. "
                  "You can print it using the W(X) macro.";
        char buf[512];
        char expected[512];
        char small[8];
        FILE *fp;
        int n;

        assert(!strcmp(S_(a), "i"));
        assert(!strcmp(S_(b), "able"));
        assert(!strcmp(S_(c), "1"));
        assert(!strcmp(S_((uint)42), "42"));
        assert(!strcmp(S_((long)-42), "-42"));
        assert(!strcmp(S_((ullong)42), "42"));
        assert(!strcmp(S_(true), "1"));
        assert(!strcmp(S_(false), "0"));
        assert(!strcmp(SF("0x%02x", 10), "0x0a"));

        n = snprint(buf, sizeof(buf),
                    "Now you can insert var" V(a) V(b) "s in situ:\n"
                    V(c) " divided by " V(d) " equals " V(c/d) "\n");
        assert(n == strlen2("Now you can insert variables in situ:\n"
                            "1 divided by 8 equals 0.125\n"));

        assert(!strcmp(buf, "Now you can insert variables in situ:\n"
                            "1 divided by 8 equals 0.125\n"));

        n = snprint(buf, sizeof(buf),
                    "This is " W(e) " It's " V(strlen(e)) " characters long\n");
        snprintf(expected, sizeof(expected),
                 "This is %s It's %lu characters long\n",
                 e, (ulong)strlen(e));
        assert(n == strlen2(expected));
        assert(!strcmp(buf, expected));

        n = snprint(buf, sizeof(buf),
                    "custom " VF("%04i", c) " " VF("%c", a) "\n");
        assert(n == strlen2("custom 0001 i\n"));
        assert(!strcmp(buf, "custom 0001 i\n"));

        n = snprint(small, sizeof(small), "prefix-" W(e));
        assert(n == (int)(strlen("prefix-") + strlen(e)));
        assert(!strcmp(small, "prefix-"));

        fp = tmpfile();
        assert(fp);
        n = fprint(fp, "file ", V(c), " ", VF("%04i", c), "\n");
        assert(n == strlen2("file 1 0001\n"));
        rewind(fp);
        assert(fgets(buf, sizeof(buf), fp));
        assert(!strcmp(buf, "file 1 0001\n"));
        fclose(fp);

        n = print0("print ", V(a), " ", W(b), "\n");
        assert(n == strlen2("print i able\n"));
        {
            char buffer[16];
            assert((print0(V(c), "\n")
                    == snprintf(buffer, sizeof(buffer), "%d\n", c)));
        }
    }
}

#endif

#endif /* GENERIC_C */
