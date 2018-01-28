// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdThreadSafe.pas' rev: 4.00

#ifndef IdThreadSafeHPP
#define IdThreadSafeHPP

#pragma delphiheader begin
#pragma option push -w-
#include <SyncObjs.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idthreadsafe
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdThreadSafe;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdThreadSafe : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Syncobjs::TCriticalSection* FCriticalSection;
	
public:
	__fastcall virtual TIdThreadSafe(void);
	__fastcall virtual ~TIdThreadSafe(void);
	void __fastcall Lock(void);
	void __fastcall Unlock(void);
};

#pragma pack(pop)

class DELPHICLASS TIdThreadSafeInteger;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdThreadSafeInteger : public TIdThreadSafe 
{
	typedef TIdThreadSafe inherited;
	
protected:
	int FValue;
	int __fastcall GetValue(void);
	void __fastcall SetValue(const int AValue);
	
public:
	int __fastcall Decrement(void);
	int __fastcall Increment(void);
	__property int Value = {read=GetValue, write=SetValue, nodefault};
public:
	#pragma option push -w-inl
	/* TIdThreadSafe.Create */ inline __fastcall virtual TIdThreadSafeInteger(void) : TIdThreadSafe() { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* TIdThreadSafe.Destroy */ inline __fastcall virtual ~TIdThreadSafeInteger(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS TIdThreadSafeCardinal;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdThreadSafeCardinal : public TIdThreadSafe 
{
	typedef TIdThreadSafe inherited;
	
protected:
	unsigned FValue;
	unsigned __fastcall GetValue(void);
	void __fastcall SetValue(const unsigned AValue);
	
public:
	unsigned __fastcall Decrement(void);
	unsigned __fastcall Increment(void);
	__property unsigned Value = {read=GetValue, write=SetValue, nodefault};
public:
	#pragma option push -w-inl
	/* TIdThreadSafe.Create */ inline __fastcall virtual TIdThreadSafeCardinal(void) : TIdThreadSafe() { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* TIdThreadSafe.Destroy */ inline __fastcall virtual ~TIdThreadSafeCardinal(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS TIdThreadSafeString;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdThreadSafeString : public TIdThreadSafe 
{
	typedef TIdThreadSafe inherited;
	
protected:
	AnsiString FValue;
	AnsiString __fastcall GetValue();
	void __fastcall SetValue(const AnsiString AValue);
	
public:
	void __fastcall Append(const AnsiString AValue);
	void __fastcall Prepend(const AnsiString AValue);
	__property AnsiString Value = {read=GetValue, write=SetValue};
public:
	#pragma option push -w-inl
	/* TIdThreadSafe.Create */ inline __fastcall virtual TIdThreadSafeString(void) : TIdThreadSafe() { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* TIdThreadSafe.Destroy */ inline __fastcall virtual ~TIdThreadSafeString(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS TIdThreadSafeStringList;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdThreadSafeStringList : public TIdThreadSafe 
{
	typedef TIdThreadSafe inherited;
	
protected:
	Classes::TStringList* FValue;
	
public:
	__fastcall TIdThreadSafeStringList(const bool ASorted);
	__fastcall virtual ~TIdThreadSafeStringList(void);
	void __fastcall Add(const AnsiString AItem);
	void __fastcall AddObject(const AnsiString AItem, System::TObject* AObject);
	void __fastcall Clear(void);
	HIDESBASE Classes::TStringList* __fastcall Lock(void);
	System::TObject* __fastcall ObjectByItem(const AnsiString AItem);
	void __fastcall Remove(const AnsiString AItem);
	HIDESBASE void __fastcall Unlock(void);
};

#pragma pack(pop)

class DELPHICLASS TIdThreadSafeList;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdThreadSafeList : public Classes::TThreadList 
{
	typedef Classes::TThreadList inherited;
	
public:
	bool __fastcall IsCountLessThan(const unsigned AValue);
public:
	#pragma option push -w-inl
	/* TThreadList.Create */ inline __fastcall TIdThreadSafeList(void) : Classes::TThreadList() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TThreadList.Destroy */ inline __fastcall virtual ~TIdThreadSafeList(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idthreadsafe */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idthreadsafe;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdThreadSafe
