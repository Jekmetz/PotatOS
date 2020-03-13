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

// Array to hold alarms
ALARM alarms[MAX_ALARM] = {};

// Global variable to count alarm number
int numberAlarms = 0;

// List all alarms
int listAlarms(){
	if(numberAlarms == 0){
	 puts("No current alarms scheduled");
	}

	// Iterate through all alarms in alarms array
	for(int i = 0; i < numberAlarms; i++){
		printf("Alarm message: %s\tAlarm date and time: %d/%d/%d %d:%d:%d\n", \
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

// Insert an alarm into the array
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
 	if(parseTandD(&alarmDandT, dateIn) == NULL) return FAILURE;
	
	ALARM newAlarm;
	newAlarm.message = messageIn;
	newAlarm.time.seconds = alarmDandT.seconds;
	newAlarm.time.minutes = alarmDandT.minutes;
	newAlarm.time.hours = alarmDandT.hours;
	newAlarm.time.day_of_month = alarmDandT.day_of_month;
	newAlarm.time.month = alarmDandT.month;
	newAlarm.time.year = alarmDandT.year;

	alarms[numberAlarms] = newAlarm;

	numberAlarms++;

	return SUCCESS;
}

// Remove an alarm
int removeAlarm(const char* message){
	// Iterating through all alarms
	for(int i = 0; i < numberAlarms; i++){
		// If the message equals
		if(strcmp(alarms[i].message, message) == 0){
			// Moving the last alarm to the deleted alarms spot
			alarms[i] = alarms[numberAlarms-1];
			alarms[numberAlarms-1].message = NULL;
			numberAlarms--;
		}
	}
	return SUCCESS;
}

// Check if an alarm has past and needs to be 
int check(){
	// Getting current time
	time_h currentTime = get_current_time();

	// Iterating through all alarms to check 
	for(int i = 0; i<numberAlarms;i++){
		// If its time to send alarm
		if(compareTime(currentTime, alarms[i].time) > 0){
			// Printing alarm then removing it
			printf("%s\n",alarms[i].message);
			removeAlarm(alarms[i].message);
		}
	}

	return SUCCESS;
}

// Infinite alarm checking proccess
void alarmProcess(){
	// Infinite process
	while(1){

		// Running check
		check();

		// Calling idle to give control back
		sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL); 
	}
}