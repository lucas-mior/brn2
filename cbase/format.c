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
    FORMAT_UTF8_MAX_BYTES = 4,
    FORMAT_UNICODE_MAX = 0x10FFFF,
    FORMAT_UNICODE_SURROGATE_FIRST = 0xD800,
    FORMAT_UNICODE_SURROGATE_LAST = 0xDFFF,
    FORMAT_UNICODE_SURROGATE_LOW_FIRST = 0xDC00,
    FORMAT_UNICODE_SURROGATE_MASK = 0x3FF,
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

static void
format_sink_write_repeat(FormatSink *sink, char byte, int64 len) {
    int64 available;
    int64 copy_len;

    ASSERT(sink != NULL);
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
    memset(sink->buffer + sink->written, byte, (size_t)copy_len);
    sink->written += copy_len;
    sink->buffer[sink->written] = '\0';
    return;
}

static bool
format_is_signed_integer_conversion(char conversion) {
    return conversion == 'd' || conversion == 'i';
}

typedef struct FormatArgs {
    va_list args;
} FormatArgs;

static int32
format_load_dynamic_width(FormatSpec *spec, FormatArgs *args) {
    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    if (spec->width_kind == FORMAT_WIDTH_ARG) {
        int32 width = va_arg(args->args, int32);

        if (width < 0) {
            spec->flags |= FORMAT_FLAG_LEFT;
            spec->width = -(int64)width;
        } else {
            spec->width = width;
        }
        spec->width_kind = FORMAT_WIDTH_LITERAL;
    }

    return 0;
}

static int32
format_load_dynamic_width_precision(FormatSpec *spec, FormatArgs *args) {
    int32 status;

    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    if ((status = format_load_dynamic_width(spec, args)) < 0) {
        return status;
    }

    if (spec->precision_kind == FORMAT_PRECISION_ARG) {
        int32 precision = va_arg(args->args, int32);

        if (precision < 0) {
            spec->precision = 0;
            spec->precision_kind = FORMAT_PRECISION_NONE;
        } else {
            spec->precision = precision;
            spec->precision_kind = FORMAT_PRECISION_LITERAL;
        }
    }

    return 0;
}

typedef struct FormatIntegerValue {
    uint64 magnitude;
    bool negative;
} FormatIntegerValue;

static uint64
format_signed_magnitude(int64 value, bool *negative) {
    ASSERT(negative != NULL);

    if (value < 0) {
        *negative = true;
        return (uint64)(-(value + 1)) + 1;
    }

    *negative = false;
    return (uint64)value;
}

static FormatIntegerValue
format_read_signed_integer(FormatSpec *spec, FormatArgs *args) {
    FormatIntegerValue value;
    int64 signed_value;

    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    if (spec->length == FORMAT_LENGTH_HH) {
        signed_value = (int8)va_arg(args->args, int32);
    } else if (spec->length == FORMAT_LENGTH_H) {
        signed_value = (int16)va_arg(args->args, int32);
    } else if (spec->length == FORMAT_LENGTH_LL) {
        signed_value = va_arg(args->args, int64);
    } else if (spec->length == FORMAT_LENGTH_W8) {
        signed_value = (int8)va_arg(args->args, int32);
    } else if (spec->length == FORMAT_LENGTH_W16) {
        signed_value = (int16)va_arg(args->args, int32);
    } else if (spec->length == FORMAT_LENGTH_W32) {
        signed_value = va_arg(args->args, int32);
    } else if (spec->length == FORMAT_LENGTH_W64) {
        signed_value = va_arg(args->args, int64);
    } else {
        ASSERT(spec->length == FORMAT_LENGTH_NONE);
        signed_value = va_arg(args->args, int32);
    }

    value.magnitude = format_signed_magnitude(signed_value, &value.negative);
    return value;
}

static FormatIntegerValue
format_read_unsigned_integer(FormatSpec *spec, FormatArgs *args) {
    FormatIntegerValue value;

    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    value.negative = false;
    if (spec->length == FORMAT_LENGTH_HH) {
        value.magnitude = (uint8)va_arg(args->args, int32);
    } else if (spec->length == FORMAT_LENGTH_H) {
        value.magnitude = (uint16)va_arg(args->args, int32);
    } else if (spec->length == FORMAT_LENGTH_LL) {
        value.magnitude = va_arg(args->args, uint64);
    } else if (spec->length == FORMAT_LENGTH_W8) {
        value.magnitude = (uint8)va_arg(args->args, int32);
    } else if (spec->length == FORMAT_LENGTH_W16) {
        value.magnitude = (uint16)va_arg(args->args, int32);
    } else if (spec->length == FORMAT_LENGTH_W32) {
        value.magnitude = va_arg(args->args, uint32);
    } else if (spec->length == FORMAT_LENGTH_W64) {
        value.magnitude = va_arg(args->args, uint64);
    } else {
        ASSERT(spec->length == FORMAT_LENGTH_NONE);
        value.magnitude = va_arg(args->args, uint32);
    }

    return value;
}

static int32
format_integer_base(char conversion) {
    if (conversion == 'b' || conversion == 'B') {
        return 2;
    }
    if (conversion == 'o') {
        return 8;
    }
    if (conversion == 'x' || conversion == 'X') {
        return 16;
    }

    return 10;
}

static int32
format_integer_digits(char *digits, uint64 value, int32 base, bool upper) {
    char lower_digits[] = "0123456789abcdef";
    char upper_digits[] = "0123456789ABCDEF";
    char reversed[64];
    char *digit_table;
    int32 len;

    ASSERT(digits != NULL);
    ASSERT(base == 2 || base == 8 || base == 10 || base == 16);

    if (upper) {
        digit_table = upper_digits;
    } else {
        digit_table = lower_digits;
    }

    if (value == 0) {
        digits[0] = '0';
        return 1;
    }

    len = 0;
    while (value > 0) {
        uint64 digit = value%(uint64)base;

        reversed[len] = digit_table[digit];
        value /= (uint64)base;
        len += 1;
    }

    for (int32 i = 0; i < len; i += 1) {
        digits[i] = reversed[len - 1 - i];
    }

    return len;
}

static int64
format_pad_len(int64 width, int64 used) {
    if (width > used) {
        return width - used;
    }

    return 0;
}

static int32
format_integer_prefix(FormatSpec *spec, FormatIntegerValue value,
                      char *prefix, int32 digit_len, int64 precision_zeros) {
    ASSERT(spec != NULL);
    ASSERT(prefix != NULL);
    ASSERT_NON_NEGATIVE(digit_len);
    ASSERT_NON_NEGATIVE(precision_zeros);

    if (format_is_signed_integer_conversion(spec->conversion)) {
        if (value.negative) {
            prefix[0] = '-';
            return 1;
        }
        if ((spec->flags & FORMAT_FLAG_SIGN) != 0) {
            prefix[0] = '+';
            return 1;
        }
        if ((spec->flags & FORMAT_FLAG_SPACE) != 0) {
            prefix[0] = ' ';
            return 1;
        }
        return 0;
    }

    if ((spec->flags & FORMAT_FLAG_ALTERNATE) == 0) {
        return 0;
    }
    if (spec->conversion == 'o') {
        if (value.magnitude == 0 && digit_len == 0) {
            prefix[0] = '0';
            return 1;
        }
        if (value.magnitude != 0 && precision_zeros == 0) {
            prefix[0] = '0';
            return 1;
        }
        return 0;
    }
    if (value.magnitude == 0) {
        return 0;
    }
    if (spec->conversion == 'x') {
        prefix[0] = '0';
        prefix[1] = 'x';
        return 2;
    }
    if (spec->conversion == 'X') {
        prefix[0] = '0';
        prefix[1] = 'X';
        return 2;
    }
    if (spec->conversion == 'b') {
        prefix[0] = '0';
        prefix[1] = 'b';
        return 2;
    }
    if (spec->conversion == 'B') {
        prefix[0] = '0';
        prefix[1] = 'B';
        return 2;
    }

    return 0;
}

static void
format_write_integer(FormatSink *sink, FormatSpec *spec,
                     FormatIntegerValue value) {
    char digits[64];
    char prefix[2];
    int32 base;
    int32 digit_len;
    int32 prefix_len;
    int64 precision_zeros;
    int64 zero_pad;
    int64 inner_len;
    int64 spaces;
    bool upper;

    ASSERT(sink != NULL);
    ASSERT(spec != NULL);

    base = format_integer_base(spec->conversion);
    upper = spec->conversion == 'X' || spec->conversion == 'B';
    if (value.magnitude == 0 && format_has_precision(spec)
        && spec->precision == 0) {
        digit_len = 0;
    } else {
        digit_len = format_integer_digits(digits, value.magnitude, base, upper);
    }

    precision_zeros = 0;
    if (format_has_precision(spec) && spec->precision > digit_len) {
        precision_zeros = spec->precision - digit_len;
    }

    prefix_len = format_integer_prefix(spec, value, prefix, digit_len,
                                       precision_zeros);
    inner_len = prefix_len + precision_zeros + digit_len;

    zero_pad = 0;
    if ((spec->flags & FORMAT_FLAG_ZERO) != 0
        && (spec->flags & FORMAT_FLAG_LEFT) == 0
        && !format_has_precision(spec)) {
        zero_pad = format_pad_len(spec->width, inner_len);
    }

    spaces = format_pad_len(spec->width, inner_len + zero_pad);
    if ((spec->flags & FORMAT_FLAG_LEFT) == 0) {
        format_sink_write_repeat(sink, ' ', spaces);
    }
    if (prefix_len > 0) {
        format_sink_write(sink, prefix, prefix_len);
    }
    format_sink_write_repeat(sink, '0', zero_pad);
    format_sink_write_repeat(sink, '0', precision_zeros);
    if (digit_len > 0) {
        format_sink_write(sink, digits, digit_len);
    }
    if ((spec->flags & FORMAT_FLAG_LEFT) != 0) {
        format_sink_write_repeat(sink, ' ', spaces);
    }
    return;
}

static int32
format_handle_integer(FormatSink *sink, FormatSpec *spec, FormatArgs *args) {
    FormatIntegerValue value;
    int32 status;

    ASSERT(sink != NULL);
    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    if ((status = format_load_dynamic_width_precision(spec, args)) < 0) {
        return status;
    }

    if (format_is_signed_integer_conversion(spec->conversion)) {
        value = format_read_signed_integer(spec, args);
    } else {
        value = format_read_unsigned_integer(spec, args);
    }

    format_write_integer(sink, spec, value);
    return sink->status;
}

static int64
format_string_len_limited(char *string, int64 limit) {
    int64 len;

    ASSERT(string != NULL);
    ASSERT_NON_NEGATIVE(limit);

    len = 0;
    while (len < limit && string[len] != '\0') {
        len += 1;
    }

    return len;
}

static void
format_write_padded_bytes(FormatSink *sink, FormatSpec *spec,
                          char *data, int64 len) {
    int64 spaces;

    ASSERT(sink != NULL);
    ASSERT(spec != NULL);
    ASSERT_NON_NEGATIVE(len);
    ASSERT(data != NULL || len == 0);

    spaces = format_pad_len(spec->width, len);
    if ((spec->flags & FORMAT_FLAG_LEFT) == 0) {
        format_sink_write_repeat(sink, ' ', spaces);
    }
    if (len > 0) {
        format_sink_write(sink, data, len);
    }
    if ((spec->flags & FORMAT_FLAG_LEFT) != 0) {
        format_sink_write_repeat(sink, ' ', spaces);
    }
    return;
}

static int32
format_handle_char(FormatSink *sink, FormatSpec *spec, FormatArgs *args) {
    int32 status;
    int32 value;
    char byte;

    ASSERT(sink != NULL);
    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    if ((status = format_load_dynamic_width(spec, args)) < 0) {
        return status;
    }

    value = va_arg(args->args, int32);
    byte = (char)(uint8)value;
    format_write_padded_bytes(sink, spec, &byte, 1);
    return sink->status;
}

static int32
format_load_string_precision(FormatSpec *spec, FormatArgs *args,
                             bool *exact_span) {
    ASSERT(spec != NULL);
    ASSERT(args != NULL);
    ASSERT(exact_span != NULL);

    *exact_span = false;
    if (spec->precision_kind == FORMAT_PRECISION_ARG) {
        int32 precision = va_arg(args->args, int32);

        if (precision < 0) {
            return -EINVAL;
        }

        *exact_span = true;
        spec->precision = precision;
        spec->precision_kind = FORMAT_PRECISION_LITERAL;
    }

    return 0;
}

static int32
format_handle_string(FormatSink *sink, FormatSpec *spec, FormatArgs *args) {
    int32 status;
    int64 len;
    char *string;
    bool exact_span;

    ASSERT(sink != NULL);
    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    if ((status = format_load_dynamic_width(spec, args)) < 0) {
        return status;
    }
    if ((status = format_load_string_precision(spec, args, &exact_span)) < 0) {
        return status;
    }

    string = va_arg(args->args, char *);
    if (exact_span) {
        len = spec->precision;
        if (string == NULL && len > 0) {
            return -EINVAL;
        }
    } else {
        if (string == NULL) {
            string = "(null)";
        }
        if (format_has_precision(spec)) {
            len = format_string_len_limited(string, spec->precision);
        } else {
            len = format_string_len_limited(string, INT64_MAX);
        }
    }

    format_write_padded_bytes(sink, spec, string, len);
    return sink->status;
}


static bool
format_unicode_is_surrogate(uint32 rune) {
    return BETWEEN(rune, FORMAT_UNICODE_SURROGATE_FIRST,
                   FORMAT_UNICODE_SURROGATE_LAST);
}

static int32
format_encode_utf8_scalar(uint32 rune, char *buffer, int32 *len) {
    ASSERT(buffer != NULL);
    ASSERT(len != NULL);

    if (rune > FORMAT_UNICODE_MAX || format_unicode_is_surrogate(rune)) {
        return -EILSEQ;
    }

    *len = utf8_encode_raw(rune, buffer);
    if (*len <= 0 || *len > FORMAT_UTF8_MAX_BYTES) {
        return -EILSEQ;
    }

    return 0;
}

static int32
format_wint_to_rune(wint_t value, uint32 *rune) {
    uint64 raw;

    ASSERT(rune != NULL);

    raw = (uint64)value;
    if (raw > FORMAT_UNICODE_MAX) {
        return -EILSEQ;
    }

    *rune = (uint32)raw;
    if (format_unicode_is_surrogate(*rune)) {
        return -EILSEQ;
    }

    return 0;
}

static int32
format_wchar32_to_rune(wchar_t value, uint32 *rune) {
    uint64 raw;

    ASSERT(rune != NULL);

    raw = (uint64)value;
    if (raw > FORMAT_UNICODE_MAX) {
        return -EILSEQ;
    }

    *rune = (uint32)raw;
    if (format_unicode_is_surrogate(*rune)) {
        return -EILSEQ;
    }

    return 0;
}

static bool
format_wchar16_is_high_surrogate(uint32 code_unit) {
    return BETWEEN(code_unit, FORMAT_UNICODE_SURROGATE_FIRST,
                   FORMAT_UNICODE_SURROGATE_LOW_FIRST - 1);
}

static bool
format_wchar16_is_low_surrogate(uint32 code_unit) {
    return BETWEEN(code_unit, FORMAT_UNICODE_SURROGATE_LOW_FIRST,
                   FORMAT_UNICODE_SURROGATE_LAST);
}

static uint32
format_wchar16_pair_to_rune(uint32 high, uint32 low) {
    uint32 high_bits;
    uint32 low_bits;

    ASSERT(format_wchar16_is_high_surrogate(high));
    ASSERT(format_wchar16_is_low_surrogate(low));

    high_bits = (high - FORMAT_UNICODE_SURROGATE_FIRST)
                & FORMAT_UNICODE_SURROGATE_MASK;
    low_bits = (low - FORMAT_UNICODE_SURROGATE_LOW_FIRST)
               & FORMAT_UNICODE_SURROGATE_MASK;
    return 0x10000 + (high_bits << 10) + low_bits;
}

static int32
format_wide_string_next_rune(wchar_t *string, int64 index, uint32 *rune,
                             int64 *consumed) {
    ASSERT(string != NULL);
    ASSERT_NON_NEGATIVE(index);
    ASSERT(rune != NULL);
    ASSERT(consumed != NULL);

    if (SIZEOF(wchar_t) == 2) {
        uint32 first;

        first = (uint16)string[index];
        if (format_wchar16_is_high_surrogate(first)) {
            uint32 second = (uint16)string[index + 1];

            if (!format_wchar16_is_low_surrogate(second)) {
                return -EILSEQ;
            }
            *rune = format_wchar16_pair_to_rune(first, second);
            *consumed = 2;
            return 0;
        }
        if (format_wchar16_is_low_surrogate(first)) {
            return -EILSEQ;
        }

        *rune = first;
        *consumed = 1;
        return 0;
    }

    if (SIZEOF(wchar_t) == 4) {
        int32 status;

        if ((status = format_wchar32_to_rune(string[index], rune)) < 0) {
            return status;
        }
        *consumed = 1;
        return 0;
    }

    return -EILSEQ;
}

static int32
format_load_wide_string_precision(FormatSpec *spec, FormatArgs *args) {
    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    if (spec->precision_kind == FORMAT_PRECISION_ARG) {
        int32 precision = va_arg(args->args, int32);

        if (precision < 0) {
            spec->precision = 0;
            spec->precision_kind = FORMAT_PRECISION_NONE;
        } else {
            spec->precision = precision;
            spec->precision_kind = FORMAT_PRECISION_LITERAL;
        }
    }

    return 0;
}

static int32
format_wide_string_utf8_len(wchar_t *string, int64 limit, int64 *len) {
    int64 total;
    int64 index;

    ASSERT(string != NULL);
    ASSERT(len != NULL);
    ASSERT(limit >= -1);

    total = 0;
    index = 0;
    while (string[index] != 0) {
        char encoded[FORMAT_UTF8_MAX_BYTES];
        int64 consumed;
        uint32 rune;
        int32 encoded_len;
        int32 status;

        if ((status = format_wide_string_next_rune(string, index, &rune,
                                                   &consumed)) < 0) {
            return status;
        }
        if ((status = format_encode_utf8_scalar(rune, encoded,
                                                &encoded_len)) < 0) {
            return status;
        }
        if (limit >= 0 && encoded_len > limit - total) {
            break;
        }
        if (encoded_len > INT64_MAX - total) {
            return -EOVERFLOW;
        }

        total += encoded_len;
        index += consumed;
    }

    *len = total;
    return 0;
}

static void
format_write_wide_string_utf8(FormatSink *sink, wchar_t *string, int64 len) {
    int64 written;
    int64 index;

    ASSERT(sink != NULL);
    ASSERT(string != NULL);
    ASSERT_NON_NEGATIVE(len);

    written = 0;
    index = 0;
    while (written < len) {
        char encoded[FORMAT_UTF8_MAX_BYTES];
        int64 consumed;
        uint32 rune;
        int32 encoded_len;
        int32 status;

        status = format_wide_string_next_rune(string, index, &rune, &consumed);
        ASSERT_EQUAL(status, 0);
        status = format_encode_utf8_scalar(rune, encoded, &encoded_len);
        ASSERT_EQUAL(status, 0);
        ASSERT(encoded_len <= len - written);

        format_sink_write(sink, encoded, encoded_len);
        written += encoded_len;
        index += consumed;
    }
    return;
}

static int32
format_handle_wide_char(FormatSink *sink, FormatSpec *spec,
                        FormatArgs *args) {
    char encoded[FORMAT_UTF8_MAX_BYTES];
    uint32 rune;
    int32 encoded_len;
    int32 status;
    wint_t value;

    ASSERT(sink != NULL);
    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    if ((status = format_load_dynamic_width(spec, args)) < 0) {
        return status;
    }

    value = va_arg(args->args, wint_t);
    if ((status = format_wint_to_rune(value, &rune)) < 0) {
        return status;
    }
    if ((status = format_encode_utf8_scalar(rune, encoded,
                                            &encoded_len)) < 0) {
        return status;
    }

    format_write_padded_bytes(sink, spec, encoded, encoded_len);
    return sink->status;
}

static int32
format_handle_wide_string(FormatSink *sink, FormatSpec *spec,
                          FormatArgs *args) {
    int64 limit;
    int64 len;
    int64 spaces;
    int32 status;
    wchar_t *string;

    ASSERT(sink != NULL);
    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    if ((status = format_load_dynamic_width(spec, args)) < 0) {
        return status;
    }
    if ((status = format_load_wide_string_precision(spec, args)) < 0) {
        return status;
    }

    string = va_arg(args->args, wchar_t *);
    if (string == NULL) {
        char *null_string = "(null)";

        if (format_has_precision(spec)) {
            len = format_string_len_limited(null_string, spec->precision);
        } else {
            len = format_string_len_limited(null_string, INT64_MAX);
        }
        format_write_padded_bytes(sink, spec, null_string, len);
        return sink->status;
    }

    if (format_has_precision(spec)) {
        limit = spec->precision;
    } else {
        limit = -1;
    }
    if ((status = format_wide_string_utf8_len(string, limit, &len)) < 0) {
        return status;
    }

    spaces = format_pad_len(spec->width, len);
    if ((spec->flags & FORMAT_FLAG_LEFT) == 0) {
        format_sink_write_repeat(sink, ' ', spaces);
    }
    format_write_wide_string_utf8(sink, string, len);
    if ((spec->flags & FORMAT_FLAG_LEFT) != 0) {
        format_sink_write_repeat(sink, ' ', spaces);
    }
    return sink->status;
}

static int32
format_handle_char_string(FormatSink *sink, FormatSpec *spec,
                          FormatArgs *args) {
    ASSERT(sink != NULL);
    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    if (spec->conversion == 'c') {
        if (spec->length == FORMAT_LENGTH_L) {
            return format_handle_wide_char(sink, spec, args);
        }
        return format_handle_char(sink, spec, args);
    }

    ASSERT(spec->conversion == 's');
    if (spec->length == FORMAT_LENGTH_L) {
        return format_handle_wide_string(sink, spec, args);
    }
    return format_handle_string(sink, spec, args);
}


static void
format_write_pointer(FormatSink *sink, FormatSpec *spec, void *pointer) {
    char digits[64];
    char prefix[] = {'0', 'x'};
    uintptr value;
    int32 digit_len;
    int64 inner_len;
    int64 spaces;

    ASSERT(sink != NULL);
    ASSERT(spec != NULL);

    value = (uintptr)pointer;
    digit_len = format_integer_digits(digits, (uint64)value, 16, false);
    inner_len = 2 + digit_len;
    spaces = format_pad_len(spec->width, inner_len);

    if ((spec->flags & FORMAT_FLAG_LEFT) == 0) {
        format_sink_write_repeat(sink, ' ', spaces);
    }
    format_sink_write(sink, prefix, 2);
    format_sink_write(sink, digits, digit_len);
    if ((spec->flags & FORMAT_FLAG_LEFT) != 0) {
        format_sink_write_repeat(sink, ' ', spaces);
    }
    return;
}

static int32
format_handle_pointer(FormatSink *sink, FormatSpec *spec, FormatArgs *args) {
    int32 status;
    void *pointer;

    ASSERT(sink != NULL);
    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    if ((status = format_load_dynamic_width(spec, args)) < 0) {
        return status;
    }

    pointer = va_arg(args->args, void *);
    format_write_pointer(sink, spec, pointer);
    return sink->status;
}

static bool
format_count_fits(FormatSpec *spec, int64 count) {
    ASSERT(spec != NULL);
    ASSERT_NON_NEGATIVE(count);

    if (spec->length == FORMAT_LENGTH_HH
        || spec->length == FORMAT_LENGTH_W8) {
        return count <= INT8_MAX;
    }
    if (spec->length == FORMAT_LENGTH_H
        || spec->length == FORMAT_LENGTH_W16) {
        return count <= INT16_MAX;
    }
    if (spec->length == FORMAT_LENGTH_LL
        || spec->length == FORMAT_LENGTH_W64) {
        return count <= INT64_MAX;
    }

    ASSERT(spec->length == FORMAT_LENGTH_NONE
           || spec->length == FORMAT_LENGTH_W32);
    return count <= INT32_MAX;
}

static int32
format_store_count(FormatSpec *spec, FormatArgs *args, int64 count) {
    ASSERT(spec != NULL);
    ASSERT(args != NULL);
    ASSERT_NON_NEGATIVE(count);

    if (!format_count_fits(spec, count)) {
        return -EOVERFLOW;
    }

    if (spec->length == FORMAT_LENGTH_HH) {
        int8 *pointer = va_arg(args->args, int8 *);

        if (pointer == NULL) {
            return -EINVAL;
        }
        *pointer = (int8)count;
        return 0;
    }
    if (spec->length == FORMAT_LENGTH_H) {
        int16 *pointer = va_arg(args->args, int16 *);

        if (pointer == NULL) {
            return -EINVAL;
        }
        *pointer = (int16)count;
        return 0;
    }
    if (spec->length == FORMAT_LENGTH_LL) {
        int64 *pointer = va_arg(args->args, int64 *);

        if (pointer == NULL) {
            return -EINVAL;
        }
        *pointer = count;
        return 0;
    }
    if (spec->length == FORMAT_LENGTH_W8) {
        int8 *pointer = va_arg(args->args, int8 *);

        if (pointer == NULL) {
            return -EINVAL;
        }
        *pointer = (int8)count;
        return 0;
    }
    if (spec->length == FORMAT_LENGTH_W16) {
        int16 *pointer = va_arg(args->args, int16 *);

        if (pointer == NULL) {
            return -EINVAL;
        }
        *pointer = (int16)count;
        return 0;
    }
    if (spec->length == FORMAT_LENGTH_W32) {
        int32 *pointer = va_arg(args->args, int32 *);

        if (pointer == NULL) {
            return -EINVAL;
        }
        *pointer = (int32)count;
        return 0;
    }
    if (spec->length == FORMAT_LENGTH_W64) {
        int64 *pointer = va_arg(args->args, int64 *);

        if (pointer == NULL) {
            return -EINVAL;
        }
        *pointer = count;
        return 0;
    }

    ASSERT(spec->length == FORMAT_LENGTH_NONE);
    {
        int32 *pointer = va_arg(args->args, int32 *);

        if (pointer == NULL) {
            return -EINVAL;
        }
        *pointer = (int32)count;
        return 0;
    }
}

static int32
format_handle_count(FormatSink *sink, FormatSpec *spec, FormatArgs *args) {
    ASSERT(sink != NULL);
    ASSERT(spec != NULL);
    ASSERT(args != NULL);

    if (sink->status < 0) {
        return sink->status;
    }
    return format_store_count(spec, args, sink->total);
}


static int32 UNUSED
format_vsnprintf_impl(char *buffer, int64 capacity, char *format,
                      va_list args) {
    FormatArgs format_args;
    FormatSink sink;
    char *literal;
    char *cursor;
    int32 result;
    int32 status;

    if (format == NULL) {
        return -EINVAL;
    }
    if ((status = format_sink_init(&sink, buffer, capacity)) < 0) {
        return status;
    }

    va_copy(format_args.args, args);
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
            result = format_sink_finish(&sink);
            goto done;
        }

        cursor += 1;
        if ((status = format_parse_spec(cursor, &cursor, &spec)) < 0) {
            result = status;
            goto done;
        }
        if (format_is_integer_conversion(spec.conversion)) {
            status = format_handle_integer(&sink, &spec, &format_args);
            if (status < 0) {
                result = status;
                goto done;
            }
        } else if (spec.conversion == 'c' || spec.conversion == 's') {
            status = format_handle_char_string(&sink, &spec, &format_args);
            if (status < 0) {
                result = status;
                goto done;
            }
        } else if (spec.conversion == 'p') {
            status = format_handle_pointer(&sink, &spec, &format_args);
            if (status < 0) {
                result = status;
                goto done;
            }
        } else if (spec.conversion == 'n') {
            status = format_handle_count(&sink, &spec, &format_args);
            if (status < 0) {
                result = status;
                goto done;
            }
        } else if (spec.conversion == '%') {
            format_sink_write_byte(&sink, '%');
        } else {
            result = -ENOSYS;
            goto done;
        }

        literal = cursor;
    }

    format_sink_write(&sink, literal, cursor - literal);
    result = format_sink_finish(&sink);

done:
    va_end(format_args.args);
    return result;
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
    ASSERT_EQUAL(format_test_validate("%0p"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%+p"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%.2p"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%10n"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%-n"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%+n"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%.0n"), -EINVAL);
    ASSERT_EQUAL(format_test_validate("%ln"), -EINVAL);
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
test_format_integer_capacity(char *expected, char *format, ...) {
    char buffer[256];
    int32 expected_len;

    expected_len = strlen32(expected);
    ASSERT_LESS(expected_len + 2, SIZEOF(buffer));

    for (int32 capacity = 0; capacity <= expected_len + 2; capacity += 1) {
        va_list args;
        int32 copied;
        int32 len;

        memset(buffer, 0x7f, SIZEOF(buffer));
        va_start(args, format);
        len = format_vsnprintf_impl(buffer, capacity, format, args);
        va_end(args);
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
test_format_bytes_capacity(char *expected, int32 expected_len,
                           char *format, ...) {
    char buffer[256];

    ASSERT_NON_NEGATIVE(expected_len);
    ASSERT_LESS(expected_len + 2, SIZEOF(buffer));

    for (int32 capacity = 0; capacity <= expected_len + 2; capacity += 1) {
        va_list args;
        int32 copied;
        int32 len;

        memset(buffer, 0x7f, SIZEOF(buffer));
        va_start(args, format);
        len = format_vsnprintf_impl(buffer, capacity, format, args);
        va_end(args);
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
test_format_integer_outputs(void) {
    test_format_integer_capacity("0", "%d", 0);
    test_format_integer_capacity("-123", "%i", -123);
    test_format_integer_capacity("-2147483648", "%d", INT32_MIN);
    test_format_integer_capacity("4294967295", "%u", (uint32)UINT32_MAX);
    test_format_integer_capacity("12", "%o", (uint32)10);
    test_format_integer_capacity("abc", "%x", (uint32)0xabc);
    test_format_integer_capacity("ABC", "%X", (uint32)0xabc);
    test_format_integer_capacity("1010", "%b", (uint32)10);
    test_format_integer_capacity("1010", "%B", (uint32)10);

    test_format_integer_capacity("+42", "%+d", 42);
    test_format_integer_capacity(" 42", "% d", 42);
    test_format_integer_capacity("+42", "%+ d", 42);
    test_format_integer_capacity("-0000042", "%08d", -42);
    test_format_integer_capacity("42    ", "%-6d", 42);
    test_format_integer_capacity("   00042", "%8.5d", 42);
    test_format_integer_capacity("   00042", "%08.5d", 42);
    test_format_integer_capacity("00042   ", "%-8.5d", 42);
    test_format_integer_capacity("", "%.0d", 0);
    test_format_integer_capacity("     ", "%5.0d", 0);

    test_format_integer_capacity("012", "%#o", (uint32)10);
    test_format_integer_capacity("0", "%#.0o", (uint32)0);
    test_format_integer_capacity("    0", "%#5.0o", (uint32)0);
    test_format_integer_capacity("012", "%#.3o", (uint32)10);
    test_format_integer_capacity("00012", "%#.5o", (uint32)10);
    test_format_integer_capacity("0x2a", "%#x", (uint32)42);
    test_format_integer_capacity("0X2A", "%#X", (uint32)42);
    test_format_integer_capacity("0b101", "%#b", (uint32)5);
    test_format_integer_capacity("0B101", "%#B", (uint32)5);
    test_format_integer_capacity(" 0x0a", "%#5.2x", (uint32)10);
    test_format_integer_capacity("00000012", "%#08o", (uint32)10);

    test_format_integer_capacity("42    ", "%*d", -6, 42);
    test_format_integer_capacity("00042", "%0*d", 5, 42);
    test_format_integer_capacity("42", "%.*d", -1, 42);
    test_format_integer_capacity("00042", "%.*d", 5, 42);
    test_format_integer_capacity("   00042", "%*.*d", 8, 5, 42);

    test_format_integer_capacity("-1 2a 3", "%d %x %u", -1,
                                 (uint32)0x2a, (uint32)3);

    test_format_integer_capacity("18", "%hhd", 0x12);
    test_format_integer_capacity("44", "%hhu", 300);
    test_format_integer_capacity("-1234", "%hd", -1234);
    test_format_integer_capacity("65535", "%hu", 65535);
    test_format_integer_capacity("-9223372036854775808", "%lld",
                                 (int64)INT64_MIN);
    test_format_integer_capacity("18446744073709551615", "%llu",
                                 (uint64)UINT64_MAX);

    test_format_integer_capacity("-128", "%w8d", -128);
    test_format_integer_capacity("255", "%w8u", 255);
    test_format_integer_capacity("65535", "%w16u", 65535);
    test_format_integer_capacity("-2147483648", "%w32d", INT32_MIN);
    test_format_integer_capacity("4294967295", "%w32u",
                                 (uint32)UINT32_MAX);
    test_format_integer_capacity("-9223372036854775808", "%w64d",
                                 (int64)INT64_MIN);
    test_format_integer_capacity("18446744073709551615", "%w64u",
                                 (uint64)UINT64_MAX);

    return;
}

static void
test_format_char_string_outputs(void) {
    char nul_char_expected[] = {'\0'};
    char span[] = {'a', '\0', 'b', 'c'};
    char span_expected[] = {'a', '\0', 'b', 'c'};
    char span_width_expected[] = {' ', ' ', 'a', '\0', 'b'};
    char span_left_expected[] = {'a', '\0', 'b', ' ', ' '};
    char plain_precision_expected[] = {'a'};
    char spaces[] = {' ', ' ', ' '};
    char buffer[16];

    test_format_bytes_capacity("A", 1, "%c", 'A');
    test_format_bytes_capacity("  A", 3, "%3c", 'A');
    test_format_bytes_capacity("A  ", 3, "%-3c", 'A');
    test_format_bytes_capacity(nul_char_expected, 1, "%c", 0);

    test_format_bytes_capacity("abc", 3, "%s", "abc");
    test_format_bytes_capacity("  abc", 5, "%5s", "abc");
    test_format_bytes_capacity("abc  ", 5, "%-5s", "abc");
    test_format_bytes_capacity("ab", 2, "%.2s", "abc");
    test_format_bytes_capacity("   ab", 5, "%5.2s", "abc");
    test_format_bytes_capacity("(null)", 6, "%s", (char *)NULL);
    test_format_bytes_capacity("(nu", 3, "%.3s", (char *)NULL);
    test_format_bytes_capacity("     (nu", 8, "%8.3s", (char *)NULL);

    test_format_bytes_capacity(span_expected, 4, "%.*s", 4, span);
    test_format_bytes_capacity(span_width_expected, 5, "%5.*s", 3, span);
    test_format_bytes_capacity(span_left_expected, 5, "%-5.*s", 3, span);
    test_format_bytes_capacity(plain_precision_expected, 1, "%.4s", span);
    test_format_bytes_capacity("", 0, "%.*s", 0, (char *)NULL);
    test_format_bytes_capacity(spaces, 3, "%3.*s", 0, (char *)NULL);

    test_format_bytes_capacity("x=abc n=7 c=Z", 13, "x=%s n=%d c=%c",
                               "abc", 7, 'Z');

    memset(buffer, 0x7f, SIZEOF(buffer));
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%.*s", -1,
                                      "abc"), -EINVAL);
    ASSERT_EQUAL(buffer[0], '\0');
    ASSERT_EQUAL(buffer[1], (char)0x7f);

    memset(buffer, 0x7f, SIZEOF(buffer));
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%.*s", 1,
                                      (char *)NULL), -EINVAL);
    ASSERT_EQUAL(buffer[0], '\0');
    ASSERT_EQUAL(buffer[1], (char)0x7f);

    return;
}


static void
test_format_wide_char_string_outputs(void) {
    char nul_char_expected[] = {'\0'};
    char e_acute[] = {(char)0xC3, (char)0xA9};
    char euro[] = {(char)0xE2, (char)0x82, (char)0xAC};
    char emoji[] = {(char)0xF0, (char)0x9F, (char)0x98, (char)0x80};
    char wide_text_expected[] = {
        'A', (char)0xC3, (char)0xA9,
        (char)0xE2, (char)0x82, (char)0xAC,
    };
    char wide_text_precision_expected[] = {
        'A', (char)0xC3, (char)0xA9,
    };
    char wide_text_width_expected[] = {
        ' ', ' ', 'A', (char)0xC3, (char)0xA9,
    };
    char wide_text_left_expected[] = {
        'A', (char)0xC3, (char)0xA9, ' ', ' ',
    };
    wchar_t wide_text[] = {'A', 0x00E9, 0x20AC, 0};
    wchar_t bad_high[] = {(wchar_t)0xD800, 0};
    wchar_t bad_low[] = {(wchar_t)0xDC00, 0};
    char buffer[32];

    test_format_bytes_capacity("A", 1, "%lc", (wint_t)'A');
    test_format_bytes_capacity(e_acute, 2, "%lc", (wint_t)0x00E9);
    test_format_bytes_capacity(euro, 3, "%lc", (wint_t)0x20AC);
    test_format_bytes_capacity(emoji, 4, "%lc", (wint_t)0x1F600);
    test_format_bytes_capacity("  A", 3, "%3lc", (wint_t)'A');
    test_format_bytes_capacity("A  ", 3, "%-3lc", (wint_t)'A');
    test_format_bytes_capacity(nul_char_expected, 1, "%lc", (wint_t)0);

    test_format_bytes_capacity(wide_text_expected, 6, "%ls", wide_text);
    test_format_bytes_capacity(wide_text_precision_expected, 3, "%.3ls",
                               wide_text);
    test_format_bytes_capacity(wide_text_precision_expected, 3, "%.4ls",
                               wide_text);
    test_format_bytes_capacity(wide_text_width_expected, 5, "%5.3ls",
                               wide_text);
    test_format_bytes_capacity(wide_text_left_expected, 5, "%-5.3ls",
                               wide_text);
    test_format_bytes_capacity(wide_text_precision_expected, 3, "%.*ls", 3,
                               wide_text);
    test_format_bytes_capacity(wide_text_expected, 6, "%.*ls", -1,
                               wide_text);
    test_format_bytes_capacity("(null)", 6, "%ls", (wchar_t *)NULL);
    test_format_bytes_capacity("(nu", 3, "%.3ls", (wchar_t *)NULL);

    {
        wchar_t wide_emoji[3] = {0};

        if (SIZEOF(wchar_t) == 2) {
            wide_emoji[0] = (wchar_t)0xD83D;
            wide_emoji[1] = (wchar_t)0xDE00;
        } else {
            wide_emoji[0] = (wchar_t)0x1F600;
        }
        test_format_bytes_capacity(emoji, 4, "%ls", wide_emoji);
    }

    memset(buffer, 0x7f, SIZEOF(buffer));
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%lc",
                                      (wint_t)0xD800), -EILSEQ);
    ASSERT_EQUAL(buffer[0], '\0');
    ASSERT_EQUAL(buffer[1], (char)0x7f);

    memset(buffer, 0x7f, SIZEOF(buffer));
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%lc",
                                      (wint_t)0x110000), -EILSEQ);
    ASSERT_EQUAL(buffer[0], '\0');
    ASSERT_EQUAL(buffer[1], (char)0x7f);

    memset(buffer, 0x7f, SIZEOF(buffer));
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%ls",
                                      bad_high), -EILSEQ);
    ASSERT_EQUAL(buffer[0], '\0');
    ASSERT_EQUAL(buffer[1], (char)0x7f);

    memset(buffer, 0x7f, SIZEOF(buffer));
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%ls",
                                      bad_low), -EILSEQ);
    ASSERT_EQUAL(buffer[0], '\0');
    ASSERT_EQUAL(buffer[1], (char)0x7f);

    return;
}

static void
test_format_pointer_count_outputs(void) {
    char buffer[16];
    void *pointer;
    int8 count8;
    int16 count16;
    int32 count32;
    int64 count64;

    pointer = (void *)(uintptr)0x1234;
    test_format_bytes_capacity("0x0", 3, "%p", (void *)NULL);
    test_format_bytes_capacity("   0x0", 6, "%6p", (void *)NULL);
    test_format_bytes_capacity("0x0   ", 6, "%-6p", (void *)NULL);
    test_format_bytes_capacity("0x1234", 6, "%p", pointer);
    test_format_bytes_capacity("p=0x1234.", 9, "p=%p.", pointer);
    test_format_bytes_capacity("   0x0", 6, "%*p", 6, (void *)NULL);
    test_format_bytes_capacity("0x0   ", 6, "%*p", -6, (void *)NULL);

    count32 = -1;
    test_format_bytes_capacity("abcd", 4, "ab%ncd", &count32);
    ASSERT_EQUAL(count32, 2);

    memset(buffer, 0x7f, SIZEOF(buffer));
    count32 = -1;
    ASSERT_EQUAL(format_test_snprintf(buffer, 2, "abcd%n", &count32), 4);
    ASSERT_EQUAL(count32, 4);
    ASSERT_EQUAL(buffer[0], 'a');
    ASSERT_EQUAL(buffer[1], '\0');
    ASSERT_EQUAL(buffer[2], (char)0x7f);

    count8 = -1;
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "abc%hhn",
                                      &count8), 3);
    ASSERT_EQUAL(count8, 3);

    count16 = -1;
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "abc%hn",
                                      &count16), 3);
    ASSERT_EQUAL(count16, 3);

    count64 = -1;
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "abc%lln",
                                      &count64), 3);
    ASSERT_EQUAL(count64, 3);

    count8 = -1;
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "abc%w8n",
                                      &count8), 3);
    ASSERT_EQUAL(count8, 3);

    count16 = -1;
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "abc%w16n",
                                      &count16), 3);
    ASSERT_EQUAL(count16, 3);

    count32 = -1;
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "abc%w32n",
                                      &count32), 3);
    ASSERT_EQUAL(count32, 3);

    count64 = -1;
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "abc%w64n",
                                      &count64), 3);
    ASSERT_EQUAL(count64, 3);

    count8 = -7;
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%128d%hhn", 0,
                                      &count8), -EOVERFLOW);
    ASSERT_EQUAL(count8, -7);

    count16 = -7;
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%32768d%hn", 0,
                                      &count16), -EOVERFLOW);
    ASSERT_EQUAL(count16, -7);

    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "abc%n",
                                      (int32 *)NULL), -EINVAL);

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
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%*d",
                                      INT32_MIN, 0), -EOVERFLOW);

    memset(buffer, 0x7f, SIZEOF(buffer));
    ASSERT_EQUAL(format_test_snprintf(buffer, SIZEOF(buffer), "%f", 1.0),
                 -ENOSYS);
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
    test_format_integer_outputs();
    test_format_char_string_outputs();
    test_format_wide_char_string_outputs();
    test_format_pointer_count_outputs();
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
