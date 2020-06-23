#ifndef STATION_CONTROLLER_H
#define STATION_CONTROLLER_H

#include <IOController.h>
#include <VarContainer.h>
#include <WateringPreset.h>

class StationController {

private:
  IOController *io;
  WateringPreset presets[PRESETS_NUM];

public:
  StationController(IOController *io) {
    this->io = io;

    //PRESETS
    // MINIMAL - 19.00 - 19.10
    // MEDMIUM - 19.00 - 19.20
    // INTENSE - 19.00 - 19.30
    // VERY_INTENSE - 19.00 - 19.40
    for (int i = 0; i < PRESETS_NUM; i++) {
      presets[i] = WateringPreset(19, 0, 19, (i+1)*10);
    }
  };

  void controlValves(VarContainer *vars);
};

#endif
