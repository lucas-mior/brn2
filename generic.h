#if !defined(GENERIC_H)
#define GENERIC_H

#define TYPENAME(VAR) \
_Generic((VAR), \
  void *:  "void*",   \
  char *:  "char*",   \
  schar:   "schar",   \
  short:   "short",   \
  int:     "int",     \
  long:    "long",    \
  llong:   "llong",   \
  uchar:   "uchar",   \
  ushort:  "ushort",  \
  uint:    "uint",    \
  ulong:   "ulong",   \
  ullong:  "ullong",  \
  ldouble: "ldouble", \
  double:  "double",  \
  float:   "float",   \
  default: _Generic((VAR), \
    int8:    "int8",   \
    int16:   "int16",  \
    int32:   "int32",  \
    int64:   "int64",  \
    uint8:   "uint8",  \
    uint16:  "uint16", \
    uint32:  "uint32", \
    uint64:  "uint64", \
    default: unsupported_type_for_generic() \
  ) \
)

#define MINOF(VARIABLE) \
_Generic((VARIABLE), \
  schar:       SCHAR_MIN, \
  short:       SHRT_MIN,  \
  int:         INT_MIN,   \
  long:        LONG_MIN,  \
  llong:       LLONG_MIN, \
  uchar:       0,         \
  ushort:      0,         \
  uint:        0u,        \
  ulong:       0ul,       \
  ullong:      0ull,      \
  char:        CHAR_MIN,  \
  bool:        0,         \
  float:       -FLT_MAX,  \
  double:      -DBL_MAX,  \
  long double: -LDBL_MAX,  \
  default: _Generic((VARIABLE), \
    int8:      INT8_MIN, \
    int16:     INT16_MIN, \
    int32:     INT32_MIN, \
    int64:     INT64_MIN, \
    uint8:     (uint8)0, \
    uint16:    (uint16)0, \
    uint32:    (uint32)0, \
    uint64:    (uint64)0, \
    default:   unsupported_type_for_generic() \
  ) \
)

#define MAXOF(VARIABLE) \
_Generic((VARIABLE), \
  schar:       SCHAR_MAX,  \
  short:       SHRT_MAX,   \
  int:         INT_MAX,    \
  long:        LONG_MAX,   \
  llong:       LLONG_MAX,  \
  uchar:       UCHAR_MAX,  \
  ushort:      USHRT_MAX,  \
  uint:        UINT_MAX,   \
  ulong:       ULONG_MAX,  \
  ullong:      ULLONG_MAX, \
  char:        CHAR_MAX,   \
  bool:        1,          \
  float:       FLT_MAX,    \
  double:      DBL_MAX,    \
  long double: LDBL_MAX,    \
  default: _Generic((VARIABLE), \
    int8:      INT8_MAX,   \
    int16:     INT16_MAX,  \
    int32:     INT32_MAX,  \
    int64:     INT64_MAX,  \
    uint8:     UINT8_MAX,  \
    uint16:    UINT16_MAX, \
    uint32:    UINT32_MAX, \
    uint64:    UINT64_MAX, \
    default:   unsupported_type_for_generic() \
  ) \
)

#endif
