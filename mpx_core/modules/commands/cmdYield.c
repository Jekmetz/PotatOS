#include <system.h>
#include "commandUtils.h"

int cmd_yield(char* params)
{
	if(params == NULL) {}
	asm volatile ("int $60");
	return SUCCESS;
}