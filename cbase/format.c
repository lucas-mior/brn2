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

#if !defined(EOVERFLOW)
#define EOVERFLOW ERANGE
#endif

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

typedef struct FormatSink {
    char *buffer;
    int64 capacity;
    int64 written;
    int64 total;
    int32 status;
} FormatSink;

static int32
format_sink_init(FormatSink *sink, char *buffer, int64 capacity) {
    ASSERT(sink != NULL);

    if (capacity < 0) {
        return -EINVAL;
    }
    if (capacity > 0 && buffer == NULL) {
        return -EINVAL;
    }

    sink->buffer = buffer;
    sink->capacity = capacity;
    sink->written = 0;
    sink->total = 0;
    sink->status = 0;

    if (capacity > 0) {
        buffer[0] = '\0';
    }

    return 0;
}

static void
format_sink_add_total(FormatSink *sink, int64 len) {
    ASSERT(sink != NULL);
    ASSERT_NON_NEGATIVE(len);

    if (sink->status < 0) {
        return;
    }
    if (len > INT64_MAX - sink->total) {
        sink->status = -EOVERFLOW;
        return;
    }

    sink->total += len;
    return;
}

static void
format_sink_write(FormatSink *sink, char *data, int64 len) {
    int64 available;
    int64 copy_len;

    ASSERT(sink != NULL);
    ASSERT(data != NULL);
    ASSERT_NON_NEGATIVE(len);

    if (sink->status < 0) {
        return;
    }

    format_sink_add_total(sink, len);
    if (sink->status < 0) {
        return;
    }
    if (sink->capacity <= 0) {
        return;
    }

    ASSERT(sink->buffer != NULL);
    ASSERT_LESS(sink->written, sink->capacity);

    available = sink->capacity - 1 - sink->written;
    if (available <= 0) {
        return;
    }

    copy_len = MIN(len, available);
    memcpy(sink->buffer + sink->written, data, (size_t)copy_len);
    sink->written += copy_len;
    sink->buffer[sink->written] = '\0';
    return;
}

static void
format_sink_write_byte(FormatSink *sink, char byte) {
    format_sink_write(sink, &byte, 1);
    return;
}

static int32
format_sink_finish(FormatSink *sink) {
    ASSERT(sink != NULL);

    if (sink->status < 0) {
        return sink->status;
    }
    if (sink->total > INT32_MAX) {
        return -EOVERFLOW;
    }

    return (int32)sink->total;
}

static int32 UNUSED
format_vsnprintf_impl(char *buffer, int64 capacity, char *format,
                      va_list args) {
    FormatSink sink;
    char *literal;
    char *cursor;
    int32 status;

    (void)args;

    if (format == NULL) {
        return -EINVAL;
    }
    if ((status = format_sink_init(&sink, buffer, capacity)) < 0) {
        return status;
    }

    literal = format;
    cursor = format;
    while (*cursor != '\0') {
        if (*cursor != '%') {
            cursor += 1;
            continue;
        }

        format_sink_write(&sink, literal, cursor - literal);
        if (sink.status < 0) {
            return format_sink_finish(&sink);
        }

        cursor += 1;
        if (*cursor != '%') {
            return -EINVAL;
        }

        format_sink_write_byte(&sink, '%');
        cursor += 1;
        literal = cursor;
    }

    format_sink_write(&sink, literal, cursor - literal);
    return format_sink_finish(&sink);
}

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
format_float_copy(char *buffer, int64 capacity,
                  char *source, int32 source_len) {
    ASSERT(buffer != NULL);
    ASSERT_POSITIVE(capacity);
    ASSERT(source != NULL);
    ASSERT_NON_NEGATIVE(source_len);
    ASSERT_LESS(source_len, FORMAT_FLOAT_RYU_BUFFER_SIZE);

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

void
sb_float64(StrBuilder *str_builder, double value) {
    int32 len;

    sb_reserve(str_builder, FORMAT_FLOAT_RYU_BUFFER_SIZE);
    len = (int32)d2s_buffered_n(value, str_builder->data + str_builder->len);
    str_builder->len += len;
    str_builder->data[str_builder->len] = '\0';
    return;
}

void
sb_float64_fixed(StrBuilder *sb, double value, int32 precision) {
    int32 status;
    int32 len;

    if ((status = format_float_validate_precision(precision)) < 0) {
        error("Invalid float precision %d.\n", precision);
        fatal(EXIT_FAILURE);
    }

    sb_reserve(sb, FORMAT_FLOAT_RYU_BUFFER_SIZE);
    len = d2fixed_buffered_n(value, (uint32_t)precision, sb->data + sb->len);
    sb->len += len;
    sb->data[sb->len] = '\0';
    return;
}

#if TESTING_format
static int32
format_test_snprintf(char *buffer, int64 capacity, char *format, ...) {
    va_list args;
    int32 len;

    va_start(args, format);
    len = format_vsnprintf_impl(buffer, capacity, format, args);
    va_end(args);
    return len;
}

static void
test_format_sink_capacity(char *format, char *expected) {
    char buffer[128];
    int32 expected_len;

    expected_len = strlen32(expected);
    ASSERT_LESS(expected_len + 2, SIZEOF(buffer));

    for (int32 capacity = 0; capacity <= expected_len + 2; capacity += 1) {
        int32 copied;
        int32 len;

        memset(buffer, 0x7f, SIZEOF(buffer));
        len = format_test_snprintf(buffer, capacity, format);
        ASSERT_EQUAL(len, expected_len);

        if (capacity == 0) {
            ASSERT_EQUAL(buffer[0], (char)0x7f);
            continue;
        }

        copied = MIN(expected_len, capacity - 1);
        ASSERT_EQUAL(buffer, copied, expected, copied);
        ASSERT_EQUAL(buffer[copied], '\0');
        ASSERT_EQUAL(buffer[capacity], (char)0x7f);
    }

    return;
}

static void
test_format_sink_validation(void) {
    char buffer[8];
    FormatSink sink;

    ASSERT_EQUAL(format_test_snprintf(NULL, 0, "abc"), 3);
    ASSERT_EQUAL(format_test_snprintf(NULL, 1, "abc"), -EINVAL);
    ASSERT_EQUAL(format_test_snprintf(buffer, -1, "abc"), -EINVAL);
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), NULL), -EINVAL);

    memset(buffer, 0x7f, SIZEOF(buffer));
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%d"), -EINVAL);
    ASSERT_EQUAL(buffer[0], '\0');
    ASSERT_EQUAL(buffer[1], (char)0x7f);

    memset(buffer, 0x7f, SIZEOF(buffer));
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%"), -EINVAL);
    ASSERT_EQUAL(buffer[0], '\0');
    ASSERT_EQUAL(buffer[1], (char)0x7f);

    ASSERT_EQUAL(format_sink_init(&sink, buffer, SIZEOF(buffer)), 0);
    sink.total = (int64)INT32_MAX + 1;
    ASSERT_EQUAL(format_sink_finish(&sink), -EOVERFLOW);

    return;
}

static void
test_format_float32_shortest(float value, char *expected) {
    char buffer[FORMAT_FLOAT_RYU_BUFFER_SIZE];
    int32 len;

    len = format_float32_shortest(buffer, SIZEOF(buffer), value);
    ASSERT_EQUAL(len, strlen32(expected));
    ASSERT_EQUAL(buffer, expected);

    return;
}

static void
test_format_float64_shortest(double value, char *expected) {
    char buffer[FORMAT_FLOAT_RYU_BUFFER_SIZE];
    int32 len;

    len = format_float64_shortest(buffer, SIZEOF(buffer), value);
    ASSERT_EQUAL(len, strlen32(expected));
    ASSERT_EQUAL(buffer, expected);

    return;
}

static void
test_format_float64_fixed(double value, int32 precision, char *expected) {
    char buffer[FORMAT_FLOAT_RYU_BUFFER_SIZE];
    int32 len;

    len = format_float64_fixed(buffer, SIZEOF(buffer), value, precision);
    ASSERT_EQUAL(len, strlen32(expected));
    ASSERT_EQUAL(buffer, expected);

    return;
}

static void
test_format_float64_scientific(double value, int32 precision, char *expected) {
    char buffer[FORMAT_FLOAT_RYU_BUFFER_SIZE];
    int32 len;

    len = format_float64_scientific(buffer, SIZEOF(buffer), value, precision);
    ASSERT_EQUAL(len, strlen32(expected));
    ASSERT_EQUAL(buffer, expected);

    return;
}

static uint32
test_format_float32_bits(float value) {
    uint32 bits;

    memcpy(&bits, &value, SIZEOF(bits));
    return bits;
}

static uint64
test_format_float64_bits(double value) {
    uint64 bits;

    memcpy(&bits, &value, SIZEOF(bits));
    return bits;
}

static void
test_format_float32_round_trip(float value) {
    char buffer[FORMAT_FLOAT_RYU_BUFFER_SIZE];
    char *end;
    float parsed;
    int32 len;

    len = format_float32_shortest(buffer, SIZEOF(buffer), value);
    ASSERT_POSITIVE(len);

    end = NULL;
    parsed = strtof(buffer, &end);
    ASSERT(end == buffer + len);
    ASSERT_EQUAL(test_format_float32_bits(parsed),
                 test_format_float32_bits(value));

    return;
}

static void
test_format_float64_round_trip(double value) {
    char buffer[FORMAT_FLOAT_RYU_BUFFER_SIZE];
    char *end;
    double parsed;
    int32 len;

    len = format_float64_shortest(buffer, SIZEOF(buffer), value);
    ASSERT_POSITIVE(len);

    end = NULL;
    parsed = strtod(buffer, &end);
    ASSERT(end == buffer + len);
    ASSERT_EQUAL(test_format_float64_bits(parsed),
                 test_format_float64_bits(value));

    return;
}

int
main(void) {
    char buffer[16];

    test_format_sink_capacity("", "");
    test_format_sink_capacity("abc", "abc");
    test_format_sink_capacity("%%", "%");
    test_format_sink_capacity("a%%b%%c", "a%b%c");
    test_format_sink_capacity("abc%%def", "abc%def");
    test_format_sink_validation();

    test_format_float64_shortest(0.0, "0E0");
    test_format_float64_shortest(-0.0, "-0E0");
    test_format_float64_shortest(1.0, "1E0");
    test_format_float64_shortest(0.1, "1E-1");
    test_format_float64_shortest(1234567.89, "1.23456789E6");
    test_format_float64_shortest(1e-7, "1E-7");

    test_format_float32_shortest(0.0f, "0E0");
    test_format_float32_shortest(-0.0f, "-0E0");
    test_format_float32_shortest(1.0f, "1E0");
    test_format_float32_shortest(0.1f, "1E-1");
    test_format_float32_shortest(1e-7f, "1E-7");

    test_format_float64_fixed(1.25, 2, "1.25");
    test_format_float64_fixed(1.2, 4, "1.2000");
    test_format_float64_fixed(-0.0, 3, "-0.000");

    test_format_float64_scientific(1234.0, 2, "1.23e+03");
    test_format_float64_scientific(0.00123, 3, "1.230e-03");

    {
        StrBuilder builder = {0};

        SB_APPEND(&builder, "x=");
        sb_float64(&builder, 0.1);
        SB_APPEND(&builder, " y=");
        sb_float64_fixed(&builder, 1.25, 2);
        ASSERT_EQUAL(builder.data, "x=1E-1 y=1.25");
        sb_free(&builder);
    }

    ASSERT_EQUAL(format_float64_shortest(NULL, 64, 1.0), -EINVAL);
    ASSERT_EQUAL(format_float64_shortest(buffer, 0, 1.0), -EINVAL);
    ASSERT_EQUAL(format_float64_fixed(buffer, SIZEOF(buffer), 1.0, -1),
                 -EINVAL);
    ASSERT_EQUAL(format_float64_fixed(buffer, 4, 1.25, 2), -ENOSPC);
    ASSERT_EQUAL(format_float64_fixed(buffer, SIZEOF(buffer), 1.0,
                                      FORMAT_FLOAT_MAX_PRECISION + 1),
                 -ERANGE);

    test_format_float64_round_trip(0.1);
    test_format_float32_round_trip(0.1f);

    exit(EXIT_SUCCESS);
}
#endif

#endif /* FORMAT_C */
