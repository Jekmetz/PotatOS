#include <string.h>
#include <core/io.h>

#include "time.h"

#define cat_val(scale) \
{\
	strcat(dest, ":");\
	itoa(time->scale, buff, 10);\
	strcat(dest, buff);\
}

#define dow_cases(abbr) \
{\
	strcpy(buff, abbr);\
	break;\
}

void format_time(char *dest, time_h *time)
{
	char buff[5];
	itoa(time->year, buff, 10);
	strcpy(dest, buff);

	cat_val(month);
	cat_val(day_of_month);
	strcat(dest, "/");
	switch (time->day_of_week)
	{
		case SUNDAY:
		dow_cases("SUN");
		case MONDAY:
		dow_cases("MON");
		case TUESDAY:
		dow_cases("TUE");
		case WEDNESDAY:
		dow_cases("WED");
		case THURSDAY:
		dow_cases("THU");
		case FRIDAY:
		dow_cases("FRI");
		case SATURDAY:
		dow_cases("SAT");
		default:
		dow_cases("ERR");
	}
	strcat(dest, buff);

	cat_val(hours);
	cat_val(minutes);
	cat_val(seconds);

	strcat(dest, " 1.3");
}

time_h get_current_time()
{
	char dat = inb(ACCESS_REG);
	dat |= 0x80;
	outb(ACCESS_REG, dat);

	time_h ret;
	ret.seconds = ((int)inb(SECOND_REG));
	ret.minutes = ((int)inb(MINUTE_REG));
	ret.hours = ((int)inb(HOUR_REG));
	ret.day_of_week = ((int)inb(DAY_OF_WEEK_REG));
	ret.day_of_month = ((int)inb(DAY_OF_MONTH_REG));
	ret.month = ((int)inb(MONTH_REG));
	ret.year = ((int)inb(YEAR_REG));

	dat = inb(ACCESS_REG);
	dat &= 0x7F;
	outb(ACCESS_REG, dat);

	return ret;
}

//void set_current_time(time_h time)
//{

//}
