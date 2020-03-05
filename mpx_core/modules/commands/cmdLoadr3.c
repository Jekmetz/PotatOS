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

process_loader("r3proc1",1,&proc1);
process_loader("r3proc2",2,&proc2);
process_loader("r3proc3",3,&proc3);
process_loader("r3proc4",3,&proc4);
process_loader("r3proc5",3,&proc5);

return SUCCESS;
}