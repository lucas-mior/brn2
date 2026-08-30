// SPDX-License-Identifier: AGPL
// Copyright (c) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
// Copyright (c) 2026 Mior, Lucas

#define CBASE_IMPLEMENT
#include "cbase.h"

#include "brn2.h"
#include "brn2.c"

bool brn2_options_fatal = false;
bool brn2_options_implicit = false;
bool brn2_options_quiet = false;
bool brn2_options_sort = true;
bool brn2_options_autosolve = false;
bool brn2_options_vim_split = false;
int32 nthreads;
static int32 narenas;
int32 (*print)(const char *, ...) = noop;

static struct option options[] = {
    {"dir",         required_argument, NULL, 'd'},
    {"file",        required_argument, NULL, 'f'},
    {"file-target", required_argument, NULL, 't'},
    {"explicit",    no_argument,       NULL, 'e'},
    {"fatal",       no_argument,       NULL, 'F'},
    {"help",        no_argument,       NULL, 'h'},
    {"implicit",    no_argument,       NULL, 'i'},
    {"quiet",       no_argument,       NULL, 'q'},
    {"sort",        no_argument,       NULL, 's'},
    {"verbose",     no_argument,       NULL, 'v'},
    {"autosolve",   no_argument,       NULL, 'a'},
    {"vim-split",   no_argument,       NULL, 'V'},
    {NULL,          0,                 NULL, 0},
};

enum Brn2InputMode {
    FILES_FROM_FILE,
    FILES_FROM_ARGS,
    FILES_FROM_DIR,
};

static File brn2_buffer;
static File brn2_buffer_old;

static void
write_fatal(int32 fd, char *buffer, int64 size, int32 line) {
    int64 w;
    int unsu;

    if ((w = write64(fd, buffer, size)) != size) {
        error("Error writing %lld bytes to buffer (line %d)",
              size, line);
        if (w < 0) {
            error(": %s", strerror(errno));
        }
        error(".\n");
        fatal(EXIT_FAILURE);
    }

    return;
}

static void
delete_brn2_buffer(void) {
    if (!DEBUGGING) {
        unlink(brn2_buffer.path);
        if (brn2_options_vim_split) {
            unlink(brn2_buffer_old.path);
        }
    }
    return;
}

static int32
main_command_run(Command *command) {
    int32 status = -1;

    if (command_run(command, COMMAND_STDIN_TTY) == 0) {
        status = command->result.status;
        if (!command->result.exited) {
            status = -1;
        }
    }
    return status;
}

static Arena *
xarena_create(int64 size, char *name) {
    Arena *arena;

    if ((arena = arena_create(size, name)) == NULL) {
        error("Error creating arena of size %lld: %s.\n",
              size, arena_strerror(errno));
        fatal(EXIT_FAILURE);
    }
    return arena;
}

#if !BRN2_BENCHMARK
static void
main_edit_buffer(FileList *new, char *editor) {
    int32 status;
    Command command = {0};

    if (isatty(fileno(stdin))) {
        clearerr(stdin);
    } else {
        char *tty_path;
        if (OS_WINDOWS) {
            tty_path = "CONIN$";
        } else {
            tty_path = "/dev/tty";
        }

        if (freopen(tty_path, "r", stdin) == NULL) {
            error("Error reopening stdin: %s.\n", strerror(errno));
            fatal(EXIT_FAILURE);
        }
    }

    if (brn2_options_vim_split) {
        COMMAND_PUSH(
            &command,
            "vim",
            "-O", brn2_buffer_old.path, brn2_buffer.path,
            "-c",
            "wincmd h | set nomodifiable scrollbind cursorbind cursorline",
            "-c",
            "wincmd l | set scrollbind cursorbind",
            "-c",
            " | au QuitPre */brn2.* quitall"
        );
    } else {
        COMMAND_PUSH(&command, editor, brn2_buffer.path);
    }

    status = main_command_run(&command);
    command_free(&command);

    if (status != 0) {
        if (OS_WINDOWS) {
            Command command_windows = {0};

            COMMAND_PUSH(&command_windows, "Notepad.exe", brn2_buffer.path);
            if (main_command_run(&command_windows) < 0) {
                command_free(&command_windows);
                fatal(EXIT_FAILURE);
            }
            command_free(&command_windows);
        } else {
            fatal(EXIT_FAILURE);
        }
    }
    brn2_list_from_file(new, brn2_buffer.path, false);
    return;
}
#endif

int
main(int argc, char **argv) {
    FileList old_stack = {0};
    FileList new_stack = {0};
    FileList *old;
    FileList *new;
    struct Hash_map *oldlist_map = NULL;
    struct Hash_map *newlist_map = NULL;
    int32 available_threads;
    int32 unfiltered_old_length;
    uint32 main_capacity;
    char *editor;
    char *directory = ".";
    char *lines = NULL;
    char *lines_target = NULL;
    enum Brn2InputMode mode = FILES_FROM_DIR;
    int32 opt;

#if BRN2_BENCHMARK
    struct timespec t0;
    struct timespec t1;
    time_monotonic_precise(&t0);
#endif

    program_len = strlen32(argv[0]);
    program = basename2(argv[0], &program_len, NULL);

    while ((opt = getopt_long(argc, argv, "d:f:t:eFhiqsvaV", options, NULL))
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
        case 't':
            if (optarg == NULL) {
                brn2_usage(stderr);
            }
            lines_target = optarg;
            break;
        case '?':
            brn2_usage(stderr);
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
        case 'V':
            brn2_options_vim_split = true;
            break;
        default:
            brn2_usage(stderr);
        }
    }
    if ((optind < argc) && strequal(argv[optind], "--")) {
        optind += 1;
    }
    if ((argc - optind) >= 1) {
        mode = FILES_FROM_ARGS;
    }

#if BRN2_BENCHMARK
    (void)lines_target;
#endif

    available_threads = util_nthreads();
    if (available_threads <= 0) {
        nthreads = 1;
    } else {
        nthreads = (int32)MIN(available_threads, BRN2_MAX_THREADS);
    }

    old = &old_stack;
    new = &new_stack;

    narenas = nthreads;
    for (int32 i = 0; i < narenas; i += 1) {
        char buffer_old[256];
        char buffer_new[256];

        SNPRINTF(buffer_old, "arena_old[%d]", i);
        SNPRINTF(buffer_new, "arena_new[%d]", i);
        old->arenas[i] = xarena_create(BRN2_ARENA_SIZE / narenas,
                                       buffer_old);
        new->arenas[i] = xarena_create(BRN2_ARENA_SIZE / narenas,
                                       buffer_new);
    }

    switch (mode) {
    case FILES_FROM_FILE:
        if (lines == NULL) {
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
#endif

    if (!brn2_options_quiet) {
        printf("Normalizing filenames...\n");
    }

#if BRN2_NORMALIZE_NAMES_BENCHMARK
    {
        struct timespec normalize_t0;
        struct timespec normalize_t1;

        time_monotonic_precise(&normalize_t0);
        brn2_normalize_names(old, NULL);
        time_monotonic_precise(&normalize_t1);
        PRINT_TIMINGS(old->length,
                      normalize_t0, normalize_t1,
                      "brn2_normalize_names");
        exit(EXIT_SUCCESS);
    }
#else
    brn2_normalize_names(old, NULL);
#endif

    {
        int64 length_before;
        int32 j = 0;

        for (int32 i = 0; i < old->length; i += 1) {
            FileName *filename = old->files[i];
            if (filename->type == TYPE_ERR) {
                fprintf(stderr, "Removing '%s' from list.\n", filename->name);
                continue;
            }
            if (j != i) {
                old->files[j] = filename;
            }
            j += 1;
        }

        length_before = old->length;
        old->length = j;

        if (old->length == 0) {
            error("No files to rename.\n");
            fatal(EXIT_FAILURE);
        }

        old->files = realloc2(old->files,
                              length_before, old->length,
                              SIZEOF(*(old->files)));
        old->capacity = old->length;
    }

    if (brn2_options_sort) {
        brn2_sort(old);
    }

    if ((editor = getenv("EDITOR")) == NULL) {
        editor = "vim";
        error("EDITOR variable is not set. Using %s by default.\n", editor);
    } else {
        editor = xstrdup(editor);
    }

    {
        char write_buffer[BRN2_PATH_MAX*2];
        char *pointer = write_buffer;
        uint32 capacity_map;
        int32 j = 0;
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

        SNPRINTF(brn2_buffer.path, "%s/%s", temp, "brn2.new.XXXXXX");
        if ((brn2_buffer.fd = mkstemp(brn2_buffer.path)) < 0) {
            error("Error opening '%s': %s.\n",
                  brn2_buffer.path, strerror(errno));
            fatal(EXIT_FAILURE);
        }
#if CC_GCC || CC_CLANG
        atexit(delete_brn2_buffer);
#endif

        if (brn2_options_vim_split) {
            SNPRINTF(brn2_buffer_old.path, "%s/%s", temp, "brn2.old.XXXXXX");
            if ((brn2_buffer_old.fd = mkstemp(brn2_buffer_old.path)) < 0) {
                error("Error opening '%s': %s.\n",
                      brn2_buffer_old.path, strerror(errno));
                fatal(EXIT_FAILURE);
            }
        }

        unfiltered_old_length = old->length;
        oldlist_map = hash_create_map((uint32)old->length, "oldlist_map");
        capacity_map = hash_capacity(oldlist_map);

        old->indexes_size = old->length*SIZEOF(*(old->indexes));
        old->indexes = xmmap_commit(&(old->indexes_size));

        brn2_create_hashes(old, capacity_map);

        for (int32 i = 0; i < old->length; i += 1) {
            FileName *file = old->files[i];
            uint32 index = old->indexes[i];
            bool contains_newline;

            if ((contains_newline = memchr64(file->name, '\n', file->length))
                || !hash_insert_pre_calc_map(oldlist_map,
                                             file->name, file->length,
                                             file->hash, index, j)) {
                if (contains_newline) {
                    error2(RED("'%s'") " contains new line.", file->name);
                } else {
                    error2(RED("'%s'") " repeated in the buffer.", file->name);
                }
                if (brn2_options_fatal) {
                    error2("\n");
                    fatal(EXIT_FAILURE);
                }

                error2(" Removing from list...\n");
                continue;
            }

            buffered = pointer - write_buffer;
            if (buffered >= BRN2_PATH_MAX) {
                write_fatal(brn2_buffer.fd, write_buffer, buffered, i);
                if (brn2_options_vim_split) {
                    write_fatal(brn2_buffer_old.fd, write_buffer, buffered, i);
                }
                pointer = write_buffer;
            }

            if (j != i) {
                old->files[j] = file;
                old->indexes[j] = index;
            }
            j += 1;

            file->name[file->length] = '\n';
            memcpy64(pointer, file->name, file->length + 1);
            pointer += file->length + 1;
            file->name[file->length] = '\0';
        }
        buffered = pointer - write_buffer;
        write_fatal(brn2_buffer.fd, write_buffer, buffered, -1);
        old->length = j;
        if (brn2_options_vim_split) {
            write_fatal(brn2_buffer_old.fd, write_buffer, buffered, -1);
        }

        if (XCLOSE(&(brn2_buffer.fd)) < 0) {
            fatal(EXIT_FAILURE);
        }

        if (brn2_options_vim_split) {
            if (XCLOSE(&(brn2_buffer_old.fd)) < 0) {
                fatal(EXIT_FAILURE);
            }
        }
    }

    if (old->length <= 0) {
        error("Error: old list is empty\n");
        fatal(EXIT_FAILURE);
    }

    {
#if BRN2_BENCHMARK
        {
            Command command = {0};

            COMMAND_PUSH(&command, "shuf", brn2_buffer.path);
            COMMAND_PUSH(&command, "-o", brn2_buffer.path);

            main_command_run(&command);
            command_free(&command);
            brn2_list_from_file(new, brn2_buffer.path, false);

            for (int32 i = 0; i < new->length; i += 1) {
                int32 rand1 = rand_int();
                double x = (double)rand1 / (double)INT32_MAX;
                if (x < 0.4) {
                    random_filename_inplace(new->files[i]->name,
                                            new->files[i]->length);
                }
            }
            brn2_normalize_names(old, new);

            newlist_map = hash_create_map(unfiltered_old_length, "newlist_map");

            main_capacity = hash_capacity(newlist_map);

            new->indexes_size = new->length*SIZEOF(*(new->indexes));
            new->indexes = xmmap_commit(&(new->indexes_size));
            brn2_create_hashes(new, main_capacity);

            brn2_verify(new, old, oldlist_map, newlist_map, new->indexes);

            hash_print_summary_map(newlist_map);
        }
#else
        while (true) {
            if (lines_target) {
                brn2_list_from_file(new, lines_target, false);
            } else {
                main_edit_buffer(new, editor);
            }

            if (new->length <= 0) {
                error("New list is empty. Exiting...\n");
                fatal(EXIT_FAILURE);
            }

            if (old->length != new->length) {
                error("You are renaming " RED("%d") " file%.*s "
                      "but buffer contains " RED("%d") " file name%.*s\n",
                      old->length, old->length != 1, "s",
                      new->length, new->length != 1, "s");
                brn2_free_list(new);
                printf("Fix your renames. Press control-c to cancel or press"
                       " ENTER to open the file list editor again.\n");
                fgetc(stdin);
                continue;
            }

            brn2_normalize_names(old, new);

            if (newlist_map == NULL) {
                newlist_map = hash_create_map((uint32)unfiltered_old_length,
                                              "newlist_map");
            } else {
                hash_zero_map(newlist_map);
            }
            if (new->indexes == NULL) {
                new->indexes_size = new->length*SIZEOF(*(new->indexes));
                new->indexes = xmmap_commit(&(new->indexes_size));
            }

            main_capacity = hash_capacity(newlist_map);
            brn2_create_hashes(new, main_capacity);

            if (brn2_verify(new, old, oldlist_map, newlist_map, new->indexes)) {
                break;
            }

            brn2_free_list(new);
            if (lines_target) {
                exit(EXIT_FAILURE);
            } else {
                printf("Fix your renames. Press control-c to cancel or press"
                       " ENTER to open the file list editor again.\n");
                fgetc(stdin);
                continue;
            }
        }
#endif
    }

#if BRN2_BENCHMARK
    time_monotonic_precise(&t1);
    PRINT_TIMINGS(old->length, t0, t1, "before renames");
#endif

    {
        int32 number_changes = brn2_get_number_changes(old, new);
        int32 number_renames = 0;

        if (number_changes > 0) {
            struct Hash_set *names_renamed
                = hash_create_set((uint32)unfiltered_old_length,
                                  "names_renamed");

            if (brn2_options_quiet) {
                print = noop;
            } else {
                print = printf;
            }

            brn2_execute(old, new, oldlist_map, names_renamed,
                         &number_renames);
            if (DEBUGGING) {
                hash_destroy_set(names_renamed);
            }
        }
        if (number_changes != number_renames) {
            error("%d name%.*s changed but %d file%.*s renamed. ",
                  number_changes, number_changes != 1, "s", number_renames,
                  number_renames != 1, "s");
            error2("Check your files.\n");
            fatal(EXIT_FAILURE);
        } else {
            print("%d file%.*s renamed.\n",
                       number_renames, number_renames != 1, "s");
        }
    }

#if BRN2_BENCHMARK
    time_monotonic_precise(&t1);
    PRINT_TIMINGS(old->length, t0, t1, "renames");
#endif

    if (DEBUGGING) {
        brn2_free_list(old);
        brn2_free_list(new);
        xmunmap(old->indexes, old->indexes_size);
        xmunmap(new->indexes, new->indexes_size);
        hash_destroy_map(oldlist_map);
        hash_destroy_map(newlist_map);
        arenas_destroy(old->arenas, narenas);
        arenas_destroy(new->arenas, narenas);
    }
#if OS_WINDOWS
    printf("Press enter to continue.\n");
    fgetc(stdin);
#endif
    exit(EXIT_SUCCESS);
}
