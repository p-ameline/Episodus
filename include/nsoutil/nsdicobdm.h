#ifndef __NSDICOBDM_H#define __NSDICOBDM_H

#include <owl\edit.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "nsoutil/nsexport.h"
#include "nssavoir/nsBdmDriver.h"
#include "nsbb\NTFunctor.h"

//
// Contrôle Edit avec accès au dictionnaire englobe NSEditDico et lié au BBItem
//
class _NSOUTILCLASSE NSEditBdm : public OWL::TEdit, public NSRoot
{
  public:

    NSBdmDriver::BAMTABLETYPE _iTableType ;
    bool                      _bOuvreDialog ;
    bool                      _bGardeFocus ;
    int                       _iPrevGardeFocus ;
    NS_CLASSLIB::TRect        _rect ;
    bool                      _bValidContent ;

    //
    // Constructeur et destructeur
    //
    NSEditBdm(NSContexte* pCtx, TWindow* parent, int resourceId,
              NSBdmDriver::BAMTABLETYPE iTableType, uint textLimit = 0,
              OWL::TModule* module = 0) ;
    NSEditBdm(NSContexte* pCtx, TWindow* parent, int resourceId,
              NSBdmDriver::BAMTABLETYPE iTableType, const char far* text,
              int x, int y, int w, int h, uint textLimit = 0,
              bool multiline = false, OWL::TModule* module = 0) ;
    virtual ~NSEditBdm() ;

    bool   WakeUp(string* sTexteInitial, RECT* rectEdit) ;

    void   ElementWasSelected() ;

    string GetSelectedLabel() ;
    string GetSelectedCode() ;

    void   SetupWindow() ;
    void   EvKillFocus(THandle hWndGetFocus) ;
    void   EvSetFocus(HWND hWndLostFocus) ;

    void   EvChar(uint key, uint repeatCount, uint flags) ;
    void   EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void   UpdateDico() ;
    uint   EvGetDlgCode(MSG far* msg) ;

    void   SleepNow() ;
    bool   canWeClose() ;

    bool   isEmpty() ;

    void   InsererElementLexique() ;

    void   AcceptToLooseFocus() { _bYouMustAcceptToLooseFocus = true ; }

    NSBdmDriver::BAMTABLETYPE getTableType() { return _iTableType ; }

    bool   isGardeFocus()             { return _bGardeFocus ; }
    void   setGardeFocus(bool bGF)    { _bGardeFocus = bGF ; }

    void   setRect(NS_CLASSLIB::TRect rect) { _rect = rect ; }

    string getLabel() { return _sSelectedLabel ; }
    string getCode()  { return _sSelectedCode ;  }

    void   injectCode(string sCode) { _sSelectedCode = sCode ;  }

    string getTextAsString() ;

    void   setLostFocusFunctor(Functor* func) { _pLostFocusFunctor = func ; }
    void   setGetCodeFunctor(Functor* func)   { _pGotCodeFunctor = func ; }

  protected :

    Functor* _pGotCodeFunctor ;   // Function to execute when an element is selected
    Functor* _pLostFocusFunctor ; // Function to execute when losing focus

    bool     _bYouMustAcceptToLooseFocus ;

    string   _sSelectedLabel ;
    string   _sSelectedCode ;

    bool     _bWinStd ;

    void    goToNextNode() ;
    void    goToPreviousNode() ;

    ChoixBdmDialog* getBdmDialog() ;
    void            SetPositionLexique() ;

  DECLARE_RESPONSE_TABLE(NSEditBdm) ;
};

#endif      // __NSDICOBDM_H