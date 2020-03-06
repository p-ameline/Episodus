// NSTTX.CPP		Document/Vues Traitement de texte// ------------------------------------------------
// R�mi SPAAK - Mai 1997

#define __NS_TTX_CPP
#include "AutoWord.h"

#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>
#include <owl\window.rh>

#include <fstream.h>

// #include "nautilus\wordbasic.h"
#include "nautilus\wordObjects.h"

#include "nautilus\nssuper.h"#include "partage\nsdivfct.h"
#include "partage\ole_utils.h"
#include "nautilus\nsttx.h"
#include "nautilus\ns_html.h"
#include "nautilus\nscompub.h"		// pour les formules
#include "nautilus\nsresour.h"      // pour AutoWordDialog
#include "nautilus\nsvisual.h"
#include "nautilus\nshistdo.h"
#include "nsbb\nsattvaltools.h"
#include "nsbb\nsfltbtnga.h"
#include "nsoutil\nsFormules.h"
#include "partage\ns_timer.h"

#include "nautilus\nautilus.rh"
/******************************************************************************/
//
//					METHODES DE NSAutoWordView
//
/******************************************************************************/

DEFINE_RESPONSE_TABLE1(NSAutoWordView, TWindowView)	EV_WM_CLOSE,
  EV_WM_TIMER,
  EV_COMMAND(CM_FILECLOSE, EvClose),
  EV_MESSAGE(WM_OCEVENT, EvOcEvent),
  // EV_OC_CTRLEVENT_CUSTOMEVENT,

  // Evenements g�r�s par la classe VCL AutoWord
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
	_Form              = (TAutoWordForm*)      0 ;
  _pApplicationProxy = (T_ApplicationProxy*) 0 ;
  _pDocumentsProxy   = (TDocumentsProxy*)    0 ;
  _pCurrentDocument  = (T_DocumentProxy*)    0 ;
  _sWordVersion      = string("") ;

  _bSessionEnCours = false ;
}

NSAutoWordView::~NSAutoWordView(){
  if (_Form)
	  delete _Form ;
}
voidNSAutoWordView::PerformCreate(int /*menuOrId*/)
{
	// on cr�e la Form pour servir de zone client (on lui passe le handle parent)
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
  // La vue sera la fenetre client de la TMDIChild cr��e dans TMyApp::EvNewView  // Pour appliquer une taille � la vue, on doit donc agir sur la fenetre Parent

  Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER) ;  ModifyStyle(WS_BORDER, WS_CHILD) ;

  // ModifyExStyle(0, WS_EX_TOPMOST | WS_EX_TOOLWINDOW);  MakeVisible() ;
  _bSessionEnCours = false ;

  CmLancer() ;  // Starts timer that check if Word.Application is still valid  //  SetTimer(ID_WORD_TIMER, 1000) ;}

void
NSAutoWordView::EvTimer(uint timerId)
{
	if (ID_WORD_TIMER != timerId)
		return ;

  string ps = string("") ;

  if (_pApplicationProxy && _pApplicationProxy->IsBound())
	{
    try
    {
      long lDocumentsCount = _pDocumentsProxy->Count() ;
      return ;
    }
    catch (const TXOle& e)
    {
      ps = string("Word command timer: Application is deconnected, closing... ") + e.what() ;
    }
    catch (...)
    {
      ps = string("Word command timer: Application is deconnected, closing...") ;
    }
  }
  else
  {
    ps = string("Word command timer: Application is null or not bound, closing...") ;
  }

  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

  _bSessionEnCours = false ;

  if (_Form)
    _Form->Close() ;

  CloseWindow() ;
}

voidNSAutoWordView::EvClose()
{
  string ps = string("User closes a Word text") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	// fonction appel�e sur Fichier->Fermer

	CmQuitter() ;}

boolNSAutoWordView::CanClose()
{
try
{
  string ps = string("Word text: entering CanClose") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	// on place ici le code qui v�rifie si le document est bien sauvegard�  // car CanClose est appel�e directement lorsqu'on clique sur la case de fermeture
  // et la vue est ferm�e sans autre forme de proc�s...

	// l� encore, pour bien faire, il faudrait un hook	if (_bSessionEnCours)	{    // Is document dirty?    //    if (_pCurrentDocument && _pCurrentDocument->IsBound() && (false == _pCurrentDocument->Saved()))    {      string sDoISave = _pDocTtx->pContexte->getSuperviseur()->getText("msWordManagement", "yourWordDocumentWasModified.DoYouWantToSaveChanges?") ;    	string sCaption = string("Message ") + _pDocTtx->pContexte->getSuperviseur()->getAppName().c_str() ;

			int idRet = MessageBox(sDoISave.c_str(), sCaption.c_str(), MB_YESNOCANCEL) ;
      if      (IDYES == idRet)      	CmArchiver() ;
      else if (IDCANCEL == idRet)
      {
        string ps = string("User answers No to proposal to save the Word text, canceling Canclose") ;
        _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

      	return false ;
      }    }    if (_pCurrentDocument && _pCurrentDocument->IsBound())
    {
      TAutoVal SaveChanges(wdDoNotSaveChanges) ;
      _pCurrentDocument->Close(&SaveChanges) ;
    }
    if (_pDocumentsProxy && _pDocumentsProxy->IsBound())
    {
      long lDocumentsCount = _pDocumentsProxy->Count() ;
      if ((0 == lDocumentsCount) && _pApplicationProxy && _pApplicationProxy->IsBound())
		    _pApplicationProxy->Quit() ;
    }

		_bSessionEnCours = false ;	}

  KillTimer(ID_WORD_TIMER) ;

  if (_pCurrentDocument && _pCurrentDocument->IsBound())
	{
    ps = string("Word text: Unbinding document") ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

		_pCurrentDocument->Unbind() ;
    delete _pCurrentDocument ;
    _pCurrentDocument = (T_DocumentProxy*) 0 ;
	}

	if (_pDocumentsProxy && _pDocumentsProxy->IsBound())	{
    ps = string("Word text: Unbinding documents proxy") ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

		_pDocumentsProxy->Unbind() ;
    delete _pDocumentsProxy ;
    _pDocumentsProxy = (TDocumentsProxy*) 0 ;
	}

  if (_pApplicationProxy && _pApplicationProxy->IsBound())
	{
    ps = string("Word text: Unbinding application proxy") ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

		_pApplicationProxy->Unbind() ;
    delete _pApplicationProxy ;
    _pApplicationProxy = (T_ApplicationProxy*) 0 ;
	}

	// on rafraichit, en sortant, la visualisation du document si elle existe	// Note : on suppose ici, comme dans NSComposView::CmOk, qu'il n'existe
	// qu'une seule instance de vue de type Visual par document

	TView*        pView       = (TView*)        0 ;	NSVisualView* pVisualView = (NSVisualView*) 0 ;

	if (_pDocTtx->_pDocInfo)  // si le document est r�f�renc�	{
		pView = _pDocTtx->pContexte->getPatient()->getDocHis()->ActiveFenetre(_pDocTtx->_pDocInfo, "NSVisualView") ;

		if (pView)    {
    	pVisualView = dynamic_cast<NSVisualView*>(pView) ;
      pVisualView->Rafraichir() ;
    }
  }

	/* bool bOk = */ TWindowView::CanClose() ;
  ps = string("Word text: leaving CanClose") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	// ici, bizarrement, apr�s un FileSaveAs, TWindowView::CanClose renvoie false	// on force donc la valeur � true pour pouvoir sortir...
	return true ;
}catch (const TXOle& e){
  string sExept = string("Exception NSAutoWordView::CanClose ") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&sExept, 1, NSSuper::trError) ;

	string sConnectionLost = _pDocTtx->pContexte->getSuperviseur()->getText("msWordManagement", "automationLinkWithWordWasLost") ;
  string sMustCloseHere  = _pDocTtx->pContexte->getSuperviseur()->getText("msWordManagement", "youMustCloseWordFromOurMenu") ;
  sExept = sConnectionLost + string(" ") + sMustCloseHere ;

  erreur(sExept.c_str(), standardError, 0) ;
  return true ;
}catch (...)
{
  string sExept = string("Exception NSAutoWordView::CanClose") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&sExept, 1, NSSuper::trError) ;

  erreur(sExept.c_str(), standardError, 0) ;
  return true ;
}
}
boolNSAutoWordView::IsTabKeyMessage(MSG &msg)
{
  if (GetCapture() == NULL)
  {
    if ((WM_KEYDOWN == msg.message) || (WM_KEYUP == msg.message))
    {
      if (VK_TAB == msg.wParam)
      {
        SendMessage(CN_BASE + msg.message, msg.wParam, msg.lParam);
        return true;
      }
    }
  }

  return false ;}

//// Let the form process keystrokes in its own way.  Without
// this method, you can't tab between control on the form.
//
bool
NSAutoWordView::PreProcessMsg(MSG &msg)
{
  bool result = IsTabKeyMessage(msg) ;

  if (false == result)    result = TWindow::PreProcessMsg(msg) ;

  return result ;}

TResult
NSAutoWordView::EvOcEvent(TParam1 param1, TParam2 param2)
{
  return 0;
}

/*
bool
NSAutoWordView::EvOcCtrlCustomEvent(TCtrlCustomEvent* pev)
{
  switch (pev->Args->DispId)
	{
    case 6 :
	  {
		  WordDocument* arg0 = pev->Args->operator[](0);
		  __OWL TBool* arg1 = pev->Args->operator[](1);
		  // DocumentBeforeClose(arg0, arg1);
		  break;
	  }
  }
  return true ;
}
*/

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
  string ps = string("Word interface: Creating Application proxy") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  _pApplicationProxy = new T_ApplicationProxy() ;

  // Bind Word.Application
  //
try
{
  ps = string("Word interface: Bind Word.Application") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  _pApplicationProxy->Bind("Word.Application") ;
}
catch (const TXOle& e)
{
  ps = string("NSAutoWordView::CmLancer Error binding Word.Application ") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  ps = string("NSAutoWordView::CmLancer Error binding Word.Application") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}

  _pDocumentsProxy = new TDocumentsProxy() ;

  // Get word version
  //
try
{
  ps = string("Word interface: Get Word version") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  TString tsVersion = _pApplicationProxy->Version() ;
  _sWordVersion = string(tsVersion) ;
}
catch (const TXOle& e)
{
  ps = string("NSAutoWordView::CmLancer Error getting Word version ") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  ps = string("NSAutoWordView::CmLancer Error getting Word version") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}

  ps = string("Word interface: Word version is ") + _sWordVersion ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  // Get documents proxy
  //
try
{
  ps = string("Word interface: Get Documents proxy") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  _pApplicationProxy->_Documents(*_pDocumentsProxy) ;
}
catch (const TXOle& e)
{
  ps = string("NSAutoWordView::CmLancer Error getting Documents proxy ") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  ps = string("NSAutoWordView::CmLancer Error getting Documents proxy") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
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

  if ((_pApplicationProxy) && (_pApplicationProxy->IsBound()))
  {
    // on ne controle pas si Word est d�j� ouvert
    // car il faudrait rajouter un hook pour d�tecter la fermeture
    // on se contente d'intercepter les exceptions XOLE
    if (false == _bSessionEnCours)    {
      _pApplicationProxy->Visible(TRUE) ;
      _bSessionEnCours = true ;

      if (NULL == _pDocTtx->_pDocInfo)
      {
        // on regarde ici si on a un fichier externe
        // Si c'est le cas, on l'ouvre directement sans
        // passer par FileNew pour n'avoir qu'une fenetre Word
        if (string("") == _pDocTtx->GetExtFileName())
        {
          ps = string("Word text: FileNew") ;
          _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

          // Add a new document in documents' manager
          //
          _pCurrentDocument = _pDocumentsProxy->Add() ;
        }
        else
        {
          ps = string("Word text : FileOpen ") + _pDocTtx->GetExtFileName() ;
          _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

          wchar_t wcFileName[2048] ;
          swprintf(wcFileName, L"%S", _pDocTtx->GetExtFileName().c_str()) ;
          TAutoString asFileName(wcFileName) ;

          TAutoVal fileName(asFileName) ;

          if      (string("9.0")  == _sWordVersion)
            _pCurrentDocument = _pDocumentsProxy->Open2000(&fileName) ;
          else if (string("10.0") == _sWordVersion)
            _pCurrentDocument = _pDocumentsProxy->Open2002(&fileName) ;
          else
            _pCurrentDocument = _pDocumentsProxy->Open(&fileName) ;
        }
        // on met un titre provisoire...        _pDocTtx->SetTitle("Document Word (en cours)");
      }
      else
      {
        ps = string("Word text : FileOpen ") + _pDocTtx->GetFileName() ;
        _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

        wchar_t wcFileName[2048] ;
        swprintf(wcFileName, L"%S", _pDocTtx->GetFileName().c_str()) ;
        TAutoString asFileName(wcFileName);

        TAutoVal fileName(asFileName) ;
        _pCurrentDocument = _pDocumentsProxy->Open(&fileName) ;
      }

      // on affiche le titre du document      SetDocTitle(_pDocTtx->GetTitle(), 0) ;
    }
    else
      MessageBox("Impossible d'�diter car une session est en cours...") ;
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

  sExept = string("Nautilus a perdu le contr�le de Word dans la fonction d'�dition. ") +
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
  if (((T_DocumentProxy*) NULL == _pCurrentDocument) || (false == _pCurrentDocument->IsBound()))
    return ;

	string sPath = _pDocTtx->pContexte->PathName("NLTR") ;
	char path[1024] ;
	strcpy(path, sPath.c_str()) ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
																	"Fichiers texte (*.doc)|*.doc;*.docx;|",
																	0,
																	path,
																	"*.doc;*.docx");

	int iUserChoice = TFileOpenDialog(this, initData).Execute() ;

  _pDocTtx->pContexte->reallocateBaseDirectory() ;

	if (IDOK != iUserChoice)
		return ;

  string ps = string("Word text : FileOpen for model ") + string(initData.FileName) ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  // Get a Range for current document
  //
  TRangeProxy rangeProxy ;

try
{
  ps = string("NSAutoWordView::CmFileOpen: Get Range proxy") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  _pCurrentDocument->_Range(rangeProxy) ;
  if (false == rangeProxy.IsBound())
    return ;
}
catch (const TXOle& e)
{
  ps = string("NSAutoWordView::CmFileOpen: Error Getting Range proxy ") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  ps = string("NSAutoWordView::CmFileOpen: Error Getting Range proxy") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}

  // Insert file in Range
  //
  wchar_t wcFileName[1024] ;
  swprintf(wcFileName, L"%S", initData.FileName) ;
  TAutoString asFileName(wcFileName) ;

try
{
  ps = string("NSAutoWordView::CmFileOpen: Insert file \"") + initData.FileName + string("\" in Range") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  rangeProxy.InsertFile(asFileName) ;
}
catch (const TXOle& e)
{
  ps = string("NSAutoWordView::CmFileOpen: Error inserting file in Range ") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  ps = string("NSAutoWordView::CmFileOpen: Error inserting file in Range") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}

  ReplaceTags(&rangeProxy) ;

  // if (_pDocTtx->HaveToReplaceTags())
	//	CmAutoFilling() ;

}
catch (const TXOle& e)
{
  string sExept = "Exception NSAutoWordView::CmFileOpen " + string(e.what()) ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&sExept, 1, NSSuper::trError) ;

  sExept = string("Nautilus a perdu le contr�le de Word dans la fonction d'archivage. ") +
           string("Pour fermer Word, vous devez utiliser <Quitter> dans la fenetre <Document Word> de Nautilus.");
  erreur(sExept.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("Exception NSAutoWordView::CmFileOpen.", standardError, 0) ;
}
}
void
NSAutoWordView::ReplaceTags(TRangeProxy *pRangeProxy){try{  if ((NULL == pRangeProxy) || (false == pRangeProxy->IsBound()))    return ;  string ps = string("NSAutoWordView::ReplaceTags Setting Range object to whole document") ;  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;  pRangeProxy->WholeStory() ;/*  TRangeProxy cursorRangeProxy ;  // Getting a Range object at document's start  //  string ps = string("NSAutoWordView::ReplaceTags Getting a Range object at document's start") ;  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;  TAutoVal What(wdGoToBookmark) ;  TAutoVal Which(wdGoToFirst) ;  TAutoVal Count(1) ;  TAutoVal Name("\\StartOfDoc") ;  pRangeProxy->GoTo(cursorRangeProxy, &What, &Which, &Count, &Name) ;  if (false == cursorRangeProxy.IsBound())    return ;*/  // Getting a Find object  //  ps = string("NSAutoWordView::ReplaceTags Getting a Find object") ;  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;  TFindProxy findProxy ;  pRangeProxy->_Find(findProxy) ;  if (false == findProxy.IsBound())    return ;  // Execute find for first tag start  //  ps = string("NSAutoWordView::ReplaceTags Getting a Find object") ;  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;  wchar_t wcOpenTag[3] ;  swprintf(wcOpenTag, L"%S", "[$") ;  TAutoString asOpenTag(wcOpenTag) ;  wchar_t wcCloseTag[3] ;  swprintf(wcCloseTag, L"%S", "$]") ;  TAutoString asCloseTag(wcCloseTag) ;  // Tip from http://msdn.microsoft.com/en-us/library/aa192495%28v=office.11%29.aspx  //  // Find criteria are cumulative, which means that criteria are added to  // previous search criteria. You should get in the habit of clearing  // formatting from previous searches by using the ClearFormatting method  // prior to each search.  //  findProxy.ClearFormatting() ;  TAutoVal FindOpenTag(asOpenTag) ;  TBool bOpenTagFound = findProxy.Execute(&FindOpenTag) ;  map<string, string> tagsBuffer ;  map<string, bool>   condBuffer ;  while (bOpenTagFound)  {    // Opening tag was found, and Range was moved to the location of the match.    //    long lOpenTagStartPos = pRangeProxy->Start() ;    long lOpenTagEndPos   = pRangeProxy->End() ;    // Extend Range end to end of document    //    TAutoVal Unit(wdStory) ;    TAutoVal Count(1) ;    long lRealCount = pRangeProxy->MoveEnd(&Unit, &Count) ;    if (0 == lRealCount)      return ;    // Find closing tag    //    findProxy.ClearFormatting() ;    TAutoVal FindCloseTag(asCloseTag) ;    TBool bCloseTagFound = findProxy.Execute(&FindCloseTag) ;    if (false == bCloseTagFound)    {      string sErrMsg = _pDocTtx->pContexte->getSuperviseur()->getText("wordProcessorManagement", "noClosingTagFound") ;			erreur(sErrMsg.c_str(), standardError, 0) ;
      // No need to continue since there is no ending sequence left
      return ;    }    long lCloseTagStartPos = pRangeProxy->Start() ;    long lCloseTagEndPos   = pRangeProxy->End() ;    string sValue = string("") ;    // If there is at least one character inside tags    //    if (lCloseTagStartPos > lOpenTagEndPos + 1)    {      // Set Range between open tag and close tag      //      pRangeProxy->SetRange(lOpenTagEndPos, lCloseTagStartPos) ;      // Get text      //      TString asText = pRangeProxy->Text() ;      wchar_t* pwchText = (wchar_t*) asText ;      // string sText = "nomPatient" ;      string sText = BSTRtoSTLstring(pwchText) ;      delete[] pwchText ;      if ((strlen(sText.c_str()) > 8) && (string("docTitle") == string(sText, 0, 8)))      {
      	string sDocTitle = string(sText, 8, strlen(sText.c_str()) - 8) ;
        strip(sDocTitle) ;
        _pDocTtx->SetDocTitle(sDocTitle) ;
      }
      else if (string("nomPatient") == sText)
      	sValue = _pDocTtx->pContexte->getPatient()->getNomLong() ;
      else if (string("dateJour") == sText)
      {
      	string sLang = _pDocTtx->pContexte->getUserLanguage() ;
        sValue = donne_date(donne_date_duJour(), sLang) ;
      }
      else
      {
      	sValue = sText ;
        NSPatPathoArray* pPatPathoArray = NULL ;
        _pDocTtx->pContexte->getPatient()->remplaceTagsChemin(sValue, pPatPathoArray, &tagsBuffer, &condBuffer, this->HWindow) ;
        if (pPatPathoArray)
					delete pPatPathoArray ;

        if (sValue == sText)
          sValue = string("") ;
      }    }    // Set Range englobing the whole tag bloc    //    pRangeProxy->SetRange(lOpenTagStartPos, lCloseTagEndPos) ;/*    // Insert text after Range    //    // Tip: After this method is applied, the range expands to include the new text.    //    if (string("") != sValue)    {      wchar_t *wcValue = stringToWchart(sValue) ;      if (wcValue)      {        TAutoString asValue(wcValue) ;        pRangeProxy->InsertAfter(asValue) ;        delete[] wcValue ;      }    }    // Delete tag    //    TAutoVal CollapseDirection(wdCollapseStart) ;    pRangeProxy->Collapse(&CollapseDirection) ;    TAutoVal DeleteUnit(wdCharacter) ;    TAutoVal DeleteCount(lCloseTagEndPos - lOpenTagStartPos) ;    long lTrueDeletedCount = pRangeProxy->Delete(&DeleteUnit, &DeleteCount) ;*/    // Replace text in Range (tag) with text    //    wchar_t *wcValue = stringToWchart(sValue) ;    if (wcValue)    {      TAutoString asValue(wcValue) ;      pRangeProxy->Text(asValue) ;      delete[] wcValue ;    }    // Extend Range end to end of document    //    TAutoVal MoveEndUnit(wdStory) ;    TAutoVal MoveEndCount(1) ;    long lMoveEndRealCount = pRangeProxy->MoveEnd(&MoveEndUnit, &MoveEndCount) ;    if (0 == lMoveEndRealCount)      return ;    // Find a new starting tag to iterate    //    findProxy.ClearFormatting() ;    bOpenTagFound = findProxy.Execute(&FindOpenTag) ;  }/*  wchar_t wcOpenTag[3] ;  swprintf(wcOpenTag, L"%S", "[$") ;  wchar_t wcCloseTag[3] ;  swprintf(wcOpenTag, L"%S", "$]") ;  map<string, string> tagsBuffer ;

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
*/

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
catch (const TXOle& e)
{
  string sExcept = "Exception NSAutoWordView::ReplaceTags " + string(e.what()) ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&sExcept, 1, NSSuper::trError) ;
  erreur(sExcept.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("Exception NSAutoWordView::ReplaceTags.", standardError, 0) ;
}
}

voidNSAutoWordView::CmArchiver()
{
try{  string ps = string("User saves a Word text") ;  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;  string sNomFichier, sLocalis ;

  if (false == _bSessionEnCours)  {
    string sMessage = string("Vous devez avoir un fichier ouvert dans Word avant de pouvoir l'archiver dans ") + _pDocTtx->pContexte->getSuperviseur()->getAppName().c_str();
    MessageBox(sMessage.c_str());
    return;
  }

  // on enregistre le document au format HTML pour pouvoir le publier  if (NULL == _pDocTtx->_pDocInfo)
  {
    // Get "current document": either the one we opened in the first place
    //                         or the one that was openened after closing it
    //
    try
    {
      long lDocumentsCountBefore = _pDocumentsProxy->Count() ;
      if (0L == lDocumentsCountBefore)
        return ;

      TBool isDirty = _pCurrentDocument->Saved() ;
    }
    catch (const TXOle& e)
    {
      string oleExcpt = string("NSAutoWordView::CmArchiver: Initial document is deconnected ") + e.what() ;
      _pDocTtx->pContexte->getSuperviseur()->trace(&oleExcpt, 1, NSSuper::trError) ;

      try
      {
        _pCurrentDocument->Unbind() ;
        delete _pCurrentDocument ;

        long lDocumentsCount = _pDocumentsProxy->Count() ;
        if (0L == lDocumentsCount)
          return ;

        _pCurrentDocument = _pApplicationProxy->ActiveDocument() ;
      }
      catch (const TXOle& e)
      {
        string excpt = string("NSAutoWordView::CmArchiver: Error when saving a document ") + e.what() ;
        _pDocTtx->pContexte->getSuperviseur()->trace(&excpt, 1, NSSuper::trError) ;

        string sConnectionLost = _pDocTtx->pContexte->getSuperviseur()->getText("msWordManagement", "automationLinkWithWordWasLost") ;
        string sMustCloseHere  = _pDocTtx->pContexte->getSuperviseur()->getText("msWordManagement", "youMustCloseWordFromOurMenu") ;
        excpt = sConnectionLost + string(" ") + sMustCloseHere ;

        erreur(excpt.c_str(), standardError, 0) ;
      }
      catch (...)
      {
        string excpt = string("NSAutoWordView::CmArchiver: Error when saving a document") ;
        _pDocTtx->pContexte->getSuperviseur()->trace(&excpt, 1, NSSuper::trError) ;
        erreur(excpt.c_str(), standardError, 0) ;
        return ;
      }

      string sDoISave = _pDocTtx->pContexte->getSuperviseur()->getText("msWordManagement", "openedDocumentWasClosed.DoYouWantToSaveActiveOne?") ;
      string sCaption = string("Message ") + _pDocTtx->pContexte->getSuperviseur()->getAppName().c_str() ;
      int idRet = MessageBox(sDoISave.c_str(), sCaption.c_str(), MB_YESNOCANCEL) ;
      if      (IDYES != idRet)      {
        string ps = string("User answers No to proposal to save the Word text, canceling Canclose") ;
        _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;
      	return ;
      }
    }
    catch (...)
    {
      string excpt = string("NSAutoWordView::CmArchiver: Error when saving a document") ;
      _pDocTtx->pContexte->getSuperviseur()->trace(&excpt, 1, NSSuper::trError) ;
      erreur(excpt.c_str(), standardError, 0) ;
      return ;
    }

    _pDocTtx->Renommer(sNomFichier, sLocalis, "ZTHTM") ;

    // Note : Referencer remet � jour la donn�e Title du document    //    if (_pDocTtx->Referencer("ZTHTM", "", sNomFichier, sLocalis))
    {
      TAutoVal fileFormat(wdFormatHTML) ;

      wchar_t wcFileName[2048] ;
      swprintf(wcFileName, L"%S", _pDocTtx->GetFileName().c_str()) ;      TAutoString asFileName(wcFileName) ;
      TAutoVal fileName(asFileName) ;

      if (string("9.0") == _sWordVersion)
        _pCurrentDocument->SaveAs2000(&fileName, &fileFormat) ;
      else
        _pCurrentDocument->SaveAs(&fileName, &fileFormat) ;

      SetDocTitle(_pDocTtx->GetTitle(), 0) ;    }
  }
  else
  {
    // on sauvegarde uniquement car le fichier est ouvert
    // sous le bon format
    //
    _pCurrentDocument->Save() ;
  }

  ps = string("Leaving Word text save function") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
}
catch (const TXOle& e)
{
  string excpt = string("NSAutoWordView::CmArchiver: Error when saving a document ") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&excpt, 1, NSSuper::trError) ;
  erreur(excpt.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  string excpt = string("NSAutoWordView::CmArchiver: Error when saving a document") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&excpt, 1, NSSuper::trError) ;
  erreur(excpt.c_str(), standardError, 0) ;
  return ;
}
}
bool
NSAutoWordView::saveHTMLtoWORD(string sWordName)
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
    _pCurrentDocument->SaveAs(&fileName, &fileFormat) ;  return true ;
}
catch (const TXOle& e)
{
  string sExept = string("Nautilus a perdu le contr�le de Word dans la fonction d'archivage. ") +
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
// Fonction de r�ponse � la commande Composer////////////////////////////////////////////////////////////////
void
NSAutoWordView::CmComposer()
{
	// pDocTtx->Composer();
	MessageBox("Pour la composition Word, archivez le document et ouvrez le en mode visualisation.");
}

// Fonction de r�ponse � la commande Publier////////////////////////////////////////////////////////////////
void
NSAutoWordView::CmPublier()
{
  string ps = string("User publishes a Word text") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	_pDocTtx->Publier();
}

// Fonction de r�ponse � la commande Visualiser////////////////////////////////////////////////////////////////
void
NSAutoWordView::CmVisualiser()
{
  string ps = string("User display the HTML version of a Word text") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  TView* pView ;
  NSVisualView* pVisualView ;

  if (_pDocTtx->_pDocInfo)  // si le document est r�f�renc�  {
    // s'il existe une vue visual, on l'active et on la rafraichit
    pView = _pDocTtx->pContexte->getPatient()->getDocHis()->ActiveFenetre(_pDocTtx->_pDocInfo, "NSVisualView");

    if (pView)    {
      pVisualView = dynamic_cast<NSVisualView*>(pView);
      pVisualView->Rafraichir();
    }
    else // on lance une visualisation
      _pDocTtx->Visualiser();
  }
  else
    MessageBox("Le document doit �tre archiv� avant de pouvoir le visualiser");
}

// Fonction de r�ponse � la commande Formule////////////////////////////////////////////////////////////////
void
NSAutoWordView::CmFormules()
{
try{  string ps = string("User adds formulas to a Word text") ;  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;  string sTexte = string("") ;

  if (false == _bSessionEnCours)  {
    MessageBox("Word doit d�j� �tre ouvert pour pouvoir ins�rer des formules.") ;
    return ;
  }

  LettreTypeDialog *pLTDlg = new LettreTypeDialog(this, _pDocTtx->pContexte) ;
  if (pLTDlg->Execute() == IDOK)  {
    if (pLTDlg->existsIntro())
      sTexte += pLTDlg->getIntroText() + string("\n\n") ;

    if (pLTDlg->existsBody())      sTexte += pLTDlg->getBodyText() + string("\n\n") ;

    if (pLTDlg->existsPolite())      sTexte += pLTDlg->getPoliteText() + string("\n\n") ;
  }

  delete pLTDlg ;
/*
  swprintf(wcText, L"%S", sTexte.c_str()) ;  TAutoString asText(wcText) ;
  _pControl->Insert(asText) ;
*/

  // Get a Selection proxy for current document
  //
  TSelectionProxy selectionProxy ;

try
{
  ps = string("NSAutoWordView::CmFormules Get Selection proxy") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  _pApplicationProxy->_Selection(selectionProxy) ;
  if (false == selectionProxy.IsBound())
  {
    ps = string("NSAutoWordView::CmFormules Cannot get Word selection.") ;
    _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

    return ;
  }
}
catch (const TXOle& e)
{
  ps = string("NSAutoWordView::CmFormules Error getting Selection proxy ") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  ps = string("NSAutoWordView::CmFormules Error getting Selection proxy") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}
  wchar_t *wcText = new wchar_t[strlen(sTexte.c_str()) + 1] ;
  swprintf(wcText, L"%S", sTexte.c_str()) ;
  TAutoString asText(wcText) ;

  selectionProxy.TypeText(asText) ;
  // selectionProxy.TypeParagraph() ;

  delete[] wcText ;
}catch (const TXOle& e)
{
  string ps = string("NSAutoWordView::CmFormules Error ") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  string ps = string("NSAutoWordView::CmFormules Error") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
  return ;
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

/*
  swprintf(wcTopDocTag, L"%S", sBookmarkName.c_str()) ;
  _pControl->EditGoTo(TAutoVal(wcTopDocTag)) ;
*/

  delete[] wcTopDocTag ;
  return true ;
}
catch (const TXOle& e)
{
  string ps = string("Exception NSAutoWordView::GotoBookmark") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  return false ;
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
  // 33 Donne le type d'en-t�te ou de pied de page o� se trouve la s�lection.
  //     -1 la s�lection ne se trouve ni dans une en-t�te de page ni dans un pied de page
  //     0(z�ro) La s�lection se trouve dans l' en-t�te de page paire
  //     1 La s�lection se trouve dans l' en-t�te de page impaire
  //     2 La s�lection se trouve dans le pied de page paire
  //     3 La s�lection se trouve dans le pied de page impaire
  //     4 La s�lection se trouve dans le premi�re en-t�te de page
  //     5 La s�lection se trouve dans le premier pied de page
  // 34 Retourne -1 si le document actif est un document ma�tre (il est reconnu en tant que tel s'il contient au moins un sous document)
  // 35 Retourne -1 si la s�lection se trouve dans le panneau de pied de page ou dans le pied de page en mode page.
  // 36 Retourne -1 si la s�lection se trouve dans le panneau de notes de fin ou dans les notes de fin en mode page.

try
{
/*
  return _pControl->SelInfo(iType) ;
*/
  return 0 ;
}
catch (const TXOle& e)
{
  string ps = string("Exception NSAutoWordView::GetSelInfo") + e.what() ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  return 0 ;
}
catch (...)
{
  string ps = string("Exception NSAutoWordView::GetSelInfo") ;
  _pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  return 0 ;
}
}

/******************************************************************************/
//					METHODES DE NSTtx Document / Vue/******************************************************************************/
// Constructeur NSTtxDocument////////////////////////////////////////////////////////////////

NSTtxDocument::NSTtxDocument(TDocument* parent, bool bROnly, NSDocumentInfo* pDocumentInfo,                             NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx)
              :NSRefDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, bROnly)
{
	// Mise � vide du nom de fichier
  SetNomFichier("") ;
  SetFichierExterne("") ;

  if ((NSDocumentInfo*) NULL == pDocumentInfo)
		_sDocTitle = string("") ;
  else
  	_sDocTitle = pDocumentInfo->getDocName() ;

	Open(0, "") ;
  _bMustReplaceTagsWhenOpening = true ;
}

NSTtxDocument::NSTtxDocument(TDocument *parent, NSContexte *pCtx)              :NSRefDocument(parent, pCtx)
{
	// Mise � vide du nom de fichier
  SetNomFichier("") ;
  SetFichierExterne("") ;
  _sDocTitle = string("") ;

	Open(0, "") ;
  _bMustReplaceTagsWhenOpening = true ;
}

NSTtxDocument::~NSTtxDocument()
{
  string ps = string("NSTtxDocument destructor for ") + _sFileName ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
}

voidNSTtxDocument::SetNomFichier(string sFich){
  _sFileName = sFich ;
}

void
NSTtxDocument::SetFichierExterne(string sFich)
{
  _sExternalFileName = sFich ;
}

stringNSTtxDocument::GetDocExt(){
  if (string("") == _sFileName)
    return string("") ;

  // Find last dot
  //
  size_t iDotPos = _sFileName.find_last_of(".") ;

  if (NPOS == iDotPos)
    return string("") ;

  size_t iNameLen = strlen(_sFileName.c_str()) ;

  if (iDotPos >= iNameLen - 1)
    return string("") ;

  return string(_sFileName, iDotPos + 1, iNameLen - iDotPos - 1) ;
}

// Ouverture du document////////////////////////////////////////////////////////////////
bool
NSTtxDocument::Open(int /*mode*/, LPCSTR path){
	string sNomFichier = string("") ;

	// Si il existe, prise du nom de fichier de la fiche document  ValideFichier(&sNomFichier) ;

	// Sinon, on sort	if (false == _bDocumentValide)
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

// G�n�ration du html correspondant au document
////////////////////////////////////////////////////////////////
bool
NSTtxDocument::GenereHtml(string sPathHtml, string& sNomHtml,
                          OperationType typeOperation,
                          string sAdresseCorresp, string sPathDest)
{
	NSHtmlRTF hrtf(typeOperation, this, _sFileName, pContexte, sAdresseCorresp) ;
	string    sBaseImg = string("") ;
	// NSBaseImages* 	pBase;

	// on trouve le nom du fichier temporaire � visualiser
	sNomHtml = hrtf.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	string sFichierHtml = sPathHtml + sNomHtml ;

	if (_pDocInfo->getTypeSem() == string("ZTRTF"))
		hrtf.Convertir() ;   // conversion RTF ==> HTML (inutile pour Word)

	// on passe aussi le path destination pour les images
	if (!hrtf.genereHtml(sFichierHtml, sBaseImg, _pHtmlInfo, sPathDest))
		return false ;

	// Mise � jour de la base d'images
	switch (typeOperation)
	{
  	case toComposer:
    	_sBaseCompo = sBaseImg ;
      break ;

		default:
    	_sBaseImages = sBaseImg ;
	}

	return true ;
}

bool
NSTtxDocument::GenereHtmlText(string& sHtmlText)
{
  sHtmlText = string("") ;

  return true ;
}

bool
NSTtxDocument::GenereRawText(string& sRawText)
{
  if (false == IsOpen())
    return false ;

  // Fichier RTF
  //
  if (_pDocInfo->getTypeSem() == string("ZTRTF"))
  {
    ConvertRTFDialog rtfDialog(pContexte, pContexte->GetMainWindow(), _sFileName) ;
    rtfDialog.Create() ;

    // Get text
    //
    char far str[MAXCARS + 1] ;
    rtfDialog.getRichEdit()->GetSubText(str, 0, MAXCARS) ;

    rtfDialog.CmOk() ;

    sRawText = getRawTextHeader() ;

    if (string("") != sRawText)
      sRawText += string(NEWLINE) + string(NEWLINE) ;

    sRawText += string(str) ;

    return true ;
  }

  // Fichier HTML
  //
  if (_pDocInfo->getTypeSem() == string("ZTHTM"))
  {
    sRawText = getRawTextHeader() ;

    if (string("") != sRawText)
      sRawText += string(NEWLINE) + string(NEWLINE) ;

    ifstream inFile ;
    inFile.open(_sFileName.c_str()) ;
    if (!inFile)
      return false ;

    while (!inFile.eof())
    {
      string sLine ;
      getline(inFile, sLine) ;
      if (string("") != sLine)
        sRawText += sLine + string(NEWLINE) ;
    }
    inFile.close() ;

    int iTagLevel = 0 ;

    size_t cc   = 0 ;
    size_t iDeb = 0 ;

    while (cc < strlen(sRawText.c_str()))
    {
      // lecture jusqu'au prochain marqueur ou fin de fichier
      //
      size_t iRawTextLen = strlen(sRawText.c_str()) ;
      while ((cc < iRawTextLen) && ('<' != sRawText[cc]) && ('>' != sRawText[cc]))
        cc++ ;

      // Opening tag
      //
      if ('<' == sRawText[cc])
      {
        if (0 == iTagLevel)
          iDeb = cc ;
        cc++ ;

        iTagLevel++ ;
      }
      // Closing tag
      //
      else if ('>' == sRawText[cc])
      {
        iTagLevel-- ;

        if (0 == iTagLevel)
        {
          if (cc < strlen(sRawText.c_str()) - 1)
          {
            // Tag ending a line, check if the full line should be removed
            //
            // Get tag
            //
            string sTag = "" ;
            int i = iDeb + 1 ;
            for ( ; (' ' != sRawText[i]) && ('>' != sRawText[i]) && ('/' != sRawText[i]) ; i++) ;
            sTag = pseumaj(string(sRawText, iDeb + 1, i - iDeb -1)) ;

            // Skip the whole <HEAD> block
            //
            if (string("HEAD") == sTag)
            {
              size_t iPos = sRawText.find("</head>", cc) ;
              if (NPOS != iPos)
                cc = iPos + 6 ;
            }
            // Skip <!--[IF ><![endif]--> blocks
            //
            else if (string("!--[IF") == sTag)
            {
              size_t iPos = sRawText.find("<![endif]-->", cc) ;
              if (NPOS != iPos)
                cc = iPos + 11 ;
            }
            // Skip the whole <XML> block in order not to get the variables it contains
            //
            else if (string("XML") == sTag)
            {
              size_t iPos = sRawText.find("</xml>", cc) ;
              if (NPOS != iPos)
                cc = iPos + 5 ;
            }

            string sTagReplacer = string("") ;
            if ((string("BR") == sTag) || (string("P") == sTag))
              sTagReplacer = string(NEWLINE) ;

            if ((cc < strlen(sRawText.c_str()) - 1 - strlen(NEWLINE)) &&
                (string(NEWLINE) == string(sRawText, cc + 1, strlen(NEWLINE))))
            {
              cc += strlen(NEWLINE) ;

              if (string("") == sTagReplacer)
                sTagReplacer = string(" ") ;
            }

            sRawText = string(sRawText, 0, iDeb) + sTagReplacer + string(sRawText, cc + 1, strlen(sRawText.c_str()) - cc - 1) ;
          }
          else
            sRawText = string(sRawText, 0, iDeb) ;

          cc = iDeb ;
        }
      }
    }

    return true ;
  }

  return true ;
}

// Indique si document ouvert (il existe un fichier associ�)////////////////////////////////////////////////////////////////
bool
NSTtxDocument::IsOpen()
{
	if (string("") != _sFileName)
		return true ;
	else
		return false ;
}

// Trouve un nom de fichier nautilus au document
////////////////////////////////////////////////////////////////
bool
NSTtxDocument::Renommer(string& sNomFichier, string& sLocalis, string sType, string* psNomBrut)
{
	string sExt = GetDocExt() ;

	if (false == TrouveNomFichier(sType, sExt, sNomFichier, sLocalis))
	{
  	erreur("Pb � l'attribution d'un nouveau nom pour ce fichier", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

	if (psNomBrut)
	{
  	// Dans ce cas on renvoie aussi le nom brut (sans extension)
    size_t pos = sNomFichier.find_last_of('.') ;
    if (NPOS != pos)
    	*psNomBrut = string(sNomFichier, 0, pos) ;
    else
    	*psNomBrut = sNomFichier ;
	}

	string sPath = pContexte->PathName(sLocalis) ;

	// on construit le nom complet pour la sauvegarde
	SetNomFichier(sPath + sNomFichier) ;

	SetDirty(true) ;

	return true ;
}

// Table de r�ponses de la classe NSTtxView////////////////////////////////////////////////////////////////

DEFINE_RESPONSE_TABLE2(NSTtxView, TView, NSRichEdit)  EV_VN_ISWINDOW,
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

NSTtxView::NSTtxView(NSTtxDocument& doc, TWindow* parent)          :TView(doc), pDocTtx(&doc),           NSRichEdit(doc.pContexte, parent, GetNextViewId(), "", 0, 0, 0, 0){
	Attr.AccelTable = IDA_RICHEDIT ;

  pMUEViewMenu = (TMenuDescr*) 0 ;
  hAccelerator = 0 ;

  TMyApp* pMyApp = pDocTtx->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->setMenu(string("menubar_txt"), &hAccelerator) ;

	pToolBar = (OWL::TControlBar*) 0 ;  pPrinter = (OWL::TPrinter*)    0 ;
  bSetupToolBar = true ;

  memset(&_defaultFont, 0, sizeof(LOGFONT)) ;
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

// GetWindow renvoie this (� red�finir car virtual dans TView)
////////////////////////////////////////////////////////////////

TWindow* NSTtxView::GetWindow(){
  return (TWindow*) this ;
}

// Appel de la fonction de remplissage de la vue////////////////////////////////////////////////////////////////

void NSTtxView::SetupWindow(){
	LimitText(MAXCARS) ;
  NSRichEdit::SetupWindow() ;

  // Change font to ANSI Variable
  HFONT hFont = HFONT(::GetStockObject(ANSI_VAR_FONT)) ;
  if (hFont)
  	SetWindowFont(hFont,true) ;

    /************************************* S�lection des icones
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

	FillViewData() ;  if ((string("") != pDocTtx->GetExtFileName()) && (pDocTtx->HaveToReplaceTags()))  	CmAutoFilling() ;}

voidNSTtxView::SetupTxtBar()
{
	TMyApp* pMyApp = pDocTtx->pContexte->getSuperviseur()->getApplication() ;

	pMyApp->FlushControlBar() ;

	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_EDITUNDO,   CM_EDITUNDO,   TButtonGadget::Command)) ;	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_EDITCUT,    CM_EDITCUT,    TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_EDITCOPY,   CM_EDITCOPY,   TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_EDITPASTE,  CM_EDITPASTE,  TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatSeparatorGadget(6)) ;
	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_FORMULE,    CM_FORMULE,    TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_LETTRETYPE, CM_LETTRETYPE, TButtonGadget::Command)) ;
  string sFichDat = pDocTtx->pContexte->PathName("IDOS") ;
  if (string("") != sFichDat)
    pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(IDM_NAUTIDOS, IDM_NAUTIDOS, TButtonGadget::Command)) ;

  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatSeparatorGadget(6)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(CM_FORMATBOLD,      CM_FORMATBOLD,      TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(CM_FORMATITALIC,    CM_FORMATITALIC,    TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(CM_FORMATUNDERLINE, CM_FORMATUNDERLINE, TButtonGadget::Command)) ;

  pMyApp->getSecondaryControlBar()->LayoutSession() ;}

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
  	NSRichEdit::ReplaceWith(pDocTtx->GetFileName().c_str()) ;
    NSRichEdit::ClearModify() ;
    pDocTtx->SetDirty(false) ;
  }
  else if (string("") != pDocTtx->GetExtFileName())  {  	CmOuvrir() ;  }  else
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
  _defaultFont.lfHeight         = -12 ; // setup font characteristics
  _defaultFont.lfWidth          = 0 ;
  _defaultFont.lfEscapement     = 0 ;
  _defaultFont.lfOrientation    = 0 ;
  _defaultFont.lfWeight         = FW_NORMAL ;
  _defaultFont.lfItalic         = FALSE ;
  _defaultFont.lfUnderline      = FALSE ;
  _defaultFont.lfStrikeOut      = FALSE ;
  _defaultFont.lfCharSet        = ANSI_CHARSET ;
  _defaultFont.lfOutPrecision   = OUT_DEFAULT_PRECIS ;
  _defaultFont.lfClipPrecision  = CLIP_DEFAULT_PRECIS ;
  _defaultFont.lfQuality        = PROOF_QUALITY ;
  _defaultFont.lfPitchAndFamily = DEFAULT_PITCH || FF_ROMAN ;

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

  string sData = string("") ;

  while (!inFile.eof())
  {
    string sLine ;
    getline(inFile, sLine) ;
    if (string("") != sLine)
      sData += sLine + string("\n") ;
  }

  inFile.close() ;

  size_t i = 0 ;
  int    j = 0 ;

  string sNomAttrib[3] = {"", "", ""} ;
  string sValAttrib[3] = {"", "", ""} ;

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

  // on positionne les param�tres de la police (LOGFONT)
  _defaultFont.lfHeight = -1 * iTaille ;

  if (strlen((sValAttrib[0]).c_str()) > (LF_FACESIZE - 1))
    erreur("La police par d�faut ne peut etre fix�e (31 caract�res max.)",standardError,0,pDocTtx->pContexte->GetMainWindow()->GetHandle()) ;
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
  // Retrouve format caract�re courant
  NSCharFormat2 chFormat(*this) ;
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

	if (false == CanClear())
		return false ;

	pMyApp->FlushControlBar() ;
	bSetupToolBar = false ;	return true ;
}

bool NSTtxView::CanClear()
{
	if (false == NSRichEdit::IsModified())
		return true ;

  char far cfNomDocument[1024] = "" ;

	// si le fichier n'est pas r�f�renc�, on prend le nom de fichier (si il existe)
	if (0 == pDocTtx->_pDocInfo)
	{
		if (FileName)
    	strcpy(cfNomDocument, FileName) ;
	}
	else // sinon on prend le nom de document
		strcpy(cfNomDocument, pDocTtx->GetTitle()) ;

	string sCaption = string("Message ") + pDocTtx->pContexte->getSuperviseur()->getAppName().c_str();  string sMsgTxt = pDocTtx->pContexte->getSuperviseur()->getText("wordProcessorManagement", "doYouWantToSaveChangesToDocument%s") ;  char msg[1024] ;	sprintf(msg, sMsgTxt.c_str(),             FileName ? (const char far*)cfNomDocument : "texte cr��");

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

  if (NSRichEdit::HasSelection())
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
  menu->AppendMenu(MF_STRING, CM_FORMULE,     "Ins�rer formule");

  ClientToScreen(point) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;
  delete menu ;
}

voidNSTtxView::EvChar(uint key, uint repeatCount, uint flags){
	NSRichEdit::EvChar(key,repeatCount,flags) ;
  pDocTtx->SetDirty(true) ;
}

// Fonctions EvSetFocus et EvKillFocus pour la control Bar
////////////////////////////////////////////////////////////////
void
NSTtxView::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
{
	// activateMUEViewMenu() ;

	TMyApp* pMyApp = pDocTtx->pContexte->getSuperviseur()->getApplication() ;

	NSRichEdit::EvSetFocus(hWndLostFocus) ;

  pMyApp->setMenu(string("menubar_txt"), &hAccelerator) ;

  if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
  {
  	SetupTxtBar() ;
    pMyApp->SetToolBarWindow(GetWindow()) ;
  }
}

voidNSTtxView::EvKillFocus(THandle hWndGetFocus /* may be 0 */)
{
	NSRichEdit::EvKillFocus(hWndGetFocus);
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
bool
NSTtxView::Enregistrer()
{
	bool 	 bRef        = false ;
	string sFichier    = pDocTtx->GetFileName() ;
	string sNomFichier = string("") ;
	string sLocalis    = string("") ;

	// si le fichier n'est pas r�f�renc�
	if (0 == pDocTtx->_pDocInfo)
	{
  	// On doit le r�f�rencer
    bRef = true ;

    // si ce n'est pas un nouveau fichier (nomFichier != NULL)
    if (string("") != sFichier)
    {
    	int retVal = MessageBox("Voulez-vous importer le document ?", "Message", MB_YESNOCANCEL);

      if (IDCANCEL == retVal)
      	return false ;

      if (IDYES == retVal) // cas de l'importation      	pDocTtx->Renommer(sNomFichier, sLocalis) ;
      else // cas du r�f�rencement
      {
      	sNomFichier = sFichier ;
        sLocalis    = string("") ;
      }
    }
    else // Nouveau fichier : on le renomme
    	pDocTtx->Renommer(sNomFichier, sLocalis) ;
	}

	if (bRef)
	{
  	if (pDocTtx->Referencer("ZTRTF", pDocTtx->GetDocTitle(), sNomFichier, sLocalis))
		{
    	NSRichEdit::SetFileName(pDocTtx->GetFileName().c_str()) ;
      bool bSaveOk = NSRichEdit::Write() ;

      if (false == bSaveOk)
      {
      	erreur("Erreur lors de la sauvegarde.", standardError, 0, pDocTtx->pContexte->GetMainWindow()->GetHandle()) ;
        pDocTtx->SetNomFichier(sFichier) ;
        return false ;
      }

      string ps = string("Text written on disk as ") + pDocTtx->GetFileName() ;
      pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

      pDocTtx->SetDirty(false) ;
      // on remet le titre � jour
      SetDocTitle(pDocTtx->GetTitle(), 0) ;
    }
    else
    {
    	pDocTtx->SetDirty(true) ;
      // on remet le document dans l'�tat initial
      pDocTtx->SetNomFichier(sFichier) ;
    }
  }
  else // cas pDocInfo != 0 : on sauvegarde le fichier
  {
  	NSRichEdit::SetFileName(pDocTtx->GetFileName().c_str()) ;
    bool bSaveOk = NSRichEdit::Write() ;

    if (false == bSaveOk)
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
	string sFichier = pDocTtx->GetFileName() ;
	string sNomFichier ;
	string sLocalis ;
	bool	 bSaveOk ;

	pDocTtx->Renommer(sNomFichier, sLocalis) ;

	if (pDocTtx->Referencer("ZTRTF", pDocTtx->GetDocTitle(), sNomFichier, sLocalis))
	{
  	NSRichEdit::SetFileName(pDocTtx->GetFileName().c_str()) ;
    bSaveOk = NSRichEdit::Write() ;

    if (false == bSaveOk)
    {
    	erreur("Erreur lors de la sauvegarde.", standardError, 0, pDocTtx->pContexte->GetMainWindow()->GetHandle()) ;
      pDocTtx->SetNomFichier(sFichier) ;
      return false ;
    }

    pDocTtx->SetDirty(false) ;
    // on remet le titre � jour    SetDocTitle(pDocTtx->GetTitle(), 0) ;
	}
	else
	{
  	pDocTtx->SetDirty(true) ;
    // on remet le document dans l'�tat initial
    pDocTtx->SetNomFichier(sFichier) ;
	}

	return true ;
}

// Fonction de r�ponse � la commande Composer
////////////////////////////////////////////////////////////////
void NSTtxView::CmComposer()
{
	pDocTtx->Composer() ;
}

// Fonction de r�ponse � la commande Publier////////////////////////////////////////////////////////////////
void NSTtxView::CmPublier()
{
	pDocTtx->Publier() ;
}

// Fonction de r�ponse � la commande Visualiser
////////////////////////////////////////////////////////////////
void NSTtxView::CmVisualiser()
{
	pDocTtx->Visualiser() ;
}

// Fonction de r�ponse � la commande Formule
////////////////////////////////////////////////////////////////
void NSTtxView::CmFormules()
{
try
{
	LettreTypeDialog *pLTDlg = new LettreTypeDialog(this, pDocTtx->pContexte) ;

	if (pLTDlg->Execute() == IDOK)
	{
  	string sTexte = getText() ;

  	if (pLTDlg->existsIntro())
    	sTexte = pLTDlg->getIntroText() + string("\n\n") + sTexte ;

    if (pLTDlg->existsBody())
    	sTexte += pLTDlg->getBodyText() + string("\n\n") ;

    if (pLTDlg->existsPolite())
    	sTexte += pLTDlg->getPoliteText() + string("\n\n") ;

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

  bool bTitleUpdated = false ;

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
            // Replace "ECHOGRAPHIE" by "�CHOGRAPHIE ABDOMINALE TRANSCUTAN�E"
            //
            if (false == bTitleUpdated)
            {
              size_t posit = sText.find("ECHOGRAPHIE") ;
              if (NPOS != posit)
              {
                sText.replace(posit, 11, "�CHOGRAPHIE ABDOMINALE TRANSCUTAN�E") ;
                bTitleUpdated = true ;
              }
            }

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

// Fonction de r�ponse � la commande EditCopy
////////////////////////////////////////////////////////////////
void NSTtxView::CmEditCopy()
{
	NSRichEdit::CmEditCopy() ;
}

// Fonction de r�ponse � la commande EditCut
////////////////////////////////////////////////////////////////
void NSTtxView::CmEditCut()
{
	NSRichEdit::CmEditCut() ;
	pDocTtx->SetDirty(true) ;
}

// Fonction de r�ponse � la commande EditPaste
////////////////////////////////////////////////////////////////
void NSTtxView::CmEditPaste()
{
	NSRichEdit::CmEditPaste() ;
	pDocTtx->SetDirty(true) ;
}

// Fonctions de r�ponse aux commandes File
////////////////////////////////////////////////////////////////

void NSTtxView::CmFileNew()
{
	NSRichEdit::NewFile() ;
	pDocTtx->SetNomFichier("") ;
}

void NSTtxView::CmFileOpen(){
	// NSRichEdit::Open();
	if (false == CanClear())
		return ;

	// *FileData.FileName = 0 ;
  strcpy(FileData.FileName, "") ;
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

	char *szText = new char[iTextLen + 1] ;
	GetText(szText, iTextLen + 1) ;
	string sTexte = string(szText) ;
  delete[] szText ;  return sTexte ;}voidNSTtxView::RemplaceTagsRTF(){	// First, we take the text - not to make change in it, because changes are  // made directly into the control, but to know where to work  string sTexte = getText() ;  if (string("") == sTexte)		return ;
  map<string, string> tagsBuffer ;
  map<string, bool>   condBuffer ;

	size_t pos1 = sTexte.find("[$") ;

	while (string::npos != pos1)
	{
		size_t pos2 = sTexte.find("$]", pos1+2) ;

		if (string::npos != pos2)
    {
    	string sTag    = string(sTexte, pos1+2, pos2-pos1-2) ;
      string sValeur = string("") ;

      // Specific tags (docTitle, nomPatient, dateJour)
      //
      if ((strlen(sTag.c_str()) > 8) && (string("docTitle") == string(sTag, 0, 8)))
      {
      	string sDocTitle = string(sTag, 8, strlen(sTag.c_str()) - 8) ;
        strip(sDocTitle) ;
        pDocTtx->SetDocTitle(sDocTitle) ;
      }
      else if (string("nomPatient") == sTag)
      	sValeur = pDocTtx->pContexte->getPatient()->getNomLong() ;
      else if (string("signature") == sTag)
      {
        sValeur = pDocTtx->pContexte->getUtilisateur()->getCivilProf() ;
        if (string("") == sValeur)
        {
          sValeur = pDocTtx->pContexte->getUtilisateur()->donneSignature(pDocTtx->pContexte, pDocTtx->pContexte->getUtilisateur()->donneLang()) ;
          pDocTtx->pContexte->getUtilisateur()->setCivilProf(sValeur) ;
        }
      }
      else if (string("dateJour") == sTag)
      {
      	string sLang = pDocTtx->pContexte->getUserLanguage() ;
        sValeur = donne_date(donne_date_duJour(), sLang) ;
      }
      // Generic tag
      //
      else
      {
      	sValeur = sTag ;
        NSPatPathoArray* pPatPathoArray = (NSPatPathoArray*) 0 ;
        pDocTtx->pContexte->getPatient()->remplaceTagsChemin(sValeur, pPatPathoArray, &tagsBuffer, &condBuffer, this->HWindow) ;
        if (pPatPathoArray)
					delete pPatPathoArray ;
      }

      int iLenToBeDeleted = strlen(sTag.c_str()) + 4 ;

      // Specifically manage empty results to avoid letting an empty line
      //
      if (string("") == sValeur)
      {
        // Detect if removing tag would create a "\r\n\r\n" pattern
        //
        if ((pos1 > 1) && (pos1 + iLenToBeDeleted + 2 < strlen(sTexte.c_str())))
        {
          if ((string("\r\n") == string(sTexte, pos1 - 2, 2)) &&
              (string("\r\n") == string(sTexte, pos1 + iLenToBeDeleted, 2)))
            iLenToBeDeleted += 2 ;
        }
      }

      // on remplace le texte directement dans le controle
      //
      GetApplication()->PumpWaitingMessages() ;

      // DeleteSubText(pos1, pos1 + iLenToBeDeleted) ;
      // Insert(sValeur.c_str()) ;
      ReplaceText(pos1, iLenToBeDeleted, sValeur) ;

      // on effectue la meme modif dans la string pour le calcul des positions
      //
      sTexte.replace(pos1, iLenToBeDeleted, sValeur.c_str()) ;
      pos1 = sTexte.find("[$", pos1 + strlen(sValeur.c_str())) ;
    }
    else
    {
    	string sErrMsg = pDocTtx->pContexte->getSuperviseur()->getText("wordProcessorManagement", "noClosingTagFound") ;
			erreur(sErrMsg.c_str(), standardError, 0) ;

      // No need to continue since there is no ending sequence left
      //
      return ;
    }
	}
}
voidNSTtxView::CmAutoFilling(){	if (string("") == pDocTtx->GetExtFileName())	{
  	string sErrMsg = pDocTtx->pContexte->getSuperviseur()->getText("wordProcessorManagement", "fileToOpenNotSpecified") ;
		erreur(sErrMsg.c_str(), standardError, 0) ;
		return ;
	}

	NSRichEdit::ReplaceWith(pDocTtx->GetExtFileName().c_str()) ;

	// on met un nom de fichier vide pour forcer l'importation  // set a blank file name in order to force import  //	pDocTtx->SetNomFichier("") ;	RemplaceTagsRTF() ;	pDocTtx->SetDirty(true) ;}voidNSTtxView::CmOuvrir(){
  if (string("") == pDocTtx->GetExtFileName())
  {
    erreur("Le nom de fichier � ouvrir n'est pas pr�cis�.", standardError, 0) ;
    return ;
  }

  NSRichEdit::ReplaceWith(pDocTtx->GetExtFileName().c_str()) ;

  string sFileName = (string)(NSRichEdit::GetFileName()) ;
  pDocTtx->SetNomFichier(sFileName) ;
}
void
NSTtxView::CmFileSave()
{
  string ps = string("User saves text") ;
  pDocTtx->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	Enregistrer() ;
}

voidNSTtxView::CmFileSaveAs(){
	EnregistrerSous() ;
}

voidNSTtxView::CmFilePrint(){
/*
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
*/
}

void
NSTtxView::CmFilePrintPreview()
{
/*
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
*/
}

// Fonctions de r�ponse aux commandes de format////////////////////////////////////////////////////////////////
void
NSTtxView::CmFormatFont()
{
  // Retrouve format caract�re courant
  NSCharFormat2 chFormat(*this) ;

  // Fill logfont structure using info. From character format
  LOGFONT lf ;
  chFormat.SetFontInfo(lf) ;
  lf.lfCharSet = DEFAULT_CHARSET ;
  TClientDC dc(*this) ;

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
	// Mise � vide du nom de fichier
	SetNomFichier("") ;
	Open(0, "") ;
}

NSSimpleTxtDocument::NSSimpleTxtDocument(TDocument *parent, NSContexte *pCtx)              :NSRefDocument(parent, pCtx)
{
	// Mise � vide du nom de fichier
	SetNomFichier("") ;
	Open(0, "") ;
}

void NSSimpleTxtDocument::SetNomFichier(string sFich){
	_sFileName = sFich ;
}

string NSSimpleTxtDocument::GetDocExt(){
	size_t i ;
	string sExt = string("") ;

	for (i = 0; (i < strlen(_sFileName.c_str())) && (_sFileName[i] != '.'); i++) ;

  if (i < strlen(_sFileName.c_str()))
  {
    // nomDoc[i] == '.'
    i++ ;
    while (i < strlen(_sFileName.c_str()))
      sExt += _sFileName[i++] ;
  }

  return sExt ;
}

// Ouverture du document////////////////////////////////////////////////////////////////

bool NSSimpleTxtDocument::Open(int /*mode*/, LPCSTR path){
	string sNomFichier = string("") ;

	// Si il existe, prise du nom de fichier de la fiche document  ValideFichier(&sNomFichier);

	// Sinon, on sort	if (false == _bDocumentValide)
		return false ;

	//	// Rangement du nom de fichier dans le document
	//
	SetNomFichier(sNomFichier) ;  SetDirty(false) ;

  return true ;}

// Fermeture du document////////////////////////////////////////////////////////////////
bool NSSimpleTxtDocument::Close()
{
  // if (TDocument::Close())
  // 	 return false;

  SetNomFichier("");
  return true;}

// G�n�ration du html correspondant au document////////////////////////////////////////////////////////////////
bool
NSSimpleTxtDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	NSHtmlTXT htxt(typeOperation, this, _sFileName.c_str(), pContexte, sAdresseCorresp) ;
	string    sBaseImg ;
	// NSBaseImages* pBase ;

	// on trouve le nom du fichier temporaire � visualiser	sNomHtml = htxt.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	string sFichierHtml = sPathHtml + sNomHtml ;

	// on passe aussi le path destination pour les images	if (!htxt.genereHtml(sFichierHtml, sBaseImg, _pHtmlInfo, sPathDest))
		return false ;

	// Mise � jour de la base d'images	switch (typeOperation)
	{
  	case toComposer:
    	_sBaseCompo = sBaseImg ;
      break ;

		default:    	_sBaseImages = sBaseImg ;
	}

	return true ;}

bool
NSSimpleTxtDocument::GenereHtmlText(string& sHtmlText)
{
  sHtmlText = string("") ;

  return true ;
}

// Indique si document ouvert (il existe un fichier associ�)////////////////////////////////////////////////////////////////
bool NSSimpleTxtDocument::IsOpen()
{
  return (string("") != _sFileName) ;
}

// Trouve un nom de fichier nautilus au document////////////////////////////////////////////////////////////////
bool NSSimpleTxtDocument::Renommer(string& sNomFichier, string& sLocalis, string sType, string* psNomBrut)
{
    string sPath = "";
    string sExt = GetDocExt();

    if (!TrouveNomFichier(sType,sExt,sNomFichier,sLocalis))
    {
    	erreur("Pb � l'attribution d'un nouveau nom pour ce fichier",standardError,0,pContexte->GetMainWindow()->GetHandle());
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

// Table de r�ponses de la classe NSSimpleTxtView

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

// GetWindow renvoie this (� red�finir car virtual dans TView)
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

  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_EDITUNDO,   CM_EDITUNDO,   TButtonGadget::Command));  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_EDITCUT,    CM_EDITCUT,    TButtonGadget::Command));
  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_EDITCOPY,   CM_EDITCOPY,   TButtonGadget::Command));
  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_EDITPASTE,  CM_EDITPASTE,  TButtonGadget::Command));
  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatSeparatorGadget(6));
	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_FORMULE,    CM_FORMULE,    TButtonGadget::Command));
  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_LETTRETYPE, CM_LETTRETYPE, TButtonGadget::Command));
  pMyApp->getSecondaryControlBar()->LayoutSession();}

// Remplissage de la vue////////////////////////////////////////////////////////////////
void NSSimpleTxtView::FillViewData()
{
  if (pDocTxt->IsOpen())
  {
    TEditFile::ReplaceWith(pDocTxt->_sFileName.c_str()) ;
    TEditFile::ClearModify() ;
    pDocTxt->SetDirty(false) ;
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

		// si le fichier n'est pas r�f�renc�, on prend le nom de fichier (si il existe)		if (NULL == pDocTxt->_pDocInfo)
		{
			if (FileName)
				strcpy(cfNomDocument, FileName) ;
		}
		else // sinon on prend le nom de document
			strcpy(cfNomDocument, pDocTxt->GetTitle()) ;

		string sCaption = string("Message ") + pDocTxt->pContexte->getSuperviseur()->getAppName().c_str() ;
		sprintf(msg, "Le document %s a �t� modifi�. Voulez vous le sauvegarder ?",
                FileName ? (const char far*)cfNomDocument : "texte cr��") ;

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
    menu->AppendMenu(MF_STRING, CM_FORMULE,     "Ins�rer formule");

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
	bool   bRef = false;
  string sFichier = pDocTxt->_sFileName ;
  string sNomFichier;
  string sLocalis;

  // si le fichier n'est pas r�f�renc�  if (NULL == pDocTxt->_pDocInfo)  {
    // On doit le r�f�rencer
    bRef = true ;

    // si ce n'est pas un nouveau fichier (nomFichier != NULL)    if (string("") != sFichier)
    {
      int retVal = MessageBox("Voulez-vous importer le document ?", "Message", MB_YESNOCANCEL) ;

      if (IDCANCEL == retVal)        return false ;

      if (IDYES == retVal) // cas de l'importation        pDocTxt->Renommer(sNomFichier, sLocalis) ;
      else // cas du r�f�rencement
      {
        sNomFichier = sFichier ;
        sLocalis    = string("") ;
      }
    }
    else // Nouveau fichier : on le renomme
      pDocTxt->Renommer(sNomFichier, sLocalis) ;
  }

  if (bRef)  {
    if (pDocTxt->Referencer("ZTRTF", "", sNomFichier, sLocalis))
    {
      TEditFile::SetFileName(pDocTxt->_sFileName.c_str()) ;
      bool bSaveOk = TEditFile::Write() ;

      if (false == bSaveOk)      {
        erreur("Erreur lors de la sauvegarde.",standardError,0,pDocTxt->pContexte->GetMainWindow()->GetHandle());
        pDocTxt->SetNomFichier(sFichier);
        return false;
      }

      pDocTxt->SetDirty(false);
      // on remet le titre � jour      TWindow::SetDocTitle(pDocTxt->GetTitle(), 0) ;
    }
    else
    {
      pDocTxt->SetDirty(true);

      // on remet le document dans l'�tat initial      pDocTxt->SetNomFichier(sFichier);
    }
  }
  else // cas pDocInfo != 0 : on sauvegarde le fichier
  {
    TEditFile::SetFileName(pDocTxt->_sFileName.c_str()) ;
    bool bSaveOk = TEditFile::Write() ;

    if (false == bSaveOk)    {
      erreur("Erreur lors de la sauvegarde.", standardError, 0, pDocTxt->pContexte->GetMainWindow()->GetHandle()) ;
      pDocTxt->SetNomFichier(sFichier) ;
      return false ;
    }

    pDocTxt->SetDirty(false) ;    TWindow::SetDocTitle(pDocTxt->GetTitle(), 0) ;
  }

  return true ;}

// Fonction EnregistrerSous : Importe automatiquement////////////////////////////////////////////////////////////////
bool NSSimpleTxtView::EnregistrerSous(){	string   sFichier = pDocTxt->_sFileName ;
   	string   sNomFichier;
   	string   sLocalis;
   	bool	 bSaveOk;

   	pDocTxt->Renommer(sNomFichier,sLocalis);
    if (pDocTxt->Referencer("ZTTXT","",sNomFichier,sLocalis))
    {        TEditFile::SetFileName(pDocTxt->_sFileName.c_str()) ;
   	    bSaveOk = TEditFile::Write();

   	    if (!bSaveOk)   	    {
   		    erreur("Erreur lors de la sauvegarde.",standardError,0,pDocTxt->pContexte->GetMainWindow()->GetHandle());
   		    pDocTxt->SetNomFichier(sFichier);
   		    return false;
   	    }

   	    pDocTxt->SetDirty(false);
        // on remet le titre � jour        TWindow::SetDocTitle(pDocTxt->GetTitle(),0);
    }
    else
   	{
      	pDocTxt->SetDirty(true);

      	// on remet le document dans l'�tat initial      	pDocTxt->SetNomFichier(sFichier);
   	}

   	return true;}

// Fonction de r�ponse � la commande Composer////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmComposer()
{
	pDocTxt->Composer();
}

// Fonction de r�ponse � la commande Publier////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmPublier()
{
	pDocTxt->Publier();
}

// Fonction de r�ponse � la commande Visualiser////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmVisualiser()
{
	pDocTxt->Visualiser();
}

// Fonction de r�ponse � la commande Formule////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmFormules()
{
	string sTexte = string("") ;

	char far str[MAXCARS] ;
	LettreTypeDialog *pLTDlg = new LettreTypeDialog(this, pDocTxt->pContexte) ;
	if (pLTDlg->Execute() == IDOK)	{
		if (pLTDlg->existsIntro())
    	sTexte = pLTDlg->getIntroText() + string("\n\n") + sTexte ;

    if (pLTDlg->existsBody())    	sTexte += pLTDlg->getBodyText() + string("\n\n") ;

    if (pLTDlg->existsPolite())    	sTexte += pLTDlg->getPoliteText() + string("\n\n") ;

    GetText(str, MAXCARS) ;
    if (strcmp(str, ""))    	strcat(str, "\n\n") ;

    strcat(str, sTexte.c_str()) ;    SetText(str) ;
    pDocTxt->SetDirty(true) ;
	}

	delete pLTDlg ;}

// Fonction de r�ponse � la commande EditCopy////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmEditCopy()
{
	TEditFile::CmEditCopy();
}

// Fonction de r�ponse � la commande EditCut////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmEditCut()
{
	TEditFile::CmEditCut();
	pDocTxt->SetDirty(true);
}

// Fonction de r�ponse � la commande EditPaste////////////////////////////////////////////////////////////////
void NSSimpleTxtView::CmEditPaste()
{
	TEditFile::CmEditPaste();
	pDocTxt->SetDirty(true);
}

// Fonctions de r�ponse aux commandes File////////////////////////////////////////////////////////////////
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


