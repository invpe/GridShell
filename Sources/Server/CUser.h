// ------------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2022-2023-2024 https://github.com/invpe/GridShell
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
// -----------------------------------------------------------------------------
#ifndef __CUSER__
#define __CUSER__
#include <stdint.h>
#include <string>
#include <vector> 
#include <map>
#include "CSerializer.h" 
class CUser
{
public: 
	enum eAwardCategories
	{
	    AWARD_SUBMISSIONS = 0,
	    AWARD_EXECUTION,
	    AWARD_NODES,
	    AWARD_SHELLS,
	    AWARD_TOTAL
	};
	enum eAwardMedal
	{
		MEDAL_NONE = 0,
		MEDAL_BRONZE,
		MEDAL_SILVER,
		MEDAL_GOLD,
		MEDAL_TOTAL
	};

	CUser();
	~CUser(); 
	void AddUPF(const uint32_t& ruiValue, const uint32_t& ruiUPFExecutionsCount);	 
	void AddSubmitCount();	
	void SetUsername(const std::string& rstrName); 
	void SetHash(const std::string& rstrHash);
	void SetBalance(const uint32_t& ruiBalance);
	void SetCreatedTime(const uint32_t& ruiCreated);
	void SetLastSeen(const uint32_t& ruiLastSeen);  
	void SetValidator(const uint32_t& ruiValue);
	void SetFilesystem(const uint32_t& ruiValue);
	void SetRetention(const uint32_t& ruiValue);
	void SetMaxFiles(const uint32_t& ruiValue);
	void SetMaxFileSize(const uint32_t& ruiValue);
	void SetTotals(const uint32_t& ruiValue);
	void SetSubmitCount(const uint32_t& ruiValue);
	void SetRank(const uint32_t& ruiValue); 
	std::string GetUsername(); 
	std::string GetHash();
	uint32_t GetRank();
	uint32_t GetSubmitCount();
	uint32_t IsValidator();
	uint32_t IsFilesystem();
	uint32_t GetMaxFileSize();
	uint32_t GetMaxFiles();
	uint32_t GetBalance();   
	uint32_t GetCreatedTime();
	uint32_t GetLastSeenTime(); 
	uint32_t GetRetention();
	uint32_t GetGood();
	uint32_t GetBad();
	uint32_t GetTotals(); 
	uint32_t GetLastCommandTime();
	CUser::eAwardMedal GetMedal(const CUser::eAwardCategories& rCategory);
	std::vector<uint32_t> GetGoodBad();
	std::vector<uint8_t> GetSerialized();  
	uint32_t GetUPF();
	void AddGap();
	void AddTotals();
	void RemoveGap();
	void AddBalance(const uint32_t& ruiValue); 
	void UpdateLastCommand();
	void UpdateAwards(const uint32_t& ruiNodes);
 
private:	
	std::string m_strUsername;		// Username - known to all
	std::string m_strHash;			// User Hash - only known to user, internal refference
	uint32_t m_uiLastCommandTime;	// For excessive commands throttling
	uint32_t m_uiBalance;			// Wallet balance 
	uint32_t m_uiCreated;			// Created epoch
	uint32_t m_uiLastSeen;			// Last seen epoch  
	uint32_t m_uiValidator;			// Validator flag
	uint32_t m_uiFilesystem;		// Can perform filesystem operations
	uint32_t m_uiRetention;			// Filesystem retention time (in sec)
	uint32_t m_uiMaxFilesCount;		// Allowed count of files
	uint32_t m_uiMaxFileSize; 		// Allowed max file size per user
	uint32_t m_uiTotalValidExecs; 	// Total number of valid executions
	uint32_t m_uiSubmitCount;		// Total tasks submitted
	uint32_t m_uiRank;				// The normalized rank value
    std::vector<uint32_t> m_vGoodBad; // Vector of good/bad for UPF 
    std::map<eAwardCategories, eAwardMedal> m_mAwards; // Map of awards


 };

#endif