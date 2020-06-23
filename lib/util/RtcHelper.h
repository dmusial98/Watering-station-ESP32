#ifndef RTC_HELPER_H
#define RTC_HELPER_H

#include <Wire.h>
#include <RtcDS1307.h>
#include <VarContainer.h>
#include <Arduino.h>

// NOTE!!
//(SDA, SCL) = (21, 22)

// offset needed, because writing/saving from RTC nvram
// did not work for lower adresses
// edit: seems to be working with 0 offset after erasing RTC's ram
const unsigned char RTC_MEM_OFFSET = 0;

class RtcHelper {

public:
  void writeTime(RtcDS1307<TwoWire> *rtc, VarContainer* vars);
  void readTime(RtcDS1307<TwoWire> *rtc, VarContainer* vars);
  void writeConfig(RtcDS1307<TwoWire> *rtc, VarContainer* vars);
  void readConfig(RtcDS1307<TwoWire> *rtc, VarContainer* vars);
};

#endif
