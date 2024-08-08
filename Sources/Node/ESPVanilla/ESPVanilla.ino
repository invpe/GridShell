//
// GridShell Vanilla Node for ESP32 & ESP8266
// (c) 2022-2023-2024 GridShell.net
// https://github.com/invpe/GridShell
//
#if defined(ESP8266)
#pragma message "GridShell - ESP8266"
#include <ESP8266WiFi.h>
#include <FS.h>
#define LED_BUILTIN 2
#else
#pragma message "GridShell - ESP32"
#include <WiFi.h>
#include "SPIFFS.h"
#define LED_BUILTIN 27
#endif
#include <Preferences.h>
#include "CGridShell.h"
///////////////////////////////////
// DEEPSLEEP 5minutes            //
///////////////////////////////////
#define DEEP_SLEEP_TIME 5e6 * 60
String DEEP_SLEEP = "";
///////////////////////////////////
// NVRAM Prefs                   //
///////////////////////////////////
Preferences preferences;
///////////////////////////////////
// Config saver                  //
///////////////////////////////////
void SaveConfig(const String& WIFI_SSID, const String& WIFI_PWD, const String& GRID_USERHASH, const String& DEEPSLEEP) {
  preferences.begin("config", false);
  preferences.putString("ssid", WIFI_SSID);
  preferences.putString("pass", WIFI_PWD);
  preferences.putString("uhash", GRID_USERHASH);
  preferences.putString("dsleep", DEEPSLEEP);
  preferences.end();
}
///////////////////////////////////
// Config loader                 //
///////////////////////////////////
void LoadConfig(String& WIFI_SSID, String& WIFI_PWD, String& GRID_USERHASH, String& DEEPSLEEP) {
  preferences.begin("config", false);
  WIFI_SSID = preferences.getString("ssid", "");
  WIFI_PWD = preferences.getString("pass", "");
  GRID_USERHASH = preferences.getString("uhash", "");
  DEEPSLEEP = preferences.getString("dsleep", "");
  preferences.end();
}
///////////////////////////////////
// Sketch setup                  //
///////////////////////////////////
void setup() {
  //
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Mounting FS...");
  while (!SPIFFS.begin()) {
    SPIFFS.format();
    Serial.println("Failed to mount file system");
    delay(1000);
  }

  String WIFI_SSID = "";
  String WIFI_PWD = "";
  String GRID_USERHASH = "";

  // Load config
  LoadConfig(WIFI_SSID, WIFI_PWD, GRID_USERHASH, DEEP_SLEEP);

  //
  Serial.println("NODEID: " + CGridShell::GetInstance().GetNodeID());
  Serial.println("MEM: " + String(CGridShell::GetInstance().MEMGetFree()));
  Serial.println("SPIFFS: " + String(CGridShell::GetInstance().FSGetTotal()));
  Serial.println("SPIFFSU: " + String(CGridShell::GetInstance().FSGetUsed()));
  Serial.println("VERSION: " GNODE_VERSION);

  // Check if necessary creds given
  if (WIFI_SSID == "" || GRID_USERHASH == "") {
    Serial.println("Time to setup, hit ENTER to start.");

    // Wait for user readiness
    while (!Serial.available()) {
      // Wait for input
    }
    Serial.readString();

    // Wait for user to provide WiFi name
    Serial.println("Please enter WiFi name:");
    while (!Serial.available()) {
      // Wait for input
    }
    WIFI_SSID = Serial.readString();
    WIFI_SSID.trim();  // Remove leading/trailing spaces

    // Wait for user to provide WiFi password
    Serial.println("Please enter WiFi password:");
    while (!Serial.available()) {
      // Wait for input
    }
    WIFI_PWD = Serial.readString();
    WIFI_PWD.trim();  // Remove leading/trailing spaces

    // Wait for user to provide UUID
    Serial.println("Please enter UUID:");
    while (!Serial.available()) {
      // Wait for input
    }
    GRID_USERHASH = Serial.readString();
    GRID_USERHASH.trim();  // Remove leading/trailing spaces

    // V10 Ask for deep sleep if no work
    Serial.println("Go to sleep if no work(YES/NO):");
    #if defined(ESP8266)
      Serial.println("ESP8266: Ensure to solder GPIO16 with RST! ");
    #endif
    while (!Serial.available()) {
      // Wait for input
    }
    DEEP_SLEEP = Serial.readString();
    DEEP_SLEEP.trim();  // Remove leading/trailing spaces
  }

  // Save config
  SaveConfig(WIFI_SSID, WIFI_PWD, GRID_USERHASH, DEEP_SLEEP);

  //
  Serial.println("Connecting to WiFi " + WIFI_SSID);

  // Connect to WiFi
  WiFi.hostname("GSNODE" GNODE_VERSION);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PWD.c_str());

  // Wait for WIFI
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(1000);
  }

  Serial.println("Connected " + WiFi.localIP().toString());

  ///////////////////////////////////////////////////
  // Initialize GridShell Node with your user hash//
  ///////////////////////////////////////////////////
  if (CGridShell::GetInstance().Init(GRID_USERHASH) == true) {
  } else {
    Serial.println("The GridShell didn't init properly, rebooting");
    ESP.restart();
  }

  // Register callback
  CGridShell::GetInstance().RegisterEventCallback(GridShellCB);



  digitalWrite(LED_BUILTIN, LOW);
}
///////////////////////////////////
// Callback                      //
///////////////////////////////////
void GridShellCB(uint8_t uiEventType) {
  switch (uiEventType) {

    ///////////////////////////////////
    // Nothing to do                 //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_NO_TASKS_TO_EXECUTE:
      {
        if (DEEP_SLEEP == "YES") {
          GDEBUG("Taking a nap");
          #if defined(ESP8266)
                    // Remember ESP8266 needs GPIO16 and RST soldered
                    // To wake up from the DEEP SLEEP, otherwise
                    // It will not come back!
                    ESP.deepSleep(DEEP_SLEEP_TIME);
          #else
                    CGridShell::GetInstance().Stop();
                    esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME);
                    esp_deep_sleep_start();
          #endif 
        }
      }
      break;
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
// Sketch loop                   //
///////////////////////////////////
void loop() {
  /////////////////////////////
  // Keep working on the grid//
  /////////////////////////////
  CGridShell::GetInstance().Tick();

  // Check if WiFi available, if not just boot.
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WIFI Bad, rebooting");
    ESP.restart();
  }
}
