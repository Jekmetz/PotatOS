#include <string.h>
#include <core/serial.h>
#include <core/utility.h>

int bcd_to_decimal(int bcd);

#define SECOND_REGISTER 0x00
#define MINUTE_REGISTER 0x02
#define HOUR_REGISTER 0x04
#define DAY_REGISTER 0x07
#define MONTH_REGISTER 0x08
#define CENTURY_REGISTER 0x32
#define YEAR_REGISTER 0x09
#define INDEX_REGISTER 0x70
#define DATA_REGISTER 0x71

int get_date() {
  int day, month,century, year;
  char buf[100], month_string[20];
  char temp = 0;

  // Passing day register to user index register then reading from correlated data register
  outb(INDEX_REGISTER,DAY_REGISTER);
  temp = inb(DATA_REGISTER);
  day = bcd_to_decimal(temp);

  // Passing month register to user index register then reading from correlated data register
  outb(INDEX_REGISTER,MONTH_REGISTER);
  temp = inb(DATA_REGISTER);
  month = bcd_to_decimal(temp);

  // Passing century register to user index register then reading from correlated data register
  outb(INDEX_REGISTER, CENTURY_REGISTER);
  temp = inb(DATA_REGISTER);
  century = bcd_to_decimal(temp);

  // Passing year register to user index register then reading from correlated data register
  outb(INDEX_REGISTER,YEAR_REGISTER);
  temp = inb(DATA_REGISTER);
  year = bcd_to_decimal(temp);

  switch(month){
    case 1:
      strcpy(month_string, "January");
      break;
    case 2:
      strcpy(month_string, "February");
      break;
    case 3:
      strcpy(month_string, "March");
      break;
    case 4:
      strcpy(month_string, "April");
      break;
    case 5:
      strcpy(month_string, "May");
      break;
    case 6:
      strcpy(month_string, "June");
      break;
    case 7:
      strcpy(month_string, "July");
      break;
    case 8:
      strcpy(month_string, "August");
      break;
    case 9:
      strcpy(month_string, "September");
      break;
    case 10:
      strcpy(month_string, "October");
      break;
    case 11:
      strcpy(month_string, "November");
      break;
    case 12:
      strcpy(month_string, "December");
      break;
  }
  sprintf(buf, "Current date is %s %d, %d%d\n",month_string,day,century,year);
  serial_println(buf);

  return 1;
}

int set_date(int month_in, int day_in, int year_in) {
  int month = month_in;
  int day = day_in;
  int temp = year_in;
  int century, year = 0;

  // Acceptable date ranges
  if (temp < 1700 || year > 3000) {
    return 0;
  }
  // If the year is an even year divisible by 100, tuen of century
  else if (temp % 100 == 0) {
    century = temp/100;
    year = 0;
  }
  else {
    century = temp/100;
    year = temp % 100;
  }



  // if(validate_date( month, day, year) != 0) {
  //   serial_println("one");
  // }

  sti();
  outb(0x70, 0x07);
  outb(0x71, ((day/10)<<4) | (day%10));

  outb(0x70, 0x08);
  outb(0x71, ((month/10)<<4) | (month%10));

  outb(0x70, 0x09);
  //outb(0x71,(((year-(year/100)*100)/10)<<4) | (year%10) );
  outb(0x71, ((year/10)<<4) | (year%10));

  outb(0x70,0x32);
  outb(0x71, ((century/10)<<4) | (century%10));
  cli();
  return 1;
}

int get_time() {
  int hour, minute, second;
  char buf[100];
  char temp = 0;

  // Passing hour register to user index register then reading from correlated data register
  outb(INDEX_REGISTER,0x04);
  temp = inb(DATA_REGISTER);
  hour = bcd_to_decimal(temp);

  // Passing minute register to user index register then reading from correlated data register
  outb(INDEX_REGISTER,0x02);
  temp = inb(DATA_REGISTER);
  minute = bcd_to_decimal(temp);

  // Passing second register to user index register then reading from correlated data register
  outb(INDEX_REGISTER, 0x00);
  temp = inb(DATA_REGISTER);
  second = bcd_to_decimal(temp);

  sprintf(buf, "Current time is %d:%d:%d\n",hour, minute, second);
  serial_println(buf);

  return 1;
}

int set_time(int hour_in, int minute_in, int second_in) {
  int hour = hour_in;
  int minute = minute_in;
  int second = second_in;

  // if(validate_date( month, day, year) != 0) {
  //   serial_println("one");
  // }

  sti();
  outb(0x70, 0x04);
  outb(0x71, ((hour/10)<<4) | (hour%10));

  outb(0x70, 0x02);
  outb(0x71, ((minute/10)<<4) | (minute%10));

  outb(0x70, 0x00);
  //outb(0x71,(((year-(year/100)*100)/10)<<4) | (year%10) );
  outb(0x71, ((second/10)<<4) | (second%10));

  cli();
  return 1;
}

int bcd_to_decimal(int bcd){
  // Accepts a bcd value as bcd
  // We need to take the first nyble and multiply by 10
  // Then add the second nyble

  int ret;
  ret = ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);

  if (ret < 0){
    return FAILURE;
  }
  else return ret;
}
