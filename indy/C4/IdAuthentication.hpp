// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdAuthentication.pas' rev: 4.00

#ifndef IdAuthenticationHPP
#define IdAuthenticationHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdException.hpp>	// Pascal unit
#include <IdGlobal.hpp>	// Pascal unit
#include <IdHeaderList.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idauthentication
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TIdAuthenticationSchemes { asBasic, asDigest, asNTLM, asUnknown };
#pragma option pop

typedef Set<TIdAuthenticationSchemes, asBasic, asUnknown>  TIdAuthSchemeSet;

#pragma option push -b-
enum TIdAuthWhatsNext { wnAskTheProgram, wnDoRequest, wnFail };
#pragma option pop

class DELPHICLASS TIdAuthentication;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdAuthentication : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	int FAuthRetries;
	int FCurrentStep;
	Idheaderlist::TIdHeaderList* FParams;
	Idheaderlist::TIdHeaderList* FAuthParams;
	AnsiString __fastcall ReadAuthInfo(AnsiString AuthName);
	virtual TIdAuthWhatsNext __fastcall DoNext(void) = 0 ;
	void __fastcall SetAuthParams(Idheaderlist::TIdHeaderList* AValue);
	AnsiString __fastcall GetPassword();
	AnsiString __fastcall GetUserName();
	virtual int __fastcall GetSteps(void);
	virtual void __fastcall SetPassword(const AnsiString Value);
	virtual void __fastcall SetUserName(const AnsiString Value);
	
public:
	__fastcall virtual TIdAuthentication(void);
	__fastcall virtual ~TIdAuthentication(void);
	virtual void __fastcall Reset(void);
	virtual AnsiString __fastcall Authentication(void) = 0 ;
	virtual bool __fastcall KeepAlive(void) = 0 ;
	TIdAuthWhatsNext __fastcall Next(void);
	__property int AuthRetries = {read=FAuthRetries, nodefault};
	__property Idheaderlist::TIdHeaderList* AuthParams = {read=FAuthParams, write=SetAuthParams};
	__property Idheaderlist::TIdHeaderList* Params = {read=FParams};
	__property AnsiString Username = {read=GetUserName, write=SetUserName};
	__property AnsiString Password = {read=GetPassword, write=SetPassword};
	__property int Steps = {read=GetSteps, nodefault};
	__property int CurrentStep = {read=FCurrentStep, nodefault};
};

#pragma pack(pop)

typedef TMetaClass*TIdAuthenticationClass;

class DELPHICLASS TIdBasicAuthentication;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdBasicAuthentication : public TIdAuthentication 
{
	typedef TIdAuthentication inherited;
	
protected:
	AnsiString FRealm;
	virtual TIdAuthWhatsNext __fastcall DoNext(void);
	virtual int __fastcall GetSteps(void);
	
public:
	__fastcall virtual TIdBasicAuthentication(void);
	virtual AnsiString __fastcall Authentication();
	virtual bool __fastcall KeepAlive(void);
	virtual void __fastcall Reset(void);
	__property AnsiString Realm = {read=FRealm, write=FRealm};
public:
	#pragma option push -w-inl
	/* TIdAuthentication.Destroy */ inline __fastcall virtual ~TIdBasicAuthentication(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdAlreadyRegisteredAuthenticationMethod;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdAlreadyRegisteredAuthenticationMethod : public Idexception::EIdException 
	
{
	typedef Idexception::EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdAlreadyRegisteredAuthenticationMethod(const AnsiString 
		Msg) : Idexception::EIdException(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdAlreadyRegisteredAuthenticationMethod(const AnsiString 
		Msg, const System::TVarRec * Args, const int Args_Size) : Idexception::EIdException(Msg, Args, Args_Size
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdAlreadyRegisteredAuthenticationMethod(int Ident, Extended 
		Dummy) : Idexception::EIdException(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdAlreadyRegisteredAuthenticationMethod(int Ident, 
		const System::TVarRec * Args, const int Args_Size) : Idexception::EIdException(Ident, Args, Args_Size
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdAlreadyRegisteredAuthenticationMethod(const AnsiString 
		Msg, int AHelpContext) : Idexception::EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdAlreadyRegisteredAuthenticationMethod(const AnsiString 
		Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Idexception::EIdException(
		Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdAlreadyRegisteredAuthenticationMethod(int Ident, 
		int AHelpContext) : Idexception::EIdException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdAlreadyRegisteredAuthenticationMethod(int Ident
		, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Idexception::EIdException(
		Ident, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdAlreadyRegisteredAuthenticationMethod(void) { }
		
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall RegisterAuthenticationMethod(AnsiString MethodName, TMetaClass* AuthClass
	);
extern PACKAGE TMetaClass* __fastcall FindAuthClass(AnsiString AuthName);

}	/* namespace Idauthentication */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idauthentication;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdAuthentication
