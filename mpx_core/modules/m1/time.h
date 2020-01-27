#pragma once

#define SECOND_REG 0x71
#define MINUTE_REG 0x72
#define HOUR_REG 0x73
#define DAY_OF_WEEK_REG 0x74
#define DAY_OF_MONTH_REG 0x75
#define MONTH_REG 0x76
#define YEAR_REG 0x77
#define ACCESS_REG 0x70


enum DAY_OF_WEEK
{
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
};

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
    int day_of_week;
    int day_of_month;
    int month;
    int year;
};
typedef struct time time_h;

void format_time(char *dest, time_h *t);

time_h get_current_time();

//void set_current_time(time_h time);
