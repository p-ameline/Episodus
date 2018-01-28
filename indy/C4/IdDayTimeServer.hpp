// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdDayTimeServer.pas' rev: 4.00

#ifndef IdDayTimeServerHPP
#define IdDayTimeServerHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdComponent.hpp>	// Pascal unit
#include <IdTCPServer.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Iddaytimeserver
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdDayTimeServer;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdDayTimeServer : public Idtcpserver::TIdTCPServer 
{
	typedef Idtcpserver::TIdTCPServer inherited;
	
protected:
	AnsiString FTimeZone;
	virtual bool __fastcall DoExecute(Idtcpserver::TIdPeerThread* AThread);
	
public:
	__fastcall virtual TIdDayTimeServer(Classes::TComponent* AOwner);
	
__published:
	__property AnsiString TimeZone = {read=FTimeZone, write=FTimeZone};
	__property DefaultPort ;
public:
	#pragma option push -w-inl
	/* TIdTCPServer.Destroy */ inline __fastcall virtual ~TIdDayTimeServer(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Iddaytimeserver */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Iddaytimeserver;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdDayTimeServer
