#include "JsonHelper.h"

void JsonHelper::readZoneConfigFromJson(JsonObject &body, VarContainer *vars) {
  vars->maxRainInDay = body["maxRainInDay"].as<float>();
  for (unsigned char i = 0; i < ZONE_NUM; i++) {
    unsigned char zoneId = body["zones"][i]["id"].as<unsigned char>();
    vars->getZoneConfig(zoneId)->forceOpenValve =
        body["zones"][i]["forceOpenValve"].as<unsigned char>();
    vars->getZoneConfig(zoneId)->wateringPreset =
        body["zones"][i]["wateringPreset"].as<unsigned char>();

    for (unsigned char j = 0; j < INTERVAL_NUM; j++) {
      unsigned char intervalId =
          body["zones"][i]["intervals"][j]["id"].as<unsigned char>();
      vars->getZoneConfig(zoneId)->getInterval(intervalId)->openValveHour =
          body["zones"][i]["intervals"][j]["openValveHour"].as<unsigned char>();
      vars->getZoneConfig(zoneId)->getInterval(intervalId)->openValveMinute =
          body["zones"][i]["intervals"][j]["openValveMinute"]
              .as<unsigned char>();
      vars->getZoneConfig(zoneId)->getInterval(intervalId)->closeValveHour =
          body["zones"][i]["intervals"][j]["closeValveHour"]
              .as<unsigned char>();
      vars->getZoneConfig(zoneId)->getInterval(intervalId)->closeValveMinute =
          body["zones"][i]["intervals"][j]["closeValveMinute"]
              .as<unsigned char>();
      vars->getZoneConfig(zoneId)->getInterval(intervalId)->sunday =
          body["zones"][i]["intervals"][j]["sunday"].as<unsigned char>();
      vars->getZoneConfig(zoneId)->getInterval(intervalId)->monday =
          body["zones"][i]["intervals"][j]["monday"].as<unsigned char>();
      vars->getZoneConfig(zoneId)->getInterval(intervalId)->tuesday =
          body["zones"][i]["intervals"][j]["tuesday"].as<unsigned char>();
      vars->getZoneConfig(zoneId)->getInterval(intervalId)->wednesday =
          body["zones"][i]["intervals"][j]["wednesday"].as<unsigned char>();
      vars->getZoneConfig(zoneId)->getInterval(intervalId)->thursday =
          body["zones"][i]["intervals"][j]["thursday"].as<unsigned char>();
      vars->getZoneConfig(zoneId)->getInterval(intervalId)->friday =
          body["zones"][i]["intervals"][j]["friday"].as<unsigned char>();
      vars->getZoneConfig(zoneId)->getInterval(intervalId)->saturday =
          body["zones"][i]["intervals"][j]["saturday"].as<unsigned char>();
    }
  }
}

void JsonHelper::writeZoneConfigToJson(DynamicJsonDocument *doc,
                                       VarContainer *vars) {
  (*doc)["maxRainInDay"] = vars->maxRainInDay;
  JsonArray zones = doc->createNestedArray("zones");
  JsonObject tmp;

  for (unsigned char i = 0; i < ZONE_NUM; i++) {
    tmp = zones.createNestedObject();
    tmp["id"] = vars->getZoneConfig(i)->id;
    tmp["forceOpenValve"] = vars->getZoneConfig(i)->forceOpenValve;
    tmp["wateringPreset"] = vars->getZoneConfig(i)->wateringPreset;
    JsonArray intervals = tmp.createNestedArray("intervals");

    for (unsigned char j = 0; j < INTERVAL_NUM; j++) {
      tmp = intervals.createNestedObject();
      tmp["id"] = vars->getZoneConfig(i)->getInterval(j)->id;
      tmp["openValveHour"] =
          vars->getZoneConfig(i)->getInterval(j)->openValveHour;
      tmp["openValveMinute"] =
          vars->getZoneConfig(i)->getInterval(j)->openValveMinute;
      tmp["closeValveHour"] =
          vars->getZoneConfig(i)->getInterval(j)->closeValveHour;
      tmp["closeValveMinute"] =
          vars->getZoneConfig(i)->getInterval(j)->closeValveMinute;
      tmp["sunday"] = vars->getZoneConfig(i)->getInterval(j)->sunday;
      tmp["monday"] = vars->getZoneConfig(i)->getInterval(j)->monday;
      tmp["tuesday"] = vars->getZoneConfig(i)->getInterval(j)->tuesday;
      tmp["wednesday"] = vars->getZoneConfig(i)->getInterval(j)->wednesday;
      tmp["thursday"] = vars->getZoneConfig(i)->getInterval(j)->thursday;
      tmp["friday"] = vars->getZoneConfig(i)->getInterval(j)->friday;
      tmp["saturday"] = vars->getZoneConfig(i)->getInterval(j)->saturday;
    }
  }
}

void JsonHelper::readBasicTime(DynamicJsonDocument *doc, VarContainer *vars) {
  JsonObject json = (*doc).as<JsonObject>();
  const char *date = json["datetime"].as<const char *>();
  unsigned char dayOfWeek = json["day_of_week"].as<unsigned char>();

  int y, M, d, h, m;
  float s;
  sscanf(date, "%d-%d-%dT%d:%d:%fZ", &y, &M, &d, &h, &m, &s);

  vars->getCurrentTime()->year = y;
  vars->getCurrentTime()->month = M;
  vars->getCurrentTime()->day = d;
  vars->getCurrentTime()->hour = h;
  vars->getCurrentTime()->minute = m;
  vars->getCurrentTime()->dayOfWeek = dayOfWeek;
}
