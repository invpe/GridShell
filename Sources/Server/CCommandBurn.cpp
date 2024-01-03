#include "CCommandBurn.h"
#include "CServer.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandBurn
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CCommandBurn::CCommandBurn()
{
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandBurn
//  - Prototype :
//
//  - Purpose   : Burn tokens
//
// -----------------------------------------------------------------------------
bool CCommandBurn::Validate(const std::vector<std::string>& vData,CServer& rServer, CNode *pNode) 
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
    
    // This will limit the usage of the burn to defined timing
    if(static_cast<uint32_t>(time(0) - pUser->GetLastCommandTime()) < static_cast<uint32_t>(std::stoul(rServer.GetConfigParameter("ExcessiveCommandTime"))))
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Excessive user commands"); 
        return false;
    }

    pUser->UpdateLastCommand();

    if(vData.size() != 2)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Wrong syntax"); 
        return false;
    }

    std::string strBurnWhat  = vData[1];   

    if(std::stoul(rServer.GetConfigParameter("BurnSlotPrice"))+std::stoul(rServer.GetConfigParameter("TaskCreationCost"))> pUser->GetBalance() )
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Not enough balance"); 
        return false;
    }

    if(strBurnWhat == "TSLOT")
    { 
        pUser->SetMaxFiles(pUser->GetMaxFiles()+1); 
    } 
    else if(strBurnWhat == "TSIZE")
    { 
        pUser->SetMaxFileSize(pUser->GetMaxFileSize()+1000);  
    }
    else
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Unknown burn item"); 
        return false;                      
    } 

    std::vector<std::string> vCorePayload;
    rServer.SubmitTask("burn",CSystemUtil::base64_encode(strBurnWhat+","+pUser->GetUsername()+","),pUser->GetUsername(), false, nullptr, vCorePayload);   

    // Core validators do not get balance reduced, they are generating tokens
    if(pUser->IsValidator())
    {
        //
    }
    else
    {
        uint32_t uiBalanceNew = pUser->GetBalance()-std::stoul(rServer.GetConfigParameter("TaskCreationCost"))-std::stoul(rServer.GetConfigParameter("BurnSlotPrice"));
        pUser->SetBalance(uiBalanceNew);  
    }
    
    pUser->AddSubmitCount(); 


    return true;
}