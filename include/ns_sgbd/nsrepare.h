#ifndef __NSREPARE_H#define __NSREPARE_H

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include <string.stl>

#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\edit.h>
#include <owl\gauge.h>

#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"

#include "ns_sgbd\NsBDEUti.h"

//
// Objet "Boite de dialogue" utilis� pour r�parer les tables
//
class NSRepareTable : public NSUtilDialog
{
	public:

    string 			  sTable ;
    string 			  sWarning ;

    OWL::TStatic*	pNomTable ;
    OWL::TStatic*	pWarning ;

    OWL::TButton*	pVerifie ;
    OWL::TButton* pRepare ;

    OWL::TGauge*  pEntetProgressBar ;
    OWL::TGauge*  pIndexProgressBar ;
    OWL::TGauge*  pDonneProgressBar ;
    OWL::TGauge*  pReparProgressBar ;

    OWL::TStatic* pVerifEvolution ;
    OWL::TStatic* pReparEvolution ;

    NSRepareTable(TWindow* pere, NSContexte* pCtx, string sTbl,
                                    string sWarn = "", OWL::TModule* module = 0) ;
    ~NSRepareTable() ;

    void SetupWindow() ;

    void lanceVerifie() ;
    void lanceRepare() ;

    void resetVerifyGauge() ;
    void resetRepareGauge() ;

    void fixePositionEntete(int iPos)  { pEntetProgressBar->SetValue(iPos) ; }
    void fixePositionIndex(int iPos)   { pIndexProgressBar->SetValue(iPos) ; }
    void fixePositionDonnees(int iPos) { pDonneProgressBar->SetValue(iPos) ; }
    void fixePositionRepare(int iPos)  { pReparProgressBar->SetValue(iPos) ; }

    void donneVerifyMessage(string sMessage) { pVerifEvolution->SetText(sMessage.c_str()) ; }
    void donneRepareMessage(string sMessage) { pReparEvolution->SetText(sMessage.c_str()) ; }

  DECLARE_RESPONSE_TABLE(NSRepareTable) ;
};

#ifdef __NSREPARE_CPP    TBDEUtil* pBDEUtil;
#else
    extern TBDEUtil* pBDEUtil;
#endif

#endif // __NSREPARE_H
