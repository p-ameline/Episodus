// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdDayTimeUDP.pas' rev: 4.00

#ifndef IdDayTimeUDPHPP
#define IdDayTimeUDPHPP

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

namespace Iddaytimeudp
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdDayTimeUDP;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdDayTimeUDP : public Idudpclient::TIdUDPClient 
{
	typedef Idudpclient::TIdUDPClient inherited;
	
protected:
	AnsiString __fastcall GetDayTimeStr();
	
public:
	__fastcall virtual TIdDayTimeUDP(Classes::TComponent* AOwner);
	__property AnsiString DayTimeStr = {read=GetDayTimeStr};
	
__published:
	__property Port ;
public:
	#pragma option push -w-inl
	/* TIdUDPBase.Destroy */ inline __fastcall virtual ~TIdDayTimeUDP(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Iddaytimeudp */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Iddaytimeudp;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdDayTimeUDP
