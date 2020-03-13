#include <system.h>
#include <core/stdio.h>

#include "commandUtils.h"
#include "../cmdinput/poll_input.h"

int cmd_history(char* params) {
  if(params == NULL) {}

  puts("The 10 most recent commands: \n");

  char (*commands)[11][100] = get_command_history();
  // This 0th index is the current command, 1 - 10 are the past

  int history_length = get_history_length();

  for (int i = history_length; i >= 1; i--) {
    char* command = (*commands)[i];
    printf("%2d: %s\n", i, command);
  }

  return SUCCESS;
}
