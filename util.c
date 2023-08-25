/*
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

#include "stdlib.h"
#include "stdio.h"
#include <stdbool.h>
#include "string.h"
#include "errno.h"
#include "unistd.h"
#include "sys/wait.h"

#include "util.h"

void *util_realloc(void *old, const size_t size) {
    void *p;
    if ((p = realloc(old, size)) == NULL) {
        fprintf(stderr, "Failed to allocate %zu bytes.\n", size);
        if (old)
            fprintf(stderr, "Reallocating from: %p\n", old);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *util_calloc(const size_t nmemb, const size_t size) {
    void *p;
    if ((p = calloc(nmemb, size)) == NULL) {
        fprintf(stderr, "Failed to allocate %zu members of %zu bytes each.\n",
                        nmemb, size);
        exit(EXIT_FAILURE);
    }
    return p;
}

void util_command(int argc, char **argv) {
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    switch (fork()) {
    case 0:
        // Close the read end of the pipe (stdin for the child)
        close(pipe_fd[0]);

        // Redirect stdin to the write end of the pipe
        if (dup2(pipe_fd[1], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        // Close the write end of the pipe (not needed anymore)
        close(pipe_fd[1]);
        execvp(argv[0], argv);
        fprintf(stderr, "Error running '%s", argv[0]);
        for (int i = 1; i < argc; i += 1)
            fprintf(stderr, " %s", argv[i]);
        fprintf(stderr, "': %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    case -1:
        fprintf(stderr, "Error forking: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    default:
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        if (wait(NULL) < 0) {
            fprintf(stderr, "Error waiting for the forked child: %s\n", 
                            strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}
