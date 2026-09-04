// SPDX-License-Identifier: Apache-2.0 OR BSL-1.0
// Copyright (c) 2018 Ulf Adams

#if !defined(CBASE_RYU_H)
#define CBASE_RYU_H

#include "ryu/ryu.h"

#endif /* CBASE_RYU_H */

#if defined(CBASE_IMPLEMENT) && !defined(CBASE_RYU_IMPLEMENTED)
#define CBASE_RYU_IMPLEMENTED 1

#define to_chars ryu_d2s_to_chars
#include "ryu/d2s.c"
#undef to_chars

#define to_chars ryu_f2s_to_chars
#include "ryu/f2s.c"
#undef to_chars

#include "ryu/d2fixed.c"

#endif /* CBASE_IMPLEMENT && !CBASE_RYU_IMPLEMENTED */
