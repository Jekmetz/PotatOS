#include "../mpx_supt.h"
#include "commands.h"
#include <core/stdio.h>

void proc_inf()
{
  int a = 0, b = 1;
  while (1){
    if (a < b)
    {
      a = a + b;
      printf("%d\n", a);
    }
    else
    {
      b = a + b;
      printf("%d\n", b);
    }
    sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
  }

}


int cmd_infinity(char* params)
{
  if (params == NULL) {}

  process_loader("infinity_stone", 61, APPLICATION, &proc_inf, READY);

  return 1;
}