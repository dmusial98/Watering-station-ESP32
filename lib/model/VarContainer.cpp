#include "VarContainer.h"

ZoneConfig* VarContainer::getZoneConfig(unsigned char index) {
  return &zones[index];
}

BasicTime* VarContainer::getCurrentTime(){
  return &currentTime;
}
