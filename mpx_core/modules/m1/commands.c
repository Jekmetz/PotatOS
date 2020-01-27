//int validate_date(int months, int day, int year);

/*************************************************************
 *	This C file contains all of the commands that will be used
 *  by the command handler. All of the commands in this file
 *  should have the syntax:
 *  int cmd_<cmd_name>(char* params);
 **************************************************************/
#include <string.h>
#include <core/serial.h>

#include <core/utility.h>
#include "help.h"
#include "commands.h"
#define CMDSIZE 100

#define SUCCESS 0
#define FAILURE -1
#define MAXPARAMCOUNT 10


#include <core/io.h>

#include "date_and_time.c"

int set_flags(char** gparams, int * flag, int pcount);

int set_gparams(char* params, int* pcount);


// cmd_help flags
#define A_FLAG (1 << 0)
#define B_FLAG (1 << 1)
#define C_FLAG (1 << 2)
#define D_FLAG (1 << 3)
#define E_FLAG (1 << 4)
#define F_FLAG (1 << 5)
#define G_FLAG (1 << 6)
#define H_FLAG (1 << 7)
#define I_FLAG (1 << 8)
#define J_FLAG (1 << 9)
#define K_FLAG (1 << 10)
#define L_FLAG (1 << 11)
#define M_FLAG (1 << 12)
#define N_FLAG (1 << 13)
#define O_FLAG (1 << 14)
#define P_FLAG (1 << 15)
#define Q_FLAG (1 << 16)
#define R_FLAG (1 << 17)
#define S_FLAG (1 << 18)
#define T_FLAG (1 << 19)
#define U_FLAG (1 << 20)
#define V_FLAG (1 << 21)
#define W_FLAG (1 << 22)
#define Y_FLAG (1 << 23)
#define X_FLAG (1 << 24)
#define Z_FLAG (1 << 25)

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

/*
  Procedure: cmd_help
  Params: params-will serve as the params for each of these things
*/

// A general testing cmd that I can call "test" from mpx to test stuff
int cmd_test(char* params){
  if(params != NULL){
    serial_println("Test is working");

    int day, month,year;
    char buf[100];

    char temp = 0;
    outb(0x70,0x07);
    temp = inb(0x71);
    day = bcd_to_decimal(temp);

    sprintf(buf,"Day is: %d\n",day);
    serial_println(buf);

    outb(0x70,0x08);
    temp = inb(0x71);
    month = bcd_to_decimal(temp);

    sprintf(buf,"Month is: %d\n",month);
    serial_println(buf);

    outb(0x70,0x09);
    temp = inb(0x71);
    year = bcd_to_decimal(temp);

    sprintf(buf, "Year is: %d\n", year);
    serial_println(buf);
  }
  return SUCCESS;
}


int cmd_help(char* params) {
  // Init vars
  int pcount;
  //unsigned char flag = 0;
  int flag = 0;

  if (set_gparams(params, &pcount) != SUCCESS) {  // if setting gparams failed...
    // we know it is because too many parameters
    char ret[100];
    sprintf(ret, "There were more than %d parameters. Command FAILURE.",
            MAXPARAMCOUNT);
    serial_println(ret);
    return FAILURE;
  }
  // if we are here, params are in gparams! parameter count is in pcount

  // Calling set_flags which will set the binary flags for each flag passed
  set_flags(gparams, &flag, pcount);

  // lists all the commands that have help pages, help without pararms
  if(strcmp(gparams[0], "help") == 0 && gparams[1] == NULL){
    // Help without arguments
    HELP;
  }

  // Help page for help
  if (strcmp(gparams[1], "help") == 0) {
    // If no flag used
    if(flag == 0){
     HELP_HELP;
    }

    //If full flag was used
    else if (flag & F_FLAG)
    {
     HELP_HELP_FULL;
    }
  }

  // Help page for version
  else if (strcmp(gparams[1], "version") == 0) {
    // If no flag set
    if(flag == 0){
      HELP_VERSION;
    }

    // If full flag was used
    else if (flag & F_FLAG)
    {
      HELP_VERSION_FULL;
    }
  }

  // Help page for shutdown
  else if (strcmp(gparams[1], "shutdown") == 0) {
    // If no flag is set
    if(flag == 0){
      HELP_SHUTDOWN;
    }

    // If full flag is set
    else if (flag & F_FLAG)
    {
      HELP_SHUTDOWN_FULL;
    }
  }

  // Help page for date
  else if (strcmp(gparams[1], "date") == 0) {
    // If no flag is set
    if(flag == 0){
      HELP_DATE;
    }

    // If full flag was used
    else if (flag & F_FLAG)
    {
      HELP_DATE_FULL;
    }
  }

  // Help page for time
  else if (strcmp(gparams[1], "time") == 0) {
    // If not flag is set
    if(flag == 0){
      HELP_TIME;
    }

    // If full flag was used
    else if (flag & F_FLAG)
    {
      HELP_TIME_FULL;
    }
  }

  return SUCCESS;  // successful response
}

// Print the current version of the mpx
int cmd_version(char* params) {
  // Init vars
  int pcount;
  int flag = 0;

  if (set_gparams(params, &pcount) != SUCCESS)  // if setting gparams failed...
  {
    // we know it is because too many parameters
    char ret[100];
    sprintf(ret, "There were more than %d parameters. Command FAILURE.",
            MAXPARAMCOUNT);
    serial_println(ret);
    return FAILURE;
  }

  // Calling set_flags which will set the binary flags for each flag passed
  set_flags(gparams, &flag, pcount);

  if(strcmp(gparams[0], "version") == 0 ){
    // If no flag set
    if(flag == 0){
      VERSION;
    }

    // If full flag used
    else if (flag & F_FLAG){
      VERSION_FULL;
    }
  }

  return SUCCESS;
}

int cmd_date(char* params) {
  // Init vars
  int pcount;
  int flag = 0;

  if (set_gparams(params, &pcount) != SUCCESS) { // if setting gparams failed...
    // we know it is because too many parameters
    char ret[100];
    sprintf(ret, "There were more than %d parameters. Command FAILURE.",
            MAXPARAMCOUNT);
    serial_println(ret);
    return FAILURE;
  }

  // Calling set_flags which will set the binary flags for each flag passed
  set_flags(gparams, &flag, pcount);

  if(strcmp(gparams[0], "date") == 0 && gparams[1] == NULL) {
    serial_println("Entered date");
      get_date();
  }

  // If full flag used
  else if (flag & S_FLAG){
    serial_println("Entered set date");
    set_date(atoi(gparams[2]), atoi(gparams[3]), atoi(gparams[4]));
  }


  return SUCCESS;

}

int cmd_time(char* params) {
  // Init vars
  int pcount;
  int flag = 0;

  if (set_gparams(params, &pcount) != SUCCESS)  // if setting gparams failed...
  {
    // we know it is because too many parameters
    char ret[100];
    sprintf(ret, "There were more than %d parameters. Command FAILURE.",
            MAXPARAMCOUNT);
    serial_println(ret);
    return FAILURE;
  }

  // Calling set_flags which will set the binary flags for each flag passed
  set_flags(gparams, &flag, pcount);

  if(strcmp(gparams[0], "time") == 0 && gparams[1] == NULL) {
    serial_println("Entered time");
      get_time();
  }
  // If full flag used
  else if (flag & S_FLAG){
    serial_println("Entered set time");
    set_time(atoi(gparams[2]), atoi(gparams[3]), atoi(gparams[4]));
  }

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

  // Reseting all gparams at NULL so I can check to if only one param was passed
  for(int i = 0; i<MAXPARAMCOUNT;i++){
    gparams[i] = NULL;
  }

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


// Using the pcount from set_gparams to iterate through all the gparams, passing
//
// Setting the flags for each command
int set_flags(char** gparams, int * flag, int pcount ){
  // Using the gparams and pcount, which are already set, to set the flag

  // For 0 -> pcount ... Iterates through all the arguments
  for (int i = 0; i < pcount; i++) {  // for every parameter...
    // set the correct flag if it is there
    if ((strcmp(gparams[i], "-f") == 0) ||
        (strcmp(gparams[i], "--full") == 0)) {  // FULLLINE
      *flag = *flag | F_FLAG;
    }
    else if ((strcmp(gparams[i], "-s") == 0)) {
      *flag = *flag | S_FLAG;
    }

  }
  return SUCCESS;
}


// int validate_date(int months, int day, int year){
//   // Need to check for edge cases
//
// }
