// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdThread.pas' rev: 4.00

#ifndef IdThreadHPP
#define IdThreadHPP

#pragma delphiheader begin
#pragma option push -w-
#include <SyncObjs.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <IdGlobal.hpp>	// Pascal unit
#include <IdException.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idthread
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS EIdThreadException;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdThreadException : public Idexception::EIdException 
{
	typedef Idexception::EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdThreadException(const AnsiString Msg) : Idexception::EIdException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdThreadException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : Idexception::EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdThreadException(int Ident, Extended Dummy) : Idexception::EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdThreadException(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : Idexception::EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdThreadException(const AnsiString Msg, int AHelpContext
		) : Idexception::EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdThreadException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Idexception::EIdException(Msg, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdThreadException(int Ident, int AHelpContext) : Idexception::EIdException(
		Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdThreadException(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Idexception::EIdException(Ident, Args, Args_Size, 
		AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdThreadException(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdThreadTerminateAndWaitFor;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdThreadTerminateAndWaitFor : public EIdThreadException 
{
	typedef EIdThreadException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdThreadTerminateAndWaitFor(const AnsiString Msg) : EIdThreadException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdThreadTerminateAndWaitFor(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size) : EIdThreadException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdThreadTerminateAndWaitFor(int Ident, Extended Dummy)
		 : EIdThreadException(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdThreadTerminateAndWaitFor(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdThreadException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdThreadTerminateAndWaitFor(const AnsiString Msg, int 
		AHelpContext) : EIdThreadException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdThreadTerminateAndWaitFor(const AnsiString Msg, 
		const System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdThreadException(Msg, Args
		, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdThreadTerminateAndWaitFor(int Ident, int AHelpContext
		) : EIdThreadException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdThreadTerminateAndWaitFor(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdThreadException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdThreadTerminateAndWaitFor(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

#pragma option push -b-
enum TIdThreadStopMode { smTerminate, smSuspend };
#pragma option pop

class DELPHICLASS TIdThread;
typedef void __fastcall (__closure *TIdExceptionThreadEvent)(TIdThread* AThread, Sysutils::Exception* 
	AException);

typedef void __fastcall (__closure *TIdNotifyThreadEvent)(TIdThread* AThread);

typedef void __fastcall (__closure *TIdSynchronizeThreadEvent)(TIdThread* AThread, void * AData);

class DELPHICLASS TIdBaseThread;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdBaseThread : public Classes::TThread 
{
	typedef Classes::TThread inherited;
	
public:
	HIDESBASE void __fastcall Synchronize(Classes::TThreadMethod Method)/* overload */;
	HIDESBASE void __fastcall Synchronize(const Sysutils::TMethod &Method)/* overload */;
	__property ReturnValue ;
	__property Terminated ;
public:
	#pragma option push -w-inl
	/* TThread.Create */ inline __fastcall TIdBaseThread(bool CreateSuspended) : Classes::TThread(CreateSuspended
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TThread.Destroy */ inline __fastcall virtual ~TIdBaseThread(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdThread : public TIdBaseThread 
{
	typedef TIdBaseThread inherited;
	
protected:
	System::TObject* FData;
	Syncobjs::TCriticalSection* FLock;
	TIdThreadStopMode FStopMode;
	bool FStopped;
	AnsiString FTerminatingException;
	TMetaClass*FTerminatingExceptionClass;
	TIdExceptionThreadEvent FOnException;
	TIdNotifyThreadEvent FOnStopped;
	virtual void __fastcall AfterRun(void);
	virtual void __fastcall AfterExecute(void);
	virtual void __fastcall BeforeExecute(void);
	virtual void __fastcall BeforeRun(void);
	virtual void __fastcall Cleanup(void);
	virtual void __fastcall DoException(Sysutils::Exception* AException);
	virtual void __fastcall DoStopped(void);
	virtual void __fastcall Execute(void);
	bool __fastcall GetStopped(void);
	virtual void __fastcall Run(void) = 0 ;
	
public:
	__fastcall virtual TIdThread(bool ACreateSuspended);
	__fastcall virtual ~TIdThread(void);
	virtual void __fastcall Start(void);
	virtual void __fastcall Stop(void);
	HIDESBASE virtual void __fastcall Terminate(void);
	virtual void __fastcall TerminateAndWaitFor(void);
	__property System::TObject* Data = {read=FData, write=FData};
	__property TIdThreadStopMode StopMode = {read=FStopMode, write=FStopMode, nodefault};
	__property bool Stopped = {read=GetStopped, nodefault};
	__property AnsiString TerminatingException = {read=FTerminatingException};
	__property TMetaClass* TerminatingExceptionClass = {read=FTerminatingExceptionClass};
	__property TIdExceptionThreadEvent OnException = {read=FOnException, write=FOnException};
	__property TIdNotifyThreadEvent OnStopped = {read=FOnStopped, write=FOnStopped};
};

#pragma pack(pop)

typedef TMetaClass*TIdThreadClass;

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idthread */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idthread;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdThread
