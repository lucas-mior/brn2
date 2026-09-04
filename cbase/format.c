// SPDX-License-Identifier: AGPL
// Copyright (c) 2026 Lucas Mior

#if !defined(FORMAT_C)
#define FORMAT_C

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_format 1
#elif !defined(TESTING_format)
#define TESTING_format 0
#endif

#include "cbase.h"

#if TESTING_format && !defined(CBASE_IMPLEMENT)
#define CBASE_IMPLEMENT
#include "cbase.h"
#endif

#include "ryu.h"

enum {
    FORMAT_FLOAT_RYU_BUFFER_SIZE = 2000,
    FORMAT_FLOAT_MAX_PRECISION = 1024,
    FORMAT_FLOAT_MAX_FIXED_PREFIX = 312,
    FORMAT_FLOAT_MAX_EXP_PREFIX = 8,
};

_Static_assert(FORMAT_FLOAT_MAX_FIXED_PREFIX
               + FORMAT_FLOAT_MAX_PRECISION < FORMAT_FLOAT_RYU_BUFFER_SIZE,
               "format fixed temporary buffer is too small");
_Static_assert(FORMAT_FLOAT_MAX_EXP_PREFIX
               + FORMAT_FLOAT_MAX_PRECISION < FORMAT_FLOAT_RYU_BUFFER_SIZE,
               "format scientific temporary buffer is too small");

static int32
format_float_validate_buffer(char *buffer, int64 capacity) {
    if (buffer == NULL) {
        return -EINVAL;
    }
    if (capacity <= 0) {
        return -EINVAL;
    }

    return 0;
}

static int32
format_float_validate_precision(int32 precision) {
    if (precision < 0) {
        return -EINVAL;
    }
    if (precision > FORMAT_FLOAT_MAX_PRECISION) {
        return -ERANGE;
    }

    return 0;
}

static int32
format_float_copy(char *buffer, int64 capacity, char *source,
                  int32 source_len) {
    ASSERT(buffer != NULL);
    ASSERT(capacity > 0);
    ASSERT(source != NULL);
    ASSERT(source_len >= 0);
    ASSERT(source_len < FORMAT_FLOAT_RYU_BUFFER_SIZE);

    if ((int64)source_len >= capacity) {
        return -ENOSPC;
    }

    memcpy(buffer, source, (size_t)source_len);
    buffer[source_len] = '\0';
    return source_len;
}

int32
format_float32_shortest(char *buffer, int64 capacity, float value) {
    int32 status;
    int32 len;
    char temp[FORMAT_FLOAT_RYU_BUFFER_SIZE];

    if ((status = format_float_validate_buffer(buffer, capacity)) < 0) {
        return status;
    }

    len = (int32)f2s_buffered_n(value, temp);
    return format_float_copy(buffer, capacity, temp, len);
}

int32
format_float64_shortest(char *buffer, int64 capacity, double value) {
    int32 status;
    int32 len;
    char temp[FORMAT_FLOAT_RYU_BUFFER_SIZE];

    if ((status = format_float_validate_buffer(buffer, capacity)) < 0) {
        return status;
    }

    len = (int32)d2s_buffered_n(value, temp);
    return format_float_copy(buffer, capacity, temp, len);
}

int32
format_float64_fixed(char *buffer, int64 capacity, double value,
                     int32 precision) {
    int32 status;
    int32 len;
    char temp[FORMAT_FLOAT_RYU_BUFFER_SIZE];

    if ((status = format_float_validate_buffer(buffer, capacity)) < 0) {
        return status;
    }
    if ((status = format_float_validate_precision(precision)) < 0) {
        return status;
    }

    len = (int32)d2fixed_buffered_n(value, (uint32_t)precision, temp);
    return format_float_copy(buffer, capacity, temp, len);
}

int32
format_float64_scientific(char *buffer, int64 capacity, double value,
                          int32 precision) {
    int32 status;
    int32 len;
    char temp[FORMAT_FLOAT_RYU_BUFFER_SIZE];

    if ((status = format_float_validate_buffer(buffer, capacity)) < 0) {
        return status;
    }
    if ((status = format_float_validate_precision(precision)) < 0) {
        return status;
    }

    len = (int32)d2exp_buffered_n(value, (uint32_t)precision, temp);
    return format_float_copy(buffer, capacity, temp, len);
}

#if TESTING_format
int
main(void) {
    exit(EXIT_SUCCESS);
}
#endif

#endif /* FORMAT_C */
