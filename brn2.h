#define _USE_GNU
#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/syscall.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define MIN_HASH_TABLE_SIZE 256

typedef struct FileName {
    char *name;
    size_t len;
} FileName;

typedef struct FileList {
    FileName *files;
    size_t len;
} FileList;

typedef struct SameHash {
    char *key;
    struct SameHash *next;
} SameHash;

void *util_realloc(void *, size_t);
void *util_calloc(size_t, size_t);
size_t hash(char *);
void util_cmd(char **);
