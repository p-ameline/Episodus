// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdMessageCoderXXE.pas' rev: 4.00

#ifndef IdMessageCoderXXEHPP
#define IdMessageCoderXXEHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdComponent.hpp>	// Pascal unit
#include <IdMessage.hpp>	// Pascal unit
#include <IdMessageCoder.hpp>	// Pascal unit
#include <IdMessageCoderUUE.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idmessagecoderxxe
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdMessageEncoderXXE;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdMessageEncoderXXE : public Idmessagecoderuue::TIdMessageEncoderUUEBase 
	
{
	typedef Idmessagecoderuue::TIdMessageEncoderUUEBase inherited;
	
public:
	__fastcall virtual TIdMessageEncoderXXE(Classes::TComponent* AOwner);
public:
	#pragma option push -w-inl
	/* TIdComponent.Destroy */ inline __fastcall virtual ~TIdMessageEncoderXXE(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS TIdMessageEncoderInfoXXE;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdMessageEncoderInfoXXE : public Idmessagecoder::TIdMessageEncoderInfo 
{
	typedef Idmessagecoder::TIdMessageEncoderInfo inherited;
	
public:
	__fastcall virtual TIdMessageEncoderInfoXXE(void);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TIdMessageEncoderInfoXXE(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idmessagecoderxxe */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idmessagecoderxxe;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdMessageCoderXXE
