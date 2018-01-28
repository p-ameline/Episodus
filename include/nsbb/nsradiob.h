#ifndef __NSRADIOB_H#define __NSRADIOB_H

class Message ;

// #include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\radiobut.h>

// #include "nsbb\nsbb_dlg.h"
#include "nsbb\nsbbtran.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif
#include "nsbb\nsbb_glo.h"

class _NSBBCLASSE NSRadioButton : public OWL::TRadioButton, public NSRoot
{
	public:

		NSControle* pControle ;

    bool ActiveToi ;	//activer physiquement le controle ou juste lui donner le focus
    bool bTitre ;    // Le titre dépendant de la langue a-t-il déjà été donné

    // Constructeur et destructeur
    NSRadioButton(NSContexte *pCtx, TWindow* parent, int resId, OWL::TGroupBox* group) ;
    NSRadioButton(NSContexte *pCtx, TWindow* parent, int resId, const char far* title, int x,
                      int y, int w, int h, OWL::TGroupBox *group = 0,
                      TModule* module = 0) ;
    ~NSRadioButton() ;

    void        GetWindowClass(WNDCLASS far& wc) ;
    char far*   GetClassName() ;
    void        SetupWindow() ;
    bool        canWeClose() ;
    void        EvKeyUp(uint key, uint repeatcount, uint flags) ;

    bool        isEmpty() { return _bIsEmpty ; }

    uint        Transferer(TTransferDirection direction, CTRL_ACTIVITY* pActif, Message *pMessage = 0) ;
    uint        TempTransferer(CTRL_ACTIVITY* pActif, Message* pMessage = 0) ;
    void        activeControle(int activation, Message *pMessage = 0) { SetCheck(activation) ; }

    WNDTYPE     donneType() { return (isRadioBtn) ; }
    // child id notification handled at the child
    //
    void        BNClicked() ;  // BN_CLICKED

  protected:

    bool _bIsEmpty ; // Never been activated (checked or unchecked) by user

	DECLARE_RESPONSE_TABLE(NSRadioButton) ;
};

#endif

