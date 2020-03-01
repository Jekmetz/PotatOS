#include<system.h>
#include<core/stdio.h>

#include "commandUtils.h"
#include "../cmdinput/splash.h"

int cmd_potat(char* params)
{
	if(params == NULL) {}
	puts("\n");
	draw_splash();
	puts("\n");

	return SUCCESS;
}