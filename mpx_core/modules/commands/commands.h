/**
* @file commands.h
*
* @brief The header file for commands.c
*/

#pragma once

/*
 *  FIXME: Remove this
 */
//int cmd_test_pcb(char *params);

/**
* @brief The help command will show a page to assist users with commands
*
* The help command can be called to do one of two things
*   List all the commands that have help pages
*   Request a help page for a certain command
*
*
* @param params param string typed by user
*
* @return A help page
*/
int cmd_help(char* params);

/**
* @brief The version command will show the version information
*
* The version command can be called to display the version information.
* The shortened return will just show the short version.
* The long return will include the current module, the version, and the
* contributing developers
*
* @param params param string typed by user
*
* @return A version page
*/
int cmd_version(char* params);

/**
* @brief shutdown the PotatOS
*
* @param params string passed from command handler
*
* @return SUCCESS or FAILURE
*/ 
int cmd_shutdown(char* params);

/**
* @brief The date command will do one of two things.
*   Show the current system date
*   Set a new system date
*
* The date command can be used to query the systems RTC to display the current
* date. It can also be used to set the systems RTC to a desired date. There is
* code to check for illegal dates such as Feb 30 on a non leap year.
*
* @param params param string typed by user
*
* @return The current system date
*
* @warning The RTC only allows dates between 1700-2999
*/
int cmd_date(char* params);

/**
* @brief The time command will do one of two things.
*   Show the current system time
*   Set a new system time
*
* The time command can be used to query the systems RTC to display the current
* time. It can also be used to set the systems RTC to a desired time. There is
* code to check for illegal times.
*
* @param params param string typed by user
*
* @return The current system time
*
* @note The time is kept in 24 hour time
*/
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

/**
* @brief Create a new pcb
*
* @param params string passed from command handler
*
* @return SUCCESS or FAILURE
*/
int cmd_create_pcb(char *params);

/**
* @brief Unblock a pcb
*
* @param params string passed from command handler
*
* @return SUCCESS or FAILURE
*/
int cmd_unblock_pcb(char *params);

/**
* @brief command to block PCB by name
*
* @returns Success or Failure
*/
int cmd_blockPCB(char* params);

/**
* @brief Resume PCB command
*
* @return SUCCESS or FAILURE
*/
int cmd_resume(char* params);

/**
* @brief Suspend PCB command
*
* @return SUCCESS or FAILURE
*/
int cmd_suspend(char* params);

/**
* @brief Show PCB command
*/
int cmd_show_pcb(char* params);

/**
* @brief Show all PCBs command
*/
int cmd_show_all_pcbs(char* params);

/**
* @brief Show ready PCBs command
*/
int cmd_show_ready_pcbs(char* params);

/**
* @brief Show blocked PCBs command
*/
int cmd_show_blocked_pcbs(char* params);

/**
* @brief command to delete PCB by name
*
* @returns Success if the PCB was removed, failure for anything else
*/
int cmd_delete_pcb(char* params);

/**
* @brief command to set PCB priority
*
* @returns Success if the PCB priority was updated, failure for anything else
*/
int cmd_set_priority_pcb(char* params);

/**
* @brief command to draw the potat
*/
int cmd_potat(char* params);

/**
* @brief command to load r3 procs
*
* @returns SUCCESS
*/
int cmd_loadr3(char* params);

/**
* @brief command to yield control from commhand
*
* @returns SUCCESS
*/
int cmd_yield(char* params);

int cmd_alias(char* params);