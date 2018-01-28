// -----------------------------------------------------------------------------
// nsprocessview.h
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 16:45:28 $
// -----------------------------------------------------------------------------
// gestion des process
// -----------------------------------------------------------------------------
// FLP - aout 2003
// -----------------------------------------------------------------------------

#ifndef __NSPROCESSVIEW_H__
# define __NSPROCESSVIEW_H__

# include <owl\controlb.h>
# include <owl\listwind.h>

# include "nautilus\nsdocref.h"
# include "nsbb\nsmview.h"
# include "nautilus\nsldvdoc.h"

# include "nautilus\nsprocess.h"

class NSProcessesPropertyWindow ;

class NSProcessView : public NSMUEView{ public:
  // Constructeur - Destructeur
  NSProcessView(NSLdvDocument& doc, string sConcern = "") ;
  ~NSProcessView() ;

  // fonctions
  void                      CmNouveau() ;
  void                      CmEtape() ;
  void                      CmModifier() ;
  void                      CmArreter() ;
  void                      CmOrdonnance() ;

  void CmOk() {}
  void CmCancel() {}

  // Redéfiniton des fonctions virtuelles de TView et TRichEdit
  static LPCSTR             StaticName()        { return "NSProcessView" ; }
  //const char                *GetViewName()      { return StaticName() ; }
  const char                *GetViewName() { return sViewName.c_str() ; }
  TWindow                   *GetWindow() ;
  bool                      SetDocTitle(const char far *docname, int index)
  {
    return OWL::TWindow::SetDocTitle(docname, index) ;
  }
	void                      setViewName() ;

  void                      SetupColumns() ;
  void                      AfficheListe() ;
  void                      EvSetFocus(HWND hWndLostFocus) ;

  string                    donneContexte(string sContexte) ;
  void                      initCurentProcesses() ;
  void 											initProcessFromPatho(char* sNode, NSProcess *pNewProcess,  NSPatPathoArray *pTmpPPT, string metaNode) ;

  void                      displayChanges() ;

  // variables
  ArrayProcesses            aCurrentProcesses ;


 protected:
  // Remplissage de la vue
  void 	                    SetupWindow() ;
  void                      SetupTxtBar() ;
  void                      InitFormatFont() ;
  void                      SetupToolBar() ;

  void 	                    DispInfoListe(TLwDispInfoNotify& dispInfo) ;

  // view event handlers
  bool 	                    VnIsWindow(HWND hWnd) ;

  // Fonctions de réponse aux evts
  void                      EvSize(uint sizeType, ClassLib::TSize &size) ;

  void                      EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  void                      EvChar(uint key, uint repeatCount, uint flags) ;

  // Variables
  NSLdvDocument             *pLdVDoc ;	        // Pointeur sur document EPISOD  NSProcessesPropertyWindow *pListeWindow ;  OWL::TControlBar          *pToolBar ;	        // Barre d'outils

  bool	                    bSetupToolBar ;

  DECLARE_RESPONSE_TABLE(NSProcessView) ;
} ;

// -----------------------------------------------------------------------------
// Objet "ListWindow" avec gestion du double-click (pour les process à modifier)
// -----------------------------------------------------------------------------
class NSProcessesPropertyWindow : public TListWindow
{
    public:

        NSProcessView*  pView ;
        int             iRes ;

        NSProcessesPropertyWindow(NSProcessView *pere, int resId, int x, int y, int w, int h, TModule *module = 0) ;
        ~NSProcessesPropertyWindow() {}

        void    SetupWindow() ;

        void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;        void    EvKeyDown(uint key, uint repeatCount, uint flags) ;        int     IndexItemSelect() ;

        void    EvSetFocus(HWND hWndLostFocus) ;

    DECLARE_RESPONSE_TABLE(NSProcessesPropertyWindow) ;} ;

#endif // __NSPROCESSVIEW_H__

