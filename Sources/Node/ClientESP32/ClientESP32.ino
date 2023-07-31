/*
   Example integration with the GRIDSHELL
   ESP does idle and takes load from Grid Server if available
   https://www.gridshell.net/
   https://github.com/invpe/gridshell
*/ 
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "SPIFFS.h"
#include "CGridShell.h"
////////////////////////////////
// Your WiFi Credentials here //
////////////////////////////////
#define WIFI_SSID ""
#define WIFI_PWD  ""
////////////////////////////////
// Your GridShell user-hash   //
////////////////////////////////
#define GRID_USERHASH ""
///////////////////////////////////
// Turn on/off OTA functionality //
///////////////////////////////////
#define ENABLE_OTA
/////////////////////////////////////
// Turn on/off Diagnostic telemetry//
/////////////////////////////////////
//#define ENABLE_TELEMETRY
#ifdef ENABLE_TELEMETRY
  #define ANALOG_PIN A0
  const char* ntpServer = "pool.ntp.org";
  const long  gmtOffset_sec = 3600;
  const int   daylightOffset_sec = 3600;
  uint64_t uiLastTelemetry = 0;
#endif
//////////////////////////////////////////////
// LED Functionality                        //
//////////////////////////////////////////////
#define LED_BUILTIN 2
///////////////////////////////////
// Callback                      //
///////////////////////////////////
void GridShellCB(uint8_t  uiEventType)
{
  switch (uiEventType)
  {
    ///////////////////////////////////
    // Node is executing a task     //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_WORK:
      {
        digitalWrite(LED_BUILTIN, HIGH);
      }
      break;
    ///////////////////////////////////
    // Node is idling               //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_IDLE:
      {
        digitalWrite(LED_BUILTIN, LOW);
      }
      break;
  }
}
///////////////////////////////////
// Sketch setup                  //
///////////////////////////////////
void setup()
{
  //
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Mounting FS...");
  while (!SPIFFS.begin())
  {
    SPIFFS.format();
    Serial.println("Failed to mount file system");
    delay(1000);
  }

  //
  Serial.println("Connecting to WiFi");

  // Connect to WiFi, internet required ;-)
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWD);

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
  if (CGridShell::GetInstance().Init(GRID_USERHASH,true) == true)
  {
  }
  else
    ESP.restart();

  // Register callback, comment if not used
  CGridShell::GetInstance().RegisterEventCallback(GridShellCB);

  // Start OTA if enabled
#ifdef ENABLE_OTA
  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else  // U_SPIFFS
      type = "filesystem";
  })
  .onEnd([]() {
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    yield();
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
    ESP.restart();
  });
  ArduinoOTA.setHostname("GridShell_Node");
  ArduinoOTA.begin();
#endif

#ifdef ENABLE_TELEMETRY  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
#endif


  digitalWrite(LED_BUILTIN, LOW);
}
///////////////////////////////////
// Sketch loop                   //
///////////////////////////////////
void loop()
{
  
  static uint32_t m_uiLastHB   = 0;

  // This keeps handling the OTA functionality
#ifdef ENABLE_OTA
  ArduinoOTA.handle();
#endif

  /////////////////////////////
  // Keep working on the grid//
  /////////////////////////////
  CGridShell::GetInstance().Tick();

  // Check if WiFi available, if not just boot.
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WIFI Bad, rebooting");
    ESP.restart();
  }

  // Enable sending diagnostic telemetry every 10 minutes
  // New file will be created every day
#ifdef ENABLE_TELEMETRY
  if(millis() - uiLastTelemetry >= 60000ULL * 10)
  {
    if(CGridShell::GetInstance().Connected())
    {
        tm local_tm;
        getLocalTime(&local_tm);
        time_t tTimeSinceEpoch = mktime(&local_tm); 
        
        String strMAC = WiFi.macAddress(); 
        strMAC.replace(":", "");
        
        float fBatteryLevel = map(analogRead(ANALOG_PIN), 0.0f, 4095.0f, 0, 100);  
        
        String strFileName = "DT" + strMAC + String(local_tm.tm_year + 1900) + String(local_tm.tm_mon + 1) + String(local_tm.tm_mday);
        String strAppend = "1";
        String strPayload = strFileName + ",";
        
        strPayload += strAppend+",";
        strPayload += String(tTimeSinceEpoch) + ",";      
        strPayload += String(fBatteryLevel)+",";
        strPayload += String(SPIFFS.totalBytes())+",";
        strPayload += String(SPIFFS.usedBytes())+",";
        strPayload += String(WiFi.RSSI()) + ","; 
        strPayload += String(ESP.getFreeHeap()) + "\n"; 
         
        CGridShell::GetInstance().AddTask("write", strPayload); 
    }
    uiLastTelemetry = millis();
  }
#endif


  // Your Normal Sketch things
  if (millis() - m_uiLastHB > 1000)
  {
    m_uiLastHB = millis();
  }
}
