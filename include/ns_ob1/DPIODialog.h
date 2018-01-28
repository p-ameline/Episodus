#ifndef __DPIODIALOG_H__
# define __DPIODIALOG_H__

# include <owl/dialog.h>
# include <owl/groupbox.h>
# include <vector.h>
# include <string>

# include "nautilus\nssuper.h"
# include "ns_bbk\BB1types.h"

class DPIOref ;

class DPIOrefButton : public OWL::TRadioButton
{
  public:

    DPIOref	*pGroup ;
    int     iType ;

    DPIOrefButton(TWindow* parent, int resId, int type, const char far* title, int x,
											int y, int w, int h) ;
    ~DPIOrefButton() ;

    void BNClicked() ;  // BN_CLICKED

  DECLARE_RESPONSE_TABLE(DPIOrefButton) ;};

class	DPIOref
{
  public:

    enum TYPEREFBUTTON { non = 0, oui, refus } ;

	  DPIOref(BB1KSAR 			*theksar,
					  DPIOrefButton *theOui,
					  DPIOrefButton *theNon,
					  DPIOrefButton *theRefus,
					  TStatic 			*theText) ;

    ~DPIOref() ;

    void            Check(int etat, bool bInitEtat = false) ;

    DPIOrefButton	*pOui ;
    DPIOrefButton	*pNon ;
    DPIOrefButton	*pRefus ;
    TStatic				*pTexte ;
    BB1KSAR				*ksar ;
    int 					iEtat ;
};

class _BBKEXPORT DPIODialog : public NSUtilDialog //public TDialog, public NSRoot
{
  public:

	  DPIODialog(NSContexte *pCtx, vector<BB1KSAR *>	*KSARvector) ;
	  ~DPIODialog() ;

	  void										SetupWindow() ;
	  void										CmOk() ;
	  void										CmCancel() ;

	  bool										KSisChecked(DPIOref *refKS) ;

	  vector<BB1KSAR*>				_ksars ;

	  // boutons
	  vector<DPIOref*>				_aDPIOrefs ;

	DECLARE_RESPONSE_TABLE(DPIODialog) ;
};

#endif // __DPIODIALOG_H__
