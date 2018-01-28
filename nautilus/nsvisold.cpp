// nsvisual.cpp : Objet de visualisation des fichiers html statiques - RS Fevrier 1998
////////////////////////////////////////////////////////////////////////////////////////

#define __NSVISUAL_CPP

#include "nautilus\nsvisual.h"
// #include "nautilus\ns_html.h"
#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nautilus\nautilus.rh" 	// pour l'id de commande CM_IMPRIME
#include "nautilus\nscompub.h"
// #include "nautilus\nsresour.h"   // pour TDIEImport
#include <string.h>
#include <winsys\geometry.h>
#include <owl\oleview.h>
#include <owl\docview.rh>

const TString progIdIE = "Shell.Explorer"; // Controle OCX (Version independant progid)
GUID  guidIE = CLSID_NULL;
const TString progIdAppIE = "InternetExplorer.Application.1";
GUID  guidAppIE = CLSID_NULL;

//---------------------------------------------------------------------------
//
//Helper classes for toolbar edit box:
//
//---------------------------------------------------------------------------

void  TEditGadgetEnabler::SetText(const char far* txt)
{
   char oldtxt[256];
	TEdit* edit = dynamic_cast<TEdit*>(Gadget->GetControl());
   edit->GetText(oldtxt, 256);
   if(strcmp(txt, oldtxt) != 0)
   	edit->SetText(txt);
}


void TEditGadget::CommandEnable()
{
	if (Window->GetHandle() && (GetFocus() != GetControl()->HWindow))
		Window->HandleMessage(WM_COMMAND_ENABLE, 0, TParam2(&TEditGadgetEnabler(*Window, this)));
}


DEFINE_RESPONSE_TABLE1(TNotifyEdit, TEdit)
	EV_WM_KEYDOWN,
END_RESPONSE_TABLE;


void TNotifyEdit::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if(key == VK_RETURN)
   {
      text = new char[GetTextLen() + 1];
      GetText(text, GetTextLen() + 1);
   	Parent->HandleMessage(WM_COMMAND, GetId(), 0);
   }
   else
   	TEdit::EvKeyDown(key, repeatCount, flags);
}

TAPointer<char> TNotifyEdit::text = 0;

const char* TNotifyEdit::Text()
{
	return (const char*)text;
}

// Classe NSEditUrl pour la barre d'edition des url
////////////////////////////////////////////////////////////////

DEFINE_RESPONSE_TABLE1(NSEditUrl, TEdit)
	EV_WM_CHAR,
END_RESPONSE_TABLE;

NSEditUrl::NSEditUrl(NSVisualView* pView) : TEdit(0, 751, "", 0, 0, 300, 24, 0)
{
	pVue = pView;
}

NSEditUrl::~NSEditUrl()
{
}

void
NSEditUrl::EvChar(uint key, uint repeatCount, uint flags)
{
	char far url[200];

	if (key == VK_RETURN)
   {
   	GetText(url,200);
      pVue->Navigate(string(url));
   }
   else
   	TEdit::EvChar(key,repeatCount,flags);
}

// Table de réponses de la classe NSVisualView
////////////////////////////////////////////////////////////////
DEFINE_RESPONSE_TABLE2(NSVisualView, TView, TOleWindow)
  EV_OC_VIEWSETSITERECT,
  EV_OC_VIEWGETSITERECT,

  EV_VN_ISWINDOW,

  // Container specific messages
  EV_VN_INVALIDATE,
  EV_VN_DOCOPENED,
  EV_VN_DOCCLOSED,

  EV_WM_CLOSE,
  EV_WM_SIZE,
  EV_WM_SETFOCUS,
  EV_WM_KILLFOCUS,

  EV_COMMAND(CM_IMPRIME, CmPublier),
  EV_COMMAND(CM_FILECLOSE, EvClose),
  EV_COMMAND(CM_PRECEDENT, CmPrecedent),
  EV_COMMAND(CM_SUIVANT, CmSuivant),
END_RESPONSE_TABLE;

// Constructeur NSVisualView
////////////////////////////////////////////////////////////////
NSVisualView::NSVisualView(NSRefDocument& doc, TWindow *parent) :
	TView(doc),pDocRef(&doc),TOleWindow(parent)
{
	SetViewMenu(new TMenuDescr(IDM_MDICMNDS));

   if (!pDocRef->nbImpress)
   {
		if (pDocRef->sTemplate != "")	// le document a une template associée
   	{
   		// on récupère l'url du serveur nautilus
   		sUrl = pDocRef->pSuper->PathName("UHTM");
      	// on génère le fichier à visualiser
   		GenereHtml();
   	}
   	else
   	{
   		// l'url est contenue dans le champ fichier du document
   		sUrl = string("http://") + string(pDocRef->pDocInfo->pDonnees->fichier);
      	sHtml = "";
   	}
   }
   else
   {
		sUrl = pDocRef->pSuper->PathName("UHTM");
      sHtml = pDocRef->sNomDocHtml;
   }

   bSetupToolBar = true;
   bNewNav = true;
   nbNav = 0;
   page = 0;
}

// Destructeur NSVisualView
////////////////////////////////////////////////////////////////
NSVisualView::~NSVisualView()
{
	// !! Ne pas deleter pEditUrl (déjà fait par FlushControlBar)
   DetruireTemp();
}

// GetWindow renvoie this
////////////////////////////////////////////////////////////////
TWindow*
NSVisualView::GetWindow()
{
   return (TWindow*) this;
}

// Fonction CanClose pour détruire la barre d'outils
////////////////////////////////////////////////////////////////
bool
NSVisualView::CanClose()
{
	TMyApp* pMyApp = pDocRef->pSuper->pNSApplication;
   pMyApp->FlushControlBar();
   bSetupToolBar = false;

   // unbind du browser
	if (Control.IsBound())
   {
   	Control.Unbind();
   }

   return TOleWindow::CanClose();
}

//
// Override TOleWindow's version to pass info to TOleDocument & provide a
// second chance to find the RegLink.
//
TOcView*
NSVisualView::CreateOcView(TRegLink* link, bool isRemote, IUnknown* outer)
{
  // Assume an embedded document until we find out later if we are a link
  // or a load-from-file
  //
  if (isRemote)
    GetDocument().SetEmbedded(true);

  return TOleWindow::CreateOcView(link ? link : GetDocument().GetTemplate(),
                                  isRemote, outer);
}

void
NSVisualView::SetupNavBar()
{
	TMyApp* pMyApp = pDocRef->pSuper->pNSApplication;
   pMyApp->FlushControlBar();

   pEditUrl = new NSEditUrl(this);

   pMyApp->cb2->Insert(*new TButtonGadget(CM_PRECEDENT, CM_PRECEDENT, TButtonGadget::Command));
  	pMyApp->cb2->Insert(*new TSeparatorGadget);
   pMyApp->cb2->Insert(*new TEditGadget(*pEditUrl));
  	pMyApp->cb2->Insert(*new TSeparatorGadget);
   pMyApp->cb2->Insert(*new TButtonGadget(CM_SUIVANT, CM_SUIVANT, TButtonGadget::Command));

   pMyApp->cb2->LayoutSession();
}

// Fonction SetupWindow
////////////////////////////////////////////////////////////////
void
NSVisualView::SetupWindow()
{
	NS_CLASSLIB::TRect rectClient;

	TOleWindow::SetupWindow();

   // on récupère les coordonnées de la zone client de la TMDIChild
   rectClient = Parent->GetClientRect();

   // on insère le controle ocx internet explorer
   // par le progid : Ocx = InsertControl(progIdIE);
   ::CLSIDFromProgID(TString(progIdIE), &guidIE);

   // GUID iidControl = IID_CoWebBrowser;
   Ocx = InsertControl(guidIE);
   SetSizeOcx(rectClient);

	Control.Bind(Ocx->GetCtlDispatch());

   if (sHtml != "")
   	SetDocTitle(pDocRef->GetTitle(),0);

   // on navigue vers le fichier html à visualiser
   Navigate(sUrl + sHtml);
}

void
NSVisualView::NavigateComplete2(IDispatch* pDisp, TAutoVal URL)
{
	TAutoString autoUrl;
	char far 	url[300];

   autoUrl = Control.LocationURL();
   strcpy(url,autoUrl.operator const char far*());

   pEditUrl->SetText(url);

   // on incrémente ici nbNav et non dans Navigate
   // pour tenir compte des navigations par liens
   if (bNewNav)
   	nbNav = ++page;

   bNewNav = true;

   if (pDocRef->nbImpress)
   	CmPublier();
}

bool
NSVisualView::EvOcCtrlCustomEvent(TCtrlCustomEvent* pev)
{
	switch(pev->Args->DispId)
	{
		case 252:
		{
			IDispatch* arg0 = pev->Args->operator[](0);
			TAutoVal arg1 = pev->Args->operator[](1);
			NavigateComplete2(arg0, arg1);
			break;
		}
	}
	return true;
}

bool
NSVisualView::EvOcViewGetSiteRect(NS_CLASSLIB::TRect* rect)
{
	// Pour l'instant on fait comme avant
   return TOleWindow::EvOcViewGetSiteRect(rect);

   // *rect = GetClientRect();
   // return true;
}

// prevent the control from changing its size:
bool
NSVisualView::EvOcViewSetSiteRect(NS_CLASSLIB::TRect far* rect)
{
  // Pour l'instant on fait comme avant
  return TOleWindow::EvOcViewSetSiteRect(rect);

  // return false;
}

// Fonction SetSizeOcx(TRect newRect)
// Fixe la taille du controle donnée en unités logiques
// Transmet cette taille à l'objet d'interface après conversion
// Cette routine remplace TOcPart::UpdateRect pour pouvoir fixer la taille
///////////////////////////////////////////////////////////////////////////
void
NSVisualView::SetSizeOcx(NS_CLASSLIB::TRect newRect)
{
   NS_CLASSLIB::TRect rect(newRect);
   NS_CLASSLIB::TSize newSize;

   // On vérifie que la vue OC est bien en place
   // (ce qui est fait par TOleWindow::SetupWindow)
   if (!OcView)
   	return;

   Ocx->SetPos(rect.TopLeft());
	Ocx->SetSize(rect.Size());

   // conversion en unités physiques (dépend de TOleScaleFactor)
   // On appelle directement EvOcViewGetSiteRect
   // car le container host est notre TOleWindow
   EvOcViewGetSiteRect(&rect);
   newSize = rect.Size();

   // On met à jour l'élément d'interface
   if (Ocx->GetBPartI())
   {
   	Ocx->GetBPartI()->SetPartPos(&rect);
   	Ocx->GetBPartI()->SetPartSize(&newSize);
   }
}

//
//  Invalidate the view region specified by rect
//
bool
NSVisualView::VnInvalidate(NS_CLASSLIB::TRect& rect)
{
  InvalidateRect(rect, true);

  return true;
}

//
//  The associated doc is opened
//
bool
NSVisualView::VnDocOpened(int /*omode*/)
{
  DragPart = 0;
  Pos.SetNull();
  Scale.Reset();
  return true;
}

//
//  The associated doc is closed
//
bool
NSVisualView::VnDocClosed(int /*omode*/)
{
  OcDoc = 0;
  return true;
}

// Fonction EvSize
////////////////////////////////////////////////////////////////
void
NSVisualView::EvSize(uint sizeType, NS_CLASSLIB::TSize& size)
{
	NS_CLASSLIB::TRect rectClient;

	TOleWindow::EvSize(sizeType, size);

   // on récupère la nouvelle zone client
   // et on ajuste la taille du controle
   rectClient = Parent->GetClientRect();

   if ((Ocx->GetRect()) != rectClient)
   	SetSizeOcx(rectClient);
}

// Fonctions EvSetFocus et EvKillFocus pour la control Bar
////////////////////////////////////////////////////////////////
void
NSVisualView::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
{
	TMyApp* 		pMyApp = pDocRef->pSuper->pNSApplication;
   TAutoString autoUrl;
	char far 	url[300];

	TOleWindow::EvSetFocus(hWndLostFocus);

   if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
   {
   	SetupNavBar();
      pMyApp->SetToolBarWindow(GetWindow());

      // pour le cas où on change de focus entre plusieurs NSVisualView
      if (nbNav)
      {
   		autoUrl = Control.LocationURL();
   		strcpy(url,autoUrl.operator const char far*());
   		pEditUrl->SetText(url);
      }
   }
}

void
NSVisualView::EvKillFocus(THandle hWndGetFocus /* may be 0 */)
{
	TOleWindow::EvKillFocus(hWndGetFocus);
}

void
NSVisualView::EvClose()
{
	TWindow::EvClose();
}

// Fonction Navigate
////////////////////////////////////////////////////////////////
void
NSVisualView::Navigate(string sUrlTarget)
{
	TAutoVal flags(navNoReadFromCache);
   TAutoVal target(0);
   TAutoVal data(0);
   TAutoVal headers(0);
   TAutoString URL;

   URL = (TAutoString) (sUrlTarget);
   Control.Navigate(URL,&flags,&target,&data,&headers);
}

// Fonction GenereHtml
////////////////////////////////////////////////////////////////
bool
NSVisualView::GenereHtml()
{
	string codeDoc;
   string sPathHtml = pDocRef->pSuper->PathName("SHTM");

   // on récupère le code du document à visualiser
   codeDoc = string(pDocRef->pDocInfo->pDonnees->codePatient) + string(pDocRef->pDocInfo->pDonnees->codeDocument);

   sHtml = codeDoc;

   // generation du fichier html (dans le repertoire du serveur)
   if (!pDocRef->GenereHtml(sPathHtml,sHtml,toVisualiser))
   {
   	erreur("Impossible de générer le fichier html à visualiser",0,0);
      sHtml = "";
   	return false;
   }

   return true;
}

// Fonction de destruction des fichiers temporaires
////////////////////////////////////////////////////////////////
void
NSVisualView::DetruireTemp()
{
	// destruction des fichiers temporaires de visualisation
   if (sHtml != "") // si un fichier a été généré
   {
      string sFichVisual;

		// destruction du fichier de visualisation
      sFichVisual = pDocRef->pSuper->PathName("SHTM") + sHtml;

   	if (!DeleteFile(sFichVisual.c_str()))
   		erreur("Pb de destruction du fichier temporaire de visualisation",0,0);
   }

   // ne pas oublier, sinon bug du destructeur
   pDocRef = 0;
}

void
NSVisualView::SetConnectionPoint()
{
/******************************************************
	IDispatch* pdisp = Control.Container();
   IConnectionPointContainer* pCPContainer;
   IUnknown*						pUnknown;

   pdisp->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPContainer);

   if (pCPContainer)
   {
   	// on récupère le pUnknown associé à la vue
		pdisp->QueryInterface(IID_IUnknown, (void**)&pUnknown);

      if (pUnknown)
      {
      	pCPContainer->FindConnectionPointFromIID(IID_IWebBrowser2, &pCP);
         pCP->Advise(pUnknown, &Cookie);
      }
      else erreur("Impossible de créer l'interface associée à l'objet vue",0,0);

      pUnknown->Release();
   }
   else erreur("Pb à la mise en place des évenements",0,0);

   pCPContainer->Release();
*************************************************************/
}

// Fonction de réponse à la commande Imprimer (publier)
////////////////////////////////////////////////////////////////
void
NSVisualView::CmPublier()
{
	bool bFinish;

	IDispatch* pdisp = Control.Document();
   IOleCommandTarget* command;
   pdisp->QueryInterface(IID_IOleCommandTarget, (void**)&command);
   if(command)
   {
   	if (!pDocRef->nbImpress)
      	command->Exec(NULL, OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);
      else
      {
   		for (int i = 0; i < pDocRef->nbImpress; i++)
      		command->Exec(NULL, OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);
      }
      command->Release();
   }
   pdisp->Release();

   if (pDocRef->nbImpress)
   {
   	// On envoie le Navigate du corresp suivant ou fin (bFinish == true)
   	bFinish = pDocRef->pPubli->ImprimerSuivant();

      if (bFinish)
      {
      	MessageBox("Impression terminée", 0, MB_OK);
         pDocRef->nbImpress = 0;
         pDocRef->sNomDocHtml = "";
         pDocRef->pPubli->DeleteVisualViews();
      }
   }
}

// Fonction de réponse à la commande CM_FILECLOSE
////////////////////////////////////////////////////////////////
void
NSVisualView::CmFileClose()
{
	DetruireTemp();

   // On détruit dans ce cas la partie NSRefDocument avant de détruire NSVisualView
   GetApplication()->GetDocManager()->CmFileClose();
}

////////////////////////////////////////////////////////////////
void
NSVisualView::CmPrecedent()
{
	if (page > 1)
   {
   	bNewNav = false;
		Control.GoBack();
      page--;
   }
}

////////////////////////////////////////////////////////////////
void
NSVisualView::CmSuivant()
{
	if (page < nbNav)
   {
   	bNewNav = false;
		Control.GoForward();
      page++;
   }
}

// fin de nsvisual.cpp
/////////////////////////////////////////////////////////////////


