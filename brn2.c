/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023-2025 Mior, Lucas;
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

#if !defined(BRN2_C)
#define BRN2_C

#include "brn2.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "util.c"
#include "arena.c"

#if defined(__INCLUDE_LEVEL__) && __INCLUDE_LEVEL__ == 0
#define TESTING_brn2 1
#elif !defined(TESTING_brn2)
#define TESTING_brn2 0
#endif

#if OS_WINDOWS
#include "windows_functions.c"
#endif

typedef struct Work {
    void *(*function)(struct Work *);
    FileList *old_list;
    FileList *new_list;
    uint32 start;
    uint32 end;
    uint32 map_capacity;
    uint32 id;
    uint32 *numbers;
    char *map;
} Work;

uint32 brn2_threads(void *(*function)(Work *), uint32, FileList *old,
                    FileList *new, uint32 *numbers, uint32 map_size, char *map);

static void *brn2_threads_work_hashes(Work *);
static void *brn2_threads_work_normalization(Work *);
static void *brn2_threads_work_changes(Work *);
static inline bool brn2_is_invalid_name(char *);
static void brn2_slash_add(FileName *);
static void brn2_list_from_lines(FileList *, char *, bool);

#if OS_UNIX
static pthread_mutex_t brn2_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t brn2_done_work = PTHREAD_COND_INITIALIZER;

pthread_t thread_pool[BRN2_MAX_THREADS];
static uint32 work_pending = 0;
pthread_cond_t brn2_new_work = PTHREAD_COND_INITIALIZER;
#endif

static struct WorkQueue {
    Work *items[BRN2_MAX_THREADS];
    uint32 head;
    uint32 tail;
    uint32 count;
    uint32 padding;
} work_queue = {0};

bool stop_threads = false;

static void *
xarena_push(Arena **arenas, uint32 number, uint32 size) {
    void *p;

    if ((p = arenas_push(arenas, number, size)) == NULL) {
        error("Error pushing %u bytes into arenas %p: %s.", size,
              (void *)arenas, arena_strerror(errno));
        exit(EXIT_FAILURE);
    }
    return p;
}

void
brn2_print_list(FileList *list) {
    error("FileList = {\n");
    error(" arenas,\n");
    error(" indexes = %p\n", (void *)list->indexes);
    error(" indexes_size = %lld\n", (llong)list->indexes_size);
    error(" length = %u\n", list->length);

    for (uint32 i = 0; i < list->length;) {
        FileName *file = list->files[i];
        if (file) {
            assert(file->length == strlen(file->name));
            error("[%u] = %s\n", i, file->name);
        } else {
            error("[%u]", i);
            while (file == NULL) {
                i += 1;
                file = list->files[i];
            }
            error(" ... [%u] = NULL\n", i);
        }
        i += 1;
    }

    error("}\n");
}

int
brn2_compare(const void *a, const void *b) {
    FileName *const *file_a = a;
    FileName *const *file_b = b;
    return strcmp((*file_a)->name, (*file_b)->name);
}

static void
brn2_full_check(FileList *old, FileList *new, HashSet *newlist_set,
                char *name) {
    error("brn2_full_check(%s)\n", name);

    if (old && newlist_set) {
        for (uint32 i = 0; i < old->length; i += 1) {
        }
    }
    if (old) {
        for (uint32 i = 0; i < old->length; i += 1) {
            if (old->files[i]->length != strlen(old->files[i]->name)) {
                error("old [%u] %u != %u\n", i, old->files[i]->length,
                      (uint)strlen(old->files[i]->name));
                exit(EXIT_FAILURE);
            }
        }
    }
    if (new) {
        for (uint32 i = 0; i < new->length; i += 1) {
            assert(new->files[i]->length == strlen(new->files[i]->name));
        }
    }
    if (new && old) {
        assert(new->length == old->length);
        for (uint32 i = 0; i < old->length; i += 1) {
            assert(old->files[i]->length + 200 == new->files[i]->length);
            assert(new->files[i]->length == strlen(new->files[i]->name));
            assert(old->files[i]->length == strlen(old->files[i]->name));
        }
    }

    return;
}

void
brn2_list_from_args(FileList *list, int argc, char **argv) {
    uint32 length = 0;

    if ((uint64)argc >= (uint64)MAXOF(list->length)) {
        error("List can't hold more than %lld names.\n",
              (llong)MAXOF(list->length));
        fatal(EXIT_FAILURE);
    }

    list->files = xmalloc(argc*SIZEOF(*(list->files)));

    for (int i = 0; i < argc; i += 1) {
        char *name = argv[i];
        int64 name_length = strlen64(name);
        FileName **file_pointer = &(list->files[length]);
        FileName *file;
        uint32 size;

        if (name_length >= MAXOF(file->length)) {
            error("Error in arg %d: argument too long. Skipping...\n", i);
            continue;
        }

        if (brn2_is_invalid_name(name)) {
            continue;
        }

        size = STRUCT_ARRAY_SIZE(file, char, name_length + 2);
        *file_pointer = xarena_push(list->arenas, nthreads, ALIGN(size));
        file = *file_pointer;

        file->length = (uint16)name_length;
        memcpy64(file->name, name, name_length + 1);

        length += 1;
    }
    list->length = length;

    return;
}

#if OS_UNIX
static void *__attribute__((noreturn))
brn2_threads_function(void *arg) {
    (void)arg;

    while (true) {
        Work *work;

        xpthread_mutex_lock(&brn2_mutex);
        while (work_queue.count == 0 && !stop_threads) {
            pthread_cond_wait(&brn2_new_work, &brn2_mutex);
        }

        if (stop_threads) {
            xpthread_mutex_unlock(&brn2_mutex);
            pthread_exit(NULL);
        }

        if (work_queue.count == 0) {
            work = NULL;
        } else {
            work = work_queue.items[work_queue.head];
            work_queue.head = (work_queue.head + 1) % LENGTH(work_queue.items);
            work_queue.count -= 1;
        }

        xpthread_mutex_unlock(&brn2_mutex);

        if (work) {
            work->function(work);
            xpthread_mutex_lock(&brn2_mutex);
            work_pending -= 1;
            if (work_pending == 0 && (work_queue.count == 0)) {
                pthread_cond_signal(&brn2_done_work);
            }
            xpthread_mutex_unlock(&brn2_mutex);
        }
    }
}
#endif

void
brn2_list_from_dir(FileList *list, char *directory) {
    struct dirent **directory_list;
    uint32 length = 0;
    uint16 directory_length;
    int number_files;

    if (strcmp(directory, ".")) {
        int64 len = strlen64(directory);
        if (len >= MAXOF(directory_length)) {
            error("Error: directory name too long.\n");
            fatal(EXIT_FAILURE);
        }
        directory_length = (uint16)len;
    } else {
        directory_length = 0;
    }

    number_files = scandir(directory, &directory_list, NULL, NULL);
    if (number_files < 0) {
        error("Error scanning '%s': %s.\n", directory, strerror(errno));
        fatal(EXIT_FAILURE);
    }

    list->files = xmalloc(number_files*SIZEOF(*(list->files)));

    for (int i = 0; i < number_files; i += 1) {
        FileName **file_pointer = &(list->files[length]);
        FileName *file;
        char *name = directory_list[i]->d_name;
        int64 name_length = strlen64(name);
        uint32 size;

        if (brn2_is_invalid_name(name)) {
            if (DEBUGGING) {
                free(directory_list[i]);
            }
            continue;
        }
        if ((name_length + 1 + directory_length) >= MAXOF(file->length)) {
            error("File name too long. Skipping...\n");
            continue;
        }

        if (directory_length) {
            size = STRUCT_ARRAY_SIZE(file, char,
                                     directory_length + 1 + name_length + 2);
            *file_pointer = xarena_push(list->arenas, nthreads, ALIGN(size));
            file = *file_pointer;

            file->length = (uint16)(directory_length + 1 + name_length);
            memcpy64(file->name, directory, directory_length);
            file->name[directory_length] = '/';
            memcpy64(file->name + directory_length + 1, name, name_length + 1);
        } else {
            size = STRUCT_ARRAY_SIZE(file, char, name_length + 2);
            *file_pointer = xarena_push(list->arenas, nthreads, ALIGN(size));
            file = *file_pointer;

            file->length = (uint16)name_length;
            memcpy64(file->name, name, file->length + 1);
        }

        if (DEBUGGING) {
            free(directory_list[i]);
        }
        length += 1;
    }
    free(directory_list);
    list->length = length;
    return;
}

void
brn2_free_list(FileList *list) {
    if (DEBUGGING) {
        for (uint32 i = 0; i < list->length; i += 1) {
            FileName *file = list->files[i];
            (void)file;
            assert(arenas_pop(list->arenas, nthreads, file));
        }
    }
    free(list->files);
    arenas_reset(list->arenas, nthreads);
    return;
}

#if defined(__linux__)
void
brn2_list_from_file(FileList *list, char *filename, bool is_old) {
    char *map;
    uint32 length = 0;
    int64 map_size;
    uint32 padding;
    int fd;

    if (!strcmp(filename, "-")) {
        error("Reading from stdin...\n");
        brn2_list_from_lines(list, filename, is_old);
        return;
    }
    if ((fd = open(filename, O_RDWR)) < 0) {
        error("Error opening '%s' for reading: %s.\n", filename,
              strerror(errno));
        fatal(EXIT_FAILURE);
    }

    {
        struct stat lines_stat;
        if (fstat(fd, &lines_stat) < 0) {
            error("Error in fstat(%s): %s.\n", filename, strerror(errno));
            fatal(EXIT_FAILURE);
        }
        if ((lseek(fd, 0, SEEK_CUR) < 0) && (errno == ESPIPE)) {
            error("File is not seekable.\n");
            if (close(fd) < 0) {
                error("Error closing %s: %s.\n", filename, strerror(errno));
            }
            brn2_list_from_lines(list, filename, is_old);
            return;
        }
        if (!S_ISREG(lines_stat.st_mode)) {
            error("Error getting file names: Not a regular file.\n");
            fatal(EXIT_FAILURE);
        }
        if (lines_stat.st_size <= 0) {
            error("Error getting file names: File size = %lld.\n",
                  (llong)lines_stat.st_size);
            fatal(EXIT_FAILURE);
        }
        map_size = lines_stat.st_size;
    }
    padding = BRN2_ALIGNMENT - ((uint64)map_size % BRN2_ALIGNMENT);
    map_size += padding;
    if (ftruncate(fd, map_size) < 0) {
        error("Error in ftruncate(%s, %lld): %s.\n", filename, (llong)map_size,
              strerror(errno));
        fatal(EXIT_FAILURE);
    }

    map = mmap(NULL, (size_t)map_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        error("Error mapping history file to memory: %s.\n", strerror(errno));
        fatal(EXIT_FAILURE);
    }

    {
        int64 capacity = map_size / 2;
        if (capacity >= MAXOF(list->length)) {
            error("Error: Too large file.\n");
            fatal(EXIT_FAILURE);
        }
        list->files = xmalloc(capacity*SIZEOF(*(list->files)));
    }

    {
        char *begin = map;
        char *pointer = map;
        int64 left = map_size - padding;

        assert((pointer + left) == (map + map_size - padding));

        while ((left > 0) && (pointer = memchr64(pointer, '\n', left))) {
            FileName **file_pointer = &(list->files[length]);
            FileName *file;
            uint32 size;
            int64 name_length = pointer - begin;
            if (name_length >= MAXOF(file->length)) {
                error("Too long line. Skipping...\n");
                begin = pointer + 1;
                left -= (name_length + 1);
                pointer += 1;
                continue;
            }

            if (begin == pointer) {
                error("Empty line in file. Exiting.\n");
                fatal(EXIT_FAILURE);
            }

            size = STRUCT_ARRAY_SIZE(file, char, name_length + 2);
            *file_pointer = xarena_push(list->arenas, nthreads, ALIGN(size));

            file = *file_pointer;
            file->length = (uint16)name_length;
            memcpy64(file->name, begin, name_length + 1);
            file->name[name_length] = '\0';

            if (is_old && brn2_is_invalid_name(file->name)) {
                begin = pointer + 1;
                left -= (name_length + 1);
                pointer += 1;
                continue;
            }

            begin = pointer + 1;
            pointer += 1;
            length += 1;
            left -= (name_length + 1);
            if (length >= (MAXOF(length) / 1000)) {
                if (length % 100000 == 0) {
                    error("Read %u files...\n", length);
                }
            }
        }
    }

    if (length == 0) {
        return;
    }
    list->files = xrealloc(list->files, length*sizeof(*(list->files)));
    list->length = length;
    munmap(map, (size_t)map_size);

    if (ftruncate(fd, map_size - padding) < 0) {
        error("Error in ftruncate(%s, %lld): %s.\n", filename,
              (llong)map_size - padding, strerror(errno));
        fatal(EXIT_FAILURE);
    }
    if (close(fd) < 0) {
        error("Error closing '%s': %s.\n", filename, strerror(errno));
    }

    return;
}
#else
void
brn2_list_from_file(FileList *list, char *filename, bool is_old) {
    brn2_list_from_lines(list, filename, is_old);
    return;
}
#endif

void
brn2_list_from_lines(FileList *list, char *filename, bool is_old) {
    uint32 length = 0;
    char buffer[BRN2_PATH_MAX];
    uint32 capacity = 128;
    FILE *lines;

    if (!strcmp(filename, "-")) {
        lines = stdin;
    } else {
        if ((lines = fopen(filename, "r")) == NULL) {
            error("Error opening '%s': %s.\n", filename, strerror(errno));
            fatal(EXIT_FAILURE);
        }
    }

    list->files = xmalloc(capacity*sizeof(*(list->files)));

    errno = 0;
    while (fgets(buffer, sizeof(buffer), lines)) {
        FileName **file_pointer;
        FileName *file;
        uint16 name_length;
        uint32 size;

        name_length = (uint16)strcspn(buffer, "\n");
        buffer[name_length] = '\0';
        if (is_old && brn2_is_invalid_name(buffer)) {
            continue;
        }

        if (length >= capacity) {
            capacity *= 2;
            list->files
                = xrealloc(list->files, capacity*sizeof(*(list->files)));
        }

        size = STRUCT_ARRAY_SIZE(file, char, name_length + 2);

        file_pointer = &(list->files[length]);
        *file_pointer = xarena_push(list->arenas, nthreads, ALIGN(size));
        file = *file_pointer;

        file->length = name_length;
        memcpy64(file->name, buffer, file->length + 1);

        length += 1;
        if (length >= MAXOF(list->length)) {
            error("Error: more than %lld files being renamed.\n",
                  (llong)MAXOF(list->length));
            break;
        }
        errno = 0;
    }
    if (errno) {
        error("Error reading from %s: %s.\n", filename, strerror(errno));
        fatal(EXIT_FAILURE);
    }
    if (fclose(lines) != 0) {
        error("Error closing file %s: %s.\n", filename, strerror(errno));
    }
    if (length == 0) {
        return;
    }
    list->files = xrealloc(list->files, length*sizeof(*(list->files)));
    list->length = length;
    return;
}

bool
brn2_is_invalid_name(char *filename) {
    while (*filename) {
        if ((*filename != '.') && (*filename != '/')) {
            return false;
        }
        filename += 1;
    }
    return true;
}

void *
brn2_threads_work_normalization(Work *arg) {
    Work *work = arg;
    FileList *list;
    bool old_list;

    if (work->new_list) {
        list = work->new_list;
        old_list = false;
    } else {
        list = work->old_list;
        old_list = true;
    }

    for (uint32 i = work->start; i < work->end; i += 1) {
        FileName *file = list->files[i];
        char *p;
        uint32 off = 0;

        while ((p = memmem64(file->name + off, file->length - off, "//", 2))) {
            off = (uint32)(p - file->name);

            memmove64(&p[0], &p[1], file->length - off);
            file->length -= 1;
        }

        while (file->name[0] == '.' && file->name[1] == '/') {
            memmove64(&file->name[0], &file->name[2], file->length - 1);
            file->length -= 2;
        }

        off = 0;
        while ((p = memmem64(file->name + off, file->length - off, "/./", 3))) {
            off = (uint32)(p - file->name);

            memmove64(&p[1], &p[3], file->length - off - 2);
            file->length -= 2;
        }

        if (old_list) {
            struct stat file_stat;
            if (lstat(file->name, &file_stat) < 0) {
                if (errno != ENOENT) {
                    error("Error in stat('%s'): %s.\n", file->name,
                          strerror(errno));
                }
                work->old_list->files[i]->type = TYPE_ERR;
                continue;
            }
            if (S_ISDIR(file_stat.st_mode)) {
                work->old_list->files[i]->type = TYPE_DIR;
                brn2_slash_add(file);
            } else {
                work->old_list->files[i]->type = TYPE_FILE;
            }
        } else {
            if (work->old_list->files[i]->type == TYPE_DIR) {
                brn2_slash_add(file);
            }
        }
    }
    return 0;
}

void
brn2_slash_add(FileName *file) {
    if (file->name[file->length - 1] != '/') {
        file->name[file->length] = '/';
        file->name[file->length + 1] = '\0';
        file->length += 1;
    }
    return;
}

static void *
brn2_threads_work_sort(Work *arg) {
    Work *work = arg;
    FileName **files = &(work->old_list->files[work->start]);
    qsort(files, work->end - work->start, sizeof(*files), brn2_compare);
    return 0;
}

void *
brn2_threads_work_hashes(Work *arg) {
    Work *work = arg;

    for (uint32 i = work->start; i < work->end; i += 1) {
        FileList *list = work->old_list;
        FileName *newfile = list->files[i];
        assert(newfile->length == strlen(newfile->name));
        newfile->hash = hash_function(newfile->name, newfile->length);
        list->indexes[i] = newfile->hash % work->map_capacity;
    }
    return 0;
}

void *
brn2_threads_work_changes(Work *arg) {
    Work *work = arg;

    for (uint32 i = work->start; i < work->end; i += 1) {
        FileName *oldfile = work->old_list->files[i];
        FileName *newfile = work->new_list->files[i];
        if (oldfile->length == newfile->length) {
            if (!memcmp64(oldfile->name, newfile->name, oldfile->length)) {
                continue;
            }
        }
        work->numbers[work->id] += 1;
    }
    return 0;
}

void
brn2_timings(char *name, struct timespec t0, struct timespec t1,
             uint32 length) {
    long seconds = t1.tv_sec - t0.tv_sec;
    long nanos = t1.tv_nsec - t0.tv_nsec;

    double total_seconds = (double)seconds + (double)nanos / 1.0e9;
    double micros_per_item = 1e6*(total_seconds / (double)(length));

    printf("\ntime elapsed %s\n", name);
    printf("%gs = %gus per item.\n\n", total_seconds, micros_per_item);
    return;
}

void
brn2_normalize_names(FileList *old, FileList *new) {
    brn2_threads(brn2_threads_work_normalization, old->length, old, new, NULL,
                 0, NULL);
    return;
}

void
brn2_create_hashes(FileList *list, uint32 map_capacity) {
    brn2_threads(brn2_threads_work_hashes, list->length, list, NULL, NULL,
                 map_capacity, NULL);
    return;
}

uint32
brn2_get_number_changes(FileList *old, FileList *new) {
    uint32 total = 0;
    uint32 numbers[BRN2_MAX_THREADS] = {0};
    brn2_threads(brn2_threads_work_changes, old->length, old, new, numbers, 0,
                 NULL);

    for (uint32 i = 0; i < BRN2_MAX_THREADS; i += 1) {
        total += numbers[i];
    }
    return total;
}

#if OS_UNIX && (BRN2_MAX_THREADS > 1)
static void
brn2_threads_join(void) {
    xpthread_mutex_lock(&brn2_mutex);
    stop_threads = true;
    pthread_cond_broadcast(&brn2_new_work);
    xpthread_mutex_unlock(&brn2_mutex);

    for (uint32 i = 0; i < nthreads; i += 1) {
        int err;
        if ((err = pthread_join(thread_pool[i], NULL))) {
            error("Error joining thread %u: %s.\n", i, strerror(err));
        }
    }

    xpthread_mutex_destroy(&brn2_mutex);
    xpthread_cond_destroy(&brn2_new_work);
    xpthread_cond_destroy(&brn2_done_work);
}

uint32
brn2_threads(void *(*function)(Work *), uint32 length, FileList *old,
             FileList *new, uint32 *numbers, uint32 map_size, char *map) {
    static Work slices[BRN2_MAX_THREADS];
    uint32 range;

    range = length / nthreads;

    for (uint32 i = 0; i < nthreads; i += 1) {
        slices[i].start = i*range;
        if ((i + 1) < nthreads) {
            slices[i].end = (i + 1)*range;
        } else {
            slices[i].end = length;
        }
        slices[i].old_list = old;
        slices[i].new_list = new;
        slices[i].numbers = numbers;
        slices[i].map_capacity = map_size;
        slices[i].function = function;
        slices[i].map = map;
        slices[i].id = i;

        xpthread_mutex_lock(&brn2_mutex);

        if (work_queue.count >= LENGTH(work_queue.items)) {
            error("Error: work queue is full.\n");
            exit(EXIT_FAILURE);
        }
        work_queue.items[work_queue.tail] = &slices[i];
        work_queue.tail = (work_queue.tail + 1) % LENGTH(work_queue.items);
        work_queue.count += 1;
        work_pending += 1;

        pthread_cond_signal(&brn2_new_work);
        xpthread_mutex_unlock(&brn2_mutex);
    }

    xpthread_mutex_lock(&brn2_mutex);
    while (work_pending > 0 || (work_queue.count != 0)) {
        pthread_cond_wait(&brn2_done_work, &brn2_mutex);
    }
    pthread_cond_signal(&brn2_new_work);
    xpthread_mutex_unlock(&brn2_mutex);

    return nthreads;
}
#else
uint32
brn2_threads(void *(*function)(Work *), uint32 length, FileList *old,
             FileList *new, uint32 *numbers, uint32 map_size, char *map) {
    Work slices[1];

    slices[0].start = 0;
    slices[0].end = length;
    slices[0].old_list = old;
    slices[0].new_list = new;
    slices[0].numbers = numbers;
    slices[0].map_capacity = map_size;
    slices[0].function = function;
    slices[0].map = map;
    slices[0].id = 0;
    function(&slices[0]);

    return 1;
}
#endif

bool
brn2_verify(FileList *new, FileList *old, HashSet *repeated_set,
            uint32 *hashes_new) {
    bool failed = false;

    for (uint32 i = 0; i < new->length; i += 1) {
        FileName *newfile = new->files[i];

        if (newfile->length >= BRN2_PATH_MAX) {
            error("Error: filename on line %u is longer than %u bytes", i + 1,
                  BRN2_PATH_MAX);
            failed = true;
            if (brn2_options_fatal) {
                fatal(EXIT_FAILURE);
            }
        }

        if (!hash_insert_pre_calc_set(repeated_set, newfile->name,
                                      newfile->hash, hashes_new[i])) {
            FileName *oldfile = old->files[i];
            char *diff[] = {
                "diff", "-q", newfile->name, oldfile->name, NULL,
            };
            error("Error: " RED "'%s'" RESET " repeats on line %u. ",
                  newfile->name, i + 1);

            if (util_command(LENGTH(diff), diff) == 0) {
                error("Old (%s) and new name (%s)"
                      " have exactly the same content.\n",
                      oldfile->name, newfile->name);
                if (brn2_options_autosolve) {
                    error("--autosolve is enabled: Deleting old file...\n");
                    if (unlink(newfile->name) < 0) {
                        error("Error deleting %s: %s.\n", newfile->name,
                              strerror(errno));
                        fatal(EXIT_FAILURE);
                    }
                    continue;
                }
            } else {
                error("\n");
            }

            failed = true;
            if (brn2_options_fatal) {
                fatal(EXIT_FAILURE);
            }
        }
    }

    return !failed;
}

static inline int
noop(const char *unused, ...) {
    (void)unused;
    return 0;
}

void
brn2_execute2(FileList *old, FileList *new, HashMap *oldlist_map,
              HashSet *names_renamed, uint32 i, uint32 *number_renames) {
    int renamed;
    uint32 *newname_index_on_oldlist;
    bool newname_exists;

    FileName **oldfile = &(old->files[i]);

    char *oldname = old->files[i]->name;
    char *newname = new->files[i]->name;

    uint64 newhash = new->files[i]->hash;
    uint32 newindex = new->indexes[i];

    uint64 oldhash = old->files[i]->hash;
    uint32 oldindex = old->indexes[i];

    if (newhash == oldhash) {
        if (!strcmp(oldname, newname)) {
            return;
        }
    }
    newname_index_on_oldlist
        = hash_lookup_pre_calc_map(oldlist_map, newname, newhash, newindex);
    newname_exists = !access(newname, F_OK);
#if defined(_GNU_SOURCE)
    if (newname_exists && !newname_index_on_oldlist && !brn2_options_implicit) {
        error("Error renaming " RED "'%s'" RESET " to " RED "'%s'" RESET ":\n",
              oldname, newname);
        error(RED "'%s'" RESET " already exists,"
                  " but it was not given in the list of"
                  " files to rename, and --implict option is off.\n",
              newname);
        if (brn2_options_fatal) {
            fatal(EXIT_FAILURE);
        }
        return;
    }
    if (newname_exists) {
        renamed
            = renameat2(AT_FDCWD, oldname, AT_FDCWD, newname, RENAME_EXCHANGE);
        if (renamed >= 0) {
            if (hash_insert_pre_calc_set(names_renamed, oldname, oldhash,
                                         oldindex)) {
                *number_renames += 1;
            }
            if (hash_insert_pre_calc_set(names_renamed, newname, newhash,
                                         newindex)) {
                *number_renames += 1;
            }
            print(GREEN "%s" RESET " <-> " GREEN "%s" RESET "\n", oldname,
                  newname);

            if (newname_index_on_oldlist) {
                uint32 next = *newname_index_on_oldlist;
                FileName **file_j = &(old->files[next]);

                hash_remove_pre_calc_map(oldlist_map, newname, newhash,
                                         newindex);
                hash_remove_pre_calc_map(oldlist_map, oldname, oldhash,
                                         oldindex);

                hash_insert_pre_calc_map(oldlist_map, newname, newhash,
                                         newindex, i);
                hash_insert_pre_calc_map(oldlist_map, oldname, oldhash,
                                         oldindex, next);

                SWAP(*file_j, *oldfile);
                SWAP(old->indexes[i], old->indexes[next]);
            } else {
                error("Warning: '%s' was swapped with '%s', even though"
                      " '%s' was not in the list of files to rename.\n",
                      newname, oldname, newname);
                error("To disable this behaviour,"
                      " don't pass the --implict option.\n");
                hash_insert_pre_calc_map(oldlist_map, newname, newhash,
                                         newindex, i);
            }
            return;
        } else if (errno != ENOENT) {
            error("Error swapping " RED "'%s'" RESET " and " RED "'%s'" RESET
                  ": %s.\n",
                  oldname, newname, strerror(errno));
            if (brn2_options_fatal) {
                fatal(EXIT_FAILURE);
            }
        }
    }
#else
    (void)newname_index_on_oldlist;
    (void)oldfile;
    if (newname_exists) {
        error("Error renaming " RED "'%s'" RESET
              " to '%s': File already exists.\n",
              oldname, newname);
        if (brn2_options_fatal) {
            fatal(EXIT_FAILURE);
        }
        return;
    }
#endif
    renamed = rename(oldname, newname);
    if (renamed < 0) {
        error("Error renaming " RED "'%s'" RESET " to " RED "'%s'" RESET
              ": %s.\n",
              oldname, newname, strerror(errno));
        if (brn2_options_fatal) {
            fatal(EXIT_FAILURE);
        }
        return;
    } else {
        if (hash_insert_pre_calc_set(names_renamed, oldname, oldhash,
                                     oldindex)) {
            *number_renames += 1;
        }
        print("%s -> " GREEN "%s" RESET "\n", oldname, newname);
    }
    return;
}

void
brn2_usage(FILE *stream) {
    fprintf(
        stream,
        "usage: brn2 [OPTIONS] -- <file1> <file2> ...\n"
        "usage: brn2 [OPTIONS] -f <filename>\n"
        "usage: brn2 [OPTIONS] -d <dir>\n"
        "Rename filenames based on provided arguments.\n"
        "\n"
        "Options:\n"
        "  -F, --fatal     : Exit on first renaming error.\n"
        "  -a, --autosolve : Auto solve name conflicts for equal files.\n"
        "  -e, --explict   : Only rename files given in the list (default).\n"
        "  -h, --help      : Display this help message and exit.\n"
        "  -i, --implict   : Rename files not given in the list of files to "
        "rename.\n"
        "  -q, --quiet     : Quiet mode; suppress output messages.\n"
        "  -s, --sort      : Disable sorting of original list.\n"
        "  -v, --verbose   : Verbose mode (default); output messages.\n"
        "\n"
        "Arguments:\n"
        "  No arguments             : Rename files of current working "
        "directory.\n"
        "  1 or more arguments      : Rename filenames passed as arguments.\n"
        "  -d <dir>, --dir=<dir>    : Rename files in directory.\n"
        "  -f <file>, --file=<file> : Rename filenames listed in this "
        "argument.\n");
    exit((int)(stream != stdout));
}

#if TESTING_brn2
#include <assert.h>
#include "sort.c"

bool brn2_options_fatal = false;
bool brn2_options_implicit = false;
bool brn2_options_quiet = false;
bool brn2_options_autosolve = false;
uint32 nthreads = 2;

static bool
contains_filename(FileList *list, FileName *file, bool verbose) {
    for (uint32 i = 0; i < list->length; i += 1) {
        if (list->files[i]->length != file->length) {
            continue;
        }
        if (!memcmp64(list->files[i]->name, file->name, file->length)) {
            printf(GREEN "%s == %s\n" RESET, file->name, list->files[i]->name);
            if (i < (list->length - 1)) {
                list->length -= 1;
                memmove64(&list->files[i], &list->files[i + 1],
                          (list->length - i)*sizeof(*(list->files)));
            }
            return true;
        }
        if (verbose) {
            printf("%u / %u | %s != %s \n", i + 1, list->length,
                   list->files[i]->name, file->name);
        }
    }
    return false;
}
// flags: -lm

int (*print)(const char *, ...) = printf;

int
main(void) {
    FileList list1_stack = {0};
    FileList list2_stack = {0};
    FileList *list1 = &list1_stack;
    FileList *list2 = &list2_stack;

    for (uint32 i = 0; i < nthreads; i += 1) {
        xpthread_create(&thread_pool[i], NULL, brn2_threads_function, NULL);
    }

    {
        char command[256];
        char *filelist = "/tmp/brn2test";
        SNPRINTF(command, "ls -a > %s", filelist);

        for (uint32 i = 0; i < nthreads; i += 1) {
            list1->arenas[i] = arena_create(BRN2_ARENA_SIZE / nthreads);
            list2->arenas[i] = arena_create(BRN2_ARENA_SIZE / nthreads);
        }

        system(command);
        brn2_list_from_dir(list1, ".");
        brn2_list_from_file(list2, filelist, true);

        assert(list1->length == list2->length);

        brn2_normalize_names(list1, NULL);
        brn2_normalize_names(list2, NULL);

        for (uint32 i = 0; i < list1->length; i += 1) {
            printf(RED "%u / %u\n" RESET, i + 1, list1->length);
            assert(contains_filename(list2, list1->files[i],
                                     list1->length < 9999));
        }

        brn2_free_list(list1);
        brn2_free_list(list2);
        unlink(filelist);
    }

    {
        char command[256];
        char *filelist = "/tmp/brn2test";
        uint32 capacity_set;
        HashMap *map;

        SNPRINTF(command, "find . > %s", filelist);

        for (uint32 i = 0; i < nthreads; i += 1) {
            list1->arenas[i] = arena_create(BRN2_ARENA_SIZE / nthreads);
        }

        system(command);

        brn2_list_from_file(list1, filelist, true);
        brn2_list_from_lines(list2, filelist, true);

        assert(list1->length = list2->length);

        brn2_normalize_names(list1, NULL);
        brn2_normalize_names(list2, NULL);

        for (uint32 i = 0; i < list1->length; i += 1) {
            printf(RED "%u / %u\n" RESET, i + 1, list1->length);
            assert(contains_filename(list2, list1->files[i],
                                     list1->length < 9999));
        }

        map = hash_create_map(list1->length);
        capacity_set = hash_capacity(map);
        list1->indexes_size = list1->length*sizeof(*(list1->indexes));
        list1->indexes = xmmap_commit(&(list1->indexes_size));
        brn2_create_hashes(list1, capacity_set);

        for (uint32 i = 0; i < list1->length; i += 1) {
            FileName *file = list1->files[i];
            uint64 hash;

            assert(file->length == strlen(file->name));
            hash = hash_function(file->name, file->length);
            assert(file->hash == hash);
            assert((file->hash % capacity_set) == (hash & map->bitmask));

            assert(hash_insert_pre_calc_map(map, file->name, hash,
                                            list1->indexes[i], 0));
        }
        for (uint32 i = 0; i < list1->length; i += 1) {
            FileName *file = list1->files[i];
            uint64 hash;
            assert(file->length == strlen(file->name));
            hash = hash_function(file->name, file->length);
            assert(file->hash == hash);
            assert((file->hash % capacity_set) == (hash & map->bitmask));

            assert(hash_remove_pre_calc_map(map, file->name, hash,
                                            list1->indexes[i]));
        }

        brn2_free_list(list1);
        for (uint32 i = 0; i < nthreads; i += 1) {
            arena_destroy(list1->arenas[i]);
        }
        hash_destroy_map(map);
        unlink(filelist);
    }
    {
        int argc = 0;
        int64 capacity = 128;
        char **argv;
        FILE *args;

        char command[256];
        char *filelist = "/tmp/brn2test";
        uint32 capacity_set;
        HashMap *map;

        SNPRINTF(command, "ls *.c > %s", filelist);

        for (uint32 i = 0; i < nthreads; i += 1) {
            list1->arenas[i] = arena_create(BRN2_ARENA_SIZE / nthreads);
        }

        system(command);

        brn2_list_from_file(list1, filelist, true);

        argv = xmalloc(capacity*SIZEOF(*argv));
        for (int i = 0; i < capacity; i += 1) {
            argv[i] = xmalloc(capacity*SIZEOF(*argv[i]));
        }

        if ((args = fopen(filelist, "r")) == NULL) {
            error("Error opening %s: %s.\n", filelist, strerror(errno));
            fatal(EXIT_FAILURE);
        }

        while (fgets(argv[argc], (int)capacity, args)) {
            if (argc >= capacity) {
                error("Arguments file too long\n");
                fatal(EXIT_FAILURE);
            }
            argv[argc][strcspn(argv[argc], "\n")] = '\0';
            argc += 1;
        }
        if (fclose(args) != 0) {
            error("Error closing %s: %s.\n", filelist, strerror(errno));
            fatal(EXIT_FAILURE);
        }
        brn2_list_from_args(list2, argc, argv);

        brn2_normalize_names(list1, NULL);
        brn2_normalize_names(list2, NULL);

        for (uint32 i = 0; i < list1->length; i += 1) {
            printf(RED "%u / %u\n" RESET, i + 1, list1->length);
            assert(contains_filename(list2, list1->files[i],
                                     list1->length < 9999));
        }

        map = hash_create_map(list1->length);
        capacity_set = hash_capacity(map);
        list1->indexes_size = list1->length*sizeof(*(list1->indexes));
        list1->indexes = xmmap_commit(&(list1->indexes_size));
        brn2_create_hashes(list1, capacity_set);

        for (uint32 i = 0; i < list1->length; i += 1) {
            FileName *file = list1->files[i];
            uint64 hash;

            assert(file->length == strlen(file->name));
            hash = hash_function(file->name, file->length);
            assert(file->hash == hash);
            assert((file->hash % capacity_set) == (hash & map->bitmask));

            assert(hash_insert_pre_calc_map(map, file->name, hash,
                                            list1->indexes[i], 0));
        }
        for (uint32 i = 0; i < list1->length; i += 1) {
            FileName *file = list1->files[i];
            uint64 hash;
            assert(file->length == strlen(file->name));
            hash = hash_function(file->name, file->length);
            assert(file->hash == hash);
            assert((file->hash % capacity_set) == (hash & map->bitmask));

            assert(hash_remove_pre_calc_map(map, file->name, hash,
                                            list1->indexes[i]));
        }

        hash_destroy_map(map);
        brn2_free_list(list1);
        for (uint32 i = 0; i < nthreads; i += 1) {
            arena_destroy(list1->arenas[i]);
        }
        unlink(filelist);
    }
    if (OS_LINUX) {
        FileList old_stack;
        FileList new_stack;
        FileList *old = &old_stack;
        FileList *new = &new_stack;

        HashMap *oldlist_map;
        HashSet *names_renamed;
        uint32 number_renames = 0;
        uint32 number_changes;

        char *directory = "/tmp/brn2_abcd";
        char command_rmdir[128];

        static struct {
            char *original;
            char *renamed;
        } files[4] = {
            {"a", "c"},
            {"b", "bxx"},
            {"c", "d"},
            {"d", "a"},
        };

        SNPRINTF(command_rmdir, "rm -rf %s", directory);
        system(command_rmdir);
        if (mkdir(directory, 0777) < 0) {
            error("Error creating directory %s: %s.\n", directory,
                  strerror(errno));
            fatal(EXIT_FAILURE);
        }

        for (uint32 i = 0; i < nthreads; i += 1) {
            old->arenas[i] = arena_create(BRN2_ARENA_SIZE / nthreads);
            new->arenas[i] = arena_create(BRN2_ARENA_SIZE / nthreads);
        }

        for (uint32 i = 0; i < LENGTH(files); i += 1) {
            char path[128];
            FILE *file;
            SNPRINTF(path, "%s/%s", directory, files[i].original);

            if ((file = fopen(path, "w")) == NULL) {
                error("Error opening %s: %s.\n", path, strerror(errno));
                fatal(EXIT_FAILURE);
            }
            fprintf(file, "%s", files[i].renamed);
            if (fclose(file) != 0) {
                error("Error closing %s: %s.\n", path, strerror(errno));
            }
        }

        brn2_list_from_dir(old, directory);
        brn2_normalize_names(old, NULL);
        sort(old);

        new->files = xmalloc(old->length*sizeof(*(new->files)));
        new->length = old->length;
        for (uint32 i = 0; i < new->length; i += 1) {
            FileName **file_pointer = &(new->files[i]);
            FileName *file;
            uint32 name_length;
            uint32 size;
            char path[128];

            name_length
                = (uint32)SNPRINTF(path, "%s/%s", directory, files[i].renamed);

            size = STRUCT_ARRAY_SIZE(file, char, name_length + 2);
            *file_pointer = xarena_push(new->arenas, nthreads, ALIGN(size));
            file = *file_pointer;

            file->length = (uint16)name_length;
            memcpy64(file->name, path, name_length + 1);
        }

        brn2_normalize_names(old, new);

        {
            uint32 capacity_set;
            oldlist_map = hash_create_map(old->length);
            capacity_set = hash_capacity(oldlist_map);
            old->indexes_size = old->length*sizeof(*(old->indexes));
            old->indexes = xmmap_commit(&(old->indexes_size));
            brn2_create_hashes(old, capacity_set);
        }

        for (uint32 i = 0; i < old->length; i += 1) {
            FileName *file = old->files[i];
            uint32 *index = &(old->indexes[i]);
            assert(hash_insert_pre_calc_map(oldlist_map, file->name, file->hash,
                                            *index, i));
        }

        {
            uint32 main_capacity;
            HashSet *newlist_set = hash_create_set(new->length);
            new->indexes_size = new->length*sizeof(*(new->indexes));
            new->indexes = xmmap_commit(&(new->indexes_size));
            main_capacity = hash_capacity(newlist_set);

            brn2_create_hashes(new, main_capacity);

            assert(brn2_verify(new, old, newlist_set, new->indexes));
        }

        number_changes = brn2_get_number_changes(old, new);
        assert(number_changes == LENGTH(files));

        names_renamed = hash_create_set(old->length);

        for (uint32 i = 0; i < old->length; i += 1) {
            brn2_execute2(old, new, oldlist_map, names_renamed, i,
                          &number_renames);
        }

        assert(number_renames == LENGTH(files));
        for (uint32 i = 0; i < LENGTH(files); i += 1) {
            char path[128];
            char buffer[128];
            FILE *file;
            SNPRINTF(path, "%s/%s", directory, files[i].renamed);

            if ((file = fopen(path, "r")) == NULL) {
                error("Error opening %s: %s.\n", path, strerror(errno));
                fatal(EXIT_FAILURE);
            }

            assert(fgets(buffer, sizeof(buffer), file));
            assert(!strcmp(buffer, files[i].renamed));

            if (fclose(file) != 0) {
                error("Error closing %s: %s.\n", path, strerror(errno));
                fatal(EXIT_FAILURE);
            }
        }
        brn2_free_list(old);
        brn2_free_list(new);
        for (uint32 i = 0; i < nthreads; i += 1) {
            arena_destroy(old->arenas[i]);
            arena_destroy(new->arenas[i]);
        }
    }

    brn2_threads_join();
    exit(EXIT_SUCCESS);
}
#endif

#endif
