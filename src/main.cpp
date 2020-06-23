#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <JsonHelper.h>
#include <LcdHelper.h>
#include <RtcHelper.h>
#include <StationController.h>
#include <WiFi.h>
#include <ctime>

#define SECONDS_IN_HOUR 3600
#define WEATHER_API_REQ                                                        \
  "http://api.openweathermap.org/data/2.5/"                                    \
  "weather?id=3099230&appid=ac3f34fca395ad1ed070669087a2119d"
#define TIME_API_REQ "http://worldtimeapi.org/api/ip"

const char *ssid = "iot";
const char *password = "esp12345";

AsyncWebServer server(80);
VarContainer vars = VarContainer();
JsonHelper jsonHelper = JsonHelper();
LiquidCrystal lcd(19, 18, 5, 17, 16, 4);
LcdHelper lcdHelper = LcdHelper();
HTTPClient http;
RtcDS1307<TwoWire> rtc(Wire);
RtcHelper rtcHelper = RtcHelper();
IOController io = IOController();
StationController controller = StationController(&io);

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile bool isTimerISR = false;

// timer ticks every second, so there needs to be a second variable to determine
// that an hour has passed
volatile int hourCounter = SECONDS_IN_HOUR;

void IRAM_ATTR onTimer() {
  if (hourCounter != 0) {
    portENTER_CRITICAL_ISR(&timerMux);
    hourCounter--;
    portEXIT_CRITICAL_ISR(&timerMux);
    return;
  }
  portENTER_CRITICAL_ISR(&timerMux);
  hourCounter = SECONDS_IN_HOUR;
  isTimerISR = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void requestWeatherInfo() {
  // for some reason HTTPCLient always needs exactly 4 attempts
  // when performing this request for the first time
  for (int i = 0; i < 3; i++) {
    int code = http.GET();
    if (code == 200) {
      String payload = http.getString();
      DynamicJsonDocument doc(4096);
      deserializeJson(doc, payload);
      JsonObject json = doc.as<JsonObject>();
      // read rain in mm from the last hour
      float rainLastHour = json["rain"]["1h"].as<float>();
      vars.rainToday += rainLastHour;
      // reset at noon
      if (vars.getCurrentTime()->hour == 0)
        vars.rainToday = 0.0;
      rtcHelper.writeConfig(&rtc, &vars);
      io.blinkNetworkLight();
      break;
    }
  }
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "application/json", "{\"message\":\"not found\"}");
  Serial.println("404 SENT");
  io.blinkNetworkLight();
}

void connectToWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  io.blinkNetworkLight();
}

void setup() {
  Serial.begin(115200);

  // setup io control
  io.init();

  while (WiFi.status() != WL_CONNECTED) {
    connectToWifi();

    if (WiFi.status() != WL_CONNECTED){
      io.blinkErrorLight();
      delay(1000);
    }
  }

  lcd.begin(16, 2);
  rtc.Begin();
  
  rtcHelper.readConfig(&rtc, &vars);
  http.begin(TIME_API_REQ);

  // get time from time API
  int httpCode = http.GET();
  if (httpCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, payload);
    jsonHelper.readBasicTime(&doc, &vars);
    lcdHelper.writeDate(&lcd, vars.getCurrentTime());
    rtcHelper.writeTime(&rtc, &vars);
    io.blinkNetworkLight();
  }

  // Define HTTP Endpoints

  server.on("/rain", HTTP_GET, [](AsyncWebServerRequest *request) {
    DynamicJsonDocument buff(256);
    buff["rainToday"] = vars.rainToday;
    AsyncResponseStream *response =
        request->beginResponseStream("application/json");
    serializeJson(buff, *response);
    request->send(response);
    io.blinkNetworkLight();
  });

  server.on("/configuration", HTTP_GET, [](AsyncWebServerRequest *request) {
    DynamicJsonDocument buff(2048);
    jsonHelper.writeZoneConfigToJson(&buff, &vars);
    AsyncResponseStream *response =
        request->beginResponseStream("application/json");
    serializeJson(buff, *response);
    request->send(response);
    io.blinkNetworkLight();
  });

  // PUT with json body handler
  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler(
      "/configuration",
      [](AsyncWebServerRequest *request, JsonVariant &json) {
        JsonObject body = json.as<JsonObject>();
        jsonHelper.readZoneConfigFromJson(body, &vars);
        rtcHelper.writeConfig(&rtc, &vars);
        request->send(200, "application/json", "{\"status\":\"OK\"}");
        io.blinkNetworkLight();
      },
      4096);
  server.addHandler(handler);

  server.onNotFound(notFound);
  server.begin();

  // init http to call weather api in main loop
  http.begin(WEATHER_API_REQ);

  // one second timer interrupt
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void loop() {

  rtcHelper.readTime(&rtc, &vars);
  lcdHelper.writeDate(&lcd, vars.getCurrentTime());
  lcdHelper.writeRainToday(&lcd, vars.rainToday);

  controller.controlValves(&vars);

  // auto reconnect wifi
  if (WiFi.status() != WL_CONNECTED) {
    io.blinkErrorLight();
    Serial.println("WiFi Disconnected");
    Serial.println("Reconnecting..   .");
    connectToWifi();
    if (WiFi.status() != WL_CONNECTED)
      delay(1000);
  }

  if (isTimerISR && WiFi.status() == WL_CONNECTED) {
    Serial.println("Enter Timer ISR");

    requestWeatherInfo();

    portENTER_CRITICAL(&timerMux);
    isTimerISR = false;
    portEXIT_CRITICAL(&timerMux);

    Serial.println("Exit Timer ISR");
  }
}
