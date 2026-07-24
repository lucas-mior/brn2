#if !defined(ASSERT_MACROS_H)
#define ASSERT_MACROS_H

#define ASSERT(C) do {                                     \
    if (!(C)) {                                            \
        if (!DEBUGGING) {                                  \
            UNREACHABLE();                       \
        } else {                                           \
            error2("%s: Assertion '%s' failed at %s:%d\n", \
                   __func__, #C, __FILE__, __LINE__);      \
            TRAP();                                        \
        }                                                  \
    }                                                      \
} while (0)

#define ASSERT_FILE_CONTAINS(PATH, NEEDLE) \
    assert_file_contains(__FILE__, __LINE__, FUNC__, \
                         PATH, NEEDLE)

#define ASSERT_CONTAINS(HAYSTACK, HAYSTACK_LEN, NEEDLE) \
    assert_contains(__FILE__, __LINE__, FUNC__, \
                    HAYSTACK, HAYSTACK_LEN, NEEDLE)

#define ASSERT_NOT_CONTAINS(HAYSTACK, HAYSTACK_LEN, NEEDLE) \
    assert_not_contains(__FILE__, __LINE__, FUNC__, \
                        HAYSTACK, HAYSTACK_LEN, NEEDLE)

#define ASSERT_FP_KIND_NONE    0
#define ASSERT_FP_KIND_FLOAT   1
#define ASSERT_FP_KIND_DOUBLE  2

#define ASSERT_FP_KIND_EXPR(VAR) \
_Generic((VAR), \
    float:  ASSERT_FP_KIND_FLOAT,  \
    double: ASSERT_FP_KIND_DOUBLE, \
    default: ASSERT_FP_KIND_NONE   \
)

#if !defined(ASSERT_FLOAT_REL_TOL)
#define ASSERT_FLOAT_REL_TOL   (16.0*(double)FLT_EPSILON)
#endif
#if !defined(ASSERT_DOUBLE_REL_TOL)
#define ASSERT_DOUBLE_REL_TOL  (16.0*(double)DBL_EPSILON)
#endif

#if !defined(ASSERT_FLOAT_ABS_TOL)
#define ASSERT_FLOAT_ABS_TOL   (16.0*(double)FLT_EPSILON)
#endif
#if !defined(ASSERT_DOUBLE_ABS_TOL)
#define ASSERT_DOUBLE_ABS_TOL  (16.0*(double)DBL_EPSILON)
#endif

#define A_BOTH_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2)             \
    a_both_signed_##MODE(__FILE__, __LINE__, FUNC__,    \
                         #VAR1, #VAR2,                            \
                         typename(TYPE1), typename(TYPE2),        \
                         typebits(TYPE1), typebits(TYPE2),        \
                         (llong)(VAR1), (llong)(VAR2))

#define A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2)         \
    a_signed_unsigned##MODE(__FILE__, __LINE__, FUNC__, \
                            #VAR1, #VAR2,                         \
                            typename(TYPE1), typename(TYPE2),     \
                            typebits(TYPE1), typebits(TYPE2),     \
                            (llong)(VAR1), (ullong)(VAR2))

#if CHAR_MIN < 0
#define A_CHAR_FOR_SIGNED(MODE, VAR1, VAR2, TYPE1) \
    A_BOTH_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR)

#define A_CHAR_FOR_UNSIGNED(MODE, VAR1, VAR2, TYPE1) \
    A_UNSIGNED_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR)

#define A_FIRST_CHAR(MODE, VAR1, VAR2) \
    A_FIRST_SIGNED(MODE, VAR1, VAR2, TYPE_CHAR)
#else
#define A_CHAR_FOR_SIGNED(MODE, VAR1, VAR2, TYPE1) \
    A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR)

#define A_CHAR_FOR_UNSIGNED(MODE, VAR1, VAR2, TYPE1) \
    A_BOTH_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR)

#define A_FIRST_CHAR(MODE, VAR1, VAR2) \
    A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_CHAR)
#endif

#define A_FIRST_SIGNED(MODE, VAR1, VAR2, TYPE1)                        \
_Generic((VAR2),                                                       \
    char:    A_CHAR_FOR_SIGNED(MODE, VAR1, VAR2, TYPE1),               \
    schar:   A_BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_SCHAR  ), \
    short:   A_BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_SHORT  ), \
    int:     A_BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_INT    ), \
    long:    A_BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_LONG   ), \
    llong:   A_BOTH_SIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_LLONG  ), \
    uchar:   A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_UCHAR  ), \
    ushort:  A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_USHORT ), \
    uint:    A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_UINT   ), \
    ulong:   A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_ULONG  ), \
    ullong:  A_SIGNED_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE_ULLONG ), \
    float:   A_BOTH_DOUBLE(MODE,    VAR1, VAR2, TYPE1, TYPE_FLOAT  ),  \
    double:  A_BOTH_DOUBLE(MODE,    VAR1, VAR2, TYPE1, TYPE_DOUBLE ),  \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_A_FIRST_SIGNED()             \
)
#define A_BOTH_UNSIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2)             \
    a_both_unsigned_##MODE(__FILE__, __LINE__, FUNC__,    \
                           #VAR1, #VAR2,                            \
                           typename(TYPE1), typename(TYPE2),        \
                           typebits(TYPE1), typebits(TYPE2),        \
                           (ullong)(VAR1), (ullong)(VAR2))

#define A_UNSIGNED_SIGNED(MODE, VAR1, VAR2, TYPE1, TYPE2)           \
    a_unsigned_signed_##MODE(__FILE__, __LINE__, FUNC__,  \
                             #VAR1, #VAR2,                          \
                             typename(TYPE1), typename(TYPE2),      \
                             typebits(TYPE1), typebits(TYPE2),      \
                             (ullong)(VAR1), (llong)(VAR2))

#define A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE1)                        \
_Generic((VAR2),                                                         \
    char:    A_CHAR_FOR_UNSIGNED(MODE, VAR1, VAR2, TYPE1),               \
    schar:   A_UNSIGNED_SIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_SCHAR  ), \
    short:   A_UNSIGNED_SIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_SHORT  ), \
    int:     A_UNSIGNED_SIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_INT    ), \
    long:    A_UNSIGNED_SIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_LONG   ), \
    llong:   A_UNSIGNED_SIGNED(MODE,   VAR1, VAR2, TYPE1, TYPE_LLONG  ), \
    uchar:   A_BOTH_UNSIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_UCHAR  ), \
    ushort:  A_BOTH_UNSIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_USHORT ), \
    uint:    A_BOTH_UNSIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_UINT   ), \
    ulong:   A_BOTH_UNSIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_ULONG  ), \
    ullong:  A_BOTH_UNSIGNED(MODE,     VAR1, VAR2, TYPE1, TYPE_ULLONG ), \
    float:   A_BOTH_DOUBLE(MODE,       VAR1, VAR2, TYPE1, TYPE_FLOAT  ), \
    double:  A_BOTH_DOUBLE(MODE,       VAR1, VAR2, TYPE1, TYPE_DOUBLE ), \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_A_FIRST_UNSIGNED()             \
)
#define A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE2)                      \
    a_double_##MODE(__FILE__, __LINE__, FUNC__,                  \
                     #VAR1, #VAR2,                                         \
                     typename(TYPE1), typename(TYPE2),                     \
                     typebits(TYPE1), typebits(TYPE2),                     \
                     ASSERT_FP_KIND_EXPR(VAR1), ASSERT_FP_KIND_EXPR(VAR2), \
                     DOUBLE_GET2(VAR1, TYPE1), DOUBLE_GET2(VAR2, TYPE2))

#define A_FIRST_DOUBLE(MODE, VAR1, VAR2, TYPE1) \
_Generic((VAR2), \
    char:    A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_CHAR   ),     \
    schar:   A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_SCHAR  ),     \
    short:   A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_SHORT  ),     \
    int:     A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_INT    ),     \
    long:    A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_LONG   ),     \
    llong:   A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_LLONG  ),     \
    uchar:   A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_UCHAR  ),     \
    ushort:  A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_USHORT ),     \
    uint:    A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_UINT   ),     \
    ulong:   A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_ULONG  ),     \
    ullong:  A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_ULLONG ),     \
    float:   A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_FLOAT  ),     \
    double:  A_BOTH_DOUBLE(MODE, VAR1, VAR2, TYPE1, TYPE_DOUBLE ),     \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_A_FIRST_DOUBLE()             \
)
#define A_FIRST_BOOL(MODE, VAR1, VAR2, TYPE1)                  \
_Generic((VAR2),                                               \
    bool: a_bool_##MODE(__FILE__, __LINE__, FUNC__,  \
                        #VAR1, #VAR2,                          \
                        typename(TYPE1), typename(TYPE_BOOL),  \
                        typebits(TYPE1), typebits(TYPE_BOOL),  \
                        (VAR1), (VAR2)),                       \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_A_FIRST_BOOL()       \
)
#define A_POINTERS(MODE, VAR1, VAR2)                           \
    a_pointers_##MODE(__FILE__, __LINE__, FUNC__,    \
                      #VAR1, #VAR2,                            \
                      (void *)(uintptr_t)(VAR1),               \
                      (void *)(uintptr_t)(VAR2))

#define ASSERT_COMPARE(MODE, VAR1, VAR2)                                \
_Generic((VAR1),                                                        \
    void *: _Generic((VAR2),                                            \
        char *: A_POINTERS(MODE, VAR1, VAR2),                           \
        void *: A_POINTERS(MODE, VAR1, VAR2),                           \
        default: UNSUPPORTED_TYPE_FOR_GENERIC_ASSERT_COMPARE_VOIDP()    \
    ),                                                                  \
    char *: _Generic((VAR2),                                            \
        char *: a_strings_##MODE(__FILE__, __LINE__, FUNC__,  \
                                 #VAR1, #VAR2,                          \
                                 (char *)(uintptr_t)(VAR1),             \
                                 (char *)(uintptr_t)(VAR2)),            \
        void *: A_POINTERS(MODE, VAR1, VAR2),                           \
        default: UNSUPPORTED_TYPE_FOR_GENERIC_ASSERT_COMPARE_CHARP()    \
    ),                                                                  \
    char:    A_FIRST_CHAR(MODE,     VAR1, VAR2),                        \
    schar:   A_FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_SCHAR  ),          \
    short:   A_FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_SHORT  ),          \
    int:     A_FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_INT    ),          \
    long:    A_FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_LONG   ),          \
    llong:   A_FIRST_SIGNED(MODE,   VAR1, VAR2, TYPE_LLONG  ),          \
    uchar:   A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_UCHAR  ),          \
    ushort:  A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_USHORT ),          \
    uint:    A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_UINT   ),          \
    ulong:   A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_ULONG  ),          \
    ullong:  A_FIRST_UNSIGNED(MODE, VAR1, VAR2, TYPE_ULLONG ),          \
    float:   A_FIRST_DOUBLE(MODE,   VAR1, VAR2, TYPE_FLOAT  ),          \
    double:  A_FIRST_DOUBLE(MODE,   VAR1, VAR2, TYPE_DOUBLE ),          \
    bool:    A_FIRST_BOOL(MODE,     VAR1, VAR2, TYPE_BOOL),             \
    default: UNSUPPORTED_TYPE_FOR_GENERIC_ASSERT_COMPARE()              \
)

#define ASSERT_EQUAL(VAR1, VAR2)      ASSERT_COMPARE(equal,      VAR1, VAR2)
#define ASSERT_NOT_EQUAL(VAR1, VAR2)  ASSERT_COMPARE(not_equal,  VAR1, VAR2)
#define ASSERT_LESS(VAR1, VAR2)       ASSERT_COMPARE(less,       VAR1, VAR2)
#define ASSERT_LESS_EQUAL(VAR1, VAR2) ASSERT_COMPARE(less_equal, VAR1, VAR2)
#define ASSERT_MORE(VAR1, VAR2)       ASSERT_COMPARE(more,       VAR1, VAR2)
#define ASSERT_MORE_EQUAL(VAR1, VAR2) ASSERT_COMPARE(more_equal, VAR1, VAR2)

#define ASSERT_NULL(VAR1) do {                                          \
    void *p = VAR1;                                                     \
    if (p != NULL) {                                                    \
        error2("\nAssertion failed at %s:%d:%s\n",                      \
               __FILE__, __LINE__, __func__);                           \
        error2("%s = %p == NULL\n", #VAR1, p);                          \
        TRAP();                                                         \
    }                                                                   \
} while (0)

#endif /* ASSERT_MACROS_H */
