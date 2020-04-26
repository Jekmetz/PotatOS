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

/**
 * @brief print boot sector information
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */ 
int pbsi_command(int argc, char **argv);

/**
 * @brief print root directory
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */ 
int prd_command(int argc, char **argv);

/**
 * @brief change directory to root directory
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */ 
int root_command(int argc, char **argv);

/**
 * @brief change directory to requested directory
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */ 
int cd_command(int argc, char **argv);

/**
 * @brief list directory
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */
int ls_command(int argc, char **argv);

/**
 * @brief prints out contents of file
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */
int type_command(int argc, char **argv);

/**
 * @brief renames a file or directory
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */
int rename_command(int argc, char **argv);

/**
 * @brief move a file to an adjacent directory
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */
int move_command(int argc, char **argv);

/**
 * @brief exit the m6 environement
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */
int exit_command(int argc, char **argv);

