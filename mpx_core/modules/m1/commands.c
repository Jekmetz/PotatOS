/*************************************************************
 *	This C file contains all of the commands that will be used
 *  by the command handler. All of the commands in this file
 *  should have the syntax:
 *  int cmd_<cmd_name>(char* params);
 **************************************************************/
#include <string.h>
#include <core/serial.h>

#include <core/utility.h>

#define CMDSIZE 100

#define SUCCESS 0
#define FAILURE -1
#define MAXPARAMCOUNT 10

// cmd_help flags
#define FULLFLAG (1 << 0)
#define ONELINEFLAG (1 << 1)

// Global variables
char gparamstr[CMDSIZE];
char* gparams[MAXPARAMCOUNT];  // will hold all of the parameters

// private function prototypes

/* Procedure: set_gparams
   Description: Should be called at the beginning of each command function
                                to set the params based on the param string.
   gparams[i] will
                                be the (i)th parameter passed in by the user
   after calling
                                this function. Will mutate the string passed in
   by params to
                                save space
   Params: params-string of parameters delimited by space charaters,
   pcount-pointer
                   to integer representing the parameter count
*/
int set_gparams(char* params, int* pcount);

/*
  Procedure: cmd_help
  Params: params-will serve as the params for each of these things
*/

int cmd_help(char* params) {
  // Init vars
  int pcount;
  unsigned char flag = 0;

  if (set_gparams(params, &pcount) != SUCCESS)  // if setting gparams failed...
  {
    // we know it is because too many parameters
    char ret[100];
    sprintf(ret, "There were more than %d parameters. Command FAILURE.",
            MAXPARAMCOUNT);
    serial_println(ret);
    return FAILURE;
  }
  // if we are here, params are in gparams! parameter count is in pcount

  // Maybe unnecessary for help but to get the idea across
  // feel free to delete this and the adjacent 'help help' response
  for (int i = 0; i < pcount; i++)  // for every parameter...
  {
    // set the correct flag if it is there
    if ((strcmp(gparams[i], "-f") == 0) ||
        (strcmp(gparams[i], "--full") == 0)) {  // FULLLINE
      flag = flag | FULLFLAG;
    } else if ((strcmp(gparams[i], "-o") == 0) ||
               (strcmp(gparams[i], "--oneline") == 0)) {  // ONELINE
      flag = flag | ONELINEFLAG;
    }
  }

  if (strcmp(gparams[1], "help") == 0) {
    serial_println("Help page for help command\n");
    if (flag & FULLFLAG)  // if full was mentioned
    {
      serial_println(
          "Implementation:\n"
          "\thelp <command> [--full] [--oneline]\n"
          "\n"
          "Flags:\n"
          "\t-f, --full: Show Implementation and Flags for each command\n"
          "\t\twith explanations\n"
          "\t-o, --oneline: Show only syntax of command\n");
    } else if (flag & ONELINEFLAG)  // if oneline flag was happening
    {
      serial_println("help <command> [--full] [--oneline]");
    }
  } else if (strcmp(gparams[1], "version") == 0) {
    serial_println("Help page for version command\n");
  } else if (strcmp(gparams[1], "shutdown") == 0) {
    serial_println("Help page for shutdown command\n");
  } else if (strcmp(gparams[1], "date") == 0) {
    serial_println("Help page for date command\n");
  } else if (strcmp(gparams[1], "time") == 0) {
    serial_println("Help page for time command\n");
  } else {
    char ret[150];
    sprintf(ret, "Command '%s' not in list!", gparams[1]);
    serial_println("Something went wrong?\n");
  }

  return SUCCESS;  // successful response
}

// How should we do this? Is it so simple as to have it just print a line?
int cmd_version(char* params) {
  // Init vars
  int pcount;

  if (set_gparams(params, &pcount) != SUCCESS)  // if setting gparams failed...
  {
    // we know it is because too many parameters
    char ret[100];
    sprintf(ret, "There were more than %d parameters. Command FAILURE.",
            MAXPARAMCOUNT);
    serial_println(ret);
    return FAILURE;
  }
  serial_println(
      "Version 1.?\tModule one\nDevelopers: Hasan Ibraheem, Henry Vos, Jay "
      "Kmetz, Nicholas Fryer\n");
  return SUCCESS;
}

int cmd_shutdown(char* params) {
  // Init vars
  int pcount;

  if (set_gparams(params, &pcount) != SUCCESS)  // if setting gparams failed...
  {
    // we know it is because too many parameters
    char ret[100];
    sprintf(ret, "There were more than %d parameters. Command FAILURE.",
            MAXPARAMCOUNT);
    serial_println(ret);
    return FAILURE;
  }
  // Do we need to implement this here?
  // It is already implemented in comamnd_handler.c
  return SUCCESS;
}

int cmd_date(char* params) {
  // Init vars
  int pcount;

  if (set_gparams(params, &pcount) != SUCCESS)  // if setting gparams failed...
  {
    // we know it is because too many parameters
    char ret[100];
    sprintf(ret, "There were more than %d parameters. Command FAILURE.",
            MAXPARAMCOUNT);
    serial_println(ret);
    return FAILURE;
  }
  /*
  // TODO: Actuallt implement date, this is just the prelim framework
  // Arbitrary cap on 4 arguments, we can add more
  char* command = strtok(params, " "); // The primary command
  char* arg1 = strtok(NULL, " ");
  char* arg2 = strtok(NULL, " ");
  char* arg3 = strtok(NULL, " ");
  char* arg4 = strtok(NULL, " ");

  // Assuming that the only flag for this will be -s, set value, arg1 will be -s
  while arg2-4 will be the month-day-year
  if(arg1 == NULL) { // Date called with no args, just requesting current date
          serial_println("The date is\n");
  }
  else if(strcmp(arg1, "-s") == 0 && (arg2 == NULL || arg3 == NULL || arg4 ==
  NULL)) { // If set date arg is passed without the values
          serial_println("To set date, enter valid dates for month:day:year\n");
  }
  else if(strcmp(arg1,"-s") == 0 && arg2 != NULL && arg3 != NULL && arg4 !=
  NULL){ //If set date arg is passed and the values are not null
          // Coded to use number dates, could implement 3 letter month
          int month = atoi(arg2);
          int day = atoi(arg3);
          int year = atoi(arg4);
          // More will need to be done here due to the complexities of dates an
  uneven months, damn gregorian calander
          serial_println("month=%d\tday=%d\tyear=%d\n");
  }*/
  return SUCCESS;
}

int cmd_time(char* params) {
  // Init vars
  int pcount;

  if (set_gparams(params, &pcount) != SUCCESS)  // if setting gparams failed...
  {
    // we know it is because too many parameters
    char ret[100];
    sprintf(ret, "There were more than %d parameters. Command FAILURE.",
            MAXPARAMCOUNT);
    serial_println(ret);
    return FAILURE;
  }

  /*
  // TODO: Actually implement time with the registry, this is just the prelim
  framework
  // Arbitrary cap on 4 arguments, we can add more
  char* command = strtok(params, " "); // The primary command
  char* arg1 = strtok(NULL, " ");
  char* arg2 = strtok(NULL, " ");
  char* arg3 = strtok(NULL, " ");
  char* arg4 = strtok(NULL, " ");

  // Assuming that the only flag for this will be -s, set value, arg1 will be -s
  while arg2-4 will be hour:minute:second (24 hour time)
  if(arg1 == NULL) { // Time was called with no args, just requesting current
  time
          serial_println("The time is\n");
  }
  else if(strcmp(arg1, "-s") == 0 && (arg2 == NULL || arg3 == NULL || arg4 ==
  NULL)) { // If set time arg is set without the values
          serial_println("To set time, enter a valid value for
  hours:minutes:seconds\n");
  }
  else if(strcmp(arg1, "-s") == 0 && arg2 != NULL && arg3 != NULL && arg4 !=
  NULL) { // If set time arg is set and values are not null
          // Time string -> int
          int hours =  atoi(arg2);
          int minutes = atoi(arg3);
          int seconds = atoi(arg4);

          if(hours > 24 || hours < 0 || minutes > 60 || minutes < 0 || seconds >
  60 || seconds < 0) { // Checking for valid times
                  serial_println("Please enter a valid value for
  hours:minutes:seconds\n");
          }
          else{
                  serial_println("Set time as hours=%d, minutes=%d,
  seconds=%d\n",hours,minutes,seconds);
          }
  }*/
  return SUCCESS;
}

/* Procedure: set_gparams
   Description: Should be called at the beginning of each command function
                                to set the params based on the param string.
   gparams[i] will
                                be the (i)th parameter passed in by the user
   after calling
                                this function. Will mutate the string passed in
   by params to
                                save space
   Params: params-string of parameters delimited by space charaters,
   pcount-pointer
                   to integer representing the parameter count
*/
int set_gparams(char* params, int* pcount) {
  int loc = 0;
  *pcount = 0;  // initialize pcount at 0
  strcpy(gparamstr, params);

  while (gparamstr[loc] != '\0' &&
         *pcount < MAXPARAMCOUNT)  // while we still have more to process...
  {
    serial_print(&gparamstr[loc]);
    // skip all spaces
    while (isspace(&gparamstr[loc]))  // while we are looking at a space...
    {
      gparamstr[loc] = '\0';  // set all spaces to null
      loc++;                  // inc string location
    }
    // at this point, we are looking at the start of a param or the end of the
    // string
    if (gparamstr[loc] != '\0') {
      gparams[*pcount] =
          (gparamstr +
           loc);  // set the (pcount)th gparam to the start of that pointer
      (*pcount)++;
    }

    // skip all nonspaces
    while ((gparamstr[loc] != ' ') && (gparamstr[loc] != '\0')) {
      loc++;
    }
  }

  if (gparamstr[loc] != '\0')  // if we have exceeded the maxparamcount
  {
    return -1;
  } else {
    return 0;
  }
}