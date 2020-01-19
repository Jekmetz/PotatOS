/*************************************************************
 *	This C file contains the command handler
 **************************************************************/
#include<string.h>
#include<core/serial.h>
#include<core/utility.h>

#include "../mpx_supt.h"
#include "commands.h"
#include "poll_input.h"

#define CMDSIZE 100

//Prototypes
int search_commands(char*);

/********COMMANDS AND FUNCTION DECLARATION*******/

//Initialize commands
char *commands[] = 
{ 
	"help",
	NULL
};

int (*functions[])(char *) =
{
	&cmd_help,
	NULL
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

		//if they have typed in exit... (in the future maybe make a more elegant space-killing solution)
		if(strcmp(cmd,"exit") == 0)	exit = 1; //set exit to true

		cmdidx = search_commands(cmd);

		if(cmdidx == -1) //if we did not find anything...
		{	char ret [CMDSIZE + 18];
			sprintf(ret,"Invalid Command: %s\n",cmd);
			serial_println(ret);
		} else //if we did find something...
		{
			(*functions[cmdidx])("");
		}

	}

	serial_println("Exiting prompt! Goodbye and have a splendiferous day!");

	return 0;
}

int search_commands(char* cmd)
{
	int cmdidx;
	unsigned char found;
	char *cmdStart;
	char *testCmd;
	//remove spaces from beginning of cmd
	while(*cmd == ' ')
	{	cmd++;  }

	cmdStart = cmd;	//save starting position for command
	cmdidx = 0;		//start at command 0
	found = 0;		//we didn't find anything yet

	//while we have another command ahead to process
	//and we have not found a match yet...
	while(commands[cmdidx] != NULL && !found)
	{
		testCmd = commands[cmdidx];	//*testCmd is easier than writing *commands[cmdidx] :)

		//while each character matches and they are not null or space...
		while( (*testCmd == *cmd && !isnullorspace(*cmd)) ) 
		{	//increment both
			testCmd++;
			cmd++;
		}

		if(isnullorspace(*cmd)) //if cmd is pointing to a space or a null..
		{	//this means that testCmd matched until a space and we have ourselves a match!
			found = 1;
		} else //if we did not find a match...
		{
			cmdidx++;	//increment cmdidx!
		}

		cmd = cmdStart;	//reset cmd back to the beginning
	}

	//if we did not find anything...
	if(commands[cmdidx] == NULL) cmdidx = -1;	//set cmdidx to -1

	return cmdidx;
}