#include <string.h>
#include <core/io.h>
#include <core/utility.h>
#include <core/serial.h>

#include "time.h"

#define pull_data(val, loc) \
{\
	outb(INDEX_REG, loc);\
	char temp = inb(DATA_REG);\
	val = bcd_to_decimal(temp);\
	if (val < 0)\
	{\
		serial_println("\033[31mTIME ERROR: BCD conversion error in "#val".\033[0m");\
		return (time_h){-1,-1,-1,-1,-1,-1};\
	}\
}

#define decimal_to_bcd(val) ((val/10)<<4 | (val%10))

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
	pull_data(ret.day_of_month, DAY_OF_MONTH_REG);
	pull_data(ret.month, MONTH_REG);
	int century, year;
	pull_data(century, CENTURY_REG);
	pull_data(year, YEAR_REG);
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
			char msg[64];
			sprintf(msg, "\033[31mTIME ERROR: The year %d is not valid.\033[0m", time.year);
			serial_println(msg);
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
			char msg[64];
			sprintf(msg, "\033[31mTIME ERROR: The month %d is not valid.\033[0m", time.month);
			serial_println(msg);
			return 0;
		}
		curr.month = time.month;
	}

	// checking if day is valid
	if (time.day_of_month > -1)
	{
		int is_leap = curr.year % 4 == 0 && !(curr.year % 400 != 0 && curr.year % 100 == 0);

		if (time.day_of_month == 0 || // day cannot be 0 (negatives are allowed because of no write case)
		(curr.month == 2 && is_leap && time.day_of_month > 29) || // leap year in FEBRUARY
		(curr.month == 2 && !is_leap && time.day_of_month > 28) || // FEBRUARY
		(curr.month % 2 == 0 && time.day_of_month > 30) || // Even months have 30 days
		(time.day_of_month > 31)) // no month has more than 31 days
		{
			char msg[64];
			sprintf(msg, "\033[31mTIME ERROR: The day %d is not valid.\033[0m", time.day_of_month);
			serial_println(msg);
			return 0;
		}
		curr.day_of_month = time.day_of_month;
	}

	if (time.hours > 24)
	{
		char msg[64];
		sprintf(msg, "\033[31mTIME ERROR: The hour %d is not valid.\033[0m", time.hours);
		serial_println(msg);
		return 0;
	}

	if (time.minutes > 59)
	{
		char msg[64];
		sprintf(msg, "\033[31mTIME ERROR: The minute %d is not valid.\033[0m", time.minutes);
		serial_println(msg);
		return 0;
	}

	if (time.seconds > 59)
	{
		char msg[64];
		sprintf(msg, "\033[31mTIME ERROR: The second %d is not valid.\033[0m", time.seconds);
		serial_println(msg);
		return 0;
	}

	sti();
	neg_safe_set(time.seconds, SECOND_REG);
	neg_safe_set(time.minutes, MINUTE_REG);
	neg_safe_set(time.hours, HOUR_REG);
	neg_safe_set(century, CENTURY_REG);
	neg_safe_set(year, YEAR_REG);
	neg_safe_set(time.month, MONTH_REG);
	neg_safe_set(time.day_of_month, DAY_OF_MONTH_REG);
	cli();
	return 1; // Successful exit
}
