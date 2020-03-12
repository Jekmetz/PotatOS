#include <string.h>
#include <core/io.h>
#include <core/utility.h>
#include <core/stdio.h>

#include "time.h"

/**
* @brief Macro aquires data from a RTC register and converts the output from BCD to decimal.
*
* This macro is used by the get_time function to aquire the RTC value and convert it into a usable decimal.
* The data at the location that is specified is written to the given vlaue. The function outb is used to
* select the type of information that is going to be written to the value, this is years, months, etc..
* The bcd to base 2 conversion happens within the bcd_to_decimal macro.
*
* @param val Value that the requested data is to be stored into. This data must not be a pointer.
* @param loc Type of data being requested, this is using the macros specifed in time.h as YEAR_REG, MONTH_REG, etc.
*
* @return void
*
* @warning Do not use values that are not specifed as 'locations' as the loc field.
*/
#define pull_data(val, loc) \
{\
	outb(INDEX_REG, loc);\
	char temp = inb(DATA_REG);\
	val = bcd_to_decimal(temp);\
	if (val < 0)\
	{\
		puts("\033[31mTIME ERROR: BCD conversion error in "#val".\033[0m");\
		return (time_h){-1,-1,-1,-1,-1,-1};\
	}\
}

/**
* @brief Simple macro to convert values into BCD format to write time to the RTC.
*
* This is used to convert a value that could be defined as a literal or calculated by code,
* into BCD so that writing to the RTC is correct, and that it can keep time. This macro is
* nested in neg_safe_set.
*
* @param val The value to be converted into BCD format.
*
* @return Returns the BCD of the given value.
*
*/
#define decimal_to_bcd(val) ((val/10)<<4 | (val%10))

/**
* @brief Wrties a value to a RTC register.
*
* Writes a decimal value to a RTC register. This converts the given value to BCD, and then
* writes it into the correct data location for the RTC to recognise what is being set. Uses
* outb to set the location of the data that is being written, and writing the actual data.
*
* @param in The value that is being written to the RTC.
* @param loc Type of data being requested, this is using the macros specifed in time.h as YEAR_REG, MONTH_REG, etc.
*
* @return void
*
* @warning Do not use values that are not specifed as 'locations' as the loc field.
* @warning 'in' should not be BCD, it should be a normal value.
*/
#define neg_safe_set(in, loc) \
{\
	if (in > -1)\
	{\
		outb(INDEX_REG, loc);\
		outb(DATA_REG, decimal_to_bcd(in));\
	}\
}

int bcd_to_decimal(int bcd){
	// Accepts a bcd value as bcd
	// We need to take the first nyble and multiply by 10
	// Then add the second nyble

	int ret;
	ret = ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
	return ret;
}

void format_time(char *dest, time_h *time)
{
	sprintf(dest, "%d:%d:%d:%d:%d:%d", time->year, time->month, time->day_of_month, time->hours, time->minutes, time->seconds);
}

time_h get_current_time()
{
	time_h ret;
	// getting data
	pull_data(ret.day_of_month, DAY_OF_MONTH_REG);
	pull_data(ret.month, MONTH_REG);
	int century, year;
	pull_data(century, CENTURY_REG);
	pull_data(year, YEAR_REG);
	// assembling year
	ret.year = century * 100 + year;

	pull_data(ret.hours, HOUR_REG);
	pull_data(ret.minutes, MINUTE_REG);
	pull_data(ret.seconds, SECOND_REG);

	return ret;
}

int set_current_time(time_h time)
{
	time_h curr = get_current_time();

	// checking year validity
	// also parsing into century and year
	int century, year;
	if (time.year > -1)
	{
		if (time.year < 1700 || time.year > 3000)
		{
			printf("\033[31mTIME ERROR: The year %d is not valid.\033[0m\n", time.year);
			return 0;
		}
		century = time.year / 100;
		year = time.year % 100;

		curr.year = time.year;
	}
	else 
	{
		century = -1;
		year = -1;
	}

	// checking month validity
	if (time.month > -1)
	{
		if (time.month == 0 || time.month > 12)
		{
			printf("\033[31mTIME ERROR: The month %d is not valid.\033[0m\n", time.month);
			return 0;
		}
		curr.month = time.month;
	}

	// checking if day is valid
	if (time.day_of_month > -1)
	{
		// Leap year condition
		// if the year is divisible by 400 it is a leapyear
		// if not, and the year is divisible by 100 it is not a leapyear (1900 and 2100)
		// otherwise the year must be divisible by 4 to be a leapyear
		int is_leap = curr.year % 4 == 0 && !(curr.year % 400 != 0 && curr.year % 100 == 0);

		// day cannot be 0 (negatives are allowed because of no write case)
		// leap year in FEBRUARY
		// FEBRUARY
		// Even months have 30 days
		// no month has more than 31 days
		if (time.day_of_month == 0 ||
		(curr.month == 2 && is_leap && time.day_of_month > 29) ||
		(curr.month == 2 && !is_leap && time.day_of_month > 28) ||
		(curr.month % 2 == 0 && time.day_of_month > 30) ||
		(time.day_of_month > 31))
		{
			printf("\033[31mTIME ERROR: The day %d is not valid.\033[0m\n", time.day_of_month);
			return 0;
		}
		curr.day_of_month = time.day_of_month;
	}

	// checking if the hour is valid
	if (time.hours > 24)
	{
		printf("\033[31mTIME ERROR: The hour %d is not valid.\033[0m\n", time.hours);
		return 0;
	}

	// checking if the minutes are valid
	if (time.minutes > 59)
	{
		printf("\033[31mTIME ERROR: The minute %d is not valid.\033[0m\n", time.minutes);
		return 0;
	}

	// checking if the seconds are valid
	if (time.seconds > 59)
	{
		printf("\033[31mTIME ERROR: The second %d is not valid.\033[0m\n", time.seconds);
		return 0;
	}

	// disallowing interrupts
	sti();
	// setting values in order so that they are properly written
	neg_safe_set(century, CENTURY_REG);
	neg_safe_set(year, YEAR_REG);
	neg_safe_set(time.day_of_month, DAY_OF_MONTH_REG);
	neg_safe_set(time.month, MONTH_REG);
	neg_safe_set(time.hours, HOUR_REG);
	neg_safe_set(time.minutes, MINUTE_REG);
	neg_safe_set(time.seconds, SECOND_REG);
	// reallowing interrupts
	cli();
	// Successful exit
	return 1;
}

struct fakelong rdtsc(void) {
  struct fakelong x;
  __asm__ volatile ("RDTSC" : "=A" (x));
  return x;
}

time_h* parseTandD(time_h* dest, char* input){
 	// Splitting input into date and time section
 	char *date = strtok(input, " ");
 	char *time = strtok(NULL, " ");

 	// Using date to strip off the date values
 	char *day = strtok(date, "/");
    char *month = strtok(NULL, "/");
    char *year = strtok(NULL, "/");

    // Calling validDate
    if(!(validDate(year, month, day))){
    	return 0;
    }

    // Using time to strip off the time values 
	char *hour = strtok(time, ":");
	char *minute = strtok(NULL, ":");
	char *second = strtok(NULL, ":");

	if(!(validTime(hour, minute, second))){
		return 0;
	}

	// If everything worked to here, we are good, setting inside the destination
    dest->day_of_month = atoi(day);
    dest->month = atoi(month);
    dest->year = atoi(year);
	dest->seconds = atoi(second);
	dest->minutes = atoi(minute);
	dest->hours = atoi(hour);

	return dest;
}

int compareTime(time_h timeOne, time_h timeTwo){
	if(timeOne.year != timeTwo.year) return (timeOne.year - timeTwo.year);
	if(timeOne.month != timeTwo.month) return (timeOne.month - timeTwo.month);
	if(timeOne.day_of_month != timeTwo.day_of_month) return (timeOne.day_of_month - timeTwo.day_of_month);
	if(timeOne.hours != timeTwo.hours) return (timeOne.hours - timeTwo.hours);
	if(timeOne.minutes != timeTwo.minutes) return (timeOne.minutes - timeTwo.minutes);
	else return timeOne.seconds - timeTwo.seconds;

}

int validDate(char* year, char* month, char* day){
	int yearInt = atoi(year);
	int monthInt = atoi(month);
	int dayInt = atoi(day);

	// Year validity
	if (yearInt < MIN_YEAR || yearInt > MAX_YEAR)
	{
		printf("\033[31mTIME ERROR: The year %d is not valid.\033[0m\n", yearInt);
		return 0;
	}

	// checking month validity
	if (monthInt == 0 || monthInt > 12)
	{
		printf("\033[31mTIME ERROR: The month %d is not valid.\033[0m\n", monthInt);
		return 0;
	}
	
	// checking if day is valid
	// Leap year condition
	// if the year is divisible by 400 it is a leapyear
	// if not, and the year is divisible by 100 it is not a leapyear (1900 and 2100)
	// otherwise the year must be divisible by 4 to be a leapyear
	int is_leap = yearInt % 4 == 0 && !(yearInt % 400 != 0 && yearInt % 100 == 0);

	// day cannot be 0 (negatives are allowed because of no write case)
	// leap year in FEBRUARY
	// FEBRUARY
	// Even months have 30 days
	// no month has more than 31 days
	if (dayInt == 0 ||
	(monthInt == 2 && is_leap && dayInt > 29) ||
	(monthInt == 2 && !is_leap && dayInt > 28) ||
	(monthInt % 2 == 0 && dayInt > 30) ||
	(dayInt > 31))
	{
		printf("\033[31mTIME ERROR: The day %d is not valid.\033[0m\n", dayInt);
		return 0;
	}

	return 1;
}

int validTime(char* hours, char* minutes, char* seconds){
	int hoursInt = atoi(hours);
	int minutesInt = atoi(minutes);
	int secondsInt = atoi(seconds);


	// checking if the hour is valid
	if (hoursInt > 24 || hoursInt < -1)
	{
		printf("\033[31mTIME ERROR: The hour %d is not valid.\033[0m\n", hoursInt);
		return 0;
	}

	// checking if the minutes are valid
	if (minutesInt > 59 || minutesInt < -1)
	{
		printf("\033[31mTIME ERROR: The minute %d is not valid.\033[0m\n", minutesInt);
		return 0;
	}

	// checking if the seconds are valid
	if (secondsInt > 59 || secondsInt < -1)
	{
		printf("\033[31mTIME ERROR: The second %d is not valid.\033[0m\n", secondsInt);
		return 0;
	}

	return 1;
}