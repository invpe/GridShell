// Example sketch integrating BME280 and storing a simple telemetry with the readouts every 10 minutes.
// https://www.instructables.com/GridShell-Your-Fist-Sensor-on-the-Network/
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include "CGridShell.h"

#define PIN_BMP_SDA 21
#define PIN_BMP_SCL 22
#define TELEMETRY_MINUTES 60000ULL * 10
// !!!! UPDATE WITH YOUR CREDENTIALS HERE !!!! ////
// !!!! UPDATE WITH YOUR CREDENTIALS HERE !!!! ////
#define WIFI_ACCESSPOINT "xxxx"
#define WIFI_PASSWORD "yyyy"
#define GRID_U "zzzz"

Adafruit_BME280 m_BMESensor;
uint64_t m_uiLastSend = 0;

void setup() {
  Serial.begin(115200);
  bool status;

  while (!SPIFFS.begin()) {
    Serial.println("Formatting");
    SPIFFS.format();
    delay(1000);
  }

  status = m_BMESensor.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1) {}
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_ACCESSPOINT, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(1000);
  }

  CGridShell::GetInstance().Init(GRID_U, false);
}

void loop() {

  CGridShell::GetInstance().Tick();

  if (millis() - m_uiLastSend >= TELEMETRY_MINUTES) {
    if (CGridShell::GetInstance().Connected()) {
      Serial.println("Temperature = " + String(m_BMESensor.readTemperature()) + " C");
      Serial.println("Humidity = " + String(m_BMESensor.readHumidity()) + " %");

      String strFilename = "BME280";
      String strAppend = "0";
      String strWriteText = "{\"Temperature\": " + String(m_BMESensor.readTemperature()) + ",\n";
      strWriteText += "\"Humidity\": " + String(m_BMESensor.readHumidity()) + "\n";
      strWriteText += "}";

      String strTaskPayload = strFilename + "," + strAppend + "," + CGridShell::GetInstance().EncodeBase64(strWriteText) + ",";
      uint32_t uiTaskId = CGridShell::GetInstance().AddTask("writedfs", strTaskPayload);

      Serial.println("Task ID: " + String(uiTaskId));
    }
    m_uiLastSend = millis();
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WIFI Bad, rebooting");
    ESP.restart();
  }
}
