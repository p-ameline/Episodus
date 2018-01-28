//---------------------------------------------------------------------------#ifndef EMailH
#define EMailH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <NMpop3.hpp>
#include <Psock.hpp>
#include <owl\module.h>

#include "mail\pop3.h"#include "mail\mime.h"
#include "mail\mimeparser.h"
#include "mail\nsstream.h"
#include "partage\ns_vector.h"
#include "partage\NTArray.h"

class NSHeader{
	public :

		string 	sMsgId;        string  sContentType;
    	string	sFrom;
    	string	sSubject;
    	string	sDate;
        string  sHeure;
    	string	sPriority;

    	NSHeader()        { sMsgId = ""; sContentType = ""; sFrom = ""; sSubject = "";
          sDate = ""; sHeure = ""; sPriority = ""; }
        NSHeader(NSHeader& rv)
        { sMsgId = rv.sMsgId; sContentType = rv.sContentType; sFrom = rv.sFrom;
          sSubject = rv.sSubject; sDate = rv.sDate; sHeure = rv.sHeure;
          sPriority = rv.sPriority; }
        NSHeader& operator=(NSHeader src)
        { sMsgId = src.sMsgId; sContentType = src.sContentType; sFrom = src.sFrom;
          sSubject = src.sSubject; sDate = src.sDate; sHeure = src.sHeure;
          sPriority = src.sPriority; return *this; }
    	~NSHeader() {}
};

class NSBasicPart{
	public :

    	string	sType;        int		iType;
        int		iEncoding;
        string	sFileName;
        string  sData;
        string  sNewFileName;

        NSBasicPart()        { sType = ""; iType = -1; iEncoding = -1;
          sFileName = ""; sData = ""; sNewFileName = ""; }
        NSBasicPart(NSBasicPart& rv)
        { sType = rv.sType; iType = rv.iType; iEncoding = rv.iEncoding;
          sFileName = rv.sFileName; sData = rv.sData; sNewFileName = rv.sNewFileName; }
        NSBasicPart& operator=(NSBasicPart src)
        { sType = src.sType; iType = src.iType; iEncoding = src.iEncoding;
          sFileName = src.sFileName; sData = src.sData; sNewFileName = src.sNewFileName;
          return *this; }
        ~NSBasicPart() {}
};

//// Définition de NSPartArray (Array de NSBasicPart(s))
//
typedef vector<NSBasicPart*> NSPartVector;
typedef NSPartVector::iterator NSPartIter;
typedef NTArray<NSBasicPart> NSPartArray;

/*
class NSPartArray : public NSPartVector{
    public :

	// Constructeurs	NSPartArray() : NSPartVector() {}
	NSPartArray(NSPartArray& rv);
    // operateur =
    NSPartArray& operator=(NSPartArray src);
	// Destructeur
	virtual ~NSPartArray();
    void vider();
};   */

class NSMsg{
	public :

    	NSHeader*		pHeader;
        NSBasicPart*	pBody;
        NSPartArray*	pPartArray;
        int				nbPart;

        NSMsg();        NSMsg(NSHeader* pNSHead);
        NSMsg(NSMsg& rv);
        NSMsg& operator=(NSMsg src);
        ~NSMsg();
};

//// Définition de NSMsgArray (Array de NSMsg(s))
//
typedef vector<NSMsg*> NSMsgVector;
typedef NSMsgVector::iterator NSMsgIter;

class NSMsgArray : public NSMsgVector{

    public :
	// Constructeurs    NSMsgArray() : NSMsgVector() {}
	NSMsgArray(NSMsgArray& rv);

    // operateur =    NSMsgArray& operator=(NSMsgArray src);

	// Destructeur	virtual ~NSMsgArray();
    void vider();
};

class TMailBox;class NSContexte;

class NSPop3{
	public :

    	static TMailBox*	pForm;        NSContexte*			pContexte;
    	string 				serveur;
        int					port;
        string				user;
        string				pass;

        static pop3Client_t* 		pClient;    	static pop3Sink_t* 	 		pSink;
        static struct mimeParser* 	pParser;
        static mimeDataSink_t* 		pDataSink;

        static NSMsgArray*	pMsgArray;        static int			nbMsg;
        static int			numMsg;
        static int			numPart;
        static string		sTextMsg;
        static int			nbRetrieve;

        static NSMsgArray*	pMsgStockArray;        static int			nbMsgStock;
        static int			numMsgStock;
        static int			numPartStock;

        TModule*			pPop3Module;        TModule*			pMimeModule;
        TModule*			pCommModule;

    	NSPop3(TMailBox* pMailBox, string sServeur, int iPort, string sUser,               string sPass, NSContexte* pCtx);
        ~NSPop3();

        void Connect();        void Disconnect();
        void SetSink();
        void List();
        void List(int noMsg);
        void Retrieve(int noMsg);
        void Header(int noMsg);
        bool Stocker(int noMsg);
        bool StockageBase(NSMsg message);
        bool StockagePart(string sCodeMessage, NSBasicPart* pPart, int iCode);
        void DecodeQP(string& sData);
        void InitImageArray(NSPartArray* pArraySource, NSPartArray* pArrayDest);
        bool RemplaceNomImage(string sFichierHtml, string sNomSource, string sNomDest);

        // fonctions mime        static bool ParseDate(string sDateIn, string& sDateOut, string& sHeureOut);
        static void SetDataSink();
        static bool fStrEqual(char *s1, char *s2);
        static void freeBodyPart(void * pBodyPart, mime_content_type contentType);
        static void walkBodyPart(void * pBody, mime_content_type contentType);
        static void walkMessage(mime_message_t * pMessage);

        // Déclaration des pointeurs de Fonctions Pop3        static int 	(FAR* pAdrConnect)( pop3Client_t * in_pPOP3, const char * in_server, unsigned short in_port );
        static int 	(FAR* pAdrDelete)( pop3Client_t * in_pPOP3, int in_messageNumber );
        static int 	(FAR* pAdrDisconnect)( pop3Client_t * in_pPOP3 );
        static void (FAR* pAdrFree)( pop3Client_t ** in_ppPOP3 );
        static int	(FAR* pAdrGetOption)( pop3Client_t * in_pPOP3, int in_option, void * in_pOptionData );
        static int	(FAR* pAdrInitialize)( pop3Client_t ** out_ppPOP3, pop3SinkPtr_t in_pPOP3Sink );
        static int	(FAR* pAdrList)( pop3Client_t * in_pPOP3 );
        static int	(FAR* pAdrListA)( pop3Client_t * in_pPOP3, int in_messageNumber );
        static int	(FAR* pAdrNoop)( pop3Client_t * in_pPOP3 );
        static int	(FAR* pAdrPass)( pop3Client_t * in_pPOP3, const char * in_password );
        static int	(FAR* pAdrProcessResponses)( pop3Client_t * in_pPOP3 );
        static int	(FAR* pAdrQuit)( pop3Client_t * in_pPOP3 );
        static int	(FAR* pAdrReset)( pop3Client_t * in_pPOP3 );
        static int	(FAR* pAdrRetrieve)( pop3Client_t * in_pPOP3, int in_messageNumber );
        static int	(FAR* pAdrSendCommand)( pop3Client_t * in_pPOP3, const char * in_command, boolean in_multiLine );
        static int	(FAR* pAdrSetChunkSize)( pop3Client_t * in_pPOP3, int in_chunkSize );
        static int	(FAR* pAdrSetOption)( pop3Client_t * in_pPOP3, int in_option, void * in_pOptionData );
        static int	(FAR* pAdrSetResponseSink)( pop3Client_t * in_pPOP3, pop3SinkPtr_t in_pPOP3Sink );
        static int	(FAR* pAdrSetTimeout)( pop3Client_t * in_pPOP3, double in_timeout );
        static void	(FAR* pAdrSinkFree)( pop3Sink_t ** in_ppPOP3Sink );
        static int	(FAR* pAdrSinkInitialize)( pop3Sink_t ** out_ppPOP3Sink );
        static int	(FAR* pAdrStat)( pop3Client_t * in_pPOP3 );
        static int	(FAR* pAdrTop)( pop3Client_t * in_pPOP3, int in_messageNumber, int in_lines );
        static int	(FAR* pAdrUidList)( pop3Client_t * in_pPOP3 );
        static int	(FAR* pAdrUidListA)( pop3Client_t * in_pPOP3, int in_messageNumber );
        static int	(FAR* pAdrUser)( pop3Client_t * in_pPOP3, const char * in_user );
        static int	(FAR* pAdrXAuthList)( pop3Client_t * in_pPOP3 );
        static int	(FAR* pAdrXAuthListA)( pop3Client_t * in_pPOP3, int in_messageNumber );
        static int	(FAR* pAdrXSender)( pop3Client_t * in_pPOP3, int in_messageNumber );

        // Déclaration des pointeurs de Fonctions Mime        static int	(FAR* pAdrParseEntireMessage)(char * in_pData, int in_nLen, struct mime_message ** in_ppMimeMessage);
        static int  (FAR* pAdrBasicPartFreeAll)(mime_basicPart_t * in_pBasicPart);
        static int  (FAR* pAdrBasicPartGetDataBuf)(mime_basicPart_t * in_pBasicPart, unsigned int * out_pSize, char ** out_ppDataBuf);
        static void (FAR* pAdrMemFree)(void * in_pMem);
        static int	(FAR* pAdrMessageFreeAll)(mime_message_t * in_pMessage);
        static int	(FAR* pAdrMessageGetBody)(mime_message_t * pMessage, mime_content_type * in_pContentType, void ** out_ppTheBodyPart);
        static int 	(FAR* pAdrMessagePartFreeAll)(mime_messagePart_t * in_pMessagePart);
        static int	(FAR* pAdrMultiPartFreeAll)(mime_multiPart_t * in_pMultiPart);
        static int	(FAR* pAdrMultiPartGetPart)(mime_multiPart_t * in_pMultiPart, int in_index, mime_content_type * in_pContentType, void ** out_ppTheBodyPart);
        static int	(FAR* pAdrMultiPartGetPartCount)(mime_multiPart_t * in_pMultiPart, int * count);
        static int  (FAR* pAdrMessagePartGetMessage)(mime_messagePart_t * in_pMessagePart, mime_message_t ** out_ppMessage);
		static int	(FAR* pAdrDecodeBase64)(nsmail_inputstream_t * in_pInput_stream, nsmail_outputstream_t * in_pOutput_stream);
        static int	(FAR* pAdrDecodeHeaderString)(char * inString, char ** out_ppString);
        static int	(FAR* pAdrDecodeQP)(nsmail_inputstream_t * in_pInput_stream, nsmail_outputstream_t * in_pOutput_stream);

        // Mime dynamic parsing        static int 	(FAR* pAdrDataSinkNew)( mimeDataSink_t **pp );
        static void (FAR* pAdrDataSinkFree)( mimeDataSink_t **pp );
        static int 	(FAR* pAdrDynamicParserNew)( mimeDataSink_t *pDataSink, struct mimeParser **pp);
        static void (FAR* pAdrDynamicParserFree)( struct mimeParser **pp );
        static int 	(FAR* pAdrBeginDynamicParse)( struct mimeParser *p );
        static int 	(FAR* pAdrDynamicParse)( struct mimeParser *p, char *pData, int nLen );
        static int 	(FAR* pAdrEndDynamicParse)( struct mimeParser *p );


        // Déclaration des pointeurs de Fonctions Comm
        static int	(FAR* pAdrInputStreamCreate)(char * in_fileName, nsmail_inputstream_t ** out_ppRetInputStream);
        static int	(FAR* pAdrBufInputStreamCreate)(char * in_pDataBuf, long in_data_size, nsmail_inputstream_t ** out_ppRetInputStream);
        static int	(FAR* pAdrOutputStreamCreate)(char * in_fileName, nsmail_outputstream_t ** out_ppRetOutputStream);
        static void (FAR* pAdrStreamFree)(void * pMem);

        // Methodes de réponse aux évenements du Sink
        static void NSPOP3_connect( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
		static void NSPOP3_delete( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
		static void NSPOP3_error( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
		static void NSPOP3_listStart( pop3Sink_t * in_pPOP3Sink );
		static void NSPOP3_list( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, int in_octetCount );
		static void NSPOP3_listComplete( pop3Sink_t * in_pPOP3Sink );
		static void NSPOP3_noop( pop3Sink_t * in_pPOP3Sink );
		static void NSPOP3_pass(pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
		static void NSPOP3_quit(pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
		static void NSPOP3_reset(pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
		static void NSPOP3_retrieveStart( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, int in_octetCount );
		static void NSPOP3_retrieve( pop3Sink_t * in_pPOP3Sink, const char * in_messageChunk );
		static void NSPOP3_retrieveComplete( pop3Sink_t * in_pPOP3Sink );
		static void NSPOP3_sendCommandStart( pop3Sink_t * in_pPOP3Sink );
		static void NSPOP3_sendCommand( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
		static void NSPOP3_sendCommandComplete( pop3Sink_t * in_pPOP3Sink );
		static void NSPOP3_stat( pop3Sink_t * in_pPOP3Sink, int in_messageCount, int in_octetCount );
		static void NSPOP3_topStart( pop3Sink_t * in_pPOP3Sink, int in_messageNumber );
		static void NSPOP3_top( pop3Sink_t * in_pPOP3Sink, const char * in_responseLine );
		static void NSPOP3_topComplete( pop3Sink_t * in_pPOP3Sink );
		static void NSPOP3_uidListStart( pop3Sink_t * in_pPOP3Sink );
		static void NSPOP3_uidList( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, const char * in_uid );
		static void NSPOP3_uidListComplete( pop3Sink_t * in_pPOP3Sink );
		static void NSPOP3_user( pop3Sink_t * in_pPOP3Sink, const char * in_responseMessage );
		static void NSPOP3_xAuthListStart( pop3Sink_t * in_pPOP3Sink );
		static void NSPOP3_xAuthList( pop3Sink_t * in_pPOP3Sink, int in_messageNumber, const char * in_responseMessage );
		static void NSPOP3_xAuthListComplete( pop3Sink_t * in_pPOP3Sink );
		static void NSPOP3_xSender( pop3Sink_t * in_pPOP3Sink, const char * in_emailAddress );

        // Methodes de réponse aux évenements du DataSink
        static void DataSink_header (mimeDataSinkPtr_t pSink,void *pCallbackObject, char *name, char *value);
		static void DataSink_addHeader (mimeDataSinkPtr_t pSink,void *pCallbackObject, char *name, char *value);
        static void DataSink_endMessageHeader (mimeDataSinkPtr_t pSink, void *pCallbackObject);
		static void DataSink_contentType (mimeDataSinkPtr_t pSink, void *pCallbackObject, int nContentType);
		static void DataSink_contentSubType (mimeDataSinkPtr_t pSink, void *pCallbackObject, char * contentSubType);
		static void DataSink_contentTypeParams (mimeDataSinkPtr_t pSink, void *pCallbackObject, char * contentTypeParams);
		static void DataSink_contentID (mimeDataSinkPtr_t pSink, void *pCallbackObject, char *contentID);
		static void DataSink_contentMD5 (mimeDataSinkPtr_t pSink, void *pCallbackObject, char * contentMD5);
		static void DataSink_contentDisposition (mimeDataSinkPtr_t pSink, void *pCallbackObject, int nContentDisposition);
		static void DataSink_contentDispParams (mimeDataSinkPtr_t pSink, void *pCallbackObject, char * contentDispParams);
		static void DataSink_contentDescription (mimeDataSinkPtr_t pSink, void *pCallbackObject, char *contentDescription);
		static void DataSink_contentEncoding (mimeDataSinkPtr_t pSink, void *pCallbackObject, int nContentEncoding);
        static void *DataSink_startMessage (mimeDataSinkPtr_t pSink);
		static void DataSink_endMessage (mimeDataSinkPtr_t pSink, void *pCallbackObject);
        static void *DataSink_startBasicPart (mimeDataSinkPtr_t pSink);
		static void DataSink_bodyData (mimeDataSinkPtr_t pSink, void *pCallbackObject, char bodyData[], int len);
		static void DataSink_endBasicPart (mimeDataSinkPtr_t pSink, void *pCallbackObject);
        static void *DataSink_startMultiPart (mimeDataSinkPtr_t pSink);
		static void DataSink_boundary (mimeDataSinkPtr_t pSink, void *pCallbackObject, char * boundary);
		static void DataSink_endMultiPart (mimeDataSinkPtr_t pSink, void *pCallbackObject);
        static void *DataSink_startMessagePart (mimeDataSinkPtr_t pSink);
		static void DataSink_endMessagePart (mimeDataSinkPtr_t pSink, void *pCallbackObject);
};

//---------------------------------------------------------------------------
class TMailBox : public TForm
{
__published:	// Composants gérés par l'EDI
	TPanel *Panel;
	TStatusBar *StatusBar;
	TButton *ConsultButton;
	TListView *ListMsg;
	TPanel *PanelMsg;
	TLabel *LabelFrom;
	TLabel *LabelDate;
	TLabel *LabelObj;
	TRichEdit *EditMsg;
	TButton *DeconnectButton;
	TButton *ConnectButton;
	TLabel *LabelMsg;
	TButton *StockButton;
	void __fastcall ConsultButtonClick(TObject *Sender);
	void __fastcall DeconnectButtonClick(TObject *Sender);
	void __fastcall ConnectButtonClick(TObject *Sender);
	void __fastcall StockButtonClick(TObject *Sender);
	void __fastcall ListMsgClick(TObject *Sender);
private:	// Déclarations de l'utilisateur
public:		// Déclarations de l'utilisateur
	NSPop3* 		pPop3;
	bool 			bConnect;
	__fastcall 		TMailBox(TComponent* Owner, string Serveur, int Port, string User, string Pass, NSContexte* pCtx);
    __fastcall 		~TMailBox();
    void __fastcall SablierOn();
    void __fastcall SablierOff();
};
//---------------------------------------------------------------------------
extern PACKAGE TMailBox *MailBox;
//---------------------------------------------------------------------------
#endif
