#include "IntervalConfig.h"

unsigned char IntervalConfig::getDayConfig(unsigned char dayId) {
  switch (dayId) {
  case WeekDay_SUNDAY:
    return sunday;
  case WeekDay_MONDAY:
    return monday;
  case WeekDay_TUESDAY:
    return tuesday;
  case WeekDay_WEDNESDAY:
    return wednesday;
  case WeekDay_THURSDAY:
    return thursday;
  case WeekDay_FRIDAY:
    return friday;
  case WeekDay_SATURDAY:
    return saturday;
  default:
    return 0;
  }
}
