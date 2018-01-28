#ifndef __NSGROUPE_H#define __NSGROUPE_H

// #include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\groupbox.h>

#include "nsbb\nsbb_dlg.h"
#include "nsbb\nsbbtran.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

class NSGroupBox : public TGroupBox, public NSRoot
{
  public:

    NSControle* pControle ;
    // Constructeur et destructeur
    NSGroupBox(NSContexte *pCtx, TWindow* parent, int resId) ;
    NSGroupBox(NSContexte *pCtx, TWindow* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module = 0) ;    ~NSGroupBox() ;

    uint    Transferer(TTransferDirection direction, CTRL_ACTIVITY* pActif, Message* pMessage = 0) ;
    void    activeControle(int activation, Message* pMessage = 0) ;
    void    SetupWindow() ;

    void    SelectionChanged(int controlId) ;

    WNDTYPE donneType() { return(isGroup) ; }

    // child id notification handled at the child
    //
    void    BNClicked() ;  // BN_CLICKED
    bool    canWeClose() ;

  DECLARE_RESPONSE_TABLE(NSGroupBox) ;
};

#endif
