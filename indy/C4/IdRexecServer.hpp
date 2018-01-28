// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdRexecServer.pas' rev: 4.00

#ifndef IdRexecServerHPP
#define IdRexecServerHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdComponent.hpp>	// Pascal unit
#include <IdTCPServer.hpp>	// Pascal unit
#include <IdTCPClient.hpp>	// Pascal unit
#include <IdRemoteCMDServer.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idrexecserver
{
//-- type declarations -------------------------------------------------------
typedef void __fastcall (__closure *TIdRexecCommandEvent)(Idtcpserver::TIdPeerThread* AThread, Idtcpclient::TIdTCPClient* 
	AStdError, AnsiString AUserName, AnsiString APassword, AnsiString ACommand);

class DELPHICLASS TIdRexecServer;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdRexecServer : public Idremotecmdserver::TIdRemoteCMDServer 
{
	typedef Idremotecmdserver::TIdRemoteCMDServer inherited;
	
protected:
	TIdRexecCommandEvent FOnCommand;
	virtual void __fastcall DoCMD(Idtcpserver::TIdPeerThread* AThread, Idtcpclient::TIdTCPClient* AStdError
		, AnsiString AParam1, AnsiString AParam2, AnsiString ACommand);
	
public:
	__fastcall virtual TIdRexecServer(Classes::TComponent* AOwner);
	
__published:
	__property TIdRexecCommandEvent OnCommand = {read=FOnCommand, write=FOnCommand};
	__property DefaultPort ;
public:
	#pragma option push -w-inl
	/* TIdTCPServer.Destroy */ inline __fastcall virtual ~TIdRexecServer(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idrexecserver */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idrexecserver;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdRexecServer
