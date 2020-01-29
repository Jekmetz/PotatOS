#pragma once

#define SECOND_REG 0x00
#define MINUTE_REG 0x02
#define HOUR_REG 0x04
#define DAY_OF_MONTH_REG 0x07
#define MONTH_REG 0x08
#define CENTURY_REG 0x32
#define YEAR_REG 0x09
#define INDEX_REG 0x70
#define DATA_REG 0x71

enum MONTH
{
  JANUARY = 1,
  FEBRUARY,
  MARCH,
  APRIL,
  MAY,
  JUNE,
  JULY,
  AGUST,
  SEPTEMBER,
  OCTOBER,
  NOVEMBER,
  DECEMBER
};

struct time
{
  int seconds;
  int minutes;
  int hours;
  int day_of_month;
  int month;
  int year;
};
typedef struct time time_h;

void format_time(char *dest, time_h *t);

time_h get_current_time();

int set_current_time(time_h time);

int bcd_to_decimal(int bcd);
