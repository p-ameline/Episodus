//---------------------------------------------------------------------------
//     NSFRAMEDOC.H
//
//  Frames Index Documents
//
//  1ere version : PA nov 2011
//---------------------------------------------------------------------------
#ifndef __NSFRAMEDOCS_H
#define __NSFRAMEDOCS_H

#if defined(_DANSNSDNDLL)
	#define _CLASSENSDN __export
#else
	#define _CLASSENSDN __import
#endif

class NSDocumentsTank ;

#include "nsdn\nsdochis.h"
#include "nsldv\nsLdv_Vars.h"

class _CLASSENSDN NSFrameDocuments : public NSRoot
{
  public:

    NSFrameDocuments(NSDocumentsTank* pDocumentsTank, NSContexte *pCtx, LDVFRAME iFrame, NSDocumentHisto *pDocHis) ;
	  NSFrameDocuments(const NSFrameDocuments& rv) ;
	  ~NSFrameDocuments() ;

	  NSFrameDocuments& operator=(const NSFrameDocuments& src) ;

    LDVFRAME         getFrame()           { return _iFrame ; }

    NSDocumentHisto* getIndexDocument()   { return _pIndexDoc ; }

    // NSPatPathoArray* getPatPathoIndex()   { return _pIndexDoc ? _pIndexDoc->getPatPatho() : NULL ; }
    void             setPatPathoIndex(const NSPatPathoArray* pPpt)      { if (_pIndexDoc) _pIndexDoc->setPatPatho(pPpt) ; }
    void             initFromPatPathoIndex(NSPatPathoArray* pPpt) const { if (_pIndexDoc) _pIndexDoc->initFromPatPatho(pPpt) ; }

    NSDocumentHistoArray* getConcernDocs()     { return &_aConcernDocs ; }
    NSDocumentHistoArray* getTreatmentDocs()   { return &_aTreatmentDocs ; }
    NSDocumentHistoArray* getGoalDocs()        { return &_aGoalDocs ; }

    NSDocumentHisto* findNode(string sNodeId) ;

    void setIndexDocument(NSDocumentHisto* pDoc) { _pIndexDoc = pDoc ; }
    void addDocumentInTank(NSDocumentHisto* pDoc, NSDocumentHistoArray *pTank) ;
    void addConcernDocument(NSDocumentHisto* pDoc)   { addDocumentInTank(pDoc, &_aConcernDocs) ; }
    void addTreatmentDocument(NSDocumentHisto* pDoc) { addDocumentInTank(pDoc, &_aTreatmentDocs) ; }
    void addGoalsDocument(NSDocumentHisto* pDoc)     { addDocumentInTank(pDoc, &_aGoalDocs) ; }

    void setDocumentsTank(NSDocumentsTank *pDocsTank) { _pDocumentsTank = pDocsTank ; }

  protected:

    NSDocumentsTank *_pDocumentsTank ;

    LDVFRAME         _iFrame ;

    NSDocumentHisto *_pIndexDoc ;

    NSDocumentHistoArray _aConcernDocs ;
    NSDocumentHistoArray _aTreatmentDocs ;
    NSDocumentHistoArray _aGoalDocs ;

    void loadSubTrees() ;
    void loadSubTreesForDataType(string sDocRoot, NSDocumentHistoArray *pTank, NSRootLink::NODELINKTYPES iRelation) ;

    NSDocumentHisto* findNodeInArray(string sNodeId, NSDocumentHistoArray* pDocsArray) ;
} ;

typedef vector<NSFrameDocuments*>	           NSVectFrameDocuments ;
typedef NSVectFrameDocuments::iterator       FrameDocsIter ;
typedef NSVectFrameDocuments::const_iterator FrameDocsConstIter ;

class _CLASSENSDN NSFrameDocumentsArray : public NSVectFrameDocuments
{
  public:

    // Constructeurs
    NSFrameDocumentsArray(NSDocumentsTank* pDocumentsTank) ;
    NSFrameDocumentsArray(const NSFrameDocumentsArray& rv) ;

    // Destructeur
    virtual ~NSFrameDocumentsArray() ;
    void vider() ;

    void              openFrameInformation(LDVFRAME iFrame) ;

    NSFrameDocuments* getFrameDocuments(LDVFRAME iFrame) ;

    // Opérateurs
    NSFrameDocumentsArray& operator=(const NSFrameDocumentsArray& src) ;

 protected:

    NSDocumentsTank *_pDocumentsTank ;
} ;

//---------------------------------------------------------------------------
#endif    // __NSFRAMEDOCS_H
