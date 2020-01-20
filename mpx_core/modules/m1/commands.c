/*************************************************************
 *	This C file contains all of the commands that will be used
 *  by the command handler. All of the commands in this file
 *  should have the syntax:
 *  int cmd_<cmd_name>(char* params);
 **************************************************************/
#include <string.h>
#include <core/serial.h>

#define SUCCESS 0
//#include "commands.h"

/*
  Procedure: cmd_help
  Params: params-will serve as the params for each of these things
*/
int cmd_help(char* params) {
	
	char* token = strtok(params, " ");
	
	int help_argument = 1, counter = 0; // help_argument is the 2nd argument when using help, which is the command you are requesting help for	
	while(token != NULL && counter < help_argument){
		token = strtok(NULL, " ");
		counter++;
	}
  	
	// TODO: Decide if we want to take the token and take every character to lower case so we avoid the edge case of Help vs help
	if(strcmp(token, "help") == 0) {
		serial_println(
			"Help page for help command\n");
	}
	else if(strcmp(token, "version") == 0) {
		serial_println(
			"Help page for version command\n");
	}
	else if(strcmp(token, "shutdown") == 0) {
		serial_println(
			"Help page for shutdown command\n");
	}
	else if(strcmp(token, "date") == 0) {
		serial_println(
			"Help page for date command\n");
	}
	else if(strcmp(token, "time") == 0) {
		serial_println(
			"Help page for time command\n");
	}
	else{
		serial_println(
			"Something went wrong?\n");
	}

  return SUCCESS;  // successful response
}

// How should we do this? Is it so simple as to have it just print a line?
int cmd_version(char* params) {
	serial_println(
		"Version 1.?\tModule one");
}	
