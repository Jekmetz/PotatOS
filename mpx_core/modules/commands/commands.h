/**
* @file commands.h
*
* @brief The header file for commands.c
*/

/**
* @brief Macro to return a 0 on success
*/
#define SUCCESS 0


/////////////////////////////////////////////////
/**
 *  FIXME: Remove this
 */
int cmd_test_pcb(char *params);
/////////////////////////////////////////////////

int cmd_help(char* params);

int cmd_version(char* params);

int cmd_shutdown(char* params);

int cmd_date(char* params);

int cmd_time(char* params);

int cmd_test(char* params);

/**
* @brief clears the screen and sets the pointer at home
*
* @param params param string typed by user
*
* @return SUCCESS or FAILURE
*/
int cmd_clear(char* params);
