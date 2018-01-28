// NSTTX.CPP		Document/Vues Traitement de texte// ------------------------------------------------
// Rémi SPAAK - Mai 1997

#define __NS_TTX_CPP
#include "AutoWordExp.h"#include <owl\owlpch.h>#include <owl\validate.h>
#include <owl\inputdia.h>
#include <owl\window.rh>

#include <fstream.h>// #include "nautilus\wordbasic.h"
#include "nautilus\wordObjects.h"

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
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
				          :TWindowView(doc, parent), _pDocTtx(&doc)
{
	_Form              = (TAutoWordFormEx*) 0 ;
	_pDialog           = (NSUtilDialog*) 0 ;

  _pApplicationProxy = (T_ApplicationProxy*) 0 ;
  _pDocumentsProxy   = (TDocumentsProxy*)    0 ;
  _pCurrentDocument  = (T_DocumentProxy*)    0 ;
  _sWordVersion      = string("") ;

  _bSessionEnCours   = false ;
}

NSAutoWordViewExp::~NSAutoWordViewExp(){
  if (_Form)
	  delete _Form ;

	if (_pDialog)		delete _pDialog ;
}

voidNSAutoWordViewExp::PerformCreate(int /*menuOrId*/)
{
	// on crée la Form pour servir de zone client (on lui passe le handle parent)
	_Form = new TAutoWordFormEx(Parent->GetHandle(), this) ;
	_Form->Visible = false ;
	_Form->ParentWindow = Parent->HWindow ;
	SetHandle(_Form->Handle) ;

	::SetParent(Forms::Application->Handle, _pDocTtx->pContexte->GetMainWindow()->HWindow) ;
	SetDocTitle(_pDocTtx->GetTitle(), 0) ;
}

// Fonction MakeVisiblevoid
NSAutoWordViewExp::MakeVisible()
{
	_Form->Show() ;
	_Form->Visible = true ;
}

voidNSAutoWordViewExp::SetupWindow()
{
	int X = 0, Y = 0;
	int W = _Form->Width, H = _Form->Height;

	TWindowView::SetupWindow();
	// La vue sera la fenetre client de la TMDIChild créée dans TMyApp::EvNewView	// Pour appliquer une taille à la vue, on doit donc agir sur la fenetre Parent

	Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER);	ModifyStyle(WS_BORDER, WS_CHILD);

	_bSessionEnCours = false;
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
	if (_bSessionEnCours)	{    if (_pDocumentsProxy && _pDocumentsProxy->IsBound())    {
      long lDocumentsCount = _pDocumentsProxy->Count() ;
      if ((1 == lDocumentsCount) && _pApplicationProxy && _pApplicationProxy->IsBound())
		    _pApplicationProxy->Quit() ;
    }
		_bSessionEnCours = false ;	}  if (_pCurrentDocument && _pCurrentDocument->IsBound())	{
    string ps = string("Word text: Unbinding document") ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

		_pCurrentDocument->Unbind() ;
    delete _pCurrentDocument ;
    _pCurrentDocument = (T_DocumentProxy*) 0 ;
	}

	if (_pDocumentsProxy && _pDocumentsProxy->IsBound())	{
    string ps = string("Word text: Unbinding documents proxy") ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

		_pDocumentsProxy->Unbind() ;
    delete _pDocumentsProxy ;
    _pDocumentsProxy = (TDocumentsProxy*) 0 ;
	}

  if (_pApplicationProxy && _pApplicationProxy->IsBound())
	{
    string ps = string("Word text: Unbinding application proxy") ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

		_pApplicationProxy->Unbind() ;
    delete _pApplicationProxy ;
    _pApplicationProxy = (T_ApplicationProxy*) 0 ;
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
  string ps = string("Html to Word export interface: Creating Application proxy") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  _pApplicationProxy = new T_ApplicationProxy() ;

  // Bind Word.Application
  //
try
{
  ps = string("Html to Word export interface: Bind Word.Application") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  _pApplicationProxy->Bind("Word.Application") ;
}
catch (const TXOle& e)
{
  ps = string("NSAutoWordViewExp::CmLancer Error binding Word.Application ") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  ps = string("NSAutoWordViewExp::CmLancer Error binding Word.Application") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}

  _pDocumentsProxy = new TDocumentsProxy() ;

  // Get word version
  //
try
{
  ps = string("Html to Word export interface: Get Word version") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  TString tsVersion = _pApplicationProxy->Version() ;
  _sWordVersion = string(tsVersion) ;
}
catch (const TXOle& e)
{
  ps = string("NSAutoWordViewExp::CmLancer Error getting Word version ") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  ps = string("NSAutoWordViewExp::CmLancer Error getting Word version") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}

  ps = string("Html to Word export interface: Word version is ") + _sWordVersion ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  // Get documents proxy
  //
try
{
  ps = string("Html to Word export interface: Get Documents proxy") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  _pApplicationProxy->_Documents(*_pDocumentsProxy) ;
}
catch (const TXOle& e)
{
  ps = string("NSAutoWordViewExp::CmLancer Error getting Documents proxy ") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  ps = string("NSAutoWordViewExp::CmLancer Error getting Documents proxy") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}

  CmEditer() ;
}

void
NSAutoWordViewExp::CmEditer()
{
try
{
  if (((T_ApplicationProxy*) NULL == _pApplicationProxy) || (false == _pApplicationProxy->IsBound()))
  {
    MessageBox("Impossible de lancer Word pour exporter le document...") ;
    return ;
  }

  // on ne controle pas si Word est déjà ouvert
  // car il faudrait rajouter un hook pour détecter la fermeture
  // on se contente d'intercepter les exceptions XOLE
  if (false == _bSessionEnCours)  {
    _pApplicationProxy->Visible(TRUE) ;
    _bSessionEnCours = true ;

    string ps = string("Html to Word export interface: opening file ") + _pDocTtx->GetNomFichier() ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

    wchar_t wcFileName[2048] ;
    swprintf(wcFileName, L"%S", _pDocTtx->GetNomFichier().c_str()) ;
    TAutoString asFileName(wcFileName);

    TAutoVal fileName(asFileName) ;
    _pCurrentDocument = _pDocumentsProxy->Open(&fileName) ;

    SetDocTitle(_pDocTtx->GetTitle(), 0) ;
  }
}
catch (const TXOle& e)
{
  string sExept = "Exception NSAutoWordViewExp::CmEditer " + string(e.what()) ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&sExept, 1, NSSuper::trError) ;

  sExept = string("Nautilus a perdu le contrôle de Word dans la fonction d'édition. ") +
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
	if (((T_DocumentProxy*) NULL == _pCurrentDocument) || (false == _pCurrentDocument->IsBound()))
    return false ;

	if (strlen(sWordName.c_str()) > 254)
	{
		string sMessage = string("HTMLtoWORF, nom de fichier trop long : ") + sWordName ;
		MessageBox(sMessage.c_str()) ;
		return false ;
	}

	wchar_t wcFileName[1024] ;

	if (false == _bSessionEnCours)	{
		string sMessage = string("Vous devez avoir un fichier ouvert dans Word avant de pouvoir l'archiver dans ") + _pDocTtx->pContexte->getSuperviseur()->getAppName().c_str() ;
		MessageBox(sMessage.c_str()) ;
		return false ;
	}

  TAutoVal fileFormat(wdFormatDocument) ;

	swprintf(wcFileName, L"%S", sWordName.c_str()) ;	TAutoString asFileName(wcFileName) ;  TAutoVal fileName(asFileName) ;
  if (string("9.0") == _sWordVersion)
    _pCurrentDocument->SaveAs2000(&fileName, &fileFormat) ;
  else
    _pCurrentDocument->SaveAs(&fileName, &fileFormat) ;
  // _pControl->FileClose(1);
  // _pControl->AppClose();
  
  CloseWindow() ;

  // _bSessionEnCours = false ;
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


