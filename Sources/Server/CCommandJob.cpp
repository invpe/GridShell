#include "CCommandJob.h"
#include "CServer.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandJob
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CCommandJob::CCommandJob()
{
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandJob
//  - Prototype :
//
//  - Purpose   : Ident
//
// -----------------------------------------------------------------------------
bool CCommandJob::Validate(const std::vector<std::string>& vData,CServer& rServer, CNode *pNode) 
{
    // Safety check
    if(vData.size() != 4)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Wrong syntax");         
        return false;
    }
    
    // 
    std::string strUserHash  = vData[1];      // Base64(sha1(hash))
    std::string strVersion   = vData[2];      // Node version
    std::string strIdent     = vData[3];      // Node ident

 
    if(pNode->GetIdent() != "")
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Duplicate Ident"); 
        return false;                        
    } 
    if( strUserHash.empty() )
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Empty UID"); 
        return false;                       
    }                     
    if(CSystemUtil::base64_decode(strUserHash) == "")
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Empty HASH"); 
        return false;                       
    }
    if(CSystemUtil::base64_decode(strUserHash).size() != SHA1_LENGTH && CSystemUtil::base64_decode(strUserHash).size() != SHA256_LENGTH)  
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Wrong HASH Len"); 
        return false;                       
    }                    
    if(strVersion.empty() )
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Empty Version"); 
        return false;                       
    } 
    if(strVersion != SERVER_VERSION)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Version mismatch");
        return false; 
    }
    if( strIdent.empty() )
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Empty ID"); 
        return false;                       
    }  
    if( strIdent.size() != std::stoul(rServer.GetConfigParameter("IDLength")) )
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Wrong Ident Length"); 
        return false;                       
    }  
    if(rServer.GetNodeByIdent(strIdent) != nullptr)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Duplicate node ident" ); 
        return false;        
    }
    //
    strUserHash = CSystemUtil::base64_decode(strUserHash);    
    CUser *pUser = rServer.GetUserByHash(strUserHash);
    if(pUser == nullptr)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") User not found");        
        return false;
    }

    if(rServer.IsBanned(strUserHash) == true)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Banned" ); 
        return false;
    } 
    
    // All set, update this node 
    pNode->SetUserHash(strUserHash);                
    pNode->SetVersion(strVersion);  
    pNode->SetIdent(CSystemUtil::ToLower(strIdent));    
    std::string strNodeHash = rServer.GetSHA1(pNode->GetIdent() + pNode->GetUserHash() + pNode->GetIP());
    pNode->SetHash(strNodeHash);     
    pNode->ClearJob(); 
    pNode->SetIdentTime(time(0));
    
    // If this is API IFace connecting, we set NON EXEC flag
    if(pNode->GetIP() == rServer.GetConfigParameter("APIIface")) 
    {
        pNode->SetExecFlag(0);
    }
    else
        pNode->SetExecFlag(1);
    

    if(rServer.GetNodesCountWithHash(strNodeHash) > std::stoul(rServer.GetConfigParameter("MaxNodesPerDevice")))
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Too many nodes ("+std::to_string(rServer.GetNodesCountWithHash(strNodeHash))+")");
        return false;
    }

    if(rServer.GetUserByHash(strUserHash)->IsValidator())
        pNode->SetValidatorFlag(1);   

    // When connecting, add epoch for node availability calculations
    rServer.AddNodeAvailability(pNode);

    CLogger::GetInstance().Log(SERVER_LOG_INFO,pUser->GetUsername()+" ("+pNode->GetIdent()+") Idented ("+std::to_string(pNode->IsValidator())+") Exec: "+std::to_string(pNode->IsExecutingTasks()));
    return true;
}