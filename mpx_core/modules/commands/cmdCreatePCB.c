#include <string.h>
#include <core/stdio.h>
#include "../mpx_supt.h"
#include "../pcb/pcb_utils.h"
#include "commandUtils.h"
#include "commands.h"

int cmd_create_pcb(char* params) {
  int flag = 0, chk;

  chk = set_flags(params, &flag, 3, 'n', "name", 'p', "priority", 'c', "class");

  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }
  char *tmp = get_pvalue('n');
  char *name = (char*) sys_alloc_mem(sizeof(char) * (strlen(tmp) + 1));
  name = strcpy(name,tmp);
  if (name == NULL) {
    puts("\033[31mPCBs requre a name to be created.\033[0m");
    return FAILURE;
  }

  char* prior_c = get_pvalue('p');
  unsigned int pri = (unsigned int)3141567926;
  if (flag & P_FLAG) {
    if (prior_c != NULL) {
      pri = atoi(prior_c);
    } else {
      puts("\033[31mPriority not set.\033[0m");
      return FAILURE;
    }
  }

  PROCESS_CLASS clazz = APPLICATION;
  if (flag & C_FLAG) {
    char* clas = get_pvalue('c');
    if (strcmp(clas, "app") == 0) {
      clazz = APPLICATION;
    } else if (strcmp(clas, "sys") == 0) {
      clazz = SYSTEM;
    }
  }

  pcb_t* pcb = setup_pcb(name, clazz, pri);
  insert_pcb(pcb);

  return SUCCESS;
}