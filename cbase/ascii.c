// SPDX-License-Identifier: AGPL
// Copyright (c) 2026 Lucas Mior

#if !defined(ASCII_C)
#define ASCII_C

#if !defined(TESTING_ascii)
#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_ascii 1
#else
#define TESTING_ascii 0
#endif
#endif

#include "cbase.h"

bool32
is_ascii(uint8 c) {
    return c <= 0x7f;
}

bool32
is_cntrl(uint8 c) {
    return (c <= 0x1f) || (c == 0x7f);
}

bool32
is_blank(uint8 c) {
    return (c == ' ') || (c == '\t');
}

bool32
is_space(uint8 c) {
    return ((c >= '\t') && (c <= '\r')) || (c == ' ');
}

bool32
is_digit(uint8 c) {
    return (c >= '0') && (c <= '9');
}

bool32
is_upper(uint8 c) {
    return (c >= 'A') && (c <= 'Z');
}

bool32
is_lower(uint8 c) {
    return (c >= 'a') && (c <= 'z');
}

bool32
is_alpha(uint8 c) {
    return is_upper(c) || is_lower(c);
}

bool32
is_alnum(uint8 c) {
    return is_alpha(c) || is_digit(c);
}

bool32
is_xdigit(uint8 c) {
    return is_digit(c)
           || ((c >= 'A') && (c <= 'F'))
           || ((c >= 'a') && (c <= 'f'));
}

bool32
is_print(uint8 c) {
    return (c >= 0x20) && (c <= 0x7e);
}

bool32
is_graph(uint8 c) {
    return (c >= 0x21) && (c <= 0x7e);
}

bool32
is_punct(uint8 c) {
    return is_graph(c) && !is_alnum(c);
}

#if TESTING_ascii

#define CBASE_IMPLEMENT
#include "cbase.h"

static void
test_ascii_classifiers(void) {
    for (int32 i = 0; i < 256; i += 1) {
        uint8 c = (uint8)i;

        ASSERT(is_ascii(c) == (i <= 0x7f));
        ASSERT(is_cntrl(c) == ((i <= 0x1f) || (i == 0x7f)));
        ASSERT(is_blank(c) == ((i == ' ') || (i == '\t')));
        ASSERT(is_space(c)
               == (((i >= '\t') && (i <= '\r')) || (i == ' ')));
        ASSERT(is_digit(c) == ((i >= '0') && (i <= '9')));
        ASSERT(is_upper(c) == ((i >= 'A') && (i <= 'Z')));
        ASSERT(is_lower(c) == ((i >= 'a') && (i <= 'z')));
        ASSERT(is_alpha(c)
               == (((i >= 'A') && (i <= 'Z'))
                   || ((i >= 'a') && (i <= 'z'))));
        ASSERT(is_alnum(c)
               == (((i >= 'A') && (i <= 'Z'))
                   || ((i >= 'a') && (i <= 'z'))
                   || ((i >= '0') && (i <= '9'))));
        ASSERT(is_xdigit(c)
               == (((i >= '0') && (i <= '9'))
                   || ((i >= 'A') && (i <= 'F'))
                   || ((i >= 'a') && (i <= 'f'))));
        ASSERT(is_print(c) == ((i >= 0x20) && (i <= 0x7e)));
        ASSERT(is_graph(c) == ((i >= 0x21) && (i <= 0x7e)));
        ASSERT(is_punct(c)
               == (((i >= 0x21) && (i <= 0x2f))
                   || ((i >= 0x3a) && (i <= 0x40))
                   || ((i >= 0x5b) && (i <= 0x60))
                   || ((i >= 0x7b) && (i <= 0x7e))));
    }

    return;
}

int
main(void) {
    test_ascii_classifiers();
    exit(EXIT_SUCCESS);
}

#endif /* TESTING_ascii */

#endif /* ASCII_C */
