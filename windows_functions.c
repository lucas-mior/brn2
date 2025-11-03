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
#error "ONLY INCLUDE THIS FILE IF COMPILING FOR WINDOWS"
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
scandir(const char *dir, struct dirent ***namelist,
        int (*filter)(const struct dirent *),
        int (*compar)(const struct dirent **, const struct dirent **)) {
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
    if ((haystack == NULL) || (needle == NULL)) {
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

static time_t
filetime_to_time_t(const FILETIME *ft) {
    ULARGE_INTEGER ull;
    ull.LowPart = ft->dwLowDateTime;
    ull.HighPart = ft->dwHighDateTime;
    // Convert from Windows epoch (1601) to Unix epoch (1970)
    return (time_t)((ull.QuadPart - 116444736000000000ULL) / 10000000ULL);
}

static int
lstat(const char *path, struct stat *statbuf) {
    if (path == NULL || statbuf == NULL) {
        fprintf(stderr, "lstat: Invalid argument.\n");
        return -1;
    }

    wchar_t wpath[MAX_PATH];
    if (MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, MAX_PATH) == 0) {
        return -1;
    }

    WIN32_FILE_ATTRIBUTE_DATA fd;
    if (!GetFileAttributesExW(wpath, GetFileExInfoStandard, &fd)) {
        return -1;
    }

    memset(statbuf, 0, sizeof(*statbuf));

    // File type
    if (fd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
        statbuf->st_mode = S_IFLNK;
    } else if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        statbuf->st_mode = S_IFDIR;
    } else {
        statbuf->st_mode = S_IFREG;
    }

    // File size
    ULARGE_INTEGER sz;
    sz.LowPart = fd.nFileSizeLow;
    sz.HighPart = fd.nFileSizeHigh;
    statbuf->st_size = sz.QuadPart;

    // Convert FILETIME -> time_t
    statbuf->st_mtime = filetime_to_time_t(&fd.ftLastWriteTime);
    statbuf->st_ctime = filetime_to_time_t(&fd.ftCreationTime);
    statbuf->st_atime = filetime_to_time_t(&fd.ftLastAccessTime);

    return 0;
}

#if TESTING_windows_functions
#include <assert.h>

bool
contains(char *buffer, int64 length, struct dirent **dirent, int32 *nfiles) {
    for (int32 i = 0; i < *nfiles; i += 1) {
        char *from_scan = dirent[i]->d_name;

        if ((int64)strlen(from_scan) != length) {
            continue;
        }

        if (!memcmp(buffer, from_scan, length)) {
            printf("%s == %s\n", buffer, from_scan);
            if (i < (*nfiles - 1)) {
                *nfiles -= 1;
                memmove(&dirent[i], &dirent[i + 1],
                        (*nfiles - i)*sizeof(*(dirent)));
            }
            return true;
        }
    }
    return false;
}

int
main(void) {
    char *string = "aaa/bbb/ccc";
    int64 length = strlen(string);
    struct stat stat;
    struct dirent **dirent;
    int32 nfiles;

    assert(memmem(string, length, "aaa", 3) == string);
    assert(memmem(string, length, "bbb", 3) == string + 4);
    assert(memmem(string, length, "aaaa", 4) == NULL);
    assert(memmem(string, length, "bbbb", 4) == NULL);
    assert(memmem(string, length, "/", 1) == string + 3);

    assert(lstat("LICENSE", &stat) == 0);
    assert(stat.st_size == 34523);
    assert(stat.st_mtime == 1735689600);
    assert(stat.st_ctime == 1735689600);
    error("stat.atime: %lu\n", stat.st_atime);

    if ((nfiles = scandir("./", &dirent, NULL, NULL)) <= 0) {
        error("Error in scandir for windows.\n");
        exit(EXIT_FAILURE);
    }

    {
        FILE *ls_pipe;
        char buffer[1024];
        if ((ls_pipe = popen("dir /b", "r")) == NULL) {
            error("Error in popen: %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        while (fgets(buffer, sizeof(buffer), ls_pipe)) {
            int64 length = strcspn(buffer, "\n");
            buffer[length] = '\0';
            assert(contains(buffer, length, dirent, &nfiles));
        }
    }

    exit(EXIT_SUCCESS);
}
#endif

#endif
