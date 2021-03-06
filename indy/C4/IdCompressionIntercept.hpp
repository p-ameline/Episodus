// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdCompressionIntercept.pas' rev: 4.00

#ifndef IdCompressionInterceptHPP
#define IdCompressionInterceptHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdIntercept.hpp>	// Pascal unit
#include <IdTCPConnection.hpp>	// Pascal unit
#include <IdGlobal.hpp>	// Pascal unit
#include <IdTCPClient.hpp>	// Pascal unit
#include <IdException.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idcompressionintercept
{
//-- type declarations -------------------------------------------------------
typedef void * __fastcall (*TAlloc)(void * AppData, int Items, int Size);

typedef void __fastcall (*TFree)(void * AppData, void * Block);

#pragma pack(push, 1)
struct TZStreamRec
{
	char *next_in;
	int avail_in;
	int total_in;
	char *next_out;
	int avail_out;
	int total_out;
	char *msg;
	void *internal;
	TAlloc zalloc;
	TFree zfree;
	void *AppData;
	int data_type;
	int adler;
	int reserved;
} ;
#pragma pack(pop)

class DELPHICLASS EIdCompressionException;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdCompressionException : public Idexception::EIdException 
{
	typedef Idexception::EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdCompressionException(const AnsiString Msg) : Idexception::EIdException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdCompressionException(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : Idexception::EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdCompressionException(int Ident, Extended Dummy) : Idexception::EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdCompressionException(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : Idexception::EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdCompressionException(const AnsiString Msg, int AHelpContext
		) : Idexception::EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdCompressionException(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : Idexception::EIdException(Msg, Args
		, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdCompressionException(int Ident, int AHelpContext
		) : Idexception::EIdException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdCompressionException(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Idexception::EIdException(Ident, Args, Args_Size, 
		AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdCompressionException(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdCompressorInitFailure;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdCompressorInitFailure : public EIdCompressionException 
{
	typedef EIdCompressionException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdCompressorInitFailure(const AnsiString Msg) : EIdCompressionException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdCompressorInitFailure(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdCompressionException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdCompressorInitFailure(int Ident, Extended Dummy) : EIdCompressionException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdCompressorInitFailure(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdCompressionException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdCompressorInitFailure(const AnsiString Msg, int AHelpContext
		) : EIdCompressionException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdCompressorInitFailure(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdCompressionException(Msg, Args
		, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdCompressorInitFailure(int Ident, int AHelpContext
		) : EIdCompressionException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdCompressorInitFailure(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdCompressionException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdCompressorInitFailure(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdDecompressorInitFailure;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdDecompressorInitFailure : public EIdCompressionException 
{
	typedef EIdCompressionException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdDecompressorInitFailure(const AnsiString Msg) : EIdCompressionException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdDecompressorInitFailure(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size) : EIdCompressionException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdDecompressorInitFailure(int Ident, Extended Dummy) : 
		EIdCompressionException(Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdDecompressorInitFailure(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdCompressionException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdDecompressorInitFailure(const AnsiString Msg, int AHelpContext
		) : EIdCompressionException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdDecompressorInitFailure(const AnsiString Msg, const 
		System::TVarRec * Args, const int Args_Size, int AHelpContext) : EIdCompressionException(Msg, Args
		, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdDecompressorInitFailure(int Ident, int AHelpContext
		) : EIdCompressionException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdDecompressorInitFailure(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdCompressionException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdDecompressorInitFailure(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdCompressionError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdCompressionError : public EIdCompressionException 
{
	typedef EIdCompressionException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdCompressionError(const AnsiString Msg) : EIdCompressionException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdCompressionError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdCompressionException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdCompressionError(int Ident, Extended Dummy) : EIdCompressionException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdCompressionError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdCompressionException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdCompressionError(const AnsiString Msg, int AHelpContext
		) : EIdCompressionException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdCompressionError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdCompressionException(Msg, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdCompressionError(int Ident, int AHelpContext) : 
		EIdCompressionException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdCompressionError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdCompressionException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdCompressionError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdDecompressionError;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdDecompressionError : public EIdCompressionException 
{
	typedef EIdCompressionException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdDecompressionError(const AnsiString Msg) : EIdCompressionException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdDecompressionError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : EIdCompressionException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdDecompressionError(int Ident, Extended Dummy) : EIdCompressionException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdDecompressionError(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : EIdCompressionException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdDecompressionError(const AnsiString Msg, int AHelpContext
		) : EIdCompressionException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdDecompressionError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdCompressionException(Msg, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdDecompressionError(int Ident, int AHelpContext) : 
		EIdCompressionException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdDecompressionError(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : EIdCompressionException(Ident, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdDecompressionError(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

typedef Shortint TCompressionLevel;

class DELPHICLASS TIdCompressionIntercept;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdCompressionIntercept : public Idintercept::TIdConnectionIntercept 
{
	typedef Idintercept::TIdConnectionIntercept inherited;
	
protected:
	TCompressionLevel FCompressionLevel;
	TZStreamRec FCompressRec;
	TZStreamRec FDecompressRec;
	void *FRecvBuf;
	int FRecvCount;
	int FRecvSize;
	void *FSendBuf;
	int FSendCount;
	int FSendSize;
	void __fastcall SetCompressionLevel(TCompressionLevel Value);
	void __fastcall InitCompressors(void);
	void __fastcall DeinitCompressors(void);
	
public:
	__fastcall virtual ~TIdCompressionIntercept(void);
	virtual void __fastcall Disconnect(void);
	virtual void __fastcall Receive(Classes::TStream* ABuffer);
	virtual void __fastcall Send(Classes::TStream* ABuffer);
	
__published:
	__property TCompressionLevel CompressionLevel = {read=FCompressionLevel, write=SetCompressionLevel, 
		nodefault};
public:
	#pragma option push -w-inl
	/* TIdConnectionIntercept.Create */ inline __fastcall virtual TIdCompressionIntercept(Classes::TComponent* 
		AOwner) : Idintercept::TIdConnectionIntercept(AOwner) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS TIdServerCompressionIntercept;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdServerCompressionIntercept : public Idintercept::TIdServerIntercept 
{
	typedef Idintercept::TIdServerIntercept inherited;
	
private:
	TCompressionLevel FCompressionLevel;
	
public:
	virtual void __fastcall Init(void);
	virtual Idintercept::TIdConnectionIntercept* __fastcall Accept(Classes::TComponent* AConnection);
	
__published:
	__property TCompressionLevel CompressionLevel = {read=FCompressionLevel, write=FCompressionLevel, nodefault
		};
public:
	#pragma option push -w-inl
	/* TComponent.Create */ inline __fastcall virtual TIdServerCompressionIntercept(Classes::TComponent* 
		AOwner) : Idintercept::TIdServerIntercept(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TComponent.Destroy */ inline __fastcall virtual ~TIdServerCompressionIntercept(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idcompressionintercept */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idcompressionintercept;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdCompressionIntercept
