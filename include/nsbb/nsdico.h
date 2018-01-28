#ifndef __NSDICO_H#define __NSDICO_H

class NSTreeWindow ;
class NSTreeNode ;

// #include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\edit.h>
#include <owl\validate.h>
#include <string.h>
#include <cstring.h>

#include "nautilus\nssuper.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "nsbb\nsdicogl.h"

#define START_BOLD "*b*"
#define END_BOLD   "*/b*"

class NSEditStep
{
	public:

    NSEditStep(string sContent, int iMinSel, int iMaxSel) ;

    ~NSEditStep() ;
    NSEditStep(const NSEditStep& rv) ;
    NSEditStep& operator=(const NSEditStep& src) ;

    int    getMinSel()  { return _iMinSel ; }
    int    getMaxSel()  { return _iMaxSel ; }
    string getContent() { return _sContent ; }

    void setMinSel(int iMinSel)      { _iMinSel  = iMinSel ; }
    void setMaxSel(int iMaxSel)      { _iMaxSel  = iMaxSel ; }
    void setContent(string sContent) { _sContent = sContent ; }

    void replaceStep(string sContent, int iMinSel, int iMaxSel) ;

  protected:

		int    _iMinSel ;
    int    _iMaxSel ;
    string _sContent ;
} ;

typedef vector<NSEditStep*>  NSEditStepVector ;
typedef NSEditStepVector::iterator NSEditStepIter ;
typedef NTArray<NSEditStep>  NSEditStepArray ;

//
// Contrôle Edit avec accès au dictionnaire
//
class NSEditDico : public NSEditDicoGlobal
{
	public:

		uint	 _nMaxInput ;
   	bool 	 _bPaint ;    	  //pour ignorer les killFocus tant que
   								          //la treeview n'a pas été peinte
   	bool	 _bTexteLibre ;   //on est texte libre ou non

   	bool   _bGeneral ;      // Lexique général ou lexique médicaments

   	string _sAmmorceLibre ; // Ammorce qui a fait basculer en texte libre
                            // ce n'est pas la peine de chercher dans le
                            // lexique tant que le texte débute par elle

   	//
   	// Constructeur et destructeur
   	//
   	NSEditDico(NSContexte* pCtx, TWindow* parent, int resourceId, NSDico* pDictio, string sTypeLexique,
               const char far* text,
               int x, int y, int w, int h,
               uint     textLimit = 0,
               bool     multiline = false,
               TModule* module = 0,
               bool     bLexiqueGeneral = true);

   	//
   	// méthodes
   	//

   	void EveilleToi(string* sTexteInitial, RECT* rectEdit) ;
   	void SetupWindow() ;
   	void ElementSelectionne() ;
   	void SetPositionLexique() ;

   	void EvChar(uint key, uint repeatCount, uint flags) ;
   	void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& rect) ;
   	void EvPaint() ;

   	void EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point) ;
   	void EvKeyDown(uint key, uint repeatCount, uint flags) ;
   	void SetpTreeAEditer( NSTreeWindow*  pTree) { _pTreeAEditer = pTree ; }
   	NSTreeWindow* GetpTreeAEditer() { return _pTreeAEditer ; }
   	void UpdateDico() ;
   	void TextLibre() ;
   	void AjusterRectangleEdition() ;
   	void AjusterRectangleEdition(string sLabel) ;
    //fonctions permettant à la touche ENTREE le même effet que TAB
   	void EvKeyUp(uint key, uint repeatcount, uint flags) ;
   	uint EvGetDlgCode(MSG far*) ;
   	void EvKillFocus(THandle hWndGetFocus) ;
   	void EvSetFocus(HWND hWndLostFocus) ;
   	void EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
   	void CmEditCut() ;                   // CM_EDITCUT
   	void CmEditCopy() ;                  // CM_EDITCOPY
   	void CmEditPaste() ;                 // CM_EDITPASTE
   	void CmEditDelete() ;                // CM_EDITDELETE
   	void CmEditClear() ;                 // CM_EDITCLEAR
   	void CmEditUndo() ;                  // CM_EDITUNDO
    void CmEditSelectAll() ;
   	void Paragraphe(string* pContenu) ;
   	void EnleverRetourChariot(string* pContenu, uint* pStartSel = 0, uint* pEndSel = 0) ;
    void RemoveSoftLineBreaks(string* pContenu, uint* pStartSel = 0, uint* pEndSel = 0) ;

    void switchFromFreeToLexique(string* pContent) ;

    void setBold() ;
    void setRegular() ;

    void addToUndoBuffer() ;
    void undoOneStep() ;
    void redoOneStep() ;

    NSControle*  getControl() ;
    NSDialog*    getNSDialog() ;
    NSBBMUEView* getMUEView() ;

    void         setBeingDestroyed(bool bBD) { _bBeingDestroyed = bBD ; }

	protected :

    bool            _bKillSent ;
    bool            _bBeingDestroyed ;

		NSTreeWindow*   _pTreeAEditer ;

    NSEditStepArray _undoBuffer ;
    NSEditStepIter  _undoCursor ;

    void goToNextNode() ;
    void goToPreviousNode() ;

    bool getThreeBlocks(string& sBeforeSelection, string& sSelection, string& sAfterSelection) ;
    void setThreeBlocks(string sBeforeSelection, string sSelection, string sAfterSelection) ;

	DECLARE_RESPONSE_TABLE(NSEditDico) ;
};

#endif

