// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdQOTDUDP.pas' rev: 4.00

#ifndef IdQOTDUDPHPP
#define IdQOTDUDPHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdComponent.hpp>	// Pascal unit
#include <IdUDPClient.hpp>	// Pascal unit
#include <IdUDPBase.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idqotdudp
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdQOTDUDP;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdQOTDUDP : public Idudpclient::TIdUDPClient 
{
	typedef Idudpclient::TIdUDPClient inherited;
	
protected:
	AnsiString __fastcall GetQuote();
	
public:
	__fastcall virtual TIdQOTDUDP(Classes::TComponent* AOwner);
	__property AnsiString Quote = {read=GetQuote};
	
__published:
	__property Port ;
public:
	#pragma option push -w-inl
	/* TIdUDPBase.Destroy */ inline __fastcall virtual ~TIdQOTDUDP(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idqotdudp */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idqotdudp;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdQOTDUDP
