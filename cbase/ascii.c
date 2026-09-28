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

#if TESTING_ascii

#define CBASE_IMPLEMENT
#include "cbase.h"
int main(void) {
    exit(EXIT_SUCCESS);
}

#endif /* TESTING_ascii */

#endif /* ASCII_C */
