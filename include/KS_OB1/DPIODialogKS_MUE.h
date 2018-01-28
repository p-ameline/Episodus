#ifndef __DPIODIALOG_H__
# define __DPIODIALOG_H__

# include <owl/dialog.h>
# include <owl/groupbox.h>
# include <owl/radiobut.h>
# include <vector.h>
# include <string>

# include "nautilus\nssuper.h"
# include "nsbb\nsutidlg.h"
# include "ns_ob1\BB1types.h"

#ifndef __TCONTROLER__
# include "KS\DPIOKS.h"
#endif

# include "ns_ob1\InitStructure.h"  // for KSInitStructure
# include "ns_ob1\abstractKS.h"     // for Abtract
# include "ns_ob1\Transfer.h"        // for TRansfert


class DPIOref ;
class OB1NKS ;


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

  DPIOref(OB1NKS* k,
          DPIOrefButton* theOui,
          DPIOrefButton* theNon,
          DPIOrefButton* theRefus,
          TStatic *theText) ;

	~DPIOref() ;

	void            Check(int etat, bool bInitEtat = false) ;

  OB1NKS          *ks;
	DPIOrefButton		*pOui ;
	DPIOrefButton		*pNon ;
	DPIOrefButton		*pRefus ;
	TStatic					*pTexte ;
  NautilusStatus   nStatus ;
	int 						iEtat ;
};

class DPIODialogKS_MUE;

class DPIODialog : public NSUtilDialog
{
 public:
	DPIODialog(NSContexte *pCtx, Transfert* trans) ;
	~DPIODialog() ;

	void										SetupWindow() ;
	void										CmOk() ;
	void										CmCancel() ;
  void										CmNoToAll() ;

	bool										KSisChecked(DPIOref *refKS) ;

  void  									setParentKS(DPIODialogKS_MUE* parent) {  _parent= parent; }

	// boutons
	vector<DPIOref*>				DPIOrefs ;
  OWL::TGroupBox*         pGroup ;
  OWL::TGroupBox*         pDip ;
	OWL::TButton* 					pBtOK ;
	OWL::TButton*           pBtCn ;
  OWL::TButton*           pBtHl ;

  std::vector<OB1NKS* >		*kss ;
  std::vector<OB1NKS* >		*kssProposable ;
  DPIODialogKS_MUE*				_parent ;

	DECLARE_RESPONSE_TABLE(DPIODialog) ;
};

class  _BBKEXPORT DPIODialogKS_MUE : public AbstractKS
{
  public:
  DPIODialogKS_MUE();

	bool			          TriggerCondition(const BB1Event& event) ;
  KsInitStructure*    Publication() ;
  ValidityContextType ContextValidity(const BB1BB& bb, Transfert* trans) ;
  bool			          Precondition(const BB1BB& bb, Transfert* ksar) ;
	bool			          ObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;

  Errcode		          Action(BB1BB& bb, Transfert* trans, bool isOpportuniste) ;

  public:
    bool _isActive ;
};

bool                _export init(string *pInit) ;
KsInitStructure*    _export Publication() ;
ValidityContextType _export ContextValidity(const BB1BB& bb, Transfert* trans);
bool	        	    _export KSPrecondition(const BB1BB& bb, Transfert* ksar) ;
bool                _export KSTriggerCondition(const BB1Event& event) ;

bool                _export KSObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;

Errcode             _export KSAction(BB1BB& bb, Transfert* trans, bool isOpportuniste) ;

//bool		_export TheKSAgePrecondition(const BB1BB& bb) ;

# ifdef _MAIN
#  ifndef __MAIN
		 extern TModule       	*pNSResModule ;
#  else
		 static TModule       	*pResMod ;
		 TModule              	*pNSResModule ;
		 DPIODialogKS_MUE        ks ;
#  endif
# else
		 extern TModule       	*pNSResModule ;
		 extern DPIODialogKS_MUE	ks ;
# endif


#endif // __DPIODIALOG_H__
