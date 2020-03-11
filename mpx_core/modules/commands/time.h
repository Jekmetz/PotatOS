/**
* @file time.h
*
* @brief The header file for the date and time functions
*/

#pragma once

// Indexable locations of the RTC
#define SECOND_REG 0x00
#define MINUTE_REG 0x02
#define HOUR_REG 0x04
#define DAY_OF_MONTH_REG 0x07
#define MONTH_REG 0x08
#define CENTURY_REG 0x32
#define YEAR_REG 0x09
#define INDEX_REG 0x70
#define DATA_REG 0x71

// Available months to set
// Not necessary but are for convinience
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

// Struct containing all the information about any one time
// from the RTC.
/**
* @brief A struct to all the time and date elements
*
* The time Struct is a custom struct that is designed to hold all the
* elements necessary for time and date.
*/
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

/**
* @brief Generates a string with a standard format of time.
*
* Generates a string that contains all the data contained in a time_h. This
* form shows all data from largest timescale to smallest timescale.
*
* @param dest Pointer to a string that is large enough to contain the output string
* @param time Pointer to the time to write into the destination string.
*
* @return Return is through the 'dest' pointer.
*
* @note This is merely a convienience, as it is only an sprintf call.
*/
void format_time(char *dest, time_h *t);

/**
* @brief Retrieves the current time in the Real Time Clock(RTC).
*
* Aquires data from the RTC, packaging the data into a time_h struct for ease of use.
*
* @return Returns the current time represented as 6 values in a time_h struct.
*/
time_h get_current_time();

/**
* @brief Sets the current time in the RTC.
*
* Uses a time_h struct to set the data members of the RTC. This function also does error
* checking on valid times, including leap-years, valid days of months, etc., to ensure the given
* time is valid.
*
* @param time A time_h struct containing the new time, as defined by the user.
*
* @return If the operation was successful in boolean format (1 = true, 0 = false).
*
* @note This function also ensures that the date will be set in the correct order within the RTC.
* @note Setting a value in the input struct to a '-1' will skip the value in setting the time. Essentially,
*   keeping the value as it was before. This is demonstrated in the commands.c file.
*/
int set_current_time(time_h time);

/**
* @brief Converts BCD values into decimal.
*
* This function converts BCD values, to be a more code friendly decimal value.
*
* @param bcd Value that is in BCD that needs to be a normal decimal value.
*
* @return The value of the BCD as an integer.
*/
int bcd_to_decimal(int bcd);

/**
 * @brief Fake 64 bit integer
 */
struct fakelong {
  unsigned long int lower;
  unsigned long int upper;
};

/**
 * @brief return clock cycles since reset in a fake long long
 */
struct fakelong rdtsc(void);

time_h* parseTandD(time_h* dest, char* input);

int compareTime();
