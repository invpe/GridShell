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
/*---------*/
#define GNODE_SERVER "work.gridshell.net"
#define GNODE_VERSION "03"
#define GNODE_PING_TIME 10000
#define GNODE_RECON_TIMER (1000 * 60)
#define GNODE_POOL_PORT 1911
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
    bool Init(const String& strUsername);
    uint32_t GetTaskTimeout();
    uint32_t GetTaskStartTime();
    void Pong();
    void Tick();
    bool Connected();
    void Stop();
    void RegisterEventCallback(void (*pFunc)(  uint8_t  ) );
    bool Write(const String& rstrName, const String& rstrWhat, const bool& bAppend);
    String Read(const String& rstrName, const uint32_t& ruiStart, const uint32_t& ruiCount);
    
    ~CGridShell();


    //void TestScript(const String& strScript, const String& strInputPayload);


  private:
    CGridShell();
    bool DownloadScript(const String& rstrURL,const String& rstrPath);
    String GetMD5(const String& rstrFile);
    String GetSHA1(const String& rstrFile);
    String XOR(const String& toEncrypt, const String& rstrKey);
    String sha1HW(String payload);
    String sha1HW(unsigned char *payload, int len);
    uint64_t power(uint64_t base, uint64_t exp, uint64_t mod);
    uint64_t modular_mul(uint64_t a, uint64_t b, uint64_t mod);
    String EncodeBase64(const String& strString);
    String DecodeBase64(const String& strString);
    void Send(const String& strData);
    String m_strUsername;
    String m_strMACAddress;
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

  char *cFilename;
  int_t iStart = 0;
  int_t iCount = 0;

  // Pop variables
  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_pop_string(s, l, &cFilename));
  mb_check(mb_pop_int(s, l, &iStart));
  mb_check(mb_pop_int(s, l, &iCount));
  mb_check(mb_attempt_close_bracket(s, l));



  String strRes = "";

  // GLIB
  //strRes = CGridShell::GetInstance().Read(String(cFilename), iStart, iCount);


  // HTTP API FS VERSION ( SAFER COZ SSL BUT SLOW (2SEC) )
  HTTPClient http;
  http.setReuse(true);
  http.begin("https://api.gridshell.net/fs/?file=" + String(cFilename) + "&s=" + String(iStart) + "&e=" + String(iCount));
  int httpCode = http.GET();
  String strData = http.getString();

  if (httpCode == 200)
    strRes = strData;
  http.end();

  GDEBUG("RET: '" + strRes + "'");
  char buf[strRes.length()];
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%s", strRes.c_str());
  mb_check(mb_push_string(s, l, mb_memdup(buf, (unsigned)(strlen(buf) + 1))));

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
#endif
