#pragma once

typedef struct options {
    int debug;
    char* file;
} Options;

Options* parse_cmd(int argc, char *argv[]);