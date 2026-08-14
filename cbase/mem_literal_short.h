// SPDX-License-Identifier: AGPL
// Copyright (c) 2026 Lucas Mior

#include "base_macros.h"
#include "primitives.h"

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
  #define MEM_LITERAL_SHORT_N 2
#endif

#if !defined(CBASE_H)
#error "include cbase.h before mem_literal_short.h"
#endif

#if !defined(MEM_LITERAL_SHORT_N)
#error "MEM_LITERAL_SHORT_N is undefined"
#endif

#if (MEM_LITERAL_SHORT_N < 2) || (MEM_LITERAL_SHORT_N > 8)
#error "MEM_LITERAL_SHORT_N must be between 2 and 8"
#endif

#define MEM_LITERAL_SHORT_FUNCTION \
    CAT(mem_literal_short_, MEM_LITERAL_SHORT_N)

#if MEM_LITERAL_SHORT_N == 2
#define MEM_LITERAL_SHORT_MATCH(P, L) ((P)[1] == (L)[1])
#elif MEM_LITERAL_SHORT_N == 3
#define MEM_LITERAL_SHORT_MATCH(P, L) \
    (((P)[1] == (L)[1]) && ((P)[2] == (L)[2]))
#elif MEM_LITERAL_SHORT_N == 4
#define MEM_LITERAL_SHORT_MATCH(P, L) \
    (((P)[1] == (L)[1]) && ((P)[2] == (L)[2]) \
     && ((P)[3] == (L)[3]))
#elif MEM_LITERAL_SHORT_N == 5
#define MEM_LITERAL_SHORT_MATCH(P, L) \
    (((P)[1] == (L)[1]) && ((P)[2] == (L)[2]) \
     && ((P)[3] == (L)[3]) && ((P)[4] == (L)[4]))
#elif MEM_LITERAL_SHORT_N == 6
#define MEM_LITERAL_SHORT_MATCH(P, L) \
    (((P)[1] == (L)[1]) && ((P)[2] == (L)[2]) \
     && ((P)[3] == (L)[3]) && ((P)[4] == (L)[4]) \
     && ((P)[5] == (L)[5]))
#elif MEM_LITERAL_SHORT_N == 7
#define MEM_LITERAL_SHORT_MATCH(P, L) \
    (((P)[1] == (L)[1]) && ((P)[2] == (L)[2]) \
     && ((P)[3] == (L)[3]) && ((P)[4] == (L)[4]) \
     && ((P)[5] == (L)[5]) && ((P)[6] == (L)[6]))
#elif MEM_LITERAL_SHORT_N == 8
#define MEM_LITERAL_SHORT_MATCH(P, L) \
    (((P)[1] == (L)[1]) && ((P)[2] == (L)[2]) \
     && ((P)[3] == (L)[3]) && ((P)[4] == (L)[4]) \
     && ((P)[5] == (L)[5]) && ((P)[6] == (L)[6]) \
     && ((P)[7] == (L)[7]))
#endif

INLINE UNUSED char *
MEM_LITERAL_SHORT_FUNCTION(char *haystack, int64 haystack_len,
                           char *literal) {
    char *candidate;
    char *end;

    if (haystack_len < MEM_LITERAL_SHORT_N) {
        return NULL;
    }
    if ((haystack == NULL) || (literal == NULL)) {
        return NULL;
    }

    candidate = haystack;
    end = haystack + haystack_len - MEM_LITERAL_SHORT_N + 1;
    while (candidate < end) {
        char *p = memchr(candidate, literal[0], end - candidate);

        if (p == NULL) {
            return NULL;
        }
        if (MEM_LITERAL_SHORT_MATCH(p, literal)) {
            return p;
        }
        candidate = p + 1;
    }

    return NULL;
}

#undef MEM_LITERAL_SHORT_MATCH
#undef MEM_LITERAL_SHORT_FUNCTION
#undef MEM_LITERAL_SHORT_N
