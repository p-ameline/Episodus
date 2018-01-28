// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdQotd.pas' rev: 4.00

#ifndef IdQotdHPP
#define IdQotdHPP

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

namespace Idqotd
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdQOTD;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdQOTD : public Idtcpclient::TIdTCPClient 
{
	typedef Idtcpclient::TIdTCPClient inherited;
	
protected:
	AnsiString __fastcall GetQuote();
	
public:
	__fastcall virtual TIdQOTD(Classes::TComponent* AOwner);
	__property AnsiString Quote = {read=GetQuote};
	
__published:
	__property Port ;
public:
	#pragma option push -w-inl
	/* TIdTCPClient.Destroy */ inline __fastcall virtual ~TIdQOTD(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idqotd */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idqotd;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdQotd
