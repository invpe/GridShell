#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <chrono>
#include <vector>
#include "SPIFFS.h"
#include <ctime>
/*------------------*/
#include "CGridShell.h"
#define GRID_U ""
/*------------------*/
#define WIFI_A ""
#define WIFI_P ""
/*------------------*/
#define ANALOG_PIN A0
#define LED_BUILTIN 2
#define ONE_WIRE_BUS 4
/*------------------*/ 
// Enable deep sleep after run
#define ENABLE_DEEP_SLEEP
/*------------------*/
#ifdef ENABLE_DEEP_SLEEP
// Define the time to sleep in microseconds (30 minutes)
const uint64_t SLEEP_TIME = 30e6 * 60;
void DeepSleep() {
  CGridShell::GetInstance().Stop();
  esp_deep_sleep_start();
}
#endif
/*------------------*/
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
uint32_t uiLastTick = 0;
/*------------------*/
// Set your NTP settings here
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
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
#ifdef ENABLE_DEEP_SLEEP
  esp_sleep_enable_timer_wakeup(SLEEP_TIME);
#endif

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Mounting FS...");
  while (!SPIFFS.begin()) {
    SPIFFS.format();
    Serial.println("Failed to mount file system");
    delay(1000);
  }

  WiFi.setHostname("GSWHALE");
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

  // Start up the library
  Serial.println("SENSOR");
  sensors.begin();

  ///////////////////////////////////////////////////
  // Initialize GridShell Node with your user hash//
  ///////////////////////////////////////////////////
  Serial.println("GRID");

  // Do not enable Auto Updates
  if (CGridShell::GetInstance().Init(GRID_U, false) == true) {
  } else
    ESP.restart();

  Serial.println("NTP");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}
/*------------------*/
void loop()
{

  // Check if WiFi available, if not just boot.
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WIFI Bad, rebooting");
    ESP.restart();
  }


  // Tick the GS
  CGridShell::GetInstance().Tick();

  // Extract the time from NTP server
  tm local_tm;
  if (!getLocalTime(&local_tm)) {
    Serial.println("Failed to obtain time");
    return;
  }
  // Get epoch
  time_t timeSinceEpoch = mktime(&local_tm);

  // Action time
#ifndef ENABLE_DEEP_SLEEP
  if (millis() - uiLastTick > TIME_INTERVAL * 60000) {
#endif
    digitalWrite(LED_BUILTIN, HIGH);

    // Ensure grid online
    if (CGridShell::GetInstance().Connected())
    {
      String strPayload = "";
      uint32_t uiTaskID = 0;

      Serial.println("GridShell is up");

      Serial.println("Time to push data");

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

      float fBatteryLevel = map(analogRead(ANALOG_PIN), 0.0f, 4095.0f, 0, 100);
      uint32_t uiSpace = SPIFFS.totalBytes() - SPIFFS.usedBytes();

      String strFileSettings = "POOLT" + GetMACAddress(0) + String(local_tm.tm_year + 1900) + String(local_tm.tm_mon + 1) + String(local_tm.tm_mday) + ",1,";
      strPayload = String(timeSinceEpoch) + ",";
      strPayload += String(local_tm.tm_hour) + ",";
      strPayload += String(local_tm.tm_min) + ",";
      strPayload += String(WiFi.RSSI()) + ",";
      strPayload += String(uiSpace) + ",";
      strPayload += String(ESP.getFreeHeap()) + ",";
      strPayload += String(fTempC) + ",";
      strPayload += String(fBatteryLevel) + "\n";

      // Write CSV telemetry data (append)
      String strTaskPayload = strFileSettings + CGridShell::GetInstance().EncodeBase64(strPayload) + ",";
      CGridShell::GetInstance().AddTask("writedfs", strTaskPayload); 

      // Write JSON data (overwrite)
      strFileSettings = "POOLT" + GetMACAddress(0) + "J,0,";
      strPayload = "{\"Sensor\": \"" + GetMACAddress(0) + "\",";
      strPayload += "\"Temperature\": " + String(fTempC) + ",";
      strPayload += "\"Battery\": " + String(fBatteryLevel) + ",";
      strPayload += "\"Epoch\": " + String(timeSinceEpoch) + ",";
      strPayload += "\"Time\": \"" + String(local_tm.tm_year + 1900) + String(local_tm.tm_mon + 1) + String(local_tm.tm_mday) + " " + String(local_tm.tm_hour) + ":" + String(local_tm.tm_min) + "\"";
      strPayload += "}";

      strTaskPayload = strFileSettings + CGridShell::GetInstance().EncodeBase64(strPayload) + ",";
      CGridShell::GetInstance().AddTask("writedfs", strTaskPayload); 
    }

    Serial.println("Waiting for next cycle");
    uiLastTick = millis();
    digitalWrite(LED_BUILTIN, LOW);

#ifdef ENABLE_DEEP_SLEEP
    DeepSleep();
#endif
#ifndef ENABLE_DEEP_SLEEP
  }
#endif
}
