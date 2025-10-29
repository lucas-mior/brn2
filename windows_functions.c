/*
 * Copyright (C) 2025 Mior, Lucas;
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(WINDOWS_FUNCTIONS_C)
#define WINDOWS_FUNCTIONS_C

#include "util.c"

#if !OS_WINDOWS
#error ONLY INCLUDE THIS FILE IF COMPILING FOR WINDOWS
#endif

#include <windows.h>
#include <dirent.h>
#include <sys/stat.h>
#include <wchar.h>

#ifndef S_IFLNK
#define S_IFLNK 0120000
#endif

#if defined(__INCLUDE_LEVEL__) && __INCLUDE_LEVEL__ == 0
#define TESTING_windows_functions 1
#elif !defined(TESTING_windows_functions)
#define TESTING_windows_functions 0
#endif

int
scandir(const char *dir, struct dirent ***namelist, void *filter,
        void *compar) {
    WIN32_FIND_DATAA find_data;
    HANDLE hFind;
    char buffer[MAX_PATH];
    struct dirent **list;
    int64 count = 0;
    int64 capacity = 16;
    (void)filter;
    (void)compar;

    SNPRINTF(buffer, "%s/*", dir);

    if ((hFind = FindFirstFileA(buffer, &find_data)) == INVALID_HANDLE_VALUE) {
        return -1;
    }

    list = xmalloc(capacity*sizeof(*list));
    do {
        struct dirent *ent = xmalloc(sizeof(*ent));
        int64 length = strlen(find_data.cFileName);

        if (length > (SIZEOF(ent->d_name) - 1)) {
            error("Error scanning file %s. File name is too long.\n",
                  find_data.cFileName);
            fatal(EXIT_FAILURE);
        }

        memcpy(ent->d_name, find_data.cFileName, length + 1);

        if (count >= capacity) {
            capacity *= 2;
            list = xrealloc(list, capacity*sizeof(*list));
        }
        list[count] = ent;
        count += 1;
    } while (FindNextFileA(hFind, &find_data));
    FindClose(hFind);

    *namelist = list;
    return (int)count;
}

static void *
memmem(const void *haystack, size_t hay_len, const void *needle,
       size_t needle_len) {
    const uchar *h = haystack;
    const uchar *n = needle;
    const uchar *end = h + hay_len;
    const uchar *limit = end - needle_len + 1;

    if (needle_len == 0) {
        return (void *)haystack;
    }
    if (!haystack || !needle) {
        return NULL;
    }
    if (hay_len < needle_len) {
        return NULL;
    }

    while (h < limit) {
        const uchar *p;

        if ((p = memchr(h, n[0], (size_t)(limit - h))) == NULL) {
            return NULL;
        }

        if (memcmp(p, n, needle_len) == 0) {
            return (void *)p;
        }
        h = p + 1;
    }

    return NULL;
}

static int
lstat(const char *path, struct stat *stat) {
    wchar_t wpath[MAX_PATH];
    WIN32_FIND_DATAW fd;
    LARGE_INTEGER sz;
    ULARGE_INTEGER ull;
    HANDLE h;

    if (path == NULL) {
        error("lstat: Invalid path.\n");
        return -1;
    }
    if (stat == NULL) {
        error("lstat: Invalid stat pointer.\n");
        return -1;
    }

    if (MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, MAX_PATH) == 0) {
        return -1;
    }

    if ((h = FindFirstFileW(wpath, &fd)) == INVALID_HANDLE_VALUE) {
        return -1;
    }
    FindClose(h);

    memset(stat, 0, sizeof(*stat));

    if (fd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
        stat->st_mode = S_IFLNK;
    } else if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        stat->st_mode = S_IFDIR;
    } else {
        stat->st_mode = S_IFREG;
    }

    sz.HighPart = fd.nFileSizeHigh;
    sz.LowPart = fd.nFileSizeLow;
    stat->st_size = sz.QuadPart;

    ull.LowPart = fd.ftLastWriteTime.dwLowDateTime;
    ull.HighPart = fd.ftLastWriteTime.dwHighDateTime;
    stat->st_mtime
        = (time_t)((ull.QuadPart - 116444736000000000ULL) / 10000000ULL);

    ull.LowPart = fd.ftCreationTime.dwLowDateTime;
    ull.HighPart = fd.ftCreationTime.dwHighDateTime;
    stat->st_ctime
        = (time_t)((ull.QuadPart - 116444736000000000ULL) / 10000000ULL);

    ull.LowPart = fd.ftLastAccessTime.dwLowDateTime;
    ull.HighPart = fd.ftLastAccessTime.dwHighDateTime;
    stat->st_atime
        = (time_t)((ull.QuadPart - 116444736000000000ULL) / 10000000ULL);

    return 0;
}

#if TESTING_windows_functions
#include <assert.h>
int
main(void) {
    assert(true);
    exit(EXIT_SUCCESS);
}
#endif

#endif
