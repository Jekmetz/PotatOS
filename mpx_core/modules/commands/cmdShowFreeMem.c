#include <core/stdio.h>
#include <system.h>
#include "../memory_management/memory_wrangler.h"
#include "commandUtils.h"

int cmd_show_free_mem(char* params){
  // Variables
  int flag = 0, chk;

  // Calling set flags
  chk = set_flags(params, &flag, 0);

  // If set flags fails
  if (chk != SUCCESS) {
    puts("\033[31mHouston, we have a problem. Check your flags!\033[0m");
    return FAILURE;
  }

  show_free_mem_state();

  printf("Remaining Memory: %d bytes\n",get_remaining_free());

  return SUCCESS;
}
