/*************************************************************
 *	This C file contains the command handler
 **************************************************************/
#include<string.h>
#include<core/serial.h>

#include "../mpx_supt.h"
#include "commands.h"
#include "poll_input.h"

#define CMDSIZE 100

//Prototypes
int search_commands(char*);

/********COMMANDS AND FUNCTION DECLARATION*******/

//Initialize commands
char *commmands[] = 
{
	"help"
};

int (*functions[])(char *) =
{
	&cmd_help
};

/********END OF COMMANDS AND FUNCTION DECLARATION*******/


/*  Procedure:commandHandler()
    Description: handles commands typed by the user
*/
int command_handler()
{
	//Initialize vars
	char cmd[CMDSIZE];
	int exit = 0;
	int bufSize;
	int errCode;
	int cmdidx;

	sys_set_read(&poll_input);

	while(!exit)
	{
		serial_print("\n>> ");	//show prompt

		memset(cmd,'\0',CMDSIZE);	//set command as a bunch of \0s
		bufSize = CMDSIZE - 1;		//set bufSize as the command size - 1
		errCode = sys_req(READ,DEFAULT_DEVICE,cmd,&bufSize);
		
		/******SYS_REQ VALIDATION********/
		if(errCode != 0) //If there was something wrong with sys_req...
		{
			switch(errCode)
			{

			case INVALID_OPERATION:
				serial_println("Invalid Operation from command handler!");
				break;

			case INVALID_BUFFER:
				serial_println("Invalid Buffer from command handler!");
				break;

			case INVALID_COUNT:
				serial_println("Invalid Count from command handler!");
				break;
			}

			serial_println("Failure to execute command!");
			continue;	//move forward with your life!
		}
		/*******END SYS_REQ VALIDATION*******/

		//at this point, we know we have a good command

		if(strcmp(cmd,"exit") == 0)	//if they have typed in exit...
			exit = 1;				//set exit to true


		//TODO: search through array of commands and process correct function
		cmdidx = search_commands(cmd);
		//ex (*functions[cmdidx])(cmd)
		(*functions[cmdidx])("");


	}

	serial_println("Exiting prompt! Goodbye and have a splendiferous day!");

	return 0;
}

int search_commands(char* cmd)
{
	//TODO: search through cmd
	if(*cmd == 'a')
	{}

	return 0;
}