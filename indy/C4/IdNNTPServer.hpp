// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdNNTPServer.pas' rev: 4.00

#ifndef IdNNTPServerHPP
#define IdNNTPServerHPP

#pragma delphiheader begin
#pragma option push -w-
#include <IdComponent.hpp>	// Pascal unit
#include <IdThread.hpp>	// Pascal unit
#include <IdTCPServer.hpp>	// Pascal unit
#include <IdGlobal.hpp>	// Pascal unit
#include <IdAssignedNumbers.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Idnntpserver
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TIdNNTPAuthType { atUserPass, atSimple, atGeneric };
#pragma option pop

typedef Set<TIdNNTPAuthType, atUserPass, atGeneric>  TIdNNTPAuthTypes;

class DELPHICLASS TIdNNTPThread;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdNNTPThread : public Idtcpserver::TIdPeerThread 
{
	typedef Idtcpserver::TIdPeerThread inherited;
	
protected:
	int FCurrentArticle;
	AnsiString FCurrentGroup;
	AnsiString FUserName;
	AnsiString FPassword;
	AnsiString FAuthenticator;
	AnsiString FAuthParams;
	bool FAuthenticated;
	TIdNNTPAuthType FAuthType;
	bool FModeReader;
	
public:
	__fastcall virtual TIdNNTPThread(bool ACreateSuspended);
	__property int CurrentArticle = {read=FCurrentArticle, nodefault};
	__property AnsiString CurrentGroup = {read=FCurrentGroup};
	__property bool ModeReader = {read=FModeReader, nodefault};
	__property AnsiString UserName = {read=FUserName};
	__property AnsiString Password = {read=FPassword};
	__property AnsiString Authenticator = {read=FAuthenticator};
	__property AnsiString AuthParams = {read=FAuthParams};
	__property bool Authenticated = {read=FAuthenticated, nodefault};
	__property TIdNNTPAuthType AuthType = {read=FAuthType, nodefault};
public:
	#pragma option push -w-inl
	/* TIdThread.Destroy */ inline __fastcall virtual ~TIdNNTPThread(void) { }
	#pragma option pop
	
};

#pragma pack(pop)

typedef void __fastcall (__closure *TIdNNTPOnAuth)(TIdNNTPThread* AThread, bool &VAccept);

typedef void __fastcall (__closure *TIdNNTPOnNewGroupsList)(TIdNNTPThread* AThread, const System::TDateTime 
	ADateStamp, const AnsiString ADistributions);

typedef void __fastcall (__closure *TIdNNTPOnNewNews)(TIdNNTPThread* AThread, const AnsiString Newsgroups
	, const System::TDateTime ADateStamp, const AnsiString ADistributions);

typedef void __fastcall (__closure *TIdNNTPOnIHaveCheck)(TIdNNTPThread* AThread, const AnsiString AMsgID
	, bool VAccept);

typedef void __fastcall (__closure *TIdNNTPOnArticleByNo)(TIdNNTPThread* AThread, const int AMsgNo);
	

typedef void __fastcall (__closure *TIdNNTPOnArticleByID)(TIdNNTPThread* AThread, const AnsiString AMsgID
	);

typedef void __fastcall (__closure *TIdNNTPOnCheckMsgNo)(TIdNNTPThread* AThread, const int AMsgNo, AnsiString 
	&VMsgID);

typedef void __fastcall (__closure *TIdNNTPOnCheckMsgID)(TIdNNTPThread* AThread, const AnsiString AMsgId
	, int &VMsgNo);

typedef void __fastcall (__closure *TIdNNTPOnMovePointer)(TIdNNTPThread* AThread, int &AMsgNo, AnsiString 
	&VMsgID);

typedef void __fastcall (__closure *TIdNNTPOnPost)(TIdNNTPThread* AThread, bool &VPostOk, AnsiString 
	&VErrorText);

typedef void __fastcall (__closure *TIdNNTPOnSelectGroup)(TIdNNTPThread* AThread, const AnsiString AGroup
	, int &VMsgCount, int &VMsgFirst, int &VMsgLast, bool &VGroupExists);

typedef void __fastcall (__closure *TIdNNTPOnCheckListGroup)(TIdNNTPThread* AThread, const AnsiString 
	AGroup, bool &VCanJoin, int &VFirstArticle);

typedef void __fastcall (__closure *TIdNNTPOnXHdr)(TIdNNTPThread* AThread, const AnsiString AHeaderName
	, const int AMsgFirst, const int AMsgLast, const AnsiString AMsgID);

typedef void __fastcall (__closure *TIdNNTPOnXOver)(TIdNNTPThread* AThread, const int AMsgFirst, const 
	int AMsgLast);

typedef void __fastcall (__closure *TIdNNTPOnXPat)(TIdNNTPThread* AThread, const AnsiString AHeaderName
	, const int AMsgFirst, const int AMsgLast, const AnsiString AMsgID, const AnsiString AHeaderPattern
	);

typedef void __fastcall (__closure *TIdNNTPOnAuthRequired)(TIdNNTPThread* AThread, const AnsiString 
	ACommand, const AnsiString AParams, bool &VRequired);

typedef void __fastcall (__closure *TIdNNTPOnListPattern)(TIdNNTPThread* AThread, const AnsiString AGroupPattern
	);

class DELPHICLASS TIdNNTPServer;
#pragma pack(push, 4)
class PASCALIMPLEMENTATION TIdNNTPServer : public Idtcpserver::TIdTCPServer 
{
	typedef Idtcpserver::TIdTCPServer inherited;
	
protected:
	Classes::TStrings* FHelp;
	Classes::TStrings* FDistributionPatterns;
	Classes::TStrings* FOverviewFormat;
	TIdNNTPAuthTypes FSupportedAuthTypes;
	TIdNNTPOnArticleByNo FOnArticleByNo;
	TIdNNTPOnArticleByNo FOnBodyByNo;
	TIdNNTPOnArticleByNo FOnHeadByNo;
	TIdNNTPOnCheckMsgNo FOnCheckMsgNo;
	TIdNNTPOnCheckMsgID FOnCheckMsgId;
	TIdNNTPOnMovePointer FOnStatMsgNo;
	TIdNNTPOnMovePointer FOnNextArticle;
	TIdNNTPOnMovePointer FOnPrevArticle;
	TIdNNTPOnCheckListGroup FOnCheckListGroup;
	TIdNNTPOnListPattern FOnListActiveGroups;
	TIdNNTPOnListPattern FOnListActiveGroupTimes;
	TIdNNTPOnListPattern FOnListDescriptions;
	Idtcpserver::TIdServerThreadEvent FOnListDistributions;
	Idtcpserver::TIdServerThreadEvent FOnListExtensions;
	Idtcpserver::TIdServerThreadEvent FOnListHeaders;
	Idtcpserver::TIdServerThreadEvent FOnListSubscriptions;
	Idtcpserver::TIdServerThreadEvent FOnListGroup;
	Idtcpserver::TIdServerThreadEvent FOnListGroups;
	TIdNNTPOnNewGroupsList FOnListNewGroups;
	TIdNNTPOnPost FOnPost;
	TIdNNTPOnSelectGroup FOnSelectGroup;
	TIdNNTPOnXHdr FOnXHdr;
	TIdNNTPOnXOver FOnXOver;
	TIdNNTPOnXOver FOnXROver;
	TIdNNTPOnXPat FOnXPat;
	TIdNNTPOnNewNews FOnNewNews;
	TIdNNTPOnIHaveCheck FOnIHaveCheck;
	TIdNNTPOnPost FOnIHavePost;
	TIdNNTPOnAuth FOnAuth;
	TIdNNTPOnAuthRequired FOnAuthRequired;
	bool __fastcall AuthRequired(Idtcpserver::TIdCommand* ASender);
	int __fastcall DoCheckMsgID(TIdNNTPThread* AThread, const AnsiString AMsgID);
	AnsiString __fastcall RawNavigate(TIdNNTPThread* AThread, TIdNNTPOnMovePointer AEvent);
	void __fastcall CommandArticle(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandAuthInfoUser(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandAuthInfoPassword(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandAuthInfoSimple(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandAuthInfoGeneric(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandBody(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandDate(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandHead(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandHelp(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandGroup(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandIHave(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandLast(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandList(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandListActiveGroups(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandListActiveTimes(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandListDescriptions(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandListDistributions(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandListDistribPats(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandListExtensions(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandListHeaders(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandListSubscriptions(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandListGroup(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandListOverview(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandModeReader(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandNewGroups(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandNewNews(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandNext(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandPost(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandSlave(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandStat(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandXHdr(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandXOver(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandXROver(Idtcpserver::TIdCommand* ASender);
	void __fastcall CommandXPat(Idtcpserver::TIdCommand* ASender);
	void __fastcall DoSelectGroup(TIdNNTPThread* AThread, const AnsiString AGroup, int &VMsgCount, int 
		&VMsgFirst, int &VMsgLast, bool &VGroupExists);
	virtual void __fastcall InitializeCommandHandlers(void);
	void __fastcall SetDistributionPatterns(Classes::TStrings* AValue);
	void __fastcall SetHelp(Classes::TStrings* AValue);
	void __fastcall SetOverviewFormat(Classes::TStrings* AValue);
	bool __fastcall LookupMessage(Idtcpserver::TIdCommand* ASender, int &VNo, AnsiString &VId);
	bool __fastcall LookupMessageRange(Idtcpserver::TIdCommand* ASender, const AnsiString AData, int &VMsgFirst
		, int &VMsgLast);
	bool __fastcall LookupMessageRangeOrID(Idtcpserver::TIdCommand* ASender, const AnsiString AData, int 
		&VMsgFirst, int &VMsgLast, AnsiString &VMsgID);
	
public:
	__fastcall virtual TIdNNTPServer(Classes::TComponent* AOwner);
	__fastcall virtual ~TIdNNTPServer(void);
	/*         class method */ static System::TDateTime __fastcall NNTPTimeToTime(TMetaClass* vmt, const 
		AnsiString ATimeStamp);
	/*         class method */ static System::TDateTime __fastcall NNTPDateTimeToDateTime(TMetaClass* vmt
		, const AnsiString ATimeStamp);
	
__published:
	__property DefaultPort ;
	__property Classes::TStrings* DistributionPatterns = {read=FDistributionPatterns, write=SetDistributionPatterns
		};
	__property Classes::TStrings* Help = {read=FHelp, write=SetHelp};
	__property TIdNNTPAuthTypes SupportedAuthTypes = {read=FSupportedAuthTypes, write=FSupportedAuthTypes
		, nodefault};
	__property TIdNNTPOnArticleByNo OnArticleByNo = {read=FOnArticleByNo, write=FOnArticleByNo};
	__property TIdNNTPOnAuth OnAuth = {read=FOnAuth, write=FOnAuth};
	__property TIdNNTPOnAuthRequired OnAuthRequired = {read=FOnAuthRequired, write=FOnAuthRequired};
	__property TIdNNTPOnArticleByNo OnBodyByNo = {read=FOnBodyByNo, write=FOnBodyByNo};
	__property TIdNNTPOnArticleByNo OnHeadByNo = {read=FOnHeadByNo, write=FOnHeadByNo};
	__property TIdNNTPOnCheckMsgNo OnCheckMsgNo = {read=FOnCheckMsgNo, write=FOnCheckMsgNo};
	__property TIdNNTPOnCheckMsgID OnCheckMsgID = {read=FOnCheckMsgId, write=FOnCheckMsgId};
	__property TIdNNTPOnMovePointer OnStatMsgNo = {read=FOnStatMsgNo, write=FOnStatMsgNo};
	__property TIdNNTPOnMovePointer OnNextArticle = {read=FOnNextArticle, write=FOnNextArticle};
	__property TIdNNTPOnMovePointer OnPrevArticle = {read=FOnPrevArticle, write=FOnPrevArticle};
	__property TIdNNTPOnCheckListGroup OnCheckListGroup = {read=FOnCheckListGroup, write=FOnCheckListGroup
		};
	__property TIdNNTPOnListPattern OnListActiveGroups = {read=FOnListActiveGroups, write=FOnListActiveGroups
		};
	__property TIdNNTPOnListPattern OnListActiveGroupTimes = {read=FOnListActiveGroupTimes, write=FOnListActiveGroupTimes
		};
	__property TIdNNTPOnListPattern OnListDescriptions = {read=FOnListDescriptions, write=FOnListDescriptions
		};
	__property Idtcpserver::TIdServerThreadEvent OnListDistributions = {read=FOnListDistributions, write=
		FOnListDistributions};
	__property Idtcpserver::TIdServerThreadEvent OnListExtensions = {read=FOnListExtensions, write=FOnListExtensions
		};
	__property Idtcpserver::TIdServerThreadEvent OnListHeaders = {read=FOnListHeaders, write=FOnListHeaders
		};
	__property Idtcpserver::TIdServerThreadEvent OnListSubscriptions = {read=FOnListSubscriptions, write=
		FOnListSubscriptions};
	__property Idtcpserver::TIdServerThreadEvent OnListGroups = {read=FOnListGroups, write=FOnListGroups
		};
	__property Idtcpserver::TIdServerThreadEvent OnListGroup = {read=FOnListGroup, write=FOnListGroup};
		
	__property TIdNNTPOnNewGroupsList OnListNewGroups = {read=FOnListNewGroups, write=FOnListNewGroups}
		;
	__property TIdNNTPOnSelectGroup OnSelectGroup = {read=FOnSelectGroup, write=FOnSelectGroup};
	__property TIdNNTPOnPost OnPost = {read=FOnPost, write=FOnPost};
	__property Classes::TStrings* OverviewFormat = {read=FOverviewFormat, write=SetOverviewFormat};
	__property TIdNNTPOnXHdr OnXHdr = {read=FOnXHdr, write=FOnXHdr};
	__property TIdNNTPOnXOver OnXOver = {read=FOnXOver, write=FOnXOver};
	__property TIdNNTPOnXOver OnXROver = {read=FOnXROver, write=FOnXROver};
	__property TIdNNTPOnXPat OnXPat = {read=FOnXPat, write=FOnXPat};
	__property TIdNNTPOnNewNews OnNewNews = {read=FOnNewNews, write=FOnNewNews};
	__property TIdNNTPOnIHaveCheck OnIHaveCheck = {read=FOnIHaveCheck, write=FOnIHaveCheck};
	__property TIdNNTPOnPost OnIHavePost = {read=FOnIHavePost, write=FOnIHavePost};
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Idnntpserver */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Idnntpserver;
#endif
#pragma option pop	// -w-

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdNNTPServer
