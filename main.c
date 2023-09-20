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

#include "brn2.h"
#include "hash.h"

static struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"quiet", no_argument, NULL, 'q'},
    {"verbose", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}
};

int main(int argc, char **argv) {
    File buffer;
    FileList *old;
    FileList *new;
    char *EDITOR;
    const char *tempdir = "/tmp";
    bool status = true;
    bool quiet = false;
    int opt;

    while ((opt = getopt_long(argc, argv, "hqv", long_options, NULL)) != -1) {
        switch (opt) {
        case '?':
            brn2_usage(stdout);
        case 'h':
            brn2_usage(stdout);
        case 'q':
            quiet = true;
            break;
        case 'v':
            quiet = false;
            break;
        }
    }
    if (optind < argc && !strcmp(argv[optind], "--")) {
        optind += 1;
    }

    if (argc - optind > 1) {
        old = brn2_list_from_args(argc - optind, &argv[optind]);
    } else if (argc - optind == 1) {
        old = brn2_list_from_lines(argv[optind], 0);
    } else {
        old = brn2_list_from_dir(".");
    }

    brn2_normalize_names(old);

    if (!(EDITOR = getenv("EDITOR"))) {
        EDITOR = "vim";
        fprintf(stderr, "EDITOR variable is not set. "
                        "Using %s by default.\n", EDITOR);
    }

    {
        char buffer2[BUFSIZ];
        int n;
        HashMap *repeated = hash_map_create(old->length);

        n = snprintf(buffer.name, sizeof (buffer.name),
                    "%s/%s", tempdir, "brn2.XXXXXX");
        if (n < 0) {
            fprintf(stderr, "Error printing buffer name.\n");
            exit(EXIT_FAILURE);
        }
        buffer.name[sizeof (buffer.name) - 1] = '\0';

        if ((buffer.fd = mkstemp(buffer.name)) < 0) {
            fprintf(stderr, "Error opening \"%s\": %s\n",
                            buffer.name, strerror(errno));
            exit(EXIT_FAILURE);
        }
        if ((buffer.stream = fdopen(buffer.fd, "w")) == NULL) {
            fprintf(stderr, "Error opening \"%s\": %s\n",
                            buffer.name, strerror(errno));
            exit(EXIT_FAILURE);
        }

        setvbuf(buffer.stream, buffer2, _IOFBF, BUFSIZ);
        for (uint32 i = 0; i < old->length; i += 1) {
            FileName *file = &(old->files[i]);

            while (!hash_set_insert(repeated, file->name)) {
                fprintf(stderr, RED"\"%s\""RESET" repeated in the buffer. "
                                "Removing...\n", file->name);
                old->length -= 1;
                if (old->length <= i)
                    goto close;

                memmove(file, file+1, (old->length - i) * sizeof(*file));
                file = &(old->files[i]);
            }
            file->name[file->length] = '\n';
            fwrite(file->name, 1, file->length + 1, buffer.stream);
            file->name[file->length] = '\0';
        }
        close:
        hash_map_destroy(repeated);
        fclose(buffer.stream);
        close(buffer.fd);
        buffer.fd = -1;
        buffer.stream = NULL;
    }

    {
        char *args[] = { EDITOR, buffer.name, NULL };

        while (true) {
            util_command(ARRAY_LENGTH(args), args);
            new = brn2_list_from_lines(buffer.name, old->length);
            brn2_normalize_names(new);
            if (!brn2_verify(old, new)) {
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
            number_renames = brn2_execute(old, new, number_changes, quiet);
        if (number_changes != number_renames) {
            fprintf(stderr, "%u name%.*s changed but %u file%.*s renamed. "
                            "Check your files.\n",
                            number_changes, number_changes != 1, "s",
                            number_renames, number_renames != 1, "s");
            status = false;
        } else {
            fprintf(stdout, "%u file%.*s renamed\n",
                            number_renames, number_renames != 1, "s");
        }
    }

    brn2_free_list(old);
    brn2_free_list(new);
    unlink(buffer.name);
    exit(!status);
}
