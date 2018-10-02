//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSHISTODO_H
#define __NSHISTODO_H

class NSHtml ;
class NSTreeNode ;
class NsHistorique ;
class NSCSDocument ;

#include <owl\docview.h>

// #include "nautilus\nsannexe.h"
// #include "nautilus\nsrechd2.h"
# include "nsbb\nspatpat.h"
// # include "nautilus\nsanxary.h"
# include "nsdn\nsdochis.h"

#define WM_RAFRAICHIR  WM_USER+2

bool inferieur(NSDocumentHisto* s, NSDocumentHisto* b);

class NSHISTODocument : public TDocument, public NSRoot
{
  private:

 	  static long lObjectCount ;

  protected:

    NSDocumentsTank _DocumentsTank ;
    NSPatInfo*      _pPerson ;

  public:

	 	string 	_sTitre ;    // titre
    NSHtml*	_pHtmlCS ;	  // html représentant la TreeView
	 	NSHISTODocument(TDocument* parent = (TDocument*) 0, NSContexte* pCtx = (NSContexte*) 0, NSPatInfo* pPat = (NSPatInfo*) 0) ;	 	~NSHISTODocument() ;
    NSHISTODocument(const NSHISTODocument& rv) ;
	 	NSHISTODocument& operator=(const NSHISTODocument& src) ;

	 	// implementation des méthodes virtuelles de TDocument	 	bool  Open(int mode, const char far* path=0) ;
	 	bool  Close() ;
	 	bool  IsOpen() { return true ; }
	 	bool	CanClose() ;
	 	void	SetTitle(LPCSTR title);
    void  SetDocName(char *nom);

    bool donneArbre(const string sIdentifiant, NSPatPathoArray* pNSPPt, string &sDateExam) { return _DocumentsTank.donneArbre(sIdentifiant, pNSPPt, sDateExam) ; }
    bool IsAttachedToOpenedPatient() ;

    // Fonctions de gestion de VectDocumentOuvert
    void	           RangeDocumentOuvert(NSDocumentInfo* pDocumentInfo, NSNoyauDocument* pDoc) { _DocumentsTank.RangeDocumentOuvert(pDocumentInfo, pDoc) ; }    void             FermeDocumentOuvert(NSDocumentInfo* pDocumentInfo)                        { _DocumentsTank.FermeDocumentOuvert(pDocumentInfo) ; }
    NSNoyauDocument* EstUnDocumentOuvert(NSDocumentInfo* pDocumentInfo)                        { return _DocumentsTank.EstUnDocumentOuvert(pDocumentInfo) ; }
    NSNoyauDocument* EstUnDocumentOuvert(NSTreeNode* pNode)                                    { return _DocumentsTank.EstUnDocumentOuvert(pNode) ; }
    NSCSDocument*    GetOpenDocAsCs(NSDocumentInfo* pDocumentInfo) ;    bool             ChargeDocInfo(NSDocumentInfo* pDocumentInfo);    // Fonctions de gestion de l'historique    void 	Rafraichir(NSDocumentInfo* pNSDocumentInfo, const NSPatPathoArray* pNSPatPathoArray,
                            NSNoyauDocument* pNouveauDocument = (NSNoyauDocument*) 0,
                            bool bWarnBBk = true);
    void	 AjouteDocument(NSDocumentInfo* pNSDocumentInfo);
    void   AjouteDocument();	// global : ajoute tous les documents visibles
    void	 AjouteALdV(NSDocumentInfo* pNSDocumentInfo);
    void 	 AutoriserOuverture(NSDocumentInfo* pDocument);
    void 	 AutoriserEdition(NSDocumentInfo* pDocument);
    void   FermetureDocument(NSDocumentInfo* pDocumentInfo);
    TView* ActiveFenetre(NSDocumentInfo* pDocumentInfo, string sViewName = "");
    void 	 ActiveFenetre(string sTitre);
    static BOOL FAR PASCAL _export EnumereVues(HWND hWnd, LPARAM lParam);
    void   DonnePathoSynthese(NSPatPathoArray* pPathoSynthese);
    void   AjouteDocumentParChemise();
		void 	 DocumentsDuneChemise(NSChemiseInfo* pChemise) ;

    // NSPatPathoArray* DonnePatPathoDocument(string sCodeDocum) { return _DocumentsTank.DonnePatPathoDocument(sCodeDocum) ; }
    void             setPatPathoDocument(const string sCodeDocum, const NSPatPathoArray* pPpt)      { _DocumentsTank.setPatPathoDocument(sCodeDocum, pPpt) ; }
    void             initFromPatPathoDocument(const string sCodeDocum, NSPatPathoArray* pPpt) const { _DocumentsTank.initFromPatPathoDocument(sCodeDocum, pPpt) ; }

    DocumentIter     DonnePatPathoDocument(string sLexique, NSPatPathoArray* pPatPatho, DocumentIter iter = 0) { return _DocumentsTank.DonnePatPathoDocument(sLexique, pPatPatho, iter) ; }
    DocumentIter     DonnePrevPatPathoDocument(string sLexique, NSPatPathoArray* pPatPatho, DocumentIter iter) { return _DocumentsTank.DonnePrevPatPathoDocument(sLexique, pPatPatho, iter) ; }
    DocumentIter     DonneNextPatPathoDocument(string sLexique, NSPatPathoArray* pPatPatho, DocumentIter iter) { return _DocumentsTank.DonneNextPatPathoDocument(sLexique, pPatPatho, iter) ; }

    DocumentIter     TrouveDocHisto(const string sCodeDocum) { return _DocumentsTank.TrouveDocHisto(sCodeDocum) ; }

    NSDocumentHisto* getLibChem()            { return _DocumentsTank.getLibChem() ; }
	 	NSDocHistoArray* getVectDocument()       { return _DocumentsTank.getVectDocument() ; }
    NSDocHistoArray* getVectDocumentOuvert() { return _DocumentsTank.getVectDocumentOuvert() ; }    NSChemiseArray*  getVectChemise()        { return _DocumentsTank.getVectChemise() ; }

    NSDocumentsTank* getDocumentTank()       { return &_DocumentsTank ; }

    NsHistorique*    getHistorique() ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};#endif // __NSHISTODO_H
