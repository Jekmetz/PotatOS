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
#define SHUTDOWN 1
#define MAXPARAMCOUNT 10

// cmd_help flags
#define FULLFLAG (1 << 0)
#define ONELINEFLAG (1 << 1)
#define SETDATE (1 << 2)

#include <core/io.h>

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

  if (set_gparams(params, &pcount) != SUCCESS) {  // if setting gparams failed...
    // we know it is because too many parameters
    char ret[100];
    sprintf(ret, "There were more than %d parameters. Command FAILURE.",
            MAXPARAMCOUNT);
    serial_println(ret);
    return FAILURE;
  }
  // if we are here, params are in gparams! parameter count is in pcount

  // Getting flags from gparams and assigning to flag
  for (int i = 0; i < pcount; i++) {  // for every parameter...

    // set the correct flag if it is there
    if ((strcmp(gparams[i], "-f") == 0) ||
        (strcmp(gparams[i], "--full") == 0)) {  // FULLLINE
      flag = flag | FULLFLAG;
    }
  }

  // lists all the commands that have help pages, help without pararms
  if(strcmp(gparams[0], "help") == 0){
    serial_println(
      "You can request a help page for the following command\n"
      "Command name: help\tHow to request: help help\n"
      "Command name: version\tHow to request: help version\n"
      "Command name: shutdown\tHow to request: help shutdown\n"
      "Command name: date\tHow to request: help date\n"
      "Command name: time\tHow to request: help time\n"
    );
  }

  // Help page for help
  if (strcmp(gparams[1], "help") == 0) {
    serial_println(
      "View help pages for individual commands."
      " Use flag [-f | --full] for more information\n"
    );

    // If full flag was used
    if (flag & FULLFLAG)
    {
      serial_println(
          "Implementation:\n"
          "\thelp <command> [-f | --full]\n"
          "\n"
          "Flags:\n"
          "\t[-f | --full] - Show implementation and flags for each command\n"
          "\t\twith explanations\n"
        );
    }
  }

  // Help page for version
  else if (strcmp(gparams[1], "version") == 0) {
    serial_println(
      "Display version information."
      "Use flag [-f | --full] for more information"
    );

    // If full flag was used
    if (flag & FULLFLAG)
    {
      serial_println(
        "Implementatoin:\n"
        "\tversion [-f | --full]\n"
        "Flags:\n"
        "\t[-f | --full] - Show implementation and flags for each command\n"
        "\t\twith explanations"
      );
    }
  }

  // Help page for shutdown
  else if (strcmp(gparams[1], "shutdown") == 0) {
    serial_println(
      "Shutdown the mpx instance."
      "Use flag [-f | --full] for more information"
    );

    // If full flag was used
    if (flag & FULLFLAG)
    {
      serial_println(
        "Implementatoin:\n"
        "\tshutdown\n"
      );
    }
  }

  // Help page for date
  else if (strcmp(gparams[1], "date") == 0) {
    serial_println(
      "Display date."
      "Use flag [-f | --full] for more information"
    );

    // If full flag was used
    if (flag & FULLFLAG)
    {
      serial_println(
        "Implementatoin:\n"
        "\tdate [-f | --full] [-s | --set]\n"
        "Flags:\n"
        "\t[-f | --full] - Show implementation and flags for each command\n"
        "\t\twith explanations"
        "\t[-s | --set] - Set the date in DD-MM-YYYY\n"
        "\t\tWhere all values are integers"
        "Example:\n"
        "\tdate -s 08-24-1994\n"
        "\tdate -s 01-01-2019\n"
      );
    }
  }

  // Help page for time
  else if (strcmp(gparams[1], "time") == 0) {
    serial_println(
      "Display time."
      "Use flag [-f | --full] for more information"
    );

    // If full flag was used
    if (flag & FULLFLAG)
    {
      serial_println(
        "Implementatoin:\n"
        "\tdate [-f | --full] [-s | --set]\n"
        "Flags:\n"
        "\t[-f | --full] - Show implementation and flags for each command\n"
        "\t\twith explanations"
        "\t[-s | --set] - Set the time in HH:MM:SS\n"
        "\t\tWhere all values are integers and using 24 hour time"
        "Example:\n"
        "\time -s 12:24:32\n"
        "\time -s 16:02:00\n"
      );
    }
  }

  // Help command error
  else {
    char ret[150];
    sprintf(ret, "Command '%s' not in list!", gparams[1]);
    //serial_println("Something went wrong?\n");
  }

  return SUCCESS;  // successful response
}

// Print the current version of the mpx
int cmd_version(char* params) {
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

  // Getting flags from gparams and assigning to flag
  for (int i = 0; i < pcount; i++) {  // for every parameter...

    // set the correct flag if it is there
    if ((strcmp(gparams[i], "-f") == 0) ||
        (strcmp(gparams[i], "--full") == 0)) {  // FULLLINE
      flag = flag | FULLFLAG;
    }
  }

  if(strcmp(gparams[0], "version") == 0){
    serial_println(
      "Version 1.0"
    );

    if (flag & FULLFLAG){
      serial_println(
        "\nVersion 1.0\n"
        "Module one\n"
        "Developers:\n"
        "\tHasan Ibraheem\n"
        "\tHenry Vos\n"
        "\tJay Kmetz\n"
        "\tNicholas Fryer\n"
      );
    }
  }

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
  return SHUTDOWN;
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
