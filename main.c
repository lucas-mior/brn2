/*
 * Copyright (C) 2022 Patel, Nimai <nimai.m.patel@gmail.com>
 * Author: Patel, Nimai <nimai.m.patel@gmail.com>
 * Copyright (C) 2023-2024 Mior, Lucas; <lucasbuddemior@gmail.com>
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
#include "hash.h"
#include "util.h"

#include "brn2.c"
#include "hash.c"
#include "util.c"

char *program;
static bool brn2_check = false;
bool brn2_fatal = false;
bool brn2_implict = false;
bool brn2_quiet = false;
bool brn2_sort = true;
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

static File buffer;

static void
delete_buffer(void) {
    unlink(buffer.name);
    return;
}

int main(int argc, char **argv) {
    FileList *old;
    FileList *new;
    uint32 *hashes_old;
    uint32 *hashes_new;
    HashSet *oldlist_map;
    HashSet *newlist_map;
    long available_threads;

    uint32 main_capacity;
    char *EDITOR;
    int opt;

    const char *tempdir = "/tmp";
    char *directory = ".";
    char *lines = NULL;
    int mode = FILES_FROM_DIR;

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
            brn2_check = true;
            break;
        case 'e':
            brn2_implict = false;
            break;
        case 'F':
            brn2_fatal = true;
            break;
        case 'h':
            brn2_usage(stdout);
        case 'i':
            brn2_implict = true;
            break;
        case 'q':
            brn2_quiet = true;
            break;
        case 's':
            brn2_sort = false;
            break;
        case 'v':
            brn2_quiet = false;
            break;
        default:
            brn2_usage(stderr);
        }
    }
    if (optind < argc && !strcmp(argv[optind], "--"))
        optind += 1;
    if ((argc - optind) >= 2)
        mode = FILES_FROM_ARGS;

    available_threads = sysconf(_SC_NPROCESSORS_ONLN);
    if (available_threads <= 0)
        nthreads = 1; 
    else
        nthreads = MIN((uint32)available_threads, MAX_THREADS);

    arena_new = arena_alloc(PATH_MAX*UINT32_MAX);
    arena_old = arena_alloc(PATH_MAX*UINT32_MAX);

    switch (mode) {
    case FILES_FROM_FILE:
        old = brn2_list_from_lines(lines, 0);
        break;
    case FILES_FROM_ARGS:
        old = brn2_list_from_args(argc - optind, &argv[optind]);
        break;
    case FILES_FROM_DIR_RECURSE:
        old = brn2_list_from_dir_recurse(directory);
        break;
    case FILES_FROM_DIR:
        old = brn2_list_from_dir(directory);
        break;
    default:
        error("Unexpected mode: %d\n", mode);
        exit(EXIT_FAILURE);
    }
    brn2_normalize_names(old, NULL);

    if (brn2_sort)
        qsort(old->files, old->length, sizeof(*(old->files)), brn2_compare);

    if (brn2_check) {
        for (uint32 i = 0; i < old->length; i += 1) {
            FileName *file = &(old->files[i]);
            while (access(file->name, F_OK)) {
                error("'%s' can't be accessed: %s\n",
                      file->name, strerror(errno));
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
    }

    if (!(EDITOR = getenv("EDITOR"))) {
        EDITOR = "vim";
        error("EDITOR variable is not set. Using %s by default.\n", EDITOR);
    }

    {
        char buffer2[BUFSIZ];
        uint32 capacity_set;
        int n;

        n = snprintf(buffer.name, sizeof(buffer.name),
                    "%s/%s", tempdir, "brn2.XXXXXX");
        if (n < 0) {
            error("Error printing buffer name.\n");
            exit(EXIT_FAILURE);
        }
        buffer.name[sizeof(buffer.name) - 1] = '\0';

        if ((buffer.fd = mkstemp(buffer.name)) < 0) {
            error("Error opening '%s': %s\n", buffer.name, strerror(errno));
            exit(EXIT_FAILURE);
        }
        if ((buffer.stream = fdopen(buffer.fd, "w")) == NULL) {
            error("Error opening '%s': %s\n", buffer.name, strerror(errno));
            exit(EXIT_FAILURE);
        }

        oldlist_map = hash_map_create(old->length);
        capacity_set = hash_map_capacity(oldlist_map);
        hashes_old = brn2_create_hashes(old, capacity_set);

        setvbuf(buffer.stream, buffer2, _IOFBF, BUFSIZ);
        for (uint32 i = 0; i < old->length; i += 1) {
            FileName *file = &(old->files[i]);
            uint32 *hash = &hashes_old[i];

            while (!hash_map_insert_pre_calc(oldlist_map, file->name,
                                             file->hash, *hash, i)) {
                error(RED"'%s'"RESET" repeated in the buffer. Removing...\n",
                      file->name);
                old->length -= 1;
                if (old->length <= i)
                    goto close;

                memmove(file, file+1, (old->length - i)*sizeof(*file));
                memmove(hash, hash+1, (old->length - i)*sizeof(*hash));
                file = &(old->files[i]);
                hash = &hashes_old[i];
            }

            file->name[file->length] = '\n';
            fwrite(file->name, 1, file->length + 1, buffer.stream);
            file->name[file->length] = '\0';
        }
        close:
        fclose(buffer.stream);
        close(buffer.fd);
        buffer.fd = -1;
        buffer.stream = NULL;
        atexit(delete_buffer);
    }

    {
        char *args_edit[] = { EDITOR, buffer.name, NULL };
        char *args_shuf[] = { "shuf", buffer.name, "-o", buffer.name, NULL };
        (void) args_edit;
        (void) args_shuf;

        while (true) {
#ifdef BRN2_BENCHMARK
            struct timespec t;
            char allowed[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "!@#$%&*()[]-=_+<>,"
                             "0123456789";
            util_command(ARRAY_LENGTH(args_shuf), args_shuf);
            new = brn2_list_from_lines(buffer.name, old->length);

            clock_gettime(CLOCK_MONOTONIC_RAW, &t);
            srand((uint)t.tv_nsec);
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
            main_capacity = hash_map_capacity(newlist_map);
            hashes_new = brn2_create_hashes(new, main_capacity);
            brn2_verify(new, newlist_map, hashes_new);
            break;
#else
            util_command(ARRAY_LENGTH(args_edit), args_edit);
            new = brn2_list_from_lines(buffer.name, old->length);
#endif
            if (old->length != new->length) {
                error("You are renaming "RED"%u"RESET" file%.*s "
                      "but buffer contains "RED"%u"RESET" file name%.*s\n",
                      old->length, old->length != 1, "s",
                      new->length, new->length != 1, "s");
                exit(EXIT_FAILURE);
            }

            brn2_normalize_names(old, new);
            newlist_map = hash_map_create(new->length);
            main_capacity = hash_map_capacity(newlist_map);
            hashes_new = brn2_create_hashes(new, main_capacity);

            if (!brn2_verify(new, newlist_map, hashes_new)) {
                brn2_free_list(new);
                hash_map_destroy(newlist_map);
                free(hashes_new);
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
        bool *newname_exists = brn2_newname_exists(new);

        if (number_changes)
            number_renames = brn2_execute(old, new,
                                          oldlist_map,
                                          hashes_old, hashes_new, newname_exists);
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

    if (BRN2_DEBUG) {
        brn2_free_list(old);
        brn2_free_list(new);
        hash_map_destroy(oldlist_map);
        hash_map_destroy(newlist_map);
        free(hashes_old);
        free(hashes_new);
    }
    exit(EXIT_SUCCESS);
}
