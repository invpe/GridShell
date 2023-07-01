/*
   ESP32 GridShell Playground
   https://www.gridshell.net/
   https://github.com/invpe/gridshell
*/

#include <ArduinoOTA.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "SPIFFS.h"

#define GNODE_DEBUG 1
#include "CGridShell.h"
#include "my_basic.hpp"

// Some helpful definitions
#define RSSI_MAX -50
#define RSSI_MIN -100 
#define BUILTIN_LED 2

// You can manipulate here the exec timeout of a script
#define GMINER_EXEC_TIMEOUT 60000 * 2

// Set your WiFi Credentials
#define WIFI_SSID ""
#define WIFI_PWD  ""

// If you want to contribute to GridShell with the second thread
// Define your grid uid here
#define GRID_UID ""

#ifdef GRID_UID
TaskHandle_t Task1;

void TaskGRID(void *pvParameters)
{
  while (1)
  {
    CGridShell::GetInstance().Tick();
  }
}
#endif

// Enable OTA if needed
#define ENABLE_OTA 1

WiFiClient m_Client;
WebServer WWWServer(80);

//
uint32_t uiTimerWiFiReconnector = 0;
uint32_t m_uiScriptStart = 0;

int DBTOPERCENTAGE(int dBm)
{
  int quality;
  if (dBm <= RSSI_MIN)
  {
    quality = 0;
  }
  else if (dBm >= RSSI_MAX)
  {
    quality = 100;
  }
  else
  {
    quality = 2 * (dBm + 100);
  }

  return quality;
}
void Reboot()
{
  Serial.println("Reboot");
  ESP.restart();
}
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
  {
    sprintf(MAC_char, "%s%02x", MAC_char, marray[i]);
  }

  return String(MAC_char);
}
// CUSTOM FUNCS IMPLEMENTATION TESTING //
// CUSTOM FUNCS IMPLEMENTATION TESTING //
// CUSTOM FUNCS IMPLEMENTATION TESTING //
/* Get a char a string position */
static int _charat(mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  char* arg = 0;
  int_t start = 0;
  char sub[2];
  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_pop_string(s, l, &arg));
  mb_check(mb_pop_int(s, l, &start));
  mb_check(mb_attempt_close_bracket(s, l));

  if ( start < 0 || start >= (int_t)strlen(arg))
  {
    sub[0] = '\0';
  }
  else
  {
    sub[0] = arg[start];
    sub[1] = '\0';
  }
  mb_check(mb_push_string(s, l, mb_memdup(sub, 2)));
  return result;
}
int MBStep(struct mb_interpreter_t* s, void** l, const char* f, int p, unsigned short row, unsigned short col)
{
  // Avoid endless loops
  if (millis() - m_uiScriptStart > GMINER_EXEC_TIMEOUT)
  {
    Serial.println("Exec timeout");
    return GNODE_RET_TERMINATED;
  }
  // Breath
  yield();

  //
  return MB_FUNC_OK;
}
int StartScript(const String& rFilename, const String& strInputPayload, String& rstrOutput)
{
  //
  int iRetCode = MB_FUNC_ERR;
  String strOutput = "";

  void **l = NULL;
  struct mb_interpreter_t* bas = NULL;

  // Initialize MYBASIC
  mb_init();
  mb_open(&bas);

  // Additional functions registration
  mb_register_func(bas, "BIT_AND", _bit_and);
  mb_register_func(bas, "BIT_OR", _bit_or);
  mb_register_func(bas, "BIT_NOT", _bit_not);
  mb_register_func(bas, "BIT_XOR", _bit_xor);
  mb_register_func(bas, "BIT_LSHIFT", _bit_lshift);
  mb_register_func(bas, "BIT_RSHIFT", _bit_rshift);
  mb_register_func(bas, "READ", _read);
  mb_register_func(bas, "WRITE", _write);
  mb_register_func(bas, "SHA1", _sha1);
  mb_register_func(bas, "DOWNLOAD", _download);
  mb_register_func(bas, "TSIZE", _tsize);
  mb_register_func(bas, "CHARAT", _charat);


  // Enable step by step execution to keep alive with the server
  mb_debug_set_stepped_handler(bas, MBStep);

  // Load
  File configFile = SPIFFS.open(rFilename, "r");
  String strScript = configFile.readStringUntil('\0') ;
  configFile.close();

  // Load up the script
  if (mb_load_string(bas, strScript.c_str(), true) == MB_FUNC_OK)
  {
    // payload check and upload
    if (strInputPayload != "")
    {
      mb_value_t valAdd;
      valAdd.type = MB_DT_STRING;
      valAdd.value.string =  (char *)strInputPayload.c_str();
      mb_add_var(bas, l, "INPUTPAYLOAD", valAdd, true);
    }
    else rstrOutput = "Error, no input payload";

    m_uiScriptStart = millis();

    // Run
    iRetCode = mb_run(bas, true);

    // Obtain output
    mb_value_t valGet;
    int iRes = mb_get_value_by_name(bas, l, "OUTPUTPAYLOAD", &valGet);

    // Needs to be a string
    if (valGet.type == MB_DT_STRING)
      strOutput = String(valGet.value.string);
    else
      rstrOutput = "Outputpayload is wrong type";
  }
  else rstrOutput = "Can't execute, wooh!";

  mb_close(&bas);
  mb_dispose();
  rstrOutput = strOutput;
  return iRetCode;
}
void setup()
{
  //
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  Serial.println("Mounting FS...");
  while (!SPIFFS.begin())
  {
    SPIFFS.format();
    Serial.println("Failed to mount file system");
    delay(1000);
  }

  // Connect to WiFi, internet required ;-)
  Serial.println("Connecting to WiFi "WIFI_SSID);
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
  Serial.println("Connected");

  WWWServer.onNotFound([  ]()
  {
    String dataType;
    String path = WWWServer.uri();

    if (path.endsWith(".css")) {
      dataType = "text/css";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".js")) {
      dataType = "application/javascript";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".png")) {
      dataType = "image/png";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".gif")) {
      dataType = "image/gif";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".jpg")) {
      dataType = "image/jpeg";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".ico")) {
      dataType = "image/x-icon";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".xml")) {
      dataType = "text/xml";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".pdf")) {
      dataType = "application/pdf";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".zip")) {
      dataType = "application/zip";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".txt")) {
      dataType = "text/plain";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".htm")) {
      dataType = "text/html";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".css")) {
      dataType = "text/css";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".raw")) {
      dataType = "application/octet-stream";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
    else if (path.endsWith(".json")) {
      dataType = "application/json";
      File dataFile = SPIFFS.open(path.c_str(), "r");
      if (WWWServer.streamFile(dataFile, dataType) != dataFile.size()) {}
      dataFile.close();
    }
  });
  WWWServer.on("/playground", [&]() {
    uint32_t uiStart = millis();
    uint32_t uiEnd = millis();
    String strOutput = "Not run";
    int iRet = 0;

    WWWServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
    WWWServer.send(200, "text/html", "");
    String strScript = WWWServer.arg("script");
    String strInput  = WWWServer.arg("inputpayload");
    String strLoadScript  = WWWServer.arg("loadscript");
    String strSaveScript = WWWServer.arg("savescript");


    // Saving a file
    if (strSaveScript.endsWith(".bas"))
    {

      File configFile = SPIFFS.open("/" + strSaveScript, "w");
      configFile.println(strScript);
      configFile.close();

    }

    // Loading a file
    if (strLoadScript.endsWith(".bas"))
    {

      File configFile = SPIFFS.open("/" + strLoadScript, "r");
      strScript = configFile.readStringUntil('\0') ;
      configFile.close();
    }

    // Execution a script
    if (strScript.length() > 0 && strInput.length() > 0)
    {
      File configFile = SPIFFS.open("/last.bas", "w");
      configFile.println(strScript);
      configFile.close();

      strOutput = "";
      uiStart = millis();
      iRet = StartScript("/last.bas", strInput, strOutput);
      if (iRet == GNODE_RET_TERMINATED) strOutput = "Timed out";
      uiEnd = millis();
    }

    // Load up the scripts on the SPIFFS
    String strList = "";
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
      String strPath = file.name();

      Serial.println(strPath);

      if (strPath.endsWith(".bas"))
        strList += "<option value=\"" + strPath + "\">" + strPath + "</option>";

      file = root.openNextFile();
    }
    root.close();

    // Finally stream the page
    File dataFile = SPIFFS.open("/playground.htm", "r");
    while (dataFile.available())
    {
      String strData = dataFile.readString();
      strData.replace("$SCRIPT$", strScript);
      strData.replace("$MAC$", GetMACAddress(0));
      strData.replace("$EXECTIME$", String(uiEnd - uiStart) + "/" + String(GMINER_EXEC_TIMEOUT));
      strData.replace("$INPUTPAYLOAD$", strInput);
      strData.replace("$OUTPUTPAYLOAD$", strOutput);
      strData.replace("$SCRIPTS_LIST$", strList);
      strData.replace("$RETCODE$", String(iRet));
      if (iRet == 0)
      {
        strData.replace("<!---SUBMIT_TO_GRID", " ");
        strData.replace("SUBMIT_TO_GRID-->", " ");
        strData.replace("<!---RESTART", " ");
        strData.replace("RESTART-->", " ");

      }
      WWWServer.sendContent(strData);
      WWWServer.sendContent(F(""));
    }
    dataFile.close();
  });
  WWWServer.on("/", [&]() {
    WWWServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
    WWWServer.send(200, "text/html", "");
    File dataFile = SPIFFS.open("/index.htm", "r");
    while (dataFile.available())
    {

      String strData = dataFile.readString();

      strData.replace("$VERSION$", GNODE_VERSION);
      strData.replace("$MEM$", String(ESP.getFreeHeap()));
      strData.replace("$SSID$", WiFi.SSID());
      strData.replace("$IP$", WiFi.localIP().toString());
      strData.replace("$MAC$", GetMACAddress(0));

#ifdef GRID_UID
      strData.replace("$GRIDSHELL$",  (CGridShell::GetInstance().Connected()) ? "Online" : "Offline");
#else
      strData.replace("$GRIDSHELL$", "Not defined");
#endif
      strData.replace("$WIFISIGNAL$", String(DBTOPERCENTAGE(WiFi.RSSI())));
      WWWServer.sendContent(strData);
      WWWServer.sendContent(F(""));
    }
    dataFile.close();
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
#ifdef GRID_UID
  if (CGridShell::GetInstance().Init(GRID_UID, true) == true)
  {
    xTaskCreatePinnedToCore(TaskGRID, "Task1", 10000, (void*)1, 1, &Task1, 1);
    Serial.println("GridShell Initialized");
  }
  else
    Serial.println("GridShell failed to initialize");
#endif



  digitalWrite(BUILTIN_LED, LOW);
  Serial.println("Ready, serving on " + WiFi.localIP().toString());
}
void loop()
{
  WWWServer.handleClient();

  // Check if WiFi available, if not just boot.
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WIFI Bad, rebooting");
    ESP.restart();
  }

  // This keeps handling the OTA functionality
#ifdef ENABLE_OTA
  ArduinoOTA.handle();
#endif
}
