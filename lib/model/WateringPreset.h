#ifndef WATERING_PRESET_H
#define WATERING_PRESET_H

#include <WateringPresets.h>

class WateringPreset {

private:
  unsigned char openHour = 0;
  unsigned char openMinute = 0;
  unsigned char closeHour = 0;
  unsigned char closeMinute = 0;

public:

  WateringPreset(){};
  WateringPreset(
    unsigned char openHour,
    unsigned char openMinute,
    unsigned char closeHour,
    unsigned char closeMinute
  ){
    this->openHour = openHour;
    this->openMinute = openMinute;
    this->closeHour = closeHour;
    this->closeMinute = closeMinute;
  }

  unsigned char getOpenHour();
  unsigned char getOpenMinute();
  unsigned char getCloseHour();
  unsigned char getCloseMinute();
};

#endif
