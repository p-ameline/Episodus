//---------------------------------------------------------------------------#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "EMail.h"
#include "partage\nsdivfct.h"
#include "partage\nsglobal.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsbasmsg.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)#pragma resource "*.dfm"

// Fonctions POP3
#define RES_CONNECT 			4
#define RES_DELETE              6
#define RES_DISCONNECT          5
#define RES_FREE                2
#define RES_GET_OPTION          29
#define RES_INITIALIZE          1
#define RES_LIST                7
#define RES_LISTA               8
#define RES_NOOP                10
#define RES_PASS                9
#define RES_PROCESS_RESPONSES   24
#define RES_QUIT                11
#define RES_RESET               12
#define RES_RETRIEVE            13
#define RES_SEND_COMMAND        15
#define RES_SET_CHUNK_SIZE      25
#define RES_SET_OPTION          28
#define RES_SET_RESPONSE_SINK   26
#define RES_SET_TIMEOUT         27
#define RES_SINK_FREE           31
#define RES_SINK_INITIALIZE     30
#define RES_STAT                16
#define RES_TOP                 17
#define RES_UID_LIST            19
#define RES_UID_LISTA           18
#define RES_USER                20
#define RES_XAUTH_LIST          22
#define RES_XAUTH_LISTA         21
#define RES_XSENDER             23

// Fonctions MIME#define RES_PARSE_ENTIRE_MESSAGE 	47
#define RES_BASICPART_FREE_ALL     	8
#define RES_BASICPART_GETDATABUF    6
#define RES_MEMFREE					83
#define RES_MESSAGE_FREE_ALL		33
#define RES_MESSAGE_GETBODY			31
#define RES_MESSAGEPART_FREE_ALL	22
#define RES_MULTIPART_FREE_ALL		16
#define RES_MULTIPART_GETPART		14
#define RES_MULTIPART_GETPARTCOUNT	13
#define RES_MESSAGEPART_GETMESSAGE	18
#define RES_DECODE_BASE64           38
#define RES_DECODE_HEADER_STRING    35
#define RES_DECODE_QP               39

// Fonctions MIME DYNAMIC PARSING#define RES_DATASINK_NEW			77
#define RES_DATASINK_FREE			78
#define RES_DYNAMIC_PARSER_NEW		40
#define RES_DYNAMIC_PARSER_FREE		41
#define RES_BEGIN_DYNAMIC_PARSE		42
#define RES_DYNAMIC_PARSE			44
#define RES_END_DYNAMIC_PARSE		45

// Fonctions COMM
#define RES_INPUT_STREAM_CREATE		2
#define RES_BUF_INPUT_STREAM_CREATE 3
#define RES_OUTPUT_STREAM_CREATE    4
#define RES_STREAM_FREE             21

//***************************************************************************
// 					Implémentation des méthodes NSPartArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
            /*
NSPartArray::NSPartArray(NSPartArray& rv)					 :NSPartVector()
{
	for (NSPartIter i = rv.begin(); i != rv.end(); i++)
   		push_back(new NSBasicPart(*(*i)));
}

//---------------------------------------------------------------------------
//  Operateur =
//---------------------------------------------------------------------------
NSPartArray&
NSPartArray::operator=(NSPartArray src)
{
	vider();
	for (NSPartIter i = src.begin(); i != src.end(); i++)
   		push_back(new NSBasicPart(*(*i)));
    return *this;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSPartArray::vider()
{
	for (NSPartIter i = begin(); i != end(); )
   	{
   		delete *i;
      	erase(i);
   	}
}

NSPartArray::~NSPartArray(){
	vider();
}
     */
//***************************************************************************
// 					Implémentation des méthodes NSMsg
//***************************************************************************

NSMsg::NSMsg()
{
	pHeader    = new NSHeader() ;
	pBody      = new NSBasicPart() ;
	pPartArray = new NSPartArray ;
	nbPart = 0 ;
}

NSMsg::NSMsg(NSHeader* pNSHead){
	pHeader    = new NSHeader(*pNSHead) ;
	pBody      = new NSBasicPart() ;
  pPartArray = new NSPartArray ;
  nbPart = 0 ;
}

NSMsg::NSMsg(NSMsg& rv){
	pHeader    = new NSHeader(*(rv.pHeader)) ;
	pBody      = new NSBasicPart(*(rv.pBody)) ;
	pPartArray = new NSPartArray(*(rv.pPartArray)) ;
	nbPart = rv.nbPart ;
}

NSMsg&NSMsg::operator=(NSMsg src)
{
	if (this == &src)
		return *this ;

	*pHeader    = *(src.pHeader) ;
	*pBody      = *(src.pBody) ;
	*pPartArray = *(src.pPartArray) ;
	nbPart = src.nbPart ;

	return *this ;
}

NSMsg::~NSMsg(){
	delete pHeader ;
	delete pBody ;
	delete pPartArray ;
}

//***************************************************************************// 					Implémentation des méthodes NSMsgArray
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSMsgArray::NSMsgArray(NSMsgArray& rv)
					 :NSMsgVector()
{
	if (!(rv.empty()))
		for (NSMsgIter i = rv.begin(); i != rv.end(); i++)
			push_back(new NSMsg(*(*i))) ;
}

//---------------------------------------------------------------------------//  Operateur =
//---------------------------------------------------------------------------
NSMsgArray&
NSMsgArray::operator=(NSMsgArray src)
{
	if (this == &src)
		return *this ;

	vider();

	for (NSMsgIter i = src.begin(); i != src.end(); i++)  	push_back(new NSMsg(*(*i)));

	return *this ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSMsgArray::vider()
{
	if (empty())
		return ;

	for (NSMsgIter i = begin(); i != end(); )
	{
		delete *i ;
		erase(i) ;
	}
}

NSMsgArray::~NSMsgArray(){
	vider() ;
}

/****************************************************************************//***						CLASSE NSPOP3								  ***/
/****************************************************************************/

TMailBox*          NSPop3::pForm     = NULL ;pop3Client_t*      NSPop3::pClient   = NULL ;
pop3Sink_t*        NSPop3::pSink     = NULL ;
struct mimeParser* NSPop3::pParser   = NULL ;
mimeDataSink_t*    NSPop3::pDataSink = NULL ;
NSMsgArray*        NSPop3::pMsgArray = NULL ;

int					NSPop3::nbMsg      = 0 ;int					NSPop3::numMsg     = -1 ;
int					NSPop3::numPart    = -1 ;
string			NSPop3::sTextMsg   = "" ;
int					NSPop3::nbRetrieve = 0 ;

NSMsgArray* NSPop3::pMsgStockArray = NULL ;

int					NSPop3::nbMsgStock   = 0 ;int					NSPop3::numMsgStock  = -1 ;
int					NSPop3::numPartStock = -1 ;

int 			(FAR* NSPop3::pAdrConnect)(pop3Client_t *, const char *, unsigned short) = NULL;int				(FAR* NSPop3::pAdrDelete)(pop3Client_t *, int) = NULL;
int				(FAR* NSPop3::pAdrDisconnect)(pop3Client_t *) = NULL;
void 			(FAR* NSPop3::pAdrFree)(pop3Client_t **) = NULL;
int 			(FAR* NSPop3::pAdrGetOption)(pop3Client_t *, int, void *) = NULL;
int 			(FAR* NSPop3::pAdrInitialize)(pop3Client_t **, pop3SinkPtr_t) = NULL;
int 			(FAR* NSPop3::pAdrList)(pop3Client_t *) = NULL;
int 			(FAR* NSPop3::pAdrListA)(pop3Client_t *, int) = NULL;
int 			(FAR* NSPop3::pAdrNoop)(pop3Client_t *) = NULL;
int 			(FAR* NSPop3::pAdrPass)(pop3Client_t *, const char *) = NULL;
int 			(FAR* NSPop3::pAdrProcessResponses)(pop3Client_t *) = NULL;
int 			(FAR* NSPop3::pAdrQuit)(pop3Client_t *) = NULL;
int 			(FAR* NSPop3::pAdrReset)(pop3Client_t *) = NULL;
int 			(FAR* NSPop3::pAdrRetrieve)(pop3Client_t *, int) = NULL;
int 			(FAR* NSPop3::pAdrSendCommand)(pop3Client_t *, const char *, boolean) = NULL;
int 			(FAR* NSPop3::pAdrSetChunkSize)(pop3Client_t *, int) = NULL;
int 			(FAR* NSPop3::pAdrSetOption)(pop3Client_t *, int, void *) = NULL;
int 			(FAR* NSPop3::pAdrSetResponseSink)(pop3Client_t *, pop3SinkPtr_t) = NULL;
int 			(FAR* NSPop3::pAdrSetTimeout)(pop3Client_t *, double) = NULL;
void 			(FAR* NSPop3::pAdrSinkFree)(pop3Sink_t **) = NULL;
int 			(FAR* NSPop3::pAdrSinkInitialize)(pop3Sink_t **) = NULL;
int 			(FAR* NSPop3::pAdrStat)(pop3Client_t *) = NULL;
int 			(FAR* NSPop3::pAdrTop)(pop3Client_t *, int, int) = NULL;
int 			(FAR* NSPop3::pAdrUidList)(pop3Client_t *) = NULL;
int 			(FAR* NSPop3::pAdrUidListA)(pop3Client_t *, int) = NULL;
int 			(FAR* NSPop3::pAdrUser)(pop3Client_t *, const char *) = NULL;
int 			(FAR* NSPop3::pAdrXAuthList)(pop3Client_t *) = NULL;
int 			(FAR* NSPop3::pAdrXAuthListA)(pop3Client_t *, int) = NULL;
int 			(FAR* NSPop3::pAdrXSender)(pop3Client_t *, int) = NULL;

int 			(FAR* NSPop3::pAdrParseEntireMessage)(char *, int, struct mime_message **) = NULL;int  			(FAR* NSPop3::pAdrBasicPartFreeAll)(mime_basicPart_t * in_pBasicPart) = NULL;
int  			(FAR* NSPop3::pAdrBasicPartGetDataBuf)(mime_basicPart_t * in_pBasicPart, unsigned int * out_pSize, char ** out_ppDataBuf) = NULL;
void 			(FAR* NSPop3::pAdrMemFree)(void * in_pMem) = NULL;
int				(FAR* NSPop3::pAdrMessageFreeAll)(mime_message_t * in_pMessage) = NULL;
int				(FAR* NSPop3::pAdrMessageGetBody)(mime_message_t * pMessage, mime_content_type * in_pContentType, void ** out_ppTheBodyPart) = NULL;
int 			(FAR* NSPop3::pAdrMessagePartFreeAll)(mime_messagePart_t * in_pMessagePart) = NULL;
int				(FAR* NSPop3::pAdrMultiPartFreeAll)(mime_multiPart_t * in_pMultiPart) = NULL;
int				(FAR* NSPop3::pAdrMultiPartGetPart)(mime_multiPart_t * in_pMultiPart, int in_index, mime_content_type * in_pContentType, void ** out_ppTheBodyPart) = NULL;
int				(FAR* NSPop3::pAdrMultiPartGetPartCount)(mime_multiPart_t * in_pMultiPart, int * count) = NULL;
int  			(FAR* NSPop3::pAdrMessagePartGetMessage)(mime_messagePart_t * in_pMessagePart, mime_message_t ** out_ppMessage) = NULL;
int				(FAR* NSPop3::pAdrDecodeBase64)(nsmail_inputstream_t * in_pInput_stream, nsmail_outputstream_t * in_pOutput_stream) = NULL;
int				(FAR* NSPop3::pAdrDecodeHeaderString)(char * inString, char ** out_ppString) = NULL;
int				(FAR* NSPop3::pAdrDecodeQP)(nsmail_inputstream_t * in_pInput_stream, nsmail_outputstream_t * in_pOutput_stream) = NULL;

int 			(FAR* NSPop3::pAdrDataSinkNew)( mimeDataSink_t **pp ) = NULL;void 			(FAR* NSPop3::pAdrDataSinkFree)( mimeDataSink_t **pp ) = NULL;
int 			(FAR* NSPop3::pAdrDynamicParserNew)( mimeDataSink_t *pDataSink, struct mimeParser **pp) = NULL;
void 			(FAR* NSPop3::pAdrDynamicParserFree)( struct mimeParser **pp ) = NULL;
int 			(FAR* NSPop3::pAdrBeginDynamicParse)( struct mimeParser *p ) = NULL;
int 			(FAR* NSPop3::pAdrDynamicParse)( struct mimeParser *p, char *pData, int nLen ) = NULL;
int 			(FAR* NSPop3::pAdrEndDynamicParse)( struct mimeParser *p ) = NULL;

int				(FAR* NSPop3::pAdrInputStreamCreate)(char * in_fileName, nsmail_inputstream_t ** out_ppRetInputStream) = NULL;
int				(FAR* NSPop3::pAdrBufInputStreamCreate)(char * in_pDataBuf, long in_data_size, nsmail_inputstream_t ** out_ppRetInputStream) = NULL;
int				(FAR* NSPop3::pAdrOutputStreamCreate)(char * in_fileName, nsmail_outputstream_t ** out_ppRetOutputStream) = NULL;
void 			(FAR* NSPop3::pAdrStreamFree)(void * pMem) = NULL;

// Constructeur
NSPop3::NSPop3(TMailBox* pMailBox, string sServeur, int iPort, string sUser,
               string sPass, NSContexte* pCtx)
{
	pForm     = pMailBox ;
	pContexte = pCtx ;
	serveur   = sServeur ;

	port = iPort ;
	user = sUser ;
	pass = sPass ;

	pClient   = NULL ;
	pSink     = NULL ;
	pParser   = NULL ;
	pDataSink = NULL ;

	pMsgArray = new NSMsgArray ;
	nbMsg = 0 ;
	numMsg = -1 ;

	pMsgStockArray = new NSMsgArray ;
	nbMsgStock = 0 ;
	numMsgStock = -1 ;

	pPop3Module = new TModule("d:\\nautilus\\mail\\libpop3.dll", TRUE);
	if (!pPop3Module)
		erreur("libpop3.dll est introuvable", standardError, 0, pContexte->GetMainWindow()->GetHandle());
	else
	{
  	(FARPROC) pAdrConnect = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_CONNECT));
		(FARPROC) pAdrDelete = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_DELETE));
    (FARPROC) pAdrDisconnect = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_DISCONNECT));
    (FARPROC) pAdrFree = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_FREE));
    (FARPROC) pAdrGetOption = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_GET_OPTION));
    (FARPROC) pAdrInitialize = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_INITIALIZE));
    (FARPROC) pAdrList = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_LIST));
    (FARPROC) pAdrListA = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_LISTA));
    (FARPROC) pAdrNoop = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_NOOP));
    (FARPROC) pAdrPass = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_PASS));
    (FARPROC) pAdrProcessResponses = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_PROCESS_RESPONSES));
    (FARPROC) pAdrQuit = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_QUIT));
    (FARPROC) pAdrReset = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_RESET));
    (FARPROC) pAdrRetrieve = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_RETRIEVE));
    (FARPROC) pAdrSendCommand = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_SEND_COMMAND));
    (FARPROC) pAdrSetChunkSize = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_SET_CHUNK_SIZE));
    (FARPROC) pAdrSetOption = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_SET_OPTION));
    (FARPROC) pAdrSetResponseSink = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_SET_RESPONSE_SINK));
    (FARPROC) pAdrSetTimeout = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_SET_TIMEOUT));
    (FARPROC) pAdrSinkFree = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_SINK_FREE));
    (FARPROC) pAdrSinkInitialize = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_SINK_INITIALIZE));
    (FARPROC) pAdrStat = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_STAT));
    (FARPROC) pAdrTop = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_TOP));
    (FARPROC) pAdrUidList = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_UID_LIST));
    (FARPROC) pAdrUidListA = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_UID_LISTA));
    (FARPROC) pAdrUser = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_USER));
    (FARPROC) pAdrXAuthList = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_XAUTH_LIST));
    (FARPROC) pAdrXAuthListA = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_XAUTH_LISTA));
    (FARPROC) pAdrXSender = pPop3Module->GetProcAddress(MAKEINTRESOURCE(RES_XSENDER));
	}

	pMimeModule = new TModule("d:\\nautilus\\mail\\libmime.dll", TRUE);
  if (!pMimeModule)
  	erreur("libmime.dll est introuvable", standardError, 0, pContexte->GetMainWindow()->GetHandle());
	else
	{
  	(FARPROC) pAdrParseEntireMessage = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_PARSE_ENTIRE_MESSAGE));
    (FARPROC) pAdrBasicPartFreeAll = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_BASICPART_FREE_ALL));
    (FARPROC) pAdrBasicPartGetDataBuf = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_BASICPART_GETDATABUF));
    (FARPROC) pAdrMemFree = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_MEMFREE));
    (FARPROC) pAdrMessageFreeAll = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_MESSAGE_FREE_ALL));
    (FARPROC) pAdrMessageGetBody = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_MESSAGE_GETBODY));
    (FARPROC) pAdrMessagePartFreeAll = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_MESSAGEPART_FREE_ALL));
    (FARPROC) pAdrMultiPartFreeAll = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_MULTIPART_FREE_ALL));
    (FARPROC) pAdrMultiPartGetPart = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_MULTIPART_GETPART));
    (FARPROC) pAdrMultiPartGetPartCount = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_MULTIPART_GETPARTCOUNT));
    (FARPROC) pAdrMessagePartGetMessage = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_MESSAGEPART_GETMESSAGE));
    (FARPROC) pAdrDecodeBase64 = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_DECODE_BASE64));
    (FARPROC) pAdrDecodeHeaderString = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_DECODE_HEADER_STRING));
    (FARPROC) pAdrDecodeQP = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_DECODE_QP));

    (FARPROC) pAdrDataSinkNew = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_DATASINK_NEW));
    (FARPROC) pAdrDataSinkFree = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_DATASINK_FREE));
    (FARPROC) pAdrDynamicParserNew = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_DYNAMIC_PARSER_NEW));
    (FARPROC) pAdrDynamicParserFree = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_DYNAMIC_PARSER_FREE));
    (FARPROC) pAdrBeginDynamicParse = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_BEGIN_DYNAMIC_PARSE));
    (FARPROC) pAdrDynamicParse = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_DYNAMIC_PARSE));
    (FARPROC) pAdrEndDynamicParse = pMimeModule->GetProcAddress(MAKEINTRESOURCE(RES_END_DYNAMIC_PARSE));
	}

	pCommModule = new TModule("d:\\nautilus\\mail\\libcomm.dll", TRUE);	if (!pCommModule)
  	erreur("libcomm.dll est introuvable", standardError, 0, pContexte->GetMainWindow()->GetHandle());
	else
	{
  	(FARPROC) pAdrInputStreamCreate = pCommModule->GetProcAddress(MAKEINTRESOURCE(RES_INPUT_STREAM_CREATE));
    (FARPROC) pAdrBufInputStreamCreate = pCommModule->GetProcAddress(MAKEINTRESOURCE(RES_BUF_INPUT_STREAM_CREATE));
    (FARPROC) pAdrOutputStreamCreate = pCommModule->GetProcAddress(MAKEINTRESOURCE(RES_OUTPUT_STREAM_CREATE));
    (FARPROC) pAdrStreamFree = pCommModule->GetProcAddress(MAKEINTRESOURCE(RES_STREAM_FREE));
	}
}

// destructeurNSPop3::~NSPop3()
{
	delete pMsgArray ;
	delete pPop3Module ;
	delete pMimeModule ;
}

voidNSPop3::Connect()
{
	int ret ;

	/*Initialize the response sink.*/	ret = (*pAdrSinkInitialize)( &pSink ) ;
	if ( ret != NSMAIL_OK )
	{
  	ShowMessage("Echec de l'initialisation du Sink") ;
    return ;
	}

  /*Set the function pointers on the response sink.*/
  SetSink() ;

  /*Initialize the client passing in the response sink.*/
  ret = (*pAdrInitialize)( &pClient, pSink ) ;
  if ( ret != NSMAIL_OK )
  {
  	ShowMessage("Echec de l'initialisation du Client") ;
    return ;
  }

  /*Connect to the POP3 server.*/
  ret = (*pAdrConnect)( pClient, serveur.c_str(), port ) ;
  if ( ret != NSMAIL_OK )
  {
  	ShowMessage("Echec de la connection") ;
    return ;
  }

  ret = (*pAdrProcessResponses)( pClient ) ;
  if ( ret != NSMAIL_OK )
  {
  	return ;
  }

  /*Specify a user name.*/
  ret = (*pAdrUser)( pClient, user.c_str() ) ;
  if ( ret != NSMAIL_OK )
  {
  	ShowMessage("Utilisateur non reconnu") ;
    return ;
  }

	ret = (*pAdrProcessResponses)( pClient ) ;
	if ( ret != NSMAIL_OK )
	{
		return ;
	}

	/*Specify a password.*/
	ret = (*pAdrPass)( pClient, pass.c_str() ) ;
	if ( ret != NSMAIL_OK )
	{
  	ShowMessage("Password non valide") ;
    return ;
  }

	ret = (*pAdrProcessResponses)( pClient ) ;
  if ( ret != NSMAIL_OK )
  {
  	return ;
  }

	// on lance la liste des messages (calcul de nbMsg)
	List() ;
}

void
NSPop3::Disconnect()
{
	int ret;

	/*Quit.*/
	ret = (*pAdrQuit)( pClient ) ;
	if ( ret != NSMAIL_OK )
	{
  	ShowMessage("Echec de la fonction QUIT du Pop3") ;
    return ;
	}

	ret = (*pAdrProcessResponses)( pClient ) ;
	if ( ret != NSMAIL_OK )
	{
		return ;
	}

	/*Free the client structure.*/
  (*pAdrFree)( &pClient ) ;
  /*Free the sink structure.*/
  (*pAdrSinkFree)( &pSink ) ;
}

void
NSPop3::SetSink()
{
	pSink->connect = NSPOP3_connect;
  pSink->dele = NSPOP3_delete;
  pSink->error = NSPOP3_error;
  pSink->listStart = NSPOP3_listStart;
  pSink->list = NSPOP3_list;
  pSink->listComplete = NSPOP3_listComplete;
  pSink->noop = NSPOP3_noop;
  pSink->pass = NSPOP3_pass;
  pSink->quit = NSPOP3_quit;
  pSink->reset = NSPOP3_reset;
  pSink->retrieveStart = NSPOP3_retrieveStart;
  pSink->retrieve = NSPOP3_retrieve;
  pSink->retrieveComplete = NSPOP3_retrieveComplete;
  pSink->sendCommandStart = NSPOP3_sendCommandStart;
  pSink->sendCommand = NSPOP3_sendCommand;
  pSink->sendCommandComplete = NSPOP3_sendCommandComplete;
  pSink->stat = NSPOP3_stat;
  pSink->topStart = NSPOP3_topStart;
  pSink->top = NSPOP3_top;
  pSink->topComplete = NSPOP3_topComplete;
  pSink->uidListStart = NSPOP3_uidListStart;
  pSink->uidList = NSPOP3_uidList;
  pSink->uidListComplete = NSPOP3_uidListComplete;
  pSink->user = NSPOP3_user;
  pSink->xAuthListStart = NSPOP3_xAuthListStart;
  pSink->xAuthList = NSPOP3_xAuthList;
  pSink->xAuthListComplete = NSPOP3_xAuthListComplete;
  pSink->xSender = NSPOP3_xSender;}

voidNSPop3::List()
{
	int ret ;

	pMsgArray->vider() ;
	nbMsg = 0 ;

	/*List all messages.*/
	ret = (*pAdrList)( pClient ) ;
	if ( ret != NSMAIL_OK )
	{
		ShowMessage("Echec de la fonction Liste du Pop3") ;
    return ;
	}

	ret = (*pAdrProcessResponses)( pClient ) ;
	if ( ret != NSMAIL_OK )
	{
  	return ;
	}}

voidNSPop3::List(int noMsg)
{
	int ret ;

	/*List a specified message.*/
	ret = (*pAdrListA)( pClient, noMsg ) ;
	if ( ret != NSMAIL_OK )
	{
  	ShowMessage("Echec de la fonction ListA du Pop3") ;
    return ;
  }

	ret = (*pAdrProcessResponses)( pClient ) ;
	if ( ret != NSMAIL_OK )
	{
  	return ;
	}}

voidNSPop3::Retrieve(int noMsg)
{
	int ret;

	/*Retrieve a message.*/
    ret = (*pAdrRetrieve)( pClient, noMsg );
    if ( ret != NSMAIL_OK )
    {
    	ShowMessage("Echec de la fonction Retrieve du Pop3");
        return;
    }

    ret = (*pAdrProcessResponses)( pClient );
    if ( ret != NSMAIL_OK )
    {
        return;
    }
}
voidNSPop3::Header(int noMsg)
{
	int ret;

	/*Retrieve a message.*/
    ret = (*pAdrTop)( pClient, noMsg, 0 );
    if ( ret != NSMAIL_OK )
    {
    	ShowMessage("Echec de la fonction Top du Pop3");
        return;
    }

    ret = (*pAdrProcessResponses)( pClient );
    if ( ret != NSMAIL_OK )
    {
        return;
    }}

boolNSPop3::Stocker(int noMsg)
{
	NSMsgIter 	i;
    int			j;
    NSMsg		msg;
    bool		bDelete = false;

	for (i = pMsgArray->begin(), j = 1; i != pMsgArray->end(); i++, j++)
   	{
    	if (j == noMsg)
        {
        	msg = *(*i);
   			delete *i;
      		pMsgArray->erase(i);
            bDelete = true;
            break;
        }
   	}

    if (!bDelete)
    {
    	erreur("Le message n'existe pas dans la liste des messages",standardError,0,pContexte->GetMainWindow()->GetHandle());
        return false;
    }

    if (!StockageBase(msg))
    {
    	erreur("Impossible de stocker le message dans la base",standardError,0,pContexte->GetMainWindow()->GetHandle());
        return false;
    }

    pMsgStockArray->push_back(new NSMsg(msg));
    nbMsgStock++;

    return true;
}

boolNSPop3::StockageBase(NSMsg message)
{
	bool		bExiste = false;
    bool		bTableVide = false;
    string		sCodeMsg;
    string  	sDateNau, sHeureNau;
    NSPartArray* pImageArray;
    NSPartIter 	i, k;
    int			j;

	// on ouvre la base des messages
   	NSMessage* pMsg = new NSMessage(pContexte);

   	pMsg->lastError = pMsg->open();
   	if (pMsg->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à l'ouverture du fichier Messages.db", standardError, pMsg->lastError, pContexte->GetMainWindow()->GetHandle());
		delete pMsg;
		return false;
   	}

    // on trouve le code du dernier message
    // on regarde si le message n'existe pas déjà
    pMsg->lastError = pMsg->debut(dbiWRITELOCK);
   	if ((pMsg->lastError != DBIERR_NONE) && (pMsg->lastError != DBIERR_EOF))
	{
		erreur("Erreur de positionnement dans la base des messages.", standardError, pMsg->lastError, pContexte->GetMainWindow()->GetHandle());
		pMsg->close();
		delete pMsg;
		return false;
	}

    // si la table est vide debut() renvoie DBIERR_EOF
    if (pMsg->lastError == DBIERR_EOF)
    	bTableVide = true;

    while (pMsg->lastError != DBIERR_EOF)
   	{
   		pMsg->lastError = pMsg->getRecord();
		if (pMsg->lastError != DBIERR_NONE)
		{
			erreur("Erreur de lecture dans la base des messages.", standardError, pMsg->lastError, pContexte->GetMainWindow()->GetHandle());
			pMsg->close();
			delete pMsg;
			return false;
		}

        if ((message.pHeader->sMsgId) == string(pMsg->pDonnees->msg_id))
        {
        	bExiste = true;
            break;
        }

      	// on se positionne sur le message suivant
      	pMsg->lastError = pMsg->suivant(dbiWRITELOCK);
		if ((pMsg->lastError != DBIERR_NONE) && (pMsg->lastError != DBIERR_EOF))
      	{
			erreur("Erreur d'accès au message suivant.", standardError, pMsg->lastError, pContexte->GetMainWindow()->GetHandle());
         	pMsg->close();
			delete pMsg;
			return false;
      	}
    }

    if (bExiste)
    {
    	erreur("Attention : ce message existe déjà dans la base Nautilus.",standardError,0,pContexte->GetMainWindow()->GetHandle());
        return false;
    }

    // on recupère le code du dernier message
    if (bTableVide)
    	sCodeMsg = "0001";
    else
    {
    	sCodeMsg = string(pMsg->pDonnees->code);
        if (!pMsg->IncrementeCode(&sCodeMsg))
        {
        	erreur("Pb à l'attribution du code du nouveau message.",standardError,0,pContexte->GetMainWindow()->GetHandle());
            return false;
        }
    }

    strcpy(pMsg->pDonnees->code, sCodeMsg.c_str());
    strcpy(pMsg->pDonnees->msg_id, (message.pHeader->sMsgId).c_str());
    strcpy(pMsg->pDonnees->content_type, (message.pHeader->sContentType).c_str());
    strcpy(pMsg->pDonnees->from, (message.pHeader->sFrom).c_str());
    strcpy(pMsg->pDonnees->subject, (message.pHeader->sSubject).c_str());

    donne_date_inverse(message.pHeader->sDate, sDateNau, 0);
    strcpy(pMsg->pDonnees->date, sDateNau.c_str());
    sHeureNau = string(message.pHeader->sHeure,0,2) + string(message.pHeader->sHeure,3,2) +
    			string(message.pHeader->sHeure,6,2);
    strcpy(pMsg->pDonnees->heure, sHeureNau.c_str());

    strcpy(pMsg->pDonnees->priority, (message.pHeader->sPriority).c_str());
    strcpy(pMsg->pDonnees->delmsg, "0");

    // on l'insère dans la base Messages
   	pMsg->lastError = pMsg->appendRecord();
   	if (pMsg->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à l'insertion du message dans la base Messages.", standardError, pMsg->lastError, pContexte->GetMainWindow()->GetHandle());
      	pMsg->close();
      	delete pMsg;
      	return false;
   	}

    pMsg->lastError = pMsg->close();
   	if (pMsg->lastError != DBIERR_NONE)
		erreur("Erreur de fermeture de la base des Messages.", standardError, pMsg->lastError, pContexte->GetMainWindow()->GetHandle());

	delete pMsg;

    // Stockage des différentes parts du message

    if (!StockagePart(sCodeMsg, message.pBody, 0))
    {
    	erreur("Erreur au stockage du corps du message dans la base MsgParts.",standardError,0,pContexte->GetMainWindow()->GetHandle());
        return false;
    }

    for (i = message.pPartArray->begin(), j = 1; i != message.pPartArray->end(); i++, j++)
    {
    	if (!StockagePart(sCodeMsg, *i, j))
        {
        	char msgErr[255];
        	sprintf(msgErr, "Erreur au stockage de la part n°%d du message dans la base MsgParts.", j);
            erreur(msgErr,standardError,0,pContexte->GetMainWindow()->GetHandle());
            return false;
        }
    }

    // changement des références images dans les part html
    // ---------------------------------------------------

    pImageArray = new NSPartArray;
    InitImageArray(message.pPartArray, pImageArray);

	for (i = message.pPartArray->begin(); i != message.pPartArray->end(); i++)
    {
    	if ((*i)->sType == "html")
        {
        	for (k = pImageArray->begin(), j = 1; k != pImageArray->end(); k++, j++)
            {
        		if (!RemplaceNomImage((*i)->sNewFileName, (*k)->sFileName, (*k)->sNewFileName))
                {
                	char msgErr[255];
        			sprintf(msgErr, "Erreur au remplacement de l'image n°%d du message.", j);
            		erreur(msgErr,standardError,0,pContexte->GetMainWindow()->GetHandle());
                }
            }
        }
    }
    ShowMessage("Message stocké dans la base.");

    return true;
}

boolNSPop3::StockagePart(string sCodeMessage, NSBasicPart* pPart, int iCode)
{
    int			ret;
	char 		cCode[4];
    string		sCodePart;
    ofstream 	outFile;
    string		sLocalis, sUnite, sServeur;
    string		sExt, sPath, sFichier;
    string		sData;
    char*		cData;
    char*		cFichier;
    long		lSize;
    nsmail_inputstream_t* 	pInputStream;
    nsmail_outputstream_t*	pOutputStream;

    numacar(cCode, iCode, 3);
	sCodePart = sCodeMessage + "_" + string(cCode);

    // on récupère l'extension en fonction du type de part
    if 		(pPart->sType == "plain")
    	sExt = "txt";
    else if (pPart->sType == "html")
    	sExt = "htm";
    else if (pPart->sType == "jpeg")
    	sExt = "jpg";
    else if (pPart->sType == "mpeg")
    	sExt = "mpg";
    else
    {
    	erreur("Le message contient un fichier dont le type n'est pas géré par Nautilus.",standardError,0,pContexte->GetMainWindow()->GetHandle());
        return false;
    }

    // on crée le fichier de sortie correspondant à la part
    sPath = pContexte->getSuperviseur()->PathNameType("MSIN", sLocalis, sUnite, sServeur, pContexte);
    sFichier = nom_sans_doublons(sServeur, sUnite, sPath, sCodePart, sExt);
    sFichier = sPath + string("\\") + sFichier;

    // on regarde en fonction de l'encoding si on doit préalablement décoder les datas
    switch (pPart->iEncoding)
    {
    	case MIME_ENCODING_UNINITIALIZED :

        	erreur("Type d'encoding non initialisé.",standardError,0,pContexte->GetMainWindow()->GetHandle());
            return false;

     	case MIME_ENCODING_BASE64 :

        	lSize = (long) strlen((pPart->sData).c_str());
            cData = new char[lSize + 1];
            cFichier = new char[strlen(sFichier.c_str()) + 1];

            strcpy(cData, (pPart->sData).c_str());
            strcpy(cFichier, sFichier.c_str());

            // on branche les data sur l'inputStream
        	ret = (*pAdrBufInputStreamCreate)(cData, lSize, &pInputStream);
    		if ( ret != NSMAIL_OK )
    		{
    			ShowMessage("Echec de la fonction BufInputStreamCreate du Pop3");
                delete[] cFichier;
                delete[] cData;
        		return false;
    		}
            // on branche le fichier de sortie sur l'outputStream            ret = (*pAdrOutputStreamCreate)(cFichier, &pOutputStream);
            if ( ret != NSMAIL_OK )
    		{
    			ShowMessage("Echec de la fonction OutputStreamCreate du Pop3");
                delete[] cFichier;
                delete[] cData;
        		return false;
    		}
            // on décode les data base 64            ret = (*pAdrDecodeBase64)(pInputStream, pOutputStream);
            if ( ret != NSMAIL_OK )
    		{
    			ShowMessage("Echec de la fonction DecodeBase64 du Pop3");
                delete[] cFichier;
                delete[] cData;
        		return false;
    		}
            /* close and free up the input and output streams */
			pOutputStream->close(pOutputStream->rock);
			(*pAdrStreamFree)(pOutputStream);
			pInputStream->close(pInputStream->rock);
			(pAdrStreamFree)(pInputStream);
            delete[] cFichier;            delete[] cData;
            break;

        case MIME_ENCODING_QP :
        	// On voulait utiliser (*pAdrDecodeQP)(pInputStream, pOutputStream);            // mais ça ne marche pas...
            sData = pPart->sData;
            DecodeQP(sData);

            outFile.open(sFichier.c_str());
    		if (!outFile)
    		{
   	    		erreur("Impossible de créer un fichier contenu dans le message.", standardError, 0, pContexte->GetMainWindow()->GetHandle());
   	    		return false;
    		}

    		// on écrit les datas dans ce fichier
    		for (size_t i=0; i < strlen(sData.c_str()); i++)
        		outFile.put(sData[i]);

			outFile.close();
            break;

        default :
    		outFile.open(sFichier.c_str());
    		if (!outFile)
    		{
   	    		erreur("Impossible de créer un fichier contenu dans le message.", standardError, 0, pContexte->GetMainWindow()->GetHandle());
   	    		return false;
    		}

    		// on écrit les datas dans ce fichier
    		for (size_t i=0; i < strlen((pPart->sData).c_str()); i++)
        		outFile.put(pPart->sData[i]);

			outFile.close();
    }

	// on ouvre la base des MsgParts
   	NSMsgPart* pMsgPart = new NSMsgPart(pContexte);

   	pMsgPart->lastError = pMsgPart->open();
   	if (pMsgPart->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à l'ouverture du fichier MsgParts.db", standardError, pMsgPart->lastError, pContexte->GetMainWindow()->GetHandle());
		delete pMsgPart;
		return false;
   	}

    strcpy(pMsgPart->pDonnees->message, sCodeMessage.c_str());
    strcpy(pMsgPart->pDonnees->code, cCode);
    strcpy(pMsgPart->pDonnees->stype, (pPart->sType).c_str());

    itoa(pPart->iType, pMsgPart->pDonnees->itype, 10);
    itoa(pPart->iEncoding, pMsgPart->pDonnees->encoding, 10);

    strcpy(pMsgPart->pDonnees->fichier, sFichier.c_str());
    // on stocke le nouveau nom de fichier en mémoire
    // pour pouvoir changer ultérieurement les références images dans les part html
    pPart->sNewFileName = sFichier;

    // on insere la part du message dans la base
    pMsgPart->lastError = pMsgPart->appendRecord();
    if (pMsgPart->lastError != DBIERR_NONE)
    {
    	erreur("Erreur à l'insertion d'un fichier message dans la base MsgParts.", standardError, pMsgPart->lastError, pContexte->GetMainWindow()->GetHandle());
        pMsgPart->close();
        delete pMsgPart;
        return false;
    }

    pMsgPart->lastError = pMsgPart->close();
   	if (pMsgPart->lastError != DBIERR_NONE)
		erreur("Erreur de fermeture de la base MsgParts.", standardError, pMsgPart->lastError, pContexte->GetMainWindow()->GetHandle());

	delete pMsgPart;
    return true;
}

voidNSPop3::DecodeQP(string& sData)
{
	size_t  pos;
    string	sChar;
    char	cChar[4];

    pos = sData.find("=");

    while (pos != NPOS)
    {
    	strcpy(cChar, "");
    	sChar = string(sData, pos+1, 2);

        if (sChar == string("\r\n"))
        	sprintf(cChar, "%c", '\n');
        else
        {
        	// conversion de la valeur hexa en caractere
            sChar = string("0x") + sChar;
        	sprintf(cChar, "%c", StrToInt(sChar.c_str()));
        }
        sData.replace(pos, 3, cChar);
        pos = sData.find("=", pos+1);
    }
}

voidNSPop3::InitImageArray(NSPartArray* pArraySource, NSPartArray* pArrayDest)
{
	NSPartIter 	i;

	if ((pArraySource == NULL) || (pArraySource->empty()) )  	return;	for (i = pArraySource->begin(); i != pArraySource->end(); i++)    {    	switch ((*i)->iType)        {        	case MIME_CONTENT_IMAGE :    			case MIME_CONTENT_VIDEO :            	pArrayDest->push_back(new NSBasicPart(*(*i)));                break;        }    }}
boolNSPop3::RemplaceNomImage(string sFichierHtml, string sNomSource, string sNomDest)
{
	string 		sHtml = "";
    ifstream 	inFile;
   	ofstream 	outFile;
   	string   	line;
    size_t	    pos, pos1, pos2;

    sNomDest = string("\"") + sNomDest + string("\"");

   	inFile.open(sFichierHtml.c_str());
	if (!inFile)
   	{
    	erreur("Erreur à l'ouverture du fichier html à remplacer.", standardError, 0, pContexte->GetMainWindow()->GetHandle());
		return false;
   	}

   	while (!inFile.eof())
   	{
        getline(inFile,line);
        if (line != "")
            sHtml += line + "\n";
   	}
	inFile.close();

    outFile.open(sFichierHtml.c_str());
    if (!outFile)
    {
    	erreur("Impossible d'ouvrir le fichier html à remplacer.", standardError, 0, pContexte->GetMainWindow()->GetHandle());
        return false;
    }

    pos = sHtml.find(sNomSource);

    while (pos != NPOS)
    {
    	pos1 = sHtml.rfind("\"", pos);
        pos2 = sHtml.find("\"", pos1+1);

		sHtml.replace(pos1, pos2-pos1+1, sNomDest.c_str());
        pos = sHtml.find(sNomSource, pos1 + strlen(sNomDest.c_str()));
    }

    // ecriture du nouveau fichier html
   	for (size_t i=0; i < strlen(sHtml.c_str()); i++)
      	outFile.put(sHtml[i]);
	outFile.close();

    return true;
}

/*****************************************************************************//***							FONCTIONS MIME                    		   ***/
/*****************************************************************************/

bool
NSPop3::ParseDate(string sDateIn, string& sDateOut, string& sHeureOut)
{
	size_t 	pos1, pos2;
    string	sJour, sMois, sAnnee;
    string	sMoisAnglais;
    string	sHeure, sMinute, sSeconde;
    char	cDate[11], cHeure[9];

    // calcul de la date
    pos1 = sDateIn.find(" ");
    if (pos1 == NPOS)
    	return false;

	sJour = string(sDateIn, 0, pos1);

    pos2 = sDateIn.find(" ", pos1+1);
    if (pos2 == NPOS)
    	return false;

    sMoisAnglais = string(sDateIn, pos1+1, pos2-pos1-1);

    // Conversion du mois anglais en chiffres
    if 		(sMoisAnglais == "Jan")
    	sMois = "01";
    else if (sMoisAnglais == "Feb")
    	sMois = "02";
	else if (sMoisAnglais == "Mar")
    	sMois = "03";
    else if (sMoisAnglais == "Apr")
    	sMois = "04";
    else if (sMoisAnglais == "May")
    	sMois = "05";
    else if (sMoisAnglais == "Jun")
    	sMois = "06";
    else if (sMoisAnglais == "Jul")
    	sMois = "07";
    else if (sMoisAnglais == "Aug")
    	sMois = "08";
    else if (sMoisAnglais == "Sep")
    	sMois = "09";
    else if (sMoisAnglais == "Oct")
    	sMois = "10";
    else if (sMoisAnglais == "Nov")
    	sMois = "11";
    else if (sMoisAnglais == "Dec")
    	sMois = "12";
    else
    	return false;

    pos1 = pos2;
    pos2 = sDateIn.find(" ", pos1+1);
    if (pos2 == NPOS)
    	return false;

	sAnnee = string(sDateIn, pos1+1, pos2-pos1-1);

    // calcul de l'heure
    pos1 = pos2;
    pos2 = sDateIn.find(":", pos1+1);
	if (pos2 == NPOS)
    	return false;

    sHeure = string(sDateIn, pos1+1, pos2-pos1-1);

    pos1 = pos2;
    pos2 = sDateIn.find(":", pos1+1);
	if (pos2 == NPOS)
    	return false;

    sMinute = string(sDateIn, pos1+1, pos2-pos1-1);
    sSeconde = string(sDateIn, pos2+1, 2);

    // Conversion de la date au format jj/mm/aaaa
    sprintf(cDate, "%02d/%02d/%4d", atoi(sJour.c_str()), atoi(sMois.c_str()), atoi(sAnnee.c_str()));

    // Conversion de l'heure au format hh:mm:ss
    sprintf(cHeure, "%02d:%02d:%02d", atoi(sHeure.c_str()), atoi(sMinute.c_str()), atoi(sSeconde.c_str()));

    // Retour des données
    sDateOut = string(cDate);
    sHeureOut = string(cHeure);

    return true;
}

voidNSPop3::SetDataSink()
{
	pDataSink->header 				= &DataSink_header;

	pDataSink->addHeader 			= &DataSink_addHeader;
	pDataSink->endMessageHeader 	= &DataSink_endMessageHeader;
	pDataSink->contentType 			= &DataSink_contentType;
    pDataSink->contentSubType 		= &DataSink_contentSubType;
    pDataSink->contentTypeParams    = &DataSink_contentTypeParams;
    pDataSink->contentID 			= &DataSink_contentID;
    pDataSink->contentMD5 			= &DataSink_contentMD5;
    pDataSink->contentDisposition 	= &DataSink_contentDisposition;
    pDataSink->contentDispParams 	= &DataSink_contentDispParams;
    pDataSink->contentDescription 	= &DataSink_contentDescription;
    pDataSink->contentEncoding 		= &DataSink_contentEncoding;
    pDataSink->startMessage 		= &DataSink_startMessage;
    pDataSink->endMessage 			= &DataSink_endMessage;

    pDataSink->startBasicPart 		= &DataSink_startBasicPart;
    pDataSink->bodyData 			= &DataSink_bodyData;
    pDataSink->endBasicPart			= &DataSink_endBasicPart;

    pDataSink->startMultiPart 		= &DataSink_startMultiPart;
    pDataSink->boundary 			= &DataSink_boundary;
    pDataSink->endMultiPart 		= &DataSink_endMultiPart;

    pDataSink->startMessagePart 	= &DataSink_startMessagePart;
    pDataSink->endMessagePart 		= &DataSink_endMessagePart;
}
boolNSPop3::fStrEqual(char *s1, char *s2)
{
        static char achBuffer[64];
        int len;

        if ( s1 != NULL && s2 != NULL )
        {
                len = strlen( s2 );
                strncpy( achBuffer, s1, len );
                achBuffer[len] = 0;

            	if ( stricmp( achBuffer, s2 ) == 0 )
                        return true;
        }

        return false;
}

void
NSPop3::freeBodyPart(void * pBodyPart, mime_content_type contentType)
{
	switch (contentType)
	{
		case MIME_CONTENT_TEXT:
		case MIME_CONTENT_AUDIO:
		case MIME_CONTENT_IMAGE:
		case MIME_CONTENT_VIDEO:
		case MIME_CONTENT_APPLICATION:
			(*pAdrBasicPartFreeAll)((mime_basicPart_t *)pBodyPart);
			break;
		case MIME_CONTENT_MULTIPART:
			(*pAdrMultiPartFreeAll)((mime_multiPart_t *)pBodyPart);
			break;
		case MIME_CONTENT_MESSAGEPART:
			(*pAdrMessagePartFreeAll)((mime_messagePart_t *)pBodyPart);
			break;
	}
}

void
NSPop3::walkBodyPart(void * pBody, mime_content_type contentType)
{
	bool fBasicPart = false, fMultiPart = false, fMsgPart = false;
	mime_basicPart_t * pBasicPart;
	mime_multiPart_t * pMultiPart;
	mime_messagePart_t * pMessagePart;
	mime_message_t * pLocalMessage;
	unsigned int body_len=0;
	char * pDataBuf, *cont_subType;
	int i, ret = 0, part_count=0;
	mime_content_type cType;
	void * pBodyPart=NULL;
	static int count=0;

	switch (contentType)
	{
		case MIME_CONTENT_TEXT:
		case MIME_CONTENT_AUDIO:
		case MIME_CONTENT_IMAGE:
		case MIME_CONTENT_VIDEO:
		case MIME_CONTENT_APPLICATION:
			pBasicPart = (mime_basicPart_t *) pBody;
			fBasicPart = true;
			break;
		case MIME_CONTENT_MULTIPART:
			pMultiPart = (mime_multiPart_t *) pBody;
			fMultiPart = true;
			break;
		case MIME_CONTENT_MESSAGEPART:
			pMessagePart = (mime_messagePart_t *) pBody;
			fMsgPart = true;
			break;
	}

	if (fBasicPart == true)
	{
		count++;

		/* Or If you to retireve the bodydata in a buffer do */
		ret  = (*pAdrBasicPartGetDataBuf)(pBasicPart, &body_len, &pDataBuf);

		if (ret != MIME_OK)
		{
        	char errorMsg[255];
			sprintf(errorMsg, "mime_basicPart_getDataBuf failed! ret = %d", ret);
            erreur(errorMsg,standardError,0);
			return;
		}
		else
		{
		      /* Do what ever you want with the bodydata. When done, free up things */
		      (*pAdrMemFree)(pDataBuf);
		      pDataBuf = NULL;
		}

		/* When done with the basicPart itself, free it also */
		(*pAdrBasicPartFreeAll)(pBasicPart);
		pBasicPart = NULL;
	}
	else if (fMultiPart == true)
	{
		ret = (*pAdrMultiPartGetPartCount)(pMultiPart, &part_count);

		if (ret != MIME_OK)
		{
        	char errorMsg[255];
			sprintf(errorMsg, "mime_multiPart_getPartCount failed! ret = %d", ret);
            erreur(errorMsg,standardError,0);
			return;
		}

		for (i = 1; i <= part_count; i++)
		{
			ret = (*pAdrMultiPartGetPart)(pMultiPart, i, &cType, &pBodyPart);

			if (ret != MIME_OK)
			{
            	char errorMsg[255];
				sprintf(errorMsg, "mime_multiPart_getPartCount failed! ret = %d", ret);
				erreur(errorMsg,standardError,0);
                return;
			}

			walkBodyPart (pBodyPart, cType);

			/* Free the part when done */
			freeBodyPart (pBodyPart, cType);
			pBodyPart = NULL;
		}

		/* free the multi-part */
		(*pAdrMultiPartFreeAll)(pMultiPart);
		pMultiPart = NULL;
	}
	else if (fMsgPart == true)
	{
		cont_subType = pMessagePart->content_subtype;

		if (fStrEqual (cont_subType, "rfc822"))
		{
			/* get the message that is the body of this message-part */
			ret = (*pAdrMessagePartGetMessage)(pMessagePart, &pLocalMessage);
			walkMessage (pLocalMessage);

			/* free up the message and messagePart when done */
			(*pAdrMessageFreeAll)(pLocalMessage);
			pLocalMessage = NULL;
		}
		/* handle other sub-types as needed */

		/* free the message-part when done */
		(*pAdrMessagePartFreeAll)(pMessagePart);
		pMessagePart = NULL;
	}
}

void
NSPop3::walkMessage(mime_message_t * pMessage)
{
	int ret = 0;
	void * pBody;
	mime_header_t  * pRFC822_hdrs;
	mime_content_type contentType;

	/* walk through the headers as needed */
	pRFC822_hdrs = pMessage->rfc822_headers;
	/* Now simply walk through the list of headers */

	/* walk through the Body of the message */
	ret = (*pAdrMessageGetBody)(pMessage, &contentType, &pBody);

	if (ret != MIME_OK)
	{
    	char errorMsg[255];
		sprintf(errorMsg, "mime_message_getBody failed! ret = %d", ret);
		erreur(errorMsg,standardError,0);
        return;
	}

 	walkBodyPart (pBody, contentType);

	/* free the body-part */
	freeBodyPart (pBody, contentType);
	pBody = NULL;
}
/*****************************************************************************/
/***					FONCTIONS DE REPONSE AUX EVENTS                    ***/
/*****************************************************************************/

/*Notification for the response to the connection to the server.*/

void
NSPop3::NSPOP3_connect( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
    pForm->StatusBar->SimpleText = in_responseMessage;
    pForm->bConnect = true;
}
voidNSPop3::NSPOP3_delete( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
}

voidNSPop3::NSPOP3_error( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
}

/*Notification for the start of the LIST command.*/
void
NSPop3::NSPOP3_listStart( pop3Sink_t * in_pPOP3Sink )
{
	pForm->SablierOn();
}

/*Notification for the response to the LIST command.*/
void
NSPop3::NSPOP3_list( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, int in_octetCount )
{
    if (in_messageNumber == 1)
    	pForm->LabelMsg->Caption = "1 message";
    else
    	pForm->LabelMsg->Caption = IntToStr(in_messageNumber) + " messages";

    nbMsg++;
}

/*Notification for the completion of the LIST command.*/
void
NSPop3::NSPOP3_listComplete( pop3Sink_t * in_pPOP3Sink )
{
    pForm->SablierOff();
}

voidNSPop3::NSPOP3_noop( pop3Sink_t * in_pPOP3Sink ){}voidNSPop3::NSPOP3_pass( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage ){}/*Notification for the response to the QUIT command.*/void
NSPop3::NSPOP3_quit(pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
    pForm->StatusBar->SimpleText = in_responseMessage;
    pForm->bConnect = false;
}
void
NSPop3::NSPOP3_reset( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage ){}voidNSPop3::NSPOP3_retrieveStart( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, int in_octetCount ){	int ret;    nbRetrieve = 0;    numMsg = in_messageNumber - 1;    sTextMsg = "";    pForm->LabelFrom->Caption = "Expéditeur";    pForm->LabelDate->Caption = "Date";    pForm->LabelObj->Caption = "Objet";    pForm->EditMsg->Clear();    // Procedures d'initialisation du dynamic parsing
    /* Create a new data-sink */
    ret = (*pAdrDataSinkNew)(&pDataSink);
    if (ret != MIME_OK)
    {
    	ShowMessage("Echec de l'initialisation du DataSink");
        return;
    }

    SetDataSink();

    /* create a new parser instance */
    ret = (*pAdrDynamicParserNew)(pDataSink, &pParser);
    if (ret != MIME_OK)
    {
    	(*pAdrDataSinkFree)(&pDataSink);
        ShowMessage("Echec de l'initialisation du Parser");
        return;
    }
    // On lance le parsing du message
    ret = (*pAdrBeginDynamicParse)(pParser);    if (ret != MIME_OK)
    {
    	char errorMsg[255];
        sprintf(errorMsg, "beginDynamicParse failed! ret = %d", ret);
        erreur(errorMsg,standardError,0);
        return;
    }
}
/*Notification for raw message from the RETR command.*/
void
NSPop3::NSPOP3_retrieve( pop3Sink_t * in_pPOP3Sink, const char * in_messageChunk )
{
	char* chunk = new char[strlen(in_messageChunk) + 1];
    int   ret;

    strcpy(chunk, in_messageChunk);
	sTextMsg += string(in_messageChunk);
    nbRetrieve++;

    // on parse le chunk en cours
    ret = (*pAdrDynamicParse)(pParser, chunk, strlen(chunk));

    if (ret != MIME_OK)
    {
    	char errorMsg[255];
		sprintf(errorMsg, "dynamicParse failed! ret = %d", ret);
		erreur(errorMsg,standardError,0);
        delete[] chunk;
        return;
    }

    delete[] chunk;
}

/*Notification for the completion of the RETR command.*/
void
NSPop3::NSPOP3_retrieveComplete( pop3Sink_t * in_pPOP3Sink )
{
	int ret = (*pAdrEndDynamicParse)(pParser) ;

	if (ret != MIME_OK)
	{
		char errorMsg[255];
		sprintf(errorMsg, "endDynamicParse failed! ret = %d", ret) ;
		erreur(errorMsg,standardError,0) ;
		return ;
	}

    /* free the parser and data-sink */
	(*pAdrDynamicParserFree)(&pParser);
	(*pAdrDataSinkFree)(&pDataSink);

	// On affiche maintenant le body dans la zone texte
	// (avec les infos du header en en-tete)
	pForm->LabelFrom->Caption = ((*pMsgArray)[numMsg]->pHeader->sFrom).c_str() ;
	pForm->LabelDate->Caption = ((*pMsgArray)[numMsg]->pHeader->sDate + " " +
    							 (*pMsgArray)[numMsg]->pHeader->sHeure).c_str() ;
	pForm->LabelObj->Caption = ((*pMsgArray)[numMsg]->pHeader->sSubject).c_str() ;

	pForm->EditMsg->Lines->Text = ((*pMsgArray)[numMsg]->pBody->sData).c_str() ;
}

/*Notification for the start of the extended command.*/
void
NSPop3::NSPOP3_sendCommandStart( pop3Sink_t * in_pPOP3Sink )
{
}

/*Notification for the response to sendCommand() method.*/
void
NSPop3::NSPOP3_sendCommand( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
}

/*Notification for the completion of the extended command.*/
void
NSPop3::NSPOP3_sendCommandComplete( pop3Sink_t * in_pPOP3Sink )
{
}

/*Notification for the response to the STAT command.*/
void
NSPop3::NSPOP3_stat( pop3Sink_t * in_pPOP3Sink, int in_messageCount, int in_octetCount )
{
}

/*Notification for the start of a message from the TOP command.*/
void
NSPop3::NSPOP3_topStart( pop3Sink_t * in_pPOP3Sink, int in_messageNumber )
{
	NSHeader* pHeaderNull = new NSHeader();

    pMsgArray->push_back(new NSMsg(pHeaderNull));
    numMsg = in_messageNumber - 1;
    delete pHeaderNull;
}
/*Notification for a line of the message from the TOP command.*/
void
NSPop3::NSPOP3_top( pop3Sink_t * in_pPOP3Sink, const char * in_responseLine )
{
	string 	ligne = string(in_responseLine);
    string  dateHead;
    string  subject;
    // char	subject1[255];
    // char**	subject2;
    bool	bOk = true;
    size_t 	pos ;

    if (string(ligne, 0, 11) == "Message-ID:")
    	(*pMsgArray)[numMsg]->pHeader->sMsgId = string(ligne, 12, strlen(ligne.c_str()) - 12);
    else if (string(ligne, 0, 5) == "From:")
    	(*pMsgArray)[numMsg]->pHeader->sFrom = string(ligne, 6, strlen(ligne.c_str()) - 6);
    else if (string(ligne, 0, 8) == "Subject:")
    {
    	subject = string(ligne, 9, strlen(ligne.c_str()) - 9);

        /****
        if (string(subject, 0, 2) == "=?")	// Message codé
        {
        	strcpy(subject1, subject.c_str());
            // subject2 = new char[255];
        	ret = (*pAdrDecodeHeaderString)(subject1, subject2);
            if (ret != MIME_OK)
            {
            	char msg[255];
            	sprintf(msg, "Echec du décodage de l'objet, message n°%d", numMsg+1);
            	erreur(msg,0,0);
                (*pMsgArray)[numMsg]->pHeader->sSubject = "xxx";
            }
            else
            	(*pMsgArray)[numMsg]->pHeader->sSubject = string(*subject2);

            // delete[] subject2;
        }
        else
        *****/
    		(*pMsgArray)[numMsg]->pHeader->sSubject = subject;
    }
    else if (string(ligne, 0, 5) == "Date:")
    {
        pos = ligne.find(",");
        if (pos != NPOS)
        {
        	dateHead = string(ligne, pos+2, 20);
    		bOk = ParseDate(dateHead, (*pMsgArray)[numMsg]->pHeader->sDate, (*pMsgArray)[numMsg]->pHeader->sHeure);
        }
        else
        	bOk = false;

		if (!bOk)
        {
            char msg[255];
            sprintf(msg, "Echec du parsing de la date, message n°%d", numMsg+1);
            erreur(msg,standardError,0);
        }
    }
    else if (string(ligne, 0, 13) == "Content-Type:")
		(*pMsgArray)[numMsg]->pHeader->sContentType = string(ligne, 14, strlen(ligne.c_str()) - 15);
    else if (string(ligne, 0, 11) == "X-Priority:")
    	(*pMsgArray)[numMsg]->pHeader->sPriority = string(ligne, 12, strlen(ligne.c_str()) - 12);
}

/*Notification for the completion of the TOP command.*/
void
NSPop3::NSPOP3_topComplete( pop3Sink_t * in_pPOP3Sink )
{
	TListItem* ListItem;
    ListItem = pForm->ListMsg->Items->Add();

    // 1ere colonne : spécifie si on a un fichier attaché
    if ((*pMsgArray)[numMsg]->pHeader->sContentType == "multipart/mixed")
    	ListItem->Caption = IntToStr(1);
    else if ((*pMsgArray)[numMsg]->pHeader->sContentType == "text/plain")
    	ListItem->Caption = "";
    else
    	ListItem->Caption = "x";

    ListItem->SubItems->Add(((*pMsgArray)[numMsg]->pHeader->sFrom).c_str());
    ListItem->SubItems->Add(((*pMsgArray)[numMsg]->pHeader->sSubject).c_str());
    ListItem->SubItems->Add(((*pMsgArray)[numMsg]->pHeader->sDate + " " +
    						 (*pMsgArray)[numMsg]->pHeader->sHeure).c_str());
}

/*Notification for the start of the UIDL command.*/
void
NSPop3::NSPOP3_uidListStart( pop3Sink_t * in_pPOP3Sink )
{
}

/*Notification for the response to the UIDL command.*/
void
NSPop3::NSPOP3_uidList( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, const char * in_uid )
{
}

/*Notification for the completion of the UIDL command.*/
void
NSPop3::NSPOP3_uidListComplete( pop3Sink_t * in_pPOP3Sink )
{
}

/**Notification for the response to the USER command.*/
void
NSPop3::NSPOP3_user( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage )
{
}

/*Notification for the start of the XAUTHLIST command.*/
void
NSPop3::NSPOP3_xAuthListStart( pop3Sink_t * in_pPOP3Sink )
{
}

/*Notification for the response to the XAUTHLIST command.*/
void
NSPop3::NSPOP3_xAuthList( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, const char * in_responseMessage )
{
}

/*Notification for the completion of the XAUTHLIST command.*/
void
NSPop3::NSPOP3_xAuthListComplete( pop3Sink_t * in_pPOP3Sink )
{
}

/*Notification for the response to the XSENDER command.*/
void
NSPop3::NSPOP3_xSender( pop3Sink_t * in_pPOP3Sink, const char * in_emailAddress )
{
}

/************************************************************************************/
/*** 						------ datasink methods ---------- 			    	  ***/
/************************************************************************************/

void
NSPop3::DataSink_header (mimeDataSinkPtr_t pSink, void *pCallbackObject, char *name, char *value)
{
}

void
NSPop3::DataSink_addHeader (mimeDataSinkPtr_t pSink, void *pCallbackObject, char *name, char *value)
{
}

void
NSPop3::DataSink_endMessageHeader (mimeDataSinkPtr_t pSink, void *pCallbackObject)
{
}

void
NSPop3::DataSink_contentType (mimeDataSinkPtr_t pSink, void *pCallbackObject, int nContentType)
{
	int index;

	if (numPart == 0)
		(*pMsgArray)[numMsg]->pBody->iType = nContentType;
    else if (numPart > 0)
    {
    	index = (*pMsgArray)[numMsg]->nbPart - 1;
    	(*((*pMsgArray)[numMsg]->pPartArray))[index]->iType = nContentType;
    }
}

void
NSPop3::DataSink_contentSubType (mimeDataSinkPtr_t pSink, void *pCallbackObject, char * contentSubType)
{
	int index;

	if (numPart == 0)
		(*pMsgArray)[numMsg]->pBody->sType = string(contentSubType);
    else if (numPart > 0)
    {
    	index = (*pMsgArray)[numMsg]->nbPart - 1;
    	(*((*pMsgArray)[numMsg]->pPartArray))[index]->sType = string(contentSubType);
    }
}

void
NSPop3::DataSink_contentTypeParams (mimeDataSinkPtr_t pSink, void *pCallbackObject, char * contentTypeParams)
{
}

void
NSPop3::DataSink_contentID (mimeDataSinkPtr_t pSink, void *pCallbackObject, char *contentID)
{
}

void
NSPop3::DataSink_contentMD5 (mimeDataSinkPtr_t pSink, void *pCallbackObject, char * contentMD5)
{
}

void
NSPop3::DataSink_contentDisposition (mimeDataSinkPtr_t pSink, void *pCallbackObject, int nContentDisposition)
{
}

void
NSPop3::DataSink_contentDispParams (mimeDataSinkPtr_t pSink, void *pCallbackObject, char * contentDispParams)
{
	int 	index;
    int		pos1, pos2;
    string  sContent;

    // on récupère un nom de fichier uniquement pour les parts et non pour le body
    if (numPart > 0)
    {
    	sContent = string(contentDispParams);
        pos1 = sContent.find("\"");
        pos2 = sContent.find("\"", pos1+1);
        sContent = string(sContent, pos1+1, pos2-pos1-1);
    	index = (*pMsgArray)[numMsg]->nbPart - 1;
    	(*((*pMsgArray)[numMsg]->pPartArray))[index]->sFileName = sContent;
    }
}

void
NSPop3::DataSink_contentDescription (mimeDataSinkPtr_t pSink, void *pCallbackObject, char *contentDescription)
{
}

void
NSPop3::DataSink_contentEncoding (mimeDataSinkPtr_t pSink, void *pCallbackObject, int nContentEncoding)
{
	int index;

	if (numPart == 0)
		(*pMsgArray)[numMsg]->pBody->iEncoding = nContentEncoding;
    else if (numPart > 0)
    {
    	index = (*pMsgArray)[numMsg]->nbPart - 1;
    	(*((*pMsgArray)[numMsg]->pPartArray))[index]->iEncoding = nContentEncoding;
    }
}

void*
NSPop3::DataSink_startMessage (mimeDataSinkPtr_t pSink)
{
	// on remet les parts à vide
	numPart = -1;
    return NULL ;
}

void
NSPop3::DataSink_endMessage (mimeDataSinkPtr_t pSink, void *pCallbackObject)
{
}

void*
NSPop3::DataSink_startBasicPart (mimeDataSinkPtr_t pSink)
{
	NSBasicPart partNull;

	numPart++;

    // si on est après le body (numPart == 0)
    if (numPart > 0)
    {
    	(*pMsgArray)[numMsg]->pPartArray->push_back(new NSBasicPart(partNull));
        (*pMsgArray)[numMsg]->nbPart++;
    }

    return NULL ;
}

void
NSPop3::DataSink_bodyData (mimeDataSinkPtr_t pSink, void *pCallbackObject, char bodyData[], int len)
{
	int index;

	if (numPart == 0)
		(*pMsgArray)[numMsg]->pBody->sData = string(bodyData);
    else if (numPart > 0)
    {
    	index = (*pMsgArray)[numMsg]->nbPart - 1;
    	(*((*pMsgArray)[numMsg]->pPartArray))[index]->sData = string(bodyData);
    }
}

void
NSPop3::DataSink_endBasicPart (mimeDataSinkPtr_t pSink, void *pCallbackObject)
{
}

void*
NSPop3::DataSink_startMultiPart (mimeDataSinkPtr_t pSink)
{
    return NULL ;
}

void
NSPop3::DataSink_boundary (mimeDataSinkPtr_t pSink, void *pCallbackObject, char * boundary)
{
}

void
NSPop3::DataSink_endMultiPart (mimeDataSinkPtr_t pSink, void *pCallbackObject)
{
}

void*
NSPop3::DataSink_startMessagePart (mimeDataSinkPtr_t pSink)
{
    return NULL ;
}

void
NSPop3::DataSink_endMessagePart (mimeDataSinkPtr_t pSink, void *pCallbackObject)
{
}

/****************************************************************************/
/***						CLASSE TMAILBOX                               ***//****************************************************************************/

TMailBox *MailBox;
//---------------------------------------------------------------------------
__fastcall TMailBox::TMailBox(TComponent* Owner, string Serveur, int Port, string User,								string Pass, NSContexte* pCtx)
	: TForm(Owner){
	pPop3 = new NSPop3(this, Serveur, Port, User, Pass, pCtx);
	bConnect = false;
}

__fastcall TMailBox::~TMailBox(){
	if (bConnect)
    	pPop3->Disconnect();

    delete pPop3;
}

void __fastcall TMailBox::SablierOn(){
	// MailBox->Cursor = crHourGlass;
}

void __fastcall TMailBox::SablierOff(){
	// MailBox->Cursor = crDefault;
}

//---------------------------------------------------------------------------void __fastcall TMailBox::ConsultButtonClick(TObject *Sender)
{
	// si le tableau est plein : on le vide et on recommence
	if (!pPop3->pMsgArray->empty())
    {
    	ListMsg->Items->Clear();
    	pPop3->pMsgArray->vider();
    	pPop3->numMsg = -1;
    }

    for (int i = 1; i <= pPop3->nbMsg; i++)
    	pPop3->Header(i);
}

//---------------------------------------------------------------------------void __fastcall TMailBox::DeconnectButtonClick(TObject *Sender)
{
	pPop3->Disconnect();
}

//---------------------------------------------------------------------------void __fastcall TMailBox::ConnectButtonClick(TObject *Sender)
{
	if (!bConnect)
  		pPop3->Connect();
    else
		ShowMessage("Déjà connecté");
}

//---------------------------------------------------------------------------void __fastcall TMailBox::StockButtonClick(TObject *Sender)
{
	int indexSelect;

    if (ListMsg->Selected != NULL)
    {
    	indexSelect = ListMsg->Selected->Index + 1;
        if (!pPop3->Stocker(indexSelect))
        	ShowMessage("Impossible de stocker le message.");
    }
    else
    	ShowMessage("Vous devez sélectionner un message.");
}

//---------------------------------------------------------------------------void __fastcall TMailBox::ListMsgClick(TObject *Sender)
{
    int indexSelect;

    if (ListMsg->Selected != NULL)    {
    	indexSelect = ListMsg->Selected->Index + 1;
        pPop3->Retrieve(indexSelect);
    }
}

//---------------------------------------------------------------------------
