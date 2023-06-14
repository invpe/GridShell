// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : GridShell Arduino Library  https://www.gridshell.net/
//
// -----------------------------------------------------------------------------
#ifndef __CLIB_GRID__
#define __CLIB_GRID__
/*---------*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <MD5Builder.h>
#include "SPIFFS.h"
#include "my_basic.hpp"
#include "mbedtls/base64.h"
#include "CBigInteger.h"
/*---------*/
#define GNODE_MAX_PAYLOAD_LEN 256
#define GNODE_TASK_SERVER_NAME "https://api.gridshell.net/scripts/"
#define GNODE_FS_SERVER "https://api.gridshell.net/fs/"
#define GNODE_SERVER "work.gridshell.net"
#define GNODE_VERSION "05"
#define GNODE_TELEMETRY_FILENAME "/TELEMETRY"
#define GNODE_PING_TIME 10000
#define GNODE_RECON_TIMER (1000 * 60)
#define GNODE_POOL_PORT 1911
#define GNODE_WRITE_MAX 128
#define GNODE_READ_MAX 128
#define GNODE_RET_TERMINATED 777
#define GNODE_ARCH "ESP32"
/*---------*/
// Enable to dump debug informations to the serial
//#define GNODE_DEBUG 1
#ifdef GNODE_DEBUG
#define GDEBUG Serial.println
#else
#define GDEBUG void
#endif
/*---------*/
class CGridShell
{
  public:
    enum eEvent
    {
      EVENT_IDLE = 0,
      EVENT_WORK,
      EVENT_DISCONNECTED,
      EVENT_CONNECTED,
      EVENT_NO_TASKS_TO_EXECUTE,
      EVENT_NO_TASKS_TO_VALIDATE,
      EVENT_VERSIONS_MISMATCH,
      EVENT_PONG
    };
    static CGridShell& GetInstance();
    static int MBStep(struct mb_interpreter_t* s, void** l, const char* f, int p, unsigned short row, unsigned short col);
    bool Init(const String& strUsername, const bool& rbExecFlag);
    uint32_t GetTaskTimeout();
    uint32_t GetTaskStartTime();
    void Pong();
    void Tick();
    bool Connected();
    void Stop();
    void RegisterEventCallback(void (*pFunc)(  uint8_t  ) );
    bool Write(const String& rstrName, const String& rstrWhat, const bool& bAppend);
    uint32_t AddTask(const String& rstrScript, const String& rstrInputPayload);

    ~CGridShell();

  private:
    CGridShell();
    bool StreamScript(const String& rstrURL, const String& rstrPath);
    String GetMD5(const String& rstrFile);
    String GetSHA1(const String& rstrFile);
    String XOR(const String& toEncrypt, const String& rstrKey);
    String sha1HW(String payload);
    String sha1HW(unsigned char *payload, int len);
    String EncodeBase64(const String& strString);
    String DecodeBase64(const String& strString);
    void Send(const String& strData);
    String m_strUsername;
    String m_strMACAddress;
    uint8_t m_bExecFlag;
    uint32_t m_uiLastHB;
    uint32_t m_uiLastReconnection;
    uint32_t m_uiTaskStart;
    uint32_t m_uiTaskTimeout;
    WiFiClient m_Client;
    void (*m_pCallback)(uint8_t);
};
/*---------*/
static int _bit_and(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  int_t m = 0;
  int_t n = 0;

  mb_check(mb_attempt_open_bracket(s, l));

  mb_check(mb_pop_int(s, l, &m));
  mb_check(mb_pop_int(s, l, &n));

  mb_check(mb_attempt_close_bracket(s, l));
  mb_check(mb_push_int(s, l,  (m & n)));

  return result;
}
static int _bit_or(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  int_t m = 0;
  int_t n = 0;


  mb_check(mb_attempt_open_bracket(s, l));

  mb_check(mb_pop_int(s, l, &m));
  mb_check(mb_pop_int(s, l, &n));

  mb_check(mb_attempt_close_bracket(s, l));

  mb_check(mb_push_int(s, l, m | n));

  return result;
}
static int _bit_not(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  int_t m = 0;


  mb_check(mb_attempt_open_bracket(s, l));

  mb_check(mb_pop_int(s, l, &m));

  mb_check(mb_attempt_close_bracket(s, l));

  mb_check(mb_push_int(s, l, ~m));

  return result;
}
static int _bit_xor(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  int_t m = 0;
  int_t n = 0;
  mb_check(mb_attempt_open_bracket(s, l));

  mb_check(mb_pop_int(s, l, &m));
  mb_check(mb_pop_int(s, l, &n));

  mb_check(mb_attempt_close_bracket(s, l));

  mb_check(mb_push_int(s, l, m ^ n));

  return result;
}
static int _bit_lshift(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  int_t m = 0;
  int_t n = 0;


  mb_check(mb_attempt_open_bracket(s, l));

  mb_check(mb_pop_int(s, l, &m));
  mb_check(mb_pop_int(s, l, &n));

  mb_check(mb_attempt_close_bracket(s, l));

  mb_check(mb_push_int(s, l, m << n));
  return result;
}
static int _bit_rshift(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  int_t m = 0;
  int_t n = 0;

  mb_check(mb_attempt_open_bracket(s, l));

  mb_check(mb_pop_int(s, l, &m));
  mb_check(mb_pop_int(s, l, &n));

  mb_check(mb_attempt_close_bracket(s, l));

  mb_check(mb_push_int(s, l, m >> n));

  return result;
}
static int _sha1(struct mb_interpreter_t* s, void** l)
{
  int result = MB_FUNC_OK;

  mb_check(mb_attempt_open_bracket(s, l));

  char *m;
  mb_value_t arg;

  mb_check(mb_pop_string(s, l, &m));
  mb_check(mb_attempt_close_bracket(s, l));


  int_t iLen = strlen(m);
  byte shaResult[20];

  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA1;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *) m, iLen);
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);

  // To string conversions happening, need better idea some day
  String hashStr;
  for (uint16_t i = 0; i < sizeof(shaResult); i++) {
    String hex = String(shaResult[i], HEX);
    if (hex.length() < 2)
      hex = "0" + hex;
    hashStr += hex;
  }
  char buf[hashStr.length()];
  sprintf(buf, "%s", hashStr.c_str());
  mb_check(mb_push_string(s, l, mb_memdup(buf, (unsigned)(strlen(buf) + 1))));
  return result;
}
static int _read(struct mb_interpreter_t* s, void** l)
{
  int result = MB_FUNC_OK;

  int_t iStart = 0;
  int_t iCount = 0;

  // Pop variables
  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_pop_int(s, l, &iStart));
  mb_check(mb_pop_int(s, l, &iCount));
  mb_check(mb_attempt_close_bracket(s, l));

  // Returned upon failure
  char buf[] = {0x00};

  // Check for start less than 0
  if (iStart < 0)
  {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read count <= 0");
    return result;
  }

  // Check for read size > 0
  if (iCount <= 0)
  {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read count <= 0");
    return result;
  }

  // Check for count > chunk limit
  if (iCount > GNODE_READ_MAX)
  {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read count > GNODE_READ_MAX");
    return result;
  }

  // Check if exists
  File fFile = SPIFFS.open(GNODE_TELEMETRY_FILENAME);
  if (!fFile)
  {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read file doesnt exist GNODE_TELEMETRY_FILENAME");
    return result;
  }

  // Check for start boundary
  if (iStart > fFile.size())
  {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read Start boundary " + String(iStart) + " past " + String(fFile.size()));
    return result;
  }

  // Check for count boundary
  if (iCount > fFile.size())
  {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read Count boundary " + String(iCount) + " past " + String(fFile.size()));
    return result;
  }

  // Check for start+count boundary vs size
  if (iStart + iCount > fFile.size())
  {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read boundary " + String(iStart + iCount) + " past " + String(fFile.size()));
    return result;
  }

  // Finaly read the file
  GDEBUG("Reading file " + String(GNODE_TELEMETRY_FILENAME));

  //
  byte bReadBuffer[GNODE_READ_MAX + 1];
  memset(bReadBuffer, 0x00, sizeof(bReadBuffer));
  uint32_t uiBytesRead = 0;

  fFile.seek(iStart, SeekSet);
  uiBytesRead = fFile.readBytes((char*)bReadBuffer, iCount);
  fFile.close();

  // We add one more byte for NULL terminator as basic likes strings not bytes
  bReadBuffer[uiBytesRead] = '\0';

  GDEBUG("READ " + String(uiBytesRead) + " of " + String(iCount) + " requested");

  mb_check(mb_push_string(s, l, mb_memdup((char*)bReadBuffer, (unsigned)(uiBytesRead + 1))));

  return result;
}
static int _tsize(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  int_t iSize = 0;

  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_attempt_close_bracket(s, l));

  File fTele = SPIFFS.open(GNODE_TELEMETRY_FILENAME);
  if (fTele)
  {
    iSize = fTele.size();
    GDEBUG("TELEMETRY Size: " + String(fTele.size()));
    fTele.close();
  }
  else
    GDEBUG("TELEMETRY file doesnt exist");

  mb_check(mb_push_int(s, l, iSize));

  return result;
}
static int _write(struct mb_interpreter_t* s, void** l)
{
  int result = MB_FUNC_OK;
  char *cFilename;
  char *cText;
  int_t iAppend = 0;


  // Pop variables
  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_pop_string(s, l, &cFilename));
  mb_check(mb_pop_string(s, l, &cText));
  mb_check(mb_pop_int(s, l, &iAppend));
  mb_check(mb_attempt_close_bracket(s, l));

  int_t iSuccess = CGridShell::GetInstance().Write(String(cFilename), String(cText), iAppend);

  mb_check(mb_push_int(s, l, 0));
  return result;
}
static int _download(struct mb_interpreter_t* s, void** l)
{ 

  int result = MB_FUNC_OK;
  char *cFilename;

  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_pop_string(s, l, &cFilename));
  mb_check(mb_attempt_close_bracket(s, l));

  // Fixed name for overwriting (so we don't keep downloaded files)
  String strPath = GNODE_TELEMETRY_FILENAME;

  GDEBUG("HTTPS Getting " + String(cFilename));

  HTTPClient httpClient;
  uint32_t uiBytesWritten = 0;
  httpClient.begin(GNODE_FS_SERVER + String(cFilename));
  int httpCode = httpClient.GET();

  if (httpCode == HTTP_CODE_OK)
  {
    File file = SPIFFS.open(strPath, FILE_WRITE);
    if (!file)
    {
      GDEBUG("Failed to write a file " + strPath);
      httpClient.end();
      return false;
    }

    WiFiClient* stream = httpClient.getStreamPtr();
    uint8_t buffer[128] = {0};
    int bytesRead = 0;

    while (httpClient.connected() && (bytesRead = stream->readBytes(buffer, sizeof(buffer))) > 0) {
      file.write(buffer, bytesRead);
      uiBytesWritten += bytesRead;
    }

    file.close();
    GDEBUG(strPath + " Saved");
  }
  else
    GDEBUG("HTTPS Failed downloading");

  httpClient.end();

  mb_check(mb_push_int(s, l, uiBytesWritten));
  return result;
}
#endif
