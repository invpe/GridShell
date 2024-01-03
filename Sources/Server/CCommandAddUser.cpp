#include "CCommandAddUser.h"
#include "CServer.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandAddUser
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CCommandAddUser::CCommandAddUser()
{
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandAddUser
//  - Prototype :
//
//  - Purpose   : Adds the user to the server
//
// -----------------------------------------------------------------------------
bool CCommandAddUser::Validate(const std::vector<std::string>& vData,CServer& rServer, CNode *pNode) 
{ 
    
    if(pNode->GetIP() != rServer.GetConfigParameter("APIIface")) 
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Wrong Interface");
        return false;
    }

    if(vData.size() != 2)
    {  
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Wrong command size");
        return false;
    }   

    // Generate Primary GRID UHASH from the 12 words seed phrase
    // Human readable will include spaces, which when recovering should be removed
    std::string strSeedPhrase;
    std::string strHumanReadableSeedPhrase;
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::vector<std::string> vWordList = rServer.GetWordList();
    int wordlistSize = vWordList.size();
    std::uniform_int_distribution<int> distribution(0, wordlistSize - 1);

    for (int i = 0; i < 12; ++i) {
        int randomIndex = distribution(generator);
        strSeedPhrase += vWordList[randomIndex];
        strHumanReadableSeedPhrase += vWordList[randomIndex]+" ";
    }

    // Append with salt
    std::uniform_int_distribution<uint64_t> distribution64(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max());                    
    uint64_t uiSalt = distribution64(generator); 
    strSeedPhrase+=std::to_string(uiSalt);
    strHumanReadableSeedPhrase+=std::to_string(uiSalt);

    // Generate hash
    std::string strUserHash = rServer.GetSHA256(strSeedPhrase);

    // Generate User Alias
    std::string strUsername = rServer.GenerateUserAlias(strUserHash);
    
    // Check if not duplicate
    if ( rServer.GetUserByHash(strUserHash) == nullptr) 
    { 
        CUser *pUser = new CUser();
        pUser->SetHash(strUserHash); 
        pUser->SetUsername(strUsername);                         
        pUser->SetBalance(0);  
        pUser->SetCreatedTime(time(0));
        pUser->SetLastSeen(0); 
        pUser->SetRetention(std::stoul(rServer.GetConfigParameter("DFSDefaultRetentionSeconds")));
        pUser->SetFilesystem(1);
        pUser->SetMaxFiles(std::stoul(rServer.GetConfigParameter("UserDefaultTelemetrySlots")));
        pUser->SetMaxFileSize(std::stoul(rServer.GetConfigParameter("UserDefaultTelemetrySize")));

        for(uint32_t uiCt = 0; uiCt < std::stoul(rServer.GetConfigParameter("UPFExecutionsCount")); uiCt++)
            pUser->AddUPF(1, std::stoul(rServer.GetConfigParameter("UPFExecutionsCount")));

        rServer.AddUser(pUser);

        CLogger::GetInstance().Log(SERVER_LOG_INFO,"User added "+strUsername);
        rServer.SendTo(pNode, "ADDUSER,"+strUsername+","+strUserHash+","+strHumanReadableSeedPhrase+"\n"); 
    } 
    else
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,"User not added "+strUsername+" hash collision"); 

    rServer.DisconnectNode(pNode);
    return true;
}