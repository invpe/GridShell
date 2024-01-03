#include "CCommandPong.h"
#include "CServer.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandPong
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CCommandPong::CCommandPong()
{
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandPong
//  - Prototype :
//
//  - Purpose   : PONG from the nodes
//
// -----------------------------------------------------------------------------
bool CCommandPong::Validate(const std::vector<std::string>& vData,CServer& rServer, CNode *pNode) 
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
    
    // TODO: Move to config
    if(time(0) - pNode->GetHeartBeat() < 2)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Pong Flood");        
        return false;    
    }

    pNode->Pong();
    pUser->SetLastSeen(time(0));  
    return true;
}