// NSTTX.CPP		Document/Vues Traitement de texte// ------------------------------------------------
// Rémi SPAAK - Mai 1997

#define __NS_TTX_CPP
#include "AutoWordExp.h"#include "nautilus\wordbasic.h"

#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>
#include <fstream.h>

#include "nautilus\nssuper.h"#include "partage\nsdivfct.h"
#include "export\html2word.h"

/******************************************************************************/
//
//					METHODES DE NSAutoWordViewExp
//
/******************************************************************************/

DEFINE_RESPONSE_TABLE1(NSAutoWordViewExp, TWindowView)    EV_WM_CLOSE,
    EV_COMMAND(CM_FILECLOSE, EvClose),

    // Evenements gérés par la classe VCL AutoWord
    // EV_COMMAND(IDC_AW_ARCHIVER, CmArchiver),    // EV_COMMAND(IDC_AW_PUBLIER, CmPublier),
    // EV_COMMAND(IDC_AW_VISUALISER, CmVisualiser),
    // EV_COMMAND(IDC_AW_EDITER, CmEditer),
    // EV_COMMAND(IDC_AW_COMPOSER, CmComposer),
    // EV_COMMAND(IDC_AW_FORMULES, CmFormules),
    // EV_COMMAND(IDC_AW_QUITTER, CmQuitter),

END_RESPONSE_TABLE;
// Attention il faut passer doc comme NSTtxDocument avec un contexte valideNSAutoWordViewExp::NSAutoWordViewExp(NSTtxDocumentExp& doc, TWindow *parent)
				   :TWindowView(doc, parent), pDocTtx(&doc)
{
	Form     = 0 ;
	pControl = 0 ;
	pDialog  = 0 ;
}

NSAutoWordViewExp::~NSAutoWordViewExp(){
	delete Form ;

	if (pDialog)		delete pDialog ;
}

voidNSAutoWordViewExp::PerformCreate(int /*menuOrId*/)
{
	// on crée la Form pour servir de zone client (on lui passe le handle parent)
	Form = new TAutoWordFormEx(Parent->GetHandle(), this) ;
	Form->Visible = false ;
	Form->ParentWindow = Parent->HWindow ;
	SetHandle(Form->Handle) ;

	::SetParent(Forms::Application->Handle, pDocTtx->pContexte->GetMainWindow()->HWindow) ;
	SetDocTitle(pDocTtx->GetTitle(),0) ;
}

// Fonction MakeVisiblevoid
NSAutoWordViewExp::MakeVisible()
{
	Form->Show() ;
	Form->Visible = true ;
}

voidNSAutoWordViewExp::SetupWindow()
{
	int X = 0, Y = 0;
	int W = Form->Width, H = Form->Height;

	TWindowView::SetupWindow();
	// La vue sera la fenetre client de la TMDIChild créée dans TMyApp::EvNewView	// Pour appliquer une taille à la vue, on doit donc agir sur la fenetre Parent

	Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER);	ModifyStyle(WS_BORDER, WS_CHILD);

	bSessionEnCours = false;
	CmLancer();}

voidNSAutoWordViewExp::EvClose()
{
	// fonction appelée sur Fichier->Fermer
	CloseWindow() ;
}

boolNSAutoWordViewExp::CanClose()
{
try
{
	// on place ici le code qui vérifie si le document est bien sauvegardé	// car CanClose est appelée directement lorsqu'on clique sur la case de fermeture
	// et la vue est fermée sans autre forme de procès...

	if ((pControl) && (pControl->IsBound()))	{
		pControl->Unbind() ;
		// CoUninitialize() ;
		delete pControl ;
		pControl = 0 ;
	}

	bool bOk = TWindowView::CanClose() ;
	// ici, bizarrement, après un FileSaveAs, TWindowView::CanClose renvoie false	// on force donc la valeur à true pour pouvoir sortir...
	return true ;
}catch (const TXOle& e){
  string sExept = string("Nautilus a perdu le contrôle de Word dans la fonction de fermeture. ") +
                  string("Pour fermer Word, vous devez utiliser <Quitter> dans la fenetre <Document Word> de Nautilus.");
  erreur(sExept.c_str(), standardError, 0) ;
  return true ;
}catch (...)
{
  erreur("Exception NSAutoWordViewExp::CanClose.", standardError, 0) ;
  return true ;
}
}
boolNSAutoWordViewExp::IsTabKeyMessage(MSG &msg)
{
	if (GetCapture() == NULL)
	{
		if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
		{
			if (msg.wParam == VK_TAB)
			{
				SendMessage(CN_BASE + msg.message, msg.wParam, msg.lParam);
				return true;
			}
		}
	}

	return false;}

//// Let the form process keystrokes in its own way.  Without
// this method, you can't tab between control on the form.
//
bool
NSAutoWordViewExp::PreProcessMsg(MSG &msg)
{
	bool result = IsTabKeyMessage(msg) ;

	if (!result)		result = TWindow::PreProcessMsg(msg) ;

	return result ;}

voidNSAutoWordViewExp::FillStartupInfo(LPSTARTUPINFO psi)
{
    psi->cb = sizeof (*psi);
    psi->lpReserved = NULL;
    psi->lpDesktop = NULL;
    psi->lpTitle = NULL;
    psi->dwX = 0;
    psi->dwY = 0;
    psi->dwXSize = 0;
    psi->dwYSize = 0;
    psi->dwXCountChars = 0;
    psi->dwYCountChars = 0;
    psi->dwFillAttribute = 0;
    psi->dwFlags = STARTF_USESHOWWINDOW;
    psi->wShowWindow = SW_SHOWNORMAL;
    psi->cbReserved2 = 0;
    psi->lpReserved2 = NULL;
    psi->hStdInput = NULL;
    psi->hStdOutput = NULL;
    psi->hStdError = NULL;
}

voidNSAutoWordViewExp::CmLancer()
{
    /************************** pour le hook ***************************
    // pour rendre le process fils et son primary thread héritables

    STARTUPINFO startupInfo;
    PROCESS_INFORMATION procInfo;
    DWORD lRet;
    char cmdline[255], pathname[255], command[255];
    SECURITY_ATTRIBUTES saProc, saThread;

    saProc.nLength = sizeof(saProc);    saProc.lpSecurityDescriptor = NULL;
    saProc.bInheritHandle = true;

    saThread.nLength = sizeof(saThread);
    saThread.lpSecurityDescriptor = NULL;
    saThread.bInheritHandle = true;

    //////////////////////////////////////////////////////////////////////////////
    // Lancement de Word.Basic
    //////////////////////////////////////////////////////////////////////////////
    strcpy(pathname, "C:\\program files\\microsoft office\\office\\");
    strcpy(command, "winword.exe");

    // lRet = ExecuteCommand(pathname, command);
    sprintf(cmdline, "%s%s", pathname, command);
    FillStartupInfo(&startupInfo);

    if (!CreateProcess(NULL, cmdline, &saProc, &saThread, true, NORMAL_PRIORITY_CLASS,
                        NULL, pathname, &startupInfo, &procInfo))
    {
        MessageBox("Erreur sur CreateProcess");
        return;
    }

    // Attente de la fin de l'initialisation du process
    lRet = WaitForInputIdle(procInfo.hProcess, INFINITE);
    if (lRet != 0)
    {
        MessageBox("Erreur sur WaitForInputIdle");
        return;
    }
    *******************************************************************************/

	//CoInitialize(NULL);
	pControl = new TWordBasicProxy();

	try
	{
		pControl->Bind("Word.Basic");
	}
	catch (...)
	{
		MessageBox("Erreur Bind Word.Basic");
		//CoUninitialize();
		return;
	}

    CmEditer();
}

void
NSAutoWordViewExp::CmEditer()
{
try
{
	wchar_t wcFileName[255];
	if ((!pControl) || (!(pControl->IsBound())))	{		erreur("Impossible de lancer Word...", standardError, 0) ;		return ;	}	// on ne controle pas si Word est déjà ouvert
	// car il faudrait rajouter un hook pour détecter la fermeture
	// on se contente d'intercepter les exceptions XOLE
	if (bSessionEnCours)	{		erreur("Impossible d'éditer car une session est en cours...", standardError, 0) ;		return ;	}	pControl->AppShow() ;	bSessionEnCours = true ;

	swprintf(wcFileName, L"%S", pDocTtx->GetNomFichier().c_str()) ;
	TAutoString asFileName(wcFileName) ;
	pControl->FileOpen(asFileName) ;
	// on affiche le titre du document
	SetDocTitle(pDocTtx->GetTitle(), 0) ;
}
catch (const TXOle& e)
{
  string sExept = string("Nautilus a perdu le contrôle de Word dans la fonction d'édition. ") +
                  string("Pour fermer Word, vous devez utiliser <Quitter> dans la fenetre <Document Word> de Nautilus.");
  erreur(sExept.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("Exception NSAutoWordViewExp::CmEditer.", standardError, 0) ;
}
}
boolNSAutoWordViewExp::saveHTMLtoWORD(string sWordName)
{
try
{
	if (strlen(sWordName.c_str()) > 254)
	{
		string sMessage = string("HTMLtoWORD, nom de fichier trop long : ") + sWordName ;
		MessageBox(sMessage.c_str()) ;
		return false ;
	}

	short format ;
	wchar_t wcFormatName[255] ;
	wchar_t wcFileName[255] ;

	if (!bSessionEnCours)	{
		string sMessage = string("Vous devez avoir un fichier ouvert dans Word avant de pouvoir l'archiver dans ") + pDocTtx->pContexte->getSuperviseur()->getAppName() ;
		MessageBox(sMessage.c_str()) ;
		return false ;
	}

	// on sauvegarde en html sous la bonne localisation	wcscpy(wcFormatName, L"DOC") ;

	TAutoString asFormatName(wcFormatName) ;
	format = pControl->ConverterLookup(asFormatName) ;

	// sprintf(msg, "Format HTML = <%hd>", format);	// MessageBox(msg);

    string sFileName = pDocTtx->pContexte->PathName("EHTM") + sWordName;
	swprintf(wcFileName, L"%S", sFileName.c_str()) ;
	TAutoString asFileName(wcFileName) ;	pControl->FileSaveAs(asFileName, format) ;

    pControl->FileClose(1);
    pControl->AppClose();
    CloseWindow() ;

    bSessionEnCours = false ;
  return true ;
}
catch (const TXOle& e)
{
  string sExept = string("Nautilus a perdu le contrôle de Word dans la fonction d'archivage. ") +
                  string("Pour fermer Word, vous devez utiliser <Quitter> dans la fenetre <Document Word> de Nautilus.") ;
  erreur(sExept.c_str(), standardError, 0) ;
  return false ;
}
catch (...)
{
  erreur("Exception NSAutoWordViewExp::saveHTMLtoWORD", standardError, 0) ;
  return false ;
}
}
/******************************************************************************///					METHODES DE NSTtx Document / Vue
/******************************************************************************/

// Constructeur NSTtxDocument////////////////////////////////////////////////////////////////

NSTtxDocumentExp::NSTtxDocumentExp(TDocument *parent, NSContexte *pCtx)                 :TDocument(parent), NSRoot(pCtx)
{
	// Mise à vide du nom de fichier
	SetNomFichier("") ;
	SetFichierExterne("") ;
	Open(0, "") ;
}

string NSTtxDocumentExp::GetDocExt(){
	size_t i ;
	string sExt = string("") ;

  size_t iFileNameLen = strlen(_sNomFichier.c_str()) ;

	for (i = 0 ; (i < iFileNameLen) && ('.' != _sNomFichier[i]) ; i++) ;

	if (i < iFileNameLen)
	{
		// nomDoc[i] == '.'
		i++ ;
		while (i < iFileNameLen)
			sExt += _sNomFichier[i++] ;
	}

	return sExt ;
}

// Ouverture du document////////////////////////////////////////////////////////////////

bool NSTtxDocumentExp::Open(int /*mode*/, LPCSTR path){
	string sNomFichier = string("") ;

	// Si il existe, prise du nom de fichier de la fiche document	//ValideFichier(&sNomFichier);

	// Sinon, on sort	//if (!bDocumentValide)
	//	return false;

	//	// Rangement du nom de fichier dans le document
	//
	SetNomFichier(sNomFichier) ;
	SetDirty(false) ;
	return true ;
}

// Fermeture du document////////////////////////////////////////////////////////////////
bool NSTtxDocumentExp::Close()
{
  // if (TDocument::Close())
  // 	 return false;

  SetNomFichier(string("")) ;
  return true ;
}

// Indique si document ouvert (il existe un fichier associé)////////////////////////////////////////////////////////////////
bool NSTtxDocumentExp::IsOpen()
{
  return (string("") != _sNomFichier) ;
}

// Trouve un nom de fichier nautilus au document
////////////////////////////////////////////////////////////////
/*
bool NSTtxDocumentExp::Renommer(string& sNomFichier, string& sLocalis, string sType, string* psNomBrut)
{
    string sPath = "";
    string sExt = GetDocExt();

    if (!TrouveNomFichier(sType,sExt,sNomFichier,sLocalis))
    {
    	erreur("Pb à l'attribution d'un nouveau nom pour ce fichier",standardError,0,pContexte->GetMainWindow()->GetHandle());
   	    return false;
    }

    if (psNomBrut)
    {
        // Dans ce cas on renvoie aussi le nom brut (sans extension)
        size_t pos = sNomFichier.find('.');
        if (pos != NPOS)
            *psNomBrut = string(sNomFichier, 0, pos);
        else
            *psNomBrut = sNomFichier;
    }

    sPath = pContexte->PathName(sLocalis);

    // on construit le nom complet pour la sauvegarde
    SetNomFichier(sPath + sNomFichier);

    SetDirty(true);

    return true;
}
*/

// fin de html2word.cpp
//////////////////////////////////////////////////


