// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdEchoServer.pas' rev: 4.00

#ifndef IdEchoServerHPP
#define IdEchoServerHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdComponent.hpp>	// Pascal unit
#include <IdTCPServer.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idechoserver
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdECHOServer;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdECHOServer : public Idtcpserver::TIdTCPServer 
{
	typedef Idtcpserver::TIdTCPServer inherited;
	
protected:
	virtual bool __fastcall DoExecute(Idtcpserver::TIdPeerThread* AThread);
	
public:
	__fastcall virtual TIdECHOServer(Classes::TComponent* AOwner);
	
__published:
	__property DefaultPort ;
public:
	#pragma option push -w-inl
	/* TIdTCPServer.Destroy */ inline __fastcall virtual ~TIdECHOServer(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idechoserver */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idechoserver;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdEchoServer
