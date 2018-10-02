#ifndef __NSUTIDLG_H#define __NSUTIDLG_H

#include <owl\dialog.h>
#include <owl\control.h>

#include "partage\ns_vector.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "ns_sgbd\nsannexe.h"
#include "nsbb\nsExport.h"

typedef vector<OWL::TControl*>           NSVecteurControl ;typedef NSVecteurControl::iterator       NSControlIter ;
typedef NSVecteurControl::const_iterator NSControlConstIter ;

class NSVectControl : public NSVecteurControl
{
	public :

		TResId	      ressource ;
    TWindow*      AParent ;
    OWL::TModule* module ;

		//    // Constructeurs
    //
    NSVectControl() ;
		NSVectControl(const NSVectControl& rv) ;

    //		// Destructeur
    //
		virtual ~NSVectControl() ;

    void vider() ;    NSVectControl& operator=(const NSVectControl& src) ;
};

#ifdef _INCLUSclass NSRoot ;
class NSContexte ;
#endif

#ifdef _INCLUSclass _NSBBCLASSE NSDialogWrapper : public TDialog, public NSRoot
{
	public:

		NSDialogWrapper(NSContexte* pCtx, TWindow* parent, TResId resId, TModule* module = 0) ;
    void GetWindowClass(WNDCLASS& wndClass) ;    char far* GetClassName() ;
    bool Register() ;
    void RegisterChildObjects() ;
    int  Execute() ;

    NSContexte* pContexte ;
    TModule*    pModule ;
};
#endif // _INCLUS

#ifdef _INCLUS
class _NSBBCLASSE NSUtilDialog : public NSDialogWrapper
#else
class _NSBBCLASSE NSUtilDialog : public TDialog, public NSRoot
#endif
{
	public:

		NSVectControl* pNSVectControl ;
    TResId		     ressource ;
    TWindow* 	     AParent ;

    string 		     sHindex, sHcorps ;

    HWND           hWndResult ;
    int            iIdToFind ;

    // Constructeur et destructeur
    NSUtilDialog(TWindow* parent, NSContexte* pCtx, TResId resID, OWL::TModule* module = 0) ;
		virtual ~NSUtilDialog() ;
		NSUtilDialog(const NSUtilDialog& src) ;
    // Surcharge de l'opérateur d'affectation
    NSUtilDialog& operator=(const NSUtilDialog& src) ;

    virtual void CmOk() ;
    virtual void CmCancel() ;
    virtual void CmHelp() ;
    virtual void EvSetFocus(HWND hWndLostFocus) ;
    TWindow*     ChildWithId(int id) ;
    TWindow*     ChildWithHandle(HWND hWnd) ;

    void EvKeyDown(uint key, uint repeatCount, uint flags) ;    void ReferenceControl(OWL::TControl* pTControl) ;
    void SearchControls() ;    void ActiveControlSuivant(OWL::TControl* pTControle) ;    bool ValidActiveControl(OWL::TControl *pControl) ;    bool SelectTextActiveControl(OWL::TControl* pTControle) ;    //   static int FAR PASCAL EnumerWindows(HWND hWnd, LPARAM lParam);
    bool PreProcessMsg(MSG &msg) ;
    void setTextForControl(int iControlId, string sTextChapter, string sTextId) ;    void killControlFocusOnReturnClose() ;    void GetWindowClass(WNDCLASS& wndClass) ;    char far* GetClassName() ;    OWL::TApplication* GetApplication() ;	protected:    HACCEL hAccelTable ;	DECLARE_RESPONSE_TABLE(NSUtilDialog) ;
};

#endif

