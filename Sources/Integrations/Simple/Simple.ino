/*
   Simple sketch to get you started with using GridShell
   (c) GridShell.net invpe https://github.com/invpe/GridShell/
*/
#include <WiFi.h>
#include <NTPClient.h>
#include <ctime>
#include "SPIFFS.h"
/*------------------*/
#include "CGridShell.h"
/*------------------*/
#define GRID_U  ""      // << Provide your Grid UserHash
#define WIFI_A  ""      // << Provide your WiFi Access Point Name
#define WIFI_P  ""      // << Provide your WiFi Access Point Password
#define TIME_OFFSET 7200      // Time offset
/*------------------*/
uint64_t uiOneMinute;
WiFiUDP ntpUDP; // << We will use Network Time Protocol to obtain the date / time from ntp server.
NTPClient timeClient(ntpUDP, "pool.ntp.org"); // << The NTP server hostname
/*------------------*/
uint8_t bAppend = 1;               // << File append flag (true = yes, false = no)
String strFileName = "MYTELEMETRY"; // << Filename
String strTextToWrite = "";
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
  if (CGridShell::GetInstance().Init(GRID_U, false) == true)
  {
  }
  else
  {
    // If it fails, reboot the device
    Serial.println("GridShell init failed, double check settings");
    ESP.restart();
  }

  // Set time offset
  timeClient.setTimeOffset(TIME_OFFSET);
  timeClient.begin();
  timeClient.update();

  // We're done with the setup, lets enter the main loop

}
/*------------------*/
void loop()
{

  // Keep ticking the GridShell
  CGridShell::GetInstance().Tick();
  
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  // Every minute, write comma separated values to telemetry file
  // Do not append (bAppend flag = false)
  if (millis() - uiOneMinute >= 60000)
  {
    // Lets generate some random values (epoch,random,random,random)
    strTextToWrite = String(timeClient.getEpochTime()) + ","; // << First column is time
    strTextToWrite += String(rand() % 20) + ",";
    strTextToWrite += String(rand() % 15) + ",";
    strTextToWrite += String(rand() % 13) + "\n";


    // Payload for the WRITE script (filename, append flag)
    String strFilePayload = "";
    strFilePayload = strFileName + ",";       // The filename you want to write
    strFilePayload += String(bAppend) + ","; // The Append flag (1 - yes, 0 - no) 

    // Submit a task to the GRID that will execute writing to file
    String strFullPayload = strFilePayload+CGridShell::GetInstance().EncodeBase64(strTextToWrite) + ",";
    uint32_t uiTaskID = CGridShell::GetInstance().AddTask("writedfs", strFullPayload);

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
