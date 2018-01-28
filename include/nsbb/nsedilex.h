#ifndef __NSEDILEX_H#define __NSEDILEX_H

// #include <owl\owlpch.h>
#include <owl\applicat.h>
#include "owl\validate.h"
#include <string.h>
#include <cstring.h>
#include "nsbb\nsdicogl.h"
#include "nsbb\nsbb_dlg.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsExport.h"

//
//
//
class NSEditLexiqueDerive ;
class NSContexte ;

class _NSBBCLASSE NSEditLexique : public NSEditDicoGlobal
{
  public:

    NSControle*          pControle ;
    string               sContenuPere ;
    NSEditLexiqueDerive* pNSEdit ;       //fils fictif
    string               sComplement ;

    bool                 bWinStd ;       // Gestion standard windows

    //
    // Constructeur et destructeur
    //
    NSEditLexique(TWindow* parent, NSContexte* pCtx, int resourceId,
                      NSDico* pDictio, uint textLimit = 0,
                      string sTypeLexique = "", TModule* module = 0) ;
    NSEditLexique(TWindow* parent, NSContexte* pCtx, int resourceId,
                      NSDico* pDictio, const char far* text,
                      int x, int y, int w, int h, uint textLimit = 0,
                      bool multiline = false, string sTypeLexique = "",
                      TModule* module = 0) ;

    ~NSEditLexique() ;
    //
    // méthodes
    //
    void SetupWindow() ;
    void UpdateDico() ;
    void SetPositionLexique() ;
    void ElementSelectionne() ;

    void EvChar(uint key, uint repeatCount, uint flags) ;
    void EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void EvKeyUp(uint key, uint repeatcount, uint flags) ;
    void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void CmEditPaste() ;
    uint EvGetDlgCode(MSG far* ) ;

    void EvKillFocus(THandle hWndGetFocus) ;
    void EvSetFocus(HWND hWndLostFocus) ;

    WNDTYPE donneType() { return (isEditLexique) ; }
    uint Transferer(TTransferDirection direction, CTRL_ACTIVITY* pActif, Message* pMessage = 0) ;
    uint TempTransferer(CTRL_ACTIVITY* pActif, Message* pMessage = 0) ;
    void activeControle(int activation, Message* pMessage = 0) ;

    void CreerEditDerive(string sCode, string sContenu) ;
    void CreerBBItem() ;

    bool isFreeTextEnabled()         { return _bIsFreeTextEnabled ; }
    void setFreeTextEnabled(bool bT) { _bIsFreeTextEnabled = bT ; }

  protected:

    string getTextAsString() ;

    bool _bIsFreeTextEnabled ;

  DECLARE_RESPONSE_TABLE(NSEditLexique) ;
};

////classe du controle fictif crée en choisissant un élément dans le lexique
//

class NSComboSemantique ;

class NSEditLexiqueDerive : public NSRoot
{
  public:

    string             sContenuTransfert ; //label dans le champ edit
    NSControle*        pControle ;
    string             sIdentite ; //code lexique
    NSContexte*        pContexte ;
    string             sComplement ;
    NSEditLexique*     pEditPere ;
    NSComboSemantique* pComboPere ;
    //
    // Constructeur et destructeur
    //
    NSEditLexiqueDerive(NSContexte* pCtx, NSEditLexique* pEdit) ;
    NSEditLexiqueDerive(NSContexte* pCtx, NSComboSemantique* pCombo) ;
    ~NSEditLexiqueDerive() ;
    //
    // méthodes
    //
    WNDTYPE      donneType() { return(isEditLexiqueDerive) ; }
    virtual uint Transferer(TTransferDirection direction, CTRL_ACTIVITY* pActif, Message* pMessage = 0) ;
            uint TempTransferer(CTRL_ACTIVITY* pActif, Message* pMessage = 0) ;
            void activeControle(int activation, Message* pMessage = 0) ;
            bool canWeClose() ;
};

#endif

