/*
 * Copyright (C) 2025 Mior, Lucas;
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
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

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_windows_functions 1
#elif !defined(TESTING_windows_functions)
#define TESTING_windows_functions 0
#endif

#include "cbase.h"

#if !OS_WINDOWS
#error "ONLY INCLUDE THIS FILE IF COMPILING FOR WINDOWS"
#endif

#include <windows.h>
#include <dirent.h>
#include <sys/stat.h>
#include <wchar.h>

#if !defined(S_IFLNK)
#define S_IFLNK 0120000
#endif

static void
windows_set_errno(DWORD error_code) {
    switch (error_code) {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_INVALID_DRIVE:
    case ERROR_BAD_NETPATH:
    case ERROR_BAD_NET_NAME:
        errno = ENOENT;
        break;
    case ERROR_ACCESS_DENIED:
    case ERROR_NETWORK_ACCESS_DENIED:
    case ERROR_WRITE_PROTECT:
    case ERROR_SHARING_VIOLATION:
    case ERROR_LOCK_VIOLATION:
        errno = EACCES;
        break;
    case ERROR_FILE_EXISTS:
    case ERROR_ALREADY_EXISTS:
        errno = EEXIST;
        break;
    case ERROR_INVALID_PARAMETER:
    case ERROR_INVALID_NAME:
    case ERROR_BAD_PATHNAME:
        errno = EINVAL;
        break;
    case ERROR_NO_UNICODE_TRANSLATION:
        errno = EILSEQ;
        break;
    case ERROR_FILENAME_EXCED_RANGE:
        errno = ENAMETOOLONG;
        break;
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
        errno = ENOMEM;
        break;
    case ERROR_TOO_MANY_OPEN_FILES:
        errno = EMFILE;
        break;
    case ERROR_DISK_FULL:
    case ERROR_HANDLE_DISK_FULL:
        errno = ENOSPC;
        break;
    case ERROR_DIRECTORY:
        errno = ENOTDIR;
        break;
    case ERROR_DIR_NOT_EMPTY:
        errno = ENOTEMPTY;
        break;
    case ERROR_OPERATION_ABORTED:
        errno = EINTR;
        break;
    case ERROR_BROKEN_PIPE:
    case ERROR_NO_DATA:
        errno = EPIPE;
        break;
    default:
        errno = EIO;
        break;
    }
    return;
}

static void
scandir_list_free(struct dirent **list, int64 count, int64 capacity) {
    for (int64 i = 0; i < count; i += 1) {
        free2(list[i], SIZEOF(*list[i]));
    }
    free2(list, capacity*SIZEOF(*list));
    return;
}

static int
scandir(
    const char *dir,
    struct dirent ***namelist,
    int (*filter)(const struct dirent *),
    int (*compar)(const struct dirent **, const struct dirent **)
) {
    WIN32_FIND_DATAW find_data;
    HANDLE find_handle;
    wchar_t *wide_pattern;
    struct dirent **list;
    DWORD error_code;
    int64 pattern_capacity;
    int64 pattern_length;
    int64 count;
    int64 capacity = 16;
    int32 result;
    int32 wide_dir_length;
    (void)filter;
    (void)compar;

    if ((dir == NULL) || (namelist == NULL)) {
        errno = EINVAL;
        return -1;
    }

    wide_dir_length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                          dir, -1, NULL, 0);
    if (wide_dir_length <= 0) {
        windows_set_errno(GetLastError());
        return -1;
    }

    pattern_capacity = (int64)wide_dir_length + 2;
    wide_pattern = malloc2(pattern_capacity*SIZEOF(*wide_pattern));
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, dir, -1,
                            wide_pattern, wide_dir_length)
        != wide_dir_length) {
        error_code = GetLastError();
        free2(wide_pattern, pattern_capacity*SIZEOF(*wide_pattern));
        windows_set_errno(error_code);
        return -1;
    }

    pattern_length = (int64)wide_dir_length - 1;
    if ((pattern_length > 0)
        && (wide_pattern[pattern_length - 1] != L'/')
        && (wide_pattern[pattern_length - 1] != L'\\')) {
        wide_pattern[pattern_length++] = L'\\';
    }
    wide_pattern[pattern_length++] = L'*';
    wide_pattern[pattern_length] = L'\0';

    find_handle = FindFirstFileW(wide_pattern, &find_data);
    if (find_handle == INVALID_HANDLE_VALUE) {
        error_code = GetLastError();
        free2(wide_pattern, pattern_capacity*SIZEOF(*wide_pattern));
        windows_set_errno(error_code);
        return -1;
    }
    free2(wide_pattern, pattern_capacity*SIZEOF(*wide_pattern));

    count = 0;
    list = malloc2(capacity*SIZEOF(*list));
    while (true) {
        struct dirent *dir_entry;
        int32 utf8_length;

        utf8_length = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                                          find_data.cFileName, -1,
                                          NULL, 0, NULL, NULL);
        if (utf8_length <= 0) {
            error_code = GetLastError();
            FindClose(find_handle);
            scandir_list_free(list, count, capacity);
            windows_set_errno(error_code);
            return -1;
        }

        dir_entry = malloc2(SIZEOF(*dir_entry));
        if (utf8_length > SIZEOF(dir_entry->d_name)) {
            free2(dir_entry, SIZEOF(*dir_entry));
            FindClose(find_handle);
            scandir_list_free(list, count, capacity);
            errno = ENAMETOOLONG;
            return -1;
        }

        if (WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                                find_data.cFileName, -1, dir_entry->d_name,
                                utf8_length, NULL, NULL)
            != utf8_length) {
            error_code = GetLastError();
            free2(dir_entry, SIZEOF(*dir_entry));
            FindClose(find_handle);
            scandir_list_free(list, count, capacity);
            windows_set_errno(error_code);
            return -1;
        }

        if (count >= capacity) {
            int64 old_capacity = capacity;

            capacity *= 2;
            list = realloc2(list, old_capacity, capacity, SIZEOF(*list));
        }
        list[count++] = dir_entry;

        if (!FindNextFileW(find_handle, &find_data)) {
            error_code = GetLastError();
            break;
        }
    }

    if (!FindClose(find_handle) && (error_code == ERROR_NO_MORE_FILES)) {
        error_code = GetLastError();
    }
    if (error_code != ERROR_NO_MORE_FILES) {
        scandir_list_free(list, count, capacity);
        windows_set_errno(error_code);
        return -1;
    }

    if (count >= MAXOF(result)) {
        scandir_list_free(list, count, capacity);
        errno = EOVERFLOW;
        return -1;
    }

    result = (int32)count;
    *namelist = list;
    return result;
}

static time_t
filetime_to_time_t(FILETIME *filetime) {
    ULARGE_INTEGER u_large_integer;
    u_large_integer.LowPart = filetime->dwLowDateTime;
    u_large_integer.HighPart = filetime->dwHighDateTime;
    // Convert from Windows epoch (1601) to Unix epoch (1970)
    return (time_t)(
        (u_large_integer.QuadPart - 116444736000000000ULL) / 10000000ULL);
}

static int
lstat(const char *path, struct stat *statbuf) {
    wchar_t *wide_path;
    WIN32_FILE_ATTRIBUTE_DATA fd;
    ULARGE_INTEGER ull_size;
    DWORD error_code;
    int64 wide_path_size;
    int32 wide_path_length;

    if ((path == NULL) || (statbuf == NULL)) {
        errno = EINVAL;
        return -1;
    }

    wide_path_length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                           path, -1, NULL, 0);
    if (wide_path_length <= 0) {
        windows_set_errno(GetLastError());
        return -1;
    }

    wide_path_size = (int64)wide_path_length*SIZEOF(*wide_path);
    wide_path = malloc2(wide_path_size);
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path, -1,
                            wide_path, wide_path_length)
        != wide_path_length) {
        error_code = GetLastError();
        free2(wide_path, wide_path_size);
        windows_set_errno(error_code);
        return -1;
    }

    if (!GetFileAttributesExW(wide_path, GetFileExInfoStandard, &fd)) {
        error_code = GetLastError();
        free2(wide_path, wide_path_size);
        windows_set_errno(error_code);
        return -1;
    }

    memset64(statbuf, 0, SIZEOF(*statbuf));

    // File type
    if (fd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
        FILE_ATTRIBUTE_TAG_INFO tag_info;
        HANDLE file_handle;

        file_handle = CreateFileW(
            wide_path,
            0,
            FILE_SHARE_READ |FILE_SHARE_WRITE |FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS |FILE_FLAG_OPEN_REPARSE_POINT,
            NULL);
        if (file_handle == INVALID_HANDLE_VALUE) {
            error_code = GetLastError();
            free2(wide_path, wide_path_size);
            windows_set_errno(error_code);
            return -1;
        }

        if (!GetFileInformationByHandleEx(file_handle,
                                          FileAttributeTagInfo,
                                          &tag_info,
                                          (DWORD)SIZEOF(tag_info))) {
            error_code = GetLastError();
            CloseHandle(file_handle);
            free2(wide_path, wide_path_size);
            windows_set_errno(error_code);
            return -1;
        }

        if (!CloseHandle(file_handle)) {
            error_code = GetLastError();
            free2(wide_path, wide_path_size);
            windows_set_errno(error_code);
            return -1;
        }

        if (tag_info.ReparseTag == IO_REPARSE_TAG_SYMLINK) {
            statbuf->st_mode = S_IFLNK;
        } else if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            statbuf->st_mode = S_IFDIR;
        } else {
            statbuf->st_mode = S_IFREG;
        }
    } else if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        statbuf->st_mode = S_IFDIR;
    } else {
        statbuf->st_mode = S_IFREG;
    }

    free2(wide_path, wide_path_size);

    ull_size.LowPart = fd.nFileSizeLow;
    ull_size.HighPart = fd.nFileSizeHigh;
    if (ull_size.QuadPart > LONG_MAX) {
        error("Warning: file is too large, size will be wrong.\n");
    }
    statbuf->st_size = (long)ull_size.QuadPart;

    // Convert FILETIME -> time_t
    statbuf->st_mtime = filetime_to_time_t(&fd.ftLastWriteTime);
    statbuf->st_ctime = filetime_to_time_t(&fd.ftCreationTime);
    statbuf->st_atime = filetime_to_time_t(&fd.ftLastAccessTime);

    return 0;
}

#if TESTING_windows_functions
#define CBASE_IMPLEMENT
#include "cbase.h"

static bool
contains(
    char *buffer,
    int64 length,
    struct dirent **directory_entries,
    int32 *number_files
) {
    for (int32 i = 0; i < *number_files; i += 1) {
        char *from_scan = directory_entries[i]->d_name;

        if (strlen32(from_scan) != length) {
            continue;
        }

        if (!memcmp64(buffer, from_scan, length)) {
            printf("%s == %s\n", buffer, from_scan);
            if (i < (*number_files - 1)) {
                *number_files -= 1;
                free2(directory_entries[i], SIZEOF(**directory_entries));
                memmove64(&directory_entries[i], &directory_entries[i + 1],
                          (*number_files - i)*SIZEOF(*directory_entries));
            }
            return true;
        }
    }
    return false;
}

int
main(void) {
    {
        char *string = "aaa/bbb/ccc";
        int64 length = strlen32(string);

        ASSERT(memmem64(string, length, "aaa", 3) == string);
        ASSERT(memmem64(string, length, "bbb", 3) == string + 4);
        ASSERT(memmem64(string, length, "aaaa", 4) == NULL);
        ASSERT(memmem64(string, length, "bbbb", 4) == NULL);
        ASSERT(memmem64(string, length, "/", 1) == string + 3);
    }

    {
        struct stat stat;
        ASSERT(lstat("LICENSE", &stat) == 0);
        ASSERT(stat.st_size == 34523);
        ASSERT(stat.st_mtime == 1735689600);
        ASSERT(stat.st_ctime == 1735689600);
        error("stat.atime: %lld\n", stat.st_atime);
    }

    {
        struct dirent **dirent;
        int32 nfiles;
        FILE *ls_pipe;
        char buffer[1024];

        if ((nfiles = scandir("./", &dirent, NULL, NULL)) <= 0) {
            error("Error in scandir for windows.\n");
            fatal(EXIT_FAILURE);
        }

        if ((ls_pipe = popen("dir /b", "r")) == NULL) {
            error("Error in popen: %s.\n", strerror(errno));
            fatal(EXIT_FAILURE);
        }
        while (fgets(buffer, SIZEOF(buffer), ls_pipe)) {
            int64 length = (int64)strcspn(buffer, "\n");
            buffer[length] = '\0';
            ASSERT(contains(buffer, length, dirent, &nfiles));
        }

        for (int32 i = 0; i < nfiles; i += 1) {
            free2(dirent[i], SIZEOF(**dirent));
        }
    }

    exit(EXIT_SUCCESS);
}
#endif

#endif
