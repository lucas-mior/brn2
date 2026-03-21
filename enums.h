#undef XENUM
#undef XENUM_1
#undef XENUM_2
#undef BEGIN_ENUM
#undef END_ENUM

#if !defined(__INCLUDE_LEVEL__) || (__INCLUDE_LEVEL__ >= 1)
#if !defined(ENUM_PREFIX_)
#error "enums.h included but ENUM_PREFIX_ is not defined"
#endif
#endif

// use 1234 and 4321 to decrease the chance of someone else using this macro
// (even though the compiler should emit a warning anyway)
#if !defined(ENUM_GENERATE_STRINGS)
#define ENUM_GENERATE_STRINGS 4321
#endif

#if ENUM_GENERATE_STRINGS == 1234
  #undef ENUM_GENERATE_STRINGS
  #define ENUM_GENERATE_STRINGS 4321
#elif ENUM_GENERATE_STRINGS == 4321
  #undef ENUM_GENERATE_STRINGS
  #define ENUM_GENERATE_STRINGS 1234
#else
  #error "ENUM_GENERATE_STRINGS must alternate between 1234 and 4321"
#endif

#if !defined(CAT) || !defined(CAT3)
  #define CAT_(a, b)     a##b
  #define CAT3_(a, b, c) a##b##c
  #define CAT(a, b)      CAT_(a, b)
  #define CAT3(a, b, c)  CAT3_(a, b, c)
#endif

#define NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, n, ...) n
#define NUM_ARGS(...) NUM_ARGS_(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, x)
#define SELECT_ON_NUM_ARGS(macro, ...) \
    CAT(macro, NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

#if !defined(QUOTE)
#define QUOTE_(x) #x
#define QUOTE(x) QUOTE_(x)
#endif

#define XENUM(...) SELECT_ON_NUM_ARGS(XENUM_, __VA_ARGS__)

#if ENUM_GENERATE_STRINGS == 1234
  #define BEGIN_ENUM(EnumName) enum EnumName {

#if !defined(ENUM_IS_FLAGS) // don't allow automatic value in the flags case
  #define XENUM_1(e)             CAT(ENUM_PREFIX_, e),
#endif
  #define XENUM_2(e, v)          CAT(ENUM_PREFIX_, e) = v, 

  #define END_ENUM(EnumName)     CAT(ENUM_PREFIX_, LAST) \
                               }; \
                               char *CAT(ENUM_PREFIX_, str)(enum EnumName v);
#else
#if !defined(ENUM_IS_FLAGS)
  #define BEGIN_ENUM(EnumName) char *CAT(ENUM_PREFIX_, str)(enum EnumName v) { \
                                 switch (v) {

  #define XENUM_1(e)               case CAT(ENUM_PREFIX_, e):                  \
                                     return QUOTE(ENUM_PREFIX_) #e;
  #define XENUM_2(e, v)            case CAT(ENUM_PREFIX_, e):                  \
                                     return QUOTE(ENUM_PREFIX_) #e;

  #define END_ENUM(EnumName)       case CAT(ENUM_PREFIX_, LAST):               \
                                     return QUOTE(ENUM_PREFIX_) "LAST";        \
                                   default:                                    \
                                     return "Unknown value";                   \
                                 }                                             \
                               }
#else
  #define BEGIN_ENUM(EnumName) char *CAT(ENUM_PREFIX_, str)(enum EnumName x) { \
                                 char buffer[4096];                            \
                                 char *buffer_ptr = buffer;                    \
                                 char *buffer_end = buffer + sizeof(buffer);   \
                                 bool is_first_flag = true;                    \
                                 int64 final_length;                           \
                                 char *buffer_copy;

  #define XENUM_2(e, v)          if (x & CAT(ENUM_PREFIX_, e)) {               \
                                   char *flag = QUOTE(ENUM_PREFIX_) #e;        \
                                   int32 len = strlen32(flag);                 \
                                   if (is_first_flag == false) {               \
                                     *buffer_ptr++ = '|';                      \
                                   }                                           \
                                   if (buffer_ptr + len < (buffer_end - 1)) {  \
                                       memcpy64(buffer_ptr, flag, len);        \
                                       buffer_ptr += len;                      \
                                   }                                           \
                                   is_first_flag = false;                      \
                                 }

  #define END_ENUM(EnumName)     if (buffer_ptr == buffer) {                   \
                                   return "NONE";                              \
                                 }                                             \
                                 *buffer_ptr = '\0';                           \
                                 final_length = buffer_ptr - buffer + 1;       \
                                 buffer_copy = xmalloc(final_length);          \
                                 memcpy64(buffer_copy, buffer, final_length);  \
                                 return buffer_copy;                           \
                               }
#endif
#endif
