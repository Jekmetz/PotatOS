#pragma once

#define SEC_REG 0x00
#define MIN_REG 0x02
#define HOU_REG 0x04
#define DOW_REG 0x06
#define DOM_REG 0x07
#define MON_REG 0x08
#define YER_REG 0x09


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

void set_current_time(time_h time);