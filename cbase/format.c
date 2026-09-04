#if !defined(FORMAT_C)
#define FORMAT_C

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_format 1
#elif !defined(TESTING_fs)
#define TESTING_format 0
#endif

#include "cbase.h"

#if TESTING_format
#define CBASE_IMPLEMENT
#include "cbase.h"

int
main(void) {
    exit(EXIT_SUCCESS);
}
#endif

#endif /* FORMAT_C */
