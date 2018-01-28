// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdQotdServer.pas' rev: 4.00

#ifndef IdQotdServerHPP
#define IdQotdServerHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdComponent.hpp>	// Pascal unit
#include <IdTCPServer.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idqotdserver
{
//-- type declarations -------------------------------------------------------
typedef void __fastcall (__closure *TIdQOTDGetEvent)(Idtcpserver::TIdPeerThread* Thread);

class DELPHICLASS TIdQOTDServer;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdQOTDServer : public Idtcpserver::TIdTCPServer 
{
	typedef Idtcpserver::TIdTCPServer inherited;
	
protected:
	TIdQOTDGetEvent FOnCommandQOTD;
	virtual bool __fastcall DoExecute(Idtcpserver::TIdPeerThread* Thread);
	
public:
	__fastcall virtual TIdQOTDServer(Classes::TComponent* AOwner);
	
__published:
	__property TIdQOTDGetEvent OnCommandQOTD = {read=FOnCommandQOTD, write=FOnCommandQOTD};
	__property DefaultPort ;
public:
	#pragma option push -w-inl
	/* TIdTCPServer.Destroy */ inline __fastcall virtual ~TIdQOTDServer(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idqotdserver */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idqotdserver;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdQotdServer
