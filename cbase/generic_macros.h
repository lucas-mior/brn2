#if !defined(GENERIC_MACROS_H)
#define GENERIC_MACROS_H

#if !defined(GENERIC_S_BSZ)
#define GENERIC_S_BSZ 64
#endif

#if !defined(S_BSZ)
#define S_BSZ GENERIC_S_BSZ
#endif

#if !defined(MACRO_NAME)
#define MACRO_NAME(X) #X
#endif
#if !defined(MACRO_VALUE)
#define MACRO_VALUE(X) MACRO_NAME(X)
#endif

#define fprint(FP, ...) fprint_0((FP), __VA_ARGS__, (char *)0)
#define snprint(BUF, BSZ, ...) snprint_0((BUF), (BSZ), __VA_ARGS__, (char *)0)
#define print0(...) fprint_0(stdout, __VA_ARGS__, (char *)0)

#define S_(X) toString((char[S_BSZ]){ "" }, S_BSZ, _Generic((X), \
    void *: "%p", \
    char *: "%s", \
    bool: "%i", \
    char: "%c", \
    schar: "%hhi", \
    short: "%hi", \
    int: "%i", \
    long: "%li", \
    llong: "%lli", \
    uchar: "%hhu", \
    ushort: "%hu", \
    uint: "%u", \
    ulong: "%lu", \
    ullong: "%llu", \
    float: "%." MACRO_VALUE(FLT_DIG) "g", \
    double: "%." MACRO_VALUE(DBL_DIG) "g", \
    default: "%p" \
), (X))

#define V(X) "", S_(X), ""
#define W(X) "", (X), ""
#define SF(F, X) toString((char[S_BSZ]){ "" }, S_BSZ, (F), (X))
#define VF(F, X) "", SF((F), (X)), ""

#define TYPENAME(VAR)        \
_Generic((VAR),              \
    void*:   "void*",        \
    char*:   "char*",        \
    bool:    "bool",         \
    char:    "char",         \
    schar:   "schar",        \
    short:   "short",        \
    int:     "int",          \
    long:    "long",         \
    llong:   "llong",        \
    uchar:   "uchar",        \
    ushort:  "ushort",       \
    uint:    "uint",         \
    ulong:   "ulong",        \
    ullong:  "ullong",       \
    float:   "float",        \
    double:  "double",       \
    default: "unknown"       \
)

#define MINOF(VARIABLE)           \
_Generic((VARIABLE),              \
    schar:   SCHAR_MIN,           \
    short:   SHRT_MIN,            \
    int:     INT_MIN,             \
    long:    LONG_MIN,            \
    llong:   LLONG_MIN,           \
    uchar:   0,                   \
    ushort:  0,                   \
    uint:    0u,                  \
    ulong:   0ul,                 \
    ullong:  0ull,                \
    char:    CHAR_MIN,            \
    bool:    0,                   \
    float:   -FLT_MAX,            \
    double:  -DBL_MAX,            \
    default: 0                    \
)

#define MAXOF(VARIABLE)           \
_Generic((VARIABLE),              \
    schar:   SCHAR_MAX,           \
    short:   SHRT_MAX,            \
    int:     INT_MAX,             \
    long:    LONG_MAX,            \
    llong:   LLONG_MAX,           \
    uchar:   UCHAR_MAX,           \
    ushort:  USHRT_MAX,           \
    uint:    UINT_MAX,            \
    ulong:   ULONG_MAX,           \
    ullong:  ULLONG_MAX,          \
    char:    CHAR_MAX,            \
    bool:    1,                   \
    float:   FLT_MAX,             \
    double:  DBL_MAX,             \
    default: 1                    \
)

#define TYPEID(VAR) \
_Generic((VAR), \
    void*:   TYPE_VOIDP,       \
    char*:   TYPE_CHARP,       \
    bool:    TYPE_BOOL,        \
    char:    TYPE_CHAR,        \
    schar:   TYPE_SCHAR,       \
    short:   TYPE_SHORT,       \
    int:     TYPE_INT,         \
    long:    TYPE_LONG,        \
    llong:   TYPE_LLONG,       \
    uchar:   TYPE_UCHAR,       \
    ushort:  TYPE_USHORT,      \
    uint:    TYPE_UINT,        \
    ulong:   TYPE_ULONG,       \
    ullong:  TYPE_ULLONG,      \
    float:   TYPE_FLOAT,       \
    double:  TYPE_DOUBLE,      \
    default: TYPE_OTHER        \
)

#define TYPEBITS(VAR) (sizeof(VAR)*CHAR_BIT)

#define DOUBLE_GET(x) \
_Generic((x), \
    void*:   double_from_voidp,                                  \
    char*:   double_from_charp,                                  \
    bool:    double_from_bool,                                   \
    char:    double_from_char,                                   \
    schar:   double_from_schar,                                  \
    short:   double_from_short,                                  \
    int:     double_from_int,                                    \
    long:    double_from_long,                                   \
    llong:   double_from_llong,                                  \
    uchar:   double_from_uchar,                                  \
    ushort:  double_from_ushort,                                 \
    uint:    double_from_uint,                                   \
    ulong:   double_from_ulong,                                  \
    ullong:  double_from_ullong,                                 \
    float:   double_from_float,                                  \
    double:  double_from_double,                                 \
    default: UNSUPPORTED_TYPE_FOR_DOUBLE_GET_GENERIC            \
)(x)

#if defined(__GNUC__) || defined(__clang__)
#define DOUBLE_GET2(VAR, TYPE) double_get((union Primitive)(VAR), TYPE)
#else
#define DOUBLE_GET2(VAR, TYPE) DOUBLE_GET(VAR)
#endif

#define PRINT_SIGNED(VAR, TYPE) \
  fprintf(stderr, "["GREEN("%s%lld")"]%s = %lld ", \
                  typename(TYPE), typebits(TYPE), #VAR, (llong)(VAR))

#define PRINT_UNSIGNED(VAR, TYPE) \
  fprintf(stderr, "["GREEN("%s%lld")"]%s = %llu ", \
                  typename(TYPE), typebits(TYPE), #VAR, (ullong)(VAR))

#define PRINT_DOUBLE(VAR, TYPE) \
  fprintf(stderr, "["GREEN("%s%lld")"]%s = %f ", \
                  typename(TYPE), typebits(TYPE), #VAR, DOUBLE_GET2(VAR, TYPE))

#define PRINT_OTHER(VAR, TYPE, FORMAT, CAST) \
  fprintf(stderr, "["GREEN("%s%lld")"]%s = "FORMAT" ", \
                  typename(TYPE), typebits(TYPE), #VAR, (CAST)(uintptr_t)(VAR))

#define PRINT(VAR) \
_Generic((VAR), \
    void*:   PRINT_OTHER(VAR,    TYPE_VOIDP,   "%p",           void*), \
    char*:   PRINT_OTHER(VAR,    TYPE_CHARP,   RED("\"%s\""),  char*), \
    bool:    PRINT_OTHER(VAR,    TYPE_BOOL,    "%u",           bool),  \
    char:    PRINT_OTHER(VAR,    TYPE_CHAR,    YELLOW("'%c'"), char),  \
    schar:   PRINT_SIGNED(VAR,   TYPE_SCHAR),                          \
    short:   PRINT_SIGNED(VAR,   TYPE_SHORT),                          \
    int:     PRINT_SIGNED(VAR,   TYPE_INT),                            \
    long:    PRINT_SIGNED(VAR,   TYPE_LONG),                           \
    llong:   PRINT_SIGNED(VAR,   TYPE_LLONG),                          \
    uchar:   PRINT_UNSIGNED(VAR, TYPE_UCHAR),                          \
    ushort:  PRINT_UNSIGNED(VAR, TYPE_USHORT),                         \
    uint:    PRINT_UNSIGNED(VAR, TYPE_UINT),                           \
    ulong:   PRINT_UNSIGNED(VAR, TYPE_ULONG),                          \
    ullong:  PRINT_UNSIGNED(VAR, TYPE_ULLONG),                         \
    float:   PRINT_DOUBLE(VAR,  TYPE_FLOAT),                          \
    double:  PRINT_DOUBLE(VAR,  TYPE_DOUBLE),                         \
    default: 0                                                        \
)

#define PRINTLN(VAR) do { \
    fprintf(stderr, "%s:%d %s():", __FILE__, __LINE__, __func__); \
    PRINT(VAR); \
    fprintf(stderr, "\n"); \
} while (0)

#endif /* GENERIC_MACROS_H */
