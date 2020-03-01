#include<string.h>
#include<system.h>

#include "procsr3.h"
#include "commands.h"
#include "commandUtils.h"
#include "../pcb/pcb_utils.h"
#include "../mpx_supt.h"

void process_loader(char* proc_name, int priority, void (*func)(void))
{
	pcb_t* new_pcb = setup_pcb(proc_name, APPLICATION, priority);
	context_t* cp = (context_t*)(new_pcb->stacktop);
	memset(cp, 0, sizeof(context_t));
	cp->fs=0x10;
	cp->gs=0x10;
	cp->ds=0x10;
	cp->es=0x10;
	cp->cs=0x08;
	cp->ebp=(u32int)(new_pcb->stack);
	cp->esp=(u32int)(new_pcb->stacktop);
	cp->eip=(u32int)func;
	cp->eflags=0x202;
	new_pcb->state = READY;
	insert_pcb(new_pcb);
}

int cmd_loadr3(char* params) {
//no params!
if(params==NULL){}

process_loader("r3proc1",1,&proc1);
process_loader("r3proc2",2,&proc2);
process_loader("r3proc3",3,&proc3);
process_loader("r3proc4",4,&proc4);
process_loader("r3proc5",5,&proc5);

return SUCCESS;
}