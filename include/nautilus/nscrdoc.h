//----------------------------------------------------------------------------//	Objet Document (au sens doc/vue) pour les comptes rendus
//----------------------------------------------------------------------------
#ifndef __NSCRDOC_H
#define __NSCRDOC_H

#if defined(_DANSDLL)	#define _CRDOCCLASSE __import
#endif

class NSDocumentHisto ;

#include "ns_sgbd\nsannexe.h"
#include "dcodeur\nstxstyl.h"
#include "nautilus\nsdocref.h"
#include "nautilus\nsrechd2.h"

#include "dcodeur\nscr_ama.h"//#include "nsbb\nsbbsmal.h"
#include "nsbb\nspatpat.h"

bool nouveauCompteRendu(NSContexte *pCtx) ;
bool afficheCompteRendu(string *pCRString, NSContexte *pCtx) ;
bool afficheCompteRendu(NSDocumentInfo& Document, NSContexte *pCtx) ;

//class _DOCVIEWCLASS NSCRDocument : public NSRefDocument
//class NSCRDocument : public TOleDocument

class  NSCRDocument : public NSRefDocument
{
  protected:

    bool _EstDecode ;

  public:
		string          _sFichDecod ;		 // instanci� par decodeCR		char            _contexteModule ;
		char            _lexiqueModule[MODU_LEXIQUE_LEN + 1] ;
		NSCRPhraseArray _aCRPhrases ;

		NSCRDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
                 NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx, string sTypeCR, bool bROnly) ;
		NSCRDocument(TDocument* parent = (TDocument*) 0, NSContexte* pCtx = (NSContexte*) 0, string sTypeCR = string("")) ;
    NSCRDocument(NSDocumentHisto* pDocumentModele, TDocument* parent = (TDocument*) 0, NSContexte* pCtx = (NSContexte*) 0) ;
		NSCRDocument(const NSCRDocument& rv) ;
		~NSCRDocument() ;
		NSCRDocument& operator=(const NSCRDocument& src) ;

		// NSPatPathoArray* getPatPatho() { return &_PatPathoArray ; }
    string           getTextFile() { return _sFichDecod ; }

		// G�n�ration du html correspondant au document
		bool GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
    							string sAdresseCorresp = string(""), string sPathDest = string("")) ;
    bool GenereHtmlText(string& sHtmlText) ;
    bool GenereRawText(string& sRawText) ;

		// D�codage du compte rendu (avec nom du fichier de destination)
		int	 decodeCR(NSCRPhraseArray* pPhrases = (NSCRPhraseArray*) 0) ;

		// M�thodes d'acc�s aux donn�es membres prot�g�es
    //
		bool IsDecode() { return _EstDecode ; }

		// Report management functions
    //
		bool enregistre() ;
    bool exportReport(NSDocStatus *pDocStatus) ;
    bool exportReportForDll(const string sDllName, NSDocStatus *pDocStatus) ;

		// TDocument's virtual functions
    //
		bool Open(int mode, const char far* path = 0) ;
		bool IsOpen() { return (false == _PatPathoArray.empty()) ; }
};

#endif // __NSCRDOC_H
