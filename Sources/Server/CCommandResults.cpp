#include "CCommandPong.h"
#include "CServer.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandResults
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CCommandResults::CCommandResults()
{
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandResults
//  - Prototype :
//
//  - Purpose   : Nodes returning task results
//
// -----------------------------------------------------------------------------
bool CCommandResults::Validate(const std::vector<std::string>& vData,CServer& rServer, CNode *pNode) 
{

    if(pNode->GetIdent().empty())
    {                                              
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") No Ident");
        return false;
    }     

    CUser *pUser = rServer.GetUserByHash(pNode->GetUserHash());
    if(pUser == nullptr)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") User not found");        
        return false;
    }
    
    if(vData.size()!=3)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Wrong command posted");  
        return false;
    }

    std::string strSuccess  = vData[1];
    std::string strPayload  = vData[2];  

    if(pNode->GetTaskID() == 0)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Results without task"); 
        return false;                    
    }

    if(strSuccess.size()==0 || strSuccess.size()>3)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Wrong command posted BSUCCES");  
        return false;      
    }                              

    CTask *pTask = rServer.GetTask( pNode->GetTaskID() ); 
     
    // Task does not exist anymore
    if(!pTask)
    {
        pNode->ClearJob();  
        return true;
    } 
    if(strPayload == "" || CSystemUtil::base64_decode(strPayload).size() == 0)
    {   
        pTask->AddFailing(pNode->GetIdent());

        pNode->ClearJob(); 

        pTask->Fail();

        CLogger::GetInstance().Log(SERVER_LOG_INFO,pUser->GetUsername()+" ("+pNode->GetIdent()+") Task "+std::to_string(pTask->GetID())+" empty payload failed("+std::to_string(pTask->GetFailCount())+")"); 
    }
    else
    {
        
        // Add execution to the task
        CTask::tExecution       _New;
        _New.m_strPayload       = strPayload;
        _New.m_uiStartTime      = pNode->GetStartTime();
        _New.m_uiEndTime        = time(0); 
        _New.m_strUserHash      = rServer.GetUserByHash(pNode->GetUserHash())->GetHash();
        _New.m_strUserName      = rServer.GetUserByHash(pNode->GetUserHash())->GetUsername();
        _New.m_strNode          = pNode->GetHash();
        _New.m_strPayloadHash   = rServer.GetSHA1(strPayload);     
        pTask->AddExecution(_New);              
        pTask->Complete();

        // Validator returning
        if(pNode->IsValidator())
        {
            // Obtain the pointer to the executor
            CUser *pExecutingUser = rServer.GetUserByHash(pTask->GetExecutions().front().m_strUserHash);
            CNode *pExecutingNode = rServer.GetNode(pTask->GetExecutions().front().m_strNode);
            
            if(pTask->IsValid() == 0)
            { 
                pExecutingUser->AddUPF(0,std::stoul(rServer.GetConfigParameter("UPFExecutionsCount")));                   
                pTask->Fail();
                CLogger::GetInstance().Log(SERVER_LOG_INFO ,pUser->GetUsername()+" ("+pNode->GetIdent()+") Validation for task "+std::to_string(pTask->GetID())+" Invalid Hashes ("+strPayload+") Fail: "+std::to_string(pTask->GetFailCount())); 
            } 
            else
            {    
                uint32_t iRoll = rand()%100;
                if(iRoll <= pExecutingUser->GetUPF())
                {
                    pExecutingUser->AddUPF(1,std::stoul(rServer.GetConfigParameter("UPFExecutionsCount")));  

                    if(pTask->IsGrantedReward() == 1)
                    { 
                        pTask->SetRewarded(1);
                        
                        // TODO: Move to config
                        uint32_t uiReward = 1;

                        pExecutingUser->AddBalance(uiReward);

                        CLogger::GetInstance().Log(SERVER_LOG_INFO,pUser->GetUsername()+" ("+pNode->GetIdent()+") Task "+std::to_string(pTask->GetID())+" Valid - rewarded");
                    }
                    else
                    {
                        CLogger::GetInstance().Log(SERVER_LOG_INFO,pUser->GetUsername()+" ("+pNode->GetIdent()+") Task "+std::to_string(pTask->GetID())+" Valid - task not rewarded");   
                    }
                }
                else
                {
                    CLogger::GetInstance().Log(SERVER_LOG_INFO,pUser->GetUsername()+" ("+pNode->GetIdent()+") Task "+std::to_string(pTask->GetID())+" Valid - no reward (upf "+std::to_string(pExecutingUser->GetUPF())+" roll "+std::to_string(iRoll)+")");
                }

                pTask->Validate();
                pExecutingUser->AddTotals();   

                // Replication success, store the chunk id on the executing node
                if(pTask->GetScript() == "replicatedfs")
                {                                                       
                    std::vector<std::string> vCore = pTask->GetCorePayload(); 
                    uint64_t uiSeq = std::stoul(vCore[0]); 

                    // Executor
                    if(pExecutingNode!=nullptr)
                    pExecutingNode->AddChunk(uiSeq);

                    // Validator  
                    if(pNode!=nullptr)
                    pNode->AddChunk(uiSeq);
                }
            }
        }                        

        //
        CLogger::GetInstance().Log(SERVER_LOG_INFO,pNode->GetIP()+" ("+pUser->GetUsername()+") Completed "+std::to_string(pTask->GetID())+" in "+std::to_string(_New.m_uiEndTime-_New.m_uiStartTime)+"s");                        
    }    
    pTask->Save(); 
    pNode->ClearJob();  
    return true;
}