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
  m_strUsername = "";
  m_uiLastHB = 0;
  m_bExecFlag = true;
  m_uiLastReconnection = millis() + (10 * GNODE_RECON_TIMER);
  m_pCallback = NULL;
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
  m_Client.flush();
  close(m_Client.fd());
  m_Client.stop();
  delay(500);
  if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_DISCONNECTED);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Set things up internally
//
// -----------------------------------------------------------------------------
bool CGridShell::Init(const String& strUsername, const bool& rbExecFlag) {
  GDEBUG("Lib start");

  //
  m_bExecFlag = rbExecFlag;
  m_strUsername = strUsername;

  // Validate username length
  if (m_strUsername.length() != 40) {
    m_strUsername = "";

    GDEBUG("Username is wrong");
    return false;
  }

  // Obtain MAC for ident purposes
  m_strMACAddress = WiFi.macAddress();
  m_strMACAddress.replace(":", "");

  // Remove telemetry chunks, prepare space for new
  CleanFS();

  GDEBUG("Init completed");

  //
  return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Remove telemetry chunks prefixed with GS
//
// -----------------------------------------------------------------------------
void CGridShell::CleanFS()
{
  GDEBUG("Cleaning GDFS");

  // List all files in SPIFFS
  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while (file) {
    String fileName = file.name();
    if (fileName.startsWith(GNODE_FILE_PREFIX)) {
      GDEBUG("Deleting file: " + fileName);
      SPIFFS.remove("/" + fileName);
    }
    file = root.openNextFile();
  }
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
  if (m_Client.connected()) {
    if (millis() - m_uiLastHB >= GNODE_PING_TIME) {
      GDEBUG("HEAP: " + String(ESP.getFreeHeap()));
      Send("PONG\r\n");
      m_uiLastHB = millis();
      if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_PONG);
    }
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
    int iTimer = millis() - m_uiLastReconnection;

    // Check if user set and reconnection timer is expired
    if (m_strUsername.length() == 40 && abs(iTimer) > GNODE_RECON_TIMER) {

      // Remove telemetry chunks, prepare space for new
      CleanFS();

      //
      m_uiLastReconnection = millis();

      GDEBUG("Reconnecting to server");

      //
      Stop();

      // Connect
      if (m_Client.connect(GNODE_SERVER, GNODE_POOL_PORT)) {

        if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_CONNECTED);


        // Ident and provide payload if any, this is base64 encoded already by ::Init
        String strVersion = m_Client.readStringUntil(',');
        String strWelcome = m_Client.readStringUntil(',');
        String strTasksToExecute = m_Client.readStringUntil(',');
        String strTasksToValidate = m_Client.readStringUntil(',');
        String strTemp = m_Client.readStringUntil('\n');

        // Safety check
        if (strVersion.isEmpty() || strWelcome.isEmpty() || strTasksToExecute.isEmpty() || strTasksToValidate.isEmpty()) {
          Stop();
          GDEBUG("No response, cancelled");
          return;
        }

        // Confirm versions
        if (strVersion != GNODE_VERSION) {
          if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_VERSIONS_MISMATCH);
          GDEBUG("Versions mismatch " + strVersion + " != " GNODE_VERSION);
          Stop();
          return;
        }

        // ****************************
        // Diffie-Hellman Key Exchange
        // ****************************

        // Get Server public key for our node
        String strServerPublicKey = m_Client.readStringUntil('\n');
        GDEBUG("Got ServPubKey=" + strServerPublicKey);

        // Failure, drop.
        if (strServerPublicKey.isEmpty()) {
          GDEBUG("No response, cancelled");
          Stop();
          return;
        }

        CBigInteger uiP = std::string("9840485683654561415963922255243388377177431468711912621027980528684674331318089597310841532159423071472940950709936601452503154610443618922381114939628259");
        CBigInteger uiG = std::string("2");
        CBigInteger uiServerPublicKey = strServerPublicKey.c_str();

        // Calculate our private key
        CBigInteger uiMyPrivateKey = std::to_string(esp_random());

        // Calculate our public key
        CBigInteger uiMyPublicKey = uiG.powMod(uiMyPrivateKey, uiP);

        // Compute symmetric (shared secret) key
        CBigInteger uiKey = uiServerPublicKey.powMod(uiMyPrivateKey, uiP);

        // ****************************
        // SHA1
        // ****************************
        String sha1HashKey = sha1HW(String(uiKey.GetInteger().c_str()));

        // ****************************
        // XOR
        // ****************************
        String strCipher = XOR(m_strUsername, sha1HashKey);

        // ****************************
        // BASE64ENCODE
        // ****************************
        String strBase64EncodedGUID = EncodeBase64(strCipher);


        // Pass my Public Key and GUID encoded
        Send("JOB," + String(uiMyPublicKey.GetInteger().c_str()) + "," + strBase64EncodedGUID + "," + GNODE_VERSION + "," + m_strMACAddress + "," + GNODE_ARCH + "," + String(m_bExecFlag) + "\r\n");

        GDEBUG("Sent Ident");

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
      GDEBUG("Server request " + strJobType);

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
        m_uiTaskTimeout = strTimeout.toInt();

        String strURLPath = GNODE_TASK_SERVER_NAME + strScriptName + ".bas";
        String strFSPath = "/" + strScriptName + ".bas";


        //
        GDEBUG("URL: '" + strURLPath + "'");
        GDEBUG("PATH:'" + strFSPath + "'");
        GDEBUG("HASHA:" + strTaskHash);
        GDEBUG("HASHB:" + GetSHA1(strFSPath));


        // Hash does not match, download the script
        if (strTaskHash != GetSHA1(strFSPath)) {
          SPIFFS.remove(strFSPath);

          GDEBUG("Diff hashes");

          // Get the task file
          if (StreamScript(strURLPath, strFSPath) == true)
            GDEBUG("Download completed");
          else GDEBUG("Download failed");
        } else GDEBUG("Hashes OK");

        File fScriptFile = SPIFFS.open(strFSPath, "r");
        if (fScriptFile) {
          String strScript = fScriptFile.readStringUntil('\0');
          fScriptFile.close();

          GDEBUG("Script Starting for " + String(m_uiTaskTimeout) + " ms ");

          // Update task start timer
          m_uiTaskStart = millis();

          uint32_t uiStart = millis();
          void** l = NULL;
          struct mb_interpreter_t* bas = NULL;

          // Initialize MYBASIC
          mb_init();
          mb_open(&bas);

          // Additional functions registration
          mb_register_func(bas, "READ", _read);
          mb_register_func(bas, "WRITE", _write);
          mb_register_func(bas, "SHA1", _sha1);
          mb_register_func(bas, "DOWNLOAD", _download);
          mb_register_func(bas, "TSIZE", _tsize);
          mb_register_func(bas, "FMD5", _fmd5);
          mb_register_func(bas, "B64D", _b64d);
          mb_register_func(bas, "B64E", _b64e);
          mb_register_func(bas, "DEL", _del);


          // Enable step by step execution to keep alive with the server
          mb_debug_set_stepped_handler(bas, CGridShell::MBStep);

          // Load up the script
          if (mb_load_string(bas, strScript.c_str(), true) == MB_FUNC_OK) {
            // payload check and upload
            if (strPayload != "") {
              GDEBUG("Added payload");
              mb_value_t valAdd;
              valAdd.type = MB_DT_STRING;
              valAdd.value.string = (char*)strPayload.c_str();
              mb_add_var(bas, l, "INPUTPAYLOAD", valAdd, true);
            } else GDEBUG("No payload?");

            // Run
            iRetCode = mb_run(bas, true);

            // Obtain output
            mb_value_t valGet;
            mb_get_value_by_name(bas, l, "OUTPUTPAYLOAD", &valGet);

            // Needs to be a string
            if (valGet.type == MB_DT_STRING)
              strOutput = String(valGet.value.string);
          }

          GDEBUG("Script completed in " + String(millis() - uiStart) + " ms, RESCODE: " + String(iRetCode) + ", MEM: " + String(ESP.getFreeHeap()) + " OUTP: '" + strOutput + "'");

          mb_close(&bas);
          mb_dispose();
        } else {
          GDEBUG("Failed to load script");
          strOutput = "";
          iRetCode = 0;
        }
        GDEBUG("Pushing results " + String(strOutput.length()));

        // Results can be long, so we treat them differently
        if (strOutput.length() > 0) {
          // Get Len
          size_t stLen = 0;
          mbedtls_base64_encode(NULL, 0, &stLen, (unsigned char*)strOutput.c_str(), strOutput.length());

          GDEBUG("B64 Length: " + String(stLen));

          // Heap
          unsigned char* encodedData = new unsigned char[stLen + 1];

          // Encode
          mbedtls_base64_encode(encodedData, stLen, &stLen, (unsigned char*)strOutput.c_str(), strOutput.length());

          // Clea
          encodedData[stLen] = '\0';
          strOutput = "";

          // Send
          m_Client.write("RESULTS,", 8);
          m_Client.write(String(iRetCode).c_str(), String(iRetCode).length());
          m_Client.write(",", 1);
          m_Client.write(encodedData, stLen);
          m_Client.write("\r\n", 2);

          // Delete
          delete[] encodedData;
        } else
          Send("RESULTS," + String(iRetCode) + ",\r\n");

        if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_IDLE);
      }
    }

    // Keep alive
    Pong();
  }
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
  m_Client.write(strData.c_str(), strData.length());
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
//  - Purpose   : Obtains the source of the task to execute via streaming
//
// -----------------------------------------------------------------------------
bool CGridShell::StreamScript(const String& rstrURL, const String& rstrPath) {

  HTTPClient httpClient;

  httpClient.begin(rstrURL);
  int httpCode = httpClient.GET();

  if (httpCode == HTTP_CODE_OK) {
    File file = SPIFFS.open(rstrPath, FILE_WRITE);
    if (!file) {
      GDEBUG("Failed to write a file " + rstrPath);
      httpClient.end();
      return false;
    }

    WiFiClient* stream = httpClient.getStreamPtr();
    uint8_t buffer[128] = { 0 };
    int bytesRead = 0;

    while (httpClient.connected() && (bytesRead = stream->readBytes(buffer, sizeof(buffer))) > 0) {
      file.write(buffer, bytesRead);
    }

    file.close();
    GDEBUG(rstrPath + " Saved");
    return true;

  } else {
    Serial.printf("Failed to download file. HTTP error code: %d\n", httpCode);
  }

  httpClient.end();
  GDEBUG("HTTPS Failed");
  return false;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Return MD5 of the given file
//
// -----------------------------------------------------------------------------
String CGridShell::GetMD5(const String& rstrFile)
{
  GDEBUG("MD5 " + rstrFile);
  File fFile = SPIFFS.open(rstrFile, "r");
  if (!fFile)return String();

  if (fFile.seek(0, SeekSet)) {
    MD5Builder md5;
    md5.begin();
    md5.addStream(fFile, fFile.size());
    md5.calculate();
    return md5.toString();
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
//  - Purpose   : Write a chunk of the telemetry
//
// -----------------------------------------------------------------------------
void CGridShell::Delete(const String& rstrName)
{
  SPIFFS.remove(rstrName);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Write a chunk of the telemetry
//
// -----------------------------------------------------------------------------
bool CGridShell::Write(const String& rstrName, const String& rstrWhat, const bool& bAppend) {

  GDEBUG("Write " + rstrName);

  File fTelemetry;

  if (bAppend)
    fTelemetry = SPIFFS.open(rstrName, "a");
  else
    fTelemetry = SPIFFS.open(rstrName, "w");

  if (!fTelemetry)
    return false;

  fTelemetry.print(rstrWhat);
  fTelemetry.close();

  GDEBUG("Write OK");
  return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Returns 
//
// -----------------------------------------------------------------------------
uint32_t CGridShell::AddTask(const String& rstrScript, const String& rstrInputPayload) {
  if (!Connected()) return 0;
  if (rstrScript.length() <= 0) return 0;
  if (rstrInputPayload.length() <= 0) return 0;
  if (rstrInputPayload.length() > GNODE_MAX_PAYLOAD_LEN) return 0;

  String strInputBase = EncodeBase64(rstrInputPayload);
  String strCommand = "ADDT," + rstrScript + "," + strInputBase + "\r\n";

  Send(strCommand);

  String strReturn = m_Client.readStringUntil('\n'); 

  GDEBUG("AddTask : " + strReturn);
 

  return 0;
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
