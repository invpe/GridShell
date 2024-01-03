#include "CDFS.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CDFS::CDFS()
{ 
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Save to file
//
// -----------------------------------------------------------------------------
bool CDFS::Save()
{
	CSerializer _Ser;
     _Ser.Write_uint32(m_mFiles.size());

	for (itFiles = m_mFiles.begin(); itFiles != m_mFiles.end(); ++itFiles)
    { 
    	_Ser.Write_string(itFiles->second.GetName());
    	_Ser.Write_string(itFiles->second.GetOwner());
    	_Ser.Write_uint32(itFiles->second.GetCreated());
    	_Ser.Write_uint32(itFiles->second.GetUpdated());

        _Ser.Write_uint32(itFiles->second.GetChunksCount());
        std::vector<tFileChunk>::iterator itChunks;
        for(itChunks = itFiles->second.m_vChunks.begin(); itChunks != itFiles->second.m_vChunks.end(); ++itChunks)
        {
            _Ser.Write_string(itChunks->GetContent());  
            _Ser.Write_uint64(itChunks->GetSeq());
            _Ser.Write_uint32(itChunks->GetMode());
        }

    }

    std::string filename = "dfs.dat";
    std::ofstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        return false;
    }

    const std::vector<uint8_t>* pBuffer = _Ser.GetBufferPtr();

    // Ensure that pBuffer is not a nullptr
    if (pBuffer == nullptr) return false;

    // Directly write the buffer to the file
    file.write(reinterpret_cast<const char*>(pBuffer->data()), pBuffer->size());
    
    return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Load from file
//
// -----------------------------------------------------------------------------
bool CDFS::Load()
{
    std::string filename = "dfs.dat";
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> vSerialized(size);
    if (!file.read(reinterpret_cast<char*>(&vSerialized[0]), size)) {
        return false;
    }

    CSerializer _Ser;
    _Ser.Set(vSerialized);

 	uint32_t uiFilesCount   = _Ser.Read_uint32(); 

    for(uint32_t x = 0; x < uiFilesCount; x++)
    { 
    	std::string strFName = _Ser.Read_string();
    	std::string strFOwner = _Ser.Read_string();
    	uint32_t uiCreated = _Ser.Read_uint32();
    	uint32_t uiUpdated = _Ser.Read_uint32(); 
        // Update the file 
        m_mFiles[strFOwner+strFName].m_strFilename = strFName;
        m_mFiles[strFOwner+strFName].m_strOwner = strFOwner; 
        m_mFiles[strFOwner+strFName].m_uiCreated = uiCreated;
        m_mFiles[strFOwner+strFName].m_uiUpdated = uiUpdated;

        uint32_t uiChunks= _Ser.Read_uint32();
    	for(uint32_t y = 0; y < uiChunks; y++)
    	{ 
    		std::string strCContent = _Ser.Read_string();
    		uint64_t uiSeq =_Ser.Read_uint64();
            uint32_t uiMode =_Ser.Read_uint32();

            // Add chunk
            tFileChunk _Chunk; 
            _Chunk.m_strContent = strCContent;
            _Chunk.m_uiMode = uiMode;
            _Chunk.m_uiSeq = uiSeq;
            m_mFiles[strFOwner+strFName].m_vChunks.push_back(_Chunk);    		
    	}
    }
 
    Dump();
	return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Write to chunk
//
// -----------------------------------------------------------------------------
CDFS::tFileChunk *CDFS::Write(const std::string& rstrOwner, const std::string& rstrUserFilename, const std::string& rstrChunkData,  const bool& rbAppend)
{ 
	// Assign full name
	std::string strFileName = rstrOwner+rstrUserFilename;

	// File Not found, create
  	if ( m_mFiles.find(strFileName) == m_mFiles.end() ) 
    {      	
      	CDFS::tFile _New;
      	_New.m_strFilename  	= rstrUserFilename;
      	_New.m_strOwner 		= rstrOwner;
      	_New.m_uiCreated 		= time(0);
      	_New.m_uiUpdated 		= time(0);
      	m_mFiles[strFileName] 	= _New;
    }

    // Overwriting
    if(!rbAppend)
    {
        m_mFiles[strFileName].m_uiCreated = time(0);
        m_mFiles[strFileName].m_uiUpdated = time(0);

        // New chunk will be created with new seq id since we're clearing new ones due to overwrite
    	m_mFiles[strFileName].ClearChunks();  
    }
    // Appending
    else m_mFiles[strFileName].m_uiUpdated = time(0);

    // Finally write to the file
    m_mFiles[strFileName].WriteToFile(rstrChunkData);   
  
	std::ofstream out("fs/" + strFileName, rbAppend ? (std::ios_base::out | std::ios_base::app) : std::ios_base::out);
	out << rstrChunkData;
	out.close(); 
 
	return &m_mFiles[strFileName].m_vChunks.back();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Count of chunks
//
// -----------------------------------------------------------------------------
uint32_t CDFS::GetTotalCountOfChunks() {
    uint32_t uiTotal = 0;
    for ( auto& pair : m_mFiles) 
    {  
        uiTotal += pair.second.m_vChunks.size();
    }
    return uiTotal;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Count of files
//
// -----------------------------------------------------------------------------
uint32_t CDFS::GetCountOfFiles()
{
    return m_mFiles.size();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Count of files for the owner
//
// -----------------------------------------------------------------------------
uint32_t CDFS::GetCountOfFiles(const std::string& rstrOwner) {
    uint32_t uiTotal = 0;
    for ( auto& pair : m_mFiles) { 
        if (pair.second.m_strOwner == rstrOwner) {
            uiTotal++;
        }
    }
    return uiTotal;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Return all files with a specific chunk
//
// -----------------------------------------------------------------------------
CDFS::tFile *CDFS::GetFileWithChunk(const uint64_t& ruiSequenceID)
{ 
	for (itFiles = m_mFiles.begin(); itFiles != m_mFiles.end(); ++itFiles)
    { 
        CDFS::tFileChunk *pChunk = itFiles->second.GetChunk(ruiSequenceID);

        if(pChunk != nullptr)
            return &(itFiles->second);
    }
    return nullptr;
}  
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Remove file
//
// -----------------------------------------------------------------------------
void CDFS::RemoveFile(const std::string& rstrFileName, const std::string& rstrPath)
{
	if ( m_mFiles.find(rstrFileName) != m_mFiles.end() ) 
    {
    	CSystemUtil::DeleteFile(rstrPath+rstrFileName);
    	m_mFiles.erase(rstrFileName);
    }
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Get a telemetry file
//
// -----------------------------------------------------------------------------
CDFS::tFile *CDFS::GetFile(const std::string& rstrName)
{
	if(m_mFiles.find(rstrName) == m_mFiles.end() )
		return nullptr;

	return &m_mFiles[rstrName];
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Get files for the specified user
//
// -----------------------------------------------------------------------------
std::vector<CDFS::tFile*> CDFS::GetFilesForUser(const std::string& rstrUserName) {
    std::vector<CDFS::tFile*> vFiles;

    for ( auto& pair : m_mFiles) {  
        if (pair.second.GetOwner() == rstrUserName) {
            vFiles.push_back(&pair.second);
        }
    }

    return vFiles;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Get files that are for retention
//
// -----------------------------------------------------------------------------
std::vector<CDFS::tFile*> CDFS::GetFilesForUserRetention(const std::string& rstrUserName, const uint32_t& ruiRetentionTime)
{
    std::vector<CDFS::tFile*> vFiles;

    for ( auto& pair : m_mFiles) {  
        if (pair.second.GetOwner() == rstrUserName &&  (time(0) - pair.second.m_uiCreated > ruiRetentionTime) ) {
            vFiles.push_back(&pair.second);
        }
    }

    return vFiles;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Return a specific chunk
//
// -----------------------------------------------------------------------------
CDFS::tFileChunk *CDFS::GetChunk(const uint64_t& ruiSequenceID) 
{
    for ( auto& pair : m_mFiles) 
    { 
         tFile* pFile = &pair.second;

        for ( auto& chunk : pFile->m_vChunks) 
        { 
            if (chunk.GetSeq() == ruiSequenceID) 
            {
               return &chunk;
            }
        }
    }

    return nullptr;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Return a list of files ready for integrity checks
//
// -----------------------------------------------------------------------------
std::pair<CDFS::tFile*,CDFS::tFileChunk*> CDFS::GetChunkForIntegrity() 
{
    for ( auto& pair : m_mFiles) { 
        
        tFile* pFile = &pair.second;
        CDFS::tFileChunk *pChunk = pFile->GetIntegrityExpiredChunk();
       
        if(pChunk!=nullptr)
            return  std::make_pair(pFile, pChunk);            
    }

    return  std::make_pair(nullptr,nullptr);
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Total weight of DFS
//
// -----------------------------------------------------------------------------
uint32_t CDFS::GetTotalSize() {
    uint32_t uiTotal = 0;
    for ( auto& pair : m_mFiles) { 
        uiTotal += pair.second.GetSize();
    }
    return uiTotal;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Dump it onto the screen
//
// -----------------------------------------------------------------------------
void CDFS::Dump()
{  

    printf("DFS DUMP\n\n");
    for ( auto& pair : m_mFiles) 
    { 
         tFile* pFile = &pair.second;
         printf("DFS: [%s] %s C:%u U:%u Size: %ub Chunks: %u\n", 
            pFile->GetName().data(), 
            pFile->GetOwner().data(), 
            pFile->GetCreated(), 
            pFile->GetUpdated(),
            pFile->GetSize(),
            pFile->GetChunksCount()); 


        for ( auto& chunk : pFile->m_vChunks) 
        { 
            printf("Chunk %lu Size %u\n",
                chunk.GetSeq(),
                chunk.GetSize()
                );

            //printf("%s\n", chunk.GetContent().data());
        }
    }


}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Return chunks that are completed (full)
//
// -----------------------------------------------------------------------------
std::vector<uint64_t> CDFS::GetAllCompletedChunks() 
{
    std::vector<uint64_t> keys;

    // Iterate through all files
    for ( auto& filePair : m_mFiles) {
        // Iterate through all chunks in the current file
        for ( auto& chunk : filePair.second.m_vChunks) { 
            // Check the size condition and add the sequence number to keys
            if (chunk.GetSize() == DFS_CHUNK_SIZE) {
                keys.push_back(chunk.GetSeq());
            }
        }
    }
    return keys;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : Return a pointer to the map
//
// -----------------------------------------------------------------------------
std::map<std::string,CDFS::tFile> *CDFS::GetFilesPointer()
{
    return &m_mFiles;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CDFS
//  - Prototype :
//
//  - Purpose   : DTOR
//
// -----------------------------------------------------------------------------
CDFS::~CDFS()
{
	
}