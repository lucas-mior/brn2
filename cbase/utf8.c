#if !defined(UTF8_C)
#define UTF8_C

#include "util.c"
#define UTF_INVALID 0xFFFD

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_utf8 1
#elif !defined(TESTING_utf8)
#define TESTING_utf8 0
#endif

static uchar utf8_byte[] = {0x80, 0, 0xC0, 0xE0, 0xF0};
static uchar utf8_mask[] = {0xC0, 0x80, 0xE0, 0xF0, 0xF8};
static uint32 utf8_min[] = {0, 0, 0x80, 0x800, 0x10000};
static uint32 utf8_max[] = {0x10FFFF, 0x7F, 0x7FF, 0xFFFF, 0x10FFFF};

static uint32
utf8_decode_byte(char c, int64 *i) {
    for (*i = 0; *i < LENGTH(utf8_mask); ++(*i)) {
        if (((uchar)c & utf8_mask[*i]) == utf8_byte[*i]) {
            return (uchar)c & ~utf8_mask[*i];
        }
    }

    return 0;
}

static char
utf8_encode_byte(uint32 u, int64 i) {
    return (char)(utf8_byte[i] | (u & (uint32)~utf8_mask[i]));
}

static int64
utf8_validate(uint32 *u, int64 i) {
    if (!BETWEEN(*u, utf8_min[i], utf8_max[i]) || BETWEEN(*u, 0xD800, 0xDFFF)) {
        *u = UTF_INVALID;
    }
    for (i = 1; *u > utf8_max[i]; i += 1)
        ;

    return i;
}

static int64
utf8_decode(char *c, uint32 *u, int64 clen) {
    int64 len;
    int64 type;
    uint32 rune_decoded;

    *u = UTF_INVALID;
    if (!clen) {
        return 0;
    }
    rune_decoded = utf8_decode_byte(c[0], &len);
    if (!BETWEEN(len, 1, 4)) {
        return 1;
    }
    {
        int64 j = 1;
        for (int64 i = 1; i < clen && j < len; i += 1, j += 1) {
            rune_decoded = (rune_decoded << 6) | utf8_decode_byte(c[i], &type);
            if (type != 0) {
                return j;
            }
        }
        if (j < len) {
            return 0;
        }
    }
    *u = rune_decoded;
    utf8_validate(u, len);

    return len;
}

static int64
utf8_encode(uint32 u, char *c) {
    int64 len;

    len = utf8_validate(&u, 0);
    if (len > 4) {
        return 0;
    }

    for (int64 i = len - 1; i != 0; --i) {
        c[i] = utf8_encode_byte(u, 0);
        u >>= 6;
    }
    c[0] = utf8_encode_byte(u, len);

    return len;
}

#if TESTING_utf8

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "assert.c"

int
main(void) {
    {
        char buf[5];
        uint32 u;
        int64 len;

        /* Test 1-byte ASCII */
        len = utf8_encode(0x41, buf);
        ASSERT_EQUAL(len, 1);
        ASSERT_EQUAL(buf[0], 'A');
        len = utf8_decode(buf, &u, 1);
        ASSERT_EQUAL(len, 1);
        ASSERT_EQUAL(u, 0x41);

        /* Test 2-byte (e.g. U+00F1 n with tilde) */
        len = utf8_encode(0xF1, buf);
        ASSERT_EQUAL(len, 2);
        len = utf8_decode(buf, &u, 2);
        ASSERT_EQUAL(len, 2);
        ASSERT_EQUAL(u, 0xF1);

        /* Test 3-byte (e.g. U+20AC Euro sign) */
        len = utf8_encode(0x20AC, buf);
        ASSERT_EQUAL(len, 3);
        len = utf8_decode(buf, &u, 3);
        ASSERT_EQUAL(len, 3);
        ASSERT_EQUAL(u, 0x20AC);

        /* Test 4-byte (e.g. U+1F60A Smiling Face) */
        len = utf8_encode(0x1F60A, buf);
        ASSERT_EQUAL(len, 4);
        len = utf8_decode(buf, &u, 4);
        ASSERT_EQUAL(len, 4);
        ASSERT_EQUAL(u, 0x1F60A);
    }

    /* String Traversal Test with Multi-codepoint Emoji */
    {
        /* 
         * String contains:
         * 'A'      (U+0041)
         * 'ñ'      (U+00F1)
         * '€'      (U+20AC)
         * '😊'     (U+1F60A)
         * '🇩🇪'     (U+1F1E9 + U+1F1EA)  <-- Fixed 'D'
         */
        char *test_str = "Añ€😊🇩🇪";
        uint32 expected[] = {
            0x41, 
            0xF1, 
            0x20AC, 
            0x1F60A, 
            0x1F1E9, // Regional Indicator D
            0x1F1EA  // Regional Indicator E
        };
        int64 str_len = (int64)strlen(test_str);
        int64 consumed_total = 0;
        int64 expected_idx = 0;

        while (consumed_total < str_len) {
            uint32 u;
            int64 len;

            len = utf8_decode(test_str + consumed_total, &u, str_len - consumed_total);
            
            ASSERT_EQUAL(u, expected[expected_idx]);
            
            consumed_total += len;
            expected_idx += 1;
        }
        
        ASSERT_EQUAL(expected_idx, 6);
        ASSERT_EQUAL(consumed_total, str_len);
    }

    {
        uint32 u;
        int64 len;

        /* Valid character */
        u = 0x41;
        len = utf8_validate(&u, 0);
        ASSERT_EQUAL(len, 1);
        ASSERT_EQUAL(u, 0x41);

        /* Invalid surrogate half */
        u = 0xD800;
        len = utf8_validate(&u, 3);
        ASSERT_EQUAL(u, UTF_INVALID);
    }

    exit(EXIT_SUCCESS);
}

#endif /* TESTING_utf8 */

#endif /* UTF8_C */
