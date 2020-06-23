#include "LcdHelper.h"

void LcdHelper::writeDate(LiquidCrystal *lcd, BasicTime* datetime) {
  int y, M, d, h, m;
  y = datetime->year;
  M = datetime->month;
  d = datetime->day;
  h = datetime->hour;
  m = datetime->minute;

  lcd->setCursor(0, 0);
  lcd->print(y);
  lcd->print("-");
  if (M < 10)
    lcd->print("0");
  lcd->print(M);
  lcd->print("-");
  if (d < 10)
    lcd->print("0");
  lcd->print(d);
  lcd->print(" ");
  if (h < 10)
    lcd->print("0");
  lcd->print(h);
  lcd->print(":");
  if (m < 10)
    lcd->print("0");
  lcd->print(m);
}

void LcdHelper::writeRainToday(LiquidCrystal *lcd, float value){
  lcd->setCursor(0, 1);
  lcd->print("Opady mm:");
  lcd->print(value);
}
