// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdWhois.pas' rev: 4.00

#ifndef IdWhoisHPP
#define IdWhoisHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdComponent.hpp>	// Pascal unit
#include <IdTCPConnection.hpp>	// Pascal unit
#include <IdTCPClient.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idwhois
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdWhois;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdWhois : public Idtcpclient::TIdTCPClient 
{
	typedef Idtcpclient::TIdTCPClient inherited;
	
public:
	__fastcall virtual TIdWhois(Classes::TComponent* AOwner);
	AnsiString __fastcall WhoIs(const AnsiString ADomain);
public:
	#pragma option push -w-inl
	/* TIdTCPClient.Destroy */ inline __fastcall virtual ~TIdWhois(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idwhois */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idwhois;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdWhois
