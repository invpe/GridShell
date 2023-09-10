// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : GridShell Arduino Library  https://www.gridshell.net/
//
// MIT License
//
// Copyright (c) 2022-2023 GridShell.net
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
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include "SPIFFS.h"
#include "my_basic.hpp"
#include "mbedtls/base64.h"
#include "MD5Builder.h"
#include "CBigInteger.h"
/*---------*/
#define GNODE_WRITE_MAX 512
#define GNODE_READ_MAX 512
#define GNODE_PING_TIME 30000
#define GNODE_RECON_TIMER (1000 * 60)
#define GNODE_POOL_PORT 1911
#define GNODE_RET_TERMINATED 777
#define GNODE_FIRMWARE_URL "https://github.com/invpe/GridShell/releases/latest/download/latest.bin"
#define GNODE_TASK_SERVER_NAME "https://api.gridshell.net/scripts/"
#define GNODE_FS_SERVER "https://api.gridshell.net/fs/"
#define GNODE_FILE_PREFIX "GS"
#define GNODE_SERVER "work.gridshell.net"
#define GNODE_VERSION "07"
#define GNODE_TELEMETRY_FILENAME "/"GNODE_FILE_PREFIX"TELEMETRY"
/*---------*/
// Enable to dump debug informations to the serial
#define GNODE_DEBUG 1
#ifdef GNODE_DEBUG
#define GDEBUG Serial.println
#else
#define GDEBUG void
#endif
/*---------*/
class CGridShell {
  public:
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
    bool Init(const String& strUsername);
    uint32_t GetTaskTimeout();
    uint32_t GetTaskStartTime();
    void Pong();
    void Tick();
    bool Connected();
    void Stop();
    void RegisterEventCallback(void (*pFunc)(uint8_t));

    // MyBasic Exposed Methods
    bool Write(const String& rstrName, const String& rstrWhat, const bool& bAppend);
    void Delete(const String& rstrName);
    String EncodeBase64(const String& strString);
    String DecodeBase64(const String& strString);
    String GetSHA1(const String& rstrFile);
    String GetMD5(const String& rstrFile);
    String sha1HW(String payload);
    String sha1HW(unsigned char* payload, int len);
    String XOR(const String& toEncrypt, const String& rstrKey);
    ~CGridShell();

  private:
    CGridShell();
    void OTA();
    void CleanFS();
    bool StreamFile(const String& rstrURL, const String& rstrPath);
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

  String strRes =  CGridShell::GetInstance().EncodeBase64(m);
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

  String strRes =  CGridShell::GetInstance().DecodeBase64(m);
  char buf[strRes.length()];
  sprintf(buf, "%s", strRes.c_str());

  mb_check(mb_push_string(s, l, mb_memdup(buf, (unsigned)(strlen(buf) + 1))));
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
static int _sha1(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;

  mb_check(mb_attempt_open_bracket(s, l));

  char* m;

  mb_check(mb_pop_string(s, l, &m));
  mb_check(mb_attempt_close_bracket(s, l));

  String strRes =  CGridShell::GetInstance().sha1HW(m);
  char buf[strRes.length()];
  sprintf(buf, "%s", strRes.c_str());

  mb_check(mb_push_string(s, l, mb_memdup(buf, (unsigned)(strlen(buf) + 1))));
  return result;
}
static int _read(struct mb_interpreter_t* s, void** l) {
  int result = MB_FUNC_OK;

  int_t iStart = 0;
  int_t iCount = 0;

  // Pop variables
  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_pop_int(s, l, &iStart));
  mb_check(mb_pop_int(s, l, &iCount));
  mb_check(mb_attempt_close_bracket(s, l));

  // Returned upon failure
  char buf[] = { 0x00 };

  // Check for start less than 0
  if (iStart < 0) {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read count <= 0");
    return result;
  }

  // Check for read size > 0
  if (iCount <= 0) {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read count <= 0");
    return result;
  }

  // Check for count > chunk limit
  if (iCount > GNODE_READ_MAX) {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read count > GNODE_READ_MAX");
    return result;
  }

  // Check if exists
  File fFile = SPIFFS.open(GNODE_TELEMETRY_FILENAME);
  if (!fFile) {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read file doesnt exist GNODE_TELEMETRY_FILENAME");
    return result;
  }

  // Check for start boundary
  if (iStart > fFile.size()) {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read Start boundary " + String(iStart) + " past " + String(fFile.size()));
    return result;
  }

  // Check for count boundary
  if (iCount > fFile.size()) {
    mb_check(mb_push_string(s, l, mb_memdup(buf, 1)));
    GDEBUG("Read Count boundary " + String(iCount) + " past " + String(fFile.size()));
    return result;
  }

  // Check for start+count boundary vs size
  if (iStart + iCount > fFile.size()) {
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
  if (fTele) {
    iSize = fTele.size();
    GDEBUG("TELEMETRY Size: " + String(fTele.size()));
    fTele.close();
  } else
    GDEBUG("TELEMETRY file doesnt exist");

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
static int _download(struct mb_interpreter_t* s, void** l) {

  int result = MB_FUNC_OK;
  char* cFilename;

  mb_check(mb_attempt_open_bracket(s, l));
  mb_check(mb_pop_string(s, l, &cFilename));
  mb_check(mb_attempt_close_bracket(s, l));

  // Fixed name for overwriting (so we don't keep downloaded files)
  String strPath = GNODE_TELEMETRY_FILENAME;

  GDEBUG("HTTPS Getting " GNODE_FS_SERVER + String(cFilename));

  HTTPClient httpClient;
  uint32_t uiBytesWritten = 0;
  httpClient.begin(GNODE_FS_SERVER + String(cFilename));
  int httpCode = httpClient.GET();

  if (httpCode == HTTP_CODE_OK) {
    File file = SPIFFS.open(strPath, FILE_WRITE);
    if (!file) {
      GDEBUG("Failed to write a file " + strPath);
      httpClient.end();
      return false;
    }

    WiFiClient* stream = httpClient.getStreamPtr();
    uint8_t buffer[128] = { 0 };
    int bytesRead = 0;

    while (httpClient.connected() && (bytesRead = stream->readBytes(buffer, sizeof(buffer))) > 0) {
      file.write(buffer, bytesRead);
      uiBytesWritten += bytesRead;
    }

    file.close();
    GDEBUG(strPath + " Saved");
  } else
    GDEBUG("HTTPS Failed downloading " + String(httpCode));

  httpClient.end();

  mb_check(mb_push_int(s, l, uiBytesWritten));
  return result;
}
#endif
