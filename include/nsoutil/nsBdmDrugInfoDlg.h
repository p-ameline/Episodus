// -----------------------------------------------------------------------------
// nsBdmDlg.h
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Ligne de vie - Drugs database control dialogs
// -----------------------------------------------------------------------------
// PA  - april 2018
// ...
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Programme protégé par une licence Open Source GPL
// -----------------------------------------------------------------------------

#ifndef __NSBDMDRUGINFODLG_H
#define __NSBDMDRUGINFODLG_H

#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\slider.h>
#include <owl\gauge.h>
#include <owl\listwind.h>

#include "partage\ns_vector.h"
#include "partage\NTArray.h"
#include "nsoutil/nsexport.h"
#include "nsbb\nsednum.h"
#include "partage\NTArray.h"
#include "nsbb\nslistwind.h"
#include "nssavoir\NsBdmDriver.h"
#include "nsbb\NTFunctor.h"

#include "nsoutil/nsBdmInfoDlg.h"

//
// Looking for a drug in the BAM
//
class _NSOUTILCLASSE NSBdmDrugInfoDlg : public NSGenericBdmInfoDlg
{
  public:

    // enum DRUGSEARCHBY { searchByUndefined = 0, searchBySubstance, searchByATC, searchByIndication } ;

    NSBdmDrugInfoDlg(TWindow* pView, NSContexte *pCtx, NsSelectableDrug* pDrugInfo) ;
    ~NSBdmDrugInfoDlg() ;

    void fillList() ;
    void openUrl(const string sUrl) ;

    void activatedInformation(int iIndex) ;

    NsSelectableDrug* getDrugInfo() { return _pDrugInfo ; }

  protected:

    void SetupWindow() ;

    NsSelectableDrug* _pDrugInfo ;

    string            getTitle() { return _pDrugInfo->getLabel() ; }
    NsHtmlLinksArray* getLinks() { return _pDrugInfo->getLinks() ; }

  DECLARE_RESPONSE_TABLE(NSBdmDrugInfoDlg) ;
} ;

#endif // __NSBDMDRUGINFODLG_H

