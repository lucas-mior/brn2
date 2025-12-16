/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023-2025 Mior, Lucas;
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the*License,
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

#include "brn2.h"

#include "brn2.c"
#include "util.c"
#include "arena.c"

#define MAX_THREADS BRN2_MAX_THREADS
#define COMPARE brn2_compare
#include "sort.c"

bool brn2_options_fatal = DEBUGGING;
bool brn2_options_implicit = false;
bool brn2_options_quiet = false;
bool brn2_options_sort = true;
bool brn2_options_autosolve = false;
uint32 nthreads;
int (*print)(const char *, ...);

// clang-format off
static struct option options[] = {
    {"dir",      required_argument, NULL, 'd'},
    {"file",     required_argument, NULL, 'f'},
    {"explict",  no_argument,       NULL, 'e'},
    {"fatal",    no_argument,       NULL, 'F'},
    {"help",     no_argument,       NULL, 'h'},
    {"implict",  no_argument,       NULL, 'i'},
    {"quiet",    no_argument,       NULL, 'q'},
    {"sort",     no_argument,       NULL, 's'},
    {"verbose",  no_argument,       NULL, 'v'},
    {"autosave", no_argument,       NULL, 'a'},
    {NULL,       0,                 NULL, 0}
};
// clang-format on

enum {
    FILES_FROM_FILE,
    FILES_FROM_ARGS,
    FILES_FROM_DIR,
};

static File brn2_buffer;

static void
delete_brn2_buffer(void) {
    if (!DEBUGGING) {
        unlink(brn2_buffer.name);
    }
    return;
}

static void __attribute__((noreturn))
handler_segv(int unused) {
    (void)unused;
    error("%s: Memory error. Please file a bug report.\n", program);
    fatal(EXIT_FAILURE);
}

static Arena *
xarena_create(int64 size) {
    Arena *arena;
    if ((arena = arena_create(size)) == NULL) {
        error("Error creating arena of size %lld: %s.\n", (llong)size,
              arena_strerror(errno));
        fatal(EXIT_FAILURE);
    }
    return arena;
}

int
main(int argc, char **argv) {
    FileList old_stack = {0};
    FileList new_stack = {0};
    FileList *old;
    FileList *new;
    HashMap *oldlist_map = NULL;
    HashSet *newlist_set = NULL;
    uint32 available_threads;
#if OS_UNIX
    uint32 thread_ids[BRN2_MAX_THREADS];
#endif

    uint32 main_capacity;
    char *EDITOR;
    int opt;

    char *directory = ".";
    char *lines = NULL;
    int mode = FILES_FROM_DIR;

#if BRN2_BENCHMARK
    struct timespec t0;
    struct timespec t1;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
#endif

    program = basename(argv[0]);

    while ((opt = getopt_long(argc, argv, "d:f:ceFhiqsva", options, NULL))
           != -1) {
        switch (opt) {
        case 'd':
            mode = FILES_FROM_DIR;
            if (optarg == NULL) {
                brn2_usage(stderr);
            }
            directory = optarg;
            break;
        case 'f':
            mode = FILES_FROM_FILE;
            if (optarg == NULL) {
                brn2_usage(stderr);
            }
            lines = optarg;
            break;
        case '?':
            brn2_usage(stderr);
        case 'c':
            break;
        case 'e':
            brn2_options_implicit = false;
            break;
        case 'F':
            brn2_options_fatal = true;
            break;
        case 'h':
            brn2_usage(stdout);
        case 'i':
            brn2_options_implicit = true;
            break;
        case 'q':
            brn2_options_quiet = true;
            break;
        case 's':
            brn2_options_sort = false;
            break;
        case 'v':
            brn2_options_quiet = false;
            break;
        case 'a':
            brn2_options_autosolve = true;
            break;
        default:
            brn2_usage(stderr);
        }
    }
    if (optind < argc && !strcmp(argv[optind], "--")) {
        optind += 1;
    }
    if ((argc - optind) >= 1) {
        mode = FILES_FROM_ARGS;
    }

    available_threads = util_nthreads();
    if (available_threads <= 0) {
        nthreads = 1;
    } else {
        nthreads = MIN(available_threads, BRN2_MAX_THREADS);
    }

    old = &old_stack;
    new = &new_stack;

    for (uint32 i = 0; i < nthreads; i += 1) {
        old->arenas[i] = arena_create(BRN2_ARENA_SIZE / nthreads);
        new->arenas[i] = arena_create(BRN2_ARENA_SIZE / nthreads);
    }

    switch (mode) {
    case FILES_FROM_FILE:
        if (!lines) {
            brn2_usage(stderr);
        }
        brn2_list_from_file(old, lines, true);
        break;
    case FILES_FROM_ARGS:
        brn2_list_from_args(old, argc - optind, &argv[optind]);
        break;
    case FILES_FROM_DIR:
        brn2_list_from_dir(old, directory);
        break;
    default:
        brn2_usage(stderr);
    }

    if (old->length == 0) {
        error("Empty list. Exiting.\n");
        fatal(EXIT_FAILURE);
    }

#if BRN2_MAX_THREADS > 1
    if (nthreads*2 >= old->length) {
        nthreads = 1;
    }
    if (old->length <= BRN2_MIN_PARALLEL) {
        nthreads = 1;
    }

    for (uint32 i = 0; i < nthreads; i += 1) {
        thread_ids[i] = i;
        xpthread_create(&thread_pool[i], NULL, brn2_threads_function,
                        &thread_ids[i]);
    }
#endif

    if (!brn2_options_quiet) {
        printf("Normalizing filenames...\n");
    }
    brn2_normalize_names(old, NULL);

    {
        uint32 j = 0;
        for (uint32 i = 0; i < old->length; i++) {
            FileName *file = old->files[i];
            if (file->type == TYPE_ERR) {
                fprintf(stderr, "Removing '%s' from list.\n", file->name);
                continue;
            }
            if (j != i) {
                old->files[j] = file;
            }
            j += 1;
        }
        old->length = j;
    }

    if (old->length == 0) {
        error("No files to rename.\n");
        fatal(EXIT_FAILURE);
    }

    old->files = xrealloc(old->files, old->length*sizeof(*(old->files)));

    if (brn2_options_sort) {
        sort(old);
    }

    if (!(EDITOR = getenv("EDITOR"))) {
        EDITOR = "vim";
        error("EDITOR variable is not set. Using %s by default.\n", EDITOR);
    }

    {
        char write_buffer[BRN2_PATH_MAX*2];
        char *pointer = write_buffer;
        uint32 capacity_set;
        uint32 j = 0;
        int64 w;
        int64 buffered;
#if OS_UNIX
        char *temp = "/tmp";
#else
        char *temp = getenv("Temp");
        if (temp == NULL) {
            error("%%TEMP%% is not set.\n");
            fatal(EXIT_FAILURE);
        }
#endif

        SNPRINTF(brn2_buffer.name, "%s/%s", temp, "brn2.XXXXXX");
        if ((brn2_buffer.fd = mkstemp(brn2_buffer.name)) < 0) {
            error("Error opening '%s': %s.\n", brn2_buffer.name,
                  strerror(errno));
            fatal(EXIT_FAILURE);
        }

        oldlist_map = hash_create_map(old->length);
        capacity_set = hash_capacity(oldlist_map);
        old->indexes_size = old->length*sizeof(*(old->indexes));
        old->indexes = xmmap_commit(&(old->indexes_size));
        brn2_create_hashes(old, capacity_set);

        for (uint32 i = 0; i < old->length; i += 1) {
            FileName *file = old->files[i];
            uint32 index = old->indexes[i];
            bool contains_newline;

            if ((contains_newline = memchr64(file->name, '\n', file->length))
                || !hash_insert_pre_calc_map(oldlist_map, file->name,
                                             file->hash, index, i)) {
                if (contains_newline) {
                    error(RED "'%s'" RESET " contains new line.", file->name);
                } else {
                    error(RED "'%s'" RESET " repeated in the buffer.",
                          file->name);
                }
                if (brn2_options_fatal) {
                    error("\n");
                    fatal(EXIT_FAILURE);
                }

                error(" Removing from list...\n");
                continue;
            }

            buffered = pointer - write_buffer;
            if (buffered >= BRN2_PATH_MAX) {
                if ((w = write64(brn2_buffer.fd, write_buffer, buffered))
                    != buffered) {
                    error("Error writing %lld bytes to buffer (line %u)",
                          (llong)buffered, i);
                    if (w < 0) {
                        error(": %s", strerror(errno));
                    }
                    error(".\n");
                    fatal(EXIT_FAILURE);
                }
                pointer = write_buffer;
            }

            if (j != i) {
                old->files[j] = file;
                old->indexes[j] = index;
            }
            j += 1;

            file->name[file->length] = '\n';
            memcpy(pointer, file->name, file->length + 1);
            pointer += file->length + 1;
            file->name[file->length] = '\0';
        }
        buffered = pointer - write_buffer;
        if ((w = write64(brn2_buffer.fd, write_buffer, buffered)) != buffered) {
            error("Error writing %lld bytes to buffer (last line)",
                  (llong)buffered);
            if (w < 0) {
                error(": %s", strerror(errno));
            }
            error(".\n");
            fatal(EXIT_FAILURE);
        } else {
            old->length = j;
        }
        if (close(brn2_buffer.fd) < 0) {
            error("Error closing buffer: %s\n", strerror(errno));
            fatal(EXIT_FAILURE);
        }
        brn2_buffer.fd = -1;
        atexit(delete_brn2_buffer);
    }

    if (old->length <= 0) {
        error("Error: old list is empty\n");
        fatal(EXIT_FAILURE);
    }

    {
        char *args_edit[] = {EDITOR, brn2_buffer.name, NULL};
        char *args_shuf[]
            = {"shuf", brn2_buffer.name, "-o", brn2_buffer.name, NULL};
        (void)args_edit;
        (void)args_shuf;

#if BRN2_BENCHMARK
        {
            char allowed[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "!@#$%&*()[]-=_+<>,"
                             "0123456789";
            util_command(LENGTH(args_shuf), args_shuf);
            brn2_list_from_file(new, brn2_buffer.name, false);

            srand(42);
            for (uint32 i = 0; i < new->length; i += 1) {
                int32 rand1 = rand();
                float x = (float)rand1 / (float)RAND_MAX;
                uint32 length = new->files[i]->length;
                if (x < 0.4f) {
                    for (uint32 j = 0; j < length; j += 1) {
                        uint32 y = (uint32)rand();
                        char c = allowed[y % (sizeof(allowed) - 1)];
                        new->files[i]->name[j] = c;
                    }
                }
            }
            brn2_normalize_names(old, new);

            newlist_set = hash_create_set(new->length);
            main_capacity = hash_capacity(newlist_set);
            new->indexes_size = new->length*sizeof(*(new->indexes));
            new->indexes = xmmap_commit(&(new->indexes_size));
            brn2_create_hashes(new, main_capacity);
            brn2_verify(new, old, newlist_set, new->indexes);
            hash_print_summary_set(newlist_set, "newlist_set");
        }
#else
        while (true) {
            if (!isatty(fileno(stdin))) {
                char *tty_path;
                if (OS_WINDOWS) {
                    tty_path = "CONIN$";
                } else {
                    tty_path = "/dev/tty";
                }

                if (!freopen(tty_path, "r", stdin)) {
                    error("Error reopening stdin: %s.\n", strerror(errno));
                }
            }
            if (util_command(LENGTH(args_edit), args_edit) < 0) {
                if (OS_WINDOWS) {
                    args_edit[0] = "Notepad.exe";
                    if (util_command(LENGTH(args_edit), args_edit) < 0) {
                        fatal(EXIT_FAILURE);
                    }
                } else {
                    fatal(EXIT_FAILURE);
                }
            }
            brn2_list_from_file(new, brn2_buffer.name, false);

            if (old->length <= 0) {
                // to please clang static analyzer
                error("New list is empty. Exiting...\n");
                fatal(EXIT_FAILURE);
            }

            if (old->length != new->length) {
                error("You are renaming " RED "%u" RESET " file%.*s "
                      "but buffer contains " RED "%u" RESET " file name%.*s\n",
                      old->length, old->length != 1, "s", new->length,
                      new->length != 1, "s");
                brn2_free_list(new);
                printf("Fix your renames. Press control-c to cancel or press"
                       " ENTER to open the file list editor again.\n");
                fgetc(stdin);
                continue;
            }

            brn2_normalize_names(old, new);

            if (newlist_set == NULL) {
                newlist_set = hash_create_set(new->length);
            } else {
                hash_zero_set(newlist_set);
            }
            if (new->indexes == NULL) {
                new->indexes_size = new->length*sizeof(*(new->indexes));
                new->indexes = xmmap_commit(&(new->indexes_size));
            }

            main_capacity = hash_capacity(newlist_set);
            brn2_create_hashes(new, main_capacity);

            if (!brn2_verify(new, old, newlist_set, new->indexes)) {
                brn2_free_list(new);
                printf("Fix your renames. Press control-c to cancel or press"

                       " ENTER to open the file list editor again.\n");
                fgetc(stdin);
                continue;
            } else {
                break;
            }
        }
#endif
    }

#if BRN2_BENCHMARK
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    brn2_timings("before renames", t0, t1, old->length);
#endif

    {
        uint32 number_changes = brn2_get_number_changes(old, new);
        uint32 number_renames = 0;

        if (number_changes) {
            HashSet *names_renamed = hash_create_set(old->length);

            if (brn2_options_quiet) {
                print = noop;
            } else {
                print = printf;
            }

            for (uint32 i = 0; i < old->length; i += 1) {
                brn2_execute2(old, new, oldlist_map, names_renamed, i,
                              &number_renames);
            }
            if (DEBUGGING) {
                hash_destroy_set(names_renamed);
            }
        }
        if (number_changes != number_renames) {
            error("%u name%.*s changed but %u file%.*s renamed. "
                  "Check your files.\n",
                  number_changes, number_changes != 1, "s", number_renames,
                  number_renames != 1, "s");
            fatal(EXIT_FAILURE);
        } else {
            printf("%u file%.*s renamed.\n", number_renames,
                   number_renames != 1, "s");
        }
    }

#if BRN2_BENCHMARK
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    brn2_timings("renames", t0, t1, old->length);
#endif

    if (DEBUGGING) {
#if BRN2_MAX_THREADS > 1
        brn2_threads_join();
#endif
        brn2_free_list(old);
        brn2_free_list(new);
        xmunmap(old->indexes, old->indexes_size);
        xmunmap(new->indexes, new->indexes_size);
        hash_destroy_map(oldlist_map);
        hash_destroy_set(newlist_set);
        arenas_destroy(old->arenas, nthreads);
        arenas_destroy(new->arenas, nthreads);
    }
#if OS_WINDOWS
    printf("Press enter to continue.\n");
    fgetc(stdin);
#endif
    exit(EXIT_SUCCESS);
}
