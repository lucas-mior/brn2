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

enum {
    FORMAT_FLAG_LEFT = 1 << 0,
    FORMAT_FLAG_SIGN = 1 << 1,
    FORMAT_FLAG_SPACE = 1 << 2,
    FORMAT_FLAG_ALTERNATE = 1 << 3,
    FORMAT_FLAG_ZERO = 1 << 4,
};

enum FormatWidthKind {
    FORMAT_WIDTH_NONE,
    FORMAT_WIDTH_LITERAL,
    FORMAT_WIDTH_ARG,
};

enum FormatPrecisionKind {
    FORMAT_PRECISION_NONE,
    FORMAT_PRECISION_LITERAL,
    FORMAT_PRECISION_ARG,
};

enum FormatLength {
    FORMAT_LENGTH_NONE,
    FORMAT_LENGTH_HH,
    FORMAT_LENGTH_H,
    FORMAT_LENGTH_L,
    FORMAT_LENGTH_LL,
    FORMAT_LENGTH_BIG_L,
    FORMAT_LENGTH_W8,
    FORMAT_LENGTH_W16,
    FORMAT_LENGTH_W32,
    FORMAT_LENGTH_W64,
};

typedef struct FormatSpec {
    int32 flags;
    int64 width;
    int64 precision;
    enum FormatWidthKind width_kind;
    enum FormatPrecisionKind precision_kind;
    enum FormatLength length;
    char conversion;
} FormatSpec;

static bool
format_is_digit(char byte) {
    return byte >= '0' && byte <= '9';
}

static int32
format_digit_value(char byte) {
    ASSERT(format_is_digit(byte));
    return byte - '0';
}

static int32
format_parse_uint(char **cursor, int64 *value) {
    char *scan;
    int64 result;
    bool found_digit;

    ASSERT(cursor != NULL);
    ASSERT(*cursor != NULL);
    ASSERT(value != NULL);

    scan = *cursor;
    result = 0;
    found_digit = false;
    while (format_is_digit(*scan)) {
        int32 digit;

        digit = format_digit_value(*scan);
        if (result > (INT64_MAX - digit)/10) {
            return -EOVERFLOW;
        }

        result = result*10 + digit;
        scan += 1;
        found_digit = true;
    }

    if (!found_digit) {
        return -EINVAL;
    }

    *cursor = scan;
    *value = result;
    return 0;
}

static int32
format_reject_positional_prefix(char *cursor) {
    char *scan;

    ASSERT(cursor != NULL);

    if (!format_is_digit(*cursor)) {
        return 0;
    }

    scan = cursor;
    while (format_is_digit(*scan)) {
        scan += 1;
    }
    if (*scan == '$') {
        return -EINVAL;
    }

    return 0;
}

static int32
format_reject_star_positional(char *cursor) {
    char *scan;

    ASSERT(cursor != NULL);

    if (!format_is_digit(*cursor)) {
        return 0;
    }

    scan = cursor;
    while (format_is_digit(*scan)) {
        scan += 1;
    }
    if (*scan == '$') {
        return -EINVAL;
    }

    return 0;
}

static void
format_parse_flags(char **cursor, FormatSpec *spec) {
    ASSERT(cursor != NULL);
    ASSERT(*cursor != NULL);
    ASSERT(spec != NULL);

    for (;;) {
        if (**cursor == '-') {
            spec->flags |= FORMAT_FLAG_LEFT;
        } else if (**cursor == '+') {
            spec->flags |= FORMAT_FLAG_SIGN;
        } else if (**cursor == ' ') {
            spec->flags |= FORMAT_FLAG_SPACE;
        } else if (**cursor == '#') {
            spec->flags |= FORMAT_FLAG_ALTERNATE;
        } else if (**cursor == '0') {
            spec->flags |= FORMAT_FLAG_ZERO;
        } else {
            return;
        }

        *cursor += 1;
    }
}

static int32
format_parse_width(char **cursor, FormatSpec *spec) {
    int32 status;

    ASSERT(cursor != NULL);
    ASSERT(*cursor != NULL);
    ASSERT(spec != NULL);

    if (**cursor == '*') {
        *cursor += 1;
        if ((status = format_reject_star_positional(*cursor)) < 0) {
            return status;
        }
        spec->width_kind = FORMAT_WIDTH_ARG;
        return 0;
    }

    if (format_is_digit(**cursor)) {
        spec->width_kind = FORMAT_WIDTH_LITERAL;
        if ((status = format_parse_uint(cursor, &spec->width)) < 0) {
            return status;
        }
    }

    return 0;
}

static int32
format_parse_precision(char **cursor, FormatSpec *spec) {
    int32 status;

    ASSERT(cursor != NULL);
    ASSERT(*cursor != NULL);
    ASSERT(spec != NULL);

    if (**cursor != '.') {
        return 0;
    }

    *cursor += 1;
    if (**cursor == '*') {
        *cursor += 1;
        if ((status = format_reject_star_positional(*cursor)) < 0) {
            return status;
        }
        spec->precision_kind = FORMAT_PRECISION_ARG;
        return 0;
    }

    spec->precision_kind = FORMAT_PRECISION_LITERAL;
    if (format_is_digit(**cursor)) {
        char *scan;

        scan = *cursor;
        while (format_is_digit(*scan)) {
            scan += 1;
        }
        if (*scan == '$') {
            return -EINVAL;
        }

        if ((status = format_parse_uint(cursor, &spec->precision)) < 0) {
            return status;
        }
    }

    return 0;
}

static int32
format_parse_w_length(char **cursor, FormatSpec *spec) {
    int64 width;
    int32 status;

    ASSERT(cursor != NULL);
    ASSERT(*cursor != NULL);
    ASSERT(spec != NULL);
    ASSERT(**cursor == 'w');

    *cursor += 1;
    if (**cursor == 'f') {
        return -EINVAL;
    }
    if (!format_is_digit(**cursor)) {
        return -EINVAL;
    }

    if ((status = format_parse_uint(cursor, &width)) < 0) {
        return status;
    }

    if (width == 8) {
        spec->length = FORMAT_LENGTH_W8;
    } else if (width == 16) {
        spec->length = FORMAT_LENGTH_W16;
    } else if (width == 32) {
        spec->length = FORMAT_LENGTH_W32;
    } else if (width == 64) {
        spec->length = FORMAT_LENGTH_W64;
    } else {
        return -EINVAL;
    }

    return 0;
}

static int32
format_parse_length(char **cursor, FormatSpec *spec) {
    ASSERT(cursor != NULL);
    ASSERT(*cursor != NULL);
    ASSERT(spec != NULL);

    if ((*cursor)[0] == 'h' && (*cursor)[1] == 'h') {
        spec->length = FORMAT_LENGTH_HH;
        *cursor += 2;
    } else if (**cursor == 'h') {
        spec->length = FORMAT_LENGTH_H;
        *cursor += 1;
    } else if ((*cursor)[0] == 'l' && (*cursor)[1] == 'l') {
        spec->length = FORMAT_LENGTH_LL;
        *cursor += 2;
    } else if (**cursor == 'l') {
        spec->length = FORMAT_LENGTH_L;
        *cursor += 1;
    } else if (**cursor == 'L') {
        spec->length = FORMAT_LENGTH_BIG_L;
        *cursor += 1;
    } else if (**cursor == 'w') {
        return format_parse_w_length(cursor, spec);
    } else if (**cursor == 'j' || **cursor == 'z' || **cursor == 't') {
        return -EINVAL;
    }

    return 0;
}

static bool
format_is_integer_conversion(char conversion) {
    return conversion == 'd'
           || conversion == 'i'
           || conversion == 'u'
           || conversion == 'o'
           || conversion == 'x'
           || conversion == 'X'
           || conversion == 'b'
           || conversion == 'B';
}

static bool
format_is_float_conversion(char conversion) {
    return conversion == 'f'
           || conversion == 'F'
           || conversion == 'e'
           || conversion == 'E'
           || conversion == 'g'
           || conversion == 'G'
           || conversion == 'a'
           || conversion == 'A';
}

static bool
format_length_is_integer(enum FormatLength length) {
    return length == FORMAT_LENGTH_NONE
           || length == FORMAT_LENGTH_HH
           || length == FORMAT_LENGTH_H
           || length == FORMAT_LENGTH_LL
           || length == FORMAT_LENGTH_W8
           || length == FORMAT_LENGTH_W16
           || length == FORMAT_LENGTH_W32
           || length == FORMAT_LENGTH_W64;
}

static bool
format_length_is_char_string(enum FormatLength length) {
    return length == FORMAT_LENGTH_NONE || length == FORMAT_LENGTH_L;
}

static bool
format_length_is_float(enum FormatLength length) {
    return length == FORMAT_LENGTH_NONE || length == FORMAT_LENGTH_BIG_L;
}

static bool
format_has_width(FormatSpec *spec) {
    ASSERT(spec != NULL);
    return spec->width_kind != FORMAT_WIDTH_NONE;
}

static bool
format_has_precision(FormatSpec *spec) {
    ASSERT(spec != NULL);
    return spec->precision_kind != FORMAT_PRECISION_NONE;
}

static int32
format_validate_flags(FormatSpec *spec, int32 allowed_flags) {
    ASSERT(spec != NULL);

    if ((spec->flags & ~allowed_flags) != 0) {
        return -EINVAL;
    }

    return 0;
}

static int32
format_validate_spec(FormatSpec *spec) {
    ASSERT(spec != NULL);

    if (format_is_integer_conversion(spec->conversion)) {
        if (!format_length_is_integer(spec->length)) {
            return -EINVAL;
        }
        return 0;
    }

    if (format_is_float_conversion(spec->conversion)) {
        if (!format_length_is_float(spec->length)) {
            return -EINVAL;
        }
        return 0;
    }

    if (spec->conversion == 'c') {
        if (!format_length_is_char_string(spec->length)) {
            return -EINVAL;
        }
        if (format_has_precision(spec)) {
            return -EINVAL;
        }
        return format_validate_flags(spec, FORMAT_FLAG_LEFT);
    }

    if (spec->conversion == 's') {
        if (!format_length_is_char_string(spec->length)) {
            return -EINVAL;
        }
        return format_validate_flags(spec, FORMAT_FLAG_LEFT);
    }

    if (spec->conversion == 'p') {
        if (spec->length != FORMAT_LENGTH_NONE) {
            return -EINVAL;
        }
        if (format_has_precision(spec)) {
            return -EINVAL;
        }
        return format_validate_flags(spec, FORMAT_FLAG_LEFT);
    }

    if (spec->conversion == 'n') {
        if (!format_length_is_integer(spec->length)) {
            return -EINVAL;
        }
        if (spec->flags != 0 || format_has_width(spec)
            || format_has_precision(spec)) {
            return -EINVAL;
        }
        return 0;
    }

    if (spec->conversion == '%') {
        if (spec->length != FORMAT_LENGTH_NONE) {
            return -EINVAL;
        }
        if (spec->flags != 0 || format_has_width(spec)
            || format_has_precision(spec)) {
            return -EINVAL;
        }
        return 0;
    }

    return -EINVAL;
}

static int32
format_parse_spec(char *cursor, char **next, FormatSpec *spec) {
    int32 status;

    ASSERT(cursor != NULL);
    ASSERT(next != NULL);
    ASSERT(spec != NULL);

    memset(spec, 0, SIZEOF(*spec));

    if (*cursor == '\0') {
        return -EINVAL;
    }
    if ((status = format_reject_positional_prefix(cursor)) < 0) {
        return status;
    }

    format_parse_flags(&cursor, spec);
    if ((status = format_parse_width(&cursor, spec)) < 0) {
        return status;
    }
    if ((status = format_parse_precision(&cursor, spec)) < 0) {
        return status;
    }
    if ((status = format_parse_length(&cursor, spec)) < 0) {
        return status;
    }

    if (*cursor == '\0') {
        return -EINVAL;
    }

    spec->conversion = *cursor;
    cursor += 1;
    if ((status = format_validate_spec(spec)) < 0) {
        return status;
    }

    *next = cursor;
    return 0;
}

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
        FormatSpec spec;

        if (*cursor != '%') {
            cursor += 1;
            continue;
        }

        format_sink_write(&sink, literal, cursor - literal);
        if (sink.status < 0) {
            return format_sink_finish(&sink);
        }

        cursor += 1;
        if ((status = format_parse_spec(cursor, &cursor, &spec)) < 0) {
            return status;
        }
        if (spec.conversion != '%') {
            return -ENOSYS;
        }

        format_sink_write_byte(&sink, '%');
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

static int32
format_test_validate(char *format) {
    char *cursor;

    if (format == NULL) {
        return -EINVAL;
    }

    cursor = format;
    while (*cursor != '\0') {
        FormatSpec spec;
        int32 status;

        if (*cursor != '%') {
            cursor += 1;
            continue;
        }

        cursor += 1;
        if ((status = format_parse_spec(cursor, &cursor, &spec)) < 0) {
            return status;
        }
    }

    return 0;
}

static FormatSpec
format_test_parse_one(char *format) {
    FormatSpec spec;
    char *next;
    int32 status;

    ASSERT(format != NULL);
    ASSERT_EQUAL(format[0], '%');

    next = NULL;
    status = format_parse_spec(format + 1, &next, &spec);
    ASSERT_EQUAL(status, 0);
    ASSERT_EQUAL(*next, '\0');
    return spec;
}

static void
test_format_parser_valid_specs(void) {
    FormatSpec spec;

    spec = format_test_parse_one("%%");
    ASSERT_EQUAL(spec.conversion, '%');
    ASSERT_EQUAL(spec.flags, 0);
    ASSERT_EQUAL(spec.length, FORMAT_LENGTH_NONE);

    spec = format_test_parse_one("%08.3d");
    ASSERT_EQUAL(spec.conversion, 'd');
    ASSERT_EQUAL(spec.flags, FORMAT_FLAG_ZERO);
    ASSERT_EQUAL(spec.width_kind, FORMAT_WIDTH_LITERAL);
    ASSERT_EQUAL(spec.width, 8);
    ASSERT_EQUAL(spec.precision_kind, FORMAT_PRECISION_LITERAL);
    ASSERT_EQUAL(spec.precision, 3);
    ASSERT_EQUAL(spec.length, FORMAT_LENGTH_NONE);

    spec = format_test_parse_one("%*.*f");
    ASSERT_EQUAL(spec.conversion, 'f');
    ASSERT_EQUAL(spec.width_kind, FORMAT_WIDTH_ARG);
    ASSERT_EQUAL(spec.precision_kind, FORMAT_PRECISION_ARG);
    ASSERT_EQUAL(spec.length, FORMAT_LENGTH_NONE);

    spec = format_test_parse_one("%-+ #0w32x");
    ASSERT_EQUAL(spec.conversion, 'x');
    ASSERT_EQUAL(spec.flags, FORMAT_FLAG_LEFT
                             |FORMAT_FLAG_SIGN
                             |FORMAT_FLAG_SPACE
                             |FORMAT_FLAG_ALTERNATE
                             |FORMAT_FLAG_ZERO);
    ASSERT_EQUAL(spec.length, FORMAT_LENGTH_W32);

    spec = format_test_parse_one("%hhd");
    ASSERT_EQUAL(spec.conversion, 'd');
    ASSERT_EQUAL(spec.length, FORMAT_LENGTH_HH);

    spec = format_test_parse_one("%llu");
    ASSERT_EQUAL(spec.conversion, 'u');
    ASSERT_EQUAL(spec.length, FORMAT_LENGTH_LL);

    spec = format_test_parse_one("%w64B");
    ASSERT_EQUAL(spec.conversion, 'B');
    ASSERT_EQUAL(spec.length, FORMAT_LENGTH_W64);

    spec = format_test_parse_one("%lc");
    ASSERT_EQUAL(spec.conversion, 'c');
    ASSERT_EQUAL(spec.length, FORMAT_LENGTH_L);

    spec = format_test_parse_one("%.5ls");
    ASSERT_EQUAL(spec.conversion, 's');
    ASSERT_EQUAL(spec.length, FORMAT_LENGTH_L);
    ASSERT_EQUAL(spec.precision_kind, FORMAT_PRECISION_LITERAL);
    ASSERT_EQUAL(spec.precision, 5);

    spec = format_test_parse_one("%La");
    ASSERT_EQUAL(spec.conversion, 'a');
    ASSERT_EQUAL(spec.length, FORMAT_LENGTH_BIG_L);

    spec = format_test_parse_one("%w16n");
    ASSERT_EQUAL(spec.conversion, 'n');
    ASSERT_EQUAL(spec.length, FORMAT_LENGTH_W16);

    ASSERT_EQUAL(format_test_validate("a %% b %08d %*.*s"), 0);
    return;
}

static void
test_format_parser_invalid_specs(void) {
    ASSERT_EQUAL(format_test_validate("%"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%2$d"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%*2$d"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%.*2$s"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%m"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%q"), -EINVAL);

    ASSERT_EQUAL(format_test_validate("%ld"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%zd"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%td"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%jd"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%wfd"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%wf32d"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%w24d"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%wd"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%Lx"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%lf"), -EINVAL);

    ASSERT_EQUAL(format_test_validate("%+s"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%05s"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%.2c"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%#p"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%.2p"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%10n"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%-n"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%5%"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%.0%"), -EINVAL);

    return;
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
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%d"), -ENOSYS);
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
    test_format_parser_valid_specs();
    test_format_parser_invalid_specs();
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
