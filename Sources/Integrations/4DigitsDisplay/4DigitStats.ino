#include <TM1637.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "SPIFFS.h"
#include "CGridShell.h"
#define LED_BUILTIN 27
#define TELEMETRY_MINUTES 60000ULL * 10
#define WIFI_ACCESSPOINT "xxxxxxxxxxxx"
#define WIFI_PASSWORD "yyyyyyyyyyyyy"
#define GRID_U "zzzzzzzzzzzzzzz"
uint64_t m_uiLastSend = 0;


int CLK = 32;
int DIO = 26;

TM1637 tm(CLK, DIO);
void displayNumber(int num) {
  tm.display(3, num % 10);
  tm.display(2, num / 10 % 10);
  tm.display(1, num / 100 % 10);
  tm.display(0, num / 1000 % 10);
}
void GridShellCB(uint8_t uiEventType) {
  switch (uiEventType) {
    ///////////////////////////////////
    // Node is executing a task     //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_WORK:
      {
        tm.set(7);
        digitalWrite(LED_BUILTIN, HIGH);
      }
      break;
    ///////////////////////////////////
    // Node is idling               //
    ///////////////////////////////////
    case CGridShell::eEvent::EVENT_IDLE:
      {
        tm.set(0);
        digitalWrite(LED_BUILTIN, LOW);
      }
      break;
  }
}
void setup() {

  Serial.begin(115200);
  tm.init();
  tm.set(0);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  displayNumber(0);
  while (!SPIFFS.begin()) {
    Serial.println("Formatting");
    SPIFFS.format();
    delay(1000);
  }


  displayNumber(1);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_ACCESSPOINT, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(1000);
  }

  displayNumber(2);
  CGridShell::GetInstance().Init(GRID_U, false);
  CGridShell::GetInstance().RegisterEventCallback(GridShellCB);
  digitalWrite(LED_BUILTIN, LOW);

  tm.clearDisplay();
}

void loop() {

  CGridShell::GetInstance().Tick();

  if (millis() - m_uiLastSend >= TELEMETRY_MINUTES) {

    String strURL = "https://api.gridshell.net/current.json";
    HTTPClient httpClient;
    httpClient.begin(strURL);
    int httpCode = httpClient.GET();
    String payload = httpClient.getString();
    DynamicJsonDocument jsonBuffer(1024);
    deserializeJson(jsonBuffer, payload);
    String strConnections = jsonBuffer["Connections"].as<String>();
    String strUsers = jsonBuffer["UsersCount"].as<String>();

    displayNumber(strUsers.toInt());

    if (CGridShell::GetInstance().Connected()) {
    }
    m_uiLastSend = millis();
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WIFI Bad, rebooting");
    ESP.restart();
  }
}
