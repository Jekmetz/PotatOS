#include<string.h>
#include<system.h>

#include "commands.h"
#include "commandUtils.h"
#include "../pcb/pcb_utils.h"
#include "../mpx_supt.h"

int cmd_loadr3(char* params) {
//no params!
if(params==NULL){}

pcb_t* new_pcb1 = setup_pcb("r3proc1", APPLICATION, 1);
context_t* cp1 = (context_t*)(new_pcb1->stacktop);
memset(cp1, 0, sizeof(context_t));
cp1->fs=0x10;
cp1->gs=0x10;
cp1->ds=0x10;
cp1->es=0x10;
cp1->cs=0x08;
cp1->ebp=(u32int)(new_pcb1->stack);
cp1->esp=(u32int)(new_pcb1->stacktop);
cp1->eip=(u32int)(new_pcb1->stacktop);
cp1->eflags=0x202;
new_pcb1->state = READY;
insert_pcb(new_pcb1);

pcb_t* new_pcb2 = setup_pcb("r3proc2", APPLICATION, 2);
context_t* cp2 = (context_t*)(new_pcb2->stacktop);
memset(cp2, 0, sizeof(context_t));
cp2->fs=0x10;
cp2->gs=0x10;
cp2->ds=0x10;
cp2->es=0x10;
cp2->cs=0x08;
cp2->ebp=(u32int)(new_pcb2->stack);
cp2->esp=(u32int)(new_pcb2->stacktop);
cp2->eip=(u32int)(new_pcb2->stacktop);
cp2->eflags=0x202;
new_pcb2->state = READY;
insert_pcb(new_pcb2);

pcb_t* new_pcb3 = setup_pcb("r3proc3", APPLICATION, 3);
context_t* cp3 = (context_t*)(new_pcb3->stacktop);
memset(cp3, 0, sizeof(context_t));
cp3->fs=0x10;
cp3->gs=0x10;
cp3->ds=0x10;
cp3->es=0x10;
cp3->cs=0x08;
cp3->ebp=(u32int)(new_pcb3->stack);
cp3->esp=(u32int)(new_pcb3->stacktop);
cp3->eip=(u32int)(new_pcb3->stacktop);
cp3->eflags=0x202;
new_pcb3->state = READY;
insert_pcb(new_pcb3);

pcb_t* new_pcb4 = setup_pcb("r3proc4", APPLICATION, 4);
context_t* cp4 = (context_t*)(new_pcb4->stacktop);
memset(cp4, 0, sizeof(context_t));
cp4->fs=0x10;
cp4->gs=0x10;
cp4->ds=0x10;
cp4->es=0x10;
cp4->cs=0x08;
cp4->ebp=(u32int)(new_pcb4->stack);
cp4->esp=(u32int)(new_pcb4->stacktop);
cp4->eip=(u32int)(new_pcb4->stacktop);
cp4->eflags=0x202;
new_pcb4->state = READY;
insert_pcb(new_pcb4);

pcb_t* new_pcb5 = setup_pcb("r3proc5", APPLICATION, 5);
context_t* cp5 = (context_t*)(new_pcb5->stacktop);
memset(cp5, 0, sizeof(context_t));
cp5->fs=0x10;
cp5->gs=0x10;
cp5->ds=0x10;
cp5->es=0x10;
cp5->cs=0x08;
cp5->ebp=(u32int)(new_pcb5->stack);
cp5->esp=(u32int)(new_pcb5->stacktop);
cp5->eip=(u32int)(new_pcb5->stacktop);
cp5->eflags=0x202;
new_pcb5->state = READY;
insert_pcb(new_pcb5);
sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
return SUCCESS;
}