// -----------------------------------------------------------------------------// nslistwind.h
// -----------------------------------------------------------------------------
// Fenêtre ListWindow
// -----------------------------------------------------------------------------
// $Revision: 1.8 $
// $Author: pameline $
// $Date: 2015/02/07 14:34:47 $
// -----------------------------------------------------------------------------
// PA  - septembre 2003
// -----------------------------------------------------------------------------
// FLP - aout 2004
// modification de graphPrepare() - on ne fait plus de getPatho() ou appelle la
// méthode de NSDataTree qui le trie par localisation.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------
#ifndef __NSLISTWIND_H# define __NSLISTWIND_H

class gereNum ;
class gereDate ;
class gereHeure ;
class gereCode ;
class NSVectFatheredPatPathoArray ;
class Message ;
class NSPidsInfo ;
class NSPids ;
class NSPidsArray ;
class NSPersonsAttributesArray ;
class NVLdVTemps ;
class NSControle ;

# include <owl/listwind.h># include <owl/listbox.h># include <owl/imagelst.h>
# include <owl/scrollba.h>
# include <owl/groupbox.h>
# include <owl/radiobut.h>
# include <winuser.h>

// # include "nsbb\nstrnode.h"// # include "nsbb\nsdico.h"
# include "nsbb\nsednum.h"             // for NSUtilEdit
# include "nsbb\nsutidlg.h"
# include "nsbb\nspatpat.h"
# include "partage\nsperson.h"
// # include "dcodeur\nsdkd.h"

// macro pour appeler LvnGetDispInfo
# define NS_LV_DISPINFO_NOTIFY(notifyCode, method)\
  {notifyCode, UINT_MAX, (TAnyDispatcher)::v_LPARAM_Dispatch,\
  (TMyPMF)v_LV_DISPINFO_NOTIFY_Sig(&TMyClass::method)}

# define NS_LVN_GETDISPINFO(method) NS_LV_DISPINFO_NOTIFY(LVN_GETDISPINFO, method)

// macro pour gérer localement les méthodes qui utilisent un TLwNotify
# define NS_LISTWIND_NOTIFY(notifyCode, method)\
  {notifyCode, UINT_MAX, (TAnyDispatcher)::v_LPARAM_Dispatch,\
  (TMyPMF)v_LISTWIND_NOTIFY_Sig(&TMyClass::method)}

# define NS_LW_COLUMNCLICK(method) NS_LISTWIND_NOTIFY(LVN_COLUMNCLICK, method)

class _NSBBCLASSE NSSkinableListWindow : public TListWindow, public NSRoot
{
  public:

 	  // Constructeurs/Destructeur
    NSSkinableListWindow(TWindow *parent, NSContexte *pCtx, int id, int x, int y, int w, int h, OWL::TModule *module = 0) ;
    NSSkinableListWindow(TWindow *parent, NSContexte *pCtx, int resourceId, OWL::TModule *module = 0) ;
    virtual ~NSSkinableListWindow() ;

    void    SetupWindow() ;

    bool    skinSwitch(string sSkinName) ;
    void    skinSwitchOn(string sSkinName) ;
    void    skinSwitchOff(string sSkinName) ;

  DECLARE_RESPONSE_TABLE(NSSkinableListWindow) ;
} ;


// -----------------------------------------------------------------------------
// Classe controle NSAdrListWindow
// -----------------------------------------------------------------------------
class _NSBBCLASSE NSAdrListWindow : public NSSkinableListWindow
{
 public:

 	NSVectFatheredPatPathoArray* pVectData ;
  VecteurString*          pVectLib ;
  VecteurString*          pVectTel ;
  NSControle* 	  	    	pControle ;
  bool                    bSetupWindow ;

  // Constructeurs/Destructeur
  NSAdrListWindow(TWindow *parent, NSContexte *pCtx, int id, int x, int y, int w, int h, OWL::TModule *module = 0);
  NSAdrListWindow(TWindow *parent, NSContexte *pCtx, int resourceId, OWL::TModule *module = 0) ;
  ~NSAdrListWindow() ;

  // réponse aux événements
  void    EvSetFocus(HWND hWndLostFocus) ;
  void    EvKillFocus(HWND hWndGetFocus) ;
  void    EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
  void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  void    LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;
  void    EvKeyDown(uint key, uint repeatCount, uint flags) ;

  // méthodes
  void    SetupWindow() ;
  void    SetupColumns() ;
  int     IndexItemSelect() ;
  void    AfficheListe() ;
  string  TrouveLibelle(NSPatPathoArray *pPatPathoArray) ;
  bool    TrouveObjectAdr(NSPatPathoArray *pPatPathoArray, string& sObject, string& sChez) ;
  void    TrouveLibelleAdr(string sObject, string& sLib, string& sTel) ;
  void    activeControle(int activation, Message *pMessage = 0) ;
  WNDTYPE     donneType() { return (isAdrListWindow) ; }

  // entrées de menu
  void    CmCreerAdr() ;
  void    CmModifAdr() ;
  void    CmChangeAdr() ;
  void    CmCorrecAdr() ;
  void    CmSupprAdr() ;

  bool    canWeClose() ;

 DECLARE_RESPONSE_TABLE(NSAdrListWindow) ;
} ;

// -----------------------------------------------------------------------------
// Classe controle NSCorListWindow
// -----------------------------------------------------------------------------
class _NSBBCLASSE NSCorListWindow : public NSSkinableListWindow
{
 public:

 	NSVectFatheredPatPathoArray* pVectData ;
  VecteurString*          pVectLib ;
  NSControle* 	  	    	pControle ;
  bool                    bSetupWindow ;

	// Constructeurs/Destructeur
  NSCorListWindow(TWindow *parent, NSContexte *pCtx, int id, int x, int y, int w, int h, OWL::TModule *module = 0) ;
  NSCorListWindow(TWindow *parent, NSContexte *pCtx, int resourceId, OWL::TModule *module = 0) ;
  ~NSCorListWindow() ;

  // réponse aux événements
  void    EvSetFocus(HWND hWndLostFocus) ;
  void    EvKillFocus(HWND hWndGetFocus) ;
  void    EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
  void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  void    LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;
  void    EvKeyDown(uint key, uint repeatCount, uint flags) ;

  // méthodes
  void    SetupWindow() ;
  void    SetupColumns() ;
  int     IndexItemSelect() ;
  void    AfficheListe() ;
  string  TrouveLibelle(NSPatPathoArray *pPatPathoArray) ;
  string  TrouvePidsCorr(NSPatPathoArray *pPatPathoArray) ;
  string  TrouveLibelleCorr(string sPids) ;
  void    activeControle(int activation, Message *pMessage = 0) ;
  WNDTYPE     donneType() { return (isCorListWindow) ; }

  // entrées de menu
  void    CmCreerCorr() ;
  void    CmModifCorr() ;
  void    CmSupprCorr() ;

  bool    canWeClose() ;

 DECLARE_RESPONSE_TABLE(NSCorListWindow) ;
} ;


// -----------------------------------------------------------------------------
//
// Classe NSListePersonDialog
// pour la sélection d'un patient ou d'un correspondant dans la base
//
// -----------------------------------------------------------------------------

class NSListePersonScrollBar ;
class NSListePersonEdit ;
class NSListePersonWindow ;

// -----------------------------------------------------------------------------// Objet "Boite de dialogue" utilisé pour sélectionner un patient au sein d'une// liste// -----------------------------------------------------------------------------
class _NSBBCLASSE NSListePersonDialog : public NSUtilDialog
{
 public:

 	int				  	 	nbNom ;
  bool				 		bVScroll ;
  string			  	sNumSelect ;
  DBIResult 		  ErrDBI ;
  int				  	 	PersonChoisie ;

  NSPidsInfo*     pPersonSelect ;
  PIDSTYPE        iTypePids ;

  NSPids* 	      pPersonEnCours ;
  NSPidsArray*		pPersonArray ;

  bool            bCreer ;
  bool            bAvecCreer ;

  char				 		nomPerson[PAT_NOM_LEN + 1] ;  char				 		prenomPerson[PAT_PRENOM_LEN + 1] ;

  NSListePersonEdit*	    pNom;  NSListePersonEdit*	    pPrenom;

  NSListePersonWindow* 	pListe;  NSListePersonScrollBar* pVScroll;
  OWL::TButton*           pCreer;

	NSListePersonDialog(TWindow *pere, PIDSTYPE typePids, bool avecCreer, NSContexte *pCtx, OWL::TModule *module = 0) ;  ~NSListePersonDialog() ;

  void SetupWindow() ;  bool InitRecherche() ;
  void InitListe() ;
  void AfficheListe() ;
  void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;
  void InitPersonArray() ;
  void PosLastPerson() ;
  void ScrollPerson() ;
  void GardeNum(int index) ;
  void RetrouvePersonSelect() ;
  void CmCreer() ;

  bool LineDown() ;
  bool LineUp() ;

  bool CanClose() ;  void CmOk() ;

 DECLARE_RESPONSE_TABLE(NSListePersonDialog) ;} ;


// -----------------------------------------------------------------------------// Objet "ListWindow" avec gestion du double-click et du scroll (pour les correspondants)// -----------------------------------------------------------------------------
class _NSBBCLASSE NSListePersonWindow : public TListWindow
{
 public:

 	NSListePersonDialog	*pDlg ;

  NSListePersonWindow(NSListePersonDialog *pere, int resId, OWL::TModule *module = 0) ;  ~NSListePersonWindow() {}

  void    SetupWindow() ;
  void    EvSetFocus(HWND hWndLostFocus) ;
  void    EvKillFocus(HWND hWndGetFocus) ;
  void    EvKeyDown(uint key, uint repeatCount, uint flags) ;  void    EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  int     IndexItemSelect() ;

 DECLARE_RESPONSE_TABLE(NSListePersonWindow) ;} ;

// -----------------------------------------------------------------------------// Classe NSListePersonEdit// pour scroller quand on change le nom ou le prénom
// -----------------------------------------------------------------------------
class _NSBBCLASSE NSListePersonEdit : public NSUtilEdit
{
 public:

 	NSListePersonDialog	*pDlg ;

  NSListePersonEdit(NSContexte* pCtx, NSListePersonDialog *pere, int resId, UINT iTextLen)                   : NSUtilEdit(pCtx, pere, resId, iTextLen) { pDlg = pere ; }  ~NSListePersonEdit() {}

 	void EvChar(uint key, uint repeatCount, uint flags) ;
 DECLARE_RESPONSE_TABLE(NSListePersonEdit);} ;


// -----------------------------------------------------------------------------// Objet "ScrollBar" utilisé pour faire défiler verticalement la liste des// correspondants
// -----------------------------------------------------------------------------
class _NSBBCLASSE NSListePersonScrollBar : public OWL::TScrollBar
{
 public:

 	NSListePersonDialog	*pListeDlg ;

  NSListePersonScrollBar(NSListePersonDialog *parent, int resourceId, OWL::TModule *module = 0) ;
  void FixeRange(int max, int taille) ;  void SBLineDown() ;
  void SBLineUp() ;
  void SBPageDown() ;
  void SBPageUp() ;
  void PositionneCurseur() ;
} ;


// -----------------------------------------------------------------------------
// Classe NSTPersonListDialog -> pour la sélection d'une personne (patient, utilisateur, correspondant) dans la base
// -----------------------------------------------------------------------------

class NSTPersonListScrollBar ;
class NSTPersonListEdit ;
class NSTPersonListWindow ;

// -----------------------------------------------------------------------------// Objet "Boite de dialogue" utilisé pour sélectionner un patient au sein// d'une liste
// -----------------------------------------------------------------------------
class _NSBBCLASSE NSTPersonListDialog : public NSUtilDialog
{
 public:

 	int                     nbNom ;
  bool                    bVScroll ;
  string                  sNumSelect ;
  int                     PersonChoisie ;
  const char*             serviceName ;
  size_t                  iStepSize ;
  bool                    bLimitsReached ;

  NSPersonInfo*           pPersonSelect ;
  PIDSTYPE                iTypePids ;
  int                     iSelectPids ;
  NSBasicAttributeArray*  pCriteres ;
  NSPersonInfo*           pPersonEnCours ;
  NSPersonArray*          pPersonArray ;

  bool                    bCreer ;
  bool                    bAvecCreer ;
  bool                    bSinglePidsMode ;

  // char                    nomPerson[PAT_NOM_LEN + 1] ;  // char                    prenomPerson[PAT_PRENOM_LEN + 1] ;
  string                  _sNomPerson ;
  string                  _sPrenomPerson ;
  string                  _sRolePerson ;

  NSTPersonListEdit*      pNom ;  NSTPersonListEdit*      pPrenom ;
  NSUtilLexique*          pRole ;
  OWL::TGroupBox*         pGroup ;
  OWL::TRadioButton*      pPidsPatient ;
  OWL::TRadioButton*      pPidsUtilisat ;
  OWL::TRadioButton*      pPidsCorresp ;
  OWL::TRadioButton*      pPidsTous ;

  OWL::TStatic*           pFamNameTxt ;
  OWL::TStatic*           pGivenNameTxt ;
  OWL::TStatic*           pRoleTxt ;

  NSTPersonListWindow*    pListe ;  NSTPersonListScrollBar* pVScroll ;
  OWL::TButton*           pCreer ;
  OWL::TButton*           pSearchNow ;

  NSTPersonListDialog(TWindow *pere, PIDSTYPE typePids, bool avecCreer, NSContexte *pCtx,                        NSBasicAttributeArray* pCrit = 0, bool bSimpleMode = false, string sRole = string(""), OWL::TModule *module = 0) ;  ~NSTPersonListDialog() ;

  void SetupWindow() ;  void setAccurateCaption() ;  bool InitRecherche() ;
  void InitListe() ;
  void AfficheListe() ;
  void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;
  void InitPersonArray() ;
  void ReinitPersonArray(bool bForceDisplay = false) ;
  void PosLastPerson() ;
  void ScrollPerson() ;
  void repositScroll() ;
  void GardeNum(int index) ;
  void RetrouvePersonSelect() ;
  void CmSelectPids() ;
  void CmCreer() ;
  void Rechercher() ;
  void SearchNow() ;
  void EvTimer(uint id) ;
  void resetTimer() ;

  bool LineDown() ;
  bool LineUp() ;

  bool CanClose() ;  void CmOk() ;

 DECLARE_RESPONSE_TABLE(NSTPersonListDialog) ;} ;

// -----------------------------------------------------------------------------
// Objet "ListWindow" avec gestion du double-click et du scroll (pour les correspondants)// -----------------------------------------------------------------------------
class _NSBBCLASSE NSTPersonListWindow : public TListWindow
{
 public:

 	NSTPersonListDialog	*pDlg ;

  NSTPersonListWindow(NSTPersonListDialog* pere, int resId, OWL::TModule* module = 0) ;  ~NSTPersonListWindow() {}

  void    SetupWindow() ;
  void    EvSetFocus(HWND hWndLostFocus) ;
  void    EvKillFocus(HWND hWndGetFocus) ;
  void    EvKeyDown(uint key, uint repeatCount, uint flags) ;  void    EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  int     IndexItemSelect() ;

 DECLARE_RESPONSE_TABLE(NSTPersonListWindow) ;} ;

// -----------------------------------------------------------------------------// Classe NSTPersonListEdit// pour scroller quand on change le nom ou le prénom// -----------------------------------------------------------------------------
class _NSBBCLASSE NSTPersonListEdit : public NSUtilEdit
{
 public:

 	NSTPersonListDialog	*pDlg ;

  NSTPersonListEdit(NSContexte* pCtx, NSTPersonListDialog* pere, int resId, UINT iTextLen, OWL::TModule* module = 0)  	               :NSUtilEdit(pCtx, pere, resId, iTextLen, true, module)                   { pDlg = pere ; }
  ~NSTPersonListEdit() {}

  void EvChar(uint key, uint repeatCount, uint flags) ;
 DECLARE_RESPONSE_TABLE(NSTPersonListEdit) ;} ;


// -----------------------------------------------------------------------------// Objet "ScrollBar" utilisé pour faire défiler verticalement la liste des// correspondants
// -----------------------------------------------------------------------------
class _NSBBCLASSE NSTPersonListScrollBar : public OWL::TScrollBar
{
 public:

 	NSTPersonListDialog *pListeDlg ;

  NSTPersonListScrollBar(NSTPersonListDialog* parent, int resourceId, OWL::TModule* module = 0) ;

  void FixeRange(int max, int taille) ;  void SBLineDown() ;
  void SBLineUp() ;
  void SBPageDown() ;
  void SBPageUp() ;
  void PositionneCurseur() ;
} ;

// -----------------------------------------------------------------------------
// Objet "Boite de dialogue" utilisé pour le choix utilisateur en cas de doublons// -----------------------------------------------------------------------------
class NSTDoublonEquivListWindow;
class NSTDoublonDiffListWindow;

class _NSBBCLASSE NSTDoublonListDialog : public NSUtilDialog
{
 public:

    string                              sPersonSelect;
    string                              sRolePersonSelect;
    bool                                bCreer;
    NSPersonsAttributesArray*           pRoleEquivArray;
    NSPersonsAttributesArray*           pRoleDiffArray;
    string                              sRolePerson;

    OWL::TGroupBox*                     pGroupRoleEquiv ;
    OWL::TGroupBox*                     pGroupRoleDiff ;
    OWL::TStatic*                       pTexteRoleEquiv;
    OWL::TStatic*                       pTexteRoleDiff;
    OWL::TStatic*                       pButtonCreer;
    OWL::TStatic*                       pButtonAnnuler;
    OWL::TStatic*                       pButtonAttrib;

    NSTDoublonEquivListWindow* 	    	pListeRoleEquiv ;    NSTDoublonDiffListWindow* 	    	pListeRoleDiff ;

    NSTDoublonListDialog(TWindow *pere, NSContexte *pCtx, NSPersonsAttributesArray *pList, string sRole, OWL::TModule *module = 0) ;    ~NSTDoublonListDialog() ;

    void SetupWindow() ;    void InitListeRoleEquiv() ;
    void AfficheListeRoleEquiv() ;
    void LvnGetDispInfo1(TLwDispInfoNotify& dispInfo) ;
    void InitListeRoleDiff() ;
    void AfficheListeRoleDiff() ;
    void LvnGetDispInfo2(TLwDispInfoNotify& dispInfo) ;
    void CmAttribuerRole() ;
    void CmCreer() ;

    bool CanClose() ;
    void CmOk() ;
    void CmCancel() ;

 DECLARE_RESPONSE_TABLE(NSTDoublonListDialog) ;} ;

// -----------------------------------------------------------------------------
// Objet "ListWindow" avec gestion du double-click et du scroll// -----------------------------------------------------------------------------
class _NSBBCLASSE NSTDoublonEquivListWindow : public TListWindow
{
 public:

 	NSTDoublonListDialog	*pDlg ;

	NSTDoublonEquivListWindow(NSTDoublonListDialog* pere, int resId, OWL::TModule* module = 0) ;  ~NSTDoublonEquivListWindow() {}

  void SetupWindow() ;
  void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSTDoublonEquivListWindow) ;} ;

class NSTDoublonEquivRoleListWindow ;

class _NSBBCLASSE NSTSingleRoleDoublonListDialog : public NSUtilDialog
{
 public:

    string                     sPersonSelect ;
    string                     sRolePersonSelect ;
    bool                       bCreer ;
    NSPersonsAttributesArray*  pRoleEquivArray ;
    string                     sRolePerson ;

    OWL::TGroupBox*            pGroupRoleEquiv ;
    OWL::TStatic*              pTexteRoleEquiv ;
    OWL::TStatic*              pButtonCreer ;
    OWL::TStatic*              pButtonAnnuler ;
    OWL::TStatic*              pButtonAttrib ;

    NSTDoublonEquivRoleListWindow* pListeRoleEquiv ;
    NSTSingleRoleDoublonListDialog(TWindow *pere, NSContexte *pCtx, NSPersonsAttributesArray *pList, string sRole, OWL::TModule *module = 0) ;    ~NSTSingleRoleDoublonListDialog() ;

    void SetupWindow() ;    void InitListeRoleEquiv() ;
    void AfficheListeRoleEquiv() ;
    void LvnGetDispInfo1(TLwDispInfoNotify& dispInfo) ;
    void CmCreer() ;

    bool CanClose() ;
    void CmOk() ;
    void CmCancel() ;

 DECLARE_RESPONSE_TABLE(NSTSingleRoleDoublonListDialog) ;} ;

// -----------------------------------------------------------------------------
// Objet "ListWindow" avec gestion du double-click et du scroll// -----------------------------------------------------------------------------
class _NSBBCLASSE NSTDoublonEquivRoleListWindow : public TListWindow
{
 public:

 	NSTSingleRoleDoublonListDialog *pDlg ;

	NSTDoublonEquivRoleListWindow(NSTSingleRoleDoublonListDialog* pere, int resId, OWL::TModule* module = 0) ;  ~NSTDoublonEquivRoleListWindow() {}

  void SetupWindow() ;
  void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSTDoublonEquivRoleListWindow) ;} ;

// -----------------------------------------------------------------------------
// Objet "ListWindow" avec gestion du double-click et du scroll// -----------------------------------------------------------------------------
class _NSBBCLASSE NSTDoublonDiffListWindow : public TListWindow
{
 public:

 	NSTDoublonListDialog	*pDlg ;

	NSTDoublonDiffListWindow(NSTDoublonListDialog* pere, int resId, OWL::TModule* module = 0) ;  ~NSTDoublonDiffListWindow() {}

  void    SetupWindow() ;
  void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  int     IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSTDoublonDiffListWindow) ;} ;

// -----------------------------------------------------------------------------
// classe  NSProfActivity
// -----------------------------------------------------------------------------
class _NSBBCLASSE NSHistoryManagement
{
  public:

    NSHistoryManagement() ;
    NSHistoryManagement(const NSHistoryManagement& rv) ;
    ~NSHistoryManagement() ;

    NSHistoryManagement& operator=(const NSHistoryManagement& src) ;

    string getLibelle()                { return _sLibelle ; }
    void   setLibelle(string sAct)     { _sLibelle = sAct ; }

    string getStartDate()              { return _sStartDate ; }
    void   setStartDate(string sDate)  { _sStartDate = sDate ; }

    string getEndDate()                { return _sEndDate ; }
    void   setEndDate(string sDate)    { _sEndDate = sDate ; }

    int    getPosition()               { return _iTreePos ; }
    void   setPosition(int iPos)       { _iTreePos = iPos ; }

    // NSPatPathoArray   *pPatPathoArray ;

  protected:

    string _sLibelle ;
    string _sStartDate ;
    string _sEndDate ;
    int    _iTreePos ;
} ;

bool  drugSortByEndDataInf  (NSHistoryManagement* pDrug1, NSHistoryManagement* pDrug2) ;
bool  drugSortByEndDataSup  (NSHistoryManagement* pDrug1, NSHistoryManagement* pDrug2) ;
bool  drugSortByLibelleInf  (NSHistoryManagement* pDrug1, NSHistoryManagement* pDrug2) ;
bool  drugSortByLibelleSup  (NSHistoryManagement* pDrug1, NSHistoryManagement* pDrug2) ;
bool  drugSortByStartDataInf(NSHistoryManagement* pDrug1, NSHistoryManagement* pDrug2) ;
bool  drugSortByStartDataSup(NSHistoryManagement* pDrug1, NSHistoryManagement* pDrug2) ;

typedef vector<NSHistoryManagement *>     NSHistoryManArray ;
typedef NSHistoryManArray::iterator       NSHistoryManagementIter ;
typedef NSHistoryManArray::const_iterator NSHistoryManagementConstIter ;

class _NSBBCLASSE NSHistoryManagementArray : public NSHistoryManArray
{
 public:

  NSHistoryManagementArray() : NSHistoryManArray() {}
	NSHistoryManagementArray(const NSHistoryManagementArray& rv) ;
	~NSHistoryManagementArray() ;

  void            vider() ;
} ;


// -----------------------------------------------------------------------------
// Classe controle NSHistorizedListWindow
// -----------------------------------------------------------------------------
class _NSBBCLASSE NSHistorizedListWindow : public NSSkinableListWindow
{
  public:

    NSVectFatheredPatPathoArray *pVectData ;
    NSControle                  *pControle ;
    bool                        bSetupWindow ;
    NSHistoryManagementArray    aHistory ;
    bool                        bNaturallySorted ;
    int                         iSortedColumn ;
    bool                        bFirstUse ;

    // constructeurs  Destructeur
    NSHistorizedListWindow(TWindow *parent, NSContexte *pCtx, int id, int x, int y, int w, int h, OWL::TModule *module = 0) ;
    NSHistorizedListWindow(TWindow *parent, NSContexte *pCtx, int resourceId, OWL::TModule *module = 0) ;
    ~NSHistorizedListWindow() ;

    // réponse aux événements
    void                      EvSetFocus(HWND hWndLostFocus) ;
    void                      EvKillFocus(HWND hWndGetFocus) ;
    void                      EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void                      EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void                      LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;
    void                      LVNColumnclick(TLwNotify& lwn) ;
    void                      EvKeyDown(uint key, uint repeatCount, uint flags) ;

    // méthodes
    void                      SetupWindow() ;
    void                      SetupColumns() ;
    int                       IndexItemSelect() ;
    void                      AfficheListe() ;
    NSHistoryManagement       *TrouveObjectData(NSPatPathoArray *pPatPathoArray) ;
    void                      initCurentListe() ;

    void                      sortByEndingDate() ;
    void                      sortByStartingDate() ;
    void                      sortByLibelle() ;

    void                      activeControle(int activation, Message *pMessage = 0) ;
    WNDTYPE                   donneType()  { return (isHistoryListWindow) ; }

    // entrées de menu
    void                      CmCreerEnreg() ;
    void                      CmModifEnreg() ;
    void                      CmSupprEnreg() ;

    bool                      canWeClose() ;

  DECLARE_RESPONSE_TABLE(NSHistorizedListWindow) ;
} ;


// -----------------------------------------------------------------------------
// added by FLP for historized Valued list
// -----------------------------------------------------------------------------

class _NSBBCLASSE NSHistoryValueManagement
{
  private:

    static long lObjectCount ;

  public:

    NSHistoryValueManagement() ;
    ~NSHistoryValueManagement() ;

    NSHistoryValueManagement(const NSHistoryValueManagement& rv) ;
    NSHistoryValueManagement& operator=(const NSHistoryValueManagement& src) ;

    void initFromNum(gereNum* pNum, NSContexte *pCtx) ;
    void initFromDate(gereDate* pDate, NSContexte *pCtx) ;
    void initFromTime(gereHeure* pHour, NSContexte *pCtx) ;
    void initFromCode(gereCode* pHour, NSContexte *pCtx) ;

    string getLabel()               { return sLabel ; }
    void   setLabel(string sLib)    { sLabel = sLib ; }

    string getDate()                { return sDate ; }
    void   setDate(string sDat)     { sDate = sDat; }
    string getDateFin()             { return sDateFin ; }
    void   setDateFin(string sDat)  { sDateFin = sDat; }

    string getValue()               { return sValue ; }
    void   setValue(string sVal)    { sValue = sVal ; }
    string getUnit()                { return sUnit ; }
    void   setUnit(string sUni)     { sUnit = sUni ; }

    string getNormale()             { return sNormale ; }
    void   setNormale(string sNor)  { sNormale = sNor ; }
    string getNormInf()             { return sNormInf ; }
    void   setNormInf(string sNor)  { sNormInf = sNor ; }
    string getNormSup()             { return sNormSup ; }
    void   setNormSup(string sNor)  { sNormSup = sNor ; }

    string getValue2()              { return sValue2 ; }
    void   setValue2(string sVal)   { sValue2 = sVal ; }
    string getUnit2()               { return sUnit2 ; }
    void   setUnit2(string sUni)    { sUnit2 = sUni ; }

    string getNormale2()            { return sNormale2 ; }
    void   setNormale2(string sNor) { sNormale2 = sNor ; }
    string getNormInf2()            { return sNormInf2 ; }
    void   setNormInf2(string sNor) { sNormInf2 = sNor ; }
    string getNormSup2()            { return sNormSup2 ; }
    void   setNormSup2(string sNor) { sNormSup2 = sNor ; }

    string getNode()                { return sNode ; }
    void   setNode(string sNod)     { sNode = sNod ; }

    int    getPosition()            { return iTreePos ; }
    void   setPosition(int iPos)    { iTreePos = iPos ; }

    void   Reset()                  { sLabel = "" ; sDate = "" ;
                                      sValue = "" ; sUnit = "" ;
                                      sNormale = "" ; sNormInf = "" ; sNormSup = "" ;
                                      sValue2 = "" ; sUnit2 = "" ;
                                      sNormale2 = "" ; sNormInf2 = "" ; sNormSup2 = "" ;
                                      sNode = "" ; iTreePos = 0 ;
                                    }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    string sLabel ;
    string sDate ;
    string sDateFin ;

    string sValue ;
    string sUnit ;
    string sNormale ;
    string sNormInf ;
    string sNormSup ;

    string sValue2 ;
    string sUnit2 ;
    string sNormale2 ;
    string sNormInf2 ;
    string sNormSup2 ;

    string sNode ;
    int    iTreePos ;
} ;

bool _NSBBCLASSE sortByDateSup    (NSHistoryValueManagement  *pHVal1, NSHistoryValueManagement  *pHVal2) ;
bool _NSBBCLASSE sortByDateInf    (NSHistoryValueManagement  *pHVal1, NSHistoryValueManagement  *pHVal2) ;
bool _NSBBCLASSE sortByLabelSup   (NSHistoryValueManagement  *pHVal1, NSHistoryValueManagement  *pHVal2) ;
bool _NSBBCLASSE sortByLabelInf   (NSHistoryValueManagement  *pHVal1, NSHistoryValueManagement  *pHVal2) ;
bool _NSBBCLASSE sortByValueSup   (NSHistoryValueManagement  *pHVal1, NSHistoryValueManagement  *pHVal2) ;
bool _NSBBCLASSE sortByValueInf   (NSHistoryValueManagement  *pHVal1, NSHistoryValueManagement  *pHVal2) ;

typedef vector<NSHistoryValueManagement *>  NSHistoryValManArray ;
typedef NSHistoryValManArray::iterator      NSHistoryValManagementIter ;

class _NSBBCLASSE NSHistoryValManagementArray : public NSHistoryValManArray, public NSRoot
{
 public:

  enum historyType { histTypeNum = 0, histTypeDate, histTypeTime, histTypeCode } ;

  NSHistoryValManagementArray(NSContexte* pCtx, historyType iType = histTypeNum) ;
	NSHistoryValManagementArray(NSHistoryValManagementArray& rv) ;
	~NSHistoryValManagementArray() ;

  void vider() ;
  bool ChargeHistoryValueItem(string sItem) ;
  bool ChargeCaptureValueItem(string sItem) ;
  void ChargeValueFromPatPatho(const NSPatPathoArray* pPatPathoArray, PatPathoConstIter iterVal, NVLdVTemps* pDefaultDate = 0) ;
  void AddNewValue(NSPatPathoArray *pPt, NSHistoryValueManagement *pValue) ;

 protected:

  historyType _iType ;
} ;

class NSValListHistoryDialog ;

// -----------------------------------------------------------------------------
// Classe controle NSHistorizedListWindow
// -----------------------------------------------------------------------------
class _NSBBCLASSE NSHistorizedValListWindow : public NSSkinableListWindow
{
  public:

    NSVectFatheredPatPathoArray   *_pVectData ;
    NSControle                    *_pControle ;
    bool                          _bSetupWindow ;
    NSHistoryValManagementArray   _aHistory ;
    NSValListHistoryDialog*       _pDialogHistory ;
    bool                          _bNaturallySorted ;
    int                           _iSortedColumn ;
    bool                          _bFirstUse ;
    bool                          _bModeHistory ;
    string                        _sItemValue ;

    // constructeurs  Destructeur
    NSHistorizedValListWindow(TWindow *parent, NSContexte *pCtx, int id, int x, int y, int w, int h, bool bModeHis = false, OWL::TModule *module = 0) ;
    NSHistorizedValListWindow(TWindow *parent, NSContexte *pCtx, int resourceId, bool bModeHis = false, OWL::TModule *module = 0) ;
    ~NSHistorizedValListWindow() ;

    // réponse aux événements
    void                          EvSetFocus(HWND hWndLostFocus) ;
    void                          EvKillFocus(HWND hWndGetFocus) ;
    void                          EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void                          EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void                          LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;
    void                          LVNColumnclick(TLwNotify& lwn) ;
    void                          EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void                          EvChar(uint key, uint repeatCount, uint flags) ;

    // méthodes
    void                          SetupWindow() ;
    void                          SetupColumns() ;
    int                           IndexItemSelect() ;
    NSHistoryValueManagement*     getSelectedValue() ;
    void                          AfficheListe() ;
    NSHistoryValueManagement*     TrouveObjectData(NSPatPathoArray *pPPT) ;
    void                          initCurentListe() ;
    void                          selectFirstItem() ;

    void                          sortByValue() ;
    void                          sortByDate() ;
    void                          sortByLabel() ;

    void                          activeControle(int activation, Message *pMessage = 0) ;
    WNDTYPE                       donneType() { return (isHistoryValListWindow) ; }
    void                          setItemValue(string sIVal) { _sItemValue = sIVal ; }

    // entrées de menu
    void                          CmCreerEnreg() ;
    void                          CmModifEnreg() ;
    void                          CmSupprEnreg() ;
    void                          CmHistory() ;

    void                          CmReset() ;

    bool                          canWeClose() ;

  DECLARE_RESPONSE_TABLE(NSHistorizedValListWindow) ;
} ;

class NSValListHistoryDialog : public NSUtilDialog
{
  public:

    NSHistorizedValListWindow _ValList ;
    OWL::TEdit*               _pEditPath ;    OWL::TButton*             _pBtnMinus ;    OWL::TButton*             _pBtnPlus ;    string                    _sItemValue ;
    string                    _sItemPath ;
    bool                      _bEnCours ;
    string                    _sUnit ;
    string                    _sUnitSens ;

    string*                   _psNewValue ;

    string                    _sCurrentItem ;

    NSValListHistoryDialog(TWindow* pere, NSContexte* pCtx, string sItemVal, string sItemPath, string sUnit = "", string* pInsValue = NULL) ;    ~NSValListHistoryDialog() ;

    void SetupWindow() ;    void CmOk() ;
    void CmCancel() ;
    void CmMoreSpecific() ;
    void CmMoreElusive() ;
    void EvSetFocus(HWND hWndLostFocus) ;
    void EvParentNotify(uint event, uint childHandleOrX, uint childIDOrY) ;
    void EvChar(uint key, uint repeatCount, uint flags) ;

    void updatePathEdit() ;

  DECLARE_RESPONSE_TABLE(NSValListHistoryDialog) ;} ;

// -----------------------------------------------------------------------------
// Objet "Boite de dialogue" utilisé pour sélectionner un template HTML
// de présentation de document
// -----------------------------------------------------------------------------
class _NSBBCLASSE ChoixDansListeDialog : public NSUtilDialog
{
  public:

    OWL::TListBox* pListBox ;
    VecteurString* pVecteurString ;
    string         sCaption ;
    UINT           itemChoisi ;

    ChoixDansListeDialog(TWindow* pere, NSContexte* pCtx, VecteurString* pData, string caption = "") ;    ~ChoixDansListeDialog() ;

    void SetupWindow() ;    void CmSelectTemplate(WPARAM Cmd) ;
    void CmCancel() ;
    void CmOk() ;
    void CmTemplateDblClk(WPARAM Cmd) ;

    // void LanceTemplate(int NumDoc);
  DECLARE_RESPONSE_TABLE(ChoixDansListeDialog) ;} ;


// -----------------------------------------------------------------------------
// Dialogue "timer" d'enregistrement d'un document
// -----------------------------------------------------------------------------class _NSBBCLASSE NSEnregTreeDocDlg : public NSUtilDialog{ public: 	OWL::TStatic*       pTexte ;  NSEnregTreeDocDlg(TWindow* pere, NSContexte* pCtx) ;  ~NSEnregTreeDocDlg() ;  void    SetupWindow() ;  void    SetText(string sCodeDocRoot) ; DECLARE_RESPONSE_TABLE(NSEnregTreeDocDlg) ;} ;

#endif

