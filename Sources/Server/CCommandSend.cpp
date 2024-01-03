#include "CCommandSend.h"
#include "CServer.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandSend
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CCommandSend::CCommandSend()
{
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandSend
//  - Prototype :
//
//  - Purpose   : Send shells
//
// -----------------------------------------------------------------------------
bool CCommandSend::Validate(const std::vector<std::string>& vData,CServer& rServer, CNode *pNode) 
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
    
    if(static_cast<uint32_t>(time(0) - pUser->GetLastCommandTime()) < static_cast<uint32_t>(std::stoul(rServer.GetConfigParameter("ExcessiveCommandTime"))))
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Excessive user commands"); 
        return false;
    }

    pUser->UpdateLastCommand();

    if(vData.size() != 3)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Wrong syntax"); 
        return false;
    }

    std::string strReceiver  = vData[1];      
    std::string strAmount    = vData[2];     

    if(rServer.GetUserByName(strReceiver)==nullptr)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Unknown receipent"); 
        return false;                            
    }

    if(!CSystemUtil::IsNumber(strAmount))
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") NAN"); 
        return false;                  
    }

    if(strAmount.length() > 4)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Bad AMT size"); 
        return false;                       
    }

    uint32_t uiAmout = std::stoull(strAmount);


    if(uiAmout > std::stoul(rServer.GetConfigParameter("MaxShellsSend")))
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") MAX_SEND_AMT"); 
        return false;                          
    }

    if(uiAmout < std::stoul(rServer.GetConfigParameter("MinShellsSend")))
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") MIN_SEND_AMT"); 
        return false;                          
    }
    
    if(uiAmout + std::stoul(rServer.GetConfigParameter("TaskCreationCost")) > pUser->GetBalance() )
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Not enough balance");  
        return false;  
    }

    if(rServer.TransferShellTokens(pUser->GetUsername(),strReceiver,uiAmout))
    {
        std::vector<std::string> vCorePayload;
        rServer.SubmitTask("send",CSystemUtil::base64_encode(strAmount+","+pUser->GetUsername()+","+strReceiver+","),pUser->GetUsername(), false, nullptr, vCorePayload);   

        // Core validators do not get balance reduced, they are generating tokens
        if(pUser->IsValidator())
        {
            //
        }
        else
        {
            uint32_t uiBalanceNew = pUser->GetBalance()-std::stoul(rServer.GetConfigParameter("TaskCreationCost")); 
            pUser->SetBalance(uiBalanceNew);  
        }

        pUser->AddSubmitCount();

        return true;

    }
    return false;
}