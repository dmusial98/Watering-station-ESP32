#include "IOController.h"

void xBlinkNetworkLight(void *parameter) {
  digitalWrite(PIN_LED_NETWORK, HIGH);
  vTaskDelay(500);
  digitalWrite(PIN_LED_NETWORK, LOW);
  vTaskDelete(NULL);
}

void xBlinkErrorLight(void *parameter) {
  digitalWrite(PIN_LED_ERROR, HIGH);
  vTaskDelay(500);
  digitalWrite(PIN_LED_ERROR, LOW);
  vTaskDelete(NULL);
}

void IOController::init() {
  pinMode(PIN_VALVE_0, OUTPUT);
  pinMode(PIN_VALVE_1, OUTPUT);
  pinMode(PIN_LED_VALVE_0, OUTPUT);
  pinMode(PIN_LED_VALVE_1, OUTPUT);
  pinMode(PIN_LED_MANUAL_ON, OUTPUT);
  pinMode(PIN_LED_AUTO_OFF, OUTPUT);
  pinMode(PIN_LED_NETWORK, OUTPUT);
  pinMode(PIN_LED_ERROR, OUTPUT);

  digitalWrite(PIN_VALVE_0, LOW);
  digitalWrite(PIN_VALVE_1, LOW);
  digitalWrite(PIN_LED_VALVE_0, LOW);
  digitalWrite(PIN_LED_VALVE_1, LOW);
  digitalWrite(PIN_LED_MANUAL_ON, LOW);
  digitalWrite(PIN_LED_AUTO_OFF, LOW);
  digitalWrite(PIN_LED_NETWORK, LOW);
  digitalWrite(PIN_LED_ERROR, LOW);
}

void IOController::openValve(unsigned char zoneId) {
  if (zoneId == 0)
    digitalWrite(PIN_VALVE_0, HIGH);
  else if (zoneId == 1)
    digitalWrite(PIN_VALVE_1, HIGH);
}

void IOController::closeValve(unsigned char zoneId) {
  if (zoneId == 0)
    digitalWrite(PIN_VALVE_0, LOW);
  else if (zoneId == 1)
    digitalWrite(PIN_VALVE_1, LOW);
}

void IOController::setValveLight(unsigned char zoneId) {
  if (zoneId == 0)
    digitalWrite(PIN_LED_VALVE_0, HIGH);
  else if (zoneId == 1)
    digitalWrite(PIN_LED_VALVE_1, HIGH);
}

void IOController::resetValveLight(unsigned char zoneId) {
  if (zoneId == 0)
    digitalWrite(PIN_LED_VALVE_0, LOW);
  else if (zoneId == 1)
    digitalWrite(PIN_LED_VALVE_1, LOW);
}

void IOController::setForceOpenLight() {
  digitalWrite(PIN_LED_MANUAL_ON, HIGH);
}

void IOController::resetForceOpenLight() {
  digitalWrite(PIN_LED_MANUAL_ON, LOW);
}

void IOController::setAutoControlOffLight() {
  digitalWrite(PIN_LED_AUTO_OFF, HIGH);
}

void IOController::resetAutoControlOffLight() {
  digitalWrite(PIN_LED_AUTO_OFF, LOW);
}

void IOController::setErrorLight() { digitalWrite(PIN_LED_ERROR, HIGH); }
void IOController::resetErrorLight() { digitalWrite(PIN_LED_ERROR, LOW); }

void IOController::blinkErrorLight() {
  xTaskCreate(xBlinkErrorLight, "blink_error", 1024, NULL, 1, NULL);
}

void IOController::blinkNetworkLight() {
  xTaskCreate(xBlinkNetworkLight, "blink_network", 1024, NULL, 1, NULL);
}
