#if defined(__WIN32__)
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
memmem(const void *haystack, size_t hay_len,
       const void *needle,   size_t needle_len) {
    const uchar *h = haystack;
    const uchar *n = needle;
    const uchar *end = h + hay_len;
    const uchar *limit = end - needle_len + 1;

    if (needle_len == 0)
        return (void *)haystack;
    if (!haystack || !needle)
        return NULL;
    if (hay_len < needle_len)
        return NULL;

    while (h < limit) {
        const uchar *p;

        if ((p = memchr(h, n[0], (size_t)(limit - h))) == NULL)
            return NULL;

        if (memcmp(p, n, needle_len) == 0)
            return (void *)p;
        h = p + 1;
    }

    return NULL;
}
#endif
