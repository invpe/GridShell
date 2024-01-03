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
#ifndef __CSERVER__
#define __CSERVER__
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sched.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <sstream>
#include <limits>
#include <iterator>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <functional>
#include <algorithm>
#include <bitset>
#include <map>
#include <set>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <filesystem>
#include <iostream>    
#include <filesystem>
#include <fstream>       
/*----------*/ 
#include <openssl/ssl.h>
#include <openssl/err.h>
/*----------*/  
#include "CSerializer.h"
#include "sha1.hpp"
#include "SHA256.h"
#include "CLogger.h"
#include "CNode.h" 
#include "CUser.h"   
#include "CTask.h"  
#include "CDFS.h"
#include "CCommand.h"
#include "CCommandJob.h"
#include "CCommandAddTask.h"
#include "CCommandPong.h"
#include "CCommandBurn.h"
#include "CCommandSend.h"
#include "CCommandResults.h"
#include "CCommandAddUser.h" 
#include "CCommandGetTask.h"
/*----------*/   
#define SERVER_VERSION          "08"            // Server version   
#define SHA1_LENGTH             40              //  
#define SHA256_LENGTH           64              //   
/*----------*/
class CServer
{
public:  

    CServer();
    ~CServer();
    
    bool Start();
    void Tick();
    void Stop();
    uint32_t GetNumOfNodes(); 
    uint32_t GetNumOfNodesFree(); 
    uint32_t GetNumOfNodesBusy();
    uint32_t GetNumOfValidatorsFree();
    uint32_t GetNumOfValidatorsBusy(); 
    uint32_t GetUsersBalance();     
    uint32_t GetNumOfValidators();      
    uint32_t GetCountOfLostChunks();
    uint32_t GetNodesCountWithHash(const std::string& rstrHash); 
    uint32_t GetNodesCount(const std::string& rstrHash);
    uint32_t GetTaskTimeFor(const std::string& rstrUser);       
    double CalculateTheoreticalValue();

    std::string GetSHA1File(const std::string& rstrFName);
    std::string GetSHA1(const std::string& rstrInput);
    std::string GetSHA256(const std::string& rstrInput);    
    std::string GetConfigParameter(const std::string& rstrParameter);
    CNode *GetNodeWithChunk(const uint64_t& ruiSeq, bool isValidator); 
    std::vector<CNode*> GetNodesWithChunk(const uint64_t& ruiSeq);
    CNode *GetNode(const std::string& rstrHash); 
    CNode *GetNodeByIdent(const std::string& rstrIdent);
    CUser *GetUserByHash(const std::string& rstrHash);  
    CUser *GetUserByName(const std::string& rstrUsername);
    CTask *GetTask(const uint32_t&ruiValue);     
    std::string GetTaskHistory(const uint32_t& ruiTaskId);
    CDFS *GetDFS();
    std::vector<CTask*> GetTasksToValidate();
    std::vector<CTask*> GetTasksToExecute();
    std::vector<CNode*> GetFreeNodes(); 
    std::vector<CNode*> GetFreeValidators();  
    std::vector<CTask*> GetTasksForUser(const std::string& rstrScriptName, const std::string& rstrUserHash);
    std::vector<CTask*> GetTasksWithState(const CTask::eState&reState);
    std::vector<CNode*> GetNodesFor(const std::string& rstrHash);   
    void DisconnectNode(CNode *pNode);
    void DisconnectIP(const std::string& rstrIP);      
    void ThrottleIP(const std::string& rstrIP);
    bool IsIPThrottled(const std::string& rstrIP); 
    bool IsBanned(const std::string& rstrValue);
    bool IsAllowed(const std::string& strIP);     
    bool SendTo(CNode* pClient, const std::string& rstrData); 
    bool SendTo(CNode* pClient, std::vector<uint8_t> rvData); 
    void AddUser(CUser *pUser);
    bool IsCoreTask(const std::string& rstrTaskName);
    bool TransferShellTokens(const std::string& rstrFrom, const std::string& rstrTo, const uint32_t& ruiValue);
    uint32_t SubmitTask(const std::string& rstrScript, const std::string& rstrPayload64, const std::string& rstrOwnerUsername, const bool& bReward, CNode *pAssignTo, const std::vector<std::string>& rvCorePayload );
    std::vector<std::string> GetWordList();
    std::string GenerateUserAlias(const std::string& rstrHash256);
    void AddNodeAvailability(CNode *pNode);

private: 
    CCommand* GetCommand(const std::string& commandType);
    bool LoadConfig(const std::string& configFilePath);
    bool LoadWordlist();        
    double CalculateAvailability(const std::string& nodeId);
    void AwardsTick();
    void HandleConsole(); 

    bool IsNumber(const std::string& rs);  
    void RemoveUnusedAccounts();   
    bool DelUser(const std::string& rstrHash);
    bool PerformSSLHandshake(CNode* pNode);
    //  
    bool LoadUsers();
    bool LoadBanList();
    bool LoadTasks();  
    //    
    bool SaveUsers();  
    bool SaveBanList();  
    bool SaveTasks();
    bool SaveJSON(); 
    //  
 
    void DelegateTask(CNode *pNode, CTask *pTask);  
    void CleanUpTasks();    
    void DFSRetention();
    void DFSReplication(const std::vector<uint64_t>& vCompletedChunks,const std::vector<uint64_t>& vUnassignedChunks);
    void DFSIntegrity(const std::vector<uint64_t>& vAssignedChunks);
    void RotateNodesAvailability();
    float GetUtilization();     
    
    uint32_t m_uiStartTime;   
    uint32_t m_uiLastMinuteEpoch;  
    uint32_t m_uiDFSTimer;
    int32_t  m_Socket;  
    bool    m_bPrintLogs;    
    std::string m_strLastJson; 
    
    // Nodes Vector 
    std::vector<CNode*> m_vpNodes; 
    std::vector<CNode*>::iterator itNode;    

    // Users Map <hash,user>)
    std::map<std::string, CUser*> m_mUsers;
    std::map<std::string, CUser*>::iterator itUsers;

    // Throttle Map
    std::map<std::string, uint32_t> m_mIPThrottle;     

    // Ban list (ip or hash, epoch)
    std::map<std::string,uint32_t> m_mBanList;
    std::map<std::string,uint32_t>::iterator m_itBanList; 

    // Tasks vector
    std::vector<CTask*> m_vpTasks;

    // Tasks history
    std::map<uint32_t, std::string> m_mTasksHistory;    

    // Superwords
    std::vector<std::string> m_vstrWordList;      

    // Config
    std::unordered_map<std::string, std::string> m_uConfigParameters;

    // Commands map
    std::map<std::string, std::unique_ptr<CCommand>> m_mCommandsMap;   

    // Core tasks
    std::vector<std::string> m_vCoreTasks; 

    // Nodes Availability map
    std::map<std::string, std::vector<uint32_t>> m_mNodesAvailability;

    // Internal
    uint32_t m_uiTaskIDCounter;
    CDFS    m_DFS; 
    SSL_CTX* m_pSSLCtx;   

};
#endif
 
