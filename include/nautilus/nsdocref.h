//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSDOCREF_H
#define __NSDOCREF_H

// class NSDocumentArray ;
class NSPublication ;
class NSModuleArray ;
class NSSmallBrother ;

#include <owl\docmanag.h>

// #include "nsbb\nsbbsmal.h"
#include "nsdn\nsdocnoy.h"
// #include "nautilus\nsrechd2.h"
// #include "nssavoir\nsgraphe.h"
// #include "nautilus\nsbrowse.h"
#include "nsdn\nsanxary.h"
#include "nautilus\nsmodhtml.h"

class NSPrintControl
{
  public:

    NSPrintControl() ;

    // Getters

    string getNomDocHtml()     { return _sNomDocHtml ; }
    int    getNbImpress()      { return _nbImpress ; }
    bool   getImprimerLettre() { return _bImprimerLettre ; }
    string getCodeLettre()     { return _sCodeLettre ; }
    int    getIndexCorresp()   { return _indexCorresp ; }

    // Setters

    void setNomDocHtml(string sNDH)  { _sNomDocHtml = sNDH ; }
    void setNbImpress(int iNb)       { _nbImpress = iNb ; }
    void setImprimerLettre(bool bIL) { _bImprimerLettre = bIL ; }
    void setCodeLettre(string sCod)  { _sCodeLettre = sCod ; }
    void setIndexCorresp(int iIC)    { _indexCorresp = iIC ; }

  protected:

    string _sNomDocHtml ;

    int    _nbImpress ;
    bool   _bImprimerLettre ;
    string _sCodeLettre ;    int    _indexCorresp ;
} ;

//
// Classe de base des documents Nautilus
//
class NSRefDocument : public NSNoyauDocument
{
  private:

 	  static long lObjectCount ;

	public:

		// Objet d'accès/modification des patPatho
		NSSmallBrother* _pBigBoss ;

		// uniquement pour l'importation		string		      _sCodeChemise ;

		// pour la publication par visual		NSPrintControl  _printControl ;

		// pour la composition et la publication		string          _sUrlHtml ;
		NSPublication*	_pPubli ;

		// constructeurs		NSRefDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,      						NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx, bool bROnly) ;
		NSRefDocument(TDocument* parent = 0, NSContexte* pCtx = 0) ;

		virtual ~NSRefDocument() ;		// Constructeur copie
		NSRefDocument(const NSRefDocument& rv) ;
		// Operateur =
		NSRefDocument& operator=(const NSRefDocument& src) ;

		// Fonctions pour générer un fichier Html à partir du document
		virtual bool GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
      								string sAdresseCorresp = "", string sPathDest = "") ;
    virtual bool GenereHtmlText(string& sRawText) ;
    virtual bool GenereRawText(string& sRawText) ;

		// Méthodes communes de composition, publication et visualisation
		void Composer() ;
		void Publier(bool bCorrespPatient = true) ;
		void Visualiser() ;
		bool exportFile(string sFileName, string sFileType = "") ;

		// dialogue d'enregistrement du document dans la base
		bool CreeDocument(NSDocumentInfo* pDoc, int typeLink, string &sCodeChemise, bool bVerbose = true,
                            NSPersonGraphManager* pGraphManager = 0) ;

		// dialogue de modification des paramètres d'un document
		bool ParamDoc() ;

		// Referencement d'un document dans la base des documents
		bool Referencer(string typeDoc, string nomDoc = string(""), string nomFichier = string(""),
      					    string cheminDoc = string(""), bool bDocVisible = true,
                    bool bVerbose = true, string sCodeDoc = string(""),
                    NSRootLink::NODELINKTYPES typeLink = NSRootLink::personDocument,
                    NSPersonGraphManager* pGraphManager = 0, string sAuthorId = string("_User_"),
                    string tmplDoc = string(""), string enteteDoc = string(""),
                    string sDestinataire = string(""), string sCreationDate = string(""),
                    string sMasterDoc = string(""),
                    NSRootLink::NODELINKTYPES masterLink = NSRootLink::viewOfDocument,
                    NSPatPathoArray* pSpecificInfoPatPatho = (NSPatPathoArray*) 0) ;

		// Referencement des fichiers Html dynamiques
		bool ReferencerHtml(string typeDoc, string nomDoc, string tempDoc, string enteteDoc, bool bIsVisible = true) ;

		// Importation des fichiers Html statiques
		string	texteCourant(string textHtml) ;
		bool 		chargePatPatho(string sVersion, string* sTab) ;
		bool 		ExtraitNomImage(string sLien, string& sNom, string& sExt) ;
		bool 		ReferencerDocument(NSDocumentInfo* pInfo) ;
		bool 		ReferencerDocumentPatPatho(NSRefDocument* pMasterDoc, NSPatPathoArray* pPatPatho, string sNomDoc = string(""), string sTypeDoc = string("")) ;
		void		ReferencerImages(NSDocumentArray* pImageArray) ;
		bool		GetModules(NSModuleArray* pModArray) ;
//#ifdef N_TIERS
//		bool  	PatEnCours(string sParam) ;
//#else
		bool 		PatEnCours(string sParam, string& sVer) ;
		bool 		ImporterHtml(string sFichierSource) ;
		bool 		ImporterFichier(string sTypeNautilus, string sDocTitle, string sFichierSource, string sPathSource, bool bVerbose = true, bool bDocVisible = true) ;
//#endif

		// Referencement des fichiers statiques
		bool 		TrouveNomFichier(string typeDoc, string sExt, string& sNomFichier, string& sLocalis) ;

		// verification compta automatique
		bool 		ComptaAuto() ;

		// Fonctions pour générer l'intitulé d'un fichier Html à partir du document
		virtual string  InitIntitule() ;
    virtual string  InitIntituleNoBirth() ;

    bool            EcrireComposants(NSPatPathoArray* pPatPatho) ;

		// version de EcrireComposants pour les html statiques importés
		bool 		EcrireComposants(string sCodeDocPatPatho, NSDocumentArray* pImageArray) ;

		// Pour récupérer la date de l'examen dans un CR
		string 	GetDateDoc(bool bDateClaire = true) ;

    // Get second operator in report
		string 	GetSecondOperator() ;

		bool  	makeSOAP() ;

    // Getters for print params

    NSPrintControl* getPrintControler() { return &_printControl ; }

    string getNomDocHtml()     { return _printControl.getNomDocHtml() ; }
    int    getNbImpress()      { return _printControl.getNbImpress() ; }
    bool   getImprimerLettre() { return _printControl.getImprimerLettre() ; }
    string getCodeLettre()     { return _printControl.getCodeLettre() ; }
    int    getIndexCorresp()   { return _printControl.getIndexCorresp() ; }

    // Setters for print params

    void setNomDocHtml(string sNDH)  { _printControl.setNomDocHtml(sNDH) ; }
    void setNbImpress(int iNb)       { _printControl.setNbImpress(iNb) ; }
    void setImprimerLettre(bool bIL) { _printControl.setImprimerLettre(bIL) ; }
    void setCodeLettre(string sCod)  { _printControl.setCodeLettre(sCod) ; }
    void setIndexCorresp(int iIC)    { _printControl.setIndexCorresp(iIC) ; }

    NSSmallBrother* getBigBoss()                    { return _pBigBoss ; }
    void            setBigBoss(NSSmallBrother* pBB) { _pBigBoss = pBB ; }

		string		      getCodeChemise()  { return _sCodeChemise ; }
		NSPrintControl  getPrintControl() { return _printControl ; }
		string          getUrlHtml()      { return _sUrlHtml ; }

		NSPublication*	getPubli()                      { return _pPubli ; }
    void	          setPubli(NSPublication* pP)     { _pPubli = pP ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

#endif // __NSDOCREF_H
