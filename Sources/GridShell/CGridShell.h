// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : GridShell Arduino Library  https://www.gridshell.net/
//
// MIT License
//
// Copyright (c) 2022-2024 GridShell.net
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
// -----------------------------------------------------------------------------
#ifndef __CLIB_GRIDSHELL__
#define __CLIB_GRIDSHELL__
/*---------*/
#if defined(ESP8266)
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <Hash.h>
#include <Crypto.h>
#include <FS.h>
#include <Preferences.h>
#include <Base64.h>
#else
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Update.h>
#include "SPIFFS.h"
#include "esp_mac.h"
#include "mbedtls/base64.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "MD5Builder.h"
#endif
#include "my_basic.h"
/*---------*/
#define GNODE_PING_TIME 10000
#define GNODE_RECON_TIMER 1000
#define GNODE_POOL_PORT 1911
#define GNODE_RET_TERMINATED 777
#define GNODE_CACERT_URL "https://raw.githubusercontent.com/invpe/GridShell/main/Sources/GridShell/ca.crt"
#if defined(ESP8266)
#define GNODE_FIRMWARE_URL "https://github.com/invpe/GridShell/releases/latest/download/esp8266.bin"
#else
#define GNODE_FIRMWARE_URL "https://github.com/invpe/GridShell/releases/latest/download/esp32.bin"
#endif
#define GNODE_FILE_PREFIX "GS"
#define GNODE_SERVER "work.gridshell.net"
#define GNODE_VERSION "09"
#define GNODE_IO_SIZE 1024
#define GNODE_TELEMETRY_FILENAME "/" GNODE_FILE_PREFIX "TELEMETRY"
/*---------*/
// #define GNODE_DEBUG 1
#ifdef GNODE_DEBUG
#define GDEBUG Serial.println
#else
#define GDEBUG void
#endif
/*---------*/
class CGridShell {
public:
  enum eBurn {
    BURN_TELEMETRY_SLOT,
    BURN_TELEMETRY_TSIZE,
    BURN_TOTAL
  };
  enum eEvent {
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
  bool Init(const String& strUsername, const bool& bAutoUpdate = true);
  uint32_t GetTaskTimeout();
  uint32_t GetTaskStartTime();
  void Pong();
  void Tick();
  bool Connected();
  void Stop();
  void ResetFilePosition();
  void RegisterEventCallback(void (*pFunc)(uint8_t));
  String GetNodeID() const;
  uint32_t AddTask(const String& rstrScript, const String& rstrInputPayload);
  bool Send(const String& rstrReceipent, const uint32_t& ruiValue);
  bool Burn(const CGridShell::eBurn& rWhat);
  void Persist(const uint32_t& ruiTask, const uint32_t& ruiFlag, const String& rstrFilename);
  String GetTask(const uint32_t& ruiTask);
  String Read(const String& rstrTelemetry, const uint32_t& ruiStart, const uint32_t& ruiCount);
  std::tuple<int, String> Run(String& rstrBASFile, const String& rstrInputPayload, const uint32_t& ruiTaskTimeout);
  HTTPClient* GetHTTPClient();
  int GetTelemetry(const String& rstrFile);
  String ReadFile(const size_t& startPosition, const size_t& byteCount, const String& rstrFile = GNODE_TELEMETRY_FILENAME);
  String ReadFileLine();
  bool Write(const String& rstrName, const String& rstrWhat, const bool& bAppend);
  void Delete(const String& rstrName);
  String EncodeBase64(const String& strString);
  String DecodeBase64(const String& strString);
  String GetSHA1(const String& rstrFile);
  String GetMD5(const String& rstrFile);
  String sha1HW(String payload);
  String sha1HW(unsigned char* payload, int len);
  String sha256HW(unsigned char* payload, int len);
  String sha256HW(String payload);
  String XOR(const String& toEncrypt, const String& rstrKey);
  size_t FSGetTotal();
  size_t FSGetUsed();
  size_t MEMGetFree();
  ~CGridShell();

private:
  CGridShell();
  bool StreamFile(const String& rstrURL, const String& rstrPath);
  void Reboot();
  void OTA();
  void CleanFS();
  String GetCertificate();
  void Send(const String& strData);
  bool m_bAutoUpdate;
  String m_strUsername;
  String m_strUniqueID;
  uint32_t m_uiLastHB;
  uint32_t m_uiLastReconnection;
  uint32_t m_uiTaskStart;
  uint32_t m_uiTaskTimeout;
  WiFiClientSecure m_Client;
  HTTPClient m_HttpClient;
  MD5Builder m_MD5;
  uint32_t m_uiCurrentFilePosition;
  void (*m_pCallback)(uint8_t);
};
/*---------*/
static int _xor(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;

  mb_check(mb_attempt_open_bracket(s, l));

  char* m;
  char* n;

  mb_check(mb_pop_string(s, l, &m));
  mb_check(mb_pop_string(s, l, &n));
  mb_check(mb_attempt_close_bracket(s, l));

  String strRes = CGridShell::GetInstance().XOR(m, n);
  char buf[strRes.length()];
  sprintf(buf, "%s", strRes.c_str());

  mb_check(mb_push_string(s, l, mb_memdup(buf, (unsigned)(strlen(buf) + 1))));
  return result;
}

/*---------*/
static int _b64e(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;

  mb_check(mb_attempt_open_bracket(s, l));

  char* m;

  mb_check(mb_pop_string(s, l, &m));
  mb_check(mb_attempt_close_bracket(s, l));

  String strRes = CGridShell::GetInstance().EncodeBase64(m);
  char buf[strRes.length()];
  sprintf(buf, "%s", strRes.c_str());

  mb_check(mb_push_string(s, l, mb_memdup(buf, (unsigned)(strlen(buf) + 1))));
  return result;
}
/*---------*/
static int _b64d(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;

  mb_check(mb_attempt_open_bracket(s, l));

  char* m;

  mb_check(mb_pop_string(s, l, &m));
  mb_check(mb_attempt_close_bracket(s, l));

  String strRes = CGridShell::GetInstance().DecodeBase64(m);
  char buf[strRes.length()];
  sprintf(buf, "%s", strRes.c_str());

  mb_check(mb_push_string(s, l, mb_memdup(buf, (unsigned)(strlen(buf) + 1))));
  return result;
}
/*---------*/
static int _sha1(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;

  mb_check(mb_attempt_open_bracket(s, l));
  char* m;

  mb_check(mb_pop_string(s, l, &m));
  mb_check(mb_attempt_close_bracket(s, l));

  String strRes = CGridShell::GetInstance().sha1HW(m);
  char buf[strRes.length()];
  sprintf(buf, "%s", strRes.c_str());

  mb_check(mb_push_string(s, l, mb_memdup(buf, (unsigned)(strlen(buf) + 1))));
  return result;
}
/*---------*/
static int _csvtolist(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;

  mb_check(mb_attempt_open_bracket(s, l));

  char* strInputString;

  mb_check(mb_pop_string(s, l, &strInputString));
  mb_check(mb_attempt_close_bracket(s, l));

  std::vector<String> vValues;
  String strInput(strInputString);
  int start = 0;
  int end = strInput.indexOf(',');

  while (end != -1) {
    vValues.push_back(strInput.substring(start, end));
    start = end + 1;
    end = strInput.indexOf(',', start);
  }

  // Add the last substring (or the entire string if no comma was found)
  vValues.push_back(strInput.substring(start));

  mb_value_t val;
  mb_make_nil(val);

  mb_value_t coll;
  coll.type = MB_DT_LIST;
  mb_init_coll(s, l, &coll);

  int feedback_index = 0;
  for (size_t i = 0; i < vValues.size(); ++i) {
    mb_value_t mb_feedback_index;
    mb_value_t feedback_value;
    mb_make_int(mb_feedback_index, feedback_index++);
    mb_make_string(feedback_value, const_cast<char*>(vValues[i].c_str()));
    mb_set_coll(s, l, coll, mb_feedback_index, feedback_value);
  }


  // Push the comparison result onto the stack
  mb_check(mb_push_value(s, l, coll));
  return result;
}
/*---------*/
static int _hextobin(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;

  mb_check(mb_attempt_open_bracket(s, l));

  char* hashed_block_hex;

  mb_check(mb_pop_string(s, l, &hashed_block_hex));
  mb_check(mb_attempt_close_bracket(s, l));

  String strHashedBlock = String(hashed_block_hex);
  mb_value_t val;
  mb_make_nil(val);

  mb_value_t coll;
  coll.type = MB_DT_LIST;
  mb_init_coll(s, l, &coll);
  mb_value_t mb_feedback_index;
  mb_value_t feedback_value;
  int feedback_index = 0;

  for (unsigned int i = 0; i < strHashedBlock.length(); i += 2) {
    String byteString = strHashedBlock.substring(i, i + 2);
    unsigned char byte = static_cast<unsigned char>(strtoul(byteString.c_str(), nullptr, 16));
    mb_make_int(mb_feedback_index, feedback_index);
    mb_make_int(feedback_value, static_cast<int>(byte));
    mb_set_coll(s, l, coll, mb_feedback_index, feedback_value);
    feedback_index++;
  }
  // Push the comparison result onto the stack
  mb_check(mb_push_value(s, l, coll));
  return result;
}
/*---------*/
static int _sha256H(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;

  mb_check(mb_attempt_open_bracket(s, l));

  char* m;

  mb_check(mb_pop_string(s, l, &m));
  mb_check(mb_attempt_close_bracket(s, l));

  String strHexData = String(m);
  String binaryData = "";
  for (unsigned int i = 0; i < strHexData.length(); i += 2) {
    String byteString = strHexData.substring(i, i + 2);
    char byte = static_cast<char>(strtoul(byteString.c_str(), nullptr, 16));
    binaryData += byte;
  }

  String strRes = CGridShell::GetInstance().sha256HW(binaryData);
  char buf[strRes.length()];
  sprintf(buf, "%s", strRes.c_str());

  mb_check(mb_push_string(s, l, mb_memdup(buf, (unsigned)(strlen(buf) + 1))));
  return result;
}
/*---------*/
static int _sha256(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;

  mb_check(mb_attempt_open_bracket(s, l));

  char* m;
  mb_check(mb_pop_string(s, l, &m));
  mb_check(mb_attempt_close_bracket(s, l));

  String strRes = CGridShell::GetInstance().sha256HW(m);
  char buf[strRes.length()];
  sprintf(buf, "%s", strRes.c_str());

  mb_check(mb_push_string(s, l, mb_memdup(buf, (unsigned)(strlen(buf) + 1))));
  return result;
}
/*---------*/
static int _readline(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_attempt_close_bracket(s, l));
  String strRed = CGridShell::GetInstance().ReadFileLine();
  const char* cstrRed = strRed.c_str();
  mb_check(mb_push_string(s, l, mb_memdup(cstrRed, strlen(cstrRed) + 1)));
  return result;
}
/*---------*/
static int _read(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  int_t iStart = 0;
  int_t iCount = 0;
  char* m;

  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_pop_int(s, l, &iStart));
  mb_check(mb_pop_int(s, l, &iCount));
  mb_check(mb_pop_string(s, l, &m));
  mb_check(mb_attempt_close_bracket(s, l));
  String strFileName = String(m);

  // Reach to the default telemetry file
  String strRed = "";
  if (strFileName.isEmpty())
    strRed = CGridShell::GetInstance().ReadFile(iStart, iCount);
  else
    strRed = CGridShell::GetInstance().ReadFile(iStart, iCount, strFileName);

  const char* cstrRed = strRed.c_str();
  mb_check(mb_push_string(s, l, mb_memdup(cstrRed, strlen(cstrRed) + 1)));
  return result;
}
/*---------*/
static int _seconds(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_attempt_close_bracket(s, l));
  mb_check(mb_push_int(s, l, static_cast<int>(millis() / 1000)));
  return result;
}
/*---------*/
static int _tsize(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  int_t iSize = 0;
  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_attempt_close_bracket(s, l));
  File fTele = SPIFFS.open(GNODE_TELEMETRY_FILENAME, "r");
  if (fTele) {
    iSize = fTele.size();
    fTele.close();
  }
  mb_check(mb_push_int(s, l, iSize));
  return result;
}
/*---------*/
static int _del(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  mb_check(mb_attempt_open_bracket(s, l));
  char* m;
  mb_check(mb_pop_string(s, l, &m));
  mb_check(mb_attempt_close_bracket(s, l));
  CGridShell::GetInstance().Delete(String(m));
  mb_check(mb_push_int(s, l, 0));
  return result;
}
/*---------*/
static int _write(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;

  char* cFilename;
  char* cText;

  // Pop variables
  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_pop_string(s, l, &cFilename));
  mb_check(mb_pop_string(s, l, &cText));
  mb_check(mb_attempt_close_bracket(s, l));

  // We don't append chunks
  int_t iSuccess = CGridShell::GetInstance().Write(String(cFilename), String(cText), false);

  mb_check(mb_push_int(s, l, iSuccess));
  return result;
}
/*---------*/
static int _fmd5(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;

  mb_check(mb_attempt_open_bracket(s, l));

  char* m;

  mb_check(mb_pop_string(s, l, &m));
  mb_check(mb_attempt_close_bracket(s, l));

  String strRes = CGridShell::GetInstance().GetMD5(m);

  char buf[strRes.length()];
  sprintf(buf, "%s", strRes.c_str());
  mb_check(mb_push_string(s, l, mb_memdup(buf, (unsigned)(strlen(buf) + 1))));
  return result;
}
/*---------*/
static int _download(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;
  char* cFilename;

  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_pop_string(s, l, &cFilename));
  mb_check(mb_attempt_close_bracket(s, l));

  int_t uiBytesWritten = CGridShell::GetInstance().GetTelemetry(String(cFilename));
  mb_check(mb_push_int(s, l, uiBytesWritten));
  return result;
}
/*---------*/
static int _resetfilepos(struct mb_interpreter_t* s, void** l) {

  int result = MB_FUNC_OK;
  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_attempt_close_bracket(s, l));
  CGridShell::GetInstance().ResetFilePosition();
  mb_check(mb_push_int(s, l, 1));
  return result;
}

#endif
