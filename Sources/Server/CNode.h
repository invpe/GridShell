// ------------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2022-2023-2024 https://github.com/invpe/GridShell
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
#ifndef __CNode__
#define __CNode__

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_set>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sched.h>
#include <sys/time.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "CSystemUtil.h" 
/*----------*/
#define CLIENT_MAX_BUFFER   20000  
#define ALLOWED_CHARACTERS  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890,.+/="
/*----------*/
class CNode
{
public: 
    CNode();
    ~CNode();
    
    void AddChunk(const uint64_t& ruiSeqID);
    void DelChunk(const uint64_t& ruiSeqID);
    void DelAllChunks();
    void Disconnect(); 
    void SetSocket(const int32_t& ruiSocket);
    void SetSSL(SSL* pSSL);
    void SetIP(const std::string& rstrIP); 
    void SetUserHash(const std::string& rstrUser);   
    void SetHash(const std::string& rstrHash); 
    void SetVersion(const std::string& rstrVersion);
    void SetIdent(const std::string& rstrMAC);       
    void SetTaskID(const uint32_t& ruID);
    void SetValidatorFlag(const uint32_t& ruiFlag);
    void SetArchPool(const std::string& strPool);
    void SetExecFlag(const uint32_t& ruiValue);
    void SetIdentTime(const uint32_t& ruiValue); 
    SSL* GetSSL(); 
    uint32_t IsValidator();
    uint32_t IsExecutingTasks();
    uint32_t GetStartTime();   
    uint32_t GetHeartBeat(); 
    uint32_t GetTaskID();     
    std::vector<std::string> GetCommands();  
    std::string GetHash();
    std::string GetIP();
    std::string GetUserHash(); 
    std::string GetVersion();
    std::string GetIdent(); 
    std::string GetArchPool(); 
    bool HasChunk(const uint64_t& ruiSeqID);
    uint32_t GetChunksCount();

    void ClearJob();  
    void Pong();  
    void SSLReady();  
    void ClearCommands();
    bool Tick();
    bool IsFree();
    bool IsSSLReady();
private:   

    bool m_bReady;                      // SSL Handshake accepted
    SSL* m_pSSL;                        // SSL
    int32_t m_Socket;                      // Socket descriptor
    uint32_t m_uiValidator;             // Validator flag
    uint32_t m_uiExecFlag;               // Does exec tasks flag
    uint32_t m_uiStartTime;             // Task start time   
    uint32_t m_uiIdentTime;             // When ident happened
    uint32_t m_uiLastHB;              // Last PONG 
    uint32_t m_uiTaskID;              // Currently assigned task (0 = none)  
    std::string m_strHash;             // Unique (private) hash id
    std::string m_strVersion;           // Version
    std::string m_strMAC;               // Mac
    std::string m_strIP;                // IP
    std::string m_strUserHash;          // User id of this node
    std::string m_strBuffer;            // Buffer of data
    std::unordered_set<uint64_t> m_vChunks; // Set of chunks this node hosts
    std::vector<std::string> m_vCommands;   // Buffer of data split to commands
};
#endif
