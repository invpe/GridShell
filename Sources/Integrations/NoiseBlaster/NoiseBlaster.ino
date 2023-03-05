/*
   NoiseBlaster a GridShell driven Noise levels monitor
   This device is using DFRobot https://wiki.dfrobot.com/Gravity__Analog_Sound_Level_Meter_SKU_SEN0232
   to measure the value every minute and write it to a telemetry file on GridShell network.

*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <ArduinoOTA.h>
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
#define VREF  3.3

uint32_t uiSensorPoolTimer;
TaskHandle_t Task1;
int iLastDay = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7200);

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
void setup()
{
  Serial.begin(115200);

  Serial.println("Mounting FS...");
  while (!SPIFFS.begin())
  {
    SPIFFS.format();
    Serial.println("Failed to mount file system");
    delay(1000);
  }
  pinMode(A0, INPUT);


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


  ///////////////////////////////////////////////////
  // Initialize GridShell Node with your user hash//
  ///////////////////////////////////////////////////
  if (CGridShell::GetInstance().Init(GRID_U) == true)
  {
  }
  else
    ESP.restart();
 
    
  // Set time offset    
  timeClient.setTimeOffset(3600);
  timeClient.begin();
  timeClient.update();
 

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

  ArduinoOTA.setHostname("NBLASTER");
  ArduinoOTA.begin();



}
void loop()
{

  //
  ArduinoOTA.handle();

  // This sketch is time dependent
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  // Keep ticking
  CGridShell::GetInstance().Tick();

  // Ensure grid online
  if (CGridShell::GetInstance().Connected())
  {
    // Every minute
    if (millis() - uiSensorPoolTimer >= 60000)
    {
      time_t _t;
      _t = timeClient.getEpochTime();

      std::chrono::system_clock::time_point now = std::chrono::system_clock::from_time_t(_t);
      time_t tt = std::chrono::system_clock::to_time_t(now);
      tm local_tm = *localtime(&tt);

      float voltageValue, dbValue;
      voltageValue  = analogRead(A0);
      dbValue       = (voltageValue / 4095 * VREF) * 50 ;

      // File name - you can see it is rotating per day to new file
      // NB - Noise blaster
      // MAC - Mac of the sensor
      // YEAR - 2023
      // MONTH - 2
      // DAY - 13
      // We will be storing 24hrs of data per day for each sensor
      String strFileName    = "NB" + GetMACAddress(0) + String(local_tm.tm_year + 1900) + String(local_tm.tm_mon + 1) + String(local_tm.tm_mday);

      // Keep CSV (Comma separated for DataCopia visualisations)
      // HOUR,MINUTE,DBI VALUE
      String strTextToWrite = String(timeClient.getEpochTime()) + "," + String(local_tm.tm_hour) + "," + String(local_tm.tm_min) + "," + String(dbValue) + "\n";

      // Check if file exists, if not add header columns
      // String strRead = CGridShell::GetInstance().Read("PocNetGroupMiners00000000000000000000001" + strFileName, 0, 1);


      bool bSuccess   = CGridShell::GetInstance().Write(strFileName, strTextToWrite, true);

      Serial.println("Time : " + timeClient.getFormattedTime());
      Serial.println("Grid : " + String(CGridShell::GetInstance().Connected()));
      Serial.println("Value: " + String(dbValue));
      Serial.println("Memor: " + String(ESP.getFreeHeap()));
      Serial.println("Wrote: " + String(bSuccess));
      Serial.println("--------------------");

      uiSensorPoolTimer = millis();

    }
  }
}