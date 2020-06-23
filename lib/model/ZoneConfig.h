#ifndef ZONE_CONFIG_H
#define ZONE_CONFIG_H

#include <IntervalConfig.h>
#include <BasicTime.h>

#define INTERVAL_NUM 3

class ZoneConfig {

private:
  IntervalConfig intervals[INTERVAL_NUM];


public:
  unsigned char id = 0;
  unsigned char forceOpenValve = 0;
  unsigned char wateringPreset = 0;

  ZoneConfig() {
    for (unsigned char i = 0; i < INTERVAL_NUM; i++) {
      intervals[i] = IntervalConfig();
      intervals[i].id = i;
    }
    
  };

  IntervalConfig *getInterval(unsigned char index);

};

#endif
