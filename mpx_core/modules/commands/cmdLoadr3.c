#include<string.h>
#include<system.h>

#include "procsr3.h"
#include "commands.h"
#include "commandUtils.h"
#include "../pcb/pcb_utils.h"
#include "../mpx_supt.h"

int cmd_loadr3(char* params) {
  //no params!
  if(params==NULL){}

  process_loader("r3proc1", 1,APPLICATION, &proc1, SUSPENDED_READY);
  process_loader("r3proc2", 2,APPLICATION, &proc2, SUSPENDED_READY);
  process_loader("r3proc3", 3,APPLICATION, &proc3, SUSPENDED_READY);
  process_loader("r3proc4", 3,APPLICATION, &proc4, SUSPENDED_READY);
  process_loader("r3proc5", 3,APPLICATION, &proc5, SUSPENDED_READY);

  return SUCCESS;
}