#include "CServer.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CServer::CServer()
{
    m_Socket = -1;
    m_pSSLCtx = nullptr;    
    m_bPrintLogs = true;
    m_uiDFSTimer = time(0);
    m_uiLastMinuteEpoch = time(0);
    m_uiStartTime = time(0);
    m_uiTaskIDCounter = 1;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Start sequence
//
// -----------------------------------------------------------------------------
bool CServer::Start()
{
    signal(SIGPIPE, SIG_IGN);  
 
    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Loading Server Config");

    if(LoadConfig("config.txt") == false)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Failed, bailing out.");
        return false;
    }

    //
    CLogger::GetInstance().SetPath(m_uConfigParameters["LoggerPath"]);

    // Initialize the SSL library
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    
    // Create a new SSL context using the TLS method
    m_pSSLCtx = SSL_CTX_new(TLS_server_method());

    if (!m_pSSLCtx) {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"SSL context creation failed.");
        return false;
    }

    // Load the server's certificate and private key
    if (SSL_CTX_use_certificate_file(m_pSSLCtx, m_uConfigParameters["SSLCert"].c_str(), SSL_FILETYPE_PEM) <= 0) {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Failed to load server certificate");
        return false;
    }

    if (SSL_CTX_use_PrivateKey_file(m_pSSLCtx,  m_uConfigParameters["SSLKey"].c_str(), SSL_FILETYPE_PEM) <= 0) {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Failed to load server private key");
        return false;
    }

    //
    if ((m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Error creating socket");
        return false;
    }
      
    struct sockaddr_in sin;
    
    int32_t  opt = 1;
    sin.sin_port = htons(std::stoi(m_uConfigParameters["ServerPort"]));
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_family = AF_INET;    
    
    if (setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Error setting SO_REUSEADDR");
        return false;
    }
    
    if (bind(m_Socket, (struct sockaddr*) &sin, sizeof(sin)) == -1)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Error binding socket");
        return false;
    }
    
    if (listen(m_Socket, std::stoi(m_uConfigParameters["MaxSessions"])) == -1)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Error listening on socket");
        return false;
    } 
    

    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Starting");
    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Version: " SERVER_VERSION);
    

    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Loading wordlist");

    if(LoadWordlist() == false)
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,"No wordlist loaded");


    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Loading Tasks");

    if(LoadTasks() == false)
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,"No tasks loaded");
    
    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Loading users database");

    if(LoadUsers() == false)
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,"Not found, starting from scratch ?");   
     
    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Total users count: "+std::to_string(m_mUsers.size()));  
    
    CLogger::GetInstance().Log(SERVER_LOG_INFO ,"Users balance: "+std::to_string(GetUsersBalance()));

    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Loading ban list");
    if(LoadBanList())
       CLogger::GetInstance().Log(SERVER_LOG_INFO ,"Ban list loaded with "+std::to_string(m_mBanList.size())+" entries"); 
    else
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,"Failed to load ban list, starting from scratch?");

    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Loading ShellSync");
    if(m_DFS.Load())
    {
        CLogger::GetInstance().Log(SERVER_LOG_INFO,"Loaded "+std::to_string(m_DFS.GetCountOfFiles())+" telemetry files with "+std::to_string(m_DFS.GetTotalCountOfChunks())+" chunks");        
    }    

    m_uiStartTime = time(0);   

    m_mCommandsMap["JOB"] = std::make_unique<CCommandJob>();
    m_mCommandsMap["ADDT"]= std::make_unique<CCommandAddTask>();
    m_mCommandsMap["PONG"]= std::make_unique<CCommandPong>();
    m_mCommandsMap["BURN"]= std::make_unique<CCommandBurn>();
    m_mCommandsMap["SEND"]= std::make_unique<CCommandSend>();
    m_mCommandsMap["RESULTS"]= std::make_unique<CCommandResults>();
    m_mCommandsMap["ADDUSER"]= std::make_unique<CCommandAddUser>();
    m_mCommandsMap["GETTASK"]= std::make_unique<CCommandGetTask>();
     

    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Rolling");
    
    return true;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Load the wordlist used to generate user hash
//
// -----------------------------------------------------------------------------
bool CServer::LoadWordlist()
{
    std::string line;
    std::ifstream FileWordList(m_uConfigParameters["WordlistFile"]);
    if (!FileWordList.is_open()) {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Can't load wordlist.txt, bailing out");
        return false;
    } 

    while (std::getline(FileWordList, line)) 
        m_vstrWordList.push_back(line);

    FileWordList.close();
    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Wordlist loaded with "+std::to_string(m_vstrWordList.size())+" entries");
    return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Load config file
//
// -----------------------------------------------------------------------------
bool CServer::LoadConfig(const std::string& configFilePath) {

    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR, "Unable to open config file.");
        return false;
    }
    m_uConfigParameters.clear();
    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream lineStream(line);
        std::string key;
        if (std::getline(lineStream, key, '=')) {
            std::string value;
            if (std::getline(lineStream, value)) {
                m_uConfigParameters[key] = value;
                CLogger::GetInstance().Log(SERVER_LOG_INFO,"Config parameter '"+key+"' is "+value);
            }
        }
    }

    configFile.close();
    return true;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Perform SSL handshake with the node
//
// -----------------------------------------------------------------------------
bool CServer::PerformSSLHandshake(CNode* pNode) 
{
    int ssl_accept_result = SSL_accept(pNode->GetSSL());

    if (ssl_accept_result == 1) {
        // Mark SSL ready
        pNode->SSLReady();
        CLogger::GetInstance().Log(SERVER_LOG_INFO, pNode->GetIP() + " SSL HS Accepted");    
        SendTo(pNode,SERVER_VERSION","+std::to_string(m_vpNodes.size())+","+std::to_string(GetTasksToExecute().size())+","+std::to_string(GetTasksToValidate().size())+"\n");
        return true;
    }

    // Handle SSL handshake errors
    int ssl_error = SSL_get_error(pNode->GetSSL(), ssl_accept_result);
    if (ssl_error == SSL_ERROR_WANT_READ || ssl_error == SSL_ERROR_WANT_WRITE) {
        // Non-fatal error, can retry later
        return false;
    }

    // Fatal error, log and throttle IP
    CLogger::GetInstance().Log(SERVER_LOG_INFO, pNode->GetIP() + " SSL HS Failed");   
    ThrottleIP(pNode->GetIP());
    return false;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Server logic tick
//
// -----------------------------------------------------------------------------
void CServer::Tick()
{ 
       
        fd_set readfds;
        timeval timeout;
        
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        
        FD_ZERO(&readfds);
        FD_SET(m_Socket, &readfds);

        int32_t iRet = select(m_Socket + 1, &readfds, nullptr, nullptr, &timeout);
         
        if (iRet > 0)
        {  
            typedef uint32_t socklen_t;
            socklen_t        addrlen;
            sockaddr_in     address;
            
            addrlen = sizeof(struct sockaddr_in);
                
            int32_t new_socket  = accept(m_Socket, (struct sockaddr *)&address, &addrlen); 
            std::string strIP   = GetSHA256(std::string(inet_ntoa(address.sin_addr)));

            if(IsBanned(strIP) || IsIPThrottled(strIP))
            { 
                close(new_socket);
                new_socket =- 1; 
                return;
            }  
      
            CLogger::GetInstance().Log(SERVER_LOG_INFO,strIP+" Is Connected");  
            
            if(m_vpNodes.size() >= std::stoul(m_uConfigParameters["ServerPort"]))
            { 
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING,strIP+" Server Full: "+std::to_string(m_vpNodes.size())+"/"+m_uConfigParameters["MaxSessions"]);
                close(new_socket);
                new_socket = -1;
                return;
            }

            int flags = fcntl(new_socket, F_GETFL, 0);
            fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);

            SSL* pSSLSocket = SSL_new(m_pSSLCtx);
            SSL_set_fd(pSSLSocket, new_socket);

            CNode *pNewNode = new CNode();
            pNewNode->SetSocket(new_socket);
            pNewNode->SetSSL(pSSLSocket);
            pNewNode->SetIP(strIP);
            m_vpNodes.push_back(pNewNode);  

        }
    
    // Iterate nodes and ::Tick
    for (itNode = m_vpNodes.begin();  itNode!=m_vpNodes.end(); )  
    {
        CNode *pNode = *itNode;
        
        if(pNode->Tick() == false)
        { 
            if(pNode->GetHash() == "")
            {
                CLogger::GetInstance().Log(SERVER_LOG_INFO,pNode->GetIP()+" ("+pNode->GetIdent()+") Dummy behaviour, no ident");
                ThrottleIP(pNode->GetIP());
            }         
            else
                DisconnectNode(pNode);
        
            itNode = m_vpNodes.erase(itNode);
            delete pNode;
        }
        else
        {            
            if(pNode->IsSSLReady() == false)
                PerformSSLHandshake(pNode);

             ++itNode;
        } 
    }
    // Iterate and look for timeout
    for (itNode = m_vpNodes.begin(); itNode!=m_vpNodes.end(); )  
    {
        CNode *pNode = *itNode;
        
        if(static_cast<uint32_t>(time(0) - pNode->GetHeartBeat()) >= static_cast<uint32_t>(std::stoul(m_uConfigParameters["SessionTimeout"])) )
        {
            CLogger::GetInstance().Log(SERVER_LOG_INFO,pNode->GetIP()+" ("+pNode->GetIdent()+") Timed out after "+std::to_string(time(0) - pNode->GetHeartBeat())); 

            if(pNode->GetHash() == "")
            {
                CLogger::GetInstance().Log(SERVER_LOG_INFO,pNode->GetIP()+" ("+pNode->GetIdent()+") Dummy behaviour, no ident timedout");
                ThrottleIP(pNode->GetIP());
            }   
            else
                DisconnectNode(pNode);

            itNode = m_vpNodes.erase(itNode);
            delete pNode;      
        }  
        else  ++itNode;
    } 
    // Iterate and validate+execute commands
    for(uint32_t a = 0; a < m_vpNodes.size(); a++)
    {          
        std::vector<std::string> vCommands = m_vpNodes[a]->GetCommands();
 
        if(vCommands.size() > 0)
        { 
            for(uint32_t c = 0;c < vCommands.size(); c++)
            {
                std::vector<std::string> vCommandData = CSystemUtil::SplitString(vCommands[c],',');
 
                if(vCommandData[0] != "PONG")
                    CLogger::GetInstance().Log(SERVER_LOG_INFO,m_vpNodes[a]->GetIP()+" ("+m_vpNodes[a]->GetIdent()+") "+vCommands[c]);

                CCommand* pCommand = GetCommand(vCommandData[0]);

                if(pCommand == nullptr) 
                {
                    CLogger::GetInstance().Log(SERVER_LOG_WARRNING,m_vpNodes[a]->GetIP()+" ("+m_vpNodes[a]->GetIdent()+") Unknown command");
                    ThrottleIP(m_vpNodes[a]->GetIP());                          
                } 
                else 
                {
                    if(pCommand->Validate(vCommandData, *this, m_vpNodes[a])) 
                    { 
                    } 
                    else 
                    {
                        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,m_vpNodes[a]->GetIP()+" ("+m_vpNodes[a]->GetIdent()+") Failed to validate command");
                        // SendTo(command[0]+",BAD") - can be an option
                        ThrottleIP(m_vpNodes[a]->GetIP());      
                    }
                }
            } 
            m_vpNodes[a]->ClearCommands();
        }   
    } 
     
    // ************************************
    // TASK TIMEOUT
    // ************************************ 
    std::vector<CTask*> vpTaskTimeout = GetTasksWithState(CTask::eState::TASK_STATE_RUNNING);

    for(uint32_t x = 0; x < vpTaskTimeout.size();x++)
    { 
        if(static_cast<uint32_t>(time(0)) - vpTaskTimeout[x]->GetStartTime() > (std::stoul(m_uConfigParameters["TaskTimeout"])/1000) - 5 )
        { 
            CLogger::GetInstance().Log(SERVER_LOG_INFO,"Task Time out "+std::to_string(vpTaskTimeout[x]->GetID()));
            
            for(uint32_t c = 0; c < m_vpNodes.size(); c++)
            {
                if(m_vpNodes[c]->GetTaskID() == vpTaskTimeout[x]->GetID()) 
                    DisconnectNode(m_vpNodes[c]); 
            } 
 
            vpTaskTimeout[x]->Fail();
        }
    }  
  
    // ************************************
    // EXECUTION DELEGATION
    // ************************************ 
    std::vector<CTask*> vpTasks         = GetTasksToExecute();  
    std::vector<CNode*> vpFreeWorkers  = GetFreeNodes();
 
    if(vpFreeWorkers.size()> 0 && vpTasks.size()>0)
    {     

        std::random_device rd;
        std::mt19937 rng(rd()); 
        std::shuffle(vpFreeWorkers.begin(), vpFreeWorkers.end(), rng);
   
        CUser *pUser = GetUserByHash(vpFreeWorkers.front()->GetUserHash());
 
        if(vpTasks.front()->IsFailing(vpFreeWorkers.front()->GetHash()) == false)
        { 
            uint32_t uiRoll = rand()%100;

            if(uiRoll <= pUser->GetUPF()) 
                DelegateTask(vpFreeWorkers.front(), vpTasks.front());   
        }  
    }  

    // ************************************
    // VALIDATION MECHANISM 
    // ************************************ 
    std::vector<CTask*> vpValidateT     = GetTasksToValidate();
    std::vector<CNode*> vpValidators    = GetFreeValidators();

    if(vpValidators.size()>0 && vpValidateT.size()>0)
    {    

        std::random_device rd;
        std::mt19937 rng(rd()); 
        std::shuffle(vpValidators.begin(), vpValidators.end(), rng);

        CTask *pTaskToValidate      = vpValidateT.front();
        CNode *pValidatorAssigned   = vpValidators.front(); 

        // DFS MD5 Integrity Check validation needs to happen on a validator that has the chunk stored
        if(pTaskToValidate->GetScript() == "md5dfs")
        {
            // Obtain the validator node name from the vCore payload
            std::vector<std::string> vCorePayload = pTaskToValidate->GetCorePayload();

            // Find the validator with this chunk
            CNode *pValidatorWithChunk  = GetNodeByIdent(vCorePayload[0]);

            // Validator is available,free and has the chunk
            if(pValidatorWithChunk != nullptr && pValidatorWithChunk->IsFree() && pValidatorWithChunk->IsValidator())
            {
                CLogger::GetInstance().Log(SERVER_LOG_INFO,"DFS Integrity Check task "+std::to_string(pTaskToValidate->GetID())+" assigned to validator "+pValidatorWithChunk->GetIdent());
                pValidatorAssigned = pValidatorWithChunk;
            }
            else
            {
                // What if at that stage validator is offline or busy ? we fail the task for now.
                CLogger::GetInstance().Log(SERVER_LOG_INFO,"DFS Integrity Check task "+std::to_string(pTaskToValidate->GetID())+" failed, validator not available");
               
                pTaskToValidate->Fail();
                pValidatorAssigned=nullptr;
                pTaskToValidate=nullptr;
            }
        } 

        // Only if valid
        if(pValidatorAssigned!=nullptr && pTaskToValidate!=nullptr)
        {
            CLogger::GetInstance().Log(SERVER_LOG_INFO,vpValidators.front()->GetIP()+" ("+pValidatorAssigned->GetIdent()+") Validate "+std::to_string(pTaskToValidate->GetID()));         
            DelegateTask(pValidatorAssigned, pTaskToValidate); 
        }
    }
  
    // Remove completed / failed tasks, add to history 
    CleanUpTasks(); 

    
    // One minute
    if(time(0) > m_uiLastMinuteEpoch)
    {            

        AwardsTick();

        RemoveUnusedAccounts(); 

        if(SaveTasks() == false)
            CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Failed to save tasks"); 

        if(SaveJSON() == false)
            CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Failed to save current json"); 
        
        if(SaveBanList() == false)
            CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Failed to save banlist"); 

        if(SaveUsers() == false)
            CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Failed to save user data"); 

        if(m_DFS.Save() == false)
            CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Failed to save DFS"); 

        RotateNodesAvailability();

        CLogger::GetInstance().Log(SERVER_LOG_INFO,"Nodes:"+std::to_string(m_vpNodes.size())+ 
            " Tasks:"+std::to_string(m_vpTasks.size())+
            "/R"+std::to_string(GetTasksWithState(CTask::eState::TASK_STATE_RUNNING).size())+
            "/C"+std::to_string(GetTasksWithState(CTask::eState::TASK_STATE_COMPLETED).size())+
            "/V"+std::to_string(GetTasksWithState(CTask::eState::TASK_STATE_VALIDATED).size())+
            "/F"+std::to_string(GetTasksWithState(CTask::eState::TASK_STATE_FAILED).size())+ 
            " Bal:"+std::to_string(GetUsersBalance())+
            " Ban:"+std::to_string(m_mBanList.size())+
            " Thr:"+std::to_string(m_mIPThrottle.size())+
            " Users:"+std::to_string(m_mUsers.size())+ 
            " DFS:"+std::to_string(m_DFS.GetCountOfFiles())+"/"+std::to_string(m_DFS.GetTotalCountOfChunks())+
            " TV:"+std::to_string(CalculateTheoreticalValue())+
            " UT:"+std::to_string(GetUtilization())
            );
        
        m_uiLastMinuteEpoch = time(0) + 60;  
    }
 

    // Keep DFSExecutionTimer above certain level to allow replication tasks to complete,
    // Otherwise we will duplicate replication on existing chunks since task can be executing and we find it for replication once more
    // we visit this logic.
    if(static_cast<uint32_t>(time(0))-m_uiDFSTimer > std::stoul(m_uConfigParameters["DFSExecutionTimer"]))
    {
        std::vector<uint64_t> vCompletedChunks = m_DFS.GetAllCompletedChunks(); 
        
        std::vector<uint64_t> vUnassignedChunks;
        std::vector<uint64_t> vAssignedChunks;

        // Grab chunks unassigned(lost) & assigned to worker nodes
        for(const auto& chunkKey : vCompletedChunks)
        {
            if(GetNodeWithChunk(chunkKey, false) == nullptr)
                vUnassignedChunks.push_back(chunkKey);
            else
                vAssignedChunks.push_back(chunkKey);
        }


        // https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0006-FS.md
        DFSRetention();

        // Replicate
        DFSReplication(vCompletedChunks,vUnassignedChunks);

        // https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0030-GFS_IntegrityCheck.md
        DFSIntegrity(vAssignedChunks);

        m_uiDFSTimer = time(0);
    }

    HandleConsole();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}   
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : DFS Retention
//
// -----------------------------------------------------------------------------
void CServer::DFSRetention()
{
    for (auto& userPair : m_mUsers)
    {
        auto& pClient = userPair.second;
        auto username = pClient->GetUsername();
        auto retention = pClient->GetRetention();
        auto vpUserFiles = m_DFS.GetFilesForUserRetention(username, retention);

        for (auto& pFile : vpUserFiles)
        {
            std::string fileName = pFile->GetName();
            CLogger::GetInstance().Log(SERVER_LOG_INFO, "DFS file retention started: " + fileName);

            for (auto& chunk : pFile->m_vChunks)
            {
                uint64_t chunkSeq = chunk.GetSeq();
                auto vpNodes = GetNodesWithChunk(chunkSeq);

                for (auto &pNode : vpNodes)
                {
                    pNode->DelChunk(chunkSeq);
                    CLogger::GetInstance().Log(SERVER_LOG_INFO, "DFS chunk " + std::to_string(chunkSeq) + " removed from " + pNode->GetIdent());
                }
            }

            m_DFS.RemoveFile(username + fileName, m_uConfigParameters["DFSDirectoryPrefix"]);
            CLogger::GetInstance().Log(SERVER_LOG_INFO, "DFS file retention completed: " + fileName);
        }
    }
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : DFS Replication
//
// -----------------------------------------------------------------------------
void CServer::DFSReplication(const std::vector<uint64_t>& vCompletedChunks,const std::vector<uint64_t>& vUnassignedChunks)
{
    // Here we submit replication to only these chunks that are completed (Filled with DFS_CHUNK_SIZE)
    // We don't request all of them at once, we pick one and replicate in some intervals  
    if(vCompletedChunks.size()>0)
    {  
        CLogger::GetInstance().Log(SERVER_LOG_INFO, "DFS chunks completed "+std::to_string(vCompletedChunks.size())+" to replicate "+std::to_string(vUnassignedChunks.size()));

        if(vUnassignedChunks.size()>0)
        {
            // Submit a task with the chunk, once validated mark node keeping this chunk
            CDFS::tFile *pFile          = m_DFS.GetFileWithChunk(vUnassignedChunks.back());            
            if(pFile != nullptr)
            {    

                CDFS::tFileChunk *pChunk    = pFile->GetChunk(vUnassignedChunks.back());

                // Build a proper filename for SPIFFS
                std::string strFileName = GetConfigParameter("TelemetryPrefix") + GetSHA1(std::to_string(pChunk->GetSeq())); 
                strFileName = strFileName.substr(0,std::stoul(GetConfigParameter("DFSChunkFilenameLength")));

                // Create payload for replicatedfs script 
                std::string strPayload = CSystemUtil::base64_encode( strFileName + "," + CSystemUtil::base64_encode(pChunk->GetContent()) + ",");

                // And store CorePayload for this task for validator  
                std::vector<std::string> vCorePayload;
                vCorePayload.push_back(std::to_string(pChunk->GetSeq()));

                uint32_t uiTaskID = SubmitTask("replicatedfs", strPayload, pFile->GetOwner(),0, nullptr, vCorePayload);
                CLogger::GetInstance().Log(SERVER_LOG_INFO,"DFS replicating Chunk "+std::to_string(pChunk->GetSeq())+" ("+std::to_string(pChunk->GetSize())+"b) for "+pFile->GetOwner()+" TaskID: "+std::to_string(uiTaskID)+" as "+strFileName); 
            }
        }
    } 
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : DFS Integrity checks
//
// -----------------------------------------------------------------------------
void CServer::DFSIntegrity(const std::vector<uint64_t>& vAssignedChunks)
{
    // Get a chunk from worker assigned that has a validator assigned 
    std::vector<uint64_t> vValidatorAssignedChunks;
    for(const auto& chunkKey : vAssignedChunks)
    {
        if(GetNodeWithChunk(chunkKey, true) != nullptr)
            vValidatorAssignedChunks.push_back(chunkKey); 
    }

    // We've got a vector of chunk ids that are worker + validator assigned
    // Push a md5dfs task
    if(vValidatorAssignedChunks.size() > 0)
    {
        // Get a random one
        std::random_device rd;
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<std::size_t> dist(0, vValidatorAssignedChunks.size() - 1);
        std::size_t randomIndex         = dist(gen); 

        // Hate these getters - lots of iterations !
        CDFS::tFile *pChunkFile         = m_DFS.GetFileWithChunk(vValidatorAssignedChunks[randomIndex]);
        CDFS::tFileChunk* pRandomChunk  = pChunkFile->GetChunk(vValidatorAssignedChunks[randomIndex]); 
        CNode *pNodeWithChunk           = GetNodeWithChunk(vValidatorAssignedChunks[randomIndex],false);
        CNode *pValidatorWithChunk      = GetNodeWithChunk(vValidatorAssignedChunks[randomIndex],true);

        // Check if chunk eligible for integrity check
        if(time(0) < pRandomChunk->GetNextIntegrityCheck())
            return;

        // Both nodes available having the same chunk and executor+validator are free to execute 
        // Task is submitted in the name of the file owner 
        // If this fails in anyway, the usual execution chain will follow with restarts etc.
        // Since other nodes dont have this chunk they will return empty payload failing the task.
        if(pChunkFile != nullptr && pNodeWithChunk != nullptr  && pValidatorWithChunk != nullptr && pNodeWithChunk->IsFree() && pValidatorWithChunk->IsFree()  )
        {
            // Add the validator to core payload so we can assign when returned
            std::vector<std::string> vCorePayload; 
            vCorePayload.push_back(pValidatorWithChunk->GetIdent());
            
            // Build a proper filename for SPIFFS
            std::string strFileName = GetConfigParameter("TelemetryPrefix") + GetSHA1(std::to_string(vValidatorAssignedChunks[randomIndex])); 
            strFileName = strFileName.substr(0,std::stoul(GetConfigParameter("DFSChunkFilenameLength")));

            uint32_t uiNewTask = SubmitTask("md5dfs", 
                CSystemUtil::base64_encode(strFileName), 
                pChunkFile->GetOwner(), 
                false, 
                pNodeWithChunk, 
                vCorePayload);

            CLogger::GetInstance().Log(SERVER_LOG_INFO,"DFS Integrity Check on "+
                strFileName+
                " Seq: "+std::to_string(vValidatorAssignedChunks[randomIndex])+
                " hosted on "+pNodeWithChunk->GetIdent()+
                " validator "+pValidatorWithChunk->GetIdent()+
                " task "+std::to_string(uiNewTask));

            pRandomChunk->IntegrityChecked();
        }


    }


}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Nodes Availability 
//
// -----------------------------------------------------------------------------
void CServer::RotateNodesAvailability()
{
    // Rotate nodes availability map 24 hours
    uint32_t currentTime = static_cast<uint32_t>(std::time(nullptr));
    uint32_t twentyFourHoursAgo = currentTime - 86400; 

    for (auto& pair : m_mNodesAvailability) {
        auto& timestamps = pair.second;
        timestamps.erase(
            std::remove_if(
                timestamps.begin(), 
                timestamps.end(), 
                [twentyFourHoursAgo](uint32_t timestamp) { return timestamp < twentyFourHoursAgo; }
            ),
            timestamps.end()
        );
    } 
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Send to a node
//
// -----------------------------------------------------------------------------
bool CServer::SendTo(CNode* pClient, std::vector<uint8_t> rvData)
{       
    if (rvData.empty()) return true;  

    int32_t iBytesSent = SSL_write(pClient->GetSSL(), rvData.data(), rvData.size());

    if (iBytesSent <= 0) {    
        return false;
    }
    
    return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Send to a node
//
// -----------------------------------------------------------------------------
bool CServer::SendTo(CNode* pClient, const std::string& rstrData)
{
    if (rstrData.empty()) return true; 

    int32_t iBytesSent = SSL_write(pClient->GetSSL(), rstrData.data(), rstrData.size());

    if (iBytesSent <= 0) {        
        return false;
    }
    
    return true;
}    
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Load ban list
//
// -----------------------------------------------------------------------------
bool CServer::LoadBanList()
{
    std::ifstream inFile(m_uConfigParameters["BanListFile"], std::ios::binary);
    if (!inFile) {
        return false;
    }

    inFile.seekg(0, std::ios::end);
    std::size_t length = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(length);
    if (!inFile.read(reinterpret_cast<char*>(buffer.data()), length)) {
        return false; // Error reading file
    }

    CSerializer _Ser;
    _Ser.Set(buffer);

    uint32_t uiBanCount = _Ser.Read_uint32();
    for (uint32_t x = 0; x < uiBanCount; x++) {
        std::string strHashOrIP = _Ser.Read_string();
        uint32_t uiEpoch = _Ser.Read_uint32();
        m_mBanList[strHashOrIP] = uiEpoch;
    }

    return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Save ban list
//
// -----------------------------------------------------------------------------
bool CServer::SaveBanList()
{
    std::ofstream outFile(m_uConfigParameters["BanListFile"], std::ios::binary);
    if (!outFile) {
        return false;
    }

    CSerializer _Ser;
    _Ser.Write_uint32(m_mBanList.size());
    for (const auto& banEntry : m_mBanList) {
        _Ser.Write_string(banEntry.first);
        _Ser.Write_uint32(banEntry.second);
    }


    const std::vector<uint8_t>* pBuffer = _Ser.GetBufferPtr();

    // Ensure that pBuffer is not a nullptr
    if (pBuffer == nullptr) return false;

    // Directly write the buffer to the file
    outFile.write(reinterpret_cast<const char*>(pBuffer->data()), pBuffer->size());
    
  
    return outFile.good(); // Check for write success
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Save users
//
// -----------------------------------------------------------------------------
bool CServer::SaveUsers()
{
    std::ofstream outFile(m_uConfigParameters["UsersListFile"], std::ios::binary);
    if (!outFile) {
        return false;
    }

    CSerializer _Ser;
    _Ser.Write_uint32(m_mUsers.size());
    for (const auto& userEntry : m_mUsers) {
        CUser *pUser = userEntry.second;
        std::vector<uint8_t> vUser = pUser->GetSerialized();
        _Ser.Add(vUser);
    }

    const std::vector<uint8_t>* pBuffer = _Ser.GetBufferPtr();

    // Ensure that pBuffer is not a nullptr
    if (pBuffer == nullptr) return false;

    // Directly write the buffer to the file
    outFile.write(reinterpret_cast<const char*>(pBuffer->data()), pBuffer->size());
    return outFile.good();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Load users
//
// -----------------------------------------------------------------------------
bool CServer::LoadUsers()
{
    std::ifstream inFile(m_uConfigParameters["UsersListFile"], std::ios::binary);
    if (!inFile) {
        return false;
    }

    inFile.seekg(0, std::ios::end);
    std::size_t length = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(length);
    if (!inFile.read(reinterpret_cast<char*>(buffer.data()), length)) {
        return false; // Error reading file
    }

 
    CSerializer _Ser;
    _Ser.Set(buffer);
    uint32_t uiUsersCount   = _Ser.Read_uint32();
  
    for(uint32_t a =0 ; a < uiUsersCount; a++)
    { 
        CUser *pNewUser = new CUser();

        std::string strUname        = _Ser.Read_string();
        std::string strHash         = _Ser.Read_string();    
        uint32_t uiBalance          = _Ser.Read_uint32();  
        uint32_t uiCreatedTime      = _Ser.Read_uint32();
        uint32_t uiLastSeenTime     = _Ser.Read_uint32(); 
        uint32_t uiGood             = _Ser.Read_uint32();   
        uint32_t uiBad              = _Ser.Read_uint32();  
        uint32_t uiValidator        = _Ser.Read_uint32();
        uint32_t uiFilesystem       = _Ser.Read_uint32(); 
        uint32_t uiRetention        = _Ser.Read_uint32();  
        uint32_t uiMaxFiles         = _Ser.Read_uint32();
        uint32_t uiMaxFileSize      = _Ser.Read_uint32();
        uint32_t uiTotals           = _Ser.Read_uint32();
        uint32_t uiSubmitTotals     = _Ser.Read_uint32();
  
 
        pNewUser->SetUsername(strUname);
        pNewUser->SetHash(strHash); 
        pNewUser->SetBalance(uiBalance);  
        pNewUser->SetCreatedTime(uiCreatedTime);
        pNewUser->SetLastSeen(uiLastSeenTime);  
        pNewUser->SetValidator(uiValidator);
        pNewUser->SetFilesystem(uiFilesystem);
        pNewUser->SetRetention(uiRetention);
        pNewUser->SetMaxFiles(uiMaxFiles);
        pNewUser->SetMaxFileSize(uiMaxFileSize);
        pNewUser->SetTotals(uiTotals);
        pNewUser->SetSubmitCount(uiSubmitTotals); 

        // TODO: I am not taking into consideration the order of UPF          
        for(uint32_t x = 0; x < uiBad; x++)
            pNewUser->AddUPF(0,std::stoul(m_uConfigParameters["UPFExecutionsCount"])); 
        for(uint32_t x = 0; x < uiGood; x++)
            pNewUser->AddUPF(1,std::stoul(m_uConfigParameters["UPFExecutionsCount"]));
 
        m_mUsers[strHash] = pNewUser;   
    }
    
    return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Get count of all nodes
//
// -----------------------------------------------------------------------------
uint32_t CServer::GetNodesCount(const std::string& rstrHash)
{
    uint32_t iTotal = 0;
    for(const auto& node : m_vpNodes) {
        if(node && node->GetUserHash() == rstrHash) {
            iTotal++;
        }
    }
    return iTotal;
}    
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return a list of nodes for specific user hash
//
// -----------------------------------------------------------------------------
std::vector<CNode*> CServer::GetNodesFor(const std::string& rstrHash)
{
 std::vector<CNode*> vReturn;
    for(const auto& node : m_vpNodes) {
        if(node && node->GetUserHash() == rstrHash) {
            vReturn.push_back(node);
        }
    }
    return vReturn;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Dump a JSON file
//
// -----------------------------------------------------------------------------
bool CServer::SaveJSON()
{ 
    std::string strOutput ="";
    strOutput = "{\n";
    strOutput += "\"CPU\": "+std::to_string(CSystemUtil::GetCPUUsage())+",\n";
    strOutput += "\"Starttime\": "+std::to_string(m_uiStartTime)+",\n";  
    strOutput += "\"Epoch\": "+std::to_string(time(0))+",\n"; 
    strOutput += "\"Version\": \""+std::string(SERVER_VERSION)+"\",\n";    
    strOutput += "\"MaxNodesPerDevice\": "+m_uConfigParameters["MaxNodesPerDevice"]+",\n";      
    strOutput += "\"Connections\": "+std::to_string(m_vpNodes.size())+",\n"; 
    strOutput += "\"Nodes\": "+std::to_string(GetNumOfNodes())+",\n"; 
    strOutput += "\"NodesFree\": "+std::to_string(GetNumOfNodesFree())+",\n";
    strOutput += "\"NodesWorking\": "+std::to_string(GetNumOfNodesBusy())+",\n";
    strOutput += "\"Validators\": "+std::to_string(GetNumOfValidators())+",\n"; 
    strOutput += "\"ValidatorsFree\": "+std::to_string(GetNumOfValidatorsFree())+",\n"; 
    strOutput += "\"ValidatorsWorking\": "+std::to_string(GetNumOfValidatorsBusy())+",\n"; 
    strOutput += "\"MaxNodes\": "+m_uConfigParameters["MaxSessions"]+",\n"; 
    strOutput += "\"TV\": "+std::to_string(CalculateTheoreticalValue())+",\n";
    strOutput += "\"UsersCount\": "+std::to_string(m_mUsers.size())+",\n";   ;
    strOutput += "\"TotalBalance\": "+std::to_string(GetUsersBalance())+",\n";  
    strOutput += "\"TaskTimeout\": "+m_uConfigParameters["TaskTimeout"]+",\n";       
    strOutput += "\"Throttled\": "+std::to_string(m_mIPThrottle.size())+",\n";  
    strOutput += "\"TasksCount\": "+std::to_string(m_vpTasks.size())+",\n";   
    strOutput += "\"TasksNew\": "+std::to_string(GetTasksWithState(CTask::eState::TASK_STATE_NEW).size())+",\n";  
    strOutput += "\"TasksRunning\": "+std::to_string(GetTasksWithState(CTask::eState::TASK_STATE_RUNNING).size())+",\n";  
    strOutput += "\"TasksFailed\": "+std::to_string(GetTasksWithState(CTask::eState::TASK_STATE_FAILED).size())+",\n";  
    strOutput += "\"TasksCompleted\": "+std::to_string(GetTasksWithState(CTask::eState::TASK_STATE_COMPLETED).size())+",\n";   
    strOutput += "\"TasksValidated\": "+std::to_string(GetTasksWithState(CTask::eState::TASK_STATE_VALIDATED).size())+",\n";    
    strOutput += "\"TotalTasks\": "+std::to_string(m_uiTaskIDCounter)+",\n";     
    strOutput += "\"TelemetryRetention\": "+m_uConfigParameters["DFSDefaultRetentionSeconds"]+",\n"; 
    strOutput += "\"TelemetryCount\": "+std::to_string(m_DFS.GetCountOfFiles())+",\n"; 
    strOutput += "\"TelemetryChunks\": "+std::to_string(m_DFS.GetTotalCountOfChunks())+",\n"; 
    strOutput += "\"TelemetryChunksLost\": "+std::to_string(GetCountOfLostChunks())+",\n"; 
    strOutput += "\"TelemetrySize\": "+std::to_string(m_DFS.GetTotalSize())+",\n";  
    strOutput += "\"Utilization\": "+std::to_string(GetUtilization())+",\n"; 
 
    // Dump Tasks id's only 
    strOutput += "\"Tasks\":[\n";  
    for (uint32_t a = 0; a < m_vpTasks.size(); a++)
    { 
        strOutput += "{\n";  
        strOutput += "\"ID\": \""+std::to_string(m_vpTasks[a]->GetID())+"\"\n"; 

        // End single task
        if(a == m_vpTasks.size()-1)
            strOutput += "}\n";
        else strOutput += "},\n";  
    }
    strOutput += "],\n";


    // Dump telemetry and chunks - too heavy
    //strOutput += m_DFS.GetJson();
    //strOutput += ",\n";

    strOutput += "\"Users\":[\n"; 
    for (itUsers = m_mUsers.begin(); itUsers != m_mUsers.end(); ++itUsers)
    {
        CUser *pClient      = itUsers->second; 
        pClient->UpdateAwards(GetNodesCount(pClient->GetHash()));
        
        strOutput += "{\n";
        strOutput += "\"Username\": \""+pClient->GetUsername()+"\",\n";
        strOutput += "\"Rank\": "+std::to_string(pClient->GetRank())+",\n";
        strOutput += "\"Created\": "+std::to_string(pClient->GetCreatedTime())+",\n";
        strOutput += "\"LastSeen\": "+std::to_string(pClient->GetLastSeenTime())+",\n"; 
        strOutput += "\"Balance\": "+std::to_string(pClient->GetBalance())+",\n"; 
        strOutput += "\"Good\": "+std::to_string(pClient->GetGood())+",\n"; 
        strOutput += "\"Bad\": "+std::to_string(pClient->GetBad())+",\n";  
        strOutput += "\"UPF\": "+std::to_string(pClient->GetUPF())+",\n";
        strOutput += "\"Totals\": "+std::to_string(pClient->GetTotals())+",\n";
        strOutput += "\"Submits\": "+std::to_string(pClient->GetSubmitCount())+",\n";
        strOutput += "\"TelemetryCount\": "+std::to_string(m_DFS.GetCountOfFiles(pClient->GetUsername()))+",\n";   
        strOutput += "\"TelemetryMax\": "+std::to_string(pClient->GetMaxFiles())+",\n";  
        strOutput += "\"TelemetrySize\": "+std::to_string(pClient->GetMaxFileSize())+",\n";
        strOutput += "\"UPFDetais\":\"";
        std::vector<uint32_t> vGB = pClient->GetGoodBad();
        for(uint32_t iCounter =0; iCounter<vGB.size(); iCounter++)
        {
            strOutput+=std::to_string(vGB[iCounter]);
        }
        strOutput+="\",";

        strOutput += "\"Retention\": "+std::to_string(pClient->GetRetention())+",\n"; 
        strOutput += "\"Validator\": "+std::to_string(pClient->IsValidator())+",\n";   
        strOutput += "\"Nodes\": "+std::to_string(GetNodesCount(pClient->GetHash()))+",\n";   
         
        strOutput+= "\"Awards\":[\n";  
        for(uint32_t a = 0; a < CUser::eAwardCategories::AWARD_TOTAL;a++)
        {
            CUser::eAwardMedal eMedal = pClient->GetMedal(static_cast<CUser::eAwardCategories>(a)); 

            strOutput += "{\n";  
            strOutput += "\"Category\": "+std::to_string(a)+",\n"; 
            strOutput += "\"Medal\": "+std::to_string(eMedal)+"\n";

             if (a <CUser::eAwardCategories::AWARD_TOTAL-1)
                strOutput += "},\n";
            else strOutput += "}\n";
        } 
        strOutput += "],\n";
    

        // Add telemetry dump   
        strOutput+= "\"Telemetry\":[\n";
        std::vector<CDFS::tFile*> vpFiles = m_DFS.GetFilesForUser(pClient->GetUsername());

        if(vpFiles.size()>0)
        {
            for(uint32_t z = 0; z < vpFiles.size(); z++)
            {            

                strOutput += "{\n";  
                strOutput += "\"Created\": "+std::to_string(vpFiles[z]->GetCreated())+",\n"; 
                strOutput += "\"Modified\": "+std::to_string(vpFiles[z]->GetUpdated())+",\n";  
                strOutput += "\"Size\": \""+std::to_string(vpFiles[z]->GetSize())+"\",\n";   
                strOutput += "\"Name\": \""+vpFiles[z]->GetOwner()+vpFiles[z]->GetName()+"\"\n";   
                if (z < vpFiles.size()-1)
                    strOutput += "},\n";
                else strOutput += "}\n";
            }
        }        
        strOutput += "],\n";


        // Add nodes dump
        strOutput+= "\"NodesList\":[\n";

        std::vector<CNode*> vNodes = GetNodesFor(pClient->GetHash());
        if(vNodes.size()>0)
        {
            for(uint32_t a = 0; a < vNodes.size(); a++)
            {
                strOutput += "{\n";   
                strOutput += "\"Version\": \""+vNodes[a]->GetVersion()+"\",\n";   
                strOutput += "\"Task\": "+std::to_string(vNodes[a]->GetTaskID())+",\n";                
                strOutput += "\"Validator\": "+std::to_string(vNodes[a]->IsValidator())+",\n"; 
                strOutput += "\"Executor\": "+std::to_string(vNodes[a]->IsExecutingTasks())+",\n"; 
                strOutput += "\"TelemetryChunks\": "+std::to_string(  vNodes[a]->GetChunksCount()  )+",\n"; 
                strOutput += "\"Availability\": "+std::to_string(CalculateAvailability(vNodes[a]->GetIdent()))+",\n";
                strOutput += "\"Ident\": \""+vNodes[a]->GetIdent()+"\"\n";  
                 
                // End single user node
                if(a == vNodes.size()-1)
                    strOutput += "}\n";
                else strOutput += "},\n";
            }
        }
        
        strOutput += "]\n";

        // End single user
        if (std::next(itUsers) != m_mUsers.end())
            strOutput += "},\n";
        else strOutput += "}\n";
    }
    strOutput += "]\n";
    strOutput += "}";
    m_strLastJson = strOutput;



    std::ofstream file("current.json");
    if (!file.is_open()) {
        return false;
    }

    // Write to file
    file << strOutput;
 
    return true;
}   
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return user by hash
//
// -----------------------------------------------------------------------------
CUser *CServer::GetUserByHash(const std::string& rstrHash)
{
    auto it = m_mUsers.find(rstrHash);
    return it != m_mUsers.end() ? it->second : nullptr;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return user by name
//
// -----------------------------------------------------------------------------
CUser* CServer::GetUserByName(const std::string& rstrUsername)
{
    auto it = std::find_if(m_mUsers.begin(), m_mUsers.end(), 
                           [&rstrUsername](const auto& pair) {
                               return pair.second->GetUsername() == rstrUsername;
                           });

    return (it != m_mUsers.end()) ? it->second : nullptr;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Force IP to disconnect
//
// -----------------------------------------------------------------------------
void CServer::DisconnectIP(const std::string& rstrIP)
{
    std::for_each(m_vpNodes.begin(), m_vpNodes.end(), [&](CNode* pNode) {
        if (pNode && pNode->GetIP() == rstrIP) {
            DisconnectNode(pNode);
        }
    });
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return count of nodes with a specific hash
//
// -----------------------------------------------------------------------------
uint32_t CServer::GetNodesCountWithHash(const std::string& rstrHash)
{
      return std::count_if(m_vpNodes.begin(), m_vpNodes.end(),
                         [&rstrHash](CNode* node) { return node->GetHash() == rstrHash; });

} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Disconnect node
//
// -----------------------------------------------------------------------------
void CServer::DisconnectNode(CNode *pNode)
{
    assert(pNode);  

    
    //
    CLogger::GetInstance().Log(SERVER_LOG_INFO,pNode->GetIP()+" ("+pNode->GetIdent()+") Disconnected");
 
    if(pNode->GetTaskID()>0 && pNode->IsValidator() == 0)
    {   
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,"Node "+pNode->GetIdent()+" leaving Task "+std::to_string(pNode->GetTaskID())+", restarted.");
        
        CTask *pTask = GetTask(pNode->GetTaskID());
        if(pTask)
        {
            pTask->Restart(); 
        }
    }
 
    uint32_t uiTotal = pNode->GetChunksCount();
    pNode->DelAllChunks();
    CLogger::GetInstance().Log(SERVER_LOG_INFO,pNode->GetIP()+" ("+pNode->GetIdent()+") Freed "+std::to_string(uiTotal)+" chunks from this node");

    // https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0036-UPFAdjustOnNodeWithChunkDropping.md
    CUser *pUser = GetUserByHash(pNode->GetUserHash());
    if(uiTotal > 0 && m_uConfigParameters["GIP36ChunkDroppedMaxReduction"] != "" && pUser != nullptr)
    {        
        uint32_t uiReduceUPF = std::min(
            static_cast<uint32_t>(std::stoul(m_uConfigParameters["GIP36ChunkDroppedMaxReduction"])),
            std::max(static_cast<uint32_t>(0),uiTotal));
        
        for(uint32_t p = 0; p < uiReduceUPF; p++)
            pUser->AddUPF(0,std::stoul(m_uConfigParameters["UPFExecutionsCount"])); 

        CLogger::GetInstance().Log(SERVER_LOG_INFO,pUser->GetUsername()+" ("+pNode->GetIdent()+") Reduced UPF by "+std::to_string(uiReduceUPF)+"%");
    }

 
    pNode->ClearJob();
    pNode->Disconnect();

    // When dropping, add epoch for node availability calculations
    AddNodeAvailability(pNode);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return total balance of users
//
// -----------------------------------------------------------------------------
uint32_t CServer::GetUsersBalance()
{
    uint32_t uiBalance = 0;
    for (const auto& userPair : m_mUsers) {
        uiBalance += userPair.second->GetBalance();
    }
    return uiBalance;  

}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return count of validators
//
// -----------------------------------------------------------------------------
uint32_t CServer::GetNumOfValidators()
{
    return std::count_if(m_vpNodes.begin(), m_vpNodes.end(), [](CNode* node) {
        return !node->GetUserHash().empty() && node->IsValidator();
    });
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return count of nodes
//
// -----------------------------------------------------------------------------
uint32_t CServer::GetNumOfNodes()
{
    return std::count_if(m_vpNodes.begin(), m_vpNodes.end(), [](CNode* node) {
        return !node->GetUserHash().empty();
    });
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Stop server
//
// -----------------------------------------------------------------------------
void CServer::Stop()
{

    CLogger::GetInstance().Log(SERVER_LOG_WARRNING,"Server going shutdown");
 
    // Disconnect
    for (auto* pNode : m_vpNodes) {
        CLogger::GetInstance().Log(SERVER_LOG_INFO, "Disconnecting " + pNode->GetIP());
        DisconnectNode(pNode);
        delete pNode;
    }
    m_vpNodes.clear();


    //
    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Freeing SSL");
    SSL_CTX_free(m_pSSLCtx);

    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Closing socket");
    close(m_Socket);

    CLogger::GetInstance().Log(SERVER_LOG_WARRNING,"Server Saving state");

    if(SaveTasks() == false)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"- failed to save tasks data"); 
    } 

    if(SaveUsers() == false)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"- failed to save user data"); 
    }

    if(SaveJSON() == false)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"- failed to save current json"); 
    }

    if(SaveBanList() == false)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"- failed to save banlist"); 
    }

    if(m_DFS.Save() == false)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"- failed to save DFS"); 
    }

    // Free User objects
    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Freeing objects - Users");
    for (itUsers = m_mUsers.begin(); itUsers != m_mUsers.end(); ++itUsers) {
        delete itUsers->second; 
    }
    m_mUsers.clear();

    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Freeing objects - Tasks");
    for (auto* pTask : m_vpTasks) {
        delete pTask;
    }
    m_vpTasks.clear();


}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Console handling
//
// -----------------------------------------------------------------------------
void CServer::HandleConsole()
{   
     
    static std::string m_strConsoleCommandTemp;

     struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, nullptr, nullptr, &tv);
    if( !FD_ISSET(STDIN_FILENO, &fds) )
    { 
    }
    else 
    {
        int iCharacter = fgetc(stdin);

        if(iCharacter == 0x0A)
        {    
            if(m_strConsoleCommandTemp.size()>=4095)
                CLogger::GetInstance().Log(SERVER_LOG_ERROR ,"Console characters max out! Your command is not parsed properly"); 

            std::vector<std::string> vCommand = CSystemUtil::SplitString(m_strConsoleCommandTemp, ',');

            // Immediate quit with save
            if(vCommand[0]=="QUIT")
            {
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING ,"Console QUIT commanded");
                Stop();
                exit(0);
            } 
            if(vCommand[0]=="RELOAD")
            {
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING ,"Console RELOAD initiated");

                LoadConfig("./config.txt");
            }
            if(vCommand[0]=="DFS")
            {
                m_DFS.Dump();
            }                
            // Immediate quit with save
            if(vCommand[0]=="LOG")
            {
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING ,"Console LOG commanded");
                m_bPrintLogs = !m_bPrintLogs;

                CLogger::GetInstance().SetPrint(m_bPrintLogs);
                printf("Logs printout is now : %i\n", m_bPrintLogs);
            }    
            else if(vCommand[0]=="UNTHROTTLE")
            { 
                m_mIPThrottle.clear();
                
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING ,"Console Unthrottled all");
            } 
            else if(vCommand[0]=="BANT")
            {
                std::map<std::string, uint32_t>::iterator itThrottled;

                for(itThrottled = m_mIPThrottle.begin(); itThrottled != m_mIPThrottle.end(); ++itThrottled)
                {
                    m_mBanList[itThrottled->first]=time(0);
                    CLogger::GetInstance().Log(SERVER_LOG_WARRNING ,"Console Ban applied for "+itThrottled->first+" total "+std::to_string(m_mBanList.size())); 
                }   
            }                                                
            else if(vCommand[0]=="BAN")
            {
                m_mBanList[vCommand[1]]=time(0); 
                
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING ,"Console Ban applied for "+vCommand[1]+" total "+std::to_string(m_mBanList.size())); 
            }
            else if(vCommand[0]=="UBAN")
            {
                m_mBanList.erase(vCommand[1]);
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING ,"Console Ban removed for "+vCommand[1]+" total "+std::to_string(m_mBanList.size())); 
            }             
            // Zero-out
            m_strConsoleCommandTemp = "";
        }
        else 
            m_strConsoleCommandTemp+=(char)iCharacter;
    } 
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : IP Throttling
//
// -----------------------------------------------------------------------------
void CServer::ThrottleIP(const std::string& rstrIP)
{
    DisconnectIP(rstrIP); 

    if(rstrIP == m_uConfigParameters["APIIface"])
        return;
     
    if ( m_mIPThrottle.find(rstrIP) == m_mIPThrottle.end() ) 
    {
        CLogger::GetInstance().Log(SERVER_LOG_INFO,rstrIP+" Throttling started");
    }

   m_mIPThrottle[rstrIP] = time(0) + std::stoul(m_uConfigParameters["DefaultThrottleTime"]);

}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Check if IP throttled
//
// -----------------------------------------------------------------------------
bool CServer::IsIPThrottled(const std::string& rstrIP)
{ 
    //
    if ( m_mIPThrottle.find(rstrIP) == m_mIPThrottle.end() ) 
    {
        return false;
    }

    if(m_mIPThrottle[rstrIP]<time(0))
    {
        CLogger::GetInstance().Log(SERVER_LOG_INFO,rstrIP+" Throttling ended");

        m_mIPThrottle.erase(rstrIP);
        return false;
    } 

    //
    return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return list of tasks for specific user and script
//
// -----------------------------------------------------------------------------
std::vector<CTask*> CServer::GetTasksForUser(const std::string& rstrScriptName, const std::string& rstrUserHash)
{
    std::vector<CTask*> vpTasks;
    for (auto* task : m_vpTasks) {
        if (task->GetOwner() == rstrUserHash && task->GetScript() == rstrScriptName) {
            vpTasks.push_back(task);
        }
    }

    return vpTasks;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return a list of tasks ready to validate
//
// -----------------------------------------------------------------------------
std::vector<CTask*> CServer::GetTasksToValidate()
{

    std::vector<CTask*> vpTasks;  
    
    // Find out which are COMPLETED (Executed) 
    for(uint32_t b = 0; b < m_vpTasks.size(); b++)
    { 
        // Add them to the list
        if(m_vpTasks[b]->GetState() == CTask::eState::TASK_STATE_COMPLETED)
        { 
            // Check if any validator is having this task assigned
            bool bFree = true;
            for(uint32_t c = 0; c < m_vpNodes.size(); c++)
            { 
                if(m_vpNodes[c]->IsValidator() == 1 && m_vpNodes[c]->GetTaskID() == m_vpTasks[b]->GetID()) 
                    bFree = false; 
            }

            // Task is free to validate
            if(bFree)
                vpTasks.push_back(m_vpTasks[b]);
        }
    }   
 
    return vpTasks;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return tasks to execute
//
// -----------------------------------------------------------------------------
std::vector<CTask*> CServer::GetTasksToExecute()
{
    std::vector<CTask*> vpTasks;

    std::copy_if(m_vpTasks.begin(), m_vpTasks.end(), std::back_inserter(vpTasks),
                 [](CTask* task) {
                     return task->GetState() == CTask::eState::TASK_STATE_NEW;
                 });
    
    return vpTasks;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return list of free nodes
//
// -----------------------------------------------------------------------------
std::vector<CNode*> CServer::GetFreeNodes()
{ 
    std::vector<CNode*> vpFreeWorkers;

    // Use std::copy_if with a lambda function to filter out the free nodes
    std::copy_if(m_vpNodes.begin(), m_vpNodes.end(), std::back_inserter(vpFreeWorkers),
                 [](CNode* node) {
                     return !node->GetIdent().empty() &&
                            !node->IsValidator() &&
                            node->IsExecutingTasks() &&
                            node->IsFree();
                 });

    return vpFreeWorkers;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return list of free validators
//
// -----------------------------------------------------------------------------
std::vector<CNode*> CServer::GetFreeValidators()
{  
   std::vector<CNode*> vpFreeValidators;

    // Use std::copy_if with a lambda function to filter out the free validator nodes
    std::copy_if(m_vpNodes.begin(), m_vpNodes.end(), std::back_inserter(vpFreeValidators),
                 [](CNode* node) {
                     return !node->GetIdent().empty() &&
                            node->IsValidator() == 1 &&
                            node->IsFree();
                 });

    return vpFreeValidators;
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Delegate a task to a node
//
// -----------------------------------------------------------------------------
void CServer::DelegateTask(CNode *pNode, CTask *pTask)
{ 
   CLogger::GetInstance().Log(SERVER_LOG_INFO, pNode->GetIP() + " ("+pNode->GetIdent()+") Execute " + std::to_string(pTask->GetID()) + " " + pTask->GetScript());
 
    // Sending execution command to the node with the task information
    SendTo(pNode, "EXEC," + 
        CSystemUtil::base64_encode(pTask->GetScript()) + "," +
        pTask->GetPayload() + "," +
        m_uConfigParameters["TaskTimeout"] + "," +
        pTask->GetSourceHash() + ",");
 
    // Starting the task and setting the node to be busy with this task
    pTask->Start();
    pNode->SetTaskID(pTask->GetID());
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Get node by hash
//
// -----------------------------------------------------------------------------
CNode *CServer::GetNode(const std::string& rstrHash)
{
     for (auto* node : m_vpNodes) {
        if (node->GetHash() == rstrHash) {
            return node;
        }
    }
    return nullptr;
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Generates a human readable user alias from a hash
//
// -----------------------------------------------------------------------------
std::string CServer::GenerateUserAlias(const std::string& rstrHash256)
{ 
    if (rstrHash256.length() < 6) {
        throw std::invalid_argument("Hash string too short to generate alias.");
    }

    // Convert the first 6 characters of the hash into a seed number
    uint32_t seed = std::stoul(rstrHash256.substr(0, 6), nullptr, 16);

    // Use the seed to shuffle a copy of the wordlist
    std::vector<std::string> shuffled_wordlist(m_vstrWordList);
    std::mt19937 rng(seed);
    std::shuffle(shuffled_wordlist.begin(), shuffled_wordlist.end(), rng);
 
    // Build the user-friendly name using the first 3 words of the shuffled list
    std::string user_friendly_name = shuffled_wordlist[0] + shuffled_wordlist[1] + shuffled_wordlist[2];

    return user_friendly_name;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Purge unused user accounts
//
// -----------------------------------------------------------------------------
void CServer::RemoveUnusedAccounts()
{ 
   auto current_time = static_cast<uint32_t>(time(0));

    for (auto itUser = m_mUsers.begin(); itUser != m_mUsers.end(); )
    {
        auto* pClient = itUser->second; // Assuming m_mUsers stores unique_ptr
        
        if (current_time - pClient->GetCreatedTime() > std::stoul(m_uConfigParameters["UserAutoRemovalTime"]) && pClient->GetLastSeenTime() == 0)
        {
            std::string strPayload = std::to_string(rand() % 2048) + "," + pClient->GetUsername() + ",";
            std::vector<std::string> vCorePayload;
            SubmitTask("deluser", CSystemUtil::base64_encode(strPayload), pClient->GetUsername(), true, nullptr, vCorePayload);

            CLogger::GetInstance().Log(SERVER_LOG_INFO, itUser->first + " removed due to inactivity");
            itUser = m_mUsers.erase(itUser);  
        }
        else
        {
            ++itUser;
        }
    }
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Manual user removal
//
// -----------------------------------------------------------------------------
bool CServer::DelUser(const std::string& rstrHash)
{   
 auto itUser = m_mUsers.find(rstrHash);
    if (itUser == m_mUsers.end())
        return false;

    auto& pUser = itUser->second;

    CLogger::GetInstance().Log(SERVER_LOG_WARRNING, pUser->GetUsername() + " " + pUser->GetUsername() + " marked for removal, balance " + std::to_string(pUser->GetBalance()));

    if (pUser->GetBalance() > 0)
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING, pUser->GetUsername() + " " + pUser->GetUsername() + " lost balance " + std::to_string(pUser->GetBalance()));

    std::vector<CNode*> vNodes = GetNodesFor(rstrHash);

    for (auto* pNode : vNodes)
        DisconnectNode(pNode);
 
    m_mUsers.erase(itUser);

    return true;
}   
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return if IP is banned
//
// -----------------------------------------------------------------------------
bool CServer::IsBanned(const std::string& rstrValue)
{
    return m_mBanList.find(rstrValue) != m_mBanList.end();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return count of free validators
//
// -----------------------------------------------------------------------------
uint32_t CServer::GetNumOfValidatorsFree()
{ 
  return std::count_if(m_vpNodes.begin(), m_vpNodes.end(), []( CNode* node) {
        return node->GetIdent() != "" && node->GetTaskID() == 0 && node->IsValidator() == 1;
    });  
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return count of validators busy
//
// -----------------------------------------------------------------------------
uint32_t CServer::GetNumOfValidatorsBusy() {
    return std::count_if(m_vpNodes.begin(), m_vpNodes.end(), []( CNode* node) {
        return node->GetIdent() != "" && node->GetTaskID() != 0 && node->IsValidator() == 1;
    });
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return count of free nodes
//
// -----------------------------------------------------------------------------
uint32_t CServer::GetNumOfNodesFree() {
    return std::count_if(m_vpNodes.begin(), m_vpNodes.end(), []( CNode* node) {
        return node->GetIdent() != "" && node->GetTaskID() == 0 && node->IsValidator() == 0;
    });
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return count of busy nodes
//
// -----------------------------------------------------------------------------
uint32_t CServer::GetNumOfNodesBusy() {
    return std::count_if(m_vpNodes.begin(), m_vpNodes.end(), []( CNode* node) {
        return node->GetIdent() != "" && node->GetTaskID() > 0 && node->IsValidator() == 0;
    });
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return list of tasks with a specific state
//
// -----------------------------------------------------------------------------
std::vector<CTask*> CServer::GetTasksWithState(const CTask::eState& reState) {
    std::vector<CTask*> vRet;
    std::copy_if(m_vpTasks.begin(), m_vpTasks.end(), std::back_inserter(vRet),
                 [&reState]( CTask* task) { return task->GetState() == reState; });
    return vRet;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Save tasks
//
// -----------------------------------------------------------------------------
bool CServer::SaveTasks()
{
    std::ofstream fp(m_uConfigParameters["TasksListFile"], std::ios::binary | std::ios::out);
    if (!fp.is_open()) return false;

    CSerializer _Ser; 

    _Ser.Write_uint32(m_uiTaskIDCounter);
    _Ser.Write_uint32(m_vpTasks.size()); 

    for(uint32_t y = 0; y < m_vpTasks.size(); y++)
    { 
        _Ser.Write_uint32(m_vpTasks[y]->GetID()); 
        _Ser.Write_uint32(m_vpTasks[y]->GetState());
        _Ser.Write_uint32(m_vpTasks[y]->GetFailCount());
        _Ser.Write_uint32(m_vpTasks[y]->IsRewarded());
        _Ser.Write_string(m_vpTasks[y]->GetScript());
        _Ser.Write_string(m_vpTasks[y]->GetPayload());
        _Ser.Write_string(m_vpTasks[y]->GetSourceHash());
        _Ser.Write_string(m_vpTasks[y]->GetOwner());
        _Ser.Write_uint32(m_vpTasks[y]->GetCreationTime());
 
        std::vector<CTask::tExecution> vExecutions = m_vpTasks[y]->GetExecutions(); 
        _Ser.Write_uint32(vExecutions.size()); 
        for(uint32_t z = 0; z < vExecutions.size(); z++)
        {         
            _Ser.Write_string(vExecutions[z].m_strPayload);
            _Ser.Write_string(vExecutions[z].m_strPayloadHash);
            _Ser.Write_string(vExecutions[z].m_strNode);
            _Ser.Write_string(vExecutions[z].m_strUserHash);
            _Ser.Write_string(vExecutions[z].m_strUserName);
            _Ser.Write_uint32(vExecutions[z].m_uiStartTime);
            _Ser.Write_uint32(vExecutions[z].m_uiEndTime);
        }
 
        std::vector<std::string> vCorePayload = m_vpTasks[y]->GetCorePayload(); 
        _Ser.Write_uint32(vCorePayload.size()); 
        for(uint32_t z = 0; z < vCorePayload.size(); z++)
        {         
            _Ser.Write_string(vCorePayload[z]);
        }
    } 

 
    const std::vector<uint8_t>* pBuffer = _Ser.GetBufferPtr();

    // Ensure that pBuffer is not a nullptr
    if (pBuffer == nullptr) return false;

    // Directly write the buffer to the file
    fp.write(reinterpret_cast<const char*>(pBuffer->data()), pBuffer->size());
    return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Load tasks
//
// -----------------------------------------------------------------------------
bool CServer::LoadTasks()
{
    std::ifstream fp(m_uConfigParameters["TasksListFile"], std::ios::binary | std::ios::in);
    if (!fp.is_open()) return false;

    fp.seekg(0, std::ios::end);
    size_t m_nLength = fp.tellg();
    fp.seekg(0, std::ios::beg);

    std::vector<uint8_t> vIncomingPayload(m_nLength);
    if (!fp.read(reinterpret_cast<char*>(vIncomingPayload.data()), m_nLength)) {
        return false;
    }

    CSerializer _Ser;
    _Ser.Set(vIncomingPayload);

    m_uiTaskIDCounter       = _Ser.Read_uint32();
    uint32_t uiTasksCount   = _Ser.Read_uint32();


    for(uint32_t y =0; y < uiTasksCount;y++)
    {    
        uint32_t uiTaskID       = _Ser.Read_uint32();
        uint32_t uiTaskState    = _Ser.Read_uint32(); 
        uint32_t uiFailCount    = _Ser.Read_uint32();
        uint32_t uiRewarded      = _Ser.Read_uint32();
        std::string strScript   = _Ser.Read_string();
        std::string strPayload  = _Ser.Read_string();
        std::string strSourceHash  = _Ser.Read_string();
        std::string strOwner  = _Ser.Read_string();
        uint32_t uiCreationTime    = _Ser.Read_uint32();

        CTask *pTask = new CTask();
        pTask->SetID(uiTaskID);
        pTask->SetState(uiTaskState);
        pTask->SetFailCount(uiFailCount);
        pTask->SetScript(strScript);
        pTask->SetPayload(strPayload);
        pTask->SetRewarded(uiRewarded); 
        pTask->SetSourceHash(strSourceHash);
        pTask->SetOwner(strOwner);
        pTask->SetCreationTime(uiCreationTime);
 
        uint32_t uiTaskExecs    = _Ser.Read_uint32();

        for(uint32_t z = 0; z < uiTaskExecs; z++)
        { 
            std::string strPayloadE  = _Ser.Read_string();
            std::string strHash     = _Ser.Read_string();
            std::string strNode    = _Ser.Read_string();
            std::string strUser     = _Ser.Read_string();
            std::string strUserN    = _Ser.Read_string();
            uint32_t uiStartTime    = _Ser.Read_uint32();
            uint32_t uiEndTime      = _Ser.Read_uint32(); 

            CTask::tExecution _Exec;
            _Exec.m_strPayload      = strPayloadE;
            _Exec.m_strPayloadHash  = strHash;
            _Exec.m_strNode        = strNode;
            _Exec.m_strUserHash     = strUser;
            _Exec.m_strUserName     = strUserN;
            _Exec.m_uiStartTime     = uiStartTime;
            _Exec.m_uiEndTime       = uiEndTime;
            pTask->AddExecution(_Exec);
        }
 
        uint32_t uiCorePayload = _Ser.Read_uint32();
        for(uint32_t z = 0; z < uiCorePayload; z++)
        {
            std::string strPayloadC = _Ser.Read_string(); 
            pTask->AddCorePayload(strPayloadC);
        }
         
        CLogger::GetInstance().Log(SERVER_LOG_INFO,"Loaded Task."+std::to_string(pTask->GetID())+" state "+std::to_string(pTask->GetState())+" cpayload: "+std::to_string(uiCorePayload)); 
        m_vpTasks.push_back(pTask);
    }
    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Last task id "+std::to_string(m_uiTaskIDCounter));
    return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Get task
//
// -----------------------------------------------------------------------------
CTask *CServer::GetTask(const uint32_t&ruiValue)
{
    auto it = std::find_if(m_vpTasks.begin(), m_vpTasks.end(),[&ruiValue](CTask* task) { return task->GetID() == ruiValue; });
    if (it != m_vpTasks.end()) {
        return *it; // Dereference the iterator to get the pointer to CTask
    }
    return nullptr;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Get SHA1 of a file
//
// -----------------------------------------------------------------------------
std::string CServer::GetSHA1File(const std::string& rstrFName)
{
    std::string strFileHash;
    std::ifstream file(rstrFName, std::ios::binary);
    if (!file) {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR, "Can't SHA1 " + rstrFName);
        return strFileHash; // return empty hash if file can't be opened
    }
 
    std::string file_content((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
    strFileHash = GetSHA1(file_content);

    return strFileHash;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Perform tasks cleanup
//
// -----------------------------------------------------------------------------
void CServer::CleanUpTasks()
{ 
    for (auto it = m_vpTasks.begin(); it != m_vpTasks.end(); /* no increment here */) {
        CTask* pTask = *it;
        if (pTask->GetState() == CTask::eState::TASK_STATE_VALIDATED || pTask->GetState() == CTask::eState::TASK_STATE_FAILED) {
            CLogger::GetInstance().Log(SERVER_LOG_INFO, "Task " + std::to_string(pTask->GetID()) + " removed");

            // Keep history of tasks for GetTask 
            if(pTask->GetExecCount()>0)
                m_mTasksHistory[pTask->GetID()]=pTask->GetExecutions().back().m_strPayload;
            else
                m_mTasksHistory[pTask->GetID()]="";

            // Rotate last tasks
            if(m_mTasksHistory.size()>std::stoull(m_uConfigParameters["TasksHistory"]))
             m_mTasksHistory.erase(m_mTasksHistory.begin());
            

            delete pTask; 
            it = m_vpTasks.erase(it);
        } else {
            ++it;
        }
    }
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Get utilization of the network
//
// -----------------------------------------------------------------------------
float CServer::GetUtilization()
{
   auto totalNodes = GetNumOfNodes();
   return totalNodes > 0 ? (static_cast<float>(GetNumOfNodesBusy()) / totalNodes) * 100.0f : 0.0f;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Submit the task
//
// -----------------------------------------------------------------------------
uint32_t CServer::SubmitTask(const std::string& rstrScript, const std::string& rstrPayload64, const std::string& rstrOwnerUsername, 
    const bool& bReward, CNode *pAssignTo, const std::vector<std::string>& rvCorePayload)
{
    CTask *pNewTask = new CTask(); 
    pNewTask->SetID(m_uiTaskIDCounter);
    pNewTask->SetScript(rstrScript); 
    pNewTask->SetPayload(rstrPayload64);
    pNewTask->SetSourceHash(GetSHA1File("scripts/"+rstrScript+".bas")); 
    pNewTask->SetCreationTime(time(0));
    pNewTask->SetOwner(rstrOwnerUsername);    
    pNewTask->SetRewardGrant(bReward);            
    pNewTask->SetCorePayload(rvCorePayload);
    pNewTask->Save();
    m_vpTasks.push_back(pNewTask);
    

    // Forced delegation
    std::string strNodeID = "none";
    if(pAssignTo != nullptr)
    {
        CLogger::GetInstance().Log(SERVER_LOG_INFO,"Assigning "+std::to_string(m_uiTaskIDCounter)+" to "+pAssignTo->GetIdent());
        strNodeID = pAssignTo->GetIdent();
        DelegateTask(pAssignTo, pNewTask);
    }

    CLogger::GetInstance().Log(SERVER_LOG_INFO,"Added task "+
        rstrScript+
        " TaskID: "+std::to_string(m_uiTaskIDCounter)+
        " Owner: "+rstrOwnerUsername+
        " Reward: "+std::to_string(bReward)+
        " Delegate: "+strNodeID+
        " CP: "+std::to_string(rvCorePayload.size()));

    m_uiTaskIDCounter++;
 
    return m_uiTaskIDCounter-1;

}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Transfer shells
//
// -----------------------------------------------------------------------------
bool CServer::TransferShellTokens(const std::string& rstrFrom, const std::string& rstrTo, const uint32_t& ruiValue)
{

    CUser *pFrom = GetUserByName(rstrFrom);
    CUser *pTo = GetUserByName(rstrTo);
    CLogger::GetInstance().Log(SERVER_LOG_INFO,"XFER "+std::to_string(ruiValue)+" from "+rstrFrom+" to "+rstrTo);

    if(ruiValue == 0)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Cant transfer "+std::to_string(ruiValue)+" is zero");

        return false;      
    }
    if(pFrom == nullptr)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Cant transfer "+std::to_string(ruiValue)+" from "+rstrFrom+" to "+rstrTo+", from not exists");

        return false;
    }
    if(pTo == nullptr)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Cant transfer "+std::to_string(ruiValue)+" from "+rstrFrom+" to "+rstrTo+", to not exists");

        return false;
    }
 
    uint32_t uiFromBalance = pFrom->GetBalance();
    if(ruiValue > uiFromBalance)
    {
        CLogger::GetInstance().Log(SERVER_LOG_ERROR,"Cant transfer "+std::to_string(ruiValue)+" from "+rstrFrom+" to "+rstrTo+", not enough");
        return false;
    }
 
    uint32_t uiLeft = uiFromBalance - ruiValue;

    pFrom->SetBalance(uiLeft); 
    pTo->AddBalance(ruiValue);

    CLogger::GetInstance().Log(SERVER_LOG_INFO,"XFER "+std::to_string(ruiValue)+" from "+rstrFrom+" to "+rstrTo+", completed. Sender: "+std::to_string(pFrom->GetBalance())+" Recv: "+std::to_string(pTo->GetBalance()));
    return true;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Tick the awards calculations
//
// -----------------------------------------------------------------------------
void CServer::AwardsTick()
{ 
    double minRankValue = std::numeric_limits<double>::max();
    double maxRankValue = -std::numeric_limits<double>::max();
 
    for (const auto& userPair : m_mUsers) {
        CUser* user = userPair.second;
 
        double submitCount = static_cast<double>(user->GetSubmitCount());
        double totals = static_cast<double>(user->GetTotals());
        double upf = user->GetUPF();
 
        minRankValue = std::min(minRankValue, submitCount + totals + upf);
        maxRankValue = std::max(maxRankValue, submitCount + totals + upf);
    }

    for (const auto& userPair : m_mUsers) {
        CUser* user = userPair.second;
 
        double submitCount = static_cast<double>(user->GetSubmitCount());
        double totals = static_cast<double>(user->GetTotals());
        double upf = user->GetUPF();
 
        double rankValue = submitCount + totals + upf;


        std::string strRank = "";
 
        double range = maxRankValue - minRankValue;
 
        if (range < 0.001) {
            range = 0.001;
        }
 
        double normalizedRank = 100.0 * (rankValue - minRankValue) / range;
          
        user->SetRank(normalizedRank); 
    }
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return a node that has a chunk
//
// -----------------------------------------------------------------------------
CNode* CServer::GetNodeWithChunk(const uint64_t& ruiSeq, bool isValidator) {
    for (auto& node : m_vpNodes) {
        if (node->HasChunk(ruiSeq) && node->IsValidator() == isValidator) {
            return node;
        }
    }
    return nullptr;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return a list of nodes with a chunk
//
// -----------------------------------------------------------------------------
std::vector<CNode*> CServer::GetNodesWithChunk(const uint64_t& ruiSeq)
{
    std::vector<CNode*> vpNodes;
    for (auto& node : m_vpNodes) {
        if (node->HasChunk(ruiSeq)) {
            vpNodes.push_back(node);
        }
    }
    return vpNodes;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return count of chunks that are lost (dont have node assigned)
//
// -----------------------------------------------------------------------------
uint32_t CServer::GetCountOfLostChunks()
{
    auto allChunkKeys = m_DFS.GetAllCompletedChunks();
    return std::count_if(allChunkKeys.begin(), allChunkKeys.end(), [this](const uint64_t& chunkKey) {
      return GetNodeWithChunk(chunkKey, false) == nullptr;
    });
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Helper
//
// -----------------------------------------------------------------------------
std::string CServer::GetSHA1(const std::string& rstrInput)
{
    CSHA1 m_SHA1;  
    m_SHA1.update(rstrInput);   
    return m_SHA1.final();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Helper
//
// -----------------------------------------------------------------------------
std::string CServer::GetSHA256(const std::string& rstrInput)
{
    CSHA256 m_SHA256;            
    m_SHA256.update(rstrInput);  
    return m_SHA256.final();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Returns a config param from the map
//
// -----------------------------------------------------------------------------
std::string CServer::GetConfigParameter(const std::string& rstrParameter)
{
    if (m_uConfigParameters.find(rstrParameter) == m_uConfigParameters.end()) return "";

    return m_uConfigParameters[rstrParameter];
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   :  
//
// -----------------------------------------------------------------------------
CCommand* CServer::GetCommand(const std::string& commandType) {
        auto it = m_mCommandsMap.find(commandType);
        if (it != m_mCommandsMap.end()) {
            return it->second.get();
        }
        return nullptr;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Returns DFS object
//
// -----------------------------------------------------------------------------
CDFS *CServer::GetDFS()
{
    return &m_DFS;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Adds user
//
// -----------------------------------------------------------------------------
void CServer::AddUser(CUser *pUser)
{
    m_mUsers[pUser->GetHash()] = pUser;   
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Gets word list
//
// -----------------------------------------------------------------------------
std::vector<std::string> CServer::GetWordList()
{
    return m_vstrWordList;    
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Adds entry to nodes availability
//
// -----------------------------------------------------------------------------
void CServer::AddNodeAvailability(CNode *pNode)
{
    if(pNode != nullptr)
    {
        m_mNodesAvailability[pNode->GetIdent()].push_back(std::time(nullptr));
    }
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : The function CalculateAvailability takes a node ID and calculates its availability.
//    It checks if the node exists in the map and if there are timestamps associated with it.
//    The function assumes even indexes in the vector are connection times and odd indexes are disconnection times.
//    It calculates the total uptime in the last 24 hours and then computes the availability percentage.
//    If a node has an ongoing connection, it accounts for this in the uptime calculation.
//
// ----------------------------------------------------------------------------- 
double CServer::CalculateAvailability(const std::string& nodeId) {
    auto it = m_mNodesAvailability.find(nodeId);
    if (it == m_mNodesAvailability.end()) {
        // Node not found
        return 0.0;
    }

    const auto& timestamps = it->second;
    if (timestamps.empty()) {
        // No data for the node
        return 100.0; // Assuming always available if no data
    }

    uint32_t currentTime = static_cast<uint32_t>(std::time(nullptr));
    uint32_t twentyFourHoursAgo = currentTime - 86400; // 24 hours in seconds
    double totalUptime = 0.0;
    uint32_t lastTimestamp = 0;

    for (size_t i = 0; i < timestamps.size(); ++i) {
        if (timestamps[i] < twentyFourHoursAgo) {
            continue; // Skip outdated timestamps
        }

        if (i % 2 == 0) { // Assuming even index is a connection time
            lastTimestamp = timestamps[i];
        } else if (lastTimestamp != 0) { // Odd index is a disconnection time
            totalUptime += timestamps[i] - lastTimestamp;
            lastTimestamp = 0;
        }
    }

    // Handle ongoing connection
    if (lastTimestamp != 0 && lastTimestamp >= twentyFourHoursAgo) {
        totalUptime += currentTime - lastTimestamp;
    }

    return (totalUptime / 86400.0) * 100.0; // Percentage of uptime in the last 24 hours
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Return a node by a specific ident
//
// -----------------------------------------------------------------------------    
CNode *CServer::GetNodeByIdent(const std::string& rstrIdent)
{
    for(uint32_t x = 0; x < m_vpNodes.size(); x++)
    {
        CNode *pNode = m_vpNodes[x];
        
        if(pNode->GetIdent() == rstrIdent)
            return pNode;
    } 
    return nullptr;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Experimental TV Calculation
//
// -----------------------------------------------------------------------------
double CServer::CalculateTheoreticalValue() 
{
    // This is the starting point for the theoretical value. It's a predetermined constant that sets the baseline.
    double baseValue            = std::stod(GetConfigParameter("BaseValue"));
    double totalTasksToExecute  = m_vpTasks.size(); 
    double networkUtilization   = GetUtilization();
    double totalNodes           = m_vpNodes.size();

    // Workload Modifier based on totalTasksToExecute
    // This factor is calculated based on the ratio of the total tasks currently waiting to be executed (totalTasksToExecute) to the total number of active nodes (totalNodes) in the network.
    // It reflects the average workload per node. A higher ratio suggests a higher demand for the network's resources, potentially increasing the theoretical value.
    double workloadModifier = (totalNodes > 0) ? totalTasksToExecute / static_cast<double>(totalNodes) : 0;

    // A constant (e.g., 0.1) is added to this ratio to ensure that the workload modifier does not drop to zero, maintaining a baseline value for the tv even during periods of low activity.
    workloadModifier += 0.1;

    //Utilization Modifier (UM):  
    //The UM is determined by the current percentage of network utilization (networkUtilization). It represents how much of the network's total capacity is being used.
    //A minimum threshold is set for UM (e.g., 0.1) to prevent it from becoming zero, which ensures that the tv retains some value even when the network utilization is very low.
    double minimumUM = 0.1; // Set a minimum threshold for UM to avoid it becoming zero
    double UM = std::max(networkUtilization / 100.0, minimumUM);


    // Node Modifier (NM):
    // This factor is inversely proportional to the number of active nodes. It's calculated as 1 divided by the total number of nodes (totalNodes).
    // NM helps to balance the tv in relation to the scale of the network. More nodes could mean a potential increase in the supply, so NM adjusts the value downwards to prevent inflation in a large network.
    // In case there are no active nodes, NM is set to a default value (e.g., 1.0) to handle this edge case and avoid division by zero.
    double NM = (totalNodes > 0) ? 1.0 / static_cast<double>(totalNodes) : 1.0;


    // Final Theoretical Value Calculation:
    // The final value is calculated by multiplying the base value with these three modifiers: the workload modifier, the utilization modifier, and the node modifier.
    // This calculation ensures that the value dynamically reflects the network's current workload, utilization, and scale. 
    // It's designed to adapt to changing conditions in the network, ensuring that the TV value is aligned with the actual demand 
    // for and capacity of the network's resources.
    double tokenValue = baseValue * workloadModifier * UM * NM;

    // Cap the theoretical value to a maximum of 1.0
    tokenValue = std::min(tokenValue, 1.0);

    return tokenValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Returns a task payload from the history vector - used in GetTask
//
// -----------------------------------------------------------------------------
std::string CServer::GetTaskHistory(const uint32_t& ruiTaskId)
{     
    auto it = m_mTasksHistory.find(ruiTaskId);
    if(it != m_mTasksHistory.end()) return it->second;
    return "";    
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Check if a script is allowed (loaded)
//
// -----------------------------------------------------------------------------
bool CServer::IsLoadedScript(const std::string& rstrScriptName)
{
    std::stringstream ss(m_uConfigParameters["AllowedScripts"]);
    std::string script;
    std::vector<std::string> scripts;

    // Split the allowedScripts string by comma
    while (getline(ss, script, ',')) {
        scripts.push_back(script);
    }

    // Check if scriptToCheck is in the list of scripts
    return std::find(scripts.begin(), scripts.end(), rstrScriptName) != scripts.end();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : Check if a script is core 
//
// -----------------------------------------------------------------------------
bool CServer::IsCoreScript(const std::string& rstrScriptName)
{
    std::stringstream ss(m_uConfigParameters["CoreScripts"]);
    std::string script;
    std::vector<std::string> scripts;

    // Split the allowedScripts string by comma
    while (getline(ss, script, ',')) {
        scripts.push_back(script);
    }

    // Check if scriptToCheck is in the list of scripts
    return std::find(scripts.begin(), scripts.end(), rstrScriptName) != scripts.end();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CServer
//  - Prototype :
//
//  - Purpose   : DTOR
//
// -----------------------------------------------------------------------------
CServer::~CServer()
{     
}
