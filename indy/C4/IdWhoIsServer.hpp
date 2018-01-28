// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdWhoIsServer.pas' rev: 4.00

#ifndef IdWhoIsServerHPP
#define IdWhoIsServerHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdComponent.hpp>	// Pascal unit
#include <IdTCPServer.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idwhoisserver
{
//-- type declarations -------------------------------------------------------
typedef void __fastcall (__closure *TGetEvent)(Idtcpserver::TIdPeerThread* AThread, AnsiString ALookup
	);

class DELPHICLASS TIdWhoIsServer;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdWhoIsServer : public Idtcpserver::TIdTCPServer 
{
	typedef Idtcpserver::TIdTCPServer inherited;
	
protected:
	TGetEvent FOnCommandLookup;
	virtual bool __fastcall DoExecute(Idtcpserver::TIdPeerThread* AThread);
	
public:
	__fastcall virtual TIdWhoIsServer(Classes::TComponent* AOwner);
	
__published:
	__property TGetEvent OnCommandLookup = {read=FOnCommandLookup, write=FOnCommandLookup};
	__property DefaultPort ;
public:
	#pragma option push -w-inl
	/* TIdTCPServer.Destroy */ inline __fastcall virtual ~TIdWhoIsServer(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idwhoisserver */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idwhoisserver;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdWhoIsServer
