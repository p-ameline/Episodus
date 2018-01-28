// NSBROWSE.CPP : Dialogues de composition et de publication
////////////////////////////////////////////////////////////

#include <cstring.h>
#include <stdio.h>
#include <assert.h>
#include "nautilus\nsbrowse.h"
#include "nautilus\nsdocref.h"
#include "nautilus\nscompub.h"
#include "nautilus\nsresour.h"
#include "nautilus\nssuper.h"
#include "nautilus\nshistdo.h"

const TString progIdIE = "Shell.Explorer"; // Controle OCX (Version independant progid)
GUID  guidIE = CLSID_NULL;
const TString progIdAppIE = "InternetExplorer.Application.1";
GUID  guidAppIE = CLSID_NULL;

/****************** classe TDIECompo **************************/

DEFINE_RESPONSE_TABLE1(TDIECompo, TOleDialog)
	EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

TDIECompo::TDIECompo(TWindow* parent, NSRefDocument* pDocMaitre, TModule* module) :
	TOleDialog(parent,"IDD_IEBROWSER",module)
{
	fichCompo = "";
   pDocBrut = pDocMaitre;	// Warning ne pas deleter
}

TDIECompo::~TDIECompo()
{
}

void TDIECompo::SetupWindow()
{
	TOleDialog::SetupWindow();

   // Ancienne méthode : on récupère le GUID à partir du PROGID
   ::CLSIDFromProgID(progIdIE, &guidIE);
   // GUID iidControl = IID_IWebBrowser2;

   // Retrieve TOcControl objects representing the respective OCX
   // of our dialog..
   //
	Ocx = GetOcControlOfOCX(guidIE);
	Control.Bind(Ocx->GetCtlDispatch());
}

bool TDIECompo::Message(const char far *texte)
{
	THandle hDialog;
   int retVal;

	hDialog = GetHandle();

   retVal = ::MessageBox(hDialog,texte,"Message",MB_OKCANCEL);

   if (retVal == IDOK)
   	return true;
   else
   	return false;
}

void TDIECompo::Navigate(string url, bool kill)
{
   	TAutoVal flags(navNoReadFromCache);
   TAutoVal target(0);
   TAutoVal data(0);
   TAutoVal headers(0);
   TAutoString URL;

	if (kill)
   	fichCompo = pDocBrut->pSuper->PathName("SHTM") + url;

   url = pDocBrut->pSuper->PathName("UHTM") + url;
   URL = (TAutoString) url;
   Control.Navigate(URL,&flags,&target,&data,&headers);
   TWindow::SetDocTitle(pDocBrut->GetTitle(),0);
}

void TDIECompo::CmOk()
{
	bool bErreur = false;

   // Referencement du nouveau fichier Html (s'il y a lieu)
   if (pDocBrut->pHtmlInfo) // il existe deja un fichier composé
   {
   	if (pDocBrut->sNomDocHtml != "") // ce fichier change de nom
      {
      	// on le référence sous son nouveau nom
      	if (!pDocBrut->ReferencerHtml("HDHTM", pDocBrut->sNomDocHtml, pDocBrut->sTemplate))
         {
         	::MessageBox(GetHandle(),"Pb : le fichier HTML n'a pas pu etre référencé.",0,MB_OK);
            bErreur = true;
         }
      }
      else // le fichier ne change pas de nom, on met à jour la template de composition
      {
      	if (!pDocBrut->MajTemplate())
         {
         	::MessageBox(GetHandle(),"Pb : la template du fichier HTML n'a pas pu etre mise à jour.",0,MB_OK);
            bErreur = true;
         }
      }
   }
   else // cas nouvelle composition : on garde le nom du document brut
   {
   	if (!pDocBrut->ReferencerHtml("HDHTM", string(pDocBrut->pDocInfo->pDonnees->nom), pDocBrut->sTemplate))
      {
        	::MessageBox(GetHandle(),"Pb : le fichier HTML n'a pas pu etre référencé.",0,MB_OK);
         bErreur = true;
      }
   }

   // mise à jour de la base des composants Html

   if (!bErreur)
   {
   	if (!pDocBrut->DetruireComposants())
   	{
   		::MessageBox(GetHandle(),"Pb à la destruction des composants html.",0,MB_OK);
   	}

		if (!pDocBrut->EcrireComposants())
   	{
   		::MessageBox(GetHandle(),"Pb à la mise à jour des composants html.",0,MB_OK);
   	}

   	//ajouter le document composé à l'historique
   	pDocBrut->pSuper->pUtilisateur->pPatient->pDocHis->Rafraichir(pDocBrut->pHtmlInfo, 0);
   	//enlever le document brut
   	pDocBrut->pSuper->pUtilisateur->pPatient->pDocHis->Rafraichir(pDocBrut->pDocInfo, 0);
   }

   // destruction du fichier de composition
   if (fichCompo != "")
   {
   	if (!DeleteFile(fichCompo.c_str()))
   		::MessageBox(GetHandle(),"Pb de destruction du fichier de composition",0,MB_OK);
   }

	if (Control.IsBound())
   {
   	Control.Unbind();
   }

   TOleDialog::CmOk();
}

void TDIECompo::CmCancel()
{
	bool test;

	// destruction du fichier de composition
   if (fichCompo != "")
   {
   	if (!DeleteFile(fichCompo.c_str()))
   		::MessageBox(GetHandle(),"Pb de destruction du fichier de composition",0,MB_OK);
   }

	if (Control.IsBound())
   {
   	Control.Unbind();
   }

   TOleDialog::CmOk();
}

/****************** classe TDIEPubli **************************/

DEFINE_RESPONSE_TABLE1(TDIEPubli, TOleDialog)
	EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

TDIEPubli::TDIEPubli(TWindow* parent, NSPublication* pPub, TModule* module) :
	TOleDialog(parent,"IDD_IEBROWSER",module)
{
	pPubli = pPub;
}

TDIEPubli::~TDIEPubli()
{
}

void TDIEPubli::SetupWindow()
{
	TOleDialog::SetupWindow();

   // Ancienne méthode : on récupère le GUID à partir du PROGID
   ::CLSIDFromProgID(progIdIE, &guidIE);
   // GUID iidControl = IID_CoWebBrowser;

   // Retrieve TOcControl objects representing the respective OCX
   // of our dialog..
   //
	Ocx = GetOcControlOfOCX(guidIE);
	Control.Bind(Ocx->GetCtlDispatch());

   // Le hook n'est plus nécéssaire avec IE4
   // pPubli->InstalleHook();
}

bool TDIEPubli::Message(const char far *texte)
{
	THandle hDialog;
   int retVal;

	hDialog = GetHandle();

   retVal = ::MessageBox(hDialog,texte,"Message",MB_OKCANCEL);

   if (retVal == IDOK)
   	return true;
   else
   	return false;
}

void TDIEPubli::Print()
{
   IDispatch* doc = Control.Document();
   IOleCommandTarget* target;
   bool bFinish;

   assert(doc);
   doc->QueryInterface(IID_IOleCommandTarget, (void**)&target);
   assert(target);
   target->Exec(NULL,OLECMDID_PRINT,OLECMDEXECOPT_DONTPROMPTUSER,NULL,NULL);
   target->Release();
   doc->Release();

   // On envoie le Navigate du corresp suivant ou fin (bFinish == true)
   bFinish = pPubli->ImprimerSuivant();

   if (bFinish) // on envoie un return pour déclencher CmOk
   {
   	// On doit envoyer un SetFocus car le Navigate fait perdre
      // le focus au dialogue.
      SetFocus();
      keybd_event(VK_RETURN,0,0,0);
      keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);
   }
}

void TDIEPubli::Navigate(string url)
{
	TAutoVal flags(navNoReadFromCache);
   TAutoVal target(0);
   TAutoVal data(0);
   TAutoVal headers(0);
   TAutoString URL;

   url = pPubli->pSuper->PathName("UHTM") + url;
   URL = (TAutoString) url;

   Control.Navigate(URL,&flags,&target,&data,&headers);
}

bool TDIEPubli::IsBusy()
{
	TBool bBusy;

   bBusy = Control.Busy();
   return static_cast<bool>(bBusy);
}

void TDIEPubli::CmOk()
{
	// pPubli->LibereHook();

   // pPubli->DeleteFichPubli();

	if (Control.IsBound())
   {
   	Control.Unbind();
   }

   TOleDialog::CmOk();
}

void TDIEPubli::CmCancel()
{
	// pPubli->LibereHook();

	// pPubli->DeleteFichPubli();

	if (Control.IsBound())
   {
   	Control.Unbind();
   }

   TOleDialog::CmOk();
}

void TDIEPubli::StatusTextChange(TAutoString Text)
{
}

void TDIEPubli::ProgressChange(long Progress, long ProgressMax)
{
	/**************************
	if (Progress == -1)
   {
   	Message("Progress == -1");
   }
   ****************************/
}

void TDIEPubli::CommandStateChange(long Command, TBool Enable)
{
}

void TDIEPubli::DownloadBegin()
{
}

void TDIEPubli::DownloadComplete()
{
}

void TDIEPubli::TitleChange(TAutoString Text)
{
   TWindow::SetDocTitle(Text, 0);
}

void TDIEPubli::PropertyChange(TAutoString szProperty)
{
}

void TDIEPubli::BeforeNavigate2(IDispatch* pDisp, TAutoVal URL, TAutoVal Flags, TAutoVal TargetFrameName, TAutoVal PostData, TAutoVal Headers, TBool* Cancel)
{
}

void TDIEPubli::NewWindow2(IDispatch** ppDisp, TBool* Cancel)
{
}

void TDIEPubli::NavigateComplete2(IDispatch* pDisp, TAutoVal URL)
{
	// On imprime après chaque navigation
	Print();
}

void TDIEPubli::DocumentComplete(IDispatch* pDisp, TAutoVal URL)
{
   //
   // at this point we can install our UI hook because the document
   // object is now available, for some strange reason pDisp parameter
   // does not point to the document object.....
   /************************************************
   if(!IsBound())
      return;
   ICustomDoc* pdoc;
   IDispatch* pdisp = Document();
   if(pdisp)
   {
      pdisp->QueryInterface(IID_ICustomDoc, &((void*)pdoc));
      pdisp->Release();
      if(pdoc)
      {
         pdoc->SetUIHandler(static_cast<IDocHostUIHandler*>(this));
         pdoc->Release();
         init = true;
      }
   }
   ***************************************************/
}

void TDIEPubli::OnQuit()
{
}

void TDIEPubli::OnVisible(TBool Visible)
{
}

void TDIEPubli::OnToolBar(TBool ToolBar)
{
}

void TDIEPubli::OnMenuBar(TBool MenuBar)
{
}

void TDIEPubli::OnStatusBar(TBool StatusBar)
{
}

void TDIEPubli::OnFullScreen(TBool FullScreen)
{
}

void TDIEPubli::OnTheaterMode(TBool TheaterMode)
{
}

bool TDIEPubli::EvOcCtrlCustomEvent(TCtrlCustomEvent* pev)
{
	switch(pev->Args->DispId)
	{
	case 102:
	{
		TAutoString arg0 = pev->Args->operator[](0);
		StatusTextChange(arg0);
		break;
	}
	case 108:
	{
		long arg0 = pev->Args->operator[](0);
		long arg1 = pev->Args->operator[](1);
		ProgressChange(arg0, arg1);
		break;
	}
	case 105:
	{
		long arg0 = pev->Args->operator[](0);
		TBool arg1 = pev->Args->operator[](1);
		CommandStateChange(arg0, arg1);
		break;
	}
	case 106:
	{
		DownloadBegin();
		break;
	}
	case 104:
	{
		DownloadComplete();
		break;
	}
	case 113:
	{
		TAutoString arg0 = pev->Args->operator[](0);
		TitleChange(arg0);
		break;
	}
	case 112:
	{
		TAutoString arg0 = pev->Args->operator[](0);
		PropertyChange(arg0);
		break;
	}
	case 250:
	{
		IDispatch* arg0 = pev->Args->operator[](0);
		TAutoVal arg1 = pev->Args->operator[](1);
		TAutoVal arg2 = pev->Args->operator[](2);
		TAutoVal arg3 = pev->Args->operator[](3);
		TAutoVal arg4 = pev->Args->operator[](4);
		TAutoVal arg5 = pev->Args->operator[](5);
		TBool* arg6 = pev->Args->operator[](6);
		BeforeNavigate2(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
		break;
	}
	case 251:
	{
		IDispatch** arg0 = ((VARIANT*)&(pev->Args->operator[](0)))->ppdispVal;
		TBool* arg1 = pev->Args->operator[](1);
		NewWindow2(arg0, arg1);
		break;
	}
	case 252:
	{
		IDispatch* arg0 = pev->Args->operator[](0);
		TAutoVal arg1 = pev->Args->operator[](1);
		NavigateComplete2(arg0, arg1);
		break;
	}
	case 259:
	{
		IDispatch* arg0 = pev->Args->operator[](0);
		TAutoVal arg1 = pev->Args->operator[](1);
		DocumentComplete(arg0, arg1);
		break;
	}
	case 253:
	{
		OnQuit();
		break;
	}
	case 254:
	{
		TBool arg0 = pev->Args->operator[](0);
		OnVisible(arg0);
		break;
	}
	case 255:
	{
		TBool arg0 = pev->Args->operator[](0);
		OnToolBar(arg0);
		break;
	}
	case 256:
	{
		TBool arg0 = pev->Args->operator[](0);
		OnMenuBar(arg0);
		break;
	}
	case 257:
	{
		TBool arg0 = pev->Args->operator[](0);
		OnStatusBar(arg0);
		break;
	}
	case 258:
	{
		TBool arg0 = pev->Args->operator[](0);
		OnFullScreen(arg0);
		break;
	}
	case 260:
	{
		TBool arg0 = pev->Args->operator[](0);
		OnTheaterMode(arg0);
		break;
	}
	}
	return true;
}

////////////////////////// fin de nsbrowse.cpp ///////////////////////////////

