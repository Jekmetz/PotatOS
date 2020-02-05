#include <core/stdio.h>
#include <modules/mpx_supt.h>
#include <stdarg.h>
#include <system.h>
#include <string.h>

int printf(char *form, ...)
{
    static char print_buff[4096];
    va_list args;
    va_start(args, form);
    int ret = sprintf_internal(print_buff, form, args);
    va_end(args);

    int size = strlen(print_buff);

    sys_req(WRITE, DEFAULT_DEVICE, print_buff, &size);

    return ret;
}

int puts(char *buff)
{
    int size = strlen(buff);
    int nl_s = 2;
    sys_req(WRITE, DEFAULT_DEVICE, buff, &size);
    sys_req(WRITE, DEFAULT_DEVICE, "\n", &nl_s);
    return 1;
}