#include "CGridShell.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CGridShell::CGridShell()
{
  m_strUsername = "";
  m_uiLastHB    = 0;

  // To force the connection to happen immediately after poweron
  // We skip to the future ;-)
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
CGridShell& CGridShell::GetInstance()
{
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
void CGridShell::Stop()
{
  m_Client.stop();
  m_Client.flush();
  if (m_pCallback != NULL)m_pCallback(CGridShell::eEvent::EVENT_DISCONNECTED);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Set things up internally
//
// -----------------------------------------------------------------------------
bool CGridShell::Init(const String& strUsername)
{
  GDEBUG("Lib start");

  //
  m_strUsername = strUsername;

  // Validate username length
  if (m_strUsername.length() != 40)
  {
    m_strUsername = "";

    GDEBUG("Username is wrong");
    return false;
  }

  // Obtain MAC for ident purposes
  m_strMACAddress =  WiFi.macAddress();
  m_strMACAddress.replace(":", "");

  GDEBUG("Init completed");

  //
  return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Keep alive with server
//
// -----------------------------------------------------------------------------
void CGridShell::Pong()
{
  // Keep Alive
  if (m_Client.connected())
  {
    if (millis() - m_uiLastHB >= GNODE_PING_TIME)
    {
      Send("PONG\r\n");
      m_uiLastHB = millis();
      if (m_pCallback != NULL)m_pCallback(CGridShell::eEvent::EVENT_PONG);
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
bool CGridShell::Connected()
{
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
void CGridShell::Tick()
{
  // Are we up?
  if (m_Client.connected() == false)
  {
    int iTimer = millis() - m_uiLastReconnection;

    // Check if user set and reconnection timer is expired
    if (m_strUsername.length() == 40 && abs(iTimer) > GNODE_RECON_TIMER)
    {

      //
      m_uiLastReconnection = millis();

      GDEBUG("Reconnecting to server");

      //
      Stop();

      // Connect
      if (m_Client.connect(GNODE_SERVER, GNODE_POOL_PORT))
      {

        if (m_pCallback != NULL)m_pCallback(CGridShell::eEvent::EVENT_CONNECTED);


        // Ident and provide payload if any, this is base64 encoded already by ::Init
        String strVersion = m_Client.readStringUntil(',');
        String strWelcome = m_Client.readStringUntil(',');
        String strTasksToExecute = m_Client.readStringUntil(',');
        String strTasksToValidate = m_Client.readStringUntil(',');
        String strTemp = m_Client.readStringUntil('\n');

        // Safety check
        if (strVersion.isEmpty() || strWelcome.isEmpty() || strTasksToExecute.isEmpty() || strTasksToValidate.isEmpty())
        {
          Stop();
          GDEBUG("No response, cancelled");
          return;
        }

        // Confirm versions
        if (strVersion != GNODE_VERSION)
        {
          if (m_pCallback != NULL)m_pCallback(CGridShell::eEvent::EVENT_VERSIONS_MISMATCH);
          GDEBUG("Versions mismatch " + strVersion + " != "GNODE_VERSION);
          Stop();
          return;
        }

        // Nothing to execute
        if (strTasksToExecute.toInt() == 0)
          if (m_pCallback != NULL)m_pCallback(CGridShell::eEvent::EVENT_NO_TASKS_TO_EXECUTE);

        // Nothing to validate
        if (strTasksToValidate.toInt() == 0)
          if (m_pCallback != NULL)m_pCallback(CGridShell::eEvent::EVENT_NO_TASKS_TO_VALIDATE);

        // ****************************
        // Diffie-Hellman Key Exchange
        // ****************************

        // Get P,G and Server public key for our node
        String strServerP = m_Client.readStringUntil('\n');
        String strServerG = m_Client.readStringUntil('\n');
        String strServerPublicKey = m_Client.readStringUntil('\n');
        GDEBUG("Got P=" + strServerP + " G=" + strServerG + " ServPubKey=" + strServerPublicKey);

        // Failure, drop.
        if (strServerP.isEmpty() || strServerG.isEmpty() || strServerPublicKey.isEmpty())
        {
          GDEBUG("No response, cancelled");
          Stop();
          return;
        }

        //
        uint64_t uiServerP          = strtoull(strServerP.c_str(), NULL, 10);
        uint64_t uiServerG          = strtoull(strServerG.c_str(), NULL, 10);
        uint64_t uiServerPublicKey  = strtoull(strServerPublicKey.c_str(), NULL, 10);

        // Calculate our private key
        uint64_t uiDHPrivateKey = esp_random();

        // Calculate our public key
        uint64_t uiDHPublicKey = power(uiServerG, uiDHPrivateKey, uiServerP);

        // Compute symmetric (secret) key
        uint64_t uiKey = power(uiServerPublicKey, uiDHPrivateKey, uiServerP);

        // ****************************
        // SHA1
        // ****************************

        char cTemp[40];
        sprintf(cTemp, "%llu", uiKey);
        String sha1HashKey = sha1HW(cTemp);

        // ****************************
        // XOR
        // ****************************

        String strCipher = XOR(m_strUsername, sha1HashKey);

        // ****************************
        // BASE64ENCODE
        // ****************************

        String strBase64EncodedGUID = EncodeBase64(strCipher);

        // Pass my Public Key and GUID encoded
        Send("JOB," + String(std::to_string(uiDHPublicKey).c_str()) + "," + strBase64EncodedGUID + "," + GNODE_VERSION + "," + m_strMACAddress + "," + GNODE_ARCH + "\r\n");


        //
        if (m_pCallback != NULL)m_pCallback(CGridShell::eEvent::EVENT_IDLE);
      }
    }
  }
  else
  {
    // Wait for server to post a message
    if (m_Client.available())
    {

      //
      String strJobType = m_Client.readStringUntil(',');

      //
      GDEBUG("Server request " + strJobType);

      // Task coming
      if (strJobType == "EXEC")
      {

        if (m_pCallback != NULL)m_pCallback(CGridShell::eEvent::EVENT_WORK);

        //
        int iRetCode = MB_FUNC_ERR;
        String strOutput = "";

        // 
        String strScriptName  = DecodeBase64(m_Client.readStringUntil(','));
        String strPayload     = DecodeBase64(m_Client.readStringUntil(','));
        String strTimeout     = m_Client.readStringUntil(',');
        String strTaskHash    = m_Client.readStringUntil(',');
        m_uiTaskTimeout       = strTimeout.toInt();

        String strURLPath     = GNODE_TASK_SERVER_NAME + strScriptName + ".bas";
        String strFSPath      = "/" + strScriptName + ".bas";


        //
        GDEBUG("URL: '" + strURLPath + "'");
        GDEBUG("PATH:'" + strFSPath + "'");
        GDEBUG("HASHA:" + strTaskHash);
        GDEBUG("HASHB:" + GetSHA1(strFSPath));


        // Hash does not match, download the script
        if (strTaskHash != GetSHA1(strFSPath))
        {
          SPIFFS.remove(strFSPath);

          GDEBUG("Diff hashes");

          // Get the task file
          if (DownloadScript(strURLPath, strFSPath) == true)
            GDEBUG("Download completed");
          else GDEBUG("Download failed");
        }
        else  GDEBUG("Hashes OK");

        File fScriptFile = SPIFFS.open(strFSPath, "r");
        if (fScriptFile)
        {
          String strScript = fScriptFile.readStringUntil('\0') ;
          fScriptFile.close();

          GDEBUG("Script Starting for " + String(m_uiTaskTimeout) + " ms ");

          // Update task start timer
          m_uiTaskStart = millis();

          uint32_t uiStart = millis();
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

          // Enable step by step execution to keep alive with the server
          mb_debug_set_stepped_handler(bas, CGridShell::MBStep);

          // Load up the script
          if (mb_load_string(bas, strScript.c_str(), true) == MB_FUNC_OK)
          {
            // payload check and upload
            if (strPayload != "")
            {
              GDEBUG("Added payload");
              mb_value_t valAdd;
              valAdd.type = MB_DT_STRING;
              valAdd.value.string =  (char *)strPayload.c_str();
              mb_add_var(bas, l, "INPUTPAYLOAD", valAdd, true);
            }
            else GDEBUG("No payload?");

            // Run
            iRetCode = mb_run(bas, true);

            // Obtain output
            mb_value_t valGet;
            int iRes = mb_get_value_by_name(bas, l, "OUTPUTPAYLOAD", &valGet);

            // Needs to be a string
            if (valGet.type == MB_DT_STRING)
              strOutput = String(valGet.value.string);
          }

          GDEBUG("Script completed in " + String(millis() - uiStart) + " ms, RESCODE: " + String(iRetCode) + ", MEM: " + String(ESP.getFreeHeap()) + " OUTP: '" + strOutput + "'");

          mb_close(&bas);
          mb_dispose();
        }
        else
        {
          GDEBUG("Failed to load script");
          strOutput = "";
          iRetCode = 0;
        }

        Send("RESULTS," + String(iRetCode) + "," + EncodeBase64(strOutput) + "\r\n");

        if (m_pCallback != NULL)m_pCallback(CGridShell::eEvent::EVENT_IDLE);
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
void CGridShell::RegisterEventCallback(void (*pFunc)(  uint8_t  ))
{
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
void CGridShell::Send(const String & strData)
{
  m_Client.write( strData.c_str(),  strData.length() );
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Encodes Base64 to String
//
// ---------------------------------------------------------------------------
String CGridShell::EncodeBase64(const String & strString)
{
  if (strString.length() > 0)
  {
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
String CGridShell::DecodeBase64(const String & strString)
{
  if (strString.length() > 0)
  {
    size_t stLen = 0;
    mbedtls_base64_decode( NULL, 0, &stLen, (unsigned char*)strString.c_str(), strString.length());

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
uint32_t CGridShell::GetTaskTimeout()
{
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
uint32_t CGridShell::GetTaskStartTime()
{
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
int CGridShell::MBStep(struct mb_interpreter_t* s, void** l, const char* f, int p, unsigned short row, unsigned short col)
{
  // Avoid endless loops
  if (millis() - CGridShell::GetInstance().GetTaskStartTime() > CGridShell::GetInstance().GetTaskTimeout())
    return GNODE_RET_TERMINATED;

  // Breath
  yield();
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
String CGridShell::sha1HW(String payload)
{
  return sha1HW((unsigned char *)payload.c_str(), payload.length());
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : HW SHA1
//
// -----------------------------------------------------------------------------
String CGridShell::sha1HW(unsigned char *payload, int len)
{
  //
  int size = 20;
  byte shaResult[size];

  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA1;

  const size_t payloadLength = len;

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *) payload, payloadLength);
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
//  - Purpose   : Modular Mul
//
// -----------------------------------------------------------------------------
uint64_t CGridShell::modular_mul(uint64_t a, uint64_t b, uint64_t mod) {
  uint64_t result = 0;
  for (uint64_t current_term = a; b; b >>= 1) {
    if (b & 1) {
      result = (result + current_term) % mod;
    }
    current_term = 2 * current_term % mod;
  }
  return result;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Power method
//
// -----------------------------------------------------------------------------
uint64_t CGridShell::power(uint64_t base, uint64_t exp, uint64_t mod) {
  uint64_t result = 1;
  for (uint64_t current_factor = base; exp; exp >>= 1) {
    if (exp & 1) {
      result = modular_mul(result, current_factor, mod);
    }
    current_factor = modular_mul(current_factor, current_factor, mod);
  }
  return result;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : XOR
//
// -----------------------------------------------------------------------------
String CGridShell::XOR(const String & toEncrypt, const String & rstrKey)
{
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
//  - Purpose   : Obtains the source of the task to execute
//
// -----------------------------------------------------------------------------
bool CGridShell::DownloadScript(const String& rstrURL, const String& rstrPath)
{

  //
  HTTPClient http;
  http.begin(rstrURL);

  //
  int httpCode    = http.GET();
  String strData  = http.getString();

  //
  if (httpCode == 200)
  {
    http.end();

    GDEBUG("HTTPS Downloaded");

    File fScript = SPIFFS.open(rstrPath, "w");

    if (!fScript) {
      GDEBUG("Failed to write a file " + rstrPath);
      return false;
    }
    fScript.print(strData);
    fScript.close();

    GDEBUG(rstrPath + " Saved");
    return true;
  }
  http.end();

  GDEBUG("HTTPS Failed");
  return false;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Get MD5 of a given file
//
// -----------------------------------------------------------------------------
String CGridShell::GetMD5(const String& rstrFile)
{
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
String CGridShell::GetSHA1(const String& rstrFile)
{
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
//  - Purpose   : **Experimental** (LIB only)
//                Write a string to a file stored on the grid network (max 128b)
//
// -----------------------------------------------------------------------------
bool CGridShell::Write(const String& rstrName, const String& rstrWhat, const bool& bAppend)
{
  if (rstrWhat.length() > GNODE_WRITE_MAX)return false;

  String strBaseEncoded = EncodeBase64(rstrWhat);

  String strCommand;
  if (bAppend)strCommand = "APPEND,";
  else strCommand = "WRITE,";

  strCommand += rstrName + "," + strBaseEncoded + "\r\n";

  Send(strCommand);

  String strReturn      = m_Client.readStringUntil(',');
  String strReturnCode  = m_Client.readStringUntil(',');


  GDEBUG("Write : " + strReturnCode);

  //
  if (strReturn == "FILE" && strReturnCode[0] == 'O' && strReturnCode[1] == 'K')
    return true;
  return false;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype : TestScript(const String& strScript, const String& strInputPayload)
//
//  - Purpose   : Uncomment to play with testing the scripts on the device, normally
//                this is not used, so commented out.
//
//                Example:
//                Initialize and get Grid Online
//                String strScript = "OUTPUTPAYLOAD=READ(\"NB4C11AEF6ECF02023213\",10,10)\nPRINT OUTPUTPAYLOAD;";
//                CGridShell::GetInstance().TestScript(strScript, "some_input_payload");
// -----------------------------------------------------------------------------
/*
void CGridShell::TestScript(const String& strScript, const String& strInputPayload)
{
  Serial.println(strScript);

  //
  int iRetCode = MB_FUNC_ERR;
  String strOutput = "";

  void **l = NULL;
  struct mb_interpreter_t* bas = NULL;

  // Initialize MYBASIC
  mb_init();
  mb_open(&bas);

  // Bitwise operations
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

    // Run
    iRetCode = mb_run(bas, true);

    // Obtain output
    mb_value_t valGet;
    int iRes = mb_get_value_by_name(bas, l, "OUTPUTPAYLOAD", &valGet);

    // Needs to be a string
    strOutput = String(valGet.value.string);
  }

  mb_close(&bas);
  mb_dispose();
}*/
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : DTOR
//
// -----------------------------------------------------------------------------
CGridShell::~CGridShell()
{

}
