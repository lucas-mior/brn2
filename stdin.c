#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define error(...) fprintf(stderr, __VA_ARGS__)

int
main(void) {
    FILE *file;
    char buffer[1024];
    int c;

    if ((file = fopen("/dev/stdin", "r")) == NULL) {
        error("Error opening: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, sizeof(buffer), file)) {
        error("from file: %s", buffer);
        if (feof(file)) {
            error("end of file.\n");
            break;
        } else {
            error("NOT end of file.\n");
            continue;
        }
    }
    error("End of stream\n");
    fclose(file);

    /* if (!freopen("/dev/tty", "r", stdin)) { */
    /*     error("Error in freopen.\n"); */
    /* } */

    if ((c = fgetc(stdin)) == EOF) {
        printf("Error in fgetc\n");
    } else {
        printf("c: %d\n", c);
    }
    exit(EXIT_SUCCESS);
}
