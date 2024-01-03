#include "CCommandAddTask.h"
#include "CServer.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandAddTask
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CCommandAddTask::CCommandAddTask()
{
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommandAddTask
//  - Prototype :
//
//  - Purpose   : Validates and executes the command
//
// -----------------------------------------------------------------------------
bool CCommandAddTask::Validate(const std::vector<std::string>& vData,CServer& rServer, CNode *pNode) 
{
    // Few mandatory validations
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
    
    if(vData.size() != 3)
    {                     
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Wrong command size");
        return false;
    }  

    std::string strScript       = vData[1];  // Text  
    std::string strPayload      = vData[2];  // Base64 

    if(strScript.empty())
    {                                               
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Wrong script");
        return false;
    }
    if(!CSystemUtil::ContainsOnlyAllowedCharacters(strScript,rServer.GetConfigParameter("FilenameAllowedCharacters")))
    {                                   
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Bad script name"); 
        return false;
    }  
    if(rServer.IsCoreTask(strScript) && !pUser->IsValidator())
    {  
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Disallowed"); 
        return false;             
    }     
    if(strPayload.empty())
    {                                            
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Wrong payload");
        return false;
    }               
    if(strPayload.length()>std::stoul(rServer.GetConfigParameter("MaxPayloadLength")))
    {                                         
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Payload len exceeded "+std::to_string(strPayload.length()));
        return false;                         
    } 
    if(!CSystemUtil::FileExists("scripts/"+strScript+".bas"))
    {                                            
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Unknown script");
        return false; 
    } 


    // Does the user happen to have enough shells?
    if(pUser->GetBalance()>=std::stoul(rServer.GetConfigParameter("TaskCreationCost")))
    {
        std::vector<std::string> vCorePayload;
   
        if(strScript == "writedfs")
        {           
            if(!pUser->IsFilesystem())
            {
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Wrong group");         
                return false;                                                   
            } 

            std::string strDecoPayload = CSystemUtil::base64_decode(strPayload);
            std::vector<std::string> vArguments = CSystemUtil::SplitString(strDecoPayload,',');

            if(vArguments.size() != 4)
            {                                            
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Bad arg count"); 
                return false;
            }
            
            std::string strFilename     = vArguments[0];                            
            std::string strAppendFlag   = vArguments[1];
            std::string strText64       = vArguments[2];

            if(strFilename=="")
            {
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Empty fname"); 
                return false;                             
            }

            if(!CSystemUtil::ContainsOnlyAllowedCharacters(strFilename,rServer.GetConfigParameter("FilenameAllowedCharacters")))
            {                                   
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Bad fname"); 
                return false;
            }
            
            if(strAppendFlag == "0" || strAppendFlag == "1")
            {
            }
            else
            {
                strAppendFlag = "1";
            }

            if(strText64=="")
            {
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Empty txt64"); 
                return false;                               
            } 

            if(CSystemUtil::base64_decode(strText64) == "")
            { 
                CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Empty txt"); 
                return false;  
            }

            strPayload = strText64; 
 
            // Check the DFS limits
            uint32_t uiDFSCount     = rServer.GetDFS()->GetCountOfFiles(pUser->GetUsername());
            uint32_t uiSizeToWrite  = CSystemUtil::base64_decode(strText64).size();

            CDFS::tFile *pFile = rServer.GetDFS()->GetFile(pUser->GetUsername()+strFilename);

            // A new file 
            // Check how many telemetry does a user have and break if this exceeds
            if(pFile == nullptr)
            {
                if(uiDFSCount >= pUser->GetMaxFiles())
                {
                    // Do not store
                    rServer.SendTo(pNode, "ADDT,BAD\n" ); 
                    CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Max telemetry reached "+std::to_string(uiDFSCount)); 
                    return true;                                    
                } 
            }
            // An existing file
            // check for file size limit reached
            else
            { 
                if(uiSizeToWrite+pFile->GetSize() > pUser->GetMaxFileSize())
                {
                    rServer.SendTo(pNode, "ADDT,BAD\n" ); 
                    CLogger::GetInstance().Log(SERVER_LOG_WARRNING,pUser->GetUsername()+" ("+pNode->GetIdent()+") Max telemetry size reached "+std::to_string(uiSizeToWrite+pFile->GetSize())+"/"+std::to_string(pUser->GetMaxFileSize())); 
                    return true;                           
                }
            }

            rServer.GetDFS()->Write(pUser->GetUsername(),strFilename,CSystemUtil::base64_decode(strText64), std::stoi(strAppendFlag));  
        }
                    
        // Submit task       
        uint32_t uiTaskID = rServer.SubmitTask(strScript, strPayload, pUser->GetUsername(), true, nullptr, vCorePayload);
                                 
        // Return Task ID
        rServer.SendTo(pNode, "ADDT,"+std::to_string(uiTaskID)+"\n" );  

        // Core validators do not get balance reduced, they are generating tokens
        if(pUser->IsValidator())
        {
            //
        }
        else
        {
            // Update balance
            uint32_t uiBalanceNew = pUser->GetBalance()-std::stoul(rServer.GetConfigParameter("TaskCreationCost"));
            pUser->SetBalance(uiBalanceNew); 
        }
        
        pUser->AddSubmitCount();          
    }
    else
    {
        rServer.SendTo(pNode, "ADDT,BAD\n" );
        CLogger::GetInstance().Log(SERVER_LOG_WARRNING ,"Task not added, not enough balance ("+std::to_string(pUser->GetBalance())+" SH)"); 
    } 

    return true;
}