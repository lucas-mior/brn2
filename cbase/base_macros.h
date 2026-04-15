#if !defined(BASE_MACROS_H)
#define BASE_MACROS_H

#define QUOTE_(x) #x
#define QUOTE(x) QUOTE_(x)

#define CAT_(a, b)     a##b
#define CAT3_(a, b, c) a##b##c
#define CAT(a, b)      CAT_(a, b)
#define CAT3(a, b, c)  CAT3_(a, b, c)

#define NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, n, ...) n
#define NUM_ARGS(...) NUM_ARGS_(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, x)
#define SELECT_ON_NUM_ARGS(macro, ...) \
  CAT(macro, NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

#endif /* BASE_MACROS_H */
