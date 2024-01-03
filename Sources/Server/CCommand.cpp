#include "CCommand.h"
#include "CServer.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommand
//  - Prototype :
//
//  - Purpose   : CTOR
//
// -----------------------------------------------------------------------------
CCommand::CCommand()
{ 
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommand
//  - Prototype :
//
//  - Purpose   :  
//
// -----------------------------------------------------------------------------
bool CCommand::Validate(const std::vector<std::string>& vData,CServer& rServer, CNode *pNode)
{
	return false;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CCommand
//  - Prototype :
//
//  - Purpose   : DTOR
//
// -----------------------------------------------------------------------------
CCommand::~CCommand()
{

}
