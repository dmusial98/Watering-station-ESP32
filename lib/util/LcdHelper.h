#ifndef LCD_HELPER_H
#define LCD_HELPER_H

#include <LiquidCrystal.h>
#include <BasicTime.h>

class LcdHelper {
public:
  LcdHelper(){};
  void writeDate(LiquidCrystal *lcd, BasicTime* datetime);
  void writeRainToday(LiquidCrystal *lcd, float value);
};

#endif
