/**
* @file alarmWrangler.h
*
* @brief Contains all alarm processes and internal structures
*/

#include <string.h>
#include <core/stdio.h>
#include <core/utility.h>
#include <core/io.h>
#include <stdarg.h>
#include <system.h>

#include "../commands/commandUtils.h"
#include "../commands/time.h"
#include "../mpx_supt.h"
#include "../commands/time.h"

#define MAX_ALARM 10

/**
* @brief List all alarms
*
* @returns Int SUCCESS/FAILURE
*/
int listAlarms();

/**
* @brief Insert an alarm into the array of alarms
*
* @params message The new alarms message
* @params dateIn The date and or time for the alarm to execute
*
* @returns Int SUCCESS/FAILURE
*/
int insertAlarm(char* message, char* dateIn);

/**
* @brief Remove an alarm from the array of alarms
*
* @params message The name of the alarm, message is name, that you are removing
*
* @returns Int SUCCESS/FAILURE
*/
int removeAlarm(const char* message);

/**
* @brief Checks to see if any alarm has passed time and needs to send notification
*
* @returns Int SUCCESS/FAILURE
*/
int check();

/**
* @brief The Alarm Process that is initiated in Kmain
*
* @returns Nothing
*/
void alarmProcess();

/**
* @brief Struct to hold alarm information
*
* @params message The alarm message
* @params time The alarm execution time
*/
typedef struct {
  char* message;
  time_h time;
} ALARM;