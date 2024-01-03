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
#ifndef __CTASK__
#define __CTASK__
#include <assert.h> 
#include <vector>
#include <fstream> 
#include <string>
#include <map> 
class CTask
{
	public:
		enum eState
        {
            TASK_STATE_NEW = 0, 
            TASK_STATE_RUNNING,
            TASK_STATE_FAILED,
            TASK_STATE_COMPLETED ,
            TASK_STATE_VALIDATED
        };
        struct tExecution
        {
        	std::string m_strPayload;		
        	std::string m_strPayloadHash;	
        	std::string m_strNode;			
        	std::string m_strUserHash;		
        	std::string m_strUserName;		
        	uint32_t m_uiStartTime;
        	uint32_t m_uiEndTime; 
        };
		CTask();
		~CTask();

		uint32_t IsValid();
		void AddExecution(const CTask::tExecution& rtExec); 
		void Start();
		void Complete();
		void Fail();
		void Restart();
		void Validate();
		bool Save();

		void SetCreationTime(const uint32_t&ruiValue);
		void SetState(const uint32_t& ruiState);
		void SetID(const uint32_t& ruiTaskID);
		void SetFailCount(const uint32_t& ruiFailCount);
		void SetRewarded(const uint32_t& ruiValue);
		void SetRewardGrant(const uint32_t& ruiValue);
 
		void SetOwner(const std::string& rstrValue);
		void SetPayload(const std::string& rstrPayload);   
		void SetScript(const std::string& rstrValue); 
		void SetSourceHash(const std::string& rstrValue); 


		bool IsFailing(const std::string& rstrNodeHash);
		void AddFailing(const std::string& rstrNodeHash);

 
		uint32_t GetFailCount();
		uint32_t GetExecCount();
		uint32_t GetState();	
		uint32_t GetID(); 
		uint32_t GetStartTime();
		uint32_t GetCreationTime();
		uint32_t IsRewarded();
		uint32_t IsGrantedReward();

		void AddCorePayload(const std::string& rstrData); 
		void SetCorePayload(const std::vector<std::string>& rvPayloadVector);
		std::vector<std::string> GetCorePayload();
		
		std::string GetSourceHash();
		std::string GetPayload();
		std::string GetScript();
		std::string GetOwner();

		std::vector<CTask::tExecution> GetExecutions();

	private:
		uint32_t	m_uiGrantReward;// Flag will tell if we grant reward for validation of this task
		uint32_t	m_uiRewarded;	// Flag will tell if a reward was given for this task
		uint32_t	m_uiFailCount;  // Count of fails
		uint32_t	m_uiRestartCount; // Count of restarts
		uint32_t 	m_uiTaskState;  // State
		uint32_t 	m_uiTaskID;	    // Unique ID
		uint32_t	m_uiStartTime;	// Start 
		uint32_t	m_uiCreationTime; // Created epoch 

		std::string m_strOwner;		// The username
		std::string m_strScript; 	// The script
		std::string m_strSourceHash; // Source hash
		std::string m_strPayload;   // Initial payload
		std::vector<CTask::tExecution> m_vExecutions; // Vector of execs
		std::vector<std::string> m_vCorePayload; // Core internal payload
		std::map<std::string, uint32_t> m_mFailures; // Map of failing nodes+count for this task


};
#endif
