// SPDX-License-Identifier: AGPL
// Copyright (c) 2026 Lucas Mior

#if !defined(GENERIC_C)
#define GENERIC_C

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_generic 1
#elif !defined(TESTING_generic)
#define TESTING_generic 0
#endif

#if TESTING_generic
#define CBASE_IMPLEMENT
#endif

#include "cbase.h"

#if TESTING_generic
int
main(void) {
    union Primitive primitive;

    assert(MINOF(primitive.afloat)  == -FLT_MAX);
    assert(MINOF(primitive.aint)    == INT_MIN);
    assert(MINOF(primitive.allong)  == LLONG_MIN);
    assert(MINOF(primitive.along)   == LONG_MIN);
    assert(MINOF(primitive.aschar)  == SCHAR_MIN);
    assert(MINOF(primitive.ashort)  == SHRT_MIN);
    assert(MINOF(primitive.auchar)  == 0);
    assert(MINOF(primitive.auint)   == 0u);
    assert(MINOF(primitive.aullong) == 0ull);
    assert(MINOF(primitive.aulong)  == 0ul);
    assert(MINOF(primitive.aushort) == 0);

    assert(MAXOF(primitive.afloat)  == FLT_MAX);
    assert(MAXOF(primitive.aschar)  == SCHAR_MAX);
    assert(MAXOF(primitive.ashort)  == SHRT_MAX);
    assert(MAXOF(primitive.aint)    == INT_MAX);
    assert(MAXOF(primitive.along)   == LONG_MAX);
    assert(MAXOF(primitive.allong)  == LLONG_MAX);
    assert(MAXOF(primitive.auchar)  == UCHAR_MAX);
    assert(MAXOF(primitive.aushort) == USHRT_MAX);
    assert(MAXOF(primitive.auint)   == UINT_MAX);
    assert(MAXOF(primitive.aulong)  == ULONG_MAX);
    assert(MAXOF(primitive.aullong) == ULLONG_MAX);
    assert(MAXOF(primitive.abool)   == 1);

    assert(strequal(TYPENAME(primitive.avoidp),
                    typename(TYPEID(primitive.avoidp))));
    assert(strequal(TYPENAME(primitive.acharp),
                    typename(TYPEID(primitive.acharp))));
    assert(strequal(TYPENAME(primitive.abool),
                    typename(TYPEID(primitive.abool))));
    assert(strequal(TYPENAME(primitive.aschar),
                    typename(TYPEID(primitive.aschar))));
    assert(strequal(TYPENAME(primitive.ashort),
                    typename(TYPEID(primitive.ashort))));
    assert(strequal(TYPENAME(primitive.aint),
                    typename(TYPEID(primitive.aint))));
    assert(strequal(TYPENAME(primitive.along),
                    typename(TYPEID(primitive.along))));
    assert(strequal(TYPENAME(primitive.allong),
                    typename(TYPEID(primitive.allong))));
    assert(strequal(TYPENAME(primitive.auchar),
                    typename(TYPEID(primitive.auchar))));
    assert(strequal(TYPENAME(primitive.aushort),
                    typename(TYPEID(primitive.aushort))));
    assert(strequal(TYPENAME(primitive.auint),
                    typename(TYPEID(primitive.auint))));
    assert(strequal(TYPENAME(primitive.aulong),
                    typename(TYPEID(primitive.aulong))));
    assert(strequal(TYPENAME(primitive.aullong),
                    typename(TYPEID(primitive.aullong))));
    assert(strequal(TYPENAME(primitive.afloat),
                    typename(TYPEID(primitive.afloat))));
    assert(strequal(TYPENAME(primitive.adouble),
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
        double var_double = DBL_MAX;

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
        char small2[8];
        FILE *fp;
        int n;

        assert(strequal(S_(a), "i"));
        assert(strequal(S_(b), "able"));
        assert(strequal(S_(c), "1"));
        assert(strequal(S_((uint)42), "42"));
        assert(strequal(S_((long)-42), "-42"));
        assert(strequal(S_((ullong)42), "42"));
        assert(strequal(S_(true), "1"));
        assert(strequal(S_(false), "0"));
        assert(strequal(SF("0x%02x", 10), "0x0a"));

        n = snprint(buf, SIZEOF(buf),
                    "Now you can insert var" V(a) V(b) "s in situ:\n"
                    V(c) " divided by " V(d) " equals " V(c/d) "\n");
        assert(n == strlen2("Now you can insert variables in situ:\n"
                            "1 divided by 8 equals 0.125\n"));

        assert(strequal(buf, "Now you can insert variables in situ:\n"
                            "1 divided by 8 equals 0.125\n"));

        n = snprint(buf, SIZEOF(buf),
                    "This is " W(e) " It's " V(strlen(e)) " characters long\n");
        snprintf(expected, SIZEOF(expected),
                 "This is %s It's %lu characters long\n",
                 e, (ulong)strlen(e));
        assert(n == strlen2(expected));
        assert(strequal(buf, expected));

        n = snprint(buf, SIZEOF(buf),
                    "custom " VF("%04i", c) " " VF("%c", a) "\n");
        assert(n == strlen2("custom 0001 i\n"));
        assert(strequal(buf, "custom 0001 i\n"));

        n = snprint(small2, SIZEOF(small2), "prefix-" W(e));
        assert(n == (int)(strlen("prefix-") + strlen(e)));
        assert(strequal(small2, "prefix-"));

        fp = tmpfile();
        assert(fp);
        n = fprint(fp, "file ", V(c), " ", VF("%04i", c), "\n");
        assert(n == strlen2("file 1 0001\n"));
        rewind(fp);
        assert(fgets(buf, SIZEOF(buf), fp));
        assert(strequal(buf, "file 1 0001\n"));
        fclose(fp);

        n = print0("print ", V(a), " ", W(b), "\n");
        assert(n == strlen2("print i able\n"));
        {
            char buffer[16];
            assert((print0(V(c), "\n")
                    == snprintf(buffer, SIZEOF(buffer), "%d\n", c)));
        }
        print0("PRINTING a=", V(a), "; b=", V(b), "\n");
    }
}

#endif

#endif /* GENERIC_C */
