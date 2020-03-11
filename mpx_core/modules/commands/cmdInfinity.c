#include "../mpx_supt.h"
#include "commands.h"
#include <core/stdio.h>


#define COUNTS 10000

void proc_inf()
{
  int a = 0, b = 1, count = 0;
  while (1){
    if (count >= COUNTS)
    {
      count = 0;
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
    }
    count++;
    sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
  }

}


int cmd_infinity(char* params)
{
  if (params == NULL) {}

  process_loader("infinity_stone", 61, APPLICATION, &proc_inf);

  return 1;
}