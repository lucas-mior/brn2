// SPDX-License-Identifier: AGPL
// Copyright (c) 2026 Lucas Mior

#if !defined(BRN2_DIRECTORY_ENTRY_H)
#define BRN2_DIRECTORY_ENTRY_H

#include "cbase.h"

typedef struct Brn2DirectoryEntry {
    int32 name_length;
    char name[];
} Brn2DirectoryEntry;

INLINE int64
brn2_directory_entry_size(int32 name_length) {
    return SIZEOF(Brn2DirectoryEntry) + (int64)name_length + 1;
}

#endif /* BRN2_DIRECTORY_ENTRY_H */
