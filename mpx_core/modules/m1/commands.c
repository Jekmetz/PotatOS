/*************************************************************
 *	This C file contains all of the commands that will be used
 *  by the command handler. All of the commands in this file
 *  should have the syntax:
 *  int cmd_<cmd_name>(char* params);
 **************************************************************/
#include<string.h>
#include<core/serial.h>

#define SUCCESS 0
//#include "commands.h"


/*
  Procedure: cmd_help
  Params: params-will serve as the params for each of these things
*/
int cmd_help(char* params)
{
	//TODO: split params on \s+ and deal with them separately...
	if(strcmp(params,""))
	{}

	serial_println(
		"This will serve as the help menu \n"
		"This is line one of the help menu\n"
	);

	return SUCCESS; //successful response
}