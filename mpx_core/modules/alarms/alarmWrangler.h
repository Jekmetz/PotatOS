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


int listAlarms();
int alarmNumber();
int insertAlarm(char* message, char* dateIn);
int removeAlarm(const char* message);
int check();
void alarmProcess();

typedef struct {
  char* message;
  time_h time;
} ALARM;