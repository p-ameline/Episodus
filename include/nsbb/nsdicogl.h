#ifndef __NSDICOGL_H#define __NSDICOGL_H

class NSDico ;

// #include <owl\owlpch.h>
// #include <owl\applicat.h>
#include <owl\edit.h>
// #include "owl\validate.h"
// #include <string.h>
// #include <cstring.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "nsbb\nsbb_glo.h"
#include "nsbb\nsExport.h"

//
// Contrôle Edit avec accès au dictionnaire englobe NSEditDico et lié au BBItem
//
class _NSBBCLASSE NSEditDicoGlobal : public OWL::TEdit, public NSRoot
{
  public:

    NSDico*            _pDico ;
    bool               _bOuvreDialog ;
    bool               _bGardeFocus ;
    int                _iPrevGardeFocus ;
    NS_CLASSLIB::TRect _rect ;
    string             _sTypeLexique ; //base des médicaments ou lexique général
    bool               _bValidContent ;

    //
    // Constructeur et destructeur
    //
    NSEditDicoGlobal(NSContexte* pCtx, TWindow* parent, int resourceId, NSDico* pDictio, string sTypeLexique = "", uint textLimit = 0, OWL::TModule* module = 0);
    NSEditDicoGlobal(NSContexte* pCtx, TWindow* parent, int resourceId, NSDico* pDictio, const char far* text,
          		            int x, int y, int w, int h,
          		            uint     textLimit = 0,
          		            bool     multiline = false,
                            string sTypeLexique = "",
                            OWL::TModule* module = 0);
    virtual ~NSEditDicoGlobal() ;

    bool    EveilleToi(string* sTexteInitial, RECT* rectEdit) ;
    void    SetDico(NSDico* pDictio) ;
    void    GetLabelChoisi(string* sChaineRecuperee) ;
    void    SetupWindow() ;
    void    EvKillFocus(THandle hWndGetFocus) ;
    void    EvSetFocus(HWND hWndLostFocus) ;
    void    GetCodeLexiqueChoisi(char* code) ;
    void    EndortToi() ;
    bool    canWeClose() ;

    string  getRawText() ;

    bool    isEmpty() ;

    void    AcceptToLooseFocus() { _bYouMustAcceptToLooseFocus = true ; }

    NSDico* getDico()                  { return _pDico ; }

    string  getTypeLexique()           { return _sTypeLexique ; }
    void    setTypeLexique(string sTL) { _sTypeLexique = sTL ; }

    bool    isGardeFocus()             { return _bGardeFocus ; }
    void    setGardeFocus(bool bGF)    { _bGardeFocus = bGF ; }

    void    setRect(NS_CLASSLIB::TRect rect) { _rect = rect ; }

    virtual void ElementSelectionne() = 0 ;

  protected :

    bool _bYouMustAcceptToLooseFocus ;

    void    goToNextNode() ;
    void    goToPreviousNode() ;

  DECLARE_RESPONSE_TABLE(NSEditDicoGlobal) ;
};

#endif