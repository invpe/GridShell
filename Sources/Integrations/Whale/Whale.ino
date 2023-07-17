#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h> 
#include <ArduinoOTA.h>
#include <chrono>
#include <vector>
#include "SPIFFS.h"
#include <ctime> 
/*------------------*/
#include "CGridShell.h"
// Put your username here to submit daily average tasks for Your Whale sensor
//#define GRID_USERNAME "PocNetGroupMiners00000000000000000000001"
#define GRID_U ""
/*------------------*/
#define WIFI_A ""
#define WIFI_P ""
/*------------------*/
#define TIME_INTERVAL 10ULL
#define LED_BUILTIN 2
#define ONE_WIRE_BUS 4
/*------------------*/
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire); 
uint32_t uiLastTick = 0;
/*------------------*/
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;
/*------------------*/
String GetMACAddress(const int& iType) {
  String strMac;

  uint8_t marray[6] = { 0, 0, 0, 0, 0, 0 };
  char MAC_char[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

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
  while (!SPIFFS.begin()) {
    SPIFFS.format();
    Serial.println("Failed to mount file system");
    delay(1000);
  }

  WiFi.begin(WIFI_A, WIFI_P);

  // Give it 10 seconds to connect, otherwise reboot
  uint8_t iRetries = 0;
  while (WiFi.status() != WL_CONNECTED) {
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
  if (CGridShell::GetInstance().Init(GRID_U, true) == true) {
  } else
    ESP.restart();
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}
/*------------------*/
void loop() {

  //
  CGridShell::GetInstance().Tick();

  ArduinoOTA.handle();


  // Extract the time from NTP server
  tm local_tm;
  if(!getLocalTime(&local_tm)){
  Serial.println("Failed to obtain time");
  return;
} 

  time_t timeSinceEpoch = mktime(&local_tm); 


  //
  if (millis() - uiLastTick > TIME_INTERVAL * 60000) {
    Serial.println("Time to push data");
    digitalWrite(LED_BUILTIN, HIGH);


    // Ensure grid online
    if (CGridShell::GetInstance().Connected()) {

      Serial.println("GridShell is up");

      // Get temperature
      sensors.requestTemperatures();
      float fTempC = sensors.getTempCByIndex(0);

      // Check if reading was successful and submit telemetry task
      if (fTempC != DEVICE_DISCONNECTED_C) {
        Serial.println("Sensor reading OK");
      } else {
        fTempC = 0;

        Serial.println("Error: Could not read temperature data");
      }
      uint32_t uiSpace = SPIFFS.totalBytes() - SPIFFS.usedBytes();
      String strFileName = "POOLT" + GetMACAddress(0) + String(local_tm.tm_year + 1900) + String(local_tm.tm_mon + 1) + String(local_tm.tm_mday);

      String strPayload = strFileName + ",1,";
      strPayload += String(timeSinceEpoch) + ",";
      strPayload += String(local_tm.tm_hour) + ",";
      strPayload += String(local_tm.tm_min) + ",";
      strPayload += String(WiFi.RSSI()) + ",";
      strPayload += String(uiSpace) + ",";
      strPayload += String(ESP.getFreeHeap()) + ",";
      strPayload += String(fTempC) + "\n";

      // Write CSV telemetry data   (append)
      uint32_t uiTaskID = CGridShell::GetInstance().AddTask("write", strPayload);
      Serial.println("Write task :" + String(uiTaskID));

      // Write JSON data (overwrite)
      strFileName = "POOLT" + GetMACAddress(0) + "J";
      strPayload = strFileName + ",0,";
      strPayload += "{\"Sensor\": \"" + GetMACAddress(0) + "\",";
      strPayload += "\"Temperature\": " + String(fTempC) + ",";
      strPayload += "\"Epoch\": " + String(timeSinceEpoch) + ",";
      strPayload += "\"Time\": \"" + String(local_tm.tm_year + 1900) + String(local_tm.tm_mon + 1) + String(local_tm.tm_mday)+" "+String(local_tm.tm_hour)+":"+String(local_tm.tm_min) + "\"";
      strPayload += "}";
      uiTaskID = CGridShell::GetInstance().AddTask("write", strPayload);
      Serial.println("Json task: " + String(uiTaskID));

      // Are we at 23:50-59 ? Submit a daily average task to the grid
#ifdef GRID_USERNAME      
      if (local_tm.tm_hour == 23 && local_tm.tm_min >= 50) {
        strPayload = GRID_USERNAME "," + GetMACAddress(0) + "," + String(local_tm.tm_year + 1900) + String(local_tm.tm_mon + 1) + String(local_tm.tm_mday) + ",6,";
        uiTaskID = CGridShell::GetInstance().AddTask("whaledavg", strPayload);
        Serial.println("Average task :" + String(uiTaskID));
      }
#endif

    } else
      Serial.println("Gridshell is down");

    Serial.println("Waiting for next cycle");
    uiLastTick = millis();
    digitalWrite(LED_BUILTIN, LOW);
  }
}
