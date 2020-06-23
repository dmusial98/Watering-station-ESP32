#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include "ArduinoJson.h"
#include "AsyncJson.h"
#include "VarContainer.h"

class JsonHelper {
public:
  JsonHelper(){};

  void readZoneConfigFromJson(JsonObject& body, VarContainer* vars);
  void writeZoneConfigToJson(DynamicJsonDocument* doc, VarContainer* vars);
  void readBasicTime(DynamicJsonDocument* doc, VarContainer* vars);

};

#endif
