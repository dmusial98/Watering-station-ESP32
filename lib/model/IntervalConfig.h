#ifndef INTERVAL_CONFIG_H
#define INTERVAL_CONFIG_H

#include <WeekDays.h>

class IntervalConfig {

public:
  IntervalConfig() { };

  unsigned char id = 0;
  unsigned char openValveHour = 0;
  unsigned char openValveMinute = 0;
  unsigned char closeValveHour = 0;
  unsigned char closeValveMinute = 0;

  // determine, whether should open valve on particular day
  unsigned char sunday = 0;
  unsigned char monday = 0;
  unsigned char tuesday = 0;
  unsigned char wednesday = 0;
  unsigned char thursday = 0;
  unsigned char friday = 0;
  unsigned char saturday = 0;

  unsigned char getDayConfig(unsigned char dayId);

};

#endif
