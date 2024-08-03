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

static struct option options[] = {
    {"file",    required_argument, NULL, 'f'},
    {"help",    no_argument,       NULL, 'h'},
    {"quiet",   no_argument,       NULL, 'q'},
    {"verbose", no_argument,       NULL, 'v'},
    {"check",   no_argument,       NULL, 'c'},
    {"sort",    no_argument,       NULL, 's'},
    {NULL, 0, NULL, 0}
};

int main(int argc, char **argv) {
    File buffer;
    FileList *old;
    FileList *new;
    char *EDITOR;
    int opt;

    const char *tempdir = "/tmp";
    int status = EXIT_SUCCESS;
    bool quiet = false;
    bool check = false;
    bool sort = true;
    char *lines = NULL;

    program = basename(argv[0]);

    while ((opt = getopt_long(argc, argv, "f:chqvs", options, NULL)) != -1) {
        switch (opt) {
        case '?':
            brn2_usage(stderr);
        case 'h':
            brn2_usage(stdout);
        case 'c':
            check = true;
            break;
        case 'q':
            quiet = true;
            break;
        case 'v':
            quiet = false;
            break;
        case 's':
            sort = false;
            break;
        case 'f':
            if (optarg == NULL)
                brn2_usage(stderr);
            lines = optarg;
            break;
        default:
            brn2_usage(stderr);
        }
    }
    if (optind < argc && !strcmp(argv[optind], "--"))
        optind += 1;

    if (lines)
        old = brn2_list_from_lines(lines, 0);
    else if ((argc - optind) >= 1)
        old = brn2_list_from_args(argc - optind, &argv[optind]);
    else
        old = brn2_list_from_dir(".");

    if (sort)
        qsort(old->files, old->length, sizeof(*(old->files)), brn2_compare);

    brn2_normalize_names(old);
    if (check) {
        for (uint32 i = 0; i < old->length; i += 1) {
            FileName *file = &(old->files[i]);
            while (access(file->name, F_OK)) {
                error("\"%s\" can't be accessed: %s\n",
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
        int n;
        HashMap *repeated_map;
        Hash *hashes;

        n = snprintf(buffer.name, sizeof(buffer.name),
                    "%s/%s", tempdir, "brn2.XXXXXX");
        if (n < 0) {
            error("Error printing buffer name.\n");
            exit(EXIT_FAILURE);
        }
        buffer.name[sizeof(buffer.name) - 1] = '\0';

        if ((buffer.fd = mkstemp(buffer.name)) < 0) {
            error("Error opening \"%s\": %s\n", buffer.name, strerror(errno));
            exit(EXIT_FAILURE);
        }
        if ((buffer.stream = fdopen(buffer.fd, "w")) == NULL) {
            error("Error opening \"%s\": %s\n", buffer.name, strerror(errno));
            exit(EXIT_FAILURE);
        }

        repeated_map = hash_map_create(old->length);
        hashes = brn2_create_hashes_threads(old,
                                            hash_map_capacity(repeated_map));

        setvbuf(buffer.stream, buffer2, _IOFBF, BUFSIZ);
        for (uint32 i = 0; i < old->length; i += 1) {
            FileName *file = &(old->files[i]);
            Hash *hash = &hashes[i];

            while (!hash_set_insert_pre_calc(repeated_map, file->name,
                                             hash->hash, hash->mod)) {
                error(RED"\"%s\""RESET" repeated in the buffer. Removing...\n",
                      file->name);
                old->length -= 1;
                if (old->length <= i)
                    goto close;

                memmove(file, file+1, (old->length - i)*sizeof(*file));
                memmove(hash, hash+1, (old->length - i)*sizeof(*hash));
                file = &(old->files[i]);
                hash = &hashes[i];
            }
            file->name[file->length] = '\n';
            fwrite(file->name, 1, file->length + 1, buffer.stream);
            file->name[file->length] = '\0';
        }
        close:
        free(hashes);
        hash_map_destroy(repeated_map);

        fclose(buffer.stream);
        close(buffer.fd);
        buffer.fd = -1;
        buffer.stream = NULL;
    }

    {
        char *args_edit[] = { EDITOR, buffer.name, NULL };
        char *args_shuf[] = { "shuf", buffer.name, "-o", buffer.name, NULL };
        (void) args_edit;
        (void) args_shuf;

        while (true) {
#ifdef BRN2_BENCHMARK
            util_command(ARRAY_LENGTH(args_shuf), args_shuf);
#else
            util_command(ARRAY_LENGTH(args_edit), args_edit);
#endif
            new = brn2_list_from_lines(buffer.name, old->length);
            brn2_normalize_names(new);
            if (!brn2_verify(old, new)) {
                brn2_free_lines_list(new);
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
            number_renames = brn2_execute(old, new, number_changes, quiet);
        if (number_changes != number_renames) {
            error("%u name%.*s changed but %u file%.*s renamed. "
                  "Check your files.\n",
                  number_changes, number_changes != 1, "s",
                  number_renames, number_renames != 1, "s");
            status = EXIT_FAILURE;
        } else {
            fprintf(stdout, "%u file%.*s renamed\n",
                            number_renames, number_renames != 1, "s");
        }
    }

    brn2_free_lines_list(new);
    unlink(buffer.name);
    exit(status);
}
