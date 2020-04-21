#pragma once

typedef int (*CommandFunction)(int, char **);

struct command {
    char *command_name;
    CommandFunction function;
};

typedef struct command command_t;

char *trim_whitespace(char *str);

int split_args(char *command, char **argv);
