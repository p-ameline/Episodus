#ifndef __NSDOCHIS_H#define __NSDOCHIS_H

class NSNoyauDocument ;
class NSSearchStruct ;

#if defined(_DANSNSDNDLL)
	#define _CLASSENSDN __export
#else
	#define _CLASSENSDN __import
#endif

#include "partage\ns_vector.h"
#include "nsdn\nsdocum.h"
#include "nsdn\nsanxary.h"

// #include "nsdn\nsdocnoy.h"
// #include "nsbb\nspatpat.h"

//document + sa patpatho

class _CLASSENSDN NSDocumentHisto : public NSDocumentInfo{
  private:

 	  static long lObjectCount ;

  protected:

    // pointeur pDocNoy de stockage des documents (ne pas faire de new)
		NSNoyauDocument* _pDocNoy ;
    
		NSPatPathoArray  _PatPathoArray ;
		string           _sDateDoc ;

	public:

		//constructeurs  Destructeur		NSDocumentHisto(NSContexte* pContexte) ;
		NSDocumentHisto(NSDocumentInfo* pNSDocument, bool bNoDocnoy /* fictif : pour distinguer les ctors */) ;
		NSDocumentHisto(NSDocumentInfo* pNSDocumentInfo, NSNoyauDocument* pDoc = (NSNoyauDocument*) 0) ;
		~NSDocumentHisto() ;

		//copie		NSDocumentHisto(const NSDocumentHisto& rv) ;
    NSDocumentHisto& operator=(const NSDocumentHisto& src) ;

		// Surcharges des opérateurs    int operator==(const NSDocumentHisto& o) ;

    void 	SetDateDoc() ;
    void 	ResetDateDoc() ;

    bool  checkPptConsistency() ;
    bool  checkSynthesisConsistency() ;

    void  initDocumentInfo(NSDocumentInfo *pNewDoc) ;

    NSNoyauDocument* getDocNoy()   { return _pDocNoy ; }

		NSPatPathoArray* getPatPathoPointer() { return &_PatPathoArray ; }
    void             setPatPatho(const NSPatPathoArray* pPpt) ;
    void             initFromPatPatho(NSPatPathoArray* pPpt) const ;
    NSPatPathoInfo*  getRootPathoInfo() const ;

    bool             isValidRegularNode(PatPathoConstIter pptIter, NSSearchStruct *pSearchStruct, string sDateDoc, string &sRefDate) ;
    bool             SearchInDocument(string sChemin, NSSearchStruct *pSearchStruct, string sDateDoc = string("")) ;

    bool             CheminDansPatpatho(string sChaine, string separateur = string(1, cheminSeparationMARK), PatPathoIter* pIter = (PatPathoIter*) 0, PatPathoConstIter* startIter = (PatPathoConstIter*) 0, PatPathoConstIter* subTreeIter = (PatPathoConstIter*) 0) ;

    string           getDateDoc() ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

//// Définition de NSDocumenHistotArray (Array de NSDocumentHisto)
//

typedef vector<NSDocumentHisto*> NSDocumentHistoArray ;typedef NSDocumentHistoArray::iterator         DocumentIter ;
typedef NSDocumentHistoArray::const_iterator   DocumentConstIter ;
//Reverse_iter on  NSDocumentHistoArraytypedef NSDocumentHistoArray::reverse_iterator DocumentReverseIter ;
class _CLASSENSDN NSDocHistoArray : public NSDocumentHistoArray{
  private:

 	  static long lObjectCount ;

  public :

	  // Constructeurs	  NSDocHistoArray() : NSDocumentHistoArray() { lObjectCount++ ; }
	  NSDocHistoArray(const NSDocHistoArray& rv) ;

	  // Destructeur	  virtual ~NSDocHistoArray() ;
    void vider() ;

	  DocumentIter TrouveDocHisto(const string codeDocBrut) ;    static long getNbInstance()  { return lObjectCount ; }    static void initNbInstance() { lObjectCount = 0 ; }};

class NSTreeNode ;

class _CLASSENSDN NSDocumentsTank : public NSRoot
{
  private:

 	  static long lObjectCount ;

  protected :

    NSDocumentHisto* _pLibChem ;
	 	NSDocHistoArray  _VectDocument ;       //vecteur des documents
    NSDocHistoArray	 _VectDocumentOuvert ; // vecteur des documents ouverts    NSChemiseArray   _VectChemise ;        //vecteur des chemises

  public :

	  // Constructeurs	  NSDocumentsTank(NSContexte* pCtx) ;
	  NSDocumentsTank(const NSDocumentsTank& rv) ;

	  // Destructeur	  ~NSDocumentsTank() ;

    bool             donneArbre(const string sIdentifiant, NSPatPathoArray* ppNSPPt, string &DateExam) const ;

    // Fonctions de gestion de VectDocumentOuvert

    void	           RangeDocumentOuvert(NSDocumentInfo* pDocumentInfo, NSNoyauDocument* pDoc) ;    void             FermeDocumentOuvert(NSDocumentInfo* pDocumentInfo) ;
    NSNoyauDocument* EstUnDocumentOuvert(NSDocumentInfo* pDocumentInfo) ;
    NSNoyauDocument* EstUnDocumentOuvert(NSTreeNode* pNode) ;

    DocumentIter     TrouveDocHisto(const string sCodeDocum) { return _VectDocument.TrouveDocHisto(sCodeDocum) ; }

    // NSPatPathoArray* DonnePatPathoDocument(string sCodeDocum) ;
    void             setPatPathoDocument(string sCodeDocum, const NSPatPathoArray* pPpt) ;
    void             initFromPatPathoDocument(string sCodeDocum, NSPatPathoArray* pPpt) const ;

    DocumentIter     DonnePatPathoDocument(string sLexique, NSPatPathoArray* pPatPatho, DocumentIter iter = 0) ;
    DocumentIter     DonnePrevPatPathoDocument(string sLexique, NSPatPathoArray* pPatPatho, DocumentIter iter) ;
    DocumentIter     DonneNextPatPathoDocument(string sLexique, NSPatPathoArray* pPatPatho, DocumentIter iter) ;

    NSDocumentsTank& operator=(const NSDocumentsTank &src) ;

    NSDocumentHisto* getLibChem()                     { return _pLibChem ; }
    void             setLibChem(NSDocumentHisto* pDH) { _pLibChem = pDH ; }
	 	NSDocHistoArray* getVectDocument()                { return &_VectDocument ; }
    NSDocHistoArray* getVectDocumentOuvert()          { return &_VectDocumentOuvert ; }    NSChemiseArray*  getVectChemise()                 { return &_VectChemise ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

#endif
