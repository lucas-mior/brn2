#if !defined(UTF8_H)
#define UTF8_H

#define UTF_INVALID 0xFFFD

static int32 random_utf8_string(char *, int32, int32);
static int32 utf8_byte_position(char *, int32, int32);
static int32 utf8_capitalize_first_letters(char *, int32, char *, int32);
static int32 utf8_char_width(uint32);
static int32 utf8_characters(char *, int32);
static int32 utf8_cut_width(char *, int32, int32);
static int32 utf8_decode(char *, int32, uint32 *);
static uint32 utf8_decode_byte(char, int32 *);
static int32 utf8_decode_raw(char *, uint32 *, int32);
static int32 utf8_encode(uint32, char *, int32);
static char utf8_encode_byte(uint32, int32);
static int32 utf8_encode_raw(uint32, char *);
static void utf8_functions_sink(void);
static int32 utf8_next_position(char *, int32, int32);
static int32 utf8_suffix_width_position(char *, int32, int32);
static int32 utf8_validate(uint32 *, int32);
static int32 utf8_width(char *, int32);

#endif /* UTF8_H */
