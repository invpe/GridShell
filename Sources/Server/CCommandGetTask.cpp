#include "CCommandGetTask.h"
#include "CServer.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandGetTask
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CCommandGetTask::CCommandGetTask()
{
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandGetTask
//  - Prototype :
//
//  - Purpose   : Experimental (ADMIN) command to fetch task details directly from server not from API (lag)
//
// -----------------------------------------------------------------------------
bool CCommandGetTask::Validate(const std::vector<std::string>& vData,CServer& rServer, CNode *pNode) 
{   

    if(pNode->GetIP() != rServer.GetConfigParameter("DEVIface")) 
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Wrong Interface");
        return false;
    }

    if(vData.size() != 2)
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") Wrong syntax"); 
        return false;
    }

    std::string strTaskID  = vData[1];   
    if(!CSystemUtil::IsNumber(strTaskID))
    {
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pNode->GetIP()+" ("+pNode->GetIdent()+") NAN");
        return false;
    }
 
    rServer.SendTo(pNode, "GETTASK,"+rServer.GetTaskHistory(std::stoull(strTaskID))+"\n");
    return true;
}