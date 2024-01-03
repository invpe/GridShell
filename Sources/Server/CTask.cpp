#include "CTask.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CTask::CTask()
{
	m_uiRestartCount = 0;
	m_uiStartTime   = 0; 
	m_uiFailCount	= 0;
	m_uiTaskID 		= 0;
	m_uiRewarded    = 0;
	m_uiCreationTime = 0;
	m_uiGrantReward = 1;
	m_uiTaskState 	= CTask::eState::TASK_STATE_NEW;  	
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Set core payload for this task
//
// -----------------------------------------------------------------------------
void CTask::SetCorePayload(const std::vector<std::string>& rvPayloadVector)
{
	m_vCorePayload = rvPayloadVector;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Add core payload for this task
//
// -----------------------------------------------------------------------------
void CTask::AddCorePayload(const std::string& rstrData)
{
	m_vCorePayload.push_back(rstrData);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return core payload for this task
//
// -----------------------------------------------------------------------------
std::vector<std::string> CTask::GetCorePayload()
{
	return m_vCorePayload;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Set reward granted for this task completion
//
// -----------------------------------------------------------------------------
void CTask::SetRewardGrant(const uint32_t& ruiValue)
{
	m_uiGrantReward = ruiValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return reward grant flag
//
// -----------------------------------------------------------------------------
uint32_t CTask::IsGrantedReward()
{
	return m_uiGrantReward;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return owner of this task
//
// -----------------------------------------------------------------------------
std::string CTask::GetOwner()
{
	return m_strOwner;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return creation timestamp
//
// -----------------------------------------------------------------------------
uint32_t CTask::GetCreationTime()
{
	return m_uiCreationTime;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return script name
//
// -----------------------------------------------------------------------------
std::string CTask::GetScript()
{
	return m_strScript;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return script hash
//
// -----------------------------------------------------------------------------
std::string CTask::GetSourceHash()
{
	return m_strSourceHash;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Save to file
//
// -----------------------------------------------------------------------------
bool CTask::Save()
{
	std::string strName = "task/" + std::to_string(GetID()) + ".json";
    std::ofstream file(strName);

    if (!file.is_open()) {
        return false;
    }

	std::string strOutJson;
	strOutJson = "{\n";
	strOutJson+= "\"ID\": "+std::to_string(GetID())+",\n"; 
	strOutJson+= "\"Owner\": \""+m_strOwner+"\",\n";
	strOutJson+= "\"Script\": \""+m_strScript+"\",\n";
	strOutJson+= "\"Input\": \""+m_strPayload+"\",\n";
	strOutJson+= "\"Reward\": "+std::to_string(m_uiRewarded)+",\n";
	strOutJson+= "\"Fails\": "+std::to_string(m_uiFailCount)+",\n";
	strOutJson+= "\"Restarts\": "+std::to_string(m_uiRestartCount)+",\n";
	strOutJson+= "\"State\": "+std::to_string(m_uiTaskState)+",\n";
	strOutJson+= "\"Created\": "+std::to_string(m_uiCreationTime)+",\n";
	strOutJson+= "\"Execs\": "+std::to_string(GetExecCount()); 
    if(GetExecCount() > 0)
    { 
    	strOutJson += ",\n"; 
		strOutJson += "\"Start\": "+ std::to_string(GetExecutions().front().m_uiStartTime) + ",\n";
		strOutJson += "\"End\": "+ std::to_string(GetExecutions().front().m_uiEndTime) + ",\n";
		strOutJson += "\"ExecUser\": \"" + GetExecutions().front().m_strUserName + "\",\n"; 
		strOutJson += "\"ExecPayload\": \"" + GetExecutions().back().m_strPayload + "\"\n";
    } 
 
	strOutJson += "}";
	file << strOutJson; 
    return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Set creation time
//
// -----------------------------------------------------------------------------
void CTask::SetCreationTime(const uint32_t&ruiValue)
{
	m_uiCreationTime = ruiValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Set the owner of this task
//
// -----------------------------------------------------------------------------
void CTask::SetOwner(const std::string& rstrValue)
{
	m_strOwner = rstrValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Set the script for this task
//
// -----------------------------------------------------------------------------
void CTask::SetScript(const std::string& rstrValue)
{
	m_strScript = rstrValue;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Set task state
//
// -----------------------------------------------------------------------------
void CTask::SetState(const uint32_t& ruiState)
{ 
	m_uiTaskState = ruiState;
}   
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Set if this task has rewarded a node for execution
//
// -----------------------------------------------------------------------------
void CTask::SetRewarded(const uint32_t& ruiValue)
{
	m_uiRewarded = ruiValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return node rewarded flag
//
// -----------------------------------------------------------------------------
uint32_t CTask::IsRewarded()
{
	return m_uiRewarded;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Update the number of fails
//
// -----------------------------------------------------------------------------
void CTask::SetFailCount(const uint32_t& ruiFailCount)
{
	m_uiFailCount = ruiFailCount;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return the start time
//
// -----------------------------------------------------------------------------
uint32_t CTask::GetStartTime()
{
	return m_uiStartTime;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Set task ID
//
// -----------------------------------------------------------------------------
void CTask::SetID(const uint32_t& ruiTaskID)
{
	m_uiTaskID = ruiTaskID;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return executions
//
// -----------------------------------------------------------------------------
std::vector<CTask::tExecution> CTask::GetExecutions()
{
	return m_vExecutions;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Update the state to Validated
//
// -----------------------------------------------------------------------------
void CTask::Validate()
{
	m_uiTaskState = CTask::eState::TASK_STATE_VALIDATED;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Restart state
//
// -----------------------------------------------------------------------------
void CTask::Restart()
{
	m_uiRestartCount++;
	m_uiTaskState = CTask::eState::TASK_STATE_NEW;
	m_uiStartTime = 0;    
	m_uiRewarded = 0;
	m_vExecutions.clear(); 
 
	if(m_uiRestartCount > 3)
	{
		m_uiFailCount = 3;
		Fail();
	}
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Start the task
//
// -----------------------------------------------------------------------------
void CTask::Start()
{
	m_uiTaskState = CTask::eState::TASK_STATE_RUNNING;
	m_uiStartTime = time(0); 
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Complete the task
//
// -----------------------------------------------------------------------------
void CTask::Complete()
{
	m_uiTaskState = CTask::eState::TASK_STATE_COMPLETED;  
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Fail the task
//
// -----------------------------------------------------------------------------
void CTask::Fail()
{
	// Increase fail count
	m_uiFailCount++; 


	// Move to NEW again
	m_uiTaskState = CTask::eState::TASK_STATE_NEW;

	// 
	if(m_uiFailCount>=3) 
	m_uiTaskState = CTask::eState::TASK_STATE_FAILED; 
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return fail count
//
// -----------------------------------------------------------------------------
uint32_t CTask::GetFailCount()
{
	return m_uiFailCount;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Set the task payload
//
// -----------------------------------------------------------------------------
void CTask::SetPayload(const std::string& rstrPayload)
{
	m_strPayload = rstrPayload;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return the state of the task
//
// -----------------------------------------------------------------------------
uint32_t CTask::GetState()
{
	return m_uiTaskState;
}   
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return the ID of the task
//
// -----------------------------------------------------------------------------
uint32_t CTask::GetID()
{
	return m_uiTaskID;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return the valid flag
//
// -----------------------------------------------------------------------------
uint32_t CTask::IsValid()
{
	// A task is valid if his first and last(validator) results are equal AND is marked COMPLETED
	if(m_vExecutions.size() >= 2)
	{ 
		if(m_vExecutions.front().m_strPayloadHash == m_vExecutions.back().m_strPayloadHash)
		{
			return true;
		}
	}

	return false;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Set the source hash
//
// -----------------------------------------------------------------------------
void CTask::SetSourceHash(const std::string& rstrValue)
{
	m_strSourceHash = rstrValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Get the payload
//
// -----------------------------------------------------------------------------
std::string CTask::GetPayload()
{
	return m_strPayload;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Add execution to this task
//
// -----------------------------------------------------------------------------
void CTask::AddExecution(const CTask::tExecution& rtExec)
{
	m_vExecutions.push_back(rtExec);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Return number of executions
//
// -----------------------------------------------------------------------------
uint32_t CTask::GetExecCount()
{
	return m_vExecutions.size();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Add a failing node hash
//
// -----------------------------------------------------------------------------
void CTask::AddFailing(const std::string& rstrNodeHash)
{
	m_mFailures[rstrNodeHash] = GetID();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : Check if a given node is marked failing for this task
//
// -----------------------------------------------------------------------------
bool CTask::IsFailing(const std::string& rstrNodeHash)
{
	if ( m_mFailures.find(rstrNodeHash) == m_mFailures.end() ) 
    {
        return false;
    } 

    return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CTask
//  - Prototype :
//
//  - Purpose   : DTOR
//
// -----------------------------------------------------------------------------
CTask::~CTask()
{

}