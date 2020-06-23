#include "StationController.h"

int toMinutes(int hour, int minute) {
  return (unsigned int)hour * 60 + (unsigned int)minute;
}

void StationController::controlValves(VarContainer *vars) {

  if (vars->rainToday >= vars->maxRainInDay)
    io->setAutoControlOffLight();
  else
    io->resetAutoControlOffLight();

  unsigned char anyValveForcedOpen = 0;
  unsigned char anyIntervalError = 0;

  for (unsigned char i = 0; i < ZONE_NUM; i++) {
    if (vars->getZoneConfig(i)->forceOpenValve) {
      anyValveForcedOpen = 1;
      io->openValve(i);
      io->setValveLight(i);
      io->setForceOpenLight();
      continue;
    }

    if (anyValveForcedOpen == 0)
      io->resetForceOpenLight();

    unsigned char anyIntervalOpen = 0;
    unsigned int openMinute = 0;
    unsigned int closeMinute = 0;
    unsigned int currentMinute = 0;
    unsigned int wateringPreset = vars->getZoneConfig(i)->wateringPreset;
    unsigned int dayConfig = 0;

    for (unsigned char j = 0; j < INTERVAL_NUM; j++) {

      if (wateringPreset != 0) {
        openMinute = toMinutes(presets[wateringPreset - 1].getOpenHour(),
                               presets[wateringPreset - 1].getOpenMinute());
        closeMinute = toMinutes(presets[wateringPreset - 1].getCloseHour(),
                                presets[wateringPreset - 1].getCloseMinute());
        dayConfig = 1;
      } else {
        openMinute =
            toMinutes(vars->getZoneConfig(i)->getInterval(j)->openValveHour,
                      vars->getZoneConfig(i)->getInterval(j)->openValveMinute);
        closeMinute =
            toMinutes(vars->getZoneConfig(i)->getInterval(j)->closeValveHour,
                      vars->getZoneConfig(i)->getInterval(j)->closeValveMinute);
        dayConfig = vars->getZoneConfig(i)->getInterval(j)->getDayConfig(
            vars->getCurrentTime()->dayOfWeek);
      }

      if (closeMinute < openMinute) {
        io->setErrorLight();
        io->closeValve(i);
        io->resetValveLight(i);
        anyIntervalError = 1;
        continue;
      }

      if (anyIntervalError == 0)
        io->resetErrorLight();

      currentMinute = toMinutes(vars->getCurrentTime()->hour,
                                vars->getCurrentTime()->minute);

      if (openMinute <= currentMinute && currentMinute < closeMinute &&
          dayConfig == 1 && vars->rainToday < vars->maxRainInDay) {
        anyIntervalOpen = 1;
        io->openValve(i);
        io->setValveLight(i);
        continue;
      }

      if (anyIntervalOpen == 0) {
        io->closeValve(i);
        io->resetValveLight(i);
      }
    }
  }
}
