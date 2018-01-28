// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdException.pas' rev: 4.00

#ifndef IdExceptionHPP
#define IdExceptionHPP

#pragma delphiheader begin
#pragma option push -w-
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idexception
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS EIdException;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdException : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdException(const AnsiString Msg) : Sysutils::Exception(Msg
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdException(int Ident, Extended Dummy) : Sysutils::Exception(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdException(int Ident, const System::TVarRec * Args
		, const int Args_Size) : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdException(const AnsiString Msg, int AHelpContext) : 
		Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdException(int Ident, int AHelpContext) : Sysutils::Exception(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdException(int Ident, const System::TVarRec * 
		Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdException(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

typedef TMetaClass*TClassIdException;

class DELPHICLASS EIdSilentException;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSilentException : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSilentException(const AnsiString Msg) : EIdException(Msg
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSilentException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSilentException(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSilentException(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSilentException(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSilentException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSilentException(int Ident, int AHelpContext) : EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSilentException(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSilentException(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdConnClosedGracefully;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdConnClosedGracefully : public EIdSilentException 
{
	typedef EIdSilentException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdConnClosedGracefully(const AnsiString Msg) : EIdSilentException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdConnClosedGracefully(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdSilentException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdConnClosedGracefully(int Ident, Extended Dummy) : EIdSilentException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdConnClosedGracefully(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSilentException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdConnClosedGracefully(const AnsiString Msg, int AHelpContext
		) : EIdSilentException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdConnClosedGracefully(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSilentException(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdConnClosedGracefully(int Ident, int AHelpContext
		) : EIdSilentException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdConnClosedGracefully(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSilentException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdConnClosedGracefully(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdAlreadyConnected;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdAlreadyConnected : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdAlreadyConnected(const AnsiString Msg) : EIdException(Msg
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdAlreadyConnected(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdAlreadyConnected(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdAlreadyConnected(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdAlreadyConnected(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdAlreadyConnected(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdAlreadyConnected(int Ident, int AHelpContext) : 
		EIdException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdAlreadyConnected(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdAlreadyConnected(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdClosedSocket;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdClosedSocket : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdClosedSocket(const AnsiString Msg) : EIdException(Msg) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdClosedSocket(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdClosedSocket(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdClosedSocket(int Ident, const System::TVarRec * Args
		, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdClosedSocket(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdClosedSocket(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdClosedSocket(int Ident, int AHelpContext) : EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdClosedSocket(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdClosedSocket(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdResponseError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdResponseError : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdResponseError(const AnsiString Msg) : EIdException(Msg)
		 { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdResponseError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdResponseError(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdResponseError(int Ident, const System::TVarRec * 
		Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdResponseError(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdResponseError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdResponseError(int Ident, int AHelpContext) : EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdResponseError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdResponseError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdReadTimeout;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdReadTimeout : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdReadTimeout(const AnsiString Msg) : EIdException(Msg) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdReadTimeout(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdReadTimeout(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdReadTimeout(int Ident, const System::TVarRec * Args
		, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdReadTimeout(const AnsiString Msg, int AHelpContext)
		 : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdReadTimeout(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdReadTimeout(int Ident, int AHelpContext) : EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdReadTimeout(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdReadTimeout(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdReadLnMaxLineLengthExceeded;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdReadLnMaxLineLengthExceeded : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdReadLnMaxLineLengthExceeded(const AnsiString Msg) : EIdException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdReadLnMaxLineLengthExceeded(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdReadLnMaxLineLengthExceeded(int Ident, Extended Dummy
		) : EIdException(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdReadLnMaxLineLengthExceeded(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdReadLnMaxLineLengthExceeded(const AnsiString Msg, int 
		AHelpContext) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdReadLnMaxLineLengthExceeded(const AnsiString Msg
		, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdReadLnMaxLineLengthExceeded(int Ident, int AHelpContext
		) : EIdException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdReadLnMaxLineLengthExceeded(int Ident, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdReadLnMaxLineLengthExceeded(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdInvalidServiceName;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdInvalidServiceName : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdInvalidServiceName(const AnsiString Msg) : EIdException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdInvalidServiceName(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdInvalidServiceName(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdInvalidServiceName(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdInvalidServiceName(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdInvalidServiceName(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdInvalidServiceName(int Ident, int AHelpContext) : 
		EIdException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdInvalidServiceName(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdInvalidServiceName(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdProtocolReplyError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdProtocolReplyError : public EIdException 
{
	typedef EIdException inherited;
	
protected:
	int FReplyErrorCode;
	
public:
	__fastcall virtual EIdProtocolReplyError(const int AErrCode, const AnsiString AReplyMessage);
	__property int ReplyErrorCode = {read=FReplyErrorCode, nodefault};
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdProtocolReplyError(const AnsiString Msg) : EIdException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdProtocolReplyError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdProtocolReplyError(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdProtocolReplyError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdProtocolReplyError(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdProtocolReplyError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdProtocolReplyError(int Ident, int AHelpContext) : 
		EIdException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdProtocolReplyError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdProtocolReplyError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EInvalidSyslogMessage;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EInvalidSyslogMessage : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EInvalidSyslogMessage(const AnsiString Msg) : EIdException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EInvalidSyslogMessage(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EInvalidSyslogMessage(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EInvalidSyslogMessage(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EInvalidSyslogMessage(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EInvalidSyslogMessage(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EInvalidSyslogMessage(int Ident, int AHelpContext) : 
		EIdException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EInvalidSyslogMessage(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EInvalidSyslogMessage(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSSLProtocolReplyError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSSLProtocolReplyError : public EIdProtocolReplyError 
{
	typedef EIdProtocolReplyError inherited;
	
public:
	#pragma option push -w-inl
	/* EIdProtocolReplyError.CreateError */ inline __fastcall virtual EIdSSLProtocolReplyError(const int 
		AErrCode, const AnsiString AReplyMessage) : EIdProtocolReplyError(AErrCode, AReplyMessage) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSSLProtocolReplyError(const AnsiString Msg) : EIdProtocolReplyError(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSSLProtocolReplyError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdProtocolReplyError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSSLProtocolReplyError(int Ident, Extended Dummy) : EIdProtocolReplyError(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSSLProtocolReplyError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdProtocolReplyError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSSLProtocolReplyError(const AnsiString Msg, int AHelpContext
		) : EIdProtocolReplyError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSSLProtocolReplyError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdProtocolReplyError(Msg, Args, 
		Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSSLProtocolReplyError(int Ident, int AHelpContext
		) : EIdProtocolReplyError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSSLProtocolReplyError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdProtocolReplyError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSSLProtocolReplyError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdConnectTimeout;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdConnectTimeout : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdConnectTimeout(const AnsiString Msg) : EIdException(Msg
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdConnectTimeout(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdConnectTimeout(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdConnectTimeout(int Ident, const System::TVarRec * 
		Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdConnectTimeout(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdConnectTimeout(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdConnectTimeout(int Ident, int AHelpContext) : EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdConnectTimeout(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdConnectTimeout(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdConnectException;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdConnectException : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdConnectException(const AnsiString Msg) : EIdException(Msg
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdConnectException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdConnectException(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdConnectException(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdConnectException(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdConnectException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdConnectException(int Ident, int AHelpContext) : 
		EIdException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdConnectException(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdConnectException(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksError : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksError(const AnsiString Msg) : EIdException(Msg) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksError(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksError(int Ident, const System::TVarRec * Args
		, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksError(const AnsiString Msg, int AHelpContext) : 
		EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksError(int Ident, int AHelpContext) : EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksError(int Ident, const System::TVarRec * 
		Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksRequestFailed;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksRequestFailed : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksRequestFailed(const AnsiString Msg) : EIdSocksError(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksRequestFailed(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksRequestFailed(int Ident, Extended Dummy) : EIdSocksError(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksRequestFailed(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksRequestFailed(const AnsiString Msg, int AHelpContext
		) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksRequestFailed(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args, Args_Size, AHelpContext)
		 { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksRequestFailed(int Ident, int AHelpContext) : 
		EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksRequestFailed(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksRequestFailed(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksRequestServerFailed;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksRequestServerFailed : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksRequestServerFailed(const AnsiString Msg) : EIdSocksError(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksRequestServerFailed(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksRequestServerFailed(int Ident, Extended Dummy) : 
		EIdSocksError(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksRequestServerFailed(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksRequestServerFailed(const AnsiString Msg, int 
		AHelpContext) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksRequestServerFailed(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksRequestServerFailed(int Ident, int AHelpContext
		) : EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksRequestServerFailed(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksRequestServerFailed(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksRequestIdentFailed;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksRequestIdentFailed : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksRequestIdentFailed(const AnsiString Msg) : EIdSocksError(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksRequestIdentFailed(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksRequestIdentFailed(int Ident, Extended Dummy) : 
		EIdSocksError(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksRequestIdentFailed(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksRequestIdentFailed(const AnsiString Msg, int AHelpContext
		) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksRequestIdentFailed(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksRequestIdentFailed(int Ident, int AHelpContext
		) : EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksRequestIdentFailed(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksRequestIdentFailed(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksUnknownError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksUnknownError : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksUnknownError(const AnsiString Msg) : EIdSocksError(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksUnknownError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksUnknownError(int Ident, Extended Dummy) : EIdSocksError(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksUnknownError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksUnknownError(const AnsiString Msg, int AHelpContext
		) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksUnknownError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args, Args_Size, AHelpContext)
		 { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksUnknownError(int Ident, int AHelpContext) : 
		EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksUnknownError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksUnknownError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksServerRespondError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksServerRespondError : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksServerRespondError(const AnsiString Msg) : EIdSocksError(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksServerRespondError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksServerRespondError(int Ident, Extended Dummy) : 
		EIdSocksError(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksServerRespondError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksServerRespondError(const AnsiString Msg, int AHelpContext
		) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksServerRespondError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksServerRespondError(int Ident, int AHelpContext
		) : EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksServerRespondError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksServerRespondError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksAuthMethodError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksAuthMethodError : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksAuthMethodError(const AnsiString Msg) : EIdSocksError(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksAuthMethodError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksAuthMethodError(int Ident, Extended Dummy) : EIdSocksError(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksAuthMethodError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksAuthMethodError(const AnsiString Msg, int AHelpContext
		) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksAuthMethodError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksAuthMethodError(int Ident, int AHelpContext
		) : EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksAuthMethodError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksAuthMethodError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksAuthError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksAuthError : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksAuthError(const AnsiString Msg) : EIdSocksError(Msg
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksAuthError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksAuthError(int Ident, Extended Dummy) : EIdSocksError(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksAuthError(int Ident, const System::TVarRec * 
		Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksAuthError(const AnsiString Msg, int AHelpContext
		) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksAuthError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args, Args_Size, AHelpContext)
		 { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksAuthError(int Ident, int AHelpContext) : EIdSocksError(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksAuthError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksAuthError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksServerGeneralError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksServerGeneralError : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksServerGeneralError(const AnsiString Msg) : EIdSocksError(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksServerGeneralError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksServerGeneralError(int Ident, Extended Dummy) : 
		EIdSocksError(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksServerGeneralError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksServerGeneralError(const AnsiString Msg, int AHelpContext
		) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksServerGeneralError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksServerGeneralError(int Ident, int AHelpContext
		) : EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksServerGeneralError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksServerGeneralError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksServerPermissionError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksServerPermissionError : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksServerPermissionError(const AnsiString Msg) : EIdSocksError(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksServerPermissionError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksServerPermissionError(int Ident, Extended Dummy
		) : EIdSocksError(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksServerPermissionError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksServerPermissionError(const AnsiString Msg, int 
		AHelpContext) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksServerPermissionError(const AnsiString Msg, 
		const System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksServerPermissionError(int Ident, int AHelpContext
		) : EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksServerPermissionError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksServerPermissionError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksServerNetUnreachableError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksServerNetUnreachableError : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksServerNetUnreachableError(const AnsiString Msg) : 
		EIdSocksError(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksServerNetUnreachableError(const AnsiString Msg, 
		const System::TVarRec * Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksServerNetUnreachableError(int Ident, Extended Dummy
		) : EIdSocksError(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksServerNetUnreachableError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksServerNetUnreachableError(const AnsiString Msg
		, int AHelpContext) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksServerNetUnreachableError(const AnsiString 
		Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args
		, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksServerNetUnreachableError(int Ident, int AHelpContext
		) : EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksServerNetUnreachableError(int Ident, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksServerNetUnreachableError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksServerHostUnreachableError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksServerHostUnreachableError : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksServerHostUnreachableError(const AnsiString Msg) : 
		EIdSocksError(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksServerHostUnreachableError(const AnsiString Msg
		, const System::TVarRec * Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksServerHostUnreachableError(int Ident, Extended 
		Dummy) : EIdSocksError(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksServerHostUnreachableError(int Ident, const 
		System::TVarRec * Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksServerHostUnreachableError(const AnsiString Msg
		, int AHelpContext) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksServerHostUnreachableError(const AnsiString 
		Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args
		, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksServerHostUnreachableError(int Ident, int AHelpContext
		) : EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksServerHostUnreachableError(int Ident, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksServerHostUnreachableError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksServerConnectionRefusedError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksServerConnectionRefusedError : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksServerConnectionRefusedError(const AnsiString Msg)
		 : EIdSocksError(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksServerConnectionRefusedError(const AnsiString Msg
		, const System::TVarRec * Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksServerConnectionRefusedError(int Ident, Extended 
		Dummy) : EIdSocksError(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksServerConnectionRefusedError(int Ident, const 
		System::TVarRec * Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksServerConnectionRefusedError(const AnsiString 
		Msg, int AHelpContext) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksServerConnectionRefusedError(const AnsiString 
		Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args
		, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksServerConnectionRefusedError(int Ident, int 
		AHelpContext) : EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksServerConnectionRefusedError(int Ident, 
		const System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, 
		Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksServerConnectionRefusedError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksServerTTLExpiredError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksServerTTLExpiredError : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksServerTTLExpiredError(const AnsiString Msg) : EIdSocksError(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksServerTTLExpiredError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksServerTTLExpiredError(int Ident, Extended Dummy
		) : EIdSocksError(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksServerTTLExpiredError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksServerTTLExpiredError(const AnsiString Msg, int 
		AHelpContext) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksServerTTLExpiredError(const AnsiString Msg, 
		const System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksServerTTLExpiredError(int Ident, int AHelpContext
		) : EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksServerTTLExpiredError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksServerTTLExpiredError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksServerCommandError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksServerCommandError : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksServerCommandError(const AnsiString Msg) : EIdSocksError(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksServerCommandError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksServerCommandError(int Ident, Extended Dummy) : 
		EIdSocksError(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksServerCommandError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksServerCommandError(const AnsiString Msg, int AHelpContext
		) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksServerCommandError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksServerCommandError(int Ident, int AHelpContext
		) : EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksServerCommandError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksServerCommandError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocksServerAddressError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocksServerAddressError : public EIdSocksError 
{
	typedef EIdSocksError inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocksServerAddressError(const AnsiString Msg) : EIdSocksError(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocksServerAddressError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size) : EIdSocksError(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocksServerAddressError(int Ident, Extended Dummy) : 
		EIdSocksError(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocksServerAddressError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdSocksError(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocksServerAddressError(const AnsiString Msg, int AHelpContext
		) : EIdSocksError(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocksServerAddressError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdSocksError(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocksServerAddressError(int Ident, int AHelpContext
		) : EIdSocksError(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocksServerAddressError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdSocksError(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocksServerAddressError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdConnectionStateError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdConnectionStateError : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdConnectionStateError(const AnsiString Msg) : EIdException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdConnectionStateError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdConnectionStateError(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdConnectionStateError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdConnectionStateError(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdConnectionStateError(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdConnectionStateError(int Ident, int AHelpContext
		) : EIdException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdConnectionStateError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdConnectionStateError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdDnsResolverError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdDnsResolverError : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdDnsResolverError(const AnsiString Msg) : EIdException(Msg
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdDnsResolverError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdDnsResolverError(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdDnsResolverError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdDnsResolverError(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdDnsResolverError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdDnsResolverError(int Ident, int AHelpContext) : 
		EIdException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdDnsResolverError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdDnsResolverError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdInvalidSocket;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdInvalidSocket : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdInvalidSocket(const AnsiString Msg) : EIdException(Msg)
		 { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdInvalidSocket(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdInvalidSocket(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdInvalidSocket(int Ident, const System::TVarRec * 
		Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdInvalidSocket(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdInvalidSocket(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdInvalidSocket(int Ident, int AHelpContext) : EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdInvalidSocket(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdInvalidSocket(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSocketError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSocketError : public EIdException 
{
	typedef EIdException inherited;
	
private:
	int FLastError;
	
public:
	__fastcall virtual EIdSocketError(const int AErr, const AnsiString AMsg);
	__property int LastError = {read=FLastError, nodefault};
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSocketError(const AnsiString Msg) : EIdException(Msg) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSocketError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSocketError(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSocketError(int Ident, const System::TVarRec * Args
		, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSocketError(const AnsiString Msg, int AHelpContext)
		 : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSocketError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSocketError(int Ident, int AHelpContext) : EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSocketError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSocketError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdTFTPException;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdTFTPException : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdTFTPException(const AnsiString Msg) : EIdException(Msg)
		 { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdTFTPException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdTFTPException(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdTFTPException(int Ident, const System::TVarRec * 
		Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdTFTPException(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdTFTPException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdTFTPException(int Ident, int AHelpContext) : EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdTFTPException(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdTFTPException(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdTFTPFileNotFound;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdTFTPFileNotFound : public EIdTFTPException 
{
	typedef EIdTFTPException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdTFTPFileNotFound(const AnsiString Msg) : EIdTFTPException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdTFTPFileNotFound(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdTFTPFileNotFound(int Ident, Extended Dummy) : EIdTFTPException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdTFTPFileNotFound(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdTFTPFileNotFound(const AnsiString Msg, int AHelpContext
		) : EIdTFTPException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdTFTPFileNotFound(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Msg, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdTFTPFileNotFound(int Ident, int AHelpContext) : 
		EIdTFTPException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdTFTPFileNotFound(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdTFTPFileNotFound(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdTFTPAccessViolation;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdTFTPAccessViolation : public EIdTFTPException 
{
	typedef EIdTFTPException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdTFTPAccessViolation(const AnsiString Msg) : EIdTFTPException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdTFTPAccessViolation(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdTFTPAccessViolation(int Ident, Extended Dummy) : EIdTFTPException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdTFTPAccessViolation(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdTFTPAccessViolation(const AnsiString Msg, int AHelpContext
		) : EIdTFTPException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdTFTPAccessViolation(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdTFTPAccessViolation(int Ident, int AHelpContext)
		 : EIdTFTPException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdTFTPAccessViolation(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdTFTPAccessViolation(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdTFTPAllocationExceeded;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdTFTPAllocationExceeded : public EIdTFTPException 
{
	typedef EIdTFTPException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdTFTPAllocationExceeded(const AnsiString Msg) : EIdTFTPException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdTFTPAllocationExceeded(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdTFTPAllocationExceeded(int Ident, Extended Dummy) : 
		EIdTFTPException(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdTFTPAllocationExceeded(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdTFTPAllocationExceeded(const AnsiString Msg, int AHelpContext
		) : EIdTFTPException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdTFTPAllocationExceeded(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdTFTPAllocationExceeded(int Ident, int AHelpContext
		) : EIdTFTPException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdTFTPAllocationExceeded(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdTFTPAllocationExceeded(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdTFTPIllegalOperation;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdTFTPIllegalOperation : public EIdTFTPException 
{
	typedef EIdTFTPException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdTFTPIllegalOperation(const AnsiString Msg) : EIdTFTPException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdTFTPIllegalOperation(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdTFTPIllegalOperation(int Ident, Extended Dummy) : EIdTFTPException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdTFTPIllegalOperation(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdTFTPIllegalOperation(const AnsiString Msg, int AHelpContext
		) : EIdTFTPException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdTFTPIllegalOperation(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdTFTPIllegalOperation(int Ident, int AHelpContext
		) : EIdTFTPException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdTFTPIllegalOperation(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdTFTPIllegalOperation(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdTFTPUnknownTransferID;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdTFTPUnknownTransferID : public EIdTFTPException 
{
	typedef EIdTFTPException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdTFTPUnknownTransferID(const AnsiString Msg) : EIdTFTPException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdTFTPUnknownTransferID(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdTFTPUnknownTransferID(int Ident, Extended Dummy) : EIdTFTPException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdTFTPUnknownTransferID(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdTFTPUnknownTransferID(const AnsiString Msg, int AHelpContext
		) : EIdTFTPException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdTFTPUnknownTransferID(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdTFTPUnknownTransferID(int Ident, int AHelpContext
		) : EIdTFTPException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdTFTPUnknownTransferID(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdTFTPUnknownTransferID(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdTFTPFileAlreadyExists;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdTFTPFileAlreadyExists : public EIdTFTPException 
{
	typedef EIdTFTPException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdTFTPFileAlreadyExists(const AnsiString Msg) : EIdTFTPException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdTFTPFileAlreadyExists(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdTFTPFileAlreadyExists(int Ident, Extended Dummy) : EIdTFTPException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdTFTPFileAlreadyExists(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdTFTPFileAlreadyExists(const AnsiString Msg, int AHelpContext
		) : EIdTFTPException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdTFTPFileAlreadyExists(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Msg, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdTFTPFileAlreadyExists(int Ident, int AHelpContext
		) : EIdTFTPException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdTFTPFileAlreadyExists(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdTFTPFileAlreadyExists(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdTFTPNoSuchUser;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdTFTPNoSuchUser : public EIdTFTPException 
{
	typedef EIdTFTPException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdTFTPNoSuchUser(const AnsiString Msg) : EIdTFTPException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdTFTPNoSuchUser(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdTFTPNoSuchUser(int Ident, Extended Dummy) : EIdTFTPException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdTFTPNoSuchUser(int Ident, const System::TVarRec * 
		Args, const int Args_Size) : EIdTFTPException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdTFTPNoSuchUser(const AnsiString Msg, int AHelpContext
		) : EIdTFTPException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdTFTPNoSuchUser(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Msg, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdTFTPNoSuchUser(int Ident, int AHelpContext) : EIdTFTPException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdTFTPNoSuchUser(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdTFTPNoSuchUser(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdTFTPOptionNegotiationFailed;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdTFTPOptionNegotiationFailed : public EIdTFTPException 
{
	typedef EIdTFTPException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdTFTPOptionNegotiationFailed(const AnsiString Msg) : EIdTFTPException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdTFTPOptionNegotiationFailed(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size) : EIdTFTPException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdTFTPOptionNegotiationFailed(int Ident, Extended Dummy
		) : EIdTFTPException(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdTFTPOptionNegotiationFailed(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdTFTPException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdTFTPOptionNegotiationFailed(const AnsiString Msg, int 
		AHelpContext) : EIdTFTPException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdTFTPOptionNegotiationFailed(const AnsiString Msg
		, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Msg, Args
		, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdTFTPOptionNegotiationFailed(int Ident, int AHelpContext
		) : EIdTFTPException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdTFTPOptionNegotiationFailed(int Ident, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdTFTPException(Ident, Args, Args_Size
		, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdTFTPOptionNegotiationFailed(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdIcmpException;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdIcmpException : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdIcmpException(const AnsiString Msg) : EIdException(Msg)
		 { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdIcmpException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdIcmpException(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdIcmpException(int Ident, const System::TVarRec * 
		Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdIcmpException(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdIcmpException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdIcmpException(int Ident, int AHelpContext) : EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdIcmpException(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdIcmpException(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdSetSizeExceeded;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdSetSizeExceeded : public EIdException 
{
	typedef EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdSetSizeExceeded(const AnsiString Msg) : EIdException(Msg
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdSetSizeExceeded(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdSetSizeExceeded(int Ident, Extended Dummy) : EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdSetSizeExceeded(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdSetSizeExceeded(const AnsiString Msg, int AHelpContext
		) : EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdSetSizeExceeded(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Msg, Args, Args_Size, AHelpContext) { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdSetSizeExceeded(int Ident, int AHelpContext) : EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdSetSizeExceeded(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdSetSizeExceeded(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idexception */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idexception;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdException
