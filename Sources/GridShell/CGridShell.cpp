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
  m_uiCurrentFilePosition = 0;
  m_bAutoUpdate = true;
  m_strUsername = "";
  m_uiLastHB = 0;
  m_uiLastReconnection = 0;
  m_pCallback = NULL;

  // Get the unique MAC address from eFuse
  uint8_t mac[6];
#if defined(ESP8266)
  wifi_get_macaddr(STATION_IF, mac);

#else
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
#endif
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
#if defined(ESP8266)
  WiFiClientSecure client;
  client.setInsecure();
  if (m_HttpClient.begin(client, GNODE_CACERT_URL))
#else
  if (m_HttpClient.begin(GNODE_CACERT_URL))
#endif
  {
    int httpCode = m_HttpClient.GET();
    if (httpCode == HTTP_CODE_OK) {
      GDEBUG("CA certificate downloaded successfully.");
      strCert = m_HttpClient.getString();
    } else
      GDEBUG("Failed to download CA certificate: " + String(httpCode));
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
//  - Purpose   : Remove telemetry chunks prefixed with GS and script files, avoid formatting
//
// -----------------------------------------------------------------------------
void CGridShell::CleanFS() {
  GDEBUG("Chunks & Scripts removal");
  std::vector<String> patterns = { GNODE_FILE_PREFIX, ".bas" };

#if defined(ESP8266)
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    String fileName = dir.fileName();
    // Check if the filename contains any pattern from the vector
    for (const String& pattern : patterns) {
      if (fileName.indexOf(pattern) != -1) {
        // Delete the file
        if (SPIFFS.remove(fileName)) {
          Serial.println("Deleted file: " + fileName + " " + dir.fileSize());
          break;  // Break the loop if file is found and deleted
        } else {
          Serial.println("Failed to delete file: " + fileName);
        }
      }
    }
  }
#else
  File root = SPIFFS.open("/");
  if (!root) {
    GDEBUG("Failed to open directory");
    return;
  }
  File file = root.openNextFile();
  while (file) {
    String fileName = file.name();

    // Check if filename contains any pattern from the vector
    for (const String& pattern : patterns) {
      if (fileName.indexOf(pattern.c_str()) != -1) {
        // Delete the file
        if (SPIFFS.remove("/" + fileName)) {
          GDEBUG("Deleted file: " + fileName + " " + file.size());
          break;  // Break the loop if file is found and deleted
        } else {
          GDEBUG("Failed to delete file: " + fileName);
        }
      }
    }
    file = root.openNextFile();
  }
#endif
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

#ifdef GDEBUG
  static uint64_t uiDump = millis();
  if (millis() - uiDump > 1000) {
    GDEBUG("DEBUG MEM: " + String(MEMGetFree()) + " FS: " + String(FSGetUsed()) + " CONNECTED: " + String(Connected()));
    uiDump = millis();
  }
#endif


  // Are we up?
  if (m_Client.connected() == false) {
    // Check if user set and reconnection timer is expired
    if (millis() - m_uiLastReconnection >= GNODE_RECON_TIMER) {
      //
      m_uiLastReconnection = millis();
      configTime(0, 0, "pool.ntp.org");
      GDEBUG("Reconnecting");

      //
      Stop();

      // Remove telemetry chunks, prepare space for new
      CleanFS();

      // Get latest CA crt from github
      String strCert = GetCertificate();
      if (strCert.isEmpty()) {
        Stop();
        return;
      }
#if defined(ESP8266)
      BearSSL::X509List certList(strCert.c_str());
      m_Client.setBufferSizes(256, 256);
      m_Client.setTrustAnchors(&certList);
#else
      m_Client.setCACert(strCert.c_str());
#endif
      GDEBUG("Connecting");

      // Connect
      if (m_Client.connect(GNODE_SERVER, GNODE_POOL_PORT)) {

        GDEBUG("Connected");
        if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_CONNECTED);

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

        Send("JOB," + EncodeBase64(m_strUsername) + "," + GNODE_VERSION + "," + m_strUniqueID + "\r\n");

        GDEBUG("Ident");

        // Nothing to execute
        if (strTasksToExecute.toInt() == 0)
          if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_NO_TASKS_TO_EXECUTE);

        // Nothing to validate
        if (strTasksToValidate.toInt() == 0)
          if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_NO_TASKS_TO_VALIDATE);

        //
        if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_IDLE);
      } else {
        Stop();
        GDEBUG("Cant connect");
      }
    }
  } else {
    //
    String strJobType = m_Client.readStringUntil(',');

    //
    if (strJobType != "")
      GDEBUG("Server:" + strJobType);

    // Task coming
    if (strJobType == "EXEC") {

      if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_WORK);

      //
      int iRetCode = MB_FUNC_ERR;
      String strOutput = "";

      //
      String strPayload = DecodeBase64(m_Client.readStringUntil(','));
      String strTimeout = m_Client.readStringUntil(',');
      String strScript = DecodeBase64(m_Client.readStringUntil('\n'));

      if (strPayload.isEmpty()) {
        GDEBUG("Empty payload");
      }

      auto aResults = Run(strScript, strPayload, strTimeout.toInt());
      iRetCode = std::get<0>(aResults);
      strOutput = std::get<1>(aResults);


      if (strOutput.length() > 0) {
        size_t stLen = 0;

#if defined(ESP8266)
        stLen = Base64.encodedLength(strOutput.length());
        char* encodedData = new char[stLen + 1];
        Base64.encode(encodedData, (char*)strOutput.c_str(), strOutput.length());
#else
        mbedtls_base64_encode(NULL, 0, &stLen, (unsigned char*)strOutput.c_str(), strOutput.length());
        unsigned char* encodedData = new unsigned char[stLen + 1];
        mbedtls_base64_encode(encodedData, stLen, &stLen, (unsigned char*)strOutput.c_str(), strOutput.length());

#endif
        encodedData[stLen] = '\0';
        strOutput = "";

        // Send
        Send("RESULTS,");
        Send(String(iRetCode));
        Send(",");
        m_Client.write((unsigned char*)encodedData, stLen);
        Send("\r\n");

        delete[] encodedData;
      } else
        Send("RESULTS," + String(iRetCode) + ",\r\n");

      if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_IDLE);
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
std::tuple<int, String> CGridShell::Run(String& rstrScript, const String& rstrInputPayload, const uint32_t& ruiTaskTimeout) {

  uint32_t uiStart = millis();
  GDEBUG(rstrScript);
  GDEBUG("IN:" + rstrInputPayload);

  String strOutputPayload;
  int iRetCode = MB_FUNC_ERR;

  // Update task start timer
  m_uiTaskStart = millis();
  m_uiTaskTimeout = ruiTaskTimeout;
  void** l = NULL;
  struct mb_interpreter_t* bas = NULL;

  // Initialize MYBASIC
  mb_init();
  mb_open(&bas);

  // Additional functions
  mb_register_func(bas, "READ", _read);               // V08
  mb_register_func(bas, "READLINE", _readline);       // V081
  mb_register_func(bas, "RESETFPOS", _resetfilepos);  // V081
  mb_register_func(bas, "CSV2LIST", _csvtolist);      // V081
  mb_register_func(bas, "WRITE", _write);             // V08
  mb_register_func(bas, "SHA1", _sha1);               // V08
  mb_register_func(bas, "SHA256", _sha256);           // V08
  mb_register_func(bas, "SHA256H", _sha256H);         // V081
  mb_register_func(bas, "HEXTOBIN", _hextobin);       // V081
  mb_register_func(bas, "SECONDS", _seconds);         // V081
  mb_register_func(bas, "DOWNLOAD", _download);       // V08
  mb_register_func(bas, "TSIZE", _tsize);             // V08
  mb_register_func(bas, "FMD5", _fmd5);               // V08
  mb_register_func(bas, "B64D", _b64d);               // V08
  mb_register_func(bas, "B64E", _b64e);               // V08
  mb_register_func(bas, "XOR", _xor);                 // V08
  mb_register_func(bas, "DEL", _del);                 // V08

  // Enable step by step execution to keep alive with the server
  mb_debug_set_stepped_handler(bas, CGridShell::MBStep, NULL);

  // Load up the script
  if (mb_load_string(bas, rstrScript.c_str(), true) == MB_FUNC_OK) {

    rstrScript.clear();

    if (rstrInputPayload != "") {
      mb_value_t valAdd;
      valAdd.type = MB_DT_STRING;
      valAdd.value.string = (char*)rstrInputPayload.c_str();
      mb_add_var(bas, l, "INPUTPAYLOAD", valAdd, true);
    }

    iRetCode = mb_run(bas, true);

    mb_value_t valGet;
    mb_get_value_by_name(bas, l, "OUTPUTPAYLOAD", &valGet);

    if (valGet.type == MB_DT_STRING)
      strOutputPayload = String(valGet.value.string);
  }

  mb_close(&bas);
  mb_dispose();
  GDEBUG("EXEC IN " + String(millis() - uiStart) + " ms, RESCODE: " + String(iRetCode) + ", MEM: " + String(MEMGetFree()) + " OUTP: '" + strOutputPayload + "' FS: " + String(FSGetUsed()));
  return std::make_tuple(iRetCode, strOutputPayload);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Get telemetry file from the server to local storage
//
// -----------------------------------------------------------------------------
int CGridShell::GetTelemetry(const String& rstrFile) {
  GDEBUG("Getting telemetry " + rstrFile);

  int iTotal = 0;
  if (Connected()) {
    int iOffset = 0;  // Offset to keep track of the position in the file
    bool bContinueReading = true;
    File fTele = SPIFFS.open(GNODE_TELEMETRY_FILENAME, "r");

    while (bContinueReading) {
      // Pong
      Pong();

      // Request a chunk of the file
      Send("READ," + rstrFile + "," + String(iOffset) + "," + String(GNODE_IO_SIZE) + "\r\n");

      // Read the response
      String strJobType = m_Client.readStringUntil(',');
      String strContent = DecodeBase64(m_Client.readStringUntil('\n'));
      int iBytesRead = strContent.length();
      iTotal += iBytesRead;
      GDEBUG("Got " + String(iBytesRead) + "b");

      // Check if the chunk is empty or an error occurred
      if (iBytesRead == 0) {
        bContinueReading = false;  // Stop if no more data to read or error
      } else {
        // Write the chunk to the file
        fTele.print(strContent);
        iOffset += iBytesRead;  // Update the offset
      }
    }

    fTele.close();
  }
  GDEBUG("Total read: " + String(iTotal));
  return iTotal;
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
#if defined(ESP8266)

    // Rewrite string to char array for Encoding in Base64
    char cData[strString.length()];

    for (int a = 0; a < strString.length(); a++)
      cData[a] = strString[a];

    // Encode
    int encodedLength = Base64.encodedLength(strString.length());
    char encodedString[encodedLength];

    //
    Base64.encode(encodedString, cData, strString.length());

    // Here is the BASE64 Encoded Payload
    return (String(encodedString));
#else
    size_t stLen = 0;
    mbedtls_base64_encode(NULL, 0, &stLen, (unsigned char*)strString.c_str(), strString.length());
    unsigned char ucEncoded[stLen];
    mbedtls_base64_encode(ucEncoded, stLen, &stLen, (unsigned char*)strString.c_str(), strString.length());
    return String((char*)ucEncoded);
#endif
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
#if defined(ESP8266)

    // Rewrite string to char array for Encoding in Base64
    char cData[strString.length()];

    for (int a = 0; a < strString.length(); a++)
      cData[a] = strString[a];

    // Encode
    int encodedLength = Base64.decodedLength(cData, sizeof(cData));
    char encodedString[encodedLength];

    //
    Base64.decode(encodedString, cData, sizeof(cData));

    // Here is the BASE64 Encoded Payload
    return (String(encodedString));
#else
    size_t stLen = 0;
    mbedtls_base64_decode(NULL, 0, &stLen, (unsigned char*)strString.c_str(), strString.length());
    unsigned char ucDecoded[stLen];
    mbedtls_base64_decode(ucDecoded, stLen, &stLen, (unsigned char*)strString.c_str(), strString.length());
    ucDecoded[stLen] = '\0';
    return String((char*)ucDecoded);
#endif
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

  // Breath
  yield();

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
  unsigned char shaResult[20];
#if defined(ESP8266)
  sha1(payload, len, shaResult);
#else
  mbedtls_sha1_context ctx;

  mbedtls_sha1_init(&ctx);

  // Perform SHA1 hash
  mbedtls_sha1_starts_ret(&ctx);
  mbedtls_sha1_update_ret(&ctx, payload, len);
  mbedtls_sha1_finish_ret(&ctx, shaResult);

  mbedtls_sha1_free(&ctx);

#endif
  String hashStr = "";

  for (uint16_t i = 0; i < 20; i++) {
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

  // Reset the file position for ReadFileLine
  m_uiCurrentFilePosition = 0;

#if defined(ESP8266)
  WiFiClientSecure client;
  client.setInsecure();
  m_HttpClient.begin(client, rstrURL);
#else
  m_HttpClient.begin(rstrURL);
#endif
  int httpCode = m_HttpClient.GET();
  bool bSuccess = false;
  if (httpCode == HTTP_CODE_OK) {
    File file = SPIFFS.open(rstrPath, "w");
    if (!file) {
      GDEBUG("Cant write " + rstrPath);
    } else {
      WiFiClient* stream = m_HttpClient.getStreamPtr();
      uint8_t buffer[GNODE_IO_SIZE] = { 0 };
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
  unsigned char shaResult[32];  // SHA256 produces a 32-byte hash

#if defined(ESP8266)
  experimental::crypto::SHA256::hash(payload, len, shaResult);
#else
  mbedtls_sha256_context ctx;

  mbedtls_sha256_init(&ctx);

  // Perform SHA256 hash
  mbedtls_sha256_starts_ret(&ctx, 0);  // 0 for SHA256
  mbedtls_sha256_update_ret(&ctx, payload, len);
  mbedtls_sha256_finish_ret(&ctx, shaResult);

  mbedtls_sha256_free(&ctx);
#endif
  String hashStr = "";

  for (uint16_t i = 0; i < 32; i++) {
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
//  - Purpose   : Helper, reads file line by line (\n)
//
// -----------------------------------------------------------------------------
String CGridShell::ReadFileLine() {
  File file = SPIFFS.open(GNODE_TELEMETRY_FILENAME, "r");
  if (!file) {
    return String();
  }

  if (file.size() <= m_uiCurrentFilePosition) {
    file.close();
    return String();
  }

  file.seek(m_uiCurrentFilePosition);
  String line = file.readStringUntil('\n');
  m_uiCurrentFilePosition += line.length() + 1;  // +1 for \n
  file.close();
  return line;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Helper reads file from to byte
//
// -----------------------------------------------------------------------------
String CGridShell::ReadFile(const size_t& startPosition, const size_t& byteCount) {

  File file = SPIFFS.open(GNODE_TELEMETRY_FILENAME, "r");
  if (!file) {
    return String();
  }
  if (startPosition < 0 || byteCount <= 0) {
    return String();
  }
  size_t fileSize = file.size();

  if (startPosition >= fileSize) {
    file.close();
    return String();
  }

  size_t limitedByteCount = (byteCount > GNODE_IO_SIZE) ? GNODE_IO_SIZE : byteCount;
  if (startPosition + limitedByteCount > fileSize) {
    limitedByteCount = fileSize - startPosition;
  }

  char buffer[GNODE_IO_SIZE + 1];
  file.seek(startPosition);
  size_t bytesActuallyRead = file.readBytes(buffer, limitedByteCount);
  file.close();

  buffer[bytesActuallyRead] = '\0';

  return String(buffer);
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
  String strCommand = "ADDT," + EncodeBase64(rstrScript) + "," + strInputBase + "\r\n";

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

  if (!Connected()) return false;

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

  if (!Connected()) return false;

  String strCommand = "";
  switch (rWhat) {
    case CGridShell::eBurn::BURN_TELEMETRY_SLOT:
      {
        strCommand = "BURN,TSLOT\r\n";
      }
      break;
    case CGridShell::eBurn::BURN_TELEMETRY_TSIZE:
      {
        strCommand = "BURN,TSIZE\r\n";
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
//  - Purpose   : Read telemetry from server, data returned in b64
//
// -----------------------------------------------------------------------------
String CGridShell::Read(const String& rstrTelemetry, const uint32_t& ruiStart, const uint32_t& ruiCount) {

  if (!Connected()) return "";

  String strCommand = "READ," + rstrTelemetry + "," + String(ruiStart) + "," + String(ruiCount) + "\r\n";

  Send(strCommand);

  String strReturn = m_Client.readStringUntil('\n');
  String retType = strReturn.substring(0, strReturn.indexOf(","));
  String retVal = strReturn.substring(strReturn.indexOf(",") + 1);

  if (retType == "READ") {
    return retVal;
  }

  return "";
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Read exec payload of the task
//
// -----------------------------------------------------------------------------
String CGridShell::GetTask(const uint32_t& ruiTask) {
  if (!Connected()) return "";

  String strCommand = "GETTASK," + String(ruiTask) + "\r\n";

  Send(strCommand);

  String strReturn = m_Client.readStringUntil('\n');
  String retType = strReturn.substring(0, strReturn.indexOf(","));
  String retVal = strReturn.substring(strReturn.indexOf(",") + 1);

  if (retType == "GETTASK") {
    return retType;
  }

  return "";
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Set Persist flag to the task
//
// -----------------------------------------------------------------------------
void CGridShell::Persist(const uint32_t& ruiTask, const uint32_t& ruiFlag, const String& rstrFilename) {
  String strCommand = "PERSIST," + String(ruiTask) + "," + String(ruiFlag) + "," + rstrFilename + "\r\n";
  Send(strCommand);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : DTOR
//
// -----------------------------------------------------------------------------
void CGridShell::ResetFilePosition() {
  m_uiCurrentFilePosition = 0;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Helper function to obtain total size of SPIFFS
//
// -----------------------------------------------------------------------------
uint32_t CGridShell::FSGetTotal() {
  size_t totalBytes = 0;
#if defined(ESP8266)
  FSInfo fs_info;
  SPIFFS.info(fs_info);
  totalBytes = fs_info.totalBytes;
#else
  totalBytes = SPIFFS.totalBytes();
#endif
  return totalBytes;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Helper function to obtain used size of SPIFFS
//
// -----------------------------------------------------------------------------
uint32_t CGridShell::FSGetUsed() {
  size_t usedBytes = 0;
#if defined(ESP8266)
  FSInfo fs_info;
  SPIFFS.info(fs_info);
  usedBytes = fs_info.usedBytes;
#else
  usedBytes = SPIFFS.usedBytes();
#endif

  return usedBytes;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Helper to obtain heap free
//
// -----------------------------------------------------------------------------
uint32_t CGridShell::MEMGetFree() {
  return ESP.getFreeHeap();
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
