// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdTimeUDPServer.pas' rev: 4.00

#ifndef IdTimeUDPServerHPP
#define IdTimeUDPServerHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdComponent.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <IdUDPServer.hpp>	// Pascal unit
#include <IdUDPBase.hpp>	// Pascal unit
#include <IdSocketHandle.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idtimeudpserver
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdTimeUDPServer;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdTimeUDPServer : public Idudpserver::TIdUDPServer 
{
	typedef Idudpserver::TIdUDPServer inherited;
	
protected:
	System::TDateTime FBaseDate;
	virtual void __fastcall DoUDPRead(Classes::TStream* AData, Idsockethandle::TIdSocketHandle* ABinding
		);
	
public:
	__fastcall virtual TIdTimeUDPServer(Classes::TComponent* axOwner);
	
__published:
	__property DefaultPort ;
	__property System::TDateTime BaseDate = {read=FBaseDate, write=FBaseDate};
public:
	#pragma option push -w-inl
	/* TIdUDPServer.Destroy */ inline __fastcall virtual ~TIdTimeUDPServer(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idtimeudpserver */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idtimeudpserver;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdTimeUDPServer
