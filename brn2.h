/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023 Mior, Lucas; <lucasbuddemior@gmail.com>
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
    size_t length;
} FileName;

typedef struct FileList {
    FileName *files;
    size_t length;
} FileList;

typedef struct SameHash {
    char *key;
    struct SameHash *next;
} SameHash;

void *util_realloc(void *, size_t);
void *util_calloc(size_t, size_t);
void util_command(char **);

size_t hash_function(char *);
bool hash_insert(SameHash *, size_t, char *);
void hash_free(SameHash *, size_t);

FileList main_file_list_from_dir(char *);
FileList main_file_list_from_lines(char *, size_t);
bool main_duplicated_name_hash(FileList *);
bool main_duplicated_name_naive(FileList *);
bool main_verify(FileList *, FileList *);
size_t main_get_number_renames(FileList *, FileList *);
size_t main_execute(FileList *, FileList *);
void main_usage(FILE *);
void main_free_file_list(FileList *);