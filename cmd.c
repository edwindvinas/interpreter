#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "cmd.h"

Options* parse_cmd(int argc, char *argv[]) {
    Options* options = malloc(sizeof(Options));

    int c;
    while ((c = getopt (argc, argv, "d")) != -1) {
        switch(c) {
        case 'd':
            options->debug = 1;
            break;
        default:
            options->debug = 0;
            break;
        }
    }

    if (optind >= argc) {
        printf("Please provide a path to a program as the first parameter!\n");
        exit(EXIT_FAILURE);
    }

    options->file = argv[optind];
    return options;
}