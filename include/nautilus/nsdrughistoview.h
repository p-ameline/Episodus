// --------------------------------------------------------------------------// NSDRUGVIEW.H		Document/Vues Capture Données Utilisateur
// --------------------------------------------------------------------------
// PA - Juillet 2003
// --------------------------------------------------------------------------
#if !defined(__NSDRUGHISTOVIEW_H)
#define __NSDRUGHISTOVIEW_H

#include <owl\docmanag.h>#include <owl\docview.h>
#include <owl\mdichild.h>
#include <owl\decframe.h>
#include <owl\statusba.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\applicat.h>
#include <owl\richedpr.h>
#include <owl\choosefo.h>
#include <owl\listwind.h>

#include "nautilus\nsepicap.h"

#include "nautilus\nsdocref.h"
#include "nautilus\nautilus.rh"
#include "nsbb\nsmview.h"
#include "nsbb\nslistWind.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\nsldvvar.h"

#define _NSDRUGVIEWCLASS _USERCLASS

unsigned long getDuration(NSLdvDrug* pLdvDrug) ;

class NSDrugsHistoPropertyWindow;

class NSDrugHistoView : public NSLDVView{  public:

    // Constructeur - Destructeur
    NSDrugHistoView(NSLdvDocument& doc, string sConcern = string("")) ;
    ~NSDrugHistoView() ;

    void CmOk() {}
    void CmCancel() {}

    void CmFct7() ; // change concepts
    void CmHistory() ;

    void ExecutedAfterDrugSelection() ;
    void CmPublish() ;

    // Redéfiniton des fonctions virtuelles de TView et TRichEdit
    static LPCSTR   StaticName() { return "NSDrugView" ; }
    //const char      *GetViewName() {return StaticName();}
    const char      *GetViewName() { return sViewName.c_str() ; }
    TWindow         *GetWindow();
    bool            SetDocTitle(const char far* docname, int index)
                    { return OWL::TWindow::SetDocTitle(docname, index) ; }
    void            setViewName() ;

    void            SetupColumns() ;
    void            AfficheListe() ;
    void            SetWindowPosDH() ;

    // view event handlers
    bool 	          VnIsWindow(HWND hWnd) ;

    // Fonctions de réponse aux evts
    void   EvSize(uint sizeType, ClassLib::TSize &size) ;
    void   EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void   EvRButtonDownOut(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void   EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void   EvChar(uint key, uint repeatCount, uint flags) ;
    void   EvSetFocus(HWND hWndLostFocus) ;
    void   focusFct() ;

    string donneContexte(string sContexte) ;

    void   initDrugs() ;
    void   initDrugsForConcept() ;
    void   initDrugsForAtcCode() ;

    void   reloadView(string sReason) ;

    // void						autoAddInDrugView(string sConcern, NSPatPathoArray *pPPT);
    void   autoAddInDrugView(string sConcern) ;

    bool   isSortedOnPrescribe() { return (1 == _iSortedColumn) ; }
    void   sortByPrescribe() ;

    NSLdvDrug* getDrug(size_t iIndex) ;

  protected:

    // Remplissage de la vue
    void   SetupWindow() ;
    void   SetupTxtBar() ;
    void   InitFormatFont() ;
    void   SetupToolBar() ;

    void   DispInfoListe(TLwDispInfoNotify& dispInfo) ;
    void   LVNColumnclick(TLwNotify& lwn) ;

    void   sortByName() ;
    void   sortByAdmin() ;
    void   sortByDuration() ;
    void   sortByEnding() ;
    void   sortByBegining() ;
    void   sortByPrescrEnd() ;
    void   sortByPrescrDate() ;

    string getDrugRefToModify(string sWarningMsg = string("")) ;

    void   getCodes() ;
    void   initList() ;

    std::string                 _sLexiqCode ; // Lexicon code
    std::string                 _sAtcCode ;   // ATC code

    ArrayCopyDrugs              _aDrugs ;

    // Variables
    NSLdvDocument*              _pLdVDoc ;	// Pointeur sur document EPISOD    NSDrugsHistoPropertyWindow* _pListWindow ;    OWL::TControlBar*           _pToolBar ;	        // Barre d'outils
    NSUtilLexique*              _pDrugModel ;
    NSEditBdm*                  _pAtcCode ;

    bool                        _bSetupToolBar ;

    bool                        _bNaturallySorted ;
    int                         _iSortedColumn ;

	DECLARE_RESPONSE_TABLE(NSDrugHistoView) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les médicaments à modifier)
//
class NSDrugsHistoPropertyWindow : public NSSkinableListWindow
{
	public:

		NSDrugHistoView* _pView ;    int              _iRes ;
    NSDrugsHistoPropertyWindow(NSDrugHistoView* pere, NSContexte* pCtx, int resId,                                                int x, int y, int w,                                                int h, TModule* module = 0) ;

    ~NSDrugsHistoPropertyWindow() ;    void SetupWindow() ;
    void EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void EvKeyDown(uint key, uint repeatCount, uint flags) ;    void EvLButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;    void EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& pt) ;    int  IndexItemSelect() ;

    void EvSetFocus(HWND hWndLostFocus) ;
    void EvKillFocus(HWND hWndGetFocus) ;

    void switchPrescribeImage(int iItem, bool bToPrescribe) ;

  protected:

    OWL::TImageList* _pImages ;

	DECLARE_RESPONSE_TABLE(NSDrugsHistoPropertyWindow) ;};

#endif		// __NSDRUGHISTOVIEW_H
