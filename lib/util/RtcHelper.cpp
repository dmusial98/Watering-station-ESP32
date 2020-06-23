#include "RtcHelper.h"

unsigned char encodeWeekdaySetup(IntervalConfig *intervalConfig) {
  int power = 1;
  unsigned char encoded = 0;

  if (intervalConfig->sunday == 1)
    encoded += power;
  power *= 2;

  if (intervalConfig->monday == 1)
    encoded += power;
  power *= 2;

  if (intervalConfig->tuesday == 1)
    encoded += power;
  power *= 2;

  if (intervalConfig->wednesday == 1)
    encoded += power;
  power *= 2;

  if (intervalConfig->thursday == 1)
    encoded += power;
  power *= 2;

  if (intervalConfig->friday == 1)
    encoded += power;
  power *= 2;

  if (intervalConfig->saturday == 1)
    encoded += power;

  return encoded;
}

void decodeWeekdaySetup(unsigned char encoded, IntervalConfig *intervalConfig) {
  if (encoded % 2 == 1)
    intervalConfig->sunday = 1;
  else
    intervalConfig->sunday = 0;
  encoded = encoded >> 1;

  if (encoded % 2 == 1)
    intervalConfig->monday = 1;
  else
    intervalConfig->monday = 0;
  encoded = encoded >> 1;

  if (encoded % 2 == 1)
    intervalConfig->tuesday = 1;
  else
    intervalConfig->tuesday = 0;
  encoded = encoded >> 1;

  if (encoded % 2 == 1)
    intervalConfig->wednesday = 1;
  else
    intervalConfig->wednesday = 0;
  encoded = encoded >> 1;

  if (encoded % 2 == 1)
    intervalConfig->thursday = 1;
  else
    intervalConfig->thursday = 0;
  encoded = encoded >> 1;

  if (encoded % 2 == 1)
    intervalConfig->friday = 1;
  else
    intervalConfig->friday = 0;
  encoded = encoded >> 1;

  if (encoded % 2 == 1)
    intervalConfig->saturday = 1;
  else
    intervalConfig->saturday = 0;
}

void RtcHelper::writeTime(RtcDS1307<TwoWire> *rtc, VarContainer *vars) {
  int y, M, d, h, m, s;

  y = vars->getCurrentTime()->year;
  M = vars->getCurrentTime()->month;
  d = vars->getCurrentTime()->day;
  h = vars->getCurrentTime()->hour;
  m = vars->getCurrentTime()->minute;
  s = 0;

  RtcDateTime datetime = RtcDateTime(y, M, d, h, m, s);
  rtc->SetDateTime(datetime);
}
void RtcHelper::readTime(RtcDS1307<TwoWire> *rtc, VarContainer *vars) {
  RtcDateTime datetime = rtc->GetDateTime();
  int y, M, d, h, m, dow;

  y = datetime.Year();
  M = datetime.Month();
  d = datetime.Day();
  h = datetime.Hour();
  m = datetime.Minute();
  dow = datetime.DayOfWeek();

  vars->getCurrentTime()->year = y;
  vars->getCurrentTime()->month = M;
  vars->getCurrentTime()->day = d;
  vars->getCurrentTime()->hour = h;
  vars->getCurrentTime()->minute = m;
  vars->getCurrentTime()->dayOfWeek = dow;
}
void RtcHelper::writeConfig(RtcDS1307<TwoWire> *rtc, VarContainer *vars) {
  unsigned char rtcMemPtr = RTC_MEM_OFFSET;
  rtc->SetMemory(rtcMemPtr++, 0);
  int precision = 2;

  // write maxRainInDay as two integers (integer and decimal)
  unsigned char tmpInt = (unsigned char)vars->maxRainInDay;
  unsigned char tmpDec =
      (unsigned char)((vars->maxRainInDay - tmpInt) * std::pow(10, precision) +
                      0.5);
  rtc->SetMemory(rtcMemPtr++, tmpInt);
  rtc->SetMemory(rtcMemPtr++, tmpDec);

  // write rainToday as two integers (integer and decimal)
  tmpInt = (unsigned char)vars->rainToday;
  tmpDec =
      (unsigned char)((vars->rainToday - tmpInt) * std::pow(10, precision) +
                      0.5);
  rtc->SetMemory(rtcMemPtr++, tmpInt);
  rtc->SetMemory(rtcMemPtr++, tmpDec);

  for (unsigned char i = 0; i < ZONE_NUM; i++) {
    rtc->SetMemory(rtcMemPtr++, vars->getZoneConfig(i)->forceOpenValve);
    rtc->SetMemory(rtcMemPtr++, vars->getZoneConfig(i)->wateringPreset);
    for (unsigned char j = 0; j < INTERVAL_NUM; j++) {
      rtc->SetMemory(rtcMemPtr++,
                     vars->getZoneConfig(i)->getInterval(j)->openValveHour);
      rtc->SetMemory(rtcMemPtr++,
                     vars->getZoneConfig(i)->getInterval(j)->openValveMinute);
      rtc->SetMemory(rtcMemPtr++,
                     vars->getZoneConfig(i)->getInterval(j)->closeValveHour);
      rtc->SetMemory(rtcMemPtr++,
                     vars->getZoneConfig(i)->getInterval(j)->closeValveMinute);
      unsigned char weekdaySetup =
          encodeWeekdaySetup(vars->getZoneConfig(i)->getInterval(j));
      rtc->SetMemory(rtcMemPtr++, weekdaySetup);
    }
  }
}
void RtcHelper::readConfig(RtcDS1307<TwoWire> *rtc, VarContainer *vars) {
  unsigned char rtcMemPtr = RTC_MEM_OFFSET;
  rtc->GetMemory(rtcMemPtr++);

  // read maxRainInDay from two integers (integer and decimal part)
  unsigned char tmpInt = rtc->GetMemory(rtcMemPtr++);
  unsigned char tmpDec = rtc->GetMemory(rtcMemPtr++);
  vars->maxRainInDay = tmpInt + tmpDec / 100.0;

  // read rainToday from two integers (integer and decimal part)
  tmpInt = rtc->GetMemory(rtcMemPtr++);
  tmpDec = rtc->GetMemory(rtcMemPtr++);
  vars->rainToday = tmpInt + tmpDec / 100.0;

  for (unsigned char i = 0; i < ZONE_NUM; i++) {
    vars->getZoneConfig(i)->forceOpenValve = rtc->GetMemory(rtcMemPtr++);
    vars->getZoneConfig(i)->wateringPreset = rtc->GetMemory(rtcMemPtr++);
    for (unsigned char j = 0; j < INTERVAL_NUM; j++) {
      vars->getZoneConfig(i)->getInterval(j)->openValveHour =
          rtc->GetMemory(rtcMemPtr++);
      vars->getZoneConfig(i)->getInterval(j)->openValveMinute =
          rtc->GetMemory(rtcMemPtr++);
      vars->getZoneConfig(i)->getInterval(j)->closeValveHour =
          rtc->GetMemory(rtcMemPtr++);
      vars->getZoneConfig(i)->getInterval(j)->closeValveMinute =
          rtc->GetMemory(rtcMemPtr++);
      decodeWeekdaySetup(rtc->GetMemory(rtcMemPtr++),
                         vars->getZoneConfig(i)->getInterval(j));
    }
  }
}
