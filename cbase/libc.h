// SPDX-License-Identifier: AGPL
// Copyright (c) 2026 Lucas Mior

// Note: all libc or main platform headers must be included here.
// other files include them by `#include "cbase.h"` or `#include "libc.h"`
// Avoid including system headers in other files.

#if !defined(LIBC_H)
#define LIBC_H

#include "platform_detection.h"

#if CC_CLANG
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wreserved-identifier"
#endif

#if OS_UNIX && !defined(_XOPEN_SOURCE)
  #define _XOPEN_SOURCE 700
#endif

#if OS_UNIX && !defined(_DEFAULT_SOURCE)
  #define _DEFAULT_SOURCE
#endif

#if OS_LINUX && !defined(_GNU_SOURCE)
  #define _GNU_SOURCE
#endif

#if OS_NETBSD && !defined(_NETBSD_SOURCE)
  #define _NETBSD_SOURCE
#endif

#if OS_OPENBSD && !defined(_BSD_SOURCE)
  #define _BSD_SOURCE
#endif

#if CC_CLANG
  #pragma clang diagnostic pop
#endif

#if defined(__has_include)
#define HAS_INCLUDE(header) __has_include(header)
#else
#define HAS_INCLUDE(header) 1
#endif

// mandatory C11 headers
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#if !defined(_MSC_VER)
#include <tgmath.h>
#endif
#include <time.h>
#include <wchar.h>
#include <wctype.h>

// optional C11 headers
#if !defined(_MSC_VER) && !defined(__STDC_NO_COMPLEX__)
#include <complex.h>
#endif

#if !OS_WINDOWS && !defined(__STDC_NO_THREADS__)
#include <threads.h>
#endif

#if !defined(__STDC_NO_ATOMICS__)
#include <stdatomic.h>
#endif

// POSIX-like headers provided by Unix and some Windows CRTs.
#if CBASE_HAS_DIRENT_H
#include <dirent.h>
#endif
#if CBASE_HAS_FCNTL_H
#include <fcntl.h>
#endif
#if CBASE_HAS_FTW_H
#include <ftw.h>
#endif
#if CBASE_HAS_GETOPT_H
#include <getopt.h>
#endif
#if CBASE_HAS_LIBGEN_H
#include <libgen.h>
#endif
#if CBASE_HAS_SYS_FILE_H
#include <sys/file.h>
#endif
#if CBASE_HAS_SYS_STAT_H
#include <sys/stat.h>
#endif
#if CBASE_HAS_SYS_TIME_H
#include <sys/time.h>
#endif
#if CBASE_HAS_SYS_TYPES_H
#include <sys/types.h>
#endif
#if CBASE_HAS_UNISTD_H
#include <unistd.h>
#endif

#if OS_WINDOWS
#include <windows.h>
#endif

// POSIX headers
#if OS_UNIX
#define TRY_INCLUDE_WHICH <arpa/inet.h>
#include "try_include.h"
#define TRY_INCLUDE_WHICH <fnmatch.h>
#include "try_include.h"
#define TRY_INCLUDE_WHICH <glob.h>
#include "try_include.h"
#define TRY_INCLUDE_WHICH <grp.h>
#include "try_include.h"
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#define TRY_INCLUDE_WHICH <pwd.h>
#include "try_include.h"
#define TRY_INCLUDE_WHICH <regex.h>
#include "try_include.h"
#define TRY_INCLUDE_WHICH <spawn.h>
#include "try_include.h"
#define TRY_INCLUDE_WHICH <strings.h>
#include "try_include.h"
#include <sys/ioctl.h>
#include <sys/mman.h>
#define TRY_INCLUDE_WHICH <sys/resource.h>
#include "try_include.h"
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#define TRY_INCLUDE_WHICH <sys/uio.h>
#include "try_include.h"
#define TRY_INCLUDE_WHICH <sys/utsname.h>
#include "try_include.h"
#include <sys/wait.h>
#define TRY_INCLUDE_WHICH <termios.h>
#include "try_include.h"
#include <utime.h>
#define TRY_INCLUDE_WHICH <wordexp.h>
#include "try_include.h"
#define TRY_INCLUDE_WHICH <fts.h>
#include "try_include.h"
#endif

#if OS_MAC || OS_BSD
#include <sys/param.h>
#include <sys/sysctl.h>
#endif

#if OS_LINUX
#include <sys/syscall.h>
#endif

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

#if !defined(CBASE_DIRENT_HAS_D_TYPE)
#if defined(DT_DIR)
#define CBASE_DIRENT_HAS_D_TYPE 1
#else
#define CBASE_DIRENT_HAS_D_TYPE 0
#endif
#endif

#if !defined(CBASE_HAS_F_GETPATH)
#if defined(F_GETPATH)
#define CBASE_HAS_F_GETPATH 1
#else
#define CBASE_HAS_F_GETPATH 0
#endif
#endif

#if !defined(CBASE_HAS_SYSTEM_MEMMEM)
#if defined(__GLIBC__) && defined(_GNU_SOURCE)
#define CBASE_HAS_SYSTEM_MEMMEM 1
#else
#define CBASE_HAS_SYSTEM_MEMMEM 0
#endif
#endif

#if !defined(FLAGS_HUGE_PAGES)
#if defined(MAP_HUGETLB) && defined(MAP_HUGE_2MB)
#define FLAGS_HUGE_PAGES (MAP_HUGETLB | MAP_HUGE_2MB)
#else
#define FLAGS_HUGE_PAGES 0
#endif
#endif

#if !defined(MAP_POPULATE)
#define MAP_POPULATE 0
#endif

#if OS_UNIX
#if !defined(MAP_ANON) && defined(MAP_ANONYMOUS)
#define MAP_ANON MAP_ANONYMOUS
#elif !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
#define MAP_ANONYMOUS MAP_ANON
#elif OS_FREEBSD
#define MAP_ANON 0x1000
#define MAP_ANONYMOUS MAP_ANON
#elif !defined(MAP_ANONYMOUS) && !defined(MAP_ANON)
#error "Anonymous mmap is unsupported on this platform"
#endif
#endif

#endif /* LIBC_H */
