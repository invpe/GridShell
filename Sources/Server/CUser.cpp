#include "CUser.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : CTOR
//
// ----------------------------------------------------------------------------- 
CUser::CUser()
{ 	 
	m_uiCreated = 0;
	m_uiBalance = 0;  
	m_uiRank = 0; 
	m_uiLastSeen = 0;  
	m_uiFilesystem = 0;
	m_uiValidator = 0;
	m_uiSubmitCount = 0;
	m_uiTotalValidExecs = 0;
	m_uiRetention = 0;
	m_uiLastCommandTime = 0;
	m_uiMaxFilesCount = 0;
	m_uiMaxFileSize = 0;

	// Clear
	for(uint32_t a = 0; a < CUser::eAwardCategories::AWARD_TOTAL;a++)
	{
		m_mAwards[static_cast<eAwardCategories>(a)] = MEDAL_NONE;
	}

}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Return assigned medal for a specified category
//
// -----------------------------------------------------------------------------
CUser::eAwardMedal CUser::GetMedal(const CUser::eAwardCategories& rCategory)
{ 
	return m_mAwards[rCategory];
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Update the awards for this user
//
// -----------------------------------------------------------------------------
void CUser::UpdateAwards(const uint32_t& ruiNodes)
{
	for(uint32_t a = 0; a < CUser::eAwardCategories::AWARD_TOTAL;a++)
	{
		switch(a)
		{
			case CUser::eAwardCategories::AWARD_SUBMISSIONS:
			{
			    if (m_uiSubmitCount >= 1000)
			        m_mAwards[CUser::eAwardCategories::AWARD_SUBMISSIONS] = CUser::eAwardMedal::MEDAL_GOLD;
			    else if (m_uiSubmitCount >= 100)
			        m_mAwards[CUser::eAwardCategories::AWARD_SUBMISSIONS] = CUser::eAwardMedal::MEDAL_SILVER;
			    else if (m_uiSubmitCount > 0)
			        m_mAwards[CUser::eAwardCategories::AWARD_SUBMISSIONS] = CUser::eAwardMedal::MEDAL_BRONZE;
			    else
			        m_mAwards[CUser::eAwardCategories::AWARD_SUBMISSIONS] = CUser::eAwardMedal::MEDAL_NONE;
			}	
			break;
			case CUser::eAwardCategories::AWARD_EXECUTION:
			{
			    if (m_uiTotalValidExecs >= 1000)
			        m_mAwards[CUser::eAwardCategories::AWARD_EXECUTION] = CUser::eAwardMedal::MEDAL_GOLD;
			    else if (m_uiTotalValidExecs >= 100)
			        m_mAwards[CUser::eAwardCategories::AWARD_EXECUTION] = CUser::eAwardMedal::MEDAL_SILVER;
			    else if (m_uiTotalValidExecs > 0)
			        m_mAwards[CUser::eAwardCategories::AWARD_EXECUTION] = CUser::eAwardMedal::MEDAL_BRONZE;
			    else
			        m_mAwards[CUser::eAwardCategories::AWARD_EXECUTION] = CUser::eAwardMedal::MEDAL_NONE;
			}	
			break;
 			case CUser::eAwardCategories::AWARD_NODES:
			{ 
				if (ruiNodes >= 50)
			        m_mAwards[CUser::eAwardCategories::AWARD_NODES] = CUser::eAwardMedal::MEDAL_GOLD;
			    else if (ruiNodes >= 10)
			        m_mAwards[CUser::eAwardCategories::AWARD_NODES] = CUser::eAwardMedal::MEDAL_SILVER;
			    else if (ruiNodes > 0)
			        m_mAwards[CUser::eAwardCategories::AWARD_NODES] = CUser::eAwardMedal::MEDAL_BRONZE;
			    else
			        m_mAwards[CUser::eAwardCategories::AWARD_NODES] = CUser::eAwardMedal::MEDAL_NONE;
			}	
			break;	
			case CUser::eAwardCategories::AWARD_SHELLS:
			{
			    if (m_uiBalance >= 10000)
			        m_mAwards[CUser::eAwardCategories::AWARD_SHELLS] = CUser::eAwardMedal::MEDAL_GOLD;
			    else if (m_uiBalance >= 1000)
			        m_mAwards[CUser::eAwardCategories::AWARD_SHELLS] = CUser::eAwardMedal::MEDAL_SILVER;
			    else if (m_uiBalance > 100)
			        m_mAwards[CUser::eAwardCategories::AWARD_SHELLS] = CUser::eAwardMedal::MEDAL_BRONZE;
			    else
			        m_mAwards[CUser::eAwardCategories::AWARD_SHELLS] = CUser::eAwardMedal::MEDAL_NONE;
			}	
			break;				
		}
	}
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Update the timestamp of last command
//
// -----------------------------------------------------------------------------
void CUser::UpdateLastCommand()
{
	m_uiLastCommandTime = time(0);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Return the last command timestamp
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetLastCommandTime()
{
	return m_uiLastCommandTime;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Return rank
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetRank()
{
	return m_uiRank;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set rank
//
// -----------------------------------------------------------------------------
void CUser::SetRank(const std::uint32_t& ruiValue)
{
	m_uiRank = ruiValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Add number of submissions
//
// -----------------------------------------------------------------------------
void CUser::AddSubmitCount()
{
	m_uiSubmitCount++;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set number of submissions
//
// -----------------------------------------------------------------------------
void CUser::SetSubmitCount(const uint32_t& ruiValue)
{
	m_uiSubmitCount = ruiValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Get number of submissions
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetSubmitCount()
{
	return m_uiSubmitCount;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Return telemetry size
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetMaxFileSize()
{
	return m_uiMaxFileSize;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Return telemetry count
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetMaxFiles()
{
	return m_uiMaxFilesCount;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set telemetry size
//
// -----------------------------------------------------------------------------
void CUser::SetMaxFileSize(const uint32_t& ruiValue)
{
	m_uiMaxFileSize = ruiValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set telemetry count
//
// -----------------------------------------------------------------------------
void CUser::SetMaxFiles(const uint32_t& ruiValue)
{
	m_uiMaxFilesCount = ruiValue;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set allowed to use DFS flag
//
// -----------------------------------------------------------------------------
void CUser::SetFilesystem(const uint32_t& ruiValue)
{
	m_uiFilesystem = ruiValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set validator flag
//
// -----------------------------------------------------------------------------
void CUser::SetValidator(const uint32_t& ruiValue)
{
	m_uiValidator = ruiValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Is allowed for DFS 
//
// -----------------------------------------------------------------------------
uint32_t CUser::IsFilesystem()
{
	return m_uiFilesystem;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Is this a validator  
//
// -----------------------------------------------------------------------------
uint32_t CUser::IsValidator()
{
	return m_uiValidator;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Return UPF 
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetUPF()
{
	// New user, empty UPF history
	if(m_vGoodBad.size() == 0)
		return 100; 

	return GetGood();
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Add shells balance
//
// -----------------------------------------------------------------------------
void CUser::AddBalance(const uint32_t& ruiValue)
{ 
	m_uiBalance += ruiValue;  
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Return username
//
// -----------------------------------------------------------------------------
std::string CUser::GetUsername()
{
	return m_strUsername;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set username 
//
// -----------------------------------------------------------------------------
void CUser::SetUsername(const std::string& rstrName)
{
	m_strUsername = rstrName;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set user hash 
//
// -----------------------------------------------------------------------------
void CUser::SetHash(const std::string& rstrHash)
{
	m_strHash = rstrHash;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set user shells balance 
//
// -----------------------------------------------------------------------------
void CUser::SetBalance(const uint32_t& ruiBalance)
{
	m_uiBalance = ruiBalance;
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Return shells balance 
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetBalance()
{
	return m_uiBalance;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Return user hash
//
// -----------------------------------------------------------------------------
std::string CUser::GetHash()
{ 
	return m_strHash;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set user creation time 
//
// -----------------------------------------------------------------------------
void CUser::SetCreatedTime(const uint32_t& ruiCreated)
{
	m_uiCreated = ruiCreated;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set last seen time  
//
// -----------------------------------------------------------------------------
void CUser::SetLastSeen(const uint32_t& ruiLastSeen)
{
	m_uiLastSeen = ruiLastSeen;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Get creation time 
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetCreatedTime()
{
	return m_uiCreated;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Get last seen time
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetLastSeenTime()
{
	return m_uiLastSeen;
}    
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Add UPF value
//
// -----------------------------------------------------------------------------
void CUser::AddUPF(const uint32_t& ruiValue, const uint32_t& ruiUPFExecutionsCount)
{
	m_vGoodBad.push_back(ruiValue);
 
    if(m_vGoodBad.size()>ruiUPFExecutionsCount)
        m_vGoodBad.erase(m_vGoodBad.begin());     
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Get Good UPF count
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetGood()
{
	uint32_t uiGood = 0;
	for(uint32_t x = 0; x <static_cast<uint32_t>(m_vGoodBad.size()); x++)
		if(m_vGoodBad[x]==1)uiGood++;

	return uiGood;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Get bad UPF count
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetBad()
{
	uint32_t uiBad = 0;
	for(uint32_t x = 0; x <static_cast<uint32_t>(m_vGoodBad.size()); x++)
		if(m_vGoodBad[x]==0)uiBad++;

	return uiBad;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Get a number of total valid executions
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetTotals()
{
	return m_uiTotalValidExecs;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Add a number to valid executions
//
// -----------------------------------------------------------------------------
void CUser::AddTotals()
{
	m_uiTotalValidExecs++;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set total valid executions
//
// -----------------------------------------------------------------------------
void CUser::SetTotals(const uint32_t& ruiValue)
{
	m_uiTotalValidExecs = ruiValue;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Serialize
//
// -----------------------------------------------------------------------------
std::vector<uint8_t> CUser::GetSerialized()
{
	CSerializer _Ser; 

	_Ser.Write_string(m_strUsername); 
	_Ser.Write_string(m_strHash);	
	_Ser.Write_uint32(m_uiBalance); 
	_Ser.Write_uint32(m_uiCreated);
	_Ser.Write_uint32(m_uiLastSeen);  
	_Ser.Write_uint32(GetGood());  
	_Ser.Write_uint32(GetBad());  
	_Ser.Write_uint32(m_uiValidator);  
	_Ser.Write_uint32(m_uiFilesystem); 
	_Ser.Write_uint32(m_uiRetention); 
	_Ser.Write_uint32(m_uiMaxFilesCount); 
	_Ser.Write_uint32(m_uiMaxFileSize);
	_Ser.Write_uint32(m_uiTotalValidExecs);	
	_Ser.Write_uint32(m_uiSubmitCount);

	return _Ser.GetBuffer();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Get telemetry retention in seconds
//
// -----------------------------------------------------------------------------
uint32_t CUser::GetRetention()
{
	return m_uiRetention;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Set telemetry retention in seconds
//
// -----------------------------------------------------------------------------
void CUser::SetRetention(const uint32_t& ruiValue)
{
	m_uiRetention = ruiValue;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : Return the UPF vector
//
// -----------------------------------------------------------------------------
std::vector<uint32_t> CUser::GetGoodBad()
{
	return m_vGoodBad;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CUser
//  - Prototype :
//
//  - Purpose   : DTOR
//
// -----------------------------------------------------------------------------
CUser::~CUser()
{

}