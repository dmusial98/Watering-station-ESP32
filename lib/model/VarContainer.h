#ifndef VAR_CONTAINER_H
#define VAR_CONTAINER_H

#include <ZoneConfig.h>

#define ZONE_NUM 2

class VarContainer {

private:
  ZoneConfig zones[ZONE_NUM];
  BasicTime currentTime;

public:
  //today's rain amount in mm
  float rainToday;
  float maxRainInDay;

  VarContainer() {
    for (unsigned char i = 0; i < ZONE_NUM; i++) {
      zones[i] = ZoneConfig();
      zones[i].id = i;
    }
    currentTime = BasicTime();
    rainToday = 0.0;
    maxRainInDay = 0.0;
  };

  ZoneConfig *getZoneConfig(unsigned char index);
  BasicTime *getCurrentTime();
};

#endif
