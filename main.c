/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023-2024 Mior, Lucas; 
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
bool brn2_options_fatal = false;
bool brn2_options_implicit = false;
bool brn2_options_quiet = false;
bool brn2_options_sort = true;
uint32 nthreads;

Arena *arena_old;
Arena *arena_new;

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
    unlink(brn2_buffer_name);
    return;
}

int main(int argc, char **argv) {
    FileList *old;
    FileList *new;
    uint32 *indexes_old = NULL;
    uint32 *indexes_new = NULL;
    usize indexes_old_size = 0;
    usize indexes_new_size = 0;
    HashMap *oldlist_map = NULL;
    HashMap *newlist_map = NULL;
    long available_threads;

    uint32 main_capacity;
    char *EDITOR;
    int opt;

#ifdef BRN2_BENCHMARK
    struct timespec t0;
    struct timespec t1;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
#endif

    EDITOR = mmap(
        .addr=NULL,
        .size=4096,
        .prot=PROT_READ,
        .flags=MAP_PRIVATE,
        .fd=-1,
        .offset=0
    );

    char *directory = ".";
    char *lines = NULL;
    int mode = FILES_FROM_DIR;
    (void) available_threads;

    program = basename(argv[0]);
    error("parsing\n");

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

#ifndef __WIN32__
    available_threads = sysconf(_SC_NPROCESSORS_ONLN);
    if (available_threads <= 0)
        nthreads = 1; 
    else
        nthreads = MIN((uint32)available_threads, BRN2_MAX_THREADS);
#endif

    arena_old = arena_alloc("arena for old filenames", PATH_MAX*UINT32_MAX);
    arena_new = arena_alloc("arena for new filenames", PATH_MAX*UINT32_MAX);

    switch (mode) {
    case FILES_FROM_FILE:
        old = brn2_list_from_lines(lines, true);
        break;
    case FILES_FROM_ARGS:
        old = brn2_list_from_args(argc - optind, &argv[optind]);
        break;
    case FILES_FROM_DIR:
        old = brn2_list_from_dir(directory);
        break;
    case FILES_FROM_DIR_RECURSE:
#ifdef __WIN32__
        error("Finding files recursively is not implemented on windows.\n");
        exit(EXIT_FAILURE);
#else
        old = brn2_list_from_dir_recurse(directory);
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
        FileName *file = &(old->files[i]);
        while (file->type == TYPE_ERR) {
            error("Removing '%s' from list.\n", file->name);
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
        char write_buffer[PATH_MAX*2];
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
        indexes_old_size = old->length*sizeof(*indexes_old);
        indexes_old = xmmap_commit(&indexes_old_size);
        brn2_create_hashes(old, indexes_old, capacity_set);

        for (uint32 i = 0; i < old->length; i += 1) {
            FileName *file = &(old->files[i]);
            uint32 *index = &indexes_old[i];
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

                memmove(file, file+1, (old->length - i)*sizeof(*file));
                memmove(index, index+1, (old->length - i)*sizeof(*index));
                file = &(old->files[i]);
                index = &indexes_old[i];
            }

            written = (usize)(pointer - write_buffer);
            if (written >= (sizeof(write_buffer)/2)) {
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

        while (true) {
#ifdef BRN2_BENCHMARK
            char allowed[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "!@#$%&*()[]-=_+<>,"
                             "0123456789";
            util_command(ARRAY_LENGTH(args_shuf), args_shuf);
            new = brn2_list_from_lines(brn2_buffer_name, false);

            srand(42);
            for (uint32 i = 0; i < new->length; i += 1) {
                float x = (float)rand() / (float)RAND_MAX;
                uint32 length = new->files[i].length;
                if (x < 0.4f) {
                    for (uint32 j = 0; j < length; j += 1) {
                        char c = allowed[(usize)rand() % (sizeof(allowed) - 1)];
                        new->files[i].name[j] = c;
                    }
                }
            }
            brn2_normalize_names(old, new);

            newlist_map = hash_map_create(new->length);
            main_capacity = hash_capacity(newlist_map);
            indexes_new_size = new->length*sizeof(*indexes_new);
            indexes_new = xmmap_commit(&indexes_new_size);
            brn2_create_hashes(new, indexes_new, main_capacity);
            brn2_verify(new, newlist_map, indexes_new);
            hash_map_print_summary(newlist_map, "newlist_map");
            break;
#else
            util_command(ARRAY_LENGTH(args_edit), args_edit);
            new = brn2_list_from_lines(brn2_buffer_name, false);
#endif
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

            if (newlist_map == NULL) {
                newlist_map = hash_map_create(new->length);
            } else {
                hash_map_zero(newlist_map);
            }
            if (indexes_new == NULL) {
                indexes_new_size = new->length*sizeof(*indexes_new);
                indexes_new = xmmap_commit(&indexes_new_size);
            }

            main_capacity = hash_capacity(newlist_map);
            brn2_create_hashes(new, indexes_new, main_capacity);

            if (!brn2_verify(new, newlist_map, indexes_new)) {
                brn2_free_list(new);
                printf("Fix your renames. Press control-c to cancel or press"
                       " ENTER to open the file list editor again.\n");
                getc(stdin);
                continue;
            } else {
                break;
            }
        }
    }

    {
        uint32 number_changes = brn2_get_number_changes(old, new);
        uint32 number_renames = 0;

        if (number_changes)
            number_renames = brn2_execute(old, new,
                                          oldlist_map,
                                          indexes_old, indexes_new);
        if (number_changes != number_renames) {
            error("%u name%.*s changed but %u file%.*s renamed. "
                  "Check your files.\n",
                  number_changes, number_changes != 1, "s",
                  number_renames, number_renames != 1, "s");
            exit(EXIT_FAILURE);
        } else {
            fprintf(stdout, "%u file%.*s renamed\n",
                            number_renames, number_renames != 1, "s");
        }
    }

#ifdef BRN2_BENCHMARK
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    brn2_timings("500000 renames", t0, t1, old->length);
#endif

    if (BRN2_DEBUG) {
        xmunmap(indexes_old, indexes_old_size);
        xmunmap(indexes_new, indexes_new_size);
        brn2_free_list(old);
        brn2_free_list(new);
        hash_map_destroy(oldlist_map);
        hash_map_destroy(newlist_map);
        arena_destroy(arena_old);
        arena_destroy(arena_new);
    }
    exit(EXIT_SUCCESS);
}
