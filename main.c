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

#include "brn2.h"

#include "brn2.c"
#include "hash.c"
#include "util.c"
#include "arena.c"
#include "sort.c"

char *program;
bool brn2_options_fatal = BRN2_DEBUG;
bool brn2_options_implicit = false;
bool brn2_options_quiet = false;
bool brn2_options_sort = true;
uint32 nthreads;
int (*print)(const char *, ...);

static struct option options[] = {
    {"dir",     required_argument, NULL, 'd'},
    {"file",    required_argument, NULL, 'f'},
    {"recurse", required_argument, NULL, 'r'},
    {"check",   no_argument,       NULL, 'c'},
    {"explict", no_argument,       NULL, 'e'},
    {"fatal",   no_argument,       NULL, 'F'},
    {"help",    no_argument,       NULL, 'h'},
    {"implict", no_argument,       NULL, 'i'},
    {"quiet",   no_argument,       NULL, 'q'},
    {"sort",    no_argument,       NULL, 's'},
    {"verbose", no_argument,       NULL, 'v'},
    {NULL, 0, NULL, 0}
};

enum {
    FILES_FROM_FILE,
    FILES_FROM_ARGS,
    FILES_FROM_DIR,
    FILES_FROM_DIR_RECURSE,
};

static char *brn2_buffer_name;

static void
delete_brn2_buffer(void) {
    if (!BRN2_DEBUG)
        unlink(brn2_buffer_name);
    return;
}

static void __attribute__((noreturn))
handler_segv(int unused) {
    (void) unused;
    error("%s: Memory error. Please file a bug report.\n", program);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    FileList old_stack = {0};
    FileList new_stack = {0};
    FileList *old;
    FileList *new;
    HashMap *oldlist_map = NULL;
    HashSet *newlist_set = NULL;
    uint32 available_threads;

    uint32 main_capacity;
    char *EDITOR;
    int opt;

    char *directory = ".";
    char *lines = NULL;
    int mode = FILES_FROM_DIR;

#ifdef BRN2_BENCHMARK
    struct timespec t0;
    struct timespec t1;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
#endif

#ifndef __WIN32__
    /* struct sigaction signal_segment_violation; */

    /* signal_segment_violation.sa_handler = handler_segv; */
    /* signal_segment_violation.sa_flags = 0; */
    /* sigemptyset(&(signal_segment_violation.sa_mask)); */
    /* sigaction(SIGSEGV, &signal_segment_violation, NULL); */
#endif

    old = &old_stack;
    new = &new_stack;

    old->arena = arena_alloc(BRN2_ARENA_SIZE);
    new->arena = arena_alloc(BRN2_ARENA_SIZE);
    program = basename(argv[0]);

    while ((opt = getopt_long(argc, argv,
                              "d:f:r:ceFhiqsv", options, NULL)) != -1) {
        switch (opt) {
        case 'd':
            mode = FILES_FROM_DIR;
            if (optarg == NULL)
                brn2_usage(stderr);
            directory = optarg;
            break;
        case 'f':
            mode = FILES_FROM_FILE;
            if (optarg == NULL)
                brn2_usage(stderr);
            lines = optarg;
            break;
        case 'r':
            mode = FILES_FROM_DIR_RECURSE;
            if (optarg == NULL)
                brn2_usage(stderr);
            directory = optarg;
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
        default:
            brn2_usage(stderr);
        }
    }
    if (optind < argc && !strcmp(argv[optind], "--"))
        optind += 1;
    if ((argc - optind) >= 1)
        mode = FILES_FROM_ARGS;

    available_threads = util_nthreads();
    if (available_threads <= 0)
        nthreads = 1; 
    else
        nthreads = MIN(available_threads, BRN2_MAX_THREADS);

    switch (mode) {
    case FILES_FROM_FILE:
        brn2_list_from_lines(old, lines, true);
        break;
    case FILES_FROM_ARGS:
        brn2_list_from_args(old, argc - optind, &argv[optind]);
        break;
    case FILES_FROM_DIR:
        brn2_list_from_dir(old, directory);
        break;
    case FILES_FROM_DIR_RECURSE:
#ifdef __WIN32__
        error("Finding files recursively is not implemented on windows.\n");
        exit(EXIT_FAILURE);
#else
        brn2_list_from_dir_recurse(old, directory);
#endif
        break;
    default:
        error("Unexpected mode: %d.\n", mode);
        exit(EXIT_FAILURE);
    }
    if (!brn2_options_quiet)
        printf("Normalizing filenames...\n");
    brn2_normalize_names(old, NULL);

    for (uint32 i = 0; i < old->length; i += 1) {
        FileName **file = &(old->files[i]);
        while ((*file)->type == TYPE_ERR) {
            error("Removing '%s' from list.\n", (*file)->name);
            old->length -= 1;
            if (old->length <= i)
                break;
            memmove(file, file+1, (old->length - i)*sizeof(*file));
        }
    }

    if (old->length == 0) {
        error("No files to rename.\n");
        exit(EXIT_FAILURE);
    }

    if (brn2_options_sort)
        sort(old);

    if (!(EDITOR = getenv("EDITOR"))) {
        EDITOR = "vim";
        error("EDITOR variable is not set. Using %s by default.\n", EDITOR);
    }

    {
        char write_buffer[BRN2_PATH_MAX*2];
        char *pointer = write_buffer;
        uint32 capacity_set;
        static File brn2_buffer;
#ifndef __WIN32__
        char *temp = "/tmp";
#else
        char *temp = getenv("Temp");
        if (temp == NULL) {
            error("%%TEMP%% is not set.\n");
            exit(EXIT_FAILURE);
        }
#endif

        brn2_buffer_name = SNPRINTF(brn2_buffer.name,
                                    "%s/%s", temp, "brn2.XXXXXX");

        if ((brn2_buffer.fd = mkstemp(brn2_buffer_name)) < 0) {
            error("Error opening '%s': %s.\n",
                  brn2_buffer_name, strerror(errno));
            exit(EXIT_FAILURE);
        }

        oldlist_map = hash_map_create(old->length);
        capacity_set = hash_capacity(oldlist_map);
        old->indexes_size = old->length*sizeof(*(old->indexes));
        old->indexes = xmmap_commit(&(old->indexes_size));
        brn2_create_hashes(old, capacity_set);

        for (uint32 i = 0; i < old->length; i += 1) {
            FileName **filep = &(old->files[i]);
            FileName *file = *filep;
            uint32 *index = &(old->indexes[i]);
            bool contains_newline;
            usize written;

            while ((contains_newline = memchr(file->name, '\n', file->length))
                   || !hash_map_insert_pre_calc(oldlist_map, file->name,
                                                file->hash, *index, i)) {
                if (contains_newline)
                    error(RED"'%s'"RESET" contains new line.", file->name);
                else
                    error(RED"'%s'"RESET" repeated in the buffer.", file->name);
                if (brn2_options_fatal) {
                    error("\n");
                    exit(EXIT_FAILURE);
                }
                error(" Removing from list...\n");

                old->length -= 1;
                if (old->length <= i)
                    goto close;

                memmove(*filep, *filep+1, (old->length - i)*sizeof(*filep));
                memmove(index, index+1, (old->length - i)*sizeof(*index));
                filep = &(old->files[i]);
                index = &(old->indexes[i]);
            }

            written = (usize)(pointer - write_buffer);
            if (written >= BRN2_PATH_MAX) {
                write(brn2_buffer.fd, write_buffer, written);
                pointer = write_buffer;
            }

            file->name[file->length] = '\n';
            memcpy(pointer, file->name, file->length + 1);
            pointer += file->length + 1;
            file->name[file->length] = '\0';
        }
        close:
        write(brn2_buffer.fd, write_buffer, (usize)(pointer - write_buffer));
        if (close(brn2_buffer.fd) < 0) {
            error("Error closing:%s\n", strerror(errno));
        }
        brn2_buffer.fd = -1;
        atexit(delete_brn2_buffer);
    }

    {
        char *args_edit[] = { EDITOR, brn2_buffer_name, NULL };
        char *args_shuf[] = { "shuf", brn2_buffer_name,
                              "-o", brn2_buffer_name, NULL };
        (void) args_edit;
        (void) args_shuf;

#ifdef BRN2_BENCHMARK
        {
            char allowed[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "!@#$%&*()[]-=_+<>,"
                             "0123456789";
            util_command(ARRAY_LENGTH(args_shuf), args_shuf);
            brn2_list_from_lines(new, brn2_buffer_name, false);

            srand(42);
            for (uint32 i = 0; i < new->length; i += 1) {
                float x = (float)rand() / (float)RAND_MAX;
                uint32 length = new->files[i]->length;
                if (x < 0.4f) {
                    for (uint32 j = 0; j < length; j += 1) {
                        char c = allowed[(usize)rand() % (sizeof(allowed) - 1)];
                        new->files[i]->name[j] = c;
                    }
                }
            }
            brn2_normalize_names(old, new);

            newlist_set = hash_set_create(new->length);
            main_capacity = hash_capacity(newlist_set);
            new->indexes_size = new->length*sizeof(*(new->indexes));
            new->indexes = xmmap_commit(&(new->indexes_size));
            brn2_create_hashes(new, main_capacity);
            brn2_verify(new, newlist_set, new->indexes);
            hash_set_print_summary(newlist_set, "newlist_set");
        }
#else
        while (true) {
            util_command(ARRAY_LENGTH(args_edit), args_edit);
            brn2_list_from_lines(new, brn2_buffer_name, false);

            if (old->length != new->length) {
                error("You are renaming "RED"%u"RESET" file%.*s "
                      "but buffer contains "RED"%u"RESET" file name%.*s\n",
                      old->length, old->length != 1, "s",
                      new->length, new->length != 1, "s");
                brn2_free_list(new);
                printf("Fix your renames. Press control-c to cancel or press"
                       " ENTER to open the file list editor again.\n");
                getc(stdin);
                continue;
            }

            brn2_normalize_names(old, new);

            if (newlist_set == NULL) {
                newlist_set = hash_set_create(new->length);
            } else {
                hash_set_zero(newlist_set);
            }
            if (new->indexes == NULL) {
                new->indexes_size = new->length*sizeof(*(new->indexes));
                new->indexes = xmmap_commit(&(new->indexes_size));
            }

            main_capacity = hash_capacity(newlist_set);
            brn2_create_hashes(new, main_capacity);

            if (!brn2_verify(new, newlist_set, new->indexes)) {
                brn2_free_list(new);
                printf("Fix your renames. Press control-c to cancel or press"
                       " ENTER to open the file list editor again.\n");
                getc(stdin);
                continue;
            } else {
                break;
            }
        }
#endif
    }

    {
        uint32 number_changes = brn2_get_number_changes(old, new);
        uint32 number_renames = 0;

        if (number_changes) {
            HashSet *names_renamed = hash_set_create(old->length);

            if (brn2_options_quiet)
                print = noop;
            else
                print = printf;

            for (uint32 i = 0; i < old->length; i += 1) {
                brn2_execute2(old, new, oldlist_map, names_renamed,
                              i, &number_renames);
            }
            if (BRN2_DEBUG)
                hash_set_destroy(names_renamed);
        }
        if (number_changes != number_renames) {
            error("%u name%.*s changed but %u file%.*s renamed. "
                  "Check your files.\n",
                  number_changes, number_changes != 1, "s",
                  number_renames, number_renames != 1, "s");
            exit(EXIT_FAILURE);
        } else {
            printf("%u file%.*s renamed\n",
                   number_renames, number_renames != 1, "s");
        }
    }

#ifdef BRN2_BENCHMARK
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    brn2_timings("500000 renames", t0, t1, old->length);
#endif

    if (BRN2_DEBUG) {
        xmunmap(old->indexes, old->indexes_size);
        xmunmap(new->indexes, new->indexes_size);
        brn2_free_list(old);
        brn2_free_list(new);
        hash_map_destroy(oldlist_map);
        hash_set_destroy(newlist_set);
        arena_destroy(old->arena);
        arena_destroy(new->arena);
    }
    exit(EXIT_SUCCESS);
}
