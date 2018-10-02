//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------

#ifndef __NSDOCNOY_H#define __NSDOCNOY_H

#include <owl\docmanag.h>

class NSPersonGraphManager ;
class NSLinkManager ;

#if defined(_DANSNSDNDLL)	#define _CLASSENSDN __export
#else
	#define _CLASSENSDN __import
#endif

#include "nsdn\nsdocum.h"#include "nssavoir\nsRootLink.h"#include "nssavoir\nsgraphe.h"#include "nsldv\nsLdv_Vars.h"// #include "nsbb\nspatpat.h"
// #include "nsbb\nspatbd.h"

#ifndef _ENTERPRISE_DLL
class _CLASSENSDN NSPrintParams{  public:    // modif RS du 09/04/04 : ajout des paramètres d'impression    // Page Setup dialog settings
	  string              sPaperSize;
	  string              sPaperSource;
	  string              sHeader;
	  string              sFooter;
	  string              sOrientation;
	  string              sfLeftMargin;
	  string              sfTopMargin;
	  string              sfRightMargin;
	  string              sfBottomMargin;

	  // Print dialog settings
	  string              sPrinterName;
	  string              sbPrintToFile;
	  string              sPrintRange;
	  string              slPrintRangePagesFrom;
	  string              slPrintRangePagesTo;
	  string              sbCollate;
	  string              sPrintFrames;
	  string              sbPrintLinks;
	  string              sbPrintLinkTable;    NSPrintParams();    NSPrintParams(const NSPrintParams& rv);
    ~NSPrintParams();

    NSPrintParams&  operator=(const NSPrintParams& src);
    // int 		    operator==(NSPrintParams& o);    void            reset();};#endif // #ifndef _ENTERPRISE_DLL
//
// Classe de base des NSDocuments qui n'utilisent pas les fonctions OLE
//
#ifndef _ENTERPRISE_DLL
class _CLASSENSDN NSNoyauDocument : public TDocument, public NSRoot
#else
class _CLASSENSDN NSNoyauDocument : public NSRoot
#endif
{
  private:

 	  static long lObjectCount ;

	public:

		enum {      		PrevProperty = TDocument::NextProperty-1,
         	FichierInfo,
         	NextProperty,
      	};

		// Infos du document et de sa composition		NSDocumentInfo* 	_pDocInfo ;
		NSDocumentInfo* 	_pHtmlInfo ;

		// nom de fichier html associé au document		// pour le cas de documents de type fichier html que l'on veut publier, etc...
		// modif RS du 13/04/04 : on ajoute les paramètres d'impression du fichier

		string            _sNomFichierHtml ;#ifndef _ENTERPRISE_DLL		NSPrintParams     _nspp ;#endif
		// donnees de composition		string			 	    _sTemplate ;
		string			 	    _sEnTete ;
		string			 	    _sBaseCompo ;  // base des images de la chemise
		string			 	    _sBaseImages ; // base des images rattachées au document
		string			 	    _sBaseTemp ;   // base des anciennes images en cas de recomposition

		// nécéssaire à l'exportation		bool				      _bConserveBase ;

		// code du méta-document et de l'arbre présentation		string            _sCodeDocMeta ;    string            _sCodeDocPres ;		bool              _bCreerMetaLien ;    bool              _bEnregPresentation ;#ifndef _ENTERPRISE_DLL		NSNoyauDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
      						  NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx,
                    bool bROnly, bool bClone = false) ;
		NSNoyauDocument(TDocument* parent = (TDocument*) 0,
                    NSContexte* pCtx = (NSContexte*) 0, bool bClone = false) ;
#else
		NSNoyauDocument(NSDocumentInfo* pDocumentInfo,
      						  NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx,
                    bool bROnly, bool bClone = false) ;
		NSNoyauDocument(NSContexte* pCtx = (NSContexte*) 0, bool bClone = false) ;
#endif

		virtual ~NSNoyauDocument() ;
		// Constructeur copie		NSNoyauDocument(const NSNoyauDocument& rv) ;

		// Operateur =		NSNoyauDocument& operator=(const NSNoyauDocument& src) ;

		// méthodes pour charger ou enregistrer la patpatho
		bool 	chargePatPatho() ;
		bool 	enregistrePatPatho() ;
    void 	updateAfterSave(string sTreeID, string sMetaID) ;

		// Récupère la date stockée dans la patpatho
		string	GetDateExamen(bool bDateClaire = true) ;

		// Mise à jour de la pDocInfo en fonction des données de la patpatho
		bool    DocInfoModified() ;

		// passe le champ visible à zero		bool 		DocumentInvisible (string sCodeDoc) ;

		// passe le champ visible à 1		bool		DocumentVisible(string sCodeDoc) ;

    // dialogue d'enregistrement du document dans la base
		bool CreeDocument(NSDocumentInfo* pDoc, int typeLink, string &sCodeChemise, bool bVerbose = true,
                            NSPersonGraphManager* pGraphManager = 0, bool bMustConnectToFolder = true) ;

    // Referencement d'un document dans la base des documents
		bool Referencer(string typeDoc, string nomDoc = string(""), string nomFichier = string(""),
                    string cheminDoc = string(""), bool bDocVisible = true,
                    bool bVerbose = true, string sCodeDoc = string(""),
                    NSRootLink::NODELINKTYPES typeLink = NSRootLink::personDocument,
                    NSPersonGraphManager* pGraphManager = 0, string sAuthorId = string("_User_"),
                    string tmplDoc = string(""), string enteteDoc = string(""),
                    string sDestinataire = string(""), string sCreationDate = string(""),
                    string sMasterDoc = string(""), NSRootLink::NODELINKTYPES masterLink = NSRootLink::viewOfDocument,
                    bool bMustConnectToFolder = true,
                    NSPatPathoArray* pSpecificInfoPatPatho = (NSPatPathoArray*) 0) ;

		// Referencement des fichiers Html dynamiques		bool 		ReferencerHtml(string typeDoc, string nomDoc, string tempDoc, string enteteDoc, bool bIsVisible = true) ;

		// Pour récupérer la template de composition et l'en-tete par défaut		bool    TemplateCompo(string typeDoc, string& tempDoc, string& enteteDoc, bool bVerbose = true) ;
		// Pour récupérer la template du document sans le path		string  TemplateInfoBrut(bool bCompo = true) ;		// Pour récupérer l'en-tete du document sans le path		string  EnTeteInfoBrut(bool bCompo = true) ;
		// Pour savoir si la chemise qui contient le document contient des images		bool    ChemiseAvecImages(bool& bResult) ;
		// cherche le futur code document en fonction du patient		bool    DonneNouveauCodeDocument(string sPatient, string& sCodeDoc) ;
		// Referencement des fichiers statiques		void 		IncrementeCode(string& code) ;
		string 	nomSansDoublons(string serveur, string unite, string pathRef, string nom, string ext) ;

		// Pour l'ouverture des documents à base de fichier		void    ValideFichier(string* psNomFichier = 0) ;

		bool 		MajTemplate() ;    bool    DetruireComposants(NSPatPathoArray* pPatPatho) ;    bool    EcrireComposants(NSPatPathoArray* pPatPatho) ;
		bool 		RetrouveHtmlInfo() ;

		// Pour fixer le titre du document		void 		SetTitreDoc() ;

		// Méthodes liées aux informations sur le patient		// int 		donne_age(char *datex) ;
    int 		donne_age(const string sDatex) ;
		void 		donne_intitule_patient(string *intitule, int age) ;

		// Méthodes d'accès aux données membres protégées		NSDocumentInfo*  DonneDocInfo() { return _pDocInfo ; }

    NSPatPathoArray* getPatPathoPointer() { return &_PatPathoArray ; }
    void             setPatPatho(const NSPatPathoArray* pPpt) ;
    void             initFromPatPatho(NSPatPathoArray* pPpt) const ;
    NSPatPathoInfo*  getRootPathoInfo() const ;

    bool             isValidDoc()         { return _bDocumentValide ; }
    void             setValidDoc(bool bV) { _bDocumentValide = bV ; }

    string           getCodeDocMeta() { return _sCodeDocMeta ; }
    string           getCodeDocPres() { return _sCodeDocPres ; }

    void             setCodeDocMeta(string sCode) { _sCodeDocMeta = sCode ; }
    void             setCodeDocPres(string sCode) { _sCodeDocPres = sCode ; }

		bool		isEnregEnCours() const { return _bEnregEnCours ; }
    bool		isReadOnly() 		 const { return _bReadOnly ; }
    void		setReadOnly(bool bRO)  { _bReadOnly = bRO ; }

		bool 		CanClose() ;
		// Redéfinition des fonctions de gestion des propriétés (virtual de TDocument)		int			FindProperty(const char far *name) ;		// return index
		int			PropertyFlags(int index) ;
		const char *PropertyName(int index) ;
		int 		PropertyCount()		{ return NextProperty-1 ; }
		int			GetProperty(int index, void far *dest, int textlen = 0) ;

    // Create basic documents
    //
    bool    createGraphRoot(NSPersonGraphManager* pGraphManager) ;
    bool    createFoldersLibrary(NSPersonGraphManager* pGraphManager) ;
    bool    createHealthTeam(NSPersonGraphManager* pGraphManager, string sPersonId) ;
    bool    createIdentifiersLibrary(NSPersonGraphManager* pGraphManager, string sIpp) ;
    bool    createFrame(NSPersonGraphManager* pGraphManager, LDVFRAME iFrame) ;
    bool    createFrameExtensionTree(NSPersonGraphManager* pGraphManager, string sIndexDocId, string sRootConcept, NSRootLink::NODELINKTYPES rootLink) ;
    bool    createFrameConcernsTree(NSPersonGraphManager* pGraphManager, string sIndexDocId) ;
    bool    createFrameTreatmentsTree(NSPersonGraphManager* pGraphManager, string sIndexDocId) ;
    bool    createFrameGoalsTree(NSPersonGraphManager* pGraphManager, string sIndexDocId) ;
    // bool    createLdvFrame(NSPersonGraphManager* pGraphManager) ;
    // bool    createRiskFrame(NSPersonGraphManager* pGraphManager) ;
    // bool    createSocialFrame(NSPersonGraphManager* pGraphManager) ;
    bool    createSynthesisFrame(NSPersonGraphManager* pGraphManager) ;
    bool    createIdentifiersFrame(NSPersonGraphManager* pGraphManager) ;

    bool    etablirLiensTree(string sNodeData, NSPersonGraphManager* pGraphManager) ;
    bool    connectToMasterDocument(string sMasterDocId, NSRootLink::NODELINKTYPES masterLink, NSPersonGraphManager* pGraphManager = 0) ;

    void    SetAccountingDirty(bool bDirty) { _bAccountingChanged = bDirty ; }
    bool    GetAccountingDirty()            { return _bAccountingChanged ; }

    string  getDocId()                      { return _pDocInfo ? _pDocInfo->getID() : string("") ; }

    bool    isClone()                       { return _bIsClone ; }
    void    setCloneStatus(bool bIsClone)   { _bIsClone = bIsClone ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

	protected :
    // PatPatho Array
		NSPatPathoArray 	_PatPathoArray ;
		bool              _bDocumentValide ;

		// NSPatPaDatArray* pDonneesArray ;		// NSPatPaLocArray* pLocalisArray ;
		NSArcNodeArray _ArcNodeArray ;

		bool		_bEnregEnCours ;   	// Signale que le document est en
                                // cours d'enregistrement
    bool		_bReadOnly ;
    bool    _bAccountingChanged ;

    bool    _bIsClone ;        // Clone of an open doc (don't close at delete)

    void    initDocumentInfo(NSDocumentInfo* pNewDocument, string sDocType, string sAuthorId, bool bDocVisible, string sDestinataire, string sCreationDate) ;
    void    createPresentationInfo(NSDocumentInfo* pNewDocument, NSPatPathoArray* ppPatPathoPresent, string sCodeDoc, NSRootLink::NODELINKTYPES typeLink, string typeDoc, string tmplDoc, string enteteDoc) ;
    void    createMetaPatpatho(NSDocumentInfo* pNewDocument, NSPatPathoArray* pPatPathoMeta, string sAuthorId, string typeDoc) ;

    void    setContent(NSDocumentInfo* pNewDocument, NSPatPathoArray* pPatPathoMeta, string typeDoc) ;
    void    initMetaContentFromPpt(NSDocumentInfo* pNewDocument) ;
    string  getMetaId(NSPersonGraphManager* pGraphManager, NSDocumentInfo* pNewDocument, string sCodeDoc) ;
} ;

#endif // __NSDOCNOY_H
