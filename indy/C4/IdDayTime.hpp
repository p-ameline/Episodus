// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdDayTime.pas' rev: 4.00

#ifndef IdDayTimeHPP
#define IdDayTimeHPP

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

namespace Iddaytime
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdDayTime;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdDayTime : public Idtcpclient::TIdTCPClient 
{
	typedef Idtcpclient::TIdTCPClient inherited;
	
protected:
	AnsiString __fastcall GetDayTimeStr();
	
public:
	__fastcall virtual TIdDayTime(Classes::TComponent* AOwner);
	__property AnsiString DayTimeStr = {read=GetDayTimeStr};
	
__published:
	__property Port ;
public:
	#pragma option push -w-inl
	/* TIdTCPClient.Destroy */ inline __fastcall virtual ~TIdDayTime(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Iddaytime */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Iddaytime;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdDayTime
