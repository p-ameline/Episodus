// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdMultipartFormData.pas' rev: 4.00

#ifndef IdMultipartFormDataHPP
#define IdMultipartFormDataHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdResourceStrings.hpp>	// Pascal unit
#include <IdException.hpp>	// Pascal unit
#include <IdGlobal.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idmultipartformdata
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TIdFormDataField;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdFormDataField : public Classes::TCollectionItem 
{
	typedef Classes::TCollectionItem inherited;
	
protected:
	AnsiString FFieldValue;
	AnsiString FFileName;
	AnsiString FContentType;
	AnsiString FFieldName;
	System::TObject* FFieldObject;
	bool FCanFreeFieldObject;
	int __fastcall GetFieldSize(void);
	Classes::TStream* __fastcall GetFieldStream(void);
	Classes::TStrings* __fastcall GetFieldStrings(void);
	void __fastcall SetContentType(const AnsiString Value);
	void __fastcall SetFieldName(const AnsiString Value);
	void __fastcall SetFieldStream(const Classes::TStream* Value);
	void __fastcall SetFieldStrings(const Classes::TStrings* Value);
	void __fastcall SetFieldValue(const AnsiString Value);
	void __fastcall SetFieldObject(const System::TObject* Value);
	void __fastcall SetFileName(const AnsiString Value);
	
public:
	__fastcall virtual TIdFormDataField(Classes::TCollection* Collection);
	__fastcall virtual ~TIdFormDataField(void);
	AnsiString __fastcall FormatField();
	__property AnsiString ContentType = {read=FContentType, write=SetContentType};
	__property AnsiString FieldName = {read=FFieldName, write=SetFieldName};
	__property Classes::TStream* FieldStream = {read=GetFieldStream, write=SetFieldStream};
	__property Classes::TStrings* FieldStrings = {read=GetFieldStrings, write=SetFieldStrings};
	__property System::TObject* FieldObject = {read=FFieldObject, write=SetFieldObject};
	__property AnsiString FileName = {read=FFileName, write=SetFileName};
	__property AnsiString FieldValue = {read=FFieldValue, write=SetFieldValue};
	__property int FieldSize = {read=GetFieldSize, nodefault};
};

#pragma pack(pop)

class DELPHICLASS TIdFormDataFields;
class DELPHICLASS TIdMultiPartFormDataStream;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdMultiPartFormDataStream : public Classes::TStream 
{
	typedef Classes::TStream inherited;
	
protected:
	Classes::TStream* FInputStream;
	AnsiString FBoundary;
	AnsiString FRequestContentType;
	int FCurrentItem;
	bool FInitialized;
	AnsiString FInternalBuffer;
	__int64 FPosition;
	__int64 FSize;
	TIdFormDataFields* FFields;
	AnsiString __fastcall GenerateUniqueBoundary();
	AnsiString __fastcall PrepareStreamForDispatch();
	
public:
	__fastcall TIdMultiPartFormDataStream(void);
	__fastcall virtual ~TIdMultiPartFormDataStream(void);
	virtual int __fastcall Read(void *Buffer, int Count);
	virtual int __fastcall Write(const void *Buffer, int Count);
	virtual int __fastcall Seek(int Offset, Word Origin)/* overload */;
	void __fastcall AddFormField(const AnsiString AFieldName, const AnsiString AFieldValue);
	void __fastcall AddObject(const AnsiString AFieldName, const AnsiString AContentType, System::TObject* 
		AFileData, const AnsiString AFileName);
	void __fastcall AddFile(const AnsiString AFieldName, const AnsiString AFileName, const AnsiString AContentType
		);
	__property AnsiString Boundary = {read=FBoundary};
	__property AnsiString RequestContentType = {read=FRequestContentType};
};

#pragma pack(pop)

#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdFormDataFields : public Classes::TCollection 
{
	typedef Classes::TCollection inherited;
	
protected:
	TIdMultiPartFormDataStream* FParentStream;
	TIdFormDataField* __fastcall GetFormDataField(int AIndex);
	
public:
	__fastcall TIdFormDataFields(TIdMultiPartFormDataStream* AMPStream);
	HIDESBASE TIdFormDataField* __fastcall Add(void);
	__property TIdMultiPartFormDataStream* MultipartFormDataStream = {read=FParentStream};
	__property TIdFormDataField* Items[int AIndex] = {read=GetFormDataField};
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TIdFormDataFields(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

class DELPHICLASS EIdInvalidObjectType;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION EIdInvalidObjectType : public Idexception::EIdException 
{
	typedef Idexception::EIdException inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EIdInvalidObjectType(const AnsiString Msg) : Idexception::EIdException(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EIdInvalidObjectType(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : Idexception::EIdException(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EIdInvalidObjectType(int Ident, Extended Dummy) : Idexception::EIdException(
		Ident, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EIdInvalidObjectType(int Ident, const System::TVarRec 
		* Args, const int Args_Size) : Idexception::EIdException(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EIdInvalidObjectType(const AnsiString Msg, int AHelpContext
		) : Idexception::EIdException(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EIdInvalidObjectType(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Idexception::EIdException(Msg, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EIdInvalidObjectType(int Ident, int AHelpContext) : 
		Idexception::EIdException(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EIdInvalidObjectType(int Ident, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Idexception::EIdException(Ident, Args, Args_Size, 
		AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EIdInvalidObjectType(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
#define sContentTypeFormData "multipart/form-data; boundary="
#define sContentTypeOctetStream "application/octet-stream"
#define crlf "\r\n"
#define sContentDisposition "Content-Disposition: form-data; name=\"%s\""
#define sFileNamePlaceHolder "; filename=\"%s\""
#define sContentTypePlaceHolder "Content-Type: %s"
#define sContentTransferEncoding "Content-Transfer-Encoding: binary"

}	/* namespace Idmultipartformdata */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idmultipartformdata;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdMultipartFormData
