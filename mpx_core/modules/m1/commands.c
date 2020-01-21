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
	
	// Arbitrary cap on 4 arguments, we can add more	
	char* command = strtok(params, " "); // The primary command
	char* arg1 = strtok(NULL, " "); 
	char* arg2 = strtok(NULL, " ");
	char* arg3 = strtok(NULL, " ");
	char* arg4 = strtok(NULL, " ");
	
  	
	// TODO: Decide if we want to take the token and take every character to lower case so we avoid the edge case of Help vs help
	if(strcmp(arg1, "help") == 0) {
		serial_println("Help page for help command\n");
	}
	else if(strcmp(arg1, "version") == 0) {
		serial_println("Help page for version command\n");
	}
	else if(strcmp(arg1, "shutdown") == 0) {
		serial_println("Help page for shutdown command\n");
	}
	else if(strcmp(arg1, "date") == 0) {
		serial_println("Help page for date command\n");
	}
	else if(strcmp(arg1, "time") == 0) {
		serial_println("Help page for time command\n");
	}
	else{
		serial_println("Something went wrong?\n");
	}

  	return SUCCESS;  // successful response
}

// How should we do this? Is it so simple as to have it just print a line?
int cmd_version(char* params) {
	serial_println("Version 1.?\tModule one\nDevelopers: Hasan Ibraheem, Henry Vos, Jay Kmetz, Nicholas Fryer\n");
}

int cmd_shutdown(char* params) {
	// Do we need to implement this here? 
	// It is already implemented in comamnd_handler.c
}

int cmd_date(char* params) {
	// TODO: Actuallt implement date, this is just the prelim framework
	// Arbitrary cap on 4 arguments, we can add more	
	char* command = strtok(params, " "); // The primary command
	char* arg1 = strtok(NULL, " "); 
	char* arg2 = strtok(NULL, " ");
	char* arg3 = strtok(NULL, " ");
	char* arg4 = strtok(NULL, " ");
	
	// Assuming that the only flag for this will be -s, set value, arg1 will be -s while arg2-4 will be the month-day-year
	if(arg1 == NULL) { // Date called with no args, just requesting current date
		serial_println("The date is\n");
	}
	else if(strcmp(arg1, "-s") == 0 && (arg2 == NULL || arg3 == NULL || arg4 == NULL)) { // If set date arg is passed without the values
		serial_println("To set date, enter valid dates for month:day:year\n");
	}
	else if(strcmp(arg1,"-s") == 0 && arg2 != NULL && arg3 != NULL && arg4 != NULL){ //If set date arg is passed and the values are not null
		// Coded to use number dates, could implement 3 letter month
		int month = atoi(arg2);
		int day = atoi(arg3);
		int year = atoi(arg4);
		// More will need to be done here due to the complexities of dates an uneven months, damn gregorian calander
		serial_println("month=%d\tday=%d\tyear=%d\n");	
	}	
}

int cmd_time(char* params) {
	// TODO: Actually implement time with the registry, this is just the prelim framework 
	// Arbitrary cap on 4 arguments, we can add more	
	char* command = strtok(params, " "); // The primary command
	char* arg1 = strtok(NULL, " "); 
	char* arg2 = strtok(NULL, " ");
	char* arg3 = strtok(NULL, " ");
	char* arg4 = strtok(NULL, " ");

	// Assuming that the only flag for this will be -s, set value, arg1 will be -s while arg2-4 will be hour:minute:second (24 hour time)
	if(arg1 == NULL) { // Time was called with no args, just requesting current time
		serial_println("The time is\n");
	}
	else if(strcmp(arg1, "-s") == 0 && (arg2 == NULL || arg3 == NULL || arg4 == NULL)) { // If set time arg is set without the values
		serial_println("To set time, enter a valid value for hours:minutes:seconds\n");
	}
	else if(strcmp(arg1, "-s") == 0 && arg2 != NULL && arg3 != NULL && arg4 != NULL) { // If set time arg is set and values are not null
		// Time string -> int
		int hours =  atoi(arg2);
		int minutes = atoi(arg3);
		int seconds = atoi(arg4);
		
		if(hours > 24 || hours < 0 || minutes > 60 || minutes < 0 || seconds > 60 || seconds < 0) { // Checking for valid times
			serial_println("Please enter a valid value for hours:minutes:seconds\n");
		}
		else{
			serial_println("Set time as hours=%d, minutes=%d, seconds=%d\n",hours,minutes,seconds);
		}
	}
}
