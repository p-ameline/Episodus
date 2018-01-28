#if !defined(NSREFEDIT_H)#define NSREFEDIT_H

class NSSavoirArray ;
class NSTreeWindow ;
class Cproposition ;
class nsrefParseur ;

#include <owl\listwind.h>
#include <owl\panespli.h>
#include <owl\combobox.h>
#include <owl\edit.h>

/*
#include <owl\dialog.h>#include <owl\radiobut.h>

#include <owl\checkbox.h>
#include <owl\groupbox.h>
#include <owl\listbox.h>
#include <owl\scrollba.h>

#include "nsbb\nsednum.h"#include "nsbb\nstrewi.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsarc.h"#include "nsoutil\nssavary.h"

#define ID_TREE 100#define ID_LIST 101
*/

#include "nautilus\nssuper.h"
#include "nsoutil\nsexport.h"

//// class TPaneListWindow : liste window qui contiendra les équivalents
//									sémantiques d'un item donné
// ~~~~~ ~~~~~~~~~~~~~~~
class NSPaneListWindow : public TListWindow
{
  public:

    NSPaneListWindow(TWindow* parent, int id, int x, int y, int w, int h,
        TModule* module = 0)
        :TListWindow(parent, id, x, y, w, h, module){}
};

//
// class NSsmEditeur : 	fenêtre divisée en 2 parties : la première contient
//								la patpatho de sLexiquePilote et la deuxième contient
//								les équivalents sémantiques de sLexiquePilote.
//
// ~~~~~ ~~~~~~~~~~~~~
//
class _NSOUTILCLASSE NSsmEditeur : public NSRoot, public TPaneSplitter
{  public:
    //
    // Data members
    //
    string 				sLexiquePilote ; //élément lexique choisi par l'utilisateur

    NSPatPathoArray* 	pPatPathoArray ;
    NSSavoirArray*		pSavoirArray ;

    NSSmallBrother* 	pBigBoss ;

    NSTreeWindow* 		pNSTreeWindow ;
    NSPaneListWindow* 	pListWind ;

    NSsmEditeur(TWindow* parent, NSContexte* pCtx, string sLexiquePilot) ;
    ~NSsmEditeur() ;

    void SetupWindow() ;
    void CleanupWindow() ;
    void EvClose() ;

    void CmEnregistre() ;
    bool enregPatPatho() ;

    void initPatPatho() ;
    void initSavoir() ;

    void AfficheListe() ;
    void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;

    void donneRelation(string* pQualificatif, string* pQualifie, string* pEtiquette) ;

  DECLARE_RESPONSE_TABLE(NSsmEditeur) ;
};
//// Classes Propositions//class NSProposEdit{  public:    string              _sName ;    string              _sGroup ;    string              _sValidity ;    NSVectPatPathoArray _aTreeArray ;    int                 _iNbTrees ;    NSProposEdit() ;    NSProposEdit(Cproposition* prop) ;    NSProposEdit(const NSProposEdit& rv) ;    ~NSProposEdit() ;    void init() ;    string               getName()     { return _sName ; }    string               getGroup()    { return _sGroup ; }    string               getValidity() { return _sValidity ; }    NSVectPatPathoArray* getTrees()    { return &_aTreeArray ; }    int                  getNbTrees()  { return _iNbTrees ; }    void setName(string sN)     { _sName     = sN ; }    void setGroup(string sG)    { _sGroup    = sG ; }    void setValidity(string sV) { _sValidity = sV ; }    void setNbTrees(int iNbT)   { _iNbTrees  = iNbT ; }    NSProposEdit& operator=(const NSProposEdit& src) ;};typedef vector<NSProposEdit*> NSProposEditVector ;typedef NSProposEditVector::iterator       NSProposEditIter ;
typedef NSProposEditVector::const_iterator NSProposEditConstIter ;

class NSProposEditArray : public NSProposEditVector
{  public :
    // Constructeurs		NSProposEditArray() : NSProposEditVector() {}		NSProposEditArray(const NSProposEditArray& rv) ;    NSProposEditArray& operator=(const NSProposEditArray& src) ;
		// Destructeur		virtual ~NSProposEditArray() ;    void vider() ;};//// Classe NSRefEditeur : Editeur de référentiels//
class _NSOUTILCLASSE NSListPropWindow ;

class _NSOUTILCLASSE NSRefEditeur : public NSUtilDialog
{
  public:

    OWL::TEdit*        _pRefName ;
    string             _sFileName ;
    NSListPropWindow*  _pListProp ;
    NSProposEditArray  _aPropArray ;
    nsrefParseur*      _pParseur ;
    bool               _bIsModif ;
    int                _nbProp ;

    NSRefEditeur(TWindow* pere, NSContexte* pCtx) ;
    ~NSRefEditeur() ;

    void SetupWindow() ;
    void InitListeProp() ;
    void AfficheListeProp() ;
    void DispInfoListeProp(TLwDispInfoNotify& dispInfo) ;
    void EcrireTree(NSPatPathoArray* pPatPathoArray, string& sOut) ;

    void CmReferentiel() ;
    void CmEditProp() ;
    void CmHaut() ;
    void CmBas() ;
    void CmAjouter() ;
    void CmSupprimer() ;
    void CmSauvegarde() ;
    void CmOk() ;
    void CmCancel() ;

  DECLARE_RESPONSE_TABLE(NSRefEditeur) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les fiches Props à modifier)
//

class _NSOUTILCLASSE NSListPropWindow : public TListWindow{
  public:

	  NSRefEditeur* pDlg ;
	  NSListPropWindow(NSRefEditeur* pere, int resId) : TListWindow(pere, resId)    {
      pDlg = pere ;
    }

    ~NSListPropWindow() {}
	  void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    int  IndexItemSelect() ;

  DECLARE_RESPONSE_TABLE(NSListPropWindow) ;};

class _NSOUTILCLASSE NSPropEditeur : public NSUtilDialog
{
  public:

    OWL::TEdit*       pPropName;
    OWL::TEdit*       pPropGroup;
    OWL::TEdit*       pTreeNum;
    NSPatPathoArray* 	pPatPathoArray;
    NSSmallBrother* 	pBigBoss;
    NSTreeWindow* 		pNSTreeWindow;
    NSProposEdit*     pPropos;
    bool              bIsModif;
    bool              bWithTitle;
    int               noTree;
    string            sPropName;
    string            sPropGroup;

    NSPropEditeur(TWindow* pere, NSContexte* pCtx, NSProposEdit* prop);
    ~NSPropEditeur();

    void SetupWindow();
    void initPatPatho(int idTree);
    void rechargerPatPatho();
    void sauverPatPatho();

    void CmSuivant();
    void CmPrecedent();
    void CmAjouter();
    void CmSupprimer();
    void CmOk();
    void CmCancel();

  DECLARE_RESPONSE_TABLE(NSPropEditeur) ;
};


class _NSOUTILCLASSE NSLexiqEditeur : public NSUtilDialog
{
  public:

    OWL::TEdit*         pLexiLibelle ;
    OWL::TComboBox*     pLexiType ;
    OWL::TComboBox*     pLexiGrammaire ;
    OWL::TListWindow*   pLexiListe ;

    OWL::TButton*       pAdd ;
    OWL::TButton*       pReplace ;
    OWL::TButton*       pDelete ;

    string*             psAmmorce ;
    string              sPartLib ;

    NSLexiqEditeur(TWindow* pere, NSContexte* pCtx, string* pAmmorce, string sLibel = "") ;
    ~NSLexiqEditeur() ;

    void            SetupWindow() ;

    void            CmAjouter() ;
    void            trouveAmmorce() ;

    // void            CmOk() ;
    // void            CmCancel() ;

    void            initSynonymsList() ;
    VecteurString   TrouverListeSynonimes(string sCodeTerm) ;

  DECLARE_RESPONSE_TABLE(NSLexiqEditeur);
};

#endif
