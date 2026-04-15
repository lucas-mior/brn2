#if defined(__linux__)
#define OS_LINUX 1
#define OS_MAC 0
#define OS_BSD 0
#define OS_WINDOWS 0
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_LINUX 0
#define OS_MAC 1
#define OS_BSD 0
#define OS_WINDOWS 0
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#define OS_LINUX 0
#define OS_MAC 0
#define OS_BSD 1
#define OS_WINDOWS 0
#elif defined(_WIN32) || defined(_WIN64)
#define OS_LINUX 0
#define OS_MAC 0
#define OS_BSD 0
#define OS_WINDOWS 1
#else
#error "Unsupported OS.\n"
#endif

#define OS_UNIX (OS_LINUX || OS_MAC || OS_BSD)

#if defined(__GNUC__)
#define COMPILER_GCC 1
#define COMPILER_CLANG 0
#elif defined(__clang__)
#define COMPILER_GCC 0
#define COMPILER_CLANG 1
#else
#define COMPILER_GCC 0
#define COMPILER_CLANG 0
#endif
