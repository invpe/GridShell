/*
   Simple sketch to get you started with using GridShell
   (c) GridShell.net invpe https://github.com/invpe/GridShell/
*/
#include <WiFi.h>
#include "SPIFFS.h"
/*------------------*/
#include "CGridShell.h"
/*------------------*/
#define GRID_U  ""      // << Provide your Grid UserHash
#define WIFI_A  ""      // << Provide your WiFi Access Point Name
#define WIFI_P  ""      // << Provide your WiFi Access Point Password
/*------------------*/
uint64_t uiOneMinute;
/*------------------*/
uint8_t bAppend = 0;               // << File append flag (true = yes, false = no)
String strFileName = "MYTELEMETRY"; // << Filename
String strTextToWrite = "1,3,5,6,777,\n"; // << Text to write
/*------------------*/
void setup()
{
  // Begin by opening a serial communication
  Serial.begin(115200);

  // Mount the FLASH filesystem
  Serial.println("Mounting FS...");
  while (!SPIFFS.begin())
  {
    SPIFFS.format();
    Serial.println("Failed to mount file system");
    delay(1000);
  }


  // Connect to WiFi
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

  // Once connected, print out our IP address on serial monitor
  Serial.println("Connected " + WiFi.localIP().toString());

  ///////////////////////////////////////////////////
  // Initialize GridShell Node with your user hash//
  ///////////////////////////////////////////////////
  if (CGridShell::GetInstance().Init(GRID_U, true) == true)
  {
  }
  else
  {
    // If it fails, reboot the device
    Serial.println("GridShell init failed, double check settings");
    ESP.restart();
  }

  // We're done with the setup, lets enter the main loop

}

/*------------------*/
void loop()
{

  // Keep ticking the GridShell
  CGridShell::GetInstance().Tick();


  // Every minute, write comma separated values to telemetry file
  // Do not append (bAppend flag = false)
  if (millis() - uiOneMinute >= 60000)
  {

    // Payload for the WRITE script
    String strFilePayload = "";
    strFilePayload = strFileName+",";         // The filename you want to write
    strFilePayload += String(bAppend) + ","; // The Append flag (1 - yes, 0 - no)
    strFilePayload += strTextToWrite;     // The message you want to write to the file
    strFilePayload += ",";                // Mandatory payload closure, leave it
 
    // Submit a task to the GRID that will execute writing to file
    uint32_t uiTaskID = CGridShell::GetInstance().AddTask("write", strFilePayload);

    Serial.println("WRITE Task ID: " + String(uiTaskID));
    Serial.println("Grid Status  : " + String(CGridShell::GetInstance().Connected()));

    // Update the minute timer
    uiOneMinute = millis();
  }

  // Check if WiFi available, if not just reboot.
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WIFI Bad, rebooting");
    ESP.restart();
  }
}
