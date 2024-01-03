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
#ifndef __CDFS__
#define __CDFS__
#include <vector>
#include <unordered_set>
#include <map>
#include <string>
#include <fstream>
#include "md5.h"
#include "sha1.hpp"
#include "CSystemUtil.h"
#include "CSerializer.h"
#include "CLogger.h"
#define DFS_CHUNK_SIZE 5120
class CDFS
{
	public:
		struct tFileChunk
		{
			tFileChunk()
			{
				m_uiMode = 9;
				auto now = std::chrono::high_resolution_clock::now();
				auto duration = now.time_since_epoch();
				auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
				m_uiSeq = static_cast<uint64_t>(millis);
				m_uiLastIntegrityCheck = 0;
				IntegrityChecked();
			}
			uint32_t GetMode()
			{
				return m_uiMode;
			}
			uint64_t GetSeq()
			{
				return m_uiSeq;
			}
			uint32_t GetSize()
			{
				return m_strContent.size();
			}
			uint32_t GetLastIntegrityCheck()
			{
				return m_uiLastIntegrityCheck;
			}
			uint32_t GetNextIntegrityCheck()
			{
				return m_uiNextIntegrityCheck;
			}
			void IntegrityChecked()
			{
				m_uiLastIntegrityCheck = time(0);
				m_uiNextIntegrityCheck = time(0) + (1+rand()%12 * 3600);
			}
			std::string GetContent()
			{
				return m_strContent;
			}
			std::string GetMD5()
			{				
				MD5 m_tMD5;
				m_tMD5.update(m_strContent.c_str(), m_strContent.size());
				m_tMD5.finalize();
				return m_tMD5.toString(); 
			}
 
			// This is plain text, base64 decoded 
			std::string m_strContent;		
			uint32_t m_uiMode;	
			uint64_t m_uiSeq;		
			uint32_t m_uiNextIntegrityCheck;
			uint32_t m_uiLastIntegrityCheck;

		};
		struct tFile
		{ 
			void ClearChunks(){m_vChunks.clear();}

			uint32_t GetSize()
			{
				uint32_t uiSize = 0;
				for ( auto& chunk : m_vChunks) 
                {
					uiSize += chunk.GetSize();                
				}
				return uiSize;
			}
			std::string GetName(){return m_strFilename;}
			std::string GetMD5(){return "";}
			std::string GetOwner(){return m_strOwner;}
			uint32_t GetChunksCount(){return m_vChunks.size();}
			uint32_t GetCreated(){return m_uiCreated;}
			uint32_t GetUpdated(){return m_uiUpdated;} 

			// Returns vector of chunks
			std::vector<tFileChunk*> GetIntegrityExpiredChunks()
			{
				std::vector<tFileChunk*> vReturn;

				for ( auto& chunk : m_vChunks) 
                {
                	tFileChunk *pChunk = &chunk;
					if(pChunk->GetNextIntegrityCheck() < time(0)) 
						vReturn.push_back(pChunk);
				}

				return vReturn;
			}
			// Returns single chunk
			CDFS::tFileChunk* GetIntegrityExpiredChunk()
			{
				for ( auto& chunk : m_vChunks) 
                {
                	tFileChunk *pChunk = &chunk;
					if(pChunk->GetNextIntegrityCheck() < time(0)) 
						return pChunk;
				}
				return nullptr;
			}			

			CDFS::tFileChunk *GetChunk(const uint64_t& ruiSequenceID)
			{				

				for ( auto& chunk : m_vChunks)  
				{ 
			        if (chunk.GetSeq() == ruiSequenceID)  
			           return &chunk; 
			    }
 
				return nullptr;
			} 
			void WriteToFile(const std::string &data) 
			{
				

		        size_t remainingDataSize = data.size();
		        size_t dataIndex = 0;

		        // Continue until all data is appended
		        while (remainingDataSize > 0) {
		            size_t spaceInCurrentChunk = DFS_CHUNK_SIZE;

		            if (!m_vChunks.empty()) {		            	
		                spaceInCurrentChunk -= m_vChunks.back().GetSize();
		            }else spaceInCurrentChunk = 0;

		            if (spaceInCurrentChunk == 0) {
		            	CDFS::tFileChunk _newchunk;

		                // Current chunk is full, create a new chunk
		                m_vChunks.push_back(_newchunk);		                
		                spaceInCurrentChunk = DFS_CHUNK_SIZE;
		            }

		            // Calculate the amount of data to append in this iteration
		            size_t dataToAppend = std::min(remainingDataSize, spaceInCurrentChunk);

		            m_vChunks.back().m_strContent.append(data, dataIndex, dataToAppend);

		            CLogger::GetInstance().Log(SERVER_LOG_INFO, "DFS Wrote to "+GetName()+" ("+std::to_string(GetSize())+"b) "+std::to_string(data.size())+"b of data Chunk "+std::to_string(m_vChunks.back().GetSeq())+" "+std::to_string( m_vChunks.back().GetSize())+"/"+std::to_string(DFS_CHUNK_SIZE));		            

		            // Update counters
		            dataIndex += dataToAppend;
		            remainingDataSize -= dataToAppend;
		        }


		        // Update file metadata (like last updated time, size, etc.)
		        m_uiUpdated = static_cast<uint32_t>(time(0)); 
		    }


			std::vector<tFileChunk>* GetChunksPointer() {
			    return &m_vChunks;
			}
			tFile()
			{
				m_uiCreated = 0;
				m_uiUpdated = 0;
			}

			//
			
			std::string m_strFilename;
			std::string m_strOwner;			
			uint32_t m_uiCreated;
			uint32_t m_uiUpdated; 
			std::vector<tFileChunk> m_vChunks;
		};


		CDFS();
		~CDFS();

		CDFS::tFileChunk *Write(const std::string& rstrOwner, const std::string& rstrUserFilename, const std::string& rstrChunkData,const bool& rbAppend);
		CDFS::tFileChunk *GetChunk(const uint64_t& ruiSequenceID);
		std::map<std::string,CDFS::tFile> *GetFilesPointer();
		CDFS::tFile *GetFile(const std::string& rstrName);
		CDFS::tFile *GetFileWithChunk(const uint64_t& ruiSequenceID);
		std::vector<CDFS::tFile*> GetFilesForUser(const std::string& rstrUserName); 
		std::vector<CDFS::tFile*> GetFilesForUserRetention(const std::string& rstrUserName, const uint32_t& ruiRetentionTime);  
		std::vector<uint64_t> GetAllCompletedChunks();
		std::pair<CDFS::tFile*,CDFS::tFileChunk*> GetChunkForIntegrity(); 
		uint32_t GetCountOfFiles(const std::string& rstrOwner);		
		uint32_t GetCountOfFiles();
		uint32_t GetTotalCountOfChunks(); 
		uint32_t GetTotalSize();		  
		void RemoveFile(const std::string& rstrFileName, const std::string& rstrPath); 
		void Dump();
		bool Save();
		bool Load();
	private:
		std::map<std::string,CDFS::tFile> m_mFiles;
	 	std::map<std::string,CDFS::tFile>::iterator itFiles; 
	    CSHA1 m_SHA1;  
	    MD5 m_MD5;

};
#endif