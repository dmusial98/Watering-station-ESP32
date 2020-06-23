#include "ZoneConfig.h"

IntervalConfig* ZoneConfig::getInterval(unsigned char index) {
  return &intervals[index];
}
