#include "CNode.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CNode::CNode()
{
    m_bReady = false;
    m_pSSL = NULL;
    m_Socket = -1; 
    m_uiLastHB = time(0) - 5;  
    m_uiStartTime = 0;   
    m_uiTaskID = 0;
    m_uiValidator = 0;
    m_strUserHash = ""; 
    m_strBuffer = "";
    m_strIP = "";
    m_strHash = "";   
    m_uiExecFlag = 1;
    m_uiIdentTime = 0;    
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Return if this node has a chunk id
//
// -----------------------------------------------------------------------------
bool CNode::HasChunk(const uint64_t& ruiSeqID)
{
    auto search = m_vChunks.find(ruiSeqID);
    
    if (search != m_vChunks.end()) return true;

    return false;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Remove all chunks from this node
//
// -----------------------------------------------------------------------------
void CNode::DelAllChunks()
{
    m_vChunks.clear();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Return count of chunks
//
// -----------------------------------------------------------------------------
uint32_t CNode::GetChunksCount()
{
    return m_vChunks.size();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Delete a chunk
//
// -----------------------------------------------------------------------------
void CNode::DelChunk(const uint64_t& ruiSeqID)
{ 
    m_vChunks.erase(ruiSeqID);
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Add a new chunk
//
// -----------------------------------------------------------------------------
void CNode::AddChunk(const uint64_t& ruiSeqID)
{
    m_vChunks.insert(ruiSeqID);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Is it having a task assigned?
//
// -----------------------------------------------------------------------------
bool CNode::IsFree()
{
    if(IsExecutingTasks() && GetTaskID() == 0)
        return true;

    return false;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Mark ready for commands
//
// -----------------------------------------------------------------------------
void CNode::SSLReady()
{
    m_bReady = true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : SSL Handshake completed?
//
// -----------------------------------------------------------------------------
bool CNode::IsSSLReady()
{
    return m_bReady;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Is this node allowed to execute tasks?
//
// -----------------------------------------------------------------------------
uint32_t CNode::IsExecutingTasks()
{
    return m_uiExecFlag;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Set execution flag
//
// -----------------------------------------------------------------------------
void CNode::SetExecFlag(const uint32_t& ruiValue)
{
    m_uiExecFlag = ruiValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Ident timestamp
//
// -----------------------------------------------------------------------------
void CNode::SetIdentTime(const uint32_t& ruiValue)
{
    m_uiIdentTime = ruiValue;
}   
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Validator flag
//
// -----------------------------------------------------------------------------
void CNode::SetValidatorFlag(const uint32_t& ruiFlag)
{
    m_uiValidator = ruiFlag;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Is this node a validator?
//
// -----------------------------------------------------------------------------
uint32_t CNode::IsValidator()
{
    return m_uiValidator;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Force Disconnect
//
// -----------------------------------------------------------------------------
void CNode::Disconnect()
{  
    close(m_Socket);
    m_Socket = -1; 
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Return id of the task assigned
//
// -----------------------------------------------------------------------------
uint32_t CNode::GetTaskID()
{
    return m_uiTaskID;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Set id of a task assigned
//
// -----------------------------------------------------------------------------
void CNode::SetTaskID(const uint32_t& ruID)
{   
    m_uiTaskID = ruID;
    m_uiStartTime = time(0);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Set node version
//
// -----------------------------------------------------------------------------
void CNode::SetVersion(const std::string& rstrVersion)
{
    m_strVersion = rstrVersion;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Set node ident
//
// -----------------------------------------------------------------------------
void CNode::SetIdent(const std::string& rstrMAC)
{
    m_strMAC = rstrMAC;
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Set node socket
//
// -----------------------------------------------------------------------------
void CNode::SetSocket(const int32_t& ruiSocket)
{
    m_Socket = ruiSocket;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Set node IP
//
// -----------------------------------------------------------------------------
void CNode::SetIP(const std::string& rstrIP)
{
    m_strIP = rstrIP;
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Set node user hash
//
// -----------------------------------------------------------------------------
void CNode::SetUserHash(const std::string& rstrUser)
{
    m_strUserHash = rstrUser;
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Return user hash
//
// -----------------------------------------------------------------------------
std::string CNode::GetUserHash()
{
    return m_strUserHash;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Return IP
//
// -----------------------------------------------------------------------------
std::string CNode::GetIP()
{
    return m_strIP;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Return version
//
// -----------------------------------------------------------------------------
std::string CNode::GetVersion()
{
    return m_strVersion;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Return ident
//
// -----------------------------------------------------------------------------
std::string CNode::GetIdent()
{
    return m_strMAC;
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Remove job
//
// -----------------------------------------------------------------------------
void CNode::ClearJob()
{   
    SetTaskID(0);
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Return task start time
//
// -----------------------------------------------------------------------------
uint32_t CNode::GetStartTime()
{
    return m_uiStartTime;
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Tick the node logic
//
// -----------------------------------------------------------------------------
bool CNode::Tick()
{
    if(m_Socket == -1)
        return false;
 
    fd_set readfds;
    fd_set errorfds;
    timeval timeout;
    
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    
    FD_ZERO(&readfds);
    FD_ZERO(&errorfds);
    
    
    FD_SET(m_Socket, &readfds);
    FD_SET(m_Socket, &errorfds);
    
    int32_t iMaxSocketDescriptor = m_Socket;
    
    int32_t iRet = select(iMaxSocketDescriptor + 1, &readfds, NULL, &errorfds, &timeout);
    
    if (iRet > 0)
    {
        
        if (FD_ISSET(m_Socket, &errorfds))
        {
            Disconnect();

            return false;
        }
        
        if (FD_ISSET(m_Socket, &readfds))
        {
            
            int32_t     iBytesRead;
            char        cBuffer[CLIENT_MAX_BUFFER];
             
            iBytesRead = SSL_read(m_pSSL,cBuffer,CLIENT_MAX_BUFFER); 

            // Existing data handling logic
            if (iBytesRead > 0) 
            {
                // Check if the buffer will overflow before appending
                if (m_strBuffer.size() + iBytesRead > CLIENT_MAX_BUFFER) {
                    m_strBuffer.clear();
                    return false;
                }

                m_strBuffer.append(cBuffer, iBytesRead);

                std::string::size_type index;
                while ((index = m_strBuffer.find("\r\n")) != std::string::npos) {
                    std::string msg = m_strBuffer.substr(0, index);

                    // Get rid of special characters  
                    CSystemUtil::RemoveSpecialCharacters(msg, ALLOWED_CHARACTERS);

                    m_strBuffer.erase(0, index + 2);
                    m_vCommands.push_back(msg);
                }
            }
            // Client has closed the connection
            else if(iBytesRead == 0)
            {
                return false;
            }       
        } 
    }
    
    return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Set node hash
//
// -----------------------------------------------------------------------------
void CNode::SetHash(const std::string& rstrHash)
{
    m_strHash = rstrHash;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Get node hash
//
// -----------------------------------------------------------------------------
std::string CNode::GetHash()
{
    return m_strHash;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Return last HB
//
// -----------------------------------------------------------------------------
uint32_t CNode::GetHeartBeat()
{
    return m_uiLastHB;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Update last HB
//
// -----------------------------------------------------------------------------
void CNode::Pong()
{
    m_uiLastHB = time(0);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Remove commands buffer
//
// -----------------------------------------------------------------------------
void CNode::ClearCommands()
{
    m_vCommands.clear();
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Return commands buffer
//
// -----------------------------------------------------------------------------
std::vector<std::string> CNode::GetCommands()
{
    return m_vCommands;
}   
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Set SSL Pointer
//
// -----------------------------------------------------------------------------
void CNode::SetSSL(SSL* pSSL)
{
    m_pSSL = pSSL;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : Get SSL Pointer
//
// -----------------------------------------------------------------------------
SSL* CNode::GetSSL()
{
    return m_pSSL;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CNode
//  - Prototype :
//
//  - Purpose   : DTOR
//
// -----------------------------------------------------------------------------
CNode::~CNode()
{
    if(m_pSSL != NULL)
        SSL_free(m_pSSL);

    Disconnect();

    m_vCommands.clear();
}