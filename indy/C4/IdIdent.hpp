// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdIdent.pas' rev: 4.00

#ifndef IdIdentHPP
#define IdIdentHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdComponent.hpp>	// Pascal unit
#include <IdTCPConnection.hpp>	// Pascal unit
#include <IdTCPClient.hpp>	// Pascal unit
#include <IdException.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idident
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS EIdIdentException;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdIdentException : public Idexception::EIdException 
{
	typedef Idexception::EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdIdentException(const AnsiString Msg) : Idexception::EIdException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdIdentException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : Idexception::EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdIdentException(int Ident, Extended Dummy) : Idexception::EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdIdentException(int Ident, const System::TVarRec * 
		Args, const int Args_Size) : Idexception::EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdIdentException(const AnsiString Msg, int AHelpContext
		) : Idexception::EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdIdentException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Idexception::EIdException(Msg, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdIdentException(int Ident, int AHelpContext) : Idexception::EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdIdentException(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Idexception::EIdException(Ident, Args, Args_Size, 
		AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdIdentException(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdIdentReply;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdIdentReply : public EIdIdentException 
{
	typedef EIdIdentException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdIdentReply(const AnsiString Msg) : EIdIdentException(Msg
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdIdentReply(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdIdentException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdIdentReply(int Ident, Extended Dummy) : EIdIdentException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdIdentReply(int Ident, const System::TVarRec * Args
		, const int Args_Size) : EIdIdentException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdIdentReply(const AnsiString Msg, int AHelpContext) : 
		EIdIdentException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdIdentReply(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdIdentException(Msg, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdIdentReply(int Ident, int AHelpContext) : EIdIdentException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdIdentReply(int Ident, const System::TVarRec * 
		Args, const int Args_Size, int AHelpContext) : EIdIdentException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdIdentReply(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdIdentInvalidPort;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdIdentInvalidPort : public EIdIdentReply 
{
	typedef EIdIdentReply inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdIdentInvalidPort(const AnsiString Msg) : EIdIdentReply(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdIdentInvalidPort(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdIdentReply(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdIdentInvalidPort(int Ident, Extended Dummy) : EIdIdentReply(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdIdentInvalidPort(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdIdentReply(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdIdentInvalidPort(const AnsiString Msg, int AHelpContext
		) : EIdIdentReply(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdIdentInvalidPort(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdIdentReply(Msg, Args, Args_Size, AHelpContext)
		 { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdIdentInvalidPort(int Ident, int AHelpContext) : 
		EIdIdentReply(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdIdentInvalidPort(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdIdentReply(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdIdentInvalidPort(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdIdentNoUser;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdIdentNoUser : public EIdIdentReply 
{
	typedef EIdIdentReply inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdIdentNoUser(const AnsiString Msg) : EIdIdentReply(Msg) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdIdentNoUser(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdIdentReply(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdIdentNoUser(int Ident, Extended Dummy) : EIdIdentReply(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdIdentNoUser(int Ident, const System::TVarRec * Args
		, const int Args_Size) : EIdIdentReply(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdIdentNoUser(const AnsiString Msg, int AHelpContext)
		 : EIdIdentReply(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdIdentNoUser(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdIdentReply(Msg, Args, Args_Size, AHelpContext)
		 { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdIdentNoUser(int Ident, int AHelpContext) : EIdIdentReply(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdIdentNoUser(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdIdentReply(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdIdentNoUser(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdIdentHiddenUser;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdIdentHiddenUser : public EIdIdentReply 
{
	typedef EIdIdentReply inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdIdentHiddenUser(const AnsiString Msg) : EIdIdentReply(Msg
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdIdentHiddenUser(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdIdentReply(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdIdentHiddenUser(int Ident, Extended Dummy) : EIdIdentReply(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdIdentHiddenUser(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdIdentReply(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdIdentHiddenUser(const AnsiString Msg, int AHelpContext
		) : EIdIdentReply(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdIdentHiddenUser(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdIdentReply(Msg, Args, Args_Size, AHelpContext)
		 { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdIdentHiddenUser(int Ident, int AHelpContext) : EIdIdentReply(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdIdentHiddenUser(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdIdentReply(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdIdentHiddenUser(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdIdentUnknownError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdIdentUnknownError : public EIdIdentReply 
{
	typedef EIdIdentReply inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdIdentUnknownError(const AnsiString Msg) : EIdIdentReply(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdIdentUnknownError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdIdentReply(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdIdentUnknownError(int Ident, Extended Dummy) : EIdIdentReply(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdIdentUnknownError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdIdentReply(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdIdentUnknownError(const AnsiString Msg, int AHelpContext
		) : EIdIdentReply(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdIdentUnknownError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdIdentReply(Msg, Args, Args_Size, AHelpContext)
		 { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdIdentUnknownError(int Ident, int AHelpContext) : 
		EIdIdentReply(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdIdentUnknownError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdIdentReply(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdIdentUnknownError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdIdentQueryTimeOut;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdIdentQueryTimeOut : public EIdIdentReply 
{
	typedef EIdIdentReply inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdIdentQueryTimeOut(const AnsiString Msg) : EIdIdentReply(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdIdentQueryTimeOut(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdIdentReply(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdIdentQueryTimeOut(int Ident, Extended Dummy) : EIdIdentReply(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdIdentQueryTimeOut(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdIdentReply(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdIdentQueryTimeOut(const AnsiString Msg, int AHelpContext
		) : EIdIdentReply(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdIdentQueryTimeOut(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdIdentReply(Msg, Args, Args_Size, AHelpContext)
		 { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdIdentQueryTimeOut(int Ident, int AHelpContext) : 
		EIdIdentReply(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdIdentQueryTimeOut(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdIdentReply(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdIdentQueryTimeOut(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS TIdIdent;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdIdent : public Idtcpclient::TIdTCPClient 
{
	typedef Idtcpclient::TIdTCPClient inherited;
	
protected:
	int FQueryTimeOut;
	AnsiString FReplyString;
	AnsiString __fastcall GetReplyCharset();
	AnsiString __fastcall GetReplyOS();
	AnsiString __fastcall GetReplyOther();
	AnsiString __fastcall GetReplyUserName();
	AnsiString __fastcall FetchUserReply();
	AnsiString __fastcall FetchOS();
	void __fastcall ParseError(void);
	
public:
	__fastcall virtual TIdIdent(Classes::TComponent* AOwner);
	void __fastcall Query(Word APortOnServer, Word APortOnClient);
	__property AnsiString Reply = {read=FReplyString};
	__property AnsiString ReplyCharset = {read=GetReplyCharset};
	__property AnsiString ReplyOS = {read=GetReplyOS};
	__property AnsiString ReplyOther = {read=GetReplyOther};
	__property AnsiString ReplyUserName = {read=GetReplyUserName};
	
__published:
	__property int QueryTimeOut = {read=FQueryTimeOut, write=FQueryTimeOut, default=60000};
	__property Port ;
public:
	#pragma option push -w-inl
	/* TIdTCPClient.Destroy */ inline __fastcall virtual ~TIdIdent(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
static const Word IdIdentQryTimeout = 0xea60;

}	/* namespace Idident */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idident;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdIdent
