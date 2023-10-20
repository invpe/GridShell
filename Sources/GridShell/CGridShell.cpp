#include "CGridShell.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CGridShell::CGridShell() {
  m_bAutoUpdate = true;
  m_strUsername = "";
  m_uiLastHB = 0;
  m_uiLastReconnection = millis() + (10 * GNODE_RECON_TIMER);
  m_pCallback = NULL;

  // Get the unique MAC address from eFuse
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);

  m_strUniqueID = "";
  for (int i = 0; i < 6; i++) m_strUniqueID += String(mac[i], HEX);

  // Generate hash & truncate to 12"
  m_strUniqueID = sha1HW(m_strUniqueID);
  m_strUniqueID = m_strUniqueID.substring(0, 12);

  m_HttpClient.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  m_HttpClient.setTimeout(10000);
  m_HttpClient.setReuse(false);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// -----------------------------------------------------------------------------
String CGridShell::GetNodeID() const {
  return m_strUniqueID;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// -----------------------------------------------------------------------------
CGridShell& CGridShell::GetInstance() {
  static CGridShell sInstance;
  return sInstance;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Helper
//
// -----------------------------------------------------------------------------
void CGridShell::Stop() {
  m_Client.stop();
  if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_DISCONNECTED);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Download CA.CRT
//
// -----------------------------------------------------------------------------
String CGridShell::GetCertificate() {
  GDEBUG("Downloading CA certificate...");
  String strCert = "";
  if (m_HttpClient.begin(GNODE_CACERT_URL)) {
    int httpCode = m_HttpClient.GET();
    if (httpCode == HTTP_CODE_OK) {
      GDEBUG("CA certificate downloaded successfully.");
      strCert = m_HttpClient.getString();
    } else
      GDEBUG("Failed to download CA certificate.");

  } else GDEBUG("HTTP client failed to begin.");


  m_HttpClient.end();
  return strCert;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Set things up internally
//
// -----------------------------------------------------------------------------
bool CGridShell::Init(const String& strUsername, const bool& bAutoUpdate) {
  GDEBUG("Start");

  m_bAutoUpdate = bAutoUpdate;
  m_strUsername = strUsername;

  // Validate username length
  if (m_strUsername.isEmpty()) {
    GDEBUG("Username is wrong");
    return false;
  }

  // Remove telemetry chunks
  CleanFS();

  GDEBUG("Init OK");

  //
  return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Perform reboot
//
// -----------------------------------------------------------------------------
void CGridShell::Reboot() {
  Stop();
  ESP.restart();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Remove telemetry chunks prefixed with GS
//
// -----------------------------------------------------------------------------
void CGridShell::CleanFS() {
  GDEBUG("Format");
  SPIFFS.format();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Keep alive with server
//
// -----------------------------------------------------------------------------
void CGridShell::Pong() {
  // Keep Alive
  if (millis() - m_uiLastHB >= GNODE_PING_TIME) {
    if (m_Client.connected()) {
      Send("PONG\r\n");
      if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_PONG);
    }
    m_uiLastHB = millis();
  }
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Returns connection status
//
// -----------------------------------------------------------------------------
bool CGridShell::Connected() {
  return m_Client.connected();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Heart of the execution
//
// -----------------------------------------------------------------------------
void CGridShell::Tick() {

  // Are we up?
  if (m_Client.connected() == false) {
    // Check if user set and reconnection timer is expired
    if (millis() - m_uiLastReconnection >= GNODE_RECON_TIMER) {
      //
      m_uiLastReconnection = millis();

      //
      Stop();

      // Remove telemetry chunks, prepare space for new
      CleanFS();

      // Get latest CA crt from github
      String strCert = GetCertificate();
      m_Client.setCACert(strCert.c_str());

      GDEBUG("Connecting");

      // Connect
      if (m_Client.connect(GNODE_SERVER, GNODE_POOL_PORT)) {

        if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_CONNECTED);

        // Ident and provide payload if any, this is base64 encoded already by ::Init
        String strVersion = m_Client.readStringUntil(',');
        String strWelcome = m_Client.readStringUntil(',');
        String strTasksToExecute = m_Client.readStringUntil(',');
        String strTasksToValidate = m_Client.readStringUntil('\n');

        // Safety check
        if (strVersion.isEmpty() || strWelcome.isEmpty() || strTasksToExecute.isEmpty() || strTasksToValidate.isEmpty()) {
          Stop();
          GDEBUG("Throttled");
          return;
        }

        // Confirm versions
        if (strVersion != GNODE_VERSION) {
          if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_VERSIONS_MISMATCH);

          GDEBUG("VMismatch " + strVersion + " != " GNODE_VERSION);

          Stop();

          // Guard users utilizing library in their sketches from
          // wiping out their firmware
          if (m_bAutoUpdate)
            OTA();

          Reboot();
          return;
        }

        String strBase64EncodedGUID = EncodeBase64(m_strUsername);

        Send("JOB," + strBase64EncodedGUID + "," + GNODE_VERSION + "," + m_strUniqueID + "\r\n");

        GDEBUG("Ident");

        // Nothing to execute
        if (strTasksToExecute.toInt() == 0)
          if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_NO_TASKS_TO_EXECUTE);

        // Nothing to validate
        if (strTasksToValidate.toInt() == 0)
          if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_NO_TASKS_TO_VALIDATE);

        //
        if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_IDLE);
      }
    }
  } else {
    // Wait for server to post a message
    if (m_Client.available()) {

      //
      String strJobType = m_Client.readStringUntil(',');

      //
      GDEBUG("Server: " + strJobType + " MEM: " + String(ESP.getFreeHeap()) + " FS: " + String(SPIFFS.usedBytes()));

      // Task coming
      if (strJobType == "EXEC") {
        if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_WORK);

        //
        int iRetCode = MB_FUNC_ERR;
        String strOutput = "";

        //
        String strScriptName = DecodeBase64(m_Client.readStringUntil(','));
        String strPayload = DecodeBase64(m_Client.readStringUntil(','));
        String strTimeout = m_Client.readStringUntil(',');
        String strTaskHash = m_Client.readStringUntil(',');

        String strURLPath = GNODE_TASK_SERVER_NAME + strScriptName + ".bas";
        String strFSPath = "/" + strScriptName + ".bas";

        // Hash does not match, download the script
        if (strTaskHash != GetSHA1(strFSPath)) {
          SPIFFS.remove(strFSPath);
          GDEBUG("Diff Hash");
          if (StreamFile(strURLPath, strFSPath) == true) {
          } else GDEBUG("Download failed");
        }

        if (strPayload.isEmpty()) {
          GDEBUG("Empty payload");
        }

        uint32_t uiStart = millis();
        auto aResults = Run(strFSPath, strPayload, strTimeout.toInt());
        iRetCode = std::get<0>(aResults);
        strOutput = std::get<1>(aResults);
        GDEBUG("Done " + String(millis() - uiStart) + " ms, RESCODE: " + String(iRetCode) + ", MEM: " + String(ESP.getFreeHeap()) + " OUTP: '" + strOutput + "' FS: " + String(SPIFFS.usedBytes()));

        if (strOutput.length() > 0) {
          size_t stLen = 0;
          mbedtls_base64_encode(NULL, 0, &stLen, (unsigned char*)strOutput.c_str(), strOutput.length());
          unsigned char* encodedData = new unsigned char[stLen + 1];
          mbedtls_base64_encode(encodedData, stLen, &stLen, (unsigned char*)strOutput.c_str(), strOutput.length());
          encodedData[stLen] = '\0';
          strOutput = "";

          // Send
          Send("RESULTS,");
          Send(String(iRetCode));
          Send(",");
          m_Client.write(encodedData, stLen);
          Send("\r\n");

          delete[] encodedData;
        } else
          Send("RESULTS," + String(iRetCode) + ",\r\n");

        if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_IDLE);
      }
    }
  }

  // Keep alive
  Pong();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Executes the BAS file stored on SPIFFS and returns ret code + outputpayload
//
// -----------------------------------------------------------------------------
std::tuple<int, String> CGridShell::Run(const String& rstrBASFile, const String& rstrInputPayload, const uint32_t& ruiTaskTimeout) {

  GDEBUG("Exe " + rstrBASFile + " Tout: " + String(ruiTaskTimeout) + "ms Heap: " + String(ESP.getFreeHeap()));
  String strOutputPayload;
  int iRetCode = MB_FUNC_ERR;

  File fScriptFile = SPIFFS.open(rstrBASFile, "r");
  if (fScriptFile) {
    String strScript = fScriptFile.readStringUntil('\0');
    fScriptFile.close();

    // Update task start timer
    m_uiTaskStart = millis();
    m_uiTaskTimeout = ruiTaskTimeout;
    void** l = NULL;
    struct mb_interpreter_t* bas = NULL;

    // Initialize MYBASIC
    mb_init();
    mb_open(&bas);

    // Additional functions
    mb_register_func(bas, "READ", _read);
    mb_register_func(bas, "WRITE", _write);
    mb_register_func(bas, "SHA1", _sha1);
    mb_register_func(bas, "SHA256", _sha256);
    mb_register_func(bas, "DOWNLOAD", _download);
    mb_register_func(bas, "TSIZE", _tsize);
    mb_register_func(bas, "FMD5", _fmd5);
    mb_register_func(bas, "B64D", _b64d);
    mb_register_func(bas, "B64E", _b64e);
    mb_register_func(bas, "XOR", _xor);
    mb_register_func(bas, "DEL", _del);

    // Enable step by step execution to keep alive with the server
    //mb_debug_set_stepped_handler(bas, CGridShell::MBStep);
    mb_debug_set_stepped_handler(bas, CGridShell::MBStep, NULL);

    // Load up the script
    if (mb_load_string(bas, strScript.c_str(), true) == MB_FUNC_OK) {

      if (rstrInputPayload != "") {
        mb_value_t valAdd;
        valAdd.type = MB_DT_STRING;
        valAdd.value.string = (char*)rstrInputPayload.c_str();
        mb_add_var(bas, l, "INPUTPAYLOAD", valAdd, true);
      }
      strScript.clear();

      GDEBUG("Exe Loaded " + rstrBASFile + " Tout: " + String(ruiTaskTimeout) + "ms Heap: " + String(ESP.getFreeHeap()));

      iRetCode = mb_run(bas, true);

      mb_value_t valGet;
      mb_get_value_by_name(bas, l, "OUTPUTPAYLOAD", &valGet);

      if (valGet.type == MB_DT_STRING)
        strOutputPayload = String(valGet.value.string);
    }
    mb_close(&bas);
    mb_dispose();
  }

  return std::make_tuple(iRetCode, strOutputPayload);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Register your callback function here
//
// -----------------------------------------------------------------------------
void CGridShell::RegisterEventCallback(void (*pFunc)(uint8_t)) {
  m_pCallback = pFunc;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Sends out String to server
//
// -----------------------------------------------------------------------------
void CGridShell::Send(const String& strData) {
  m_Client.print(strData);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Encodes Base64 to String
//
// ---------------------------------------------------------------------------
String CGridShell::EncodeBase64(const String& strString) {
  if (strString.length() > 0) {
    size_t stLen = 0;
    mbedtls_base64_encode(NULL, 0, &stLen, (unsigned char*)strString.c_str(), strString.length());
    unsigned char ucEncoded[stLen];
    mbedtls_base64_encode(ucEncoded, stLen, &stLen, (unsigned char*)strString.c_str(), strString.length());
    return String((char*)ucEncoded);
  }
  return "";
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Decodes Base64 to String
//
// ---------------------------------------------------------------------------
String CGridShell::DecodeBase64(const String& strString) {
  if (strString.length() > 0) {
    size_t stLen = 0;
    mbedtls_base64_decode(NULL, 0, &stLen, (unsigned char*)strString.c_str(), strString.length());
    unsigned char ucDecoded[stLen];
    mbedtls_base64_decode(ucDecoded, stLen, &stLen, (unsigned char*)strString.c_str(), strString.length());
    ucDecoded[stLen] = '\0';
    return String((char*)ucDecoded);
  }
  return "";
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Return task timeout in milliseconds for MBStep
//
// ---------------------------------------------------------------------------
uint32_t CGridShell::GetTaskTimeout() {
  return m_uiTaskTimeout;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Return task start time for MBStep
//
// ---------------------------------------------------------------------------
uint32_t CGridShell::GetTaskStartTime() {
  return m_uiTaskStart;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Handling BAS execution and keeping alive with the server, also checks for task endless loop
//
// ---------------------------------------------------------------------------
int CGridShell::MBStep(struct mb_interpreter_t* s, void** l, const char* f, int p, unsigned short row, unsigned short col) {

  // GDEBUG("BAS: "+String(row)+"/"+String(col)+" MEM: "+String(ESP.getFreeHeap()));

  // Avoid endless loops
  if (millis() - CGridShell::GetInstance().GetTaskStartTime() > CGridShell::GetInstance().GetTaskTimeout())
    return GNODE_RET_TERMINATED;

  CGridShell::GetInstance().Pong();

  //
  return MB_FUNC_OK;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : HW SHA1
//
// -----------------------------------------------------------------------------
String CGridShell::sha1HW(String payload) {
  return sha1HW((unsigned char*)payload.c_str(), payload.length());
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : HW SHA1
//
// -----------------------------------------------------------------------------
String CGridShell::sha1HW(unsigned char* payload, int len) {
  //
  int size = 20;
  byte shaResult[size];

  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA1;

  const size_t payloadLength = len;

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char*)payload, payloadLength);
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);

  String hashStr = "";

  for (uint16_t i = 0; i < size; i++) {
    String hex = String(shaResult[i], HEX);
    if (hex.length() < 2) {
      hex = "0" + hex;
    }
    hashStr += hex;
  }

  return hashStr;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : XOR
//
// -----------------------------------------------------------------------------
String CGridShell::XOR(const String& toEncrypt, const String& rstrKey) {
  String output = toEncrypt;

  for (int i = 0; i < toEncrypt.length(); i++)
    output[i] = toEncrypt[i] ^ rstrKey[i % (rstrKey.length() / 1)];

  return output;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Exposed for scripts to use the HTTPClient object
//
// -----------------------------------------------------------------------------
HTTPClient* CGridShell::GetHTTPClient() {
  return &m_HttpClient;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Obtains the source of the task to execute from API server over HTTPS
//
// -----------------------------------------------------------------------------
bool CGridShell::StreamFile(const String& rstrURL, const String& rstrPath) {

  m_HttpClient.begin(rstrURL);
  int httpCode = m_HttpClient.GET();
  bool bSuccess = false;
  if (httpCode == HTTP_CODE_OK) {
    File file = SPIFFS.open(rstrPath, FILE_WRITE);
    if (!file) {
      GDEBUG("Cant write " + rstrPath);
    } else {
      WiFiClient* stream = m_HttpClient.getStreamPtr();
      uint8_t buffer[2048] = { 0 };
      int bytesRead = 0;

      while (m_HttpClient.connected() && (bytesRead = stream->readBytes(buffer, sizeof(buffer))) > 0) {
        file.write(buffer, bytesRead);
      }

      file.close();
      GDEBUG(rstrPath + " Saved");
      bSuccess = true;
    }
  }
  m_HttpClient.end();
  GDEBUG("HTTPS GET Code: " + String(httpCode));
  return bSuccess;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Return MD5 of the given file
//
// -----------------------------------------------------------------------------
String CGridShell::GetMD5(const String& rstrFile) {
  File fFile = SPIFFS.open(rstrFile, "r");
  if (!fFile) return String();

  if (fFile.seek(0, SeekSet)) {
    m_MD5.begin();
    m_MD5.addStream(fFile, fFile.size());
    m_MD5.calculate();
    return m_MD5.toString();
  }

  return String();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Return SHA1 of the given file
//
// -----------------------------------------------------------------------------
String CGridShell::GetSHA1(const String& rstrFile) {
  File fScriptFile = SPIFFS.open(rstrFile, "r");
  if (!fScriptFile)
    return "";
  String strScript = fScriptFile.readString();
  fScriptFile.close();

  return sha1HW(strScript);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Delete a chunk of the telemetry
//
// -----------------------------------------------------------------------------
void CGridShell::Delete(const String& rstrName) {
  SPIFFS.remove(rstrName);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Write a chunk of the telemetry to the local storage
//
// -----------------------------------------------------------------------------
bool CGridShell::Write(const String& rstrName, const String& rstrWhat, const bool& bAppend) {

  File fTelemetry;

  if (bAppend)
    fTelemetry = SPIFFS.open(rstrName, "a");
  else
    fTelemetry = SPIFFS.open(rstrName, "w");

  if (!fTelemetry) {
    return false;
  }

  fTelemetry.print(rstrWhat);
  fTelemetry.close();

  return true;
}

// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : HW SHA256
//
// -----------------------------------------------------------------------------
String CGridShell::sha256HW(String payload) {
  return sha256HW((unsigned char*)payload.c_str(), payload.length());
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : HW SHA256
//
// -----------------------------------------------------------------------------
String CGridShell::sha256HW(unsigned char* payload, int len) {
  byte shaResult[32];  // SHA-256 produces a 32-byte hash

  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

  const size_t payloadLength = len;

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, payload, payloadLength);
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);

  String hashStr = "";

  for (uint16_t i = 0; i < 32; i++) {  // SHA-256 produces a 32-byte hash
    String hex = String(shaResult[i], HEX);
    if (hex.length() < 2) {
      hex = "0" + hex;
    }
    hashStr += hex;
  }

  return hashStr;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : OTA
//
// -----------------------------------------------------------------------------
void CGridShell::OTA() {
  GDEBUG("OTA");

  // Since we have a clean FS, we download the binary and upload from there
  if (StreamFile(GNODE_FIRMWARE_URL, "/GSFirmware") == true) {
    GDEBUG("Downloaded");

    // Open the firmware binary file from SPIFFS
    File firmwareFile = SPIFFS.open("/GSFirmware", "r");
    if (!firmwareFile) {
      GDEBUG("Cant open FMW");
      return;
    }
    size_t fileSize = firmwareFile.size();
    GDEBUG("FMW size: " + String(fileSize));
    Update.begin(fileSize);
    Update.writeStream(firmwareFile);
    firmwareFile.close();

    // Finish the firmware update process
    if (Update.end()) {
      GDEBUG("OTA Done");
    } else {
      GDEBUG(String(Update.getError()));
    }
  } else GDEBUG("Download failed");
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Returns new task ID or 0 if failed
//
// -----------------------------------------------------------------------------
uint32_t CGridShell::AddTask(const String& rstrScript, const String& rstrInputPayload) {
  if (!Connected()) return 0;

  String strInputBase = EncodeBase64(rstrInputPayload);
  String strCommand = "ADDT," + rstrScript + "," + strInputBase + "\r\n";

  Send(strCommand);

  String strReturn = m_Client.readStringUntil('\n');

  GDEBUG("AddTask : " + strReturn);

  // Return the task ID or 0 if failed
  // Split the server response by comma
  String taskType = strReturn.substring(0, strReturn.indexOf(","));
  String taskValue = strReturn.substring(strReturn.indexOf(",") + 1);

  if (taskType == "ADDT" && taskValue != "BAD") {
    return taskValue.toInt();
  }

  return 0;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : SEND tokens
//
// -----------------------------------------------------------------------------
bool CGridShell::Send(const String& rstrReceipent, const uint32_t& ruiValue) {

  if (rstrReceipent.isEmpty())
    return false;

  if (ruiValue == 0)
    return false;

  String strCommand = "SEND," + rstrReceipent + "," + String(ruiValue) + "\r\n";
  Send(strCommand);
  return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : BURN tokens
//
// -----------------------------------------------------------------------------
bool CGridShell::Burn(const CGridShell::eBurn& rWhat) {
  String strCommand = "";
  switch (rWhat) {
    case CGridShell::eBurn::BURN_TELEMETRY_SLOT:
      {
        strCommand = "BURN,TSLOT\r\n";
      }
      break;
    default:
      {
      }
      break;
  }

  if (strCommand.isEmpty())
    return false;

  Send(strCommand);
  return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : DTOR
//
// -----------------------------------------------------------------------------
CGridShell::~CGridShell() {
}
