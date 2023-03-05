/*
   Example integration with the GRIDSHELL
   M5ATOM with LED and ULP Implementation
   The node will go sleep if no work is available
   The node will blink led to show the status.

   RED    - Wifi
   YELLOW - Connection to Grid
   GREEN  - Idling
   BLUE   - Working
   
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
///////////////////////////////////
// Power saving on idle          //
///////////////////////////////////
#define SLEEP_WHEN_IDLE
#ifdef SLEEP_WHEN_IDLE
#define SLEEP_IDLE_TIMEOUT 1 * 60000    // Idling time allowed
#define SLEEP_TIME 1 * (60 * 1000000)   // Sleep time
uint32_t uiIdleTime = 0;
#endif
//////////////////////////////////////////////
// LED Functionality Adafruit vs Builtin Led//
//////////////////////////////////////////////
//#define NOTIFY_LED_SIMPLE
#define NOTIFY_LED_ADA
#ifdef NOTIFY_LED_ADA
#include <Adafruit_NeoPixel.h>
#define ADA_PIN       27
#define ADA_NUMPIXELS 1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(ADA_NUMPIXELS, ADA_PIN, NEO_GRB + NEO_KHZ800);
#endif
#ifdef NOTIFY_LED_SIMPLE
#define LED_BUILTIN 2
#endif
///////////////////////////////////
// Goes to sleep                 //
///////////////////////////////////
void DeepSleep()
{

#ifdef NOTIFY_LED_ADA
  pixels.setPixelColor(0, 0, 0, 0);
  pixels.show();
#endif
#ifdef NOTIFY_LED_SIMPLE
  digitalWrite(LED_BUILTIN, LOW);
#endif
  esp_deep_sleep_start();
}
///////////////////////////////////
// Callback                      //
///////////////////////////////////
void GridShellCB(uint8_t  uiEventType)
{
  switch (uiEventType)
  {

    ///////////////////////////////////
    // The pong                      //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_PONG:
      {
      }
      break;

    ///////////////////////////////////
    // The grid is connected         //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_CONNECTED:
      {
#ifdef SLEEP_WHEN_IDLE
        uiIdleTime = 0;
#endif
      }
      break;

    ///////////////////////////////////
    // The grid is disconnected      //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_DISCONNECTED:
      {
#ifdef SLEEP_WHEN_IDLE
        uiIdleTime = 0;
#endif

#ifdef NOTIFY_LED_ADA
        pixels.setPixelColor(0, 255, 255, 0);
        pixels.show();
#endif
      }
      break;

    ///////////////////////////////////
    // Node is executing a task     //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_WORK:
      {
#ifdef SLEEP_WHEN_IDLE
        uiIdleTime = 0;
#endif

#ifdef NOTIFY_LED_ADA
        pixels.setPixelColor(0, 0, 0, 255);
        pixels.show();
#endif
#ifdef NOTIFY_LED_SIMPLE
        digitalWrite(LED_BUILTIN, HIGH);
#endif
      }
      break;
    ///////////////////////////////////
    // Node is idling               //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_IDLE:
      {
#ifdef SLEEP_WHEN_IDLE
        //
        if (uiIdleTime == 0)
          uiIdleTime = millis();
#endif

#ifdef NOTIFY_LED_ADA
        pixels.setPixelColor(0, 0, 255, 0);
        pixels.show();
#endif
#ifdef NOTIFY_LED_SIMPLE
        digitalWrite(LED_BUILTIN, LOW);
#endif
      }
      break;
    ///////////////////////////////////
    // No tasks to execute           //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_NO_TASKS_TO_EXECUTE:
      {
#ifdef SLEEP_WHEN_IDLE
        DeepSleep();
#endif
      }
      break;
    ///////////////////////////////////
    // No tasks to validate          //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_NO_TASKS_TO_VALIDATE:
      {
      }
      break;
    ///////////////////////////////////
    // Versions mismatch             //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_VERSIONS_MISMATCH:
      {
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

#ifdef NOTIFY_LED_SIMPLE
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
#endif
#ifdef NOTIFY_LED_ADA
  pixels.begin();
  pixels.setPixelColor(0, 255, 0, 0);
  pixels.setBrightness(255);
  pixels.show();
#endif

#ifdef SLEEP_WHEN_IDLE
  esp_sleep_enable_timer_wakeup(SLEEP_TIME);
#endif

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
  if (CGridShell::GetInstance().Init(GRID_USERHASH) == true)
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


#ifdef NOTIFY_LED_SIMPLE
  digitalWrite(LED_BUILTIN, LOW);
#endif
#ifdef NOTIFY_LED_ADA
  pixels.setPixelColor(0, 255, 255, 0);
  pixels.show();
#endif
}
///////////////////////////////////
// Sketch loop                   //
///////////////////////////////////
void loop()
{
  static uint32_t m_uiLastHB   = 0;

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

  // Your Normal Sketch things
  if (millis() - m_uiLastHB > 1000)
  {
    m_uiLastHB = millis();
  }

  /////////////////////////////
  // Deep sleep check        //
  /////////////////////////////
#ifdef SLEEP_WHEN_IDLE
  if (millis() - uiIdleTime >= SLEEP_IDLE_TIMEOUT)
  { 
    DeepSleep();
  }
#endif
  // This keeps handling the OTA functionality
#ifdef ENABLE_OTA
  ArduinoOTA.handle();
#endif
}
