// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdDiscardUDPServer.pas' rev: 4.00

#ifndef IdDiscardUDPServerHPP
#define IdDiscardUDPServerHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdComponent.hpp>	// Pascal unit
#include <IdUDPServer.hpp>	// Pascal unit
#include <IdUDPBase.hpp>	// Pascal unit
#include <IdSocketHandle.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Iddiscardudpserver
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdDiscardUDPServer;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdDiscardUDPServer : public Idudpserver::TIdUDPServer 
{
	typedef Idudpserver::TIdUDPServer inherited;
	
public:
	__fastcall virtual TIdDiscardUDPServer(Classes::TComponent* AOwner);
	
__published:
	__property DefaultPort ;
public:
	#pragma option push -w-inl
	/* TIdUDPServer.Destroy */ inline __fastcall virtual ~TIdDiscardUDPServer(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Iddiscardudpserver */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Iddiscardudpserver;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdDiscardUDPServer
