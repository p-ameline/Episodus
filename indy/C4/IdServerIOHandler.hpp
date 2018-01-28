// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdServerIOHandler.pas' rev: 4.00

#ifndef IdServerIOHandlerHPP
#define IdServerIOHandlerHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdThread.hpp>	// Pascal unit
#include <IdIOHandler.hpp>	// Pascal unit
#include <IdStackConsts.hpp>	// Pascal unit
#include <IdIOHandlerSocket.hpp>	// Pascal unit
#include <IdComponent.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idserveriohandler
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdServerIOHandler;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdServerIOHandler : public Idcomponent::TIdComponent 
{
	typedef Idcomponent::TIdComponent inherited;
	
public:
	virtual void __fastcall Init(void);
	virtual Idiohandler::TIdIOHandler* __fastcall Accept(int ASocket, Idthread::TIdThread* AThread);
public:
		
	#pragma option push -w-inl
	/* TIdComponent.Create */ inline __fastcall virtual TIdServerIOHandler(Classes::TComponent* axOwner
		) : Idcomponent::TIdComponent(axOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TIdComponent.Destroy */ inline __fastcall virtual ~TIdServerIOHandler(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idserveriohandler */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idserveriohandler;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdServerIOHandler
