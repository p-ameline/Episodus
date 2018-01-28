#ifndef __PROJECTDIALOGKS_H__
# define __PROJECTDIALOGKS_H__

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

class ProjectRef ;
class OB1NKS ;

class ProjectRefButton : public OWL::TRadioButton
{
 public:

	ProjectRef *pGroup ;
	int        iType ;

	ProjectRefButton(TWindow* parent, int resId, int type, const char far* title, int x,
											int y, int w, int h) ;

	~ProjectRefButton() ;

	void BNClicked() ;  // BN_CLICKED

	DECLARE_RESPONSE_TABLE(ProjectRefButton) ;};

class	ProjectRef
{
 public:
	enum TYPEREFBUTTON { non = 0, oui, refus } ;

  ProjectRef(OB1NKS* k,
             ProjectRefButton* theOui,
             ProjectRefButton* theNon,
             TStatic *theText) ;

	~ProjectRef() ;

	void Check(int etat, bool bInitEtat = false) ;

  OB1NKS             *ks;
	ProjectRefButton	 *pOui ;
	ProjectRefButton	 *pNon ;
	TStatic 					 *pTexte ;
  NautilusStatus     nStatus ;
	int 						   iEtat ;
};

class ProjectDialogKS ;

class ProjectDialog : public NSUtilDialog
{
 public:

	ProjectDialog(NSContexte *pCtx, Transfert* trans) ;
	~ProjectDialog() ;

	void										SetupWindow() ;
	void										CmOk() ;
	void										CmCancel() ;

	bool										KSisChecked(ProjectRef *refKS) ;

  void  									setParentKS(ProjectDialogKS* parent) {  _parent= parent; }

	// boutons
	vector<ProjectRef*>  ProjectRefs ;
  OWL::TGroupBox*      pGroup ;
  OWL::TGroupBox*      pDip ;
	OWL::TButton* 			 pBtOK ;
	OWL::TButton*        pBtCn ;
  OWL::TButton*        pBtHl ;

  std::vector<OB1NKS*> *kss ;
  std::vector<OB1NKS*> *kssProposable ;
  ProjectDialogKS      *_parent ;

	DECLARE_RESPONSE_TABLE(ProjectDialog) ;
};

class _BBKEXPORT ProjectDialogKS : public AbstractKS
{
  public:
  ProjectDialogKS() ;

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
		 ProjectDialogKS        ks ;
#  endif
# else
		 extern TModule       	*pNSResModule ;
		 extern ProjectDialogKS	ks ;
# endif


#endif // __PROJECTDIALOGKS_H__

