#include <string.h>
#include <core/stdio.h>
#include <core/utility.h>
#include <core/io.h>
#include <stdarg.h>
#include <system.h>

#include "../commands/commandUtils.h"
#include "../commands/time.h"
#include "../mpx_supt.h"
#include "./alarmWrangler.h"
#include "../commands/time.h"

ALARM alarms[MAX_ALARM] = {};

int numberAlarms = 0;

int listAlarms(){
	if(numberAlarms == 0){
	 puts("No current alarms scheduled");
	}

	for(int i = 0; i < numberAlarms; i++){
		printf("%s\t%d/%d/%d %d:%d:%d\n", \
			alarms[i].message, \
			alarms[i].time.day_of_month, \
			alarms[i].time.month, \
			alarms[i].time.year, \
			alarms[i].time.hours, \
			alarms[i].time.minutes, \
			alarms[i].time.seconds
		);
	}

	return SUCCESS;
}

int insertAlarm(char* messageIn, char* dateIn){
	if(numberAlarms >= MAX_ALARM){
	  puts("Exceeded max alarms");
	  return FAILURE;
	}

	if(dateIn == NULL)
    {
    	puts("\033[31mMust include time value after [-s | --set]! Exiting!\033[0m");
    	return FAILURE;
 	}
 	time_h alarmDandT;
 	parseTandD(&alarmDandT, dateIn);
	
	ALARM newAlarm;
	newAlarm.message = messageIn;
	newAlarm.time = alarmDandT;

	alarms[numberAlarms] = newAlarm;

	numberAlarms++;

	return SUCCESS;
}

int removeAlarm(const char* message){
	for(int i = 0; i < numberAlarms; i++){
		if(strcmp(alarms[i].message, message) == 0){
			alarms[i] = alarms[numberAlarms-1];
			alarms[numberAlarms-1].message = NULL;
			numberAlarms--;
		}
	}

	return SUCCESS;
}

int check(){
	time_h currentTime = get_current_time();

	for(int i = 0; i<numberAlarms;i++){
		if(compareTime(currentTime, alarms[i].time) > 0){
			printf("%s\n",alarms[i].message);
			removeAlarm(alarms[i].message);
		}
	}

	return SUCCESS;
}

void alarmProcess(){
	while(1){
		check();
		sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL); 
	}
}