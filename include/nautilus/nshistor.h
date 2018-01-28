#if !defined(__NSHISTOR_H)#define 		 __NSHISTOR_H

#include <owl/treewind.h>
#include <winuser.h>
#include <owl/mdichild.h>
#include <owl/commctrl.h>
#include <owl/imagelst.h>
#include <owl/doctpl.h>

#include "nautilus\nshistdo.h"
#include "partage\ns_vector.h"
#include "nautilus\nstrihis.h"

//
// modéle document/vues
//
class NsHistorique : public TView, public NSTreeHistorique
{
	public:

		bool             bSetupToolBar ;
		NSHISTODocument* pDoc ;			// Pointeur sur document histo
    TMenuDescr       *pMUEViewMenu ;
    HACCEL           hAccelerator ;

		NsHistorique(NSHISTODocument& doc,  TWindow* parent = 0) ;		~NsHistorique() ;

		//		//méthodes
		//
		void           SetupWindow() ;
		void           AfficheDocument() ;
		static LPCSTR  StaticName() { return "NsHistorique" ; }
		const char*    GetViewName(){ return StaticName() ; }
		TWindow*       GetWindow() ;
		void           SetupHistoBar() ;

		void Rafraichir(NSDocumentInfo* pNSDocumentInfo, const NSPatPathoArray* pNSPatPathoArray,                        NSNoyauDocument* pNouveauDocument = 0) ;
		void EnleverDocument(NSDocumentInfo* pNSDocumentInfo) ;
		void AjouterDocument(NSDocumentInfo* pNSDocumentInfo) ;
		void VisualiserPatho(NSDocumentInfo* pNSDocumentInfo, const NSPatPathoArray* pNSPatPathoArray) ;
		void AutoriserOuverture(NSDocumentInfo* pDocument) ;
		void AutoriserEdition(NSDocumentInfo* pDocument) ;
		void SetWindowPosHisto() ;  //fixer la position de la fiche historique
		void EnregistrePosHisto() ; //enregistrer la position de la fiche historique

		void Cm0Plus() ;
		void Cm1Plus() ;
		void Cm2Plus() ;
		void Cm3Plus() ;
		void Cm4Plus() ;

		void CmPublier() ;

		void Ce0Plus(TCommandEnabler&) ;
		void Ce1Plus(TCommandEnabler&) ;
		void Ce2Plus(TCommandEnabler&) ;
		void Ce3Plus(TCommandEnabler&) ;
		void Ce4Plus(TCommandEnabler&) ;

		void CeFileClose(TCommandEnabler&) ;
		// void EvSysCommand(uint cmdType, NS_CLASSLIB::TPoint&) ;

		bool CanClose() ;		void EvClose() ;
		void EvDestroy() ;
		void EvSetFocus(THandle hWndLostFocus /* may be 0 */) ;
		void FermetureDocument(NSDocumentInfo* pDocumentInfo) ;
		// Document notifications
		bool VnIsWindow(HWND hWnd) { return HWindow == hWnd ; }

    // Overrides TWindows' in order to process our accelerators table
    bool PreProcessMsg(MSG &msg) ;

		// Fonctions pour générer un NSHtmlCS à partir de l'historique
		void InscrireHtml(NSHtml* pHtml) ;
		void InscrireFils(NSTreeNode* pNode, NSHtml* pHtml) ;

    void initMUEViewMenu(string sMenuName) ;
    void activateMUEViewMenu() ;

	DECLARE_RESPONSE_TABLE(NsHistorique) ;
};

#endif
