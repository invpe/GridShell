#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <chrono>
#include <vector>
#include "SPIFFS.h"
#include <ctime>
/*------------------*/
#include "CGridShell.h"
#define GRID_U  ""
/*------------------*/
#define WIFI_A  ""
#define WIFI_P  ""
/*------------------*/
#define TIME_INTERVAL 5
#define TIME_OFFSET 7200
#define LED_BUILTIN 2
#define ONE_WIRE_BUS 4
/*------------------*/
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
uint32_t uiLastTick = 0;
/*------------------*/
String GetMACAddress(const int& iType)
{
  String strMac;

  uint8_t marray[6] = {0, 0, 0, 0, 0, 0};
  char MAC_char[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  if (iType == 0)
    WiFi.macAddress(marray);
  else
    WiFi.softAPmacAddress(marray);

  for (int i = 0; i < sizeof(marray); ++i)
    sprintf(MAC_char, "%s%02X", MAC_char, marray[i]);

  return String(MAC_char);
}
/*------------------*/
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Mounting FS...");
  while (!SPIFFS.begin())
  {
    SPIFFS.format();
    Serial.println("Failed to mount file system");
    delay(1000);

  }

  WiFi.begin(WIFI_A, WIFI_P);

  // Give it 10 seconds to connect, otherwise reboot
  uint8_t iRetries = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(1000);
    iRetries += 1;

    if (iRetries >= 10)
      ESP.restart();
  }

  Serial.println("Connected " + WiFi.localIP().toString());

  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else  // U_SPIFFS
      type = "filesystem";
  })
  .onEnd([]() {
    Serial.println(F("\nEnd"));
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println(F("Auth Failed"));
    else if (error == OTA_BEGIN_ERROR)
      Serial.println(F("Begin Failed"));
    else if (error == OTA_CONNECT_ERROR)
      Serial.println(F("Connect Failed"));
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println(F("Receive Failed"));
    else if (error == OTA_END_ERROR)
      Serial.println(F("End Failed"));
    esp_restart();
  });

  ArduinoOTA.setHostname("GSPOOLT");
  ArduinoOTA.begin();


  // Start up the library
  sensors.begin();

  ///////////////////////////////////////////////////
  // Initialize GridShell Node with your user hash//
  ///////////////////////////////////////////////////
  if (CGridShell::GetInstance().Init(GRID_U, true) == true)
  {
  }
  else
    ESP.restart();


  // Set time offset
  timeClient.setTimeOffset(TIME_OFFSET);
  timeClient.begin();
  timeClient.update();


}
/*------------------*/
void loop() {

  //
  CGridShell::GetInstance().Tick();

  ArduinoOTA.handle();

  // This sketch is time dependent
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  // Extract the time from NTP server
  time_t _t;
  _t = timeClient.getEpochTime();
  std::chrono::system_clock::time_point now = std::chrono::system_clock::from_time_t(_t);
  time_t tt = std::chrono::system_clock::to_time_t(now);
  tm local_tm = *localtime(&tt);

  //
  if (millis() - uiLastTick > TIME_INTERVAL * 60000)
  {

    digitalWrite(LED_BUILTIN, HIGH);


    // Ensure grid online
    if (CGridShell::GetInstance().Connected())
    {
      // Get temperature
      sensors.requestTemperatures();
      float fTempC = sensors.getTempCByIndex(0);

      // Check if reading was successful and submit telemetry task
      if (fTempC != DEVICE_DISCONNECTED_C)
      {   }
      else
      {
        fTempC = 0;

        Serial.println("Error: Could not read temperature data");
      }

      String strFileName = "POOLT" + GetMACAddress(0) + String(local_tm.tm_year + 1900) + String(local_tm.tm_mon + 1) + String(local_tm.tm_mday);

      String strPayload = strFileName + ",1,";
      strPayload += String(timeClient.getEpochTime()) + ",";
      strPayload += String(local_tm.tm_hour) + ",";
      strPayload += String(local_tm.tm_min) + ",";
      strPayload += String(WiFi.RSSI()) + ",";
      strPayload += String(SPIFFS.usedBytes()) + ",";
      strPayload += String(ESP.getFreeHeap()) + ",";
      strPayload += String(fTempC) + "\n";

      uint32_t uiTaskID = CGridShell::GetInstance().AddTask("write", strPayload);
      Serial.println("Write task :" + String(uiTaskID));

      // Submit Widget task
      String strTaskPayload = "POOLTW" + GetMACAddress(0);
      strTaskPayload += ",Pool,Pool Temperature,";
      strTaskPayload += "Temperature\\n";
      strTaskPayload += timeClient.getFormattedTime();
      strTaskPayload += "\\n";

      if (fTempC != DEVICE_DISCONNECTED_C)
        strTaskPayload += String(fTempC);
      else
        strTaskPayload += "??";

      strTaskPayload += ",#AAAAAA";
      strTaskPayload += ",https://gridshell.net/,";

      uiTaskID = CGridShell::GetInstance().AddTask("widget", strTaskPayload);
      Serial.println("Widget task: " + String(uiTaskID));


    }
    uiLastTick = millis();
    digitalWrite(LED_BUILTIN, LOW);
  }
}
