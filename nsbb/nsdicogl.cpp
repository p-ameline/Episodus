#include <checks.h>
#include <owl\control.h>
#include <owl\gdiobjec.h>

#include "nssavoir\nsfilgd.h"
#include "nssavoir\nspathor.h"
#include "nsbb\nsdicogl.h"
#include "nsbb\nsbb.h"
#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"

//
// General Controls diagnostic group
//
// DIAG_DEFINE_GROUP_INIT(OWL_INI, OwlControl, 1, 0);

DEFINE_RESPONSE_TABLE1(NSEditDicoGlobal, TEdit)
   EV_WM_SETFOCUS,//le lexique récupére le focus et reférence NSEditDicoGlobal
                  //comme controle edit
END_RESPONSE_TABLE;

//
// Constructeur
//
NSEditDicoGlobal::NSEditDicoGlobal(NSContexte* pCtx, TWindow* parent, int resId,
                                   NSDico* pDictio, string sTypeLexiq,
                                   uint textLen, TModule* module)
                 :TEdit(parent, resId, textLen, module), NSRoot(pCtx)
{
	// Note : On ruse en passant par pSuper pour prendre la valeur
	// effective de pDico (mise à jour par le superviseur) car
	// pSuper->pDico peut etre mis à NULL sans que pDictio le soit
	if (pDictio)
		_pDico = pDictio->pContexte->getDico() ;
	else
		_pDico = (NSDico*) 0 ;
	_sTypeLexique  = sTypeLexiq ;
	_bGardeFocus   = false ;
	_bValidContent = true ;

  _bYouMustAcceptToLooseFocus = false ;

	// Attr.Style |= ES_MULTILINE | ES_WANTRETURN;
	// Attr.Style &= ~ES_AUTOHSCROLL ;
	Attr.Style &= ~ES_AUTOVSCROLL ;
}

NSEditDicoGlobal::NSEditDicoGlobal(NSContexte* pCtx, TWindow* parent,
                                   int resourceId, NSDico* pDictio,
                                   const char far* text, int x, int y,
                                   int w, int h, uint textLimit, bool multiline,
                                   string sTypeLexiq, TModule* module)
                 :TEdit(parent, resourceId, text, x, y, w, h, textLimit, multiline, module), NSRoot(pCtx)
{
  // Note : On ruse en passant par pSuper pour prendre la valeur
  // effective de pDico (mise à jour par le superviseur) car
  // pSuper->pDico peut etre mis à NULL sans que pDictio le soit
  if (pDictio)
    _pDico = pDictio->pContexte->getDico() ;
  else
		_pDico = (NSDico*) 0 ;
  _sTypeLexique  = sTypeLexiq ;
  _bGardeFocus   = false ;
  _bValidContent = true ;

  _bYouMustAcceptToLooseFocus = false ;

  //pour spécifier le return pour un champ edit il faut préciser tout d'abord ES_MULTILINE
  Attr.Style |= ES_WANTRETURN ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSEditDicoGlobal::SetupWindow()
{
try
{
	TEdit::SetupWindow() ;

	if (NULL == _pDico->getDicoDialog())
		_pDico->setDicoDialog(new ChoixPathoDialog(_pDico->pContexte->GetMainWindow(), _pDico, this, _sTypeLexique, pNSDLLModule)) ;
	else
		_pDico->getDicoDialog()->setEdit(this, _sTypeLexique) ;
}
catch (...)
{
	erreur("Exception NSEditDicoGlobal::SetupWindow;", standardError, 0) ;
}
}

//
// Destructeur
//
NSEditDicoGlobal::~NSEditDicoGlobal()
{
  // Note : On ruse en passant par pSuper pour prendre la valeur
  // effective de pDico (mise à jour par le superviseur).
  //
  if (_pDico->pContexte->getDico())
  {
    _pDico = _pDico->pContexte->getDico() ;

    if (_pDico->getDicoDialog())
    	_pDico->getDicoDialog()->cacherDialogue() ;
  }
}

//----------------------------------------------------------------------------
void
NSEditDicoGlobal::SetDico(NSDico* pDictio)
{
  // Note : On ruse en passant par pSuper pour prendre la valeur
  // effective de pDico (mise à jour par le superviseur) car
  // pSuper->pDico peut etre mis à NULL sans que pDictio le soit
  if (pDictio)
  	_pDico = pDictio->pContexte->getDico() ;
  else
		_pDico = (NSDico*) 0 ;
}

//----------------------------------------------------------------------------
bool
NSEditDicoGlobal::EveilleToi(string* sTexteInitial, RECT* rectEdit)
{
	if ((NULL == sTexteInitial) || (NULL == rectEdit))
		return false ;

	strip(*sTexteInitial) ;

	RECT rectEditeur = *rectEdit ;
	//
	// Calcul des dimensions et de la position à donner au contrôle Edit
	//
	if (rectEditeur.right - rectEditeur.left < 50)
		rectEditeur.right = rectEditeur.left + 40 ;
  //if (rectEditeur.bottom - rectEditeur.top < 50)
  //	rectEditeur.bottom = rectEditeur.top + 30;

  _rect = rectEditeur ;

  //position du lexique
  NS_CLASSLIB::TRect rectPere = Parent->GetWindowRect();
  // rect.left= rectEdit->left (Edit) par rapport à la boîte de dialogue
  // rectPere.left (la boîte de dialogue ) par rapport à l'écran
  // pTree->Attr.X
  NS_CLASSLIB::TRect rectEdition = GetWindowRect();
  int x = _rect.left + _rect.Width() + rectPere.left  + 100 ;
  int y = rectEdition.top ;

  if (_pDico && _pDico->getDicoDialog())
  {
    NS_CLASSLIB::TRect rectLex = _pDico->getDicoDialog()->GetWindowRect() ;
    _pDico->getDicoDialog()->SetWindowPos(0, x, y, rectLex.Width(), rectLex.Height(), SWP_NOZORDER) ;
  }
  //
  // Positionnement du contrôle Edit
  //
  MoveWindow(rectEditeur, /*bool repaint*/ false) ;
  //
  // Initialisation du contrôle
  //
  SetText(sTexteInitial->c_str()) ;
  // pDico->pDicoDialog->SetFenetreAppel(this);

  //
  // Visualisation du contrôle
  //
	Show(SW_SHOWNORMAL) ;

	return true ;
}

//----------------------------------------------------------------------------// masquer le lexique et le rectangle d'édition
//----------------------------------------------------------------------------
void
NSEditDicoGlobal::EndortToi()
{
	Show(SW_HIDE) ;
	if (_pDico && _pDico->getDicoDialog())
		_pDico->getDicoDialog()->cacherDialogue() ;
}

//-------------------------------------------------------------------------//-------------------------------------------------------------------------
void
NSEditDicoGlobal::GetLabelChoisi(string* sChaineRecuperee)
{
	if (_pDico && _pDico->getDicoDialog())
		_pDico->getDicoDialog()->GetLabelChoisi(sChaineRecuperee) ;
}

//-------------------------------------------------------------------------//-------------------------------------------------------------------------
void
NSEditDicoGlobal::GetCodeLexiqueChoisi(char* code)
{
	if (_pDico && _pDico->getDicoDialog())
		_pDico->getDicoDialog()->GetCodeLexique(code) ;
}

//-------------------------------------------------------------------------
//le control edit gagne le focus
//-------------------------------------------------------------------------
void
NSEditDicoGlobal::EvSetFocus(HWND hWndLostFocus)
{
	TEdit::EvSetFocus(hWndLostFocus) ;
	if (_pDico && _pDico->getDicoDialog())
		_pDico->getDicoDialog()->setEdit(this) ;
}

void
NSEditDicoGlobal::EvKillFocus(THandle hWndGetFocus)
{
  if (_pDico && _pDico->getDicoDialog())
    _pDico->getDicoDialog()->setEdit(NULL) ;

  TEdit::EvKillFocus(hWndGetFocus) ;
}

bool
NSEditDicoGlobal::canWeClose()
{
  return _bValidContent ;
}

bool
NSEditDicoGlobal::isEmpty()
{
  return (GetTextLen() == 0) ;
}

//-------------------------------------------------------------------------

