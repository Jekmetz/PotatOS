#pragma once

#include "command_utils.h"

command_t *search_commands(char *command);

int help_command(int argc, char **argv);

int pbsi_command(int argc, char **argv);

int prd_command(int argc, char **argv);

int cd_command(int argc, char **argv);

int ls_command(int argc, char **argv);

int type_command(int argc, char **argv);

int rename_command(int argc, char **argv);

int move_command(int argc, char **argv);

int exit_command(int argc, char **argv);
