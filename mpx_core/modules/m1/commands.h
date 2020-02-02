/*This file will serve as  the prototype list
for commands.c*/
#define SUCCESS 0

/*
  Procedure: cmd_help
  Params: params-will serve as the params for each of these things
*/
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
