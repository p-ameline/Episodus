// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdTimeUDP.pas' rev: 4.00

#ifndef IdTimeUDPHPP
#define IdTimeUDPHPP

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

namespace Idtimeudp
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdTimeUDP;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdTimeUDP : public Idudpclient::TIdUDPClient 
{
	typedef Idudpclient::TIdUDPClient inherited;
	
protected:
	System::TDateTime FBaseDate;
	unsigned FRoundTripDelay;
	unsigned __fastcall GetDateTimeCard(void);
	System::TDateTime __fastcall GetDateTime(void);
	
public:
	__fastcall virtual TIdTimeUDP(Classes::TComponent* AOwner);
	bool __fastcall SyncTime(void);
	__property unsigned DateTimeCard = {read=GetDateTimeCard, nodefault};
	__property System::TDateTime DateTime = {read=GetDateTime};
	__property unsigned RoundTripDelay = {read=FRoundTripDelay, nodefault};
	
__published:
	__property System::TDateTime BaseDate = {read=FBaseDate, write=FBaseDate};
	__property Port ;
public:
	#pragma option push -w-inl
	/* TIdUDPBase.Destroy */ inline __fastcall virtual ~TIdTimeUDP(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
static const Shortint TIMEUDP_BASEDATE = 0x2;

}	/* namespace Idtimeudp */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idtimeudp;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdTimeUDP
