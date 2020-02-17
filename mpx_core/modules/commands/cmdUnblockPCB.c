#include <string.h>
#include <core/stdio.h>
#include "../pcb/pcb_constants.h"
#include "commandUtils.h"
#include "commands.h"
#include "../mpx_supt.h"
#include "../pcb/pcb_utils.h"

int cmd_unblock_pcb(char* params) {
  int flag = 0, chk;
  chk = set_flags(params, &flag, 2, 'n', "name", 's', "state");

  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  char* name = get_pvalue('n');
  if (name == NULL) {
    puts("\033[31mPCB name must be specified to unblock it.\033[0m");
    return FAILURE;
  }

  pcb_t *rem = remove_pcb(name);
  if (rem == NULL && rem->state != BLOCKED)
  {
    printf("%s is not blocked.\n", name);
    insert_pcb(rem);
    return SUCCESS;
  }

	char *stat_c = get_pvalue('s');
  if (flag & S_FLAG)
  {
		PROCESS_STATE state = atoi(stat_c);
		if (state != RUNNING)
		{
			rem->state = state;
		}
  }

	insert_pcb(rem);
	
	return SUCCESS;
}
