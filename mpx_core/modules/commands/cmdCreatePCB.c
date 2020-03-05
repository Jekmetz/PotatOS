#include <core/stdio.h>
#include <string.h>
#include "../mpx_supt.h"
#include "../pcb/pcb_utils.h"
#include "../pcb/pcb_constants.h"
#include "commandUtils.h"
#include "commands.h"

int cmd_create_pcb(char* params) {
  int flag = 0, chk;

  chk = set_flags(params, &flag, 2, 'p', "priority", 'c', "class");

  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  if (!(flag & NO_FLAG)) {
    puts("\033[31mNo process Included!\033[0m");
    return FAILURE;
  }

  // looking for name of new PCB
  char* tmp = get_pvalue('\0');
  char* name = (char*)sys_alloc_mem(sizeof(char) * (strlen(tmp) + 1));
  name = strcpy(name, tmp);

  if (find_pcb(name) != NULL) {
    printf("\033[31mThe process \"%s\" already exists\033[0m\n", name);
    return FAILURE;
  }

  // checking for priority
  char* prior_c = get_pvalue('p');
  // default priority
  int pri = DEFAULT_PRIORITY;
  if (flag & P_FLAG) {
    // if there is a priority check to make sure it is valid
    if (prior_c != NULL) {
      pri = atoi(prior_c);
      if (pri > DEFAULT_PRIORITY) {
        printf("\033[31mThe priority \"%d\" is too large.\033[0m\n", pri);
        return FAILURE;
      } else if (pri < 0) {
        printf(
            "\033[0mNegative priorities are not valid,  \"%d\" is a negative "
            "number.\033[0m\n",
            pri);
        return FAILURE;
      }
    } else {
      puts("\033[31mPriority not set.\033[0m");
      return FAILURE;
    }
  }

  // default process class
  PROCESS_CLASS clazz = APPLICATION;
  if (flag & C_FLAG) {
    // checking validity of process class
    char* clas = get_pvalue('c');
    if (strcmp(clas, "app") == 0) {
      clazz = APPLICATION;
    } else if (strcmp(clas, "sys") == 0) {
      clazz = SYSTEM;
    }
  }

  pcb_t* pcb = setup_pcb(name, clazz, pri);
  insert_pcb(pcb);

  printf("\033[32mSuccessfully created process %s\033[0m\n", name);

  return SUCCESS;
}
