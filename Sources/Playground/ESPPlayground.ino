/*
   ESP32 GridShell Playground 
   https://github.com/invpe/gridshell
*/

#include <ArduinoOTA.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "SPIFFS.h"
#include "CGridShell.h"
#define BUILTIN_LED 27
#define WIFI_SSID ""
#define WIFI_PWD ""
#define UID ""
#define ENABLE_OTA 1

WebServer WWWServer(80);

void Reboot() {
  Serial.println("Reboot");
  ESP.restart();
}
void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  Serial.println("Mounting FS...");
  while (!SPIFFS.begin()) {
    SPIFFS.format();
    Serial.println("Failed to mount file system");
    delay(1000);
  }

  Serial.println("Connecting to WiFi " WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(1000);
  }
  Serial.println("Connected");

  WWWServer.onNotFound([]() {
    String dataType;
    String path = WWWServer.uri();

    if (path.endsWith(".css")) {
      dataType = "text/css";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".js")) {
      dataType = "application/javascript";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".png")) {
      dataType = "image/png";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".gif")) {
      dataType = "image/gif";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".jpg")) {
      dataType = "image/jpeg";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".ico")) {
      dataType = "image/x-icon";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".xml")) {
      dataType = "text/xml";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".pdf")) {
      dataType = "application/pdf";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".zip")) {
      dataType = "application/zip";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".txt")) {
      dataType = "text/plain";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".htm")) {
      dataType = "text/html";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".css")) {
      dataType = "text/css";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".raw")) {
      dataType = "application/octet-stream";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    } else if (path.endsWith(".json")) {
      dataType = "application/json";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
  });
  WWWServer.on("/run", [&]() {
    WWWServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
    WWWServer.send(200, "text/plain", "");
    String strScript = WWWServer.arg("script");
    String strInput = WWWServer.arg("inputpayload");

    String strOutputText = " ------------------------------\n";
    strOutputText += String(millis()) + ":" + String(ESP.getFreeHeap()) + " Starting\n";
    strOutputText += String(millis()) + ":" + String(ESP.getFreeHeap()) + " Inputpayload: " + strInput + "\n";

    if (strScript.isEmpty())
      strOutputText += String(millis()) + ":" + String(ESP.getFreeHeap()) + " Warrning: Script is empty!\n";

    if (strInput.isEmpty())
      strOutputText += String(millis()) + ":" + String(ESP.getFreeHeap()) + " Warrning: Inputpayload is empty!\n";

    strOutputText += String(millis()) + ":" + String(ESP.getFreeHeap()) + " Saving script\n";
    File fScriptFile = SPIFFS.open("/last.bas", "w");
    fScriptFile.println(strScript);
    fScriptFile.close();

    strOutputText += String(millis()) + ":" + String(ESP.getFreeHeap()) + " Starting script\n";
    auto aResults = CGridShell::GetInstance().Run(strScript, strInput, 120000);
    int iRetCode = std::get<0>(aResults);
    String strOutput = std::get<1>(aResults);

    strOutputText += String(millis()) + ":" + String(ESP.getFreeHeap()) + " Script ended after " + String(millis() - CGridShell::GetInstance().GetTaskStartTime()) + "ms\n";
    strOutputText += String(millis()) + ":" + String(ESP.getFreeHeap()) + " Return code: " + String(iRetCode) + "\n";
    strOutputText += String(millis()) + ":" + String(ESP.getFreeHeap()) + " Outputpayload: " + strOutput + "\n";
    strOutputText += String(millis()) + ":" + String(ESP.getFreeHeap()) + " End\n";
    strOutputText += " ------------------------------\n";

    WWWServer.sendContent(strOutputText);
    WWWServer.sendContent(F(""));
  });
  WWWServer.on("/", [&]() {
    WWWServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
    WWWServer.send(200, "text/html", "");
    WWWServer.sendContent("<title>GridShell::ESP32 Playground</title>\n");
    WWWServer.sendContent("<center>\n");
    WWWServer.sendContent("<form action=run method=get>");
    WWWServer.sendContent("<textarea name=script rows=40 cols=100>");
    WWWServer.sendContent("'This is your GridShell MYBASIC Playground\n");
    WWWServer.sendContent("'Learn and have fun writing tasks for GridShell network\n");
    WWWServer.sendContent("'Feel free to suggest changes or even modify this code for your needs!\n");
    WWWServer.sendContent("\n");
    WWWServer.sendContent("\n");
    WWWServer.sendContent("PRINT INPUTPAYLOAD;\n");
    WWWServer.sendContent("OUTPUTPAYLOAD = \"OK \"+INPUTPAYLOAD\n");
    WWWServer.sendContent("</textarea><BR>");
    WWWServer.sendContent("<input type=text value=\"INPUT,PAYLOAD,\" name=\"inputpayload\"></input><BR>");
    WWWServer.sendContent("<input type=submit value=Run>");
    WWWServer.sendContent("</form>");
    WWWServer.sendContent("<hr>");
    WWWServer.sendContent("<a href=https://github.com/invpe/GridShell>GridShell</a>\n");
    WWWServer.sendContent(F(""));
  });

  WWWServer.begin();


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
      Serial.printf("Progress: %u%%\r\n", (progress / (total / 100)));
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
  ArduinoOTA.setHostname("GS_PLAYGROUND");
  ArduinoOTA.begin();
#endif

  CGridShell::GetInstance().Init(UID, false);
  digitalWrite(BUILTIN_LED, LOW);
  Serial.println("Ready, serving on " + WiFi.localIP().toString());
}
void loop() {

  WWWServer.handleClient();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WIFI Bad, rebooting");
    ESP.restart();
  }

  CGridShell::GetInstance().Tick();
  
#ifdef ENABLE_OTA
  ArduinoOTA.handle();
#endif
}
