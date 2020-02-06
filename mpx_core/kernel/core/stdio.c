/**
* @file stdio.c
*
* @brief Holds all implementation of standard I/O functions
*/
#include <core/stdio.h>
#include <modules/mpx_supt.h>
#include <stdarg.h>
#include <system.h>
#include <string.h>

/**
* @brief takes in a format string and prints it out to the DEFAULT_DEVICE
*
* %c - charachter
* %d/%i - decimal integer
* %x - hexadecimal integer
* %s - string
* %% - percent sign
* Numbers can be included before the format specifier to declare
* alignment. i.e. %-10c = "c         "
* A pad with 0s can also be added using a '0' directly after the percent
* i.e. %03c = "00c"
*
* @param form character pointer to the format
* @param valist variadic arguments to match the format (see brief)
*
* @return 0 for failure 1 for success
*/
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

/**
* @brief prints out a string to DEFAULT_DEVICE
*
* @param buff string to print out
*
* @return 1
*/
int puts(char *buff)
{
    int size = strlen(buff);
    int nl_s = 2;
    sys_req(WRITE, DEFAULT_DEVICE, buff, &size);
    sys_req(WRITE, DEFAULT_DEVICE, "\n", &nl_s);
    return 1;
}