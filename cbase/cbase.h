#if !defined(CBASE_H)
#define CBASE_H

#include "cbase_config.h"

#if !defined(DEBUGGING)
#define DEBUGGING 0
#endif

#if !defined(RELEASING)
#define RELEASING 0
#endif

#if !defined(ERROR_NOTIFY)
#define ERROR_NOTIFY 0
#endif

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "platform_detection.h"
#include "primitives.h"
#include "base_macros.h"
#include "i18n.h"
#include "memory.h"
#include "generic.h"
#include "minmax.h"
#include "cbase_assert.h"
#include "arena.h"
#include "utf8.h"
#include "util.h"
#include "command.h"
#include "sort.h"
#include "array.h"
#include "meta.h"

#endif /* CBASE_H */

#if defined(CBASE_IMPLEMENT) && !defined(CBASE_IMPLEMENTED)
#define CBASE_IMPLEMENTED 1

#include "generic.c"
#include "assert.c"
#include "minmax.c"
#include "arena.c"
#include "memory.c"
#include "utf8.c"
#include "util.c"
#include "command.c"
#include "sort.c"
#include "array.c"
#include "meta_common.c"
#include "meta_tokenize.c"
#include "meta_parse.c"
#include "meta_generate.c"

#endif /* CBASE_IMPLEMENT && !CBASE_IMPLEMENTED */
