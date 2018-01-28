// nsvisual.h : Objet de visualisation des fichiers html statiques - RS Fevrier 1998
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(__NSVISUAL_H)
#define 		 __NSVISUAL_H

#include "nautilus\nsdocref.h"

//---------------------------------------------------------------------------
//
// Helper classes for toolbar edit box:
//
//---------------------------------------------------------------------------

class _OWLCLASS TEditGadget: public TControlGadget
{
  public:
    TEditGadget(TWindow& control, TBorderStyle style = None): TControlGadget(control, style){}
   ~TEditGadget(){}
	 virtual void CommandEnable();
};

class TEditGadgetEnabler : public TCommandEnabler
{
  public:
	 TEditGadgetEnabler(TWindow::THandle hReceiver, TEditGadget* g)
    : TCommandEnabler(g->GetControl()->GetId(), hReceiver), Gadget(g){}
	 void  SetText(const char far*);
	 void SetCheck(int){}
  protected:
	 TEditGadget*  Gadget;
};


class TNotifyEdit: public OWL::TEdit
{
public:
    TNotifyEdit(TWindow* parent, int id, const char far* text,
    			int x, int y, int w, int h, uint textLimit = 0,
          	bool multiline = false, TModule* module = 0):
            	OWL::TEdit(parent, id, text, x, y, w, h, textLimit,
               multiline, module){}
	 static TAPointer<char> text;
    static const char* Text();
    void EvKeyDown(uint key, uint repeatCount, uint flags);
  DECLARE_RESPONSE_TABLE(TNotifyEdit);
};

//
// Définition de la classe NSVisualView : Affichage des documents dans une window
//

class NSVisualView;

class NSEditUrl : public OWL::TEdit
{
	public :
   	NSVisualView* pVue;

   	NSEditUrl(NSVisualView* pView);
      ~NSEditUrl();

      void EvChar(uint key, uint repeatCount, uint flags);

		DECLARE_RESPONSE_TABLE(NSEditUrl);
};

// note : la classe document est NSNoyauDocument

class NSVisualView : public TView, public TOleWindow
{
	public :
   	// Données de visualisation
      NSRefDocument*			pDocRef;		// pointeur sur document de référence
      string 					sUrl; 		// url de référence
      string 					sHtml;     	// fichier html à visualiser
      TOcControl* 			Ocx;			// controle ocx
      TIWebBrowser2Proxy 	Control;    // Objet proxy WebBrowser
      NSEditUrl*				pEditUrl;   // Pointeur sur le champ Edit d'Url
      bool						bSetupToolBar; // booleen pour gérer la ToolBar
      bool						bNewNav;		// booleen pour tenir compte des nav par liens
      int						nbNav;      // compteur des navigations effectuées
      int						page;			// compteur de pages

   	NSVisualView(NSRefDocument& doc, TWindow *parent = 0);
      ~NSVisualView();

      // Redéfiniton des fonctions virtuelles de TView
      static LPCSTR		StaticName() { return "VisualView"; }
      const char*			GetViewName() { return StaticName(); }
      TWindow*				GetWindow();
      bool					CanClose();

      // View notifications
    	bool 					VnInvalidate(NS_CLASSLIB::TRect& rect);
    	bool 					VnDocOpened(int omode);
    	bool 					VnDocClosed(int omode);

    	// bool EvOcViewPartInvalid(TOcPartChangeInfo& changeInfo);

      // SetDocTitle et GetFileName
		bool 					SetDocTitle(const char far* docname, int index)
  		{ return TWindow::SetDocTitle(docname, index); }

		const char far* 	GetFileName()
  		{ return GetDocument().GetTitle(); }

      // Redéfinition de CreateOcView pour transmettre les liens document/vue
      TOcView* CreateOcView(TRegLink* link, bool isEmbedded, IUnknown* outer);

      // EvClose
      void     EvClose();

      // Redéfinition de EvOcViewGetSiteRect et EvOcViewSetSiteRect (cf nsie4)
      bool		EvOcViewGetSiteRect(NS_CLASSLIB::TRect* rect);
      bool 		EvOcViewSetSiteRect(NS_CLASSLIB::TRect far* rect);

      bool		EvOcCtrlCustomEvent(TCtrlCustomEvent* pev);

      // Fonctions de la VisualView
      void 	SetupWindow();
      void  SetupNavBar();
      void  SetSizeOcx(NS_CLASSLIB::TRect newRect);
      void 	EvSize(uint sizeType, NS_CLASSLIB::TSize& size);
      void 	EvSetFocus(THandle hWndLostFocus);
      void 	EvKillFocus(THandle hWndGetFocus);
      void  Navigate(string sUrlTarget);
      void	NavigateComplete2(IDispatch* pDisp, TAutoVal URL);
      bool  GenereHtml();
      void  CmPublier();
      void  DetruireTemp();
      void  SetConnectionPoint();
      void  CmFileClose();
      void  CmPrecedent();
      void  CmSuivant();

      // Document notifications
    	bool VnIsWindow(HWND hWnd) { return hWnd == GetHandle() || IsChild(hWnd); }

      DECLARE_RESPONSE_TABLE(NSVisualView);
};

// Déclaration de la template associée au modèle document/vue visualisation
typedef TDocTemplateT<NSRefDocument, NSVisualView> VisualViewTmpl;

#endif		// __NSVISUAL_H
