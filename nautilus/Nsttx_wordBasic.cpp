// NSTTX.CPP		Document/Vues Traitement de texte// ------------------------------------------------
// Rémi SPAAK - Mai 1997

#define __NS_TTX_CPP
#include "AutoWord.h"#include "nautilus\wordbasic.h"

#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>
#include <owl\window.rh>

#include <fstream.h>

#include "nautilus\nssuper.h"#include "partage\nsdivfct.h"
#include "nautilus\nsttx.h"
#include "nautilus\ns_html.h"
#include "nautilus\nscompub.h"		// pour les formules
#include "nautilus\nsresour.h"      // pour AutoWordDialog
#include "nautilus\nsvisual.h"
#include "nautilus\nshistdo.h"
#include "nsbb\nsattvaltools.h"
#include "nsbb\nsfltbtnga.h"

#include "nautilus\nautilus.rh"
/******************************************************************************/
//
//					METHODES DE NSAutoWordView
//
/******************************************************************************/

DEFINE_RESPONSE_TABLE1(NSAutoWordView, TWindowView)	EV_WM_CLOSE,
  EV_COMMAND(CM_FILECLOSE, EvClose),

  // Evenements gérés par la classe VCL AutoWord
  // EV_COMMAND(IDC_AW_ARCHIVER, CmArchiver),  // EV_COMMAND(IDC_AW_PUBLIER, CmPublier),
  // EV_COMMAND(IDC_AW_VISUALISER, CmVisualiser),
  // EV_COMMAND(IDC_AW_EDITER, CmEditer),
  // EV_COMMAND(IDC_AW_COMPOSER, CmComposer),
  // EV_COMMAND(IDC_AW_FORMULES, CmFormules),
  // EV_COMMAND(IDC_AW_QUITTER, CmQuitter),

END_RESPONSE_TABLE ;

// Attention il faut passer doc comme NSTtxDocument avec un contexte valideNSAutoWordView::NSAutoWordView(NSTtxDocument& doc, TWindow *parent)
               :TWindowView(doc, parent), _pDocTtx(&doc)
{
	_Form     = (TAutoWordForm*)   0 ;
  _pControl = (TWordBasicProxy*) 0 ;
  _pDocName = (NSUtilEdit*)      0 ;
  _pDialog  = (NSUtilDialog*)    0 ;

  _bSessionEnCours = false ;
}

NSAutoWordView::~NSAutoWordView(){
  if (_Form)
	  delete _Form ;

	if (_pDialog)  {
  	// delete pDocName;
    delete _pDialog ;
	}
}

voidNSAutoWordView::PerformCreate(int /*menuOrId*/)
{
	// on crée la Form pour servir de zone client (on lui passe le handle parent)
  _Form = new TAutoWordForm(Parent->GetHandle(), this) ;
  _Form->Visible = false ;
  _Form->ParentWindow = Parent->HWindow ;
  SetHandle(_Form->Handle) ;

  ::SetParent(Forms::Application->Handle, _pDocTtx->pContexte->GetMainWindow()->HWindow) ;
  SetDocTitle(_pDocTtx->GetTitle(), 0) ;
}

// Fonction MakeVisiblevoid
NSAutoWordView::MakeVisible()
{
	_Form->Show() ;
  _Form->Visible = true ;
}

voidNSAutoWordView::SetupWindow()
{
	int X = 0, Y = 0 ;
  int W = _Form->Width ;
  int H = _Form->Height ;

	TWindowView::SetupWindow() ;
  // La vue sera la fenetre client de la TMDIChild créée dans TMyApp::EvNewView  // Pour appliquer une taille à la vue, on doit donc agir sur la fenetre Parent

  Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER) ;  ModifyStyle(WS_BORDER, WS_CHILD) ;

  // ModifyExStyle(0, WS_EX_TOPMOST | WS_EX_TOOLWINDOW);  MakeVisible() ;
  _bSessionEnCours = false ;

  CmLancer() ;}

voidNSAutoWordView::EvClose()
{
  string ps = string("User closes a Word text") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	// fonction appelée sur Fichier->Fermer

	CmQuitter() ;}

boolNSAutoWordView::CanClose()
{
try
{
  string ps = string("Word text: entering CanClose") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	// on place ici le code qui vérifie si le document est bien sauvegardé  // car CanClose est appelée directement lorsqu'on clique sur la case de fermeture
  // et la vue est fermée sans autre forme de procès...

	// là encore, pour bien faire, il faudrait un hook	if (_bSessionEnCours)	{  	if (_pControl->IsDocumentDirty())
    {
    	string sDoISave = _pDocTtx->pContexte->getSuperviseur()->getText("msWordManagement", "yourWordDocumentWasModified.DoYouWantToSaveChanges?") ;
    	string sCaption = string("Message ") + _pDocTtx->pContexte->getSuperviseur()->getAppName().c_str() ;

			int idRet = MessageBox(sDoISave.c_str(), sCaption.c_str(), MB_YESNOCANCEL) ;
      if      (IDYES == idRet)      	CmArchiver() ;
      else if (IDCANCEL == idRet)
      {
        string ps = string("User answers No to proposal to save the Word text, canceling Canclose") ;
        _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

      	return false ;
      }
		}

		_pControl->FileClose(1) ;		_pControl->AppClose() ;

		_bSessionEnCours = false ;	}

	if ((_pControl) && (_pControl->IsBound()))	{
    ps = string("Word text: Unbinding") ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

		_pControl->Unbind() ;
    // CoUninitialize() ;
    delete _pControl ;
    _pControl = 0 ;
	}

	// on rafraichit, en sortant, la visualisation du document si elle existe	// Note : on suppose ici, comme dans NSComposView::CmOk, qu'il n'existe
	// qu'une seule instance de vue de type Visual par document

	TView*        pView       = (TView*)        0 ;	NSVisualView* pVisualView = (NSVisualView*) 0 ;

	if (_pDocTtx->pDocInfo)  // si le document est référencé	{
		pView = _pDocTtx->pContexte->getPatient()->getDocHis()->ActiveFenetre(_pDocTtx->pDocInfo, "NSVisualView") ;

		if (pView)    {
    	pVisualView = dynamic_cast<NSVisualView*>(pView) ;
      pVisualView->Rafraichir() ;
    }
  }

	/* bool bOk = */ TWindowView::CanClose() ;
  ps = string("Word text: leaving CanClose") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	// ici, bizarrement, après un FileSaveAs, TWindowView::CanClose renvoie false	// on force donc la valeur à true pour pouvoir sortir...
	return true ;
}catch (const TXOle& e){
	string sConnectionLost = _pDocTtx->pContexte->getSuperviseur()->getText("msWordManagement", "automationLinkWithWordWasLost") ;
  string sMustCloseHere  = _pDocTtx->pContexte->getSuperviseur()->getText("msWordManagement", "youMustCloseWordFromOurMenu") ;

  string sExept = sConnectionLost + string(" ") + sMustCloseHere ;

  erreur(sExept.c_str(), standardError, 0) ;
  return true ;
}catch (...)
{
  erreur("Exception NSAutoWordView::CanClose.", standardError, 0) ;
  return true ;
}
}
boolNSAutoWordView::IsTabKeyMessage(MSG &msg)
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
NSAutoWordView::PreProcessMsg(MSG &msg)
{
  bool result = IsTabKeyMessage(msg) ;

  if (false == result)    result = TWindow::PreProcessMsg(msg) ;

  return result ;}

voidNSAutoWordView::FillStartupInfo(LPSTARTUPINFO psi)
{
  psi->cb = sizeof (*psi);
  psi->lpReserved = NULL;
  psi->lpDesktop = NULL;
  psi->lpTitle = NULL;
  psi->dwX = 0;
  psi->dwY = 0;
  psi->dwXSize = 0;
  psi->dwYSize = 0;
  psi->dwXCountChars   = 0 ;
  psi->dwYCountChars   = 0 ;
  psi->dwFillAttribute = 0 ;
  psi->dwFlags         = STARTF_USESHOWWINDOW;
  psi->wShowWindow     = SW_SHOWNORMAL;
  psi->cbReserved2     = 0;
  psi->lpReserved2     = NULL;
  psi->hStdInput       = NULL;
  psi->hStdOutput      = NULL;
  psi->hStdError       = NULL;
}

voidNSAutoWordView::CmLancer()
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

  string ps = string("Word interface: Start CoInitialize") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  //CoInitialize(NULL) ;
  _pControl = new TWordBasicProxy() ;

  try
  {
    ps = string("Word interface: Bind Word.Basic") ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    _pControl->Bind("Word.Basic") ;
  }
  catch (...)
  {
    ps = string("Error Binding Word.Basic") ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

    MessageBox("Erreur Bind Word.Basic") ;
    // CoUninitialize() ;
    return ;
  }

  CmEditer() ;

  ps = string("Word interface started") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;
}

void
NSAutoWordView::CmEditer()
{
try
{
  string ps = string("Word text: entering CmEditer") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  wchar_t wcFileName[1024] ;
  if ((_pControl) && (_pControl->IsBound()))  {
    // on ne controle pas si Word est déjà ouvert
    // car il faudrait rajouter un hook pour détecter la fermeture
    // on se contente d'intercepter les exceptions XOLE
    if (false == _bSessionEnCours)    {
      _pControl->AppShow() ;
      _bSessionEnCours = true ;

      if (_pDocTtx->pDocInfo == 0)
      {
        // on regarde ici si on a un fichier externe
        // Si c'est le cas, on l'ouvre directement sans
        // passer par FileNew pour n'avoir qu'une fenetre Word
        if (!strcmp(_pDocTtx->szFichierExterne, ""))
        {
          ps = string("Word text: FileNew") ;
          _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

          TAutoString asTmplNormal(L"Normal");
          _pControl->FileNew(asTmplNormal);
        }
        else
        {
          ps = string("Word text : FileOpen ") + string(_pDocTtx->szFichierExterne) ;
          _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

          swprintf(wcFileName, L"%S", _pDocTtx->szFichierExterne);
          TAutoString asFileName(wcFileName);
          _pControl->FileOpen(asFileName);
        }

        // on met un titre provisoire...        _pDocTtx->SetTitle("Document Word (en cours)");
      }
      else
      {
        ps = string("Word text : FileOpen ") + string(_pDocTtx->nomFichier) ;
        _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

        swprintf(wcFileName, L"%S", _pDocTtx->nomFichier);
        TAutoString asFileName(wcFileName);
        _pControl->FileOpen(asFileName);
        // pDocName->SetText(pDocTtx->GetTitle());
      }

      // TAutoString* pWindowName = new TAutoString(pControl->WindowName());      // swprintf(wcWindowName, L"%S", pWindowName);
      // on affiche le titre du document      SetDocTitle(_pDocTtx->GetTitle(), 0) ;
    }
    else
      MessageBox("Impossible d'éditer car une session est en cours...") ;
  }
  else
    MessageBox("Impossible de lancer Word...") ;

  ps = string("Word text: leaving CmEditer") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
}
catch (const TXOle& e)
{
  string sExept = "Exception NSAutoWordView::CmEditer " + string(e.what()) ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&sExept, 1, NSSuper::trError) ;

  sExept = string("Nautilus a perdu le contrôle de Word dans la fonction d'édition. ") +
           string("Pour fermer Word, vous devez utiliser <Quitter> dans la fenetre <Document Word> de Nautilus.");
  erreur(sExept.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("Exception NSAutoWordView::CmEditer.", standardError, 0) ;
}
}void NSAutoWordView::CmFileOpen(){
try
{
	string sPath = _pDocTtx->pContexte->PathName("NLTR") ;
	char path[1024] ;
	strcpy(path, sPath.c_str()) ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
																	"Fichiers texte (*.doc)|*.doc;|",
																	0,
																	path,
																	"*.doc");

	int iUserChoice = TFileOpenDialog(this, initData).Execute() ;

  _pDocTtx->pContexte->reallocateBaseDirectory() ;

	if (IDOK != iUserChoice)
		return ;

  string ps = string("Word text : FileOpen for model ") + string(initData.FileName) ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  TWordBasicProxy secondControl ;

  try
  {
    ps = string("NSAutoWordView::CmFileOpen Bind Word.Basic for source document") ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    secondControl.Bind("Word.Basic") ;
  }
  catch (...)
  {
    ps = string("NSAutoWordView::CmFileOpen Error Binding Word.Basic for source document") ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

    MessageBox("Erreur Bind Word.Basic") ;
    return ;
  }

  wchar_t wcFileName[1024] ;
  swprintf(wcFileName, L"%S", initData.FileName) ;
  TAutoString asFileName(wcFileName) ;
  secondControl.FileOpen(asFileName) ;

  // Selects the entire document, copy its content and close it
  //
  secondControl.EditSelectAll() ;
  secondControl.EditCopy() ;
  secondControl.DocClose() ;

  secondControl.Unbind() ;

  // Paste in current document
  //
  _pControl->EditPaste() ;

  ReplaceTags() ;

  // if (_pDocTtx->HaveToReplaceTags())
	//	CmAutoFilling() ;

}
catch (const TXOle& e)
{
  string sExept = "Exception NSAutoWordView::CmFileOpen " + string(e.what()) ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&sExept, 1, NSSuper::trError) ;

  sExept = string("Nautilus a perdu le contrôle de Word dans la fonction d'archivage. ") +
           string("Pour fermer Word, vous devez utiliser <Quitter> dans la fenetre <Document Word> de Nautilus.");
  erreur(sExept.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("Exception NSAutoWordView::CmFileOpen.", standardError, 0) ;
}
}
void
NSAutoWordView::ReplaceTags(){  wchar_t wcOpenTag[3] ;  swprintf(wcOpenTag, L"%S", "[$") ;  wchar_t wcCloseTag[3] ;  swprintf(wcOpenTag, L"%S", "$]") ;  map<string, string> tagsBuffer ;

  GotoStartOfDoc() ;
  
  wchar_t wcTestTag[3] ;
  swprintf(wcTestTag, L"%S", "[$") ;
  _pControl->EditFind(TAutoVal(wcTestTag)) ;

  short iFound = _pControl->EditFindFound() ;
  while (iFound)
  {
    short shTagStartRow = GetSelStartRow() ;
    short shTagStartCol = GetSelStartCol() ;

    _pControl->EditFind(TAutoVal(wcCloseTag)) ;
    short iEndFound = _pControl->EditFindFound() ;
    if (0 == iEndFound)
      return ;

    short shTagEndRow = GetSelEndRow() ;
    short shTagEndCol = GetSelEndCol() ;
  }

/*
	size_t pos1 = sTexte.find("[$") ;

	while (pos1 != string::npos)
	{
		size_t pos2 = sTexte.find("$]", pos1+2) ;

		if (pos2 != string::npos)
    {
    	string sTag    = string(sTexte, pos1+2, pos2-pos1-2) ;
      string sValeur = string("") ;

      if ((strlen(sTag.c_str()) > 8) && (string("docTitle") == string(sTag, 0, 8)))
      {
      	string sDocTitle = string(sTag, 8, strlen(sTag.c_str()) - 8) ;
        strip(sDocTitle) ;
        pDocTtx->SetDocTitle(sDocTitle) ;
      }
      else if (string("nomPatient") == sTag)
      	sValeur = pDocTtx->pContexte->getPatient()->getNomLong() ;
      else if (string("dateJour") == sTag)
      {
      	string sLang = "" ;
        if (pDocTtx->pContexte->getUtilisateur())
        	sLang = pDocTtx->pContexte->getUserLanguage() ;

        char dateJ[9], message[11] ;
        donne_date_duJour(dateJ) ;
        donne_date(dateJ, message, sLang) ;
        sValeur = string(message) ;
      }
      else
      {
      	sValeur = sTag ;
        NSPatPathoArray* pPatPathoArray = NULL ;
        pDocTtx->pContexte->getPatient()->remplaceTagsChemin(sValeur, pPatPathoArray, &tagsBuffer, this->HWindow) ;
        if (pPatPathoArray)
					delete pPatPathoArray ;
      }

      // on remplace le texte directement dans le controle
      DeleteSubText(pos1, pos1 + strlen(sTag.c_str()) + 4) ;
      Insert(sValeur.c_str()) ;
      // on effectue la meme modif dans la string pour le calcul des positions
      sTexte.replace(pos1, strlen(sTag.c_str()) + 4, sValeur.c_str()) ;
      pos1 = sTexte.find("[$", pos1 + strlen(sValeur.c_str())) ;
    }
    else
    {
    	string sErrMsg = pDocTtx->pContexte->getSuperviseur()->getText("wordProcessorManagement", "noClosingTagFound") ;
			erreur(sErrMsg.c_str(), standardError, 0) ;
      // No need to continue since there is no ending sequence left
      return ;
    }
	}
*/
}

voidNSAutoWordView::CmArchiver()
{
try{  string ps = string("User saves a Word text") ;  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;  string sNomFichier, sLocalis;

  short format;  wchar_t wcFormatName[1024];
  wchar_t wcFileName[1024];

  if (false == _bSessionEnCours)  {
    string sMessage = string("Vous devez avoir un fichier ouvert dans Word avant de pouvoir l'archiver dans ") + _pDocTtx->pContexte->getSuperviseur()->getAppName().c_str();
    MessageBox(sMessage.c_str());
    return;
  }

  // on enregistre le document au format HTML pour pouvoir le publier  if (_pDocTtx->pDocInfo == 0)
  {
    _pDocTtx->Renommer(sNomFichier, sLocalis, "ZTHTM") ;

    // Note : Referencer remet à jour la donnée Title du document    if (_pDocTtx->Referencer("ZTHTM", "", sNomFichier, sLocalis))
    {
      // on sauvegarde en html sous la bonne localisation
      wcscpy(wcFormatName, L"HTML");

      TAutoString asFormatName(wcFormatName);
      format = _pControl->ConverterLookup(asFormatName);

      // sprintf(msg, "Format HTML = <%hd>", format);      // MessageBox(msg);

      swprintf(wcFileName, L"%S", _pDocTtx->nomFichier);
      TAutoString asFileName(wcFileName);      _pControl->FileSaveAs(asFileName, format);

      // on met à jour le titre      // pDocName->SetText(pDocTtx->GetTitle());

      SetDocTitle(_pDocTtx->GetTitle(), 0);    }
  }
  else
  {
    // on sauvegarde uniquement car le fichier est ouvert
    // sous le bon format
    _pControl->FileSave();
  }

  ps = string("Leaving Word text save function") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
}
catch (const TXOle& e)
{
  string sExept = string("Nautilus a perdu le contrôle de Word dans la fonction d'archivage. ") +
                  string("Pour fermer Word, vous devez utiliser <Quitter> dans la fenetre <Document Word> de Nautilus.");
  erreur(sExept.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("Exception NSAutoWordView::CmArchiver.", standardError, 0) ;
}
}
bool
NSAutoWordView::saveHTMLtoWORD(string sWordName)
{
try
{
	if (strlen(sWordName.c_str()) > 254)
	{
		string sMessage = string("HTMLtoWORF, nom de fichier trop long : ") + sWordName ;
		MessageBox(sMessage.c_str()) ;
		return false ;
	}

	short format ;
	wchar_t wcFormatName[1024] ;
	wchar_t wcFileName[1024] ;

	if (false == _bSessionEnCours)	{
		string sMessage = string("Vous devez avoir un fichier ouvert dans Word avant de pouvoir l'archiver dans ") + _pDocTtx->pContexte->getSuperviseur()->getAppName().c_str() ;
		MessageBox(sMessage.c_str()) ;
		return false ;
	}

	// on sauvegarde en html sous la bonne localisation	wcscpy(wcFormatName, L"DOC") ;

	TAutoString asFormatName(wcFormatName) ;
	format = _pControl->ConverterLookup(asFormatName) ;

	// sprintf(msg, "Format HTML = <%hd>", format);	// MessageBox(msg);

	swprintf(wcFileName, L"%S", sWordName.c_str()) ;
	TAutoString asFileName(wcFileName) ;	_pControl->FileSaveAs(asFileName, format) ;

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
  erreur("Exception NSAutoWordView::saveHTMLtoWORD", standardError, 0) ;
  return false ;
}
}
// Fonction de réponse à la commande Composer////////////////////////////////////////////////////////////////
void
NSAutoWordView::CmComposer()
{
	// pDocTtx->Composer();
	MessageBox("Pour la composition Word, archivez le document et ouvrez le en mode visualisation.");
}

// Fonction de réponse à la commande Publier////////////////////////////////////////////////////////////////
void
NSAutoWordView::CmPublier()
{
  string ps = string("User publishes a Word text") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	_pDocTtx->Publier();
}

// Fonction de réponse à la commande Visualiser////////////////////////////////////////////////////////////////
void
NSAutoWordView::CmVisualiser()
{
  string ps = string("User display the HTML version of a Word text") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  TView* pView ;
  NSVisualView* pVisualView ;

  if (_pDocTtx->pDocInfo)  // si le document est référencé  {
    // s'il existe une vue visual, on l'active et on la rafraichit
    pView = _pDocTtx->pContexte->getPatient()->getDocHis()->ActiveFenetre(_pDocTtx->pDocInfo, "NSVisualView");

    if (pView)    {
      pVisualView = dynamic_cast<NSVisualView*>(pView);
      pVisualView->Rafraichir();
    }
    else // on lance une visualisation
      _pDocTtx->Visualiser();
  }
  else
    MessageBox("Le document doit être archivé avant de pouvoir le visualiser");
}

// Fonction de réponse à la commande Formule////////////////////////////////////////////////////////////////
void
NSAutoWordView::CmFormules()
{
try{  string ps = string("User adds formulas to a Word text") ;  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;  wchar_t wcText[1024];
  string sTexte = "";

  if (false == _bSessionEnCours)  {
    MessageBox("Word doit déjà être ouvert pour pouvoir insérer des formules.") ;
    return ;
  }

  LettreTypeDialog *pLTDlg = new LettreTypeDialog(this, _pDocTtx->pContexte) ;
  if (pLTDlg->Execute() == IDOK)  {
    if (pLTDlg->choixIntro >= 0)
      sTexte += pLTDlg->sIntro + string("\n\n") ;

    if (pLTDlg->choixCorps >= 0)      sTexte += pLTDlg->sCorps + string("\n\n") ;

    if (pLTDlg->choixPolit >= 0)      sTexte += pLTDlg->sPolit + string("\n\n") ;
  }

  delete pLTDlg ;
  swprintf(wcText, L"%S", sTexte.c_str()) ;  TAutoString asText(wcText) ;
  _pControl->Insert(asText) ;
}catch (const TXOle& e)
{
  string sExept = string("Nautilus a perdu le contrôle de Word dans la fonction d'édition des formules. ") +
                  string("Pour fermer Word, vous devez utiliser <Quitter> dans la fenetre <Document Word> de Nautilus.");
  erreur(sExept.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("Exception NSAutoWordView::CmFormules.", standardError, 0) ;
}
}
voidNSAutoWordView::CmQuitter()
{
  string ps = string("User closes a Word text") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  CloseWindow() ;
}

bool
NSAutoWordView::GotoBookmark(string sBookmarkName)
{
  // Predefined bookmarks available in Word.
  //
  // \Sel 	        Current selection or the insertion point.
  // \PrevSel1 	    Most recent selection where editing occurred; going to this bookmark is equivalent to running the GoBack method once.
  // \PrevSel2 	    Second most recent selection where editing occurred; going to this bookmark is equivalent to running the GoBack method twice.
  // \StartOfSel 	  Start of the current selection.
  // \EndOfSel 	    End of the current selection.
  // \Line 	        Current line or the first line of the current selection. If the insertion point is at the end of a line that is not the last line in the paragraph, the bookmark includes the entire next line.
  // \Char 	        Current character, which is the character following the insertion point if there is no selection, or the first character of the selection.
  // \Para 	        Current paragraph, which is the paragraph containing the insertion point or, if more than one paragraph is selected, the first paragraph of the selection. Note that if the insertion point or selection is in the last paragraph of the document, the "\Para" bookmark does not include the paragraph mark.
  // \Section 	    Current section, including the break at the end of the section, if any. The current section contains the insertion point or selection. If the selection contains more than one section, the "\Section" bookmark is the first section in the selection.
  // \Doc 	        Entire contents of the active document, with the exception of the final paragraph mark.
  // \Page 	        Current page, including the break at the end of the page, if any. The current page contains the insertion point. If the current selection contains more than one page, the "\Page" bookmark is the first page of the selection. Note that if the insertion point or selection is in the last page of the document, the "\Page" bookmark does not include the final paragraph mark.
  // \StartOfDoc 	  Beginning of the document.
  // \EndOfDoc 	    End of the document.
  // \Cell 	        Current cell in a table, which is the cell containing the insertion point. If one or more cells of a table are included in the current selection, the "\Cell" bookmark is the first cell in the selection.
  // \Table 	      Current table, which is the table containing the insertion point or selection. If the selection includes more than one table, the "\Table" bookmark is the entire first table of the selection, even if the entire table is not selected.
  // \HeadingLevel 	The heading that contains the insertion point or selection, plus any subordinate headings and text. If the current selection is body text, the "\HeadingLevel" bookmark includes the preceding heading, plus any headings and text subordinate to that heading.

try
{
  wchar_t *wcTopDocTag = new wchar_t[strlen(sBookmarkName.c_str())] ;
  
  swprintf(wcTopDocTag, L"%S", sBookmarkName.c_str()) ;
  _pControl->EditGoTo(TAutoVal(wcTopDocTag)) ;

  delete[] wcTopDocTag ;
  return true ;
}
catch (const TXOle& e)
{
  string ps = string("Exception NSAutoWordView::GotoBookmark") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
}
catch (...)
{
  string ps = string("Exception NSAutoWordView::GotoBookmark") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  return false ;
}
}

short
NSAutoWordView::GetSelInfo(short iType)
{
  // Selection here means either a true selection if any, or cursor position
  //
  //  2 Returns section number
  //  3 Returns page number
  //  5 returns the number of twips left of the selection.
  // 10 Number of lines in document
  // 12 Returns -1 if selection is within a table
  // 13 Row of selection start
  // 14 Row of selection end
  // 15 Last Row ?
  // 16 Column of selection start
  // 17 Column of selection end
  // 18 Last column
  //
  // 33 Donne le type d'en-tête ou de pied de page où se trouve la sélection.
  //     -1 la sélection ne se trouve ni dans une en-tête de page ni dans un pied de page
  //     0(zéro) La sélection se trouve dans l' en-tête de page paire
  //     1 La sélection se trouve dans l' en-tête de page impaire
  //     2 La sélection se trouve dans le pied de page paire
  //     3 La sélection se trouve dans le pied de page impaire
  //     4 La sélection se trouve dans le première en-tête de page
  //     5 La sélection se trouve dans le premier pied de page
  // 34 Retourne -1 si le document actif est un document maître (il est reconnu en tant que tel s'il contient au moins un sous document)
  // 35 Retourne -1 si la sélection se trouve dans le panneau de pied de page ou dans le pied de page en mode page.
  // 36 Retourne -1 si la sélection se trouve dans le panneau de notes de fin ou dans les notes de fin en mode page.

try
{
  return _pControl->SelInfo(iType) ;
}
catch (const TXOle& e)
{
  string ps = string("Exception NSAutoWordView::GetSelInfo") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
}
catch (...)
{
  string ps = string("Exception NSAutoWordView::GetSelInfo") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  return false ;
}
}

/******************************************************************************/
//					METHODES DE NSTtx Document / Vue/******************************************************************************/
// Constructeur NSTtxDocument////////////////////////////////////////////////////////////////

NSTtxDocument::NSTtxDocument(TDocument* parent, bool bROnly, NSDocumentInfo* pDocumentInfo,                             NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx)
              :NSRefDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, bROnly)
{
	// Mise à vide du nom de fichier
  SetNomFichier("") ;
  SetFichierExterne("") ;

  if (NULL == pDocumentInfo)
		sDocTitle = string("") ;
  else
  	sDocTitle = pDocumentInfo->getDocName() ;

	Open(0, "") ;
  bMustReplaceTagsWhenOpening = true ;
}

NSTtxDocument::NSTtxDocument(TDocument *parent, NSContexte *pCtx)              :NSRefDocument(parent, pCtx)
{
	// Mise à vide du nom de fichier
  SetNomFichier("") ;
  SetFichierExterne("") ;
  sDocTitle = string("") ;

	Open(0, "") ;
  bMustReplaceTagsWhenOpening = true ;
}

void NSTtxDocument::SetNomFichier(string sFich){
	strcpy(nomFichier, sFich.c_str()) ;
}

void NSTtxDocument::SetFichierExterne(string sFich)
{
	strcpy(szFichierExterne, sFich.c_str()) ;
}

string NSTtxDocument::GetDocExt(){
	size_t i ;
	string sExt = "" ;

	for (i = 0; (i < strlen(nomFichier)) && (nomFichier[i] != '.'); i++) ;

  if (i < strlen(nomFichier))
  {
  	// nomDoc[i] == '.'
    i++ ;
    while (i < strlen(nomFichier))
    	sExt += nomFichier[i++] ;
  }

  return sExt ;
}

// Ouverture du document////////////////////////////////////////////////////////////////

bool NSTtxDocument::Open(int /*mode*/, LPCSTR path){
	string sNomFichier = "" ;

	// Si il existe, prise du nom de fichier de la fiche document  ValideFichier(&sNomFichier) ;

	// Sinon, on sort	if (!bDocumentValide)
		return false ;

	//	// Rangement du nom de fichier dans le document
	//
	SetNomFichier(sNomFichier) ;
  SetDirty(false) ;
  return true ;
}

// Fermeture du document////////////////////////////////////////////////////////////////
bool NSTtxDocument::Close()
{
	// if (TDocument::Close())
	// 	 return false;

	SetNomFichier("") ;
	return true ;
}

// Génération du html correspondant au document
////////////////////////////////////////////////////////////////
bool
NSTtxDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	NSHtmlRTF hrtf(typeOperation, this, nomFichier, pContexte, sAdresseCorresp) ;
	string    sBaseImg = string("") ;
	// NSBaseImages* 	pBase;

	// on trouve le nom du fichier temporaire à visualiser
	sNomHtml = hrtf.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	string sFichierHtml = sPathHtml + sNomHtml ;

	if (pDocInfo->getTypeSem() == string("ZTRTF"))
		hrtf.Convertir() ;   // conversion RTF ==> HTML (inutile pour Word)

	// on passe aussi le path destination pour les images
	if (!hrtf.genereHtml(sFichierHtml, sBaseImg, pHtmlInfo, sPathDest))
		return false ;

	// Mise à jour de la base d'images
	switch (typeOperation)
	{
  	case toComposer:
    	sBaseCompo = sBaseImg ;
      break ;

		default:
    	sBaseImages = sBaseImg ;
	}

	return true ;
}

bool
NSTtxDocument::GenereHtmlText(string& sHtmlText)
{
  sHtmlText = string("") ;

  return true ;
}

// Indique si document ouvert (il existe un fichier associé)////////////////////////////////////////////////////////////////
bool NSTtxDocument::IsOpen()
{
	if (_fstricmp(nomFichier, ""))
		return true ;
	else
		return false ;
}

// Trouve un nom de fichier nautilus au document
////////////////////////////////////////////////////////////////
bool NSTtxDocument::Renommer(string& sNomFichier, string& sLocalis, string sType, string* psNomBrut)
{
	string sPath = "" ;
	string sExt = GetDocExt() ;

	if (!TrouveNomFichier(sType,sExt,sNomFichier,sLocalis))
	{
  	erreur("Pb à l'attribution d'un nouveau nom pour ce fichier", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

	if (psNomBrut)
	{
  	// Dans ce cas on renvoie aussi le nom brut (sans extension)
    size_t pos = sNomFichier.find('.') ;
    if (pos != NPOS)
    	*psNomBrut = string(sNomFichier, 0, pos) ;
    else
    	*psNomBrut = sNomFichier ;
	}

	sPath = pContexte->PathName(sLocalis) ;

	// on construit le nom complet pour la sauvegarde
	SetNomFichier(sPath + sNomFichier) ;

	SetDirty(true) ;

	return true ;
}

// Table de réponses de la classe NSTtxView////////////////////////////////////////////////////////////////

DEFINE_RESPONSE_TABLE2(NSTtxView, TView, TRichEdit)  EV_VN_ISWINDOW,
  // EV_VN_COMMIT,
  // EV_VN_REVERT,
  EV_WM_CHAR,
  EV_WM_CLOSE,
  EV_WM_DESTROY,
  EV_WM_SETFOCUS,
  EV_WM_KILLFOCUS,
  EV_WM_RBUTTONDOWN,
  EV_WM_QUERYENDSESSION,

  EV_COMMAND(CM_COMPOSE,				    CmComposer),  EV_COMMAND(CM_IMPRIME,				    CmPublier),
  EV_COMMAND(CM_VISUAL,					    CmVisualiser),
  EV_COMMAND(CM_ENREGISTRE,				  CmFileSave),
  EV_COMMAND(CM_FORMULE,				    CmFormules),
  EV_COMMAND(IDM_NAUTIDOS,				  CmNautidos),
  EV_COMMAND(CM_LETTRETYPE,				  CmFileOpen),  EV_COMMAND(CM_EDITCOPY,				    CmEditCopy),
  EV_COMMAND(CM_EDITCUT,				    CmEditCut),
  EV_COMMAND(CM_EDITPASTE,				  CmEditPaste),
  EV_COMMAND(CM_FILENEW, 				    CmFileNew),
  EV_COMMAND(CM_FILEOPEN, 				  CmFileOpen),
  EV_COMMAND(CM_FILESAVE, 				  CmFileSave),
  EV_COMMAND(CM_FILESAVEAS, 			  CmFileSaveAs),
  EV_COMMAND(CM_FORMATFONT, 			  CmFormatFont),
  EV_COMMAND(CM_FORMATPARAGRAPH, 		CmFormatPara),
  EV_COMMAND(CM_FORMATTABS,				  CmFormatTabs),
  EV_COMMAND(CM_FORMATBOLD,				  CmFormatBold),
  EV_COMMAND(CM_FORMATITALIC,			  CmFormatItalic),
  EV_COMMAND(CM_FORMATUNDERLINE,		CmFormatUnderline),
  EV_COMMAND(CM_FORMATSTRIKEOUT,		CmFormatStrikeout),
  EV_COMMAND(CM_FORMATTEXTDATA,			CmFormatTextData),
  EV_COMMAND(CM_FORMATRTFDATA,			CmFormatRTFData),

  EV_COMMAND_ENABLE(CM_FORMATFONT, 			CeFormatFont),  EV_COMMAND_ENABLE(CM_FORMATPARAGRAPH, CeFormatPara),
  EV_COMMAND_ENABLE(CM_FORMATTABS, 			CeFormatTabs),
  EV_COMMAND_ENABLE(CM_FORMATBOLD, 			CeFormatBold),
  EV_COMMAND_ENABLE(CM_FORMATITALIC, 		CeFormatItalic),
  EV_COMMAND_ENABLE(CM_FORMATUNDERLINE, CeFormatUnderline),
  EV_COMMAND_ENABLE(CM_FORMATSTRIKEOUT, CeFormatStrikeout),
  EV_COMMAND_ENABLE(CM_FORMATTEXTDATA, 	CeFormatTextData),
  EV_COMMAND_ENABLE(CM_FORMATRTFDATA, 	CeFormatRTFData),

  EV_EN_DROPFILES(IDRichEd, EnDropFiles),  EV_EN_MSGFILTER(IDRichEd, EnMsgFilter),
  EV_EN_PROTECTED(IDRichEd, EnProtected),
  EV_EN_REQRESIZE(IDRichEd, EnReqResize),

END_RESPONSE_TABLE;
// Constructeur NSTtxView
////////////////////////////////////////////////////////////////

NSTtxView::NSTtxView(NSTtxDocument& doc, TWindow* parent)          :TView(doc), pDocTtx(&doc), TRichEdit(parent,GetNextViewId(),"",0,0,0,0){
	Attr.AccelTable = IDA_RICHEDIT ;

  pMUEViewMenu = 0 ;
  hAccelerator = 0 ;

  TMyApp* pMyApp = pDocTtx->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->setMenu(string("menubar_txt"), &hAccelerator) ;

	pToolBar = 0 ;  pPrinter = 0 ;
  bSetupToolBar = true ;
}

// Destructeur NSTtxView////////////////////////////////////////////////////////////////

NSTtxView::~NSTtxView(){
	if (pMUEViewMenu)
  	delete pMUEViewMenu ;

  if (0 != hAccelerator)
  {
    DestroyAcceleratorTable(hAccelerator) ;
    hAccelerator = 0 ;
  }

	if (pPrinter)
  	delete pPrinter ;
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)
////////////////////////////////////////////////////////////////

TWindow* NSTtxView::GetWindow(){
  return (TWindow*) this ;
}

// Appel de la fonction de remplissage de la vue////////////////////////////////////////////////////////////////

void NSTtxView::SetupWindow(){
	LimitText(MAXCARS) ;
  TRichEdit::SetupWindow() ;

  // Change font to ANSI Variable
  HFONT hFont = HFONT(::GetStockObject(ANSI_VAR_FONT)) ;
  if (hFont)
  	SetWindowFont(hFont,true) ;

    /************************************* Sélection des icones
    //   TModule* module, TResId
    TModule* IconModule = GetModule();
  	TResId 	IconResId  = TTX_ICON;
    HICON   	CurIcon;

  	HINSTANCE hInstance = IconModule ? HINSTANCE(*IconModule) : HINSTANCE(0);

  	if (IconResId != 0)
    	CurIcon = TUser::LoadIcon(hInstance, IconResId);

  	if (CurIcon && IsWindow())
    	SendMessage(WM_SETICON, true, (LPARAM)(HICON)CurIcon);
    //SetClassLong(GCL_HICON,   (LONG) GetModule()->LoadIcon(MAKEINTRESOURCE(TTX_ICON)));
    //SetClassLong(GCL_HICONSM, (LONG) GetModule()->LoadIcon(MAKEINTRESOURCE(TTX_ICON_CS)));
    **************************************************************/

	FillViewData() ;  if ((strcmp(pDocTtx->szFichierExterne, "") != 0) && (pDocTtx->HaveToReplaceTags()))  	CmAutoFilling() ;}

voidNSTtxView::SetupTxtBar()
{
	TMyApp* pMyApp = pDocTtx->pContexte->getSuperviseur()->getApplication() ;

	pMyApp->FlushControlBar() ;

	pMyApp->cb2->Insert(*new NSFlatButtonGadget(CM_EDITUNDO,   CM_EDITUNDO,   TButtonGadget::Command)) ;	pMyApp->cb2->Insert(*new NSFlatButtonGadget(CM_EDITCUT,    CM_EDITCUT,    TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new NSFlatButtonGadget(CM_EDITCOPY,   CM_EDITCOPY,   TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new NSFlatButtonGadget(CM_EDITPASTE,  CM_EDITPASTE,  TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new NSFlatSeparatorGadget(6)) ;
	pMyApp->cb2->Insert(*new NSFlatButtonGadget(CM_FORMULE,    CM_FORMULE,    TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new NSFlatButtonGadget(CM_LETTRETYPE, CM_LETTRETYPE, TButtonGadget::Command)) ;
  string sFichDat = pDocTtx->pContexte->PathName("IDOS") ;
  if (string("") != sFichDat)
    pMyApp->cb2->Insert(*new NSFlatButtonGadget(IDM_NAUTIDOS, IDM_NAUTIDOS, TButtonGadget::Command)) ;

  pMyApp->cb2->Insert(*new NSFlatSeparatorGadget(6)) ;
  pMyApp->cb2->Insert(*new TButtonGadget(CM_FORMATBOLD,      CM_FORMATBOLD,      TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new TButtonGadget(CM_FORMATITALIC,    CM_FORMATITALIC,    TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new TButtonGadget(CM_FORMATUNDERLINE, CM_FORMATUNDERLINE, TButtonGadget::Command)) ;

  pMyApp->cb2->LayoutSession() ;}

bool
NSTtxView::PreProcessMsg(MSG &msg)
{
  PRECONDITION(Handle) ;
  return hAccelerator ? ::TranslateAccelerator(Handle, hAccelerator, &msg) : false ;
}

// Remplissage de la vue////////////////////////////////////////////////////////////////
void
NSTtxView::FillViewData()
{
	if (pDocTtx->IsOpen())
  {
  	TRichEdit::ReplaceWith(pDocTtx->nomFichier) ;
    TRichEdit::ClearModify() ;
    pDocTtx->SetDirty(false) ;
  }
  else if (strcmp(pDocTtx->szFichierExterne, "") != 0)  {  	CmOuvrir() ;  }  else
  {
  	InitDefaultFont() ;
    SetFormatFont(_defaultFont) ;
  }
}

// Loading default font (from lettres.dat)////////////////////////////////////////////////////////////////
void
NSTtxView::InitDefaultFont(){
  // Set default parameters
  //
  _defaultFont.lfHeight = -12; // setup font characteristics
  _defaultFont.lfWidth = 0;
  _defaultFont.lfEscapement = 0;
  _defaultFont.lfOrientation = 0;
  _defaultFont.lfWeight = FW_NORMAL;
  _defaultFont.lfItalic = FALSE;
  _defaultFont.lfUnderline = FALSE;
  _defaultFont.lfStrikeOut = FALSE;
  _defaultFont.lfCharSet = ANSI_CHARSET;
  _defaultFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
  _defaultFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  _defaultFont.lfQuality = PROOF_QUALITY;
  _defaultFont.lfPitchAndFamily = DEFAULT_PITCH || FF_ROMAN;
  strcpy(_defaultFont.lfFaceName, "Times New Roman") ;

  // Open parameters file
  //
  ifstream inFile ;

  string sFichDat = pDocTtx->pContexte->PathName("FPER") + string("lettres.dat") ;
  inFile.open(sFichDat.c_str()) ;
  if (!inFile)
  {
    string sErrorMsg = pDocTtx->pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorMsg += string(" ") + sFichDat ;
    pDocTtx->pContexte->getSuperviseur()->trace(&sErrorMsg, 1, NSSuper::trError) ;
  	erreur(sErrorMsg.c_str(), standardError, 0, pDocTtx->pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  string line ;
  string sData = "" ;

  while (!inFile.eof())
  {
    getline(inFile, line) ;
    if (string("") != line)
      sData += line + "\n" ;
  }

  inFile.close() ;

  size_t i = 0 ;
  int    j = 0 ;

  string  sNomAttrib[3] = {"", "", ""} ;
  string  sValAttrib[3] = {"", "", ""} ;

  // boucle de chargement des attributs de police
  // loading font attributes
  //
  while ((i < strlen(sData.c_str())) && (j < 3))
  {
    while ((i < strlen(sData.c_str())) && (' ' != sData[i]))
        sNomAttrib[j] += sData[i++] ;

      while ((i < strlen(sData.c_str())) && (' ' == sData[i]))
        i++ ;

      while ((i < strlen(sData.c_str())) && ('\n' != sData[i]))
        sValAttrib[j] += sData[i++] ;

      i++ ;
      j++ ;
  }

  int iTaille = atoi((sValAttrib[1]).c_str()) ;

  if (0 == iTaille)
    iTaille = 10 ;

  // on positionne les paramètres de la police (LOGFONT)
  _defaultFont.lfHeight = -1 * iTaille ;

  if (strlen((sValAttrib[0]).c_str()) > (LF_FACESIZE - 1))
    erreur("La police par défaut ne peut etre fixée (31 caractères max.)",standardError,0,pDocTtx->pContexte->GetMainWindow()->GetHandle()) ;
  else
    strcpy(_defaultFont.lfFaceName, (sValAttrib[0]).c_str()) ;

  string sGraisse = sValAttrib[2] ;

  // Nothing to do: default already set
  // if (string("N") == sGraisse)

  if (NPOS != sGraisse.find("G"))
    _defaultFont.lfWeight = FW_BOLD ;
  if (NPOS != sGraisse.find("I"))
    _defaultFont.lfItalic = TRUE ;
  if (NPOS != sGraisse.find("S"))
    _defaultFont.lfUnderline = TRUE ;
}

void
NSTtxView::SetFormatFont(LOGFONT lf){
  // Retrouve format caractère courant
  TCharFormat chFormat(*this) ;
  chFormat.EnableBold(false) ;

  chFormat.GetFontInfo(lf) ;

  // on fixe la nouvelle fonte
  // chFormat.GetFontInfo(lf) ;

  int iTextLen = GetTextLen() ;
  SetSelection(iTextLen, iTextLen) ;

  SetCharFormat(chFormat) ;
}


// Fonction CanClose : Appel de CanClose du document////////////////////////////////////////////////////////////////
bool NSTtxView::CanClose()
{
	TMyApp* pMyApp = pDocTtx->pContexte->getSuperviseur()->getApplication() ;

	if (!CanClear())
		return false ;

	pMyApp->FlushControlBar() ;
	bSetupToolBar = false ;	return true ;
}

bool NSTtxView::CanClear()
{
	if (false == TRichEdit::IsModified())
		return true ;

  char far cfNomDocument[1024] = "" ;

	// si le fichier n'est pas référencé, on prend le nom de fichier (si il existe)
	if (0 == pDocTtx->pDocInfo)
	{
		if (FileName)
    	strcpy(cfNomDocument, FileName) ;
	}
	else // sinon on prend le nom de document
		strcpy(cfNomDocument, pDocTtx->GetTitle()) ;

	string sCaption = string("Message ") + pDocTtx->pContexte->getSuperviseur()->getAppName().c_str();  string sMsgTxt = pDocTtx->pContexte->getSuperviseur()->getText("wordProcessorManagement", "doYouWantToSaveChangesToDocument%s") ;  char msg[1024] ;	sprintf(msg, sMsgTxt.c_str(),             FileName ? (const char far*)cfNomDocument : "texte créé");

	int result = MessageBox(msg, sCaption.c_str(), MB_YESNOCANCEL|MB_ICONQUESTION) ;	return (result == IDYES ? Enregistrer() : result != IDCANCEL);
}

bool NSTtxView::VnIsWindow(HWND hWnd){
	return HWindow == hWnd ;
}

/****************************************bool NSTtxView::VnCommit(bool force)
{
   return true;
}

bool NSTtxView::VnRevert(bool clear){
	FillViewData();
   return true;
}

*******************************************/
void NSTtxView::EvRButtonDown(uint /*modkeys*/, NS_CLASSLIB::TPoint& point){
	OWL::TPopupMenu *menu = new OWL::TPopupMenu();

  if (TRichEdit::HasSelection())
  {
  	menu->AppendMenu(MF_STRING, CM_EDITCUT,     "Couper");
    menu->AppendMenu(MF_STRING, CM_EDITCOPY,    "Copier");
  }
  else
  {
  	menu->AppendMenu(MF_GRAYED, 0,  "Couper");
    menu->AppendMenu(MF_GRAYED, 0,  "Copier");
  }
  menu->AppendMenu(MF_STRING, CM_EDITPASTE,   "Coller");
  menu->AppendMenu(MF_SEPARATOR, 0, 0);
  menu->AppendMenu(MF_STRING, CM_FORMATFONT,  "Police...");
  menu->AppendMenu(MF_STRING, CM_FORMULE,     "Insérer formule");

  ClientToScreen(point) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;
  delete menu ;
}

void NSTtxView::EvChar(uint key, uint repeatCount, uint flags){
	TRichEdit::EvChar(key,repeatCount,flags) ;
  pDocTtx->SetDirty(true) ;
}

// Fonctions EvSetFocus et EvKillFocus pour la control Bar
////////////////////////////////////////////////////////////////
void
NSTtxView::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
{
	// activateMUEViewMenu() ;

	TMyApp* pMyApp = pDocTtx->pContexte->getSuperviseur()->getApplication() ;

	TRichEdit::EvSetFocus(hWndLostFocus) ;

  pMyApp->setMenu(string("menubar_txt"), &hAccelerator) ;

  if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
  {
  	SetupTxtBar() ;
    pMyApp->SetToolBarWindow(GetWindow()) ;
  }
}

voidNSTtxView::EvKillFocus(THandle hWndGetFocus /* may be 0 */)
{
	TRichEdit::EvKillFocus(hWndGetFocus);
}

boolNSTtxView::EvQueryEndSession()
{
  return TWindow::EvQueryEndSession() ;
}

void
NSTtxView::EvDestroy()
{
  if (pDocTtx->pContexte->getBBinterface())
    pDocTtx->pContexte->getBBinterface()->DisconnectInterface(HWindow) ;
}

// Fonction Enregistrer////////////////////////////////////////////////////////////////
bool NSTtxView::Enregistrer()
{
	bool 	 bRef = false ;
	bool	 bSaveOk ;
	int  	 retVal ;
	string sFichier = pDocTtx->nomFichier ;
	string sNomFichier ;
	string sLocalis ;

	// si le fichier n'est pas référencé
	if (pDocTtx->pDocInfo == 0)
	{
  	// On doit le référencer
    bRef = true ;

    // si ce n'est pas un nouveau fichier (nomFichier != NULL)
    if (sFichier != "")
    {
    	retVal = MessageBox("Voulez-vous importer le document ?", "Message", MB_YESNOCANCEL);

      if (retVal == IDCANCEL)
      	return false ;

      if (retVal == IDYES) // cas de l'importation      	pDocTtx->Renommer(sNomFichier, sLocalis) ;
      else // cas du référencement
      {
      	sNomFichier = sFichier ;
        sLocalis = "" ;
      }
    }
    else // Nouveau fichier : on le renomme
    	pDocTtx->Renommer(sNomFichier, sLocalis) ;
	}

	if (bRef)
	{
  	if (pDocTtx->Referencer("ZTRTF", pDocTtx->GetDocTitle(), sNomFichier, sLocalis))
		{
    	TRichEdit::SetFileName(pDocTtx->nomFichier) ;
      bSaveOk = TRichEdit::Write() ;

      if (!bSaveOk)
      {
      	erreur("Erreur lors de la sauvegarde.", standardError, 0, pDocTtx->pContexte->GetMainWindow()->GetHandle()) ;
        pDocTtx->SetNomFichier(sFichier) ;
        return false ;
      }

      pDocTtx->SetDirty(false) ;
      // on remet le titre à jour
      SetDocTitle(pDocTtx->GetTitle(), 0) ;
    }
    else
    {
    	pDocTtx->SetDirty(true) ;
      // on remet le document dans l'état initial
      pDocTtx->SetNomFichier(sFichier) ;
    }
  }
  else // cas pDocInfo != 0 : on sauvegarde le fichier
  {
  	TRichEdit::SetFileName(pDocTtx->nomFichier) ;
    bSaveOk = TRichEdit::Write() ;

    if (!bSaveOk)
    {
    	erreur("Erreur lors de la sauvegarde.", standardError, 0, pDocTtx->pContexte->GetMainWindow()->GetHandle()) ;
      pDocTtx->SetNomFichier(sFichier) ;
      return false ;
    }

    pDocTtx->SetDirty(false) ;
    SetDocTitle(pDocTtx->GetTitle(), 0) ;	}

	return true ;}
// Fonction EnregistrerSous : Importe automatiquement////////////////////////////////////////////////////////////////
bool NSTtxView::EnregistrerSous()
{
	string sFichier = pDocTtx->nomFichier ;
	string sNomFichier ;
	string sLocalis ;
	bool	 bSaveOk ;

	pDocTtx->Renommer(sNomFichier, sLocalis) ;

	if (pDocTtx->Referencer("ZTRTF", pDocTtx->GetDocTitle(), sNomFichier, sLocalis))
	{
  	TRichEdit::SetFileName(pDocTtx->nomFichier) ;
    bSaveOk = TRichEdit::Write() ;

    if (!bSaveOk)
    {
    	erreur("Erreur lors de la sauvegarde.", standardError, 0, pDocTtx->pContexte->GetMainWindow()->GetHandle()) ;
      pDocTtx->SetNomFichier(sFichier) ;
      return false ;
    }

    pDocTtx->SetDirty(false) ;
    // on remet le titre à jour    SetDocTitle(pDocTtx->GetTitle(), 0) ;
	}
	else
	{
  	pDocTtx->SetDirty(true) ;
    // on remet le document dans l'état initial
    pDocTtx->SetNomFichier(sFichier) ;
	}

	return true ;
}

// Fonction de réponse à la commande Composer
////////////////////////////////////////////////////////////////
void NSTtxView::CmComposer()
{
	pDocTtx->Composer() ;
}

// Fonction de réponse à la commande Publier////////////////////////////////////////////////////////////////
void NSTtxView::CmPublier()
{
	pDocTtx->Publier() ;
}

// Fonction de réponse à la commande Visualiser
////////////////////////////////////////////////////////////////
void NSTtxView::CmVisualiser()
{
	pDocTtx->Visualiser() ;
}

// Fonction de réponse à la commande Formule
////////////////////////////////////////////////////////////////
void NSTtxView::CmFormules()
{
try
{
	LettreTypeDialog *pLTDlg = new LettreTypeDialog(this, pDocTtx->pContexte) ;

	if (pLTDlg->Execute() == IDOK)
	{
  	string sTexte = getText() ;

  	if (pLTDlg->choixIntro >= 0)
    	sTexte = pLTDlg->sIntro + string("\n\n") + sTexte ;

    if (pLTDlg->choixCorps >= 0)
    	sTexte += pLTDlg->sCorps + string("\n\n") ;

    if (pLTDlg->choixPolit >= 0)
    	sTexte += pLTDlg->sPolit + string("\n\n") ;

    SetText(sTexte.c_str()) ;
    pDocTtx->SetDirty(true) ;
	}
	delete pLTDlg ;}catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception TMyApp::CmOutils : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
	erreur("Exception TMyApp::CmOutils.", standardError, 0) ;
}}

void
NSTtxView::CmNautidos()
{
  ifstream inFile ;

  string sFichDat = pDocTtx->pContexte->PathName("IDOS") + string("GASTRODA") ;
  inFile.open(sFichDat.c_str()) ;
  if (!inFile)
  {
    string sErrorMsg = pDocTtx->pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorMsg += string(" ") + sFichDat ;
    pDocTtx->pContexte->getSuperviseur()->trace(&sErrorMsg, 1, NSSuper::trError) ;
  	erreur(sErrorMsg.c_str(), standardError, 0, pDocTtx->pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  LOGFONT lf ;
  memcpy(&lf, &_defaultFont, sizeof(_defaultFont)) ;

  string sText = string("") ;

  char buffer[1001] ;
  int	 num_ligne = 0 ;

  inFile.unsetf(ios::skipws) ;
  while (inFile.getline(buffer, 1000))
	{
  	OemToAnsi(buffer, buffer) ;
    num_ligne++ ;

    int iTailleBuf = strlen(buffer) ;

    for (int j = 0; j < iTailleBuf; j++)
    {
    	switch (buffer[j])
      {
      	case  9 : // Tabulation, non prise en compte
        	j += 2 ;
          break ;
        case 27 :
        {
        	j++ ;
          switch (buffer[j])
          {
            case '8' : // document title : increase font
            {
              if (string("") != sText)
              {
                SetFormatFont(lf) ;
                Insert(sText.c_str()) ;
                sText = string("") ;
              }
              memcpy(&lf, &_defaultFont, sizeof(_defaultFont)) ;
              lf.lfHeight -= 2 ;
              break ;
            }
            case '9' : // title : bold
            {
              if (string("") != sText)
              {
                SetFormatFont(lf) ;
                Insert(sText.c_str()) ;
                sText = string("") ;
              }
              memcpy(&lf, &_defaultFont, sizeof(_defaultFont)) ;
              lf.lfWeight = FW_BOLD ;
              break ;
            }
          }
          break ;        }        case 28 :
        {
        	j++ ;
          if (string("") != sText)
          {
            SetFormatFont(lf) ;
            Insert(sText.c_str()) ;
            sText = string("") ;
          }
          memcpy(&lf, &_defaultFont, sizeof(_defaultFont)) ;
          break ;
        }
        default :
        	if ((num_ligne < 4) || (num_ligne > 8))
          	sText += string(1, buffer[j]) ;
      }
    }

    SetFormatFont(lf) ;
    if (string("") == sText)
      sText += string(" ") ;
    sText += string("\n") ;
    Insert(sText.c_str()) ;
    sText = string("") ;
  }

  inFile.close() ;

  // SetText(sText.c_str()) ;

  pDocTtx->SetDirty(true) ;
}

// Fonction de réponse à la commande EditCopy
////////////////////////////////////////////////////////////////
void NSTtxView::CmEditCopy()
{
	TRichEdit::CmEditCopy() ;
}

// Fonction de réponse à la commande EditCut
////////////////////////////////////////////////////////////////
void NSTtxView::CmEditCut()
{
	TRichEdit::CmEditCut() ;
	pDocTtx->SetDirty(true) ;
}

// Fonction de réponse à la commande EditPaste
////////////////////////////////////////////////////////////////
void NSTtxView::CmEditPaste()
{
	TRichEdit::CmEditPaste() ;
	pDocTtx->SetDirty(true) ;
}

// Fonctions de réponse aux commandes File
////////////////////////////////////////////////////////////////

void NSTtxView::CmFileNew()
{
	TRichEdit::NewFile() ;
	pDocTtx->SetNomFichier("") ;
}

void NSTtxView::CmFileOpen(){
	// TRichEdit::Open();
	if (false == CanClear())
		return ;

	*FileData.FileName = 0 ;
	string sPath = pDocTtx->pContexte->PathName("NLTR") ;
	char path[1024] ;
	strcpy(path, sPath.c_str()) ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
																	"Fichiers texte (*.rtf)|*.rtf;|",
																	0,
																	path,
																	"*.rtf");

	int iUserChoice = TFileOpenDialog(this, initData).Execute() ;

  pDocTtx->pContexte->reallocateBaseDirectory() ;

	if (IDOK != iUserChoice)
		return ;

	pDocTtx->SetFichierExterne(initData.FileName) ;
	if (pDocTtx->HaveToReplaceTags())
		CmAutoFilling() ;
}stringNSTtxView::getText(){	int iTextLen = GetTextLen() ;  if (iTextLen <= 0)
		return string("") ;

	char far* szText = new char[iTextLen + 1] ;
	GetText(szText, iTextLen + 1) ;
	string sTexte = string(szText) ;
  delete[] szText ;  return sTexte ;}voidNSTtxView::RemplaceTagsRTF(){	// First, we take the text - not to make change in it, because changes are  // made directly into the control, but to know were to work  string sTexte = getText() ;  if (string("") == sTexte)		return ;
  map<string, string> tagsBuffer ;

	size_t pos1 = sTexte.find("[$") ;

	while (pos1 != string::npos)
	{
		size_t pos2 = sTexte.find("$]", pos1+2) ;

		if (pos2 != string::npos)
    {
    	string sTag    = string(sTexte, pos1+2, pos2-pos1-2) ;
      string sValeur = string("") ;

      if ((strlen(sTag.c_str()) > 8) && (string("docTitle") == string(sTag, 0, 8)))
      {
      	string sDocTitle = string(sTag, 8, strlen(sTag.c_str()) - 8) ;
        strip(sDocTitle) ;
        pDocTtx->SetDocTitle(sDocTitle) ;
      }
      else if (string("nomPatient") == sTag)
      	sValeur = pDocTtx->pContexte->getPatient()->getNomLong() ;
      else if (string("dateJour") == sTag)
      {
      	string sLang = pDocTtx->pContexte->getUserLanguage() ;

        char dateJ[9], message[11] ;
        donne_date_duJour(dateJ) ;
        donne_date(dateJ, message, sLang) ;
        sValeur = string(message) ;
      }
      else
      {
      	sValeur = sTag ;
        NSPatPathoArray* pPatPathoArray = NULL ;
        pDocTtx->pContexte->getPatient()->remplaceTagsChemin(sValeur, pPatPathoArray, &tagsBuffer, this->HWindow) ;
        if (pPatPathoArray)
					delete pPatPathoArray ;
      }

      // on remplace le texte directement dans le controle
      DeleteSubText(pos1, pos1 + strlen(sTag.c_str()) + 4) ;
      Insert(sValeur.c_str()) ;
      // on effectue la meme modif dans la string pour le calcul des positions
      sTexte.replace(pos1, strlen(sTag.c_str()) + 4, sValeur.c_str()) ;
      pos1 = sTexte.find("[$", pos1 + strlen(sValeur.c_str())) ;
    }
    else
    {
    	string sErrMsg = pDocTtx->pContexte->getSuperviseur()->getText("wordProcessorManagement", "noClosingTagFound") ;
			erreur(sErrMsg.c_str(), standardError, 0) ;
      // No need to continue since there is no ending sequence left
      return ;
    }
	}
}
voidNSTtxView::CmAutoFilling(){	if (!strcmp(pDocTtx->szFichierExterne, ""))	{
  	string sErrMsg = pDocTtx->pContexte->getSuperviseur()->getText("wordProcessorManagement", "fileToOpenNotSpecified") ;
		erreur(sErrMsg.c_str(), standardError, 0) ;
		return ;
	}

	TRichEdit::ReplaceWith(pDocTtx->szFichierExterne) ;

	// on met un nom de fichier vide pour forcer l'importation  // set a blank file name in order to force import  //	pDocTtx->SetNomFichier("") ;	RemplaceTagsRTF() ;	pDocTtx->SetDirty(true) ;}void NSTtxView::CmOuvrir(){
    string sFileName;

    if (!strcmp(pDocTtx->szFichierExterne, ""))
    {
        erreur("Le nom de fichier à ouvrir n'est pas précisé.", standardError, 0) ;
        return;
    }

    TRichEdit::ReplaceWith(pDocTtx->szFichierExterne);

    sFileName = (string)(TRichEdit::GetFileName());
    pDocTtx->SetNomFichier(sFileName);
}
void NSTtxView::CmFileSave()
{
	Enregistrer();
}

void NSTtxView::CmFileSaveAs(){
	EnregistrerSous();
}

void NSTtxView::CmFilePrint (){
  // Create Printer object
  //
  if (!pPrinter)
    pPrinter = new TPrinter(GetApplication());

  // Create Printout window and set characteristics.
  //
  const char* title = GetFileName() ? GetFileName() : "Untitled";
  TRichEditPrintout printout(*pPrinter, *this, title);
  printout.SetBanding(true);

  // Bring up the Print dialog and print the document.
  //
  pPrinter->Print(this, printout, true);
}

void NSTtxView::CmFilePrintPreview()
{
    // Create 'TPrinter' object to represent physical printer device
    //
    if (!pPrinter)
      pPrinter = new TPrinter(GetApplication());

    // Create 'TRichEditPrintout' object to represent RTF documents
    //
    TRichEditPrintout printout(*pPrinter,
                               *this,
                               "Print Preview");

    // Create a preview window
    //
    TRichEditPreviewFrame *prevW;
    prevW = new TRichEditPreviewFrame(this, *pPrinter, printout,
                                     *this, printout.GetTitle(),
                                      new TLayoutWindow(0));
    prevW->Execute();
}

// Fonctions de réponse aux commandes de format////////////////////////////////////////////////////////////////
void NSTtxView::CmFormatFont()
{
	// Retrouve format caractère courant
   TCharFormat chFormat(*this);

   // Fill logfont structure using info. From character format
   LOGFONT lf;
   chFormat.SetFontInfo(lf);
   lf.lfCharSet = DEFAULT_CHARSET;
   TClientDC dc(*this);

   // Initialize structure for Font common dialog
   TChooseFontDialog::TData fontData;
   lf.lfHeight = -MulDiv((int)(-lf.lfHeight), GetDeviceCaps(CreateCompatibleDC(dc), LOGPIXELSY), 72);
   fontData.LogFont = lf;

   TPrinter pr(pDocTtx->pContexte->getSuperviseur()->getApplication()) ;
   TPrintDC* prnDC = new TPrintDC((pr.GetData())->GetDriverName(), (pr.GetData())->GetDeviceName(),
                         (pr.GetData())->GetOutputName(), (pr.GetData())->GetDevMode());
   // prnDC->SetMapMode(MM_TWIPS);

   fontData.DC = *prnDC;   // fontData.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_EFFECTS;
   fontData.Flags = CF_PRINTERFONTS | CF_INITTOLOGFONTSTRUCT | CF_EFFECTS;
   fontData.Color = chFormat.GetTextColor();
   // fontData.FontType = SCREEN_FONTTYPE | PRINTER_FONTTYPE;
   fontData.FontType = PRINTER_FONTTYPE;

   // Display dialog
   if (TChooseFontDialog(this,fontData).Execute() == IDOK)
   {
   	// Update character format from chosen font info
      lf = fontData.LogFont;
      lf.lfHeight = -MulDiv((int)(-lf.lfHeight), 72, GetDeviceCaps(CreateCompatibleDC(dc), LOGPIXELSY));
      fontData.LogFont = lf;
      chFormat.GetFontInfo(fontData.LogFont);
      chFormat.SetTextColor(fontData.Color);
      SetCharFormat(chFormat);
      pDocTtx->SetDirty(true);
   }

   delete prnDC;
}

void NSTtxView::CmFormatPara()
{
}

void NSTtxView::CmFormatTabs()
{
}

void NSTtxView::CmFormatBold()
{
  ToggleCharAttribute(CFM_BOLD, CFE_BOLD);
  pDocTtx->SetDirty(true);
}

void NSTtxView::CmFormatItalic(){
  ToggleCharAttribute(CFM_ITALIC, CFE_ITALIC);
  pDocTtx->SetDirty(true);
}

void NSTtxView::CmFormatUnderline()
{
  ToggleCharAttribute(CFM_UNDERLINE, CFE_UNDERLINE);
  pDocTtx->SetDirty(true);
}

void NSTtxView::CmFormatStrikeout()
{
  ToggleCharAttribute(CFM_STRIKEOUT, CFE_STRIKEOUT);
  pDocTtx->SetDirty(true);
}

void NSTtxView::CmFormatTextData(){
  SetFormat(SF_TEXT);
  pDocTtx->SetDirty(true);
}

void NSTtxView::CmFormatRTFData()
{
  SetFormat(SF_RTF);
  pDocTtx->SetDirty(true);
}

////////////////////////////////////////////////////////////////void NSTtxView::CeFormatFont(TCommandEnabler& /*ce*/)
{
}

void NSTtxView::CeFormatPara(TCommandEnabler& /*ce*/)
{
}

void NSTtxView::CeFormatTabs(TCommandEnabler& /*ce*/){
}

void NSTtxView::CeFormatBold(TCommandEnabler& ce)
{
  uint result = HasCharAttribute(CFM_BOLD, CFE_BOLD);
  ce.SetCheck(result  ? Chk : UnChk);
}

void NSTtxView::CeFormatItalic(TCommandEnabler& ce)
{
  uint result = HasCharAttribute(CFM_ITALIC, CFE_ITALIC);
  ce.SetCheck(result ? Chk : UnChk);
}

void NSTtxView::CeFormatUnderline(TCommandEnabler& ce){
  uint result = HasCharAttribute(CFM_UNDERLINE, CFE_UNDERLINE);
  ce.SetCheck(result ? Chk : UnChk);
}

void NSTtxView::CeFormatStrikeout(TCommandEnabler& ce)
{
  uint result = HasCharAttribute(CFM_STRIKEOUT, CFE_STRIKEOUT);
  ce.SetCheck(result ? Chk : UnChk);
}

void
NSTtxView::CeFormatTextData(TCommandEnabler& ce)
{
  ce.SetCheck((IsRTF() == false) ? Chk : UnChk);
}

voidNSTtxView::CeFormatRTFData(TCommandEnabler& ce){
  ce.SetCheck((IsRTF()) ? Chk : UnChk);
}

void
NSTtxView::initMUEViewMenu(string sMenuName)
{
	if (pMUEViewMenu)
  	delete pMUEViewMenu ;

	nsMenuIniter menuIter(pDocTtx->pContexte) ;
	pMUEViewMenu = new OWL::TMenuDescr ;
  menuIter.initMenuDescr(pMUEViewMenu, sMenuName) ;
  hAccelerator = menuIter.getAccelerator() ;

	return ;
}

void
NSTtxView::activateMUEViewMenu()
{
	if (!pMUEViewMenu)
		return ;

	TMyApp* pMyApp = pDocTtx->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->GetMainWindow()->SetMenuDescr(*pMUEViewMenu) ;

	return ;
}

/******************************************************************************///					METHODES DE NSSimpleTxtDocument
/******************************************************************************/

// Constructeur NSSimpleTxtDocument////////////////////////////////////////////////////////////////

NSSimpleTxtDocument::NSSimpleTxtDocument(TDocument* parent, bool bROnly, NSDocumentInfo* pDocumentInfo,                             NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx)
              :NSRefDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, bROnly)
{
	// Mise à vide du nom de fichier
	SetNomFichier("") ;
	Open(0, "") ;
}

NSSimpleTxtDocument::NSSimpleTxtDocument(TDocument *parent, NSContexte *pCtx)              :NSRefDocument(parent, pCtx)
{
	// Mise à vide du nom de fichier
	SetNomFichier("") ;
	Open(0, "") ;
}

void NSSimpleTxtDocument::SetNomFichier(string sFich){
	strcpy(nomFichier, sFich.c_str());
}

string NSSimpleTxtDocument::GetDocExt(){
	size_t  i;
	string  sExt = "";

	for (i = 0; (i < strlen(nomFichier)) && (nomFichier[i] != '.'); i++) ;

    if (i < strlen(nomFichier))
    {
   	    // nomDoc[i] == '.'
   	    i++;
        while (i < strlen(nomFichier))
      	    sExt += nomFichier[i++];
    }

    return sExt;
}

// Ouverture du document////////////////////////////////////////////////////////////////

bool NSSimpleTxtDocument::Open(int /*mode*/, LPCSTR path){
	string sNomFichier = "";

	// Si il existe, prise du nom de fichier de la fiche document    ValideFichier(&sNomFichier);

	// Sinon, on sort	if (!bDocumentValide)
		return false;

	//	// Rangement du nom de fichier dans le document
	//

	SetNomFichier(sNomFichier);  	SetDirty(false);

  	return true;}

// Fermeture du document////////////////////////////////////////////////////////////////
bool NSSimpleTxtDocument::Close()
{
  // if (TDocument::Close())
  // 	 return false;

  SetNomFichier("");
  return true;}

// Génération du html correspondant au document////////////////////////////////////////////////////////////////
bool
NSSimpleTxtDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	NSHtmlTXT htxt(typeOperation, this, nomFichier, pContexte, sAdresseCorresp) ;
	string    sBaseImg ;
	// NSBaseImages* pBase ;

	// on trouve le nom du fichier temporaire à visualiser	sNomHtml = htxt.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	string sFichierHtml = sPathHtml + sNomHtml ;

	// on passe aussi le path destination pour les images	if (!htxt.genereHtml(sFichierHtml, sBaseImg, pHtmlInfo, sPathDest))
		return false ;

	// Mise à jour de la base d'images	switch (typeOperation)
	{
  	case toComposer:
    	sBaseCompo = sBaseImg ;
      break ;

		default:    	sBaseImages = sBaseImg ;
	}

	return true ;}

// Indique si document ouvert (il existe un fichier associé)////////////////////////////////////////////////////////////////
bool NSSimpleTxtDocument::IsOpen()
{
   if (_fstricmp(nomFichier,""))
      return true;
   else
      return false;
}

// Trouve un nom de fichier nautilus au document////////////////////////////////////////////////////////////////
bool NSSimpleTxtDocument::Renommer(string& sNomFichier, string& sLocalis, string sType, string* psNomBrut)
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
/******************************************************************************/

//					METHODES DE NSSimpleTxtView
/******************************************************************************/

// Table de réponses de la classe NSSimpleTxtView

////////////////////////////////////////////////////////////////
DEFINE_RESPONSE_TABLE2(NSSimpleTxtView, TView, TEditFile)  EV_VN_ISWINDOW,
  // EV_VN_COMMIT,
  // EV_VN_REVERT,
  EV_WM_CHAR,
  EV_WM_CLOSE,
  EV_WM_SETFOCUS,
  EV_WM_KILLFOCUS,
  EV_WM_RBUTTONDOWN,
  EV_WM_QUERYENDSESSION,

  EV_COMMAND(CM_COMPOSE,				CmComposer),  EV_COMMAND(CM_IMPRIME,				CmPublier),
  EV_COMMAND(CM_VISUAL,					CmVisualiser),
  EV_COMMAND(CM_ENREGISTRE,				CmFileSave),
  EV_COMMAND(CM_FORMULE,				CmFormules),
  EV_COMMAND(CM_LETTRETYPE,				CmFileOpen),  EV_COMMAND(CM_EDITCOPY,				CmEditCopy),
  EV_COMMAND(CM_EDITCUT,				CmEditCut),
  EV_COMMAND(CM_EDITPASTE,				CmEditPaste),
  EV_COMMAND(CM_FILENEW, 				CmFileNew),
  EV_COMMAND(CM_FILEOPEN, 				CmFileOpen),
  EV_COMMAND(CM_FILESAVE, 				CmFileSave),
  EV_COMMAND(CM_FILESAVEAS, 			CmFileSaveAs),

  EV_EN_DROPFILES(IDRichEd, EnDropFiles),  EV_EN_MSGFILTER(IDRichEd, EnMsgFilter),
  EV_EN_PROTECTED(IDRichEd, EnProtected),
  EV_EN_REQRESIZE(IDRichEd, EnReqResize),

END_RESPONSE_TABLE;
// Constructeur NSSimpleTxtView
////////////////////////////////////////////////////////////////

NSSimpleTxtView::NSSimpleTxtView(NSSimpleTxtDocument& doc, TWindow* parent)                :TView(doc), pDocTxt(&doc), TEditFile(parent,GetNextViewId(),"",0,0,0,0){
	// Attr.AccelTable = IDA_RICHEDIT;
	pMUEViewMenu = 0 ;

  TMyApp* pMyApp = pDocTxt->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->setMenu(string("menubar_txt"), &hAccelerator) ;

  pToolBar = 0 ;  bSetupToolBar = true ;
}

// Destructeur NSSimpleTxtView////////////////////////////////////////////////////////////////
NSSimpleTxtView::~NSSimpleTxtView()
{
	if (pMUEViewMenu)
  	delete pMUEViewMenu ;

  if (0 != hAccelerator)
  {
    DestroyAcceleratorTable(hAccelerator) ;
    hAccelerator = 0 ;
  }
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)
////////////////////////////////////////////////////////////////
TWindow* NSSimpleTxtView::GetWindow()
{
  return (TWindow*) this ;
}

// Appel de la fonction de remplissage de la vue////////////////////////////////////////////////////////////////
void NSSimpleTxtView::SetupWindow()
{
	LimitText(MAXCARS) ;
  TEditFile::SetupWindow() ;

  FillViewData() ;}

bool
NSSimpleTxtView::PreProcessMsg(MSG &msg)
{
  PRECONDITION(Handle) ;
  return hAccelerator ? ::TranslateAccelerator(Handle, hAccelerator, &msg) : false ;
}

voidNSSimpleTxtView::SetupTxtBar()
{
  TMyApp* pMyApp = pDocTxt->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->FlushControlBar() ;

  pMyApp->cb2->Insert(*new NSFlatButtonGadget(CM_EDITUNDO,   CM_EDITUNDO,   TButtonGadget::Command));  pMyApp->cb2->Insert(*new NSFlatButtonGadget(CM_EDITCUT,    CM_EDITCUT,    TButtonGadget::Command));
  pMyApp->cb2->Insert(*new NSFlatButtonGadget(CM_EDITCOPY,   CM_EDITCOPY,   TButtonGadget::Command));
  pMyApp->cb2->Insert(*new NSFlatButtonGadget(CM_EDITPASTE,  CM_EDITPASTE,  TButtonGadget::Command));
  pMyApp->cb2->Insert(*new NSFlatSeparatorGadget(6));
	pMyApp->cb2->Insert(*new NSFlatButtonGadget(CM_FORMULE,    CM_FORMULE,    TButtonGadget::Command));
  pMyApp->cb2->Insert(*new NSFlatButtonGadget(CM_LETTRETYPE, CM_LETTRETYPE, TButtonGadget::Command));
  pMyApp->cb2->LayoutSession();}

// Remplissage de la vue////////////////////////////////////////////////////////////////
void NSSimpleTxtView::FillViewData()
{
  if (pDocTxt->IsOpen())
  {
    TEditFile::ReplaceWith(pDocTxt->nomFichier);
    TEditFile::ClearModify();
    pDocTxt->SetDirty(false);
  }
}

// Fonction CanClose : Appel de CanClose du document////////////////////////////////////////////////////////////////
bool NSSimpleTxtView::CanClose()
{
	TMyApp* pMyApp = pDocTxt->pContexte->getSuperviseur()->getApplication() ;

	if (!CanClear())		return false ;

	pMyApp->FlushControlBar();
	bSetupToolBar = false ;	return true ;
}

bool NSSimpleTxtView::CanClear(){
	char far cfNomDocument[1024] = "" ;

	if (TEditFile::IsModified())	{
		char msg[1024] ;

		// si le fichier n'est pas référencé, on prend le nom de fichier (si il existe)		if (pDocTxt->pDocInfo == 0)
		{
			if (FileName)
				strcpy(cfNomDocument, FileName) ;
		}
		else // sinon on prend le nom de document
			strcpy(cfNomDocument, pDocTxt->GetTitle()) ;

		string sCaption = string("Message ") + pDocTxt->pContexte->getSuperviseur()->getAppName().c_str() ;
		sprintf(msg, "Le document %s a été modifié. Voulez vous le sauvegarder ?",
                FileName ? (const char far*)cfNomDocument : "texte créé") ;

		int result = MessageBox(msg, sCaption.c_str(), MB_YESNOCANCEL|MB_ICONQUESTION) ;
		return (result == IDYES ? Enregistrer() : result != IDCANCEL) ;	}
	return true ;}

bool NSSimpleTxtView::VnIsWindow(HWND hWnd){
  	return HWindow == hWnd;
}

void NSSimpleTxtView::EvRButtonDown(uint /*modkeys*/, NS_CLASSLIB::TPoint& point){
    OWL::TPopupMenu *menu = new OWL::TPopupMenu();

    menu->AppendMenu(MF_STRING, CM_EDITCUT,     "Couper");    menu->AppendMenu(MF_STRING, CM_EDITCOPY,    "Copier");
    menu->AppendMenu(MF_STRING, CM_EDITPASTE,   "Coller");
    menu->AppendMenu(MF_SEPARATOR, 0, 0);
    menu->AppendMenu(MF_STRING, CM_FORMULE,     "Insérer formule");

    ClientToScreen(point);
  	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow);
  	delete menu;}

void NSSimpleTxtView::EvChar(uint key, uint repeatCount, uint flags){
	TEditFile::EvChar(key,repeatCount,flags);
    pDocTxt->SetDirty(true);
}

// Fonctions EvSetFocus et EvKillFocus pour la control Bar////////////////////////////////////////////////////////////////
void
NSSimpleTxtView::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
{
	// activateMUEViewMenu() ;

	TMyApp* pMyApp = pDocTxt->pContexte->getSuperviseur()->getApplication() ;

	TEditFile::EvSetFocus(hWndLostFocus) ;
  pMyApp->setMenu(string("menubar_txt"), &hAccelerator) ;

	if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))	{
		SetupTxtBar() ;
		pMyApp->SetToolBarWindow(GetWindow()) ;
	}
}

voidNSSimpleTxtView::EvKillFocus(THandle hWndGetFocus /* may be 0 */)
{
	TEditFile::EvKillFocus(hWndGetFocus);
}

boolNSSimpleTxtView::EvQueryEndSession()
{
    return TWindow::EvQueryEndSession();
}

// Fonction Enregistrer////////////////////////////////////////////////////////////////
bool NSSimpleTxtView::Enregistrer()
{
	bool 		bRef = false;
   	bool		bSaveOk;
   	int  		retVal;
   	string   	sFichier = pDocTxt->nomFichier;
   	string   	sNomFichier;
   	string   	sLocalis;

   	// si le fichier n'est pas référencé   	if (pDocTxt->pDocInfo == 0)
   	{
   		// On doit le référencer
      	bRef = true;

   		// si ce n'est pas un nouveau fichier (nomFichier != NULL)   		if (sFichier != "")
      	{
   			retVal = MessageBox("Voulez-vous importer le document ?", "Message", MB_YESNOCANCEL);

         	if (retVal == IDCANCEL)         		return false;

   			if (retVal == IDYES) // cas de l'importation   				pDocTxt->Renommer(sNomFichier,sLocalis);
            else // cas du référencement
            {
            	sNomFichier = sFichier;
                sLocalis = "";
            }
      	}
      	else // Nouveau fichier : on le renomme
      		pDocTxt->Renommer(sNomFichier,sLocalis);
   	}

   	if (bRef)   	{
        if (pDocTxt->Referencer("ZTRTF","",sNomFichier,sLocalis))
        {
            TEditFile::SetFileName(pDocTxt->nomFichier);
   	        bSaveOk = TEditFile::Write();

   	        if (!bSaveOk)   	        {
   		        erreur("Erreur lors de la sauvegarde.",standardError,0,pDocTxt->pContexte->GetMainWindow()->GetHandle());
   		        pDocTxt->SetNomFichier(sFichier);
   		        return false;
   	        }

   	        pDocTxt->SetDirty(false);
            // on remet le titre à jour   	        TWindow::SetDocTitle(pDocTxt->GetTitle(),0);
        }
        else
        {
      	    pDocTxt->SetDirty(true);

      	    // on remet le document dans l'état initial      	    pDocTxt->SetNomFichier(sFichier);
        }
   	}
    else // cas pDocInfo != 0 : on sauvegarde le fichier
    {
        TEditFile::SetFileName(pDocTxt->nomFichier);
        bSaveOk = TEditFile::Write();

        if (!bSaveOk)        {
            erreur("Erreur lors de la sauvegarde.",standardError,0,pDocTxt->pContexte->GetMainWindow()->GetHandle());
            pDocTxt->SetNomFichier(sFichier);
            return false;
        }

        pDocTxt->SetDirty(false);        TWindow::SetDocTitle(pDocTxt->GetTitle(),0);
    }

   	return true;}

// Fonction EnregistrerSous : Importe automatiquement////////////////////////////////////////////////////////////////
bool NSSimpleTxtView::EnregistrerSous(){	string   sFichier = pDocTxt->nomFichier;
   	string   sNomFichier;
   	string   sLocalis;
   	bool	 bSaveOk;

   	pDocTxt->Renommer(sNomFichier,sLocalis);
    if (pDocTxt->Referencer("ZTTXT","",sNomFichier,sLocalis))
    {        TEditFile::SetFileName(pDocTxt->nomFichier);
   	    bSaveOk = TEditFile::Write();

   	    if (!bSaveOk)   	    {
   		    erreur("Erreur lors de la sauvegarde.",standardError,0,pDocTxt->pContexte->GetMainWindow()->GetHandle());
   		    pDocTxt->SetNomFichier(sFichier);
   		    return false;
   	    }

   	    pDocTxt->SetDirty(false);
        // on remet le titre à jour        TWindow::SetDocTitle(pDocTxt->GetTitle(),0);
    }
    else
   	{
      	pDocTxt->SetDirty(true);

      	// on remet le document dans l'état initial      	pDocTxt->SetNomFichier(sFichier);
   	}

   	return true;}

// Fonction de réponse à la commande Composer////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmComposer()
{
	pDocTxt->Composer();
}

// Fonction de réponse à la commande Publier////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmPublier()
{
	pDocTxt->Publier();
}

// Fonction de réponse à la commande Visualiser////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmVisualiser()
{
	pDocTxt->Visualiser();
}

// Fonction de réponse à la commande Formule////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmFormules()
{
	string sTexte = "" ;

	char far str[MAXCARS] ;
	LettreTypeDialog *pLTDlg = new LettreTypeDialog(this, pDocTxt->pContexte) ;
	if (pLTDlg->Execute() == IDOK)	{
		if (pLTDlg->choixIntro >= 0)
    	sTexte = pLTDlg->sIntro + string("\n\n") + sTexte ;

    if (pLTDlg->choixCorps >= 0)    	sTexte += pLTDlg->sCorps + string("\n\n") ;

    if (pLTDlg->choixPolit >= 0)    	sTexte += pLTDlg->sPolit + string("\n\n") ;

    GetText(str, MAXCARS) ;
    if (strcmp(str, ""))    	strcat(str, "\n\n") ;

    strcat(str, sTexte.c_str()) ;    SetText(str) ;
    pDocTxt->SetDirty(true) ;
	}

	delete pLTDlg ;}

// Fonction de réponse à la commande EditCopy////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmEditCopy()
{
	TEditFile::CmEditCopy();
}

// Fonction de réponse à la commande EditCut////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmEditCut()
{
	TEditFile::CmEditCut();
	pDocTxt->SetDirty(true);
}

// Fonction de réponse à la commande EditPaste////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmEditPaste()
{
	TEditFile::CmEditPaste();
	pDocTxt->SetDirty(true);
}

// Fonctions de réponse aux commandes File////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmFileNew()
{
   TEditFile::NewFile();
   pDocTxt->SetNomFichier("");
}

voidNSSimpleTxtView::CmFileOpen(){
	if (false == CanClear())
		return ;

	*FileData.FileName = 0 ;
	string sPath = pDocTxt->pContexte->PathName("NLTR") ;

	TOpenSaveDialog::TData data = GetFileData() ;
	if (data.InitialDir)		delete[] data.InitialDir ;

	data.InitialDir = new char[strlen(sPath.c_str()) + 1] ;	strcpy(data.InitialDir, sPath.c_str()) ;
	SetFileData((const TOpenSaveDialog::TData&) data) ;

	if (TFileOpenDialog(this, TEditFile::FileData).Execute() == IDOK)	{
  	TEditFile::ReplaceWith(FileData.FileName) ;
    string sFileName = (string)(TEditFile::GetFileName()) ;
    pDocTxt->SetNomFichier(sFileName) ;
	}
  pDocTxt->pContexte->reallocateBaseDirectory() ;
}

voidNSSimpleTxtView::CmFileSave(){
	Enregistrer();
}

voidNSSimpleTxtView::CmFileSaveAs(){
	EnregistrerSous();
}

void
NSSimpleTxtView::initMUEViewMenu(string sMenuName)
{
	if (pMUEViewMenu)
  	delete pMUEViewMenu ;

	nsMenuIniter menuIter(pDocTxt->pContexte) ;
	pMUEViewMenu = new OWL::TMenuDescr ;
  menuIter.initMenuDescr(pMUEViewMenu, sMenuName) ;

	return ;
}

void
NSSimpleTxtView::activateMUEViewMenu()
{
	if (!pMUEViewMenu)
		return ;

	TMyApp* pMyApp = pDocTxt->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->GetMainWindow()->SetMenuDescr(*pMUEViewMenu) ;

	return ;
}

// fin de nsttx.cpp
//////////////////////////////////////////////////


