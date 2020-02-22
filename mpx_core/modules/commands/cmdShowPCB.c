#include <core/stdio.h>
#include <string.h>
#include <system.h>

#include "../pcb/pcb_constants.h"
#include "../pcb/pcb_utils.h"
#include "commandUtils.h"
#include "commands.h"

int cmd_show_pcb(char* params) {
  int flag = 0;
  int check = set_flags(trim(params), &flag, 0);

  // TODO: make this it's own function and stop copy pasting it into every cmd
  if (check != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  // We didn't have a "no flag" argument. Meaning they didn't pass us the pcb name
  if (!(flag & NO_FLAG)) {
    puts("\033[31mError: No PCB name given. Ex. showPCB PROC0\033[0m");
    return FAILURE;
  }

  char* pcb_name = get_pvalue('\0');
  pcb_t* found_pcb = find_pcb(pcb_name);

  if (found_pcb == NULL) {
    printf("\033[31mCould not find the pcb \"%s\".\033[0m\n", pcb_name);
    return SUCCESS;
  }

  print_pcb_info(found_pcb);

  return SUCCESS;  // successful response
}
