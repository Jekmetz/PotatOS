/**
* @file commands.h
*
* @brief Holds all of the command prototypes
*/

#pragma once

#include <stdint.h>

#include "command_utils.h"

/**
 * @brief Search commands for a given command
 *
 * @param command A command name to search for
 *
 * @return Pointer to command if found, else NULL
 */
command_t *search_commands(char *command);

/**
 * @brief Command to get help about other commands
 *
 * @param argc Number of arguments
 * @param argv Pointer to arguments
 *
 * @return exit status code
 */
int help_command(int argc, char **argv);

int pbsi_command(int argc, char **argv);

int prd_command(int argc, char **argv);

int root_command(int argc, char **argv);

int cd_command(int argc, char **argv);

int ls_command(int argc, char **argv);

int type_command(int argc, char **argv);

int rename_command(int argc, char **argv);

int move_command(int argc, char **argv);

int exit_command(int argc, char **argv);

