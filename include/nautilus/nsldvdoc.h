//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSLDVDOC_H
# define __NSLDVDOC_H

# if defined(_DANSDLL)
#  define _CRDOCCLASSE __import
# endif

class NSLdvDocument ;
class NSLdvGoal ;
class NSLdvDrug ;
class NSConcern ;
class ArrayGoals ;
class ArrayLdvDrugs ;
class NSLdvPubli ;
class NSFrameInformation ;
class NewConcernInfo ;

#ifndef NSDocumentHisto
  class NSDocumentHisto ;
#endif

#ifndef __NSBDMDRIVER_H
  class NsSelectableDrug ;
#endif

# include "nsldv/nsldv_vars.h"
# include "ns_sgbd/nsannexe.h"
# include "dcodeur/nstxstyl.h"
# include "nautilus/nsdocref.h"
# include "nautilus/nsrechd2.h"
# include "dcodeur/nscr_ama.h"
# include "nsldv/nsldvgoalinfo.h"
# include "nsldv/nsldvuti.h"
# include "nsepisod/eptables.h"
# include "nsepisod/nsCoopChart.h"
# include "ns_ob1/BB1types.h"
# include "ns_ob1/OB1Node.h"
# include "nsbb/nsattval.h"
# include "nsbb/nspatpat.h"
# include "nssavoir/nsvarray.h"
# include "nsdn/nsframedocs.h"
# include "nsldv/nsldvdoc_base.h"

class NSLdvView ;
class NSDrugView ;
class NSGoalView ;
class NSProcessView ;

class NSLdvDocument : public NSLdvDocumentBase
{
  protected:

    // For printing purposes
    NSPrintControl  _printControl ;

    static long lObjectCount ;

  public:

    NSPublication   *_pPubli ;

    NSLdvDocument(TDocument *parent, NSContexte *pCtx = 0) ;
    NSLdvDocument(const NSLdvDocument& rv) ;
    ~NSLdvDocument() ;

    NSLdvDocument& operator=(const NSLdvDocument& src) ;

    //
    //
    void             Publish(NSLdvPubli *pPubliDriver, bool bCorrespPatient = true) ;
    bool             GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation, NSLdvPubli *pPubliDriver, string sAdresseCorresp = "", string sPathDest = "") ;

    NSDocumentHisto* getStorageDocument(LDVFRAME iFrame, NSRootLink::NODELINKTYPES iRelation) ;
    NSDocumentHisto* getConcernsStorageDocument(LDVFRAME iFrame) { return getStorageDocument(iFrame, NSRootLink::indexConcerns) ; }
    NSDocumentHisto* getDrugsStorageDocument(LDVFRAME iFrame)    { return getStorageDocument(iFrame, NSRootLink::drugOf) ; }
    NSDocumentHisto* getGoalsStorageDocument(LDVFRAME iFrame)    { return getStorageDocument(iFrame, NSRootLink::indexGoals) ; }

    bool             getStorageDocumentsIds(VecteurString *pIdsVector, LDVFRAME iFrame, NSRootLink::NODELINKTYPES iRelation) ;

    NSDocumentHisto* newStorageDocument(NSDocumentHisto* pRootDoc, NSRootLink::NODELINKTYPES iRelation) ;

    bool            newLine(LDVFRAME iFrame, string& sNode, NewConcernInfo *pConcernInfo) ;

    // Social frame
    //
    PatPathoIter    getSocialElement(string sNodeId, NSPatPathoArray **pPatho) ;

    bool            connectObjectToConcern(LDVFRAME iFrame, string sCodeConcern, int iSeverity, string sNoeud, NSRootLink::NODELINKTYPES iRelation, bool bAutoCreate) ;
    void            showNewDrug(const NSPatPathoArray *pPatPathoArray, PatPathoIter iter) ;

    void            invalidateViews(string sReason, string sReference = string("")) ;
    void            viewsReloadDrugToons() ;

    NSLdvView       *getLdvView(string sConcern = string("")) ;
    NSDrugView      *getDrugView(string sConcern) ;
    NSGoalView      *getGoalView(string sConcern) ;
    NSProcessView   *getProcessView(string sConcern) ;

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

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

#endif // __NSLDVDOC_H

