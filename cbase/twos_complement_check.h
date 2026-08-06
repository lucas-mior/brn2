#include "base_macros.h"
#include "libc.h"

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TWOS_TYPE SHRT
#define TWOS_MIN  -32768ll
#endif

#if !defined(TWOS_TYPE)
#error "TWOS_TYPE is not defined."
#endif

#if CAT(TWOS_TYPE, _MIN) != TWOS_MIN
#error "This compiler/machine does not use two's complement for integers. Throw it out."
#endif

#undef TWOS_TYPE
#undef TWOS_MIN
