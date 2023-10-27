#include <ctime>
#include <chrono>
#include "CGridShell.h"
/*------------------*/
#define TELEMETRY_MINUTES 60000ULL * 30
#define WIFI_ACCESSPOINT ""
#define WIFI_PASSWORD ""
#define GRID_U ""
#define SENSOR_PIN 32
/*------------------*/
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
String strPlantName = "Maple Deshojo";
const float Vref = 3.3;
uint64_t m_uiLastSend = 0;
/*------------------*/
void setup() {
  Serial.begin(115200);
  bool status;

  while (!SPIFFS.begin()) {
    Serial.println("Formatting");
    SPIFFS.format();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_ACCESSPOINT, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(1000);
  }

  CGridShell::GetInstance().Init(GRID_U, false);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}
/*------------------*/
void loop() {


  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WIFI Bad, rebooting");
    ESP.restart();
  }

  CGridShell::GetInstance().Tick();

  if (millis() - m_uiLastSend >= TELEMETRY_MINUTES) {
    if (CGridShell::GetInstance().Connected()) {

      // Extract the time from NTP server
      tm local_tm;
      if (!getLocalTime(&local_tm)) {
        return;
      }
      time_t timeSinceEpoch = mktime(&local_tm);

      // Read sensor 
      int analogValue = analogRead(SENSOR_PIN);

      // Convert analog reading to voltage
      float voltage = (analogValue / 4095.0) * Vref;

      // Prepare file and task payload
      String strFileSettings = "FLORA" + String(local_tm.tm_year + 1900) + String(local_tm.tm_mon + 1) + String(local_tm.tm_mday) + ",1,";
      String strPayload = String(timeSinceEpoch) + ",";
      strPayload += String(local_tm.tm_hour) + ",";
      strPayload += String(local_tm.tm_min) + ",";
      strPayload += String(WiFi.RSSI()) + ",";
      strPayload += String(SPIFFS.totalBytes() - SPIFFS.usedBytes()) + ",";
      strPayload += String(ESP.getFreeHeap()) + ",";
      strPayload += strPlantName + ",";
      strPayload += String(4095 - analogValue) + "\n"; //aligns with the intuitive understanding that a higher value indicates a well-watered plant
      String strTaskPayload = strFileSettings + CGridShell::GetInstance().EncodeBase64(strPayload) + ",";

      // Submit task
      uint32_t uiTaskID = CGridShell::GetInstance().AddTask("writedfs", strTaskPayload);
      Serial.println("Task ID: " + String(uiTaskID));
    }
    m_uiLastSend = millis();
  }
}
