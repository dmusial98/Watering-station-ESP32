#ifndef I_O_CONTROLLER_H
#define I_O_CONTROLLER_H

#include <Arduino.h>

#define PIN_VALVE_0 0
#define PIN_VALVE_1 2

//orange LED 1, signals opened valve 1
#define PIN_LED_VALVE_0 25

//orange LED 2, signals opened valve 2
#define PIN_LED_VALVE_1 26

//blue LED, user forced valve to open
#define PIN_LED_MANUAL_ON 23

//yellow LED, automatic controll turned off due to rain
#define PIN_LED_AUTO_OFF 32

//green LED, signals any network event
#define PIN_LED_NETWORK 33

//red LED, signals any error
#define PIN_LED_ERROR 13


class IOController {

public:

  IOController(){};

  void init();

  void openValve(unsigned char zoneId);
  void closeValve(unsigned char zoneId);

  void setValveLight(unsigned char zoneId);
  void resetValveLight(unsigned char zoneId);

  void setForceOpenLight();
  void resetForceOpenLight();

  void setAutoControlOffLight();
  void resetAutoControlOffLight();

  void setErrorLight();
  void resetErrorLight();

  void blinkNetworkLight();
  void blinkErrorLight();
};

#endif
