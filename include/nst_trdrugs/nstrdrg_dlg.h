#ifndef __NSTRDRG_DLG_H#define __NSTRDRG_DLG_H

#include <owl\edit.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>

#include "nsbb\nsutidlg.h"

class TrDrugPresentation
{
 public:
  // constructor/destructor
  TrDrugPresentation() ;
  TrDrugPresentation(TrDrugPresentation& src) ;
  ~TrDrugPresentation() ;

  // ioerator
  TrDrugPresentation operator=(TrDrugPresentation& src) ;

  void setElement(int iIndice, string sLib) ;

  string sBarCode ;
  string sGeneric ;
  string sOriginal ;
  string sLib ;
  string sActivePrinciples ;
  string sStrength ;
  string sStrengthUnit ;
  string sForm ;
  string sPresentationNbre ;
  string sPresentationUnit ;
  string sLabName ;
 	string sImportStatus ;
  string sRecette ;
  string s2004Value ;
  string sRefEuroPrice ;
  string sRefCountry ;
  string sPrice1 ;
  string sPrice2 ;
  string sPrice3 ;
  string sPrice4 ;
  string s8 ;
  string sDateModif ;
  string sLibModif ;
} ;

typedef vector<TrDrugPresentation *> NSTRPRArray ;
typedef NSTRPRArray::iterator NSTrPresentationIter ;
typedef NTArray<TrDrugPresentation> NSTrPresentationArray ;

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsTrDrugsDialog : public NSUtilDialog
{
	public:

		OWL::TCheckBox*	  pProcess ;

		ofstream          logFile ;

		NsTrDrugsDialog(TWindow* pere, NSContexte* pCtx) ;
		~NsTrDrugsDialog() ;

		void SetupWindow() ;

		void trDrugsProcess() ;
    void processLine(string* psLine, NSTrPresentationArray* pPrArray) ;
    bool getNextItem(string* psLine, string* psResult, size_t* piPosit) ;

	DECLARE_RESPONSE_TABLE(NsTrDrugsDialog) ;
} ;

#endif  // __NSTRDRG_DLG_H
