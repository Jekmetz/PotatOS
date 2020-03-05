#include <system.h>
#include "../modules/mpx_supt.h"
#include "commandUtils.h"

int cmd_yield(char* params)
{
    if(params == NULL) {}
    sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    return SUCCESS;
}