#include <checks.h>
#include <owl\control.h>
#include <owl\treewind.h>
#include <owl\gdiobjec.h>

#include "nsbb\nsdico.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nstrnode.h"
#include "nsbb\nstrnode.rh"
#include "nsbb\nsPaneSplitter.h"

#include "nsbb\nsbb_dlg.h"
#include "nsbb\nsdlg.h"

#include "nsbb\nsbb.h"
#include "nssavoir\nspathor.h"

//
// General Controls diagnostic group
//
// DIAG_DEFINE_GROUP_INIT(OWL_INI, OwlControl, 1, 0);

#define CM_ANNULER    501   // Don't use CM_EDITUNDO
#define CM_EMPHASIZED 502
#define CM_STANDARD   503

DEFINE_RESPONSE_TABLE1(NSEditDico, NSEditDicoGlobal)
	EV_WM_CHAR,
  EV_WM_KEYDOWN,
  EV_WM_LBUTTONDOWN,
  EV_WM_LBUTTONUP,
  EV_WM_RBUTTONDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_KEYUP,
  EV_WM_GETDLGCODE,
  EV_WM_PAINT,
  EV_WM_MOUSEMOVE,
  EV_WM_SETFOCUS,
  EV_COMMAND(CM_EDITPASTE,  CmEditPaste),
  EV_COMMAND(CM_ANNULER,    undoOneStep),
  EV_COMMAND(CM_EDITREDO,   redoOneStep),
  EV_COMMAND(CM_EMPHASIZED, setBold),
  EV_COMMAND(CM_STANDARD,   setRegular),
END_RESPONSE_TABLE;

NSEditDico::NSEditDico(NSContexte* pCtx, TWindow* parent, int resourceId, NSDico* pDictio,
                       string sTypeLexique, const char far* text,
                       int x, int y, int w, int h,
                       uint textLimit, bool multiline, TModule* module,
                       bool bLexiqueGeneral)
           :NSEditDicoGlobal(pCtx, parent, resourceId, pDictio, text, x, y, w, h,
                       textLimit, multiline, sTypeLexique, module)
{
	_bPaint          = false ;
	_bTexteLibre     = false ;
  _bGardeFocus     = false ;
  _iPrevGardeFocus = -1 ; 
	_sAmmorceLibre   = string("") ;
	_bGeneral        = bLexiqueGeneral ;
  _bKillSent       = false ;
  _bBeingDestroyed = false ;

  Attr.Style |= ES_NOHIDESEL ;

  _undoCursor = _undoBuffer.end() ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSEditDico::SetupWindow()
{
	NSEditDicoGlobal::SetupWindow() ;

  // EM_FMTLINES Message
	// Sets a flag that determines whether a multiline edit control includes
  // soft line-break characters. A soft line break consists of two carriage
  // returns and a line feed and is inserted at the end of a line that is
  // broken because of wordwrapping.
	FormatLines(true) ;

	//
	//cacher les scrollbar
	//
	ShowScrollBar(SB_BOTH, FALSE) ;
}

//----------------------------------------------------------------------------
// positionner le lexique selon les coordonnées de l'écran et celles de la boîte
// mère
//----------------------------------------------------------------------------
void
NSEditDico::SetPositionLexique()
{
	if (((NSDico*) NULL == _pDico) || (NULL == _pDico->getDicoDialog()))
		return ;

	//
	//coordonnées de l'écran en pixels
	//
  TIC* pTic = new TIC("DISPLAY",0,0) ;
  int XScreen = pTic->GetDeviceCaps(HORZRES) ;
  int YScreen = pTic->GetDeviceCaps(VERTRES) ;
  delete pTic ;

  // rect.left= rectEdit->left (Edit) par rapport à la boîte de dialogue
  // rectPere.left (la boîte de dialogue ) par rapport à l'écran
  // pTree->Attr.X

	//position du lexique
	NS_CLASSLIB::TRect rectEdition = GetWindowRect() ; //rectangle d'édition (this)

  //
  // Attr : this
  //
  NS_CLASSLIB::TRect rectBoiteMere = Parent->GetWindowRect() ; //boîte mère
  //
  // Attr : this
  //
  //int x = Attr.X + Attr.W + rectBoiteMere.left  + 5 ;
  int x = rectEdition.left + rectEdition.Width() + 50 ;
  int y = rectEdition.top ;
  int lTW_X = 10 ;
  // int lTW_Y = 10;

  NS_CLASSLIB::TRect rectLex = _pDico->getDicoDialog()->GetWindowRect() ;  //lexique
  if ((x + rectLex.Width()) >= XScreen)//si elle ne tient pas à droite
  {
  	// si elle tient à gauche
    if (rectEdition.left - rectLex.Width() - lTW_X >= 0)
    	x = rectEdition.left - rectLex.Width() - lTW_X ;
    // si elle ne tient pas à gauche
    else
    {
    	// on la cale à droite
      if (XScreen - rectEdition.left - rectEdition.Width() > rectEdition.left)
      	x = XScreen - rectLex.Width() ;
      else
      	x = 0 ;
    }
  }
	if ((y + rectLex.Height()) >= YScreen)
		y = YScreen - rectLex.bottom + rectLex.top ;

	/*
  SWP_DRAWFRAME	Draws a frame around the window.
	SWP_FRAMECHANGED	Sends a message to the window to recalculate the window's size.
   						If this flag is not set, a recalculate size message is sent only at
            			the time the window's size is being changed.
  SWP_HIDEWINDOW	Hides the window.
	SWP_NOACTIVATE	Does not activate the window.
   					If this flag is not set, the window is activated and moved
         			to the top of the stack of windows.
	SWP_NOCOPYBITS	Discards the entire content area of the client area of the window.
   					If this flag is not set, the valid contents are saved and copied into
                  the window after the window is resized or positioned.
	SWP_NOMOVE		Remembers the window's current position.
	SWP_NOSIZE		Remembers the window's current size. +
	SWP_NOREDRAW	Does not redraw any changes to the window.  + ok
   					If this flag is set, no repainting of any window area
                  (including client, nonclient, and any window part uncovered as a result of a move) occurs. When this flag is set, the application must explicitly indicate if any area of the window is invalid and needs to be redrawn.
	SWP_NOZORDER	Remembers the current Z-order (window stacking order).
	SWP_SHOWWINDOW	Displays the window. + pas ok
	*/
	_pDico->getDicoDialog()->SetWindowPos(0, x, y, rectLex.Width(), rectLex.Height(), SWP_NOSIZE) ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSEditDico::EveilleToi(string* pTexteInitial, RECT* pRectEdit)
{
	if (((RECT*) NULL == pRectEdit) || ((string*) NULL == pTexteInitial))
		return ;

  char szThis[20] ;
  sprintf(szThis, "%p", HWindow) ;
  string sMsg = string("EditDico ") + string(szThis) + string(" EveilleToi") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	strip(*pTexteInitial) ;

	RECT rectEditeur = *pRectEdit ;
	SetPositionLexique() ;
	//
	// On fixe la position de l'Edit
	//
	if ((0 == pRectEdit->bottom) || (0 == pRectEdit->right))
	{
		// on repasse en mode texte libre
    _bTexteLibre   = true ;
    ShowScrollBar(SB_VERT, TRUE) ;
    _sAmmorceLibre = *pTexteInitial ;
    AjusterRectangleEdition(*pTexteInitial) ;
	}
	else
		MoveWindow(rectEditeur, /*bool repaint*/ true) ;
	//
	// On découpe le texte en paragraphes et on le place dans l'Edit
	//
	Paragraphe(pTexteInitial) ;
	SetText(pTexteInitial->c_str()) ;
	//
	// On demande à l'Edit de se montrer
	//
	Show(SW_SHOWNORMAL) ;

	// On regarde le nombre de lignes :
	// pour 0 (texte vide) et > 1 (texte libre) on ne fait rien
	// pour 1 on se place à la fin du texte et on appelle UpdateDico
	// (lance le lexique selon la valeur de l'ammorce)
	if (GetNumLines() == 1)
	{
    int nbCars = strlen(pTexteInitial->c_str()) ;
    SetSelection(nbCars, nbCars) ;
    UpdateDico() ;

    // si le texte est un élément lexique
    // on le sélectionne dans pDicoDialog
    // (on doit récupérer le code lexique du node en cours)

    bool bPreviousFocusState = _bGardeFocus ;
    _bGardeFocus = true ;

    if (_pTreeAEditer)
    {
      TTreeNode node = _pTreeAEditer->GetSelection() ;
      NSTreeNode* pTreeNode = _pTreeAEditer->GetNSTreeNode(&node) ;
      if (pTreeNode)
      {
        string sCodeLex = pTreeNode->getEtiquette() ;
        if ((string("£?????") != sCodeLex) && _pDico->getDicoDialog())
    	    _pDico->getDicoDialog()->SelectCodeLexique(sCodeLex) ;
      }
    }

    _bGardeFocus = bPreviousFocusState ;
	}

  sMsg = string("EditDico ") + string(szThis) + string(" EveilleToi Leaving") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
}

//-----------------------------------------------------------------------
// Ajuster le rectangle d'édition (selon la taille de sLabel)
//
// Cette fonction ne travaille que sur right et bottom
//-----------------------------------------------------------------------
void
NSEditDico::AjusterRectangleEdition(string sLabel)
{
	//
  // Prise d'un TIC lié à l'écran et des caractéristiques de la police
  //
  TIC* pIc = new TIC("DISPLAY", 0, 0) ;
  TEXTMETRIC metric ;
  pIc->GetTextMetrics(metric) ;
  // LONG largeurChar = metric.tmMaxCharWidth;
  // LONG hauteurChar = metric.tmHeight;
  int  cxScroll ;
  //
  // ?????
  //
  // int LargeurMax = (int)largeurChar*10; //10
  // int longueur  = 10;//GetTextLen();
  // int nbBloc = longueur;
  // int LargeurCalculee = nbBloc * (int)largeurChar;
  NS_CLASSLIB::TRect rectEdition = GetWindowRect() ;
  _rect = rectEdition ;
  NS_CLASSLIB::TRect rectBoiteMere = Parent->GetWindowRect() ; //boîte mère
  //TRect rectControle  = pTreeAEditer->GetWindowRect(); //TreeView

  rectEdition.right  = rectBoiteMere.right  - 50 ;
  rectEdition.bottom = rectBoiteMere.bottom - 80 ;
  if ((rectEdition.bottom - rectEdition.top) <= 50)
		rectEdition.bottom = rectEdition.top + 300 ;

	//rectEdition.bottom = rectEdition.top  + 20*(int)largeurChar;

	// on doit retrancher au rectEditDico la largeur de la scrollbar
	// (si la scrollbar est définie pour le NSEditDico)
	if (_bTexteLibre)
		cxScroll = ::GetSystemMetrics(SM_CXVSCROLL) ;
	else
		cxScroll = 0 ;

	NS_CLASSLIB::TPoint point(rectEdition.left, rectEdition.top) ;
	NS_CLASSLIB::TPoint point2(rectEdition.right - cxScroll, rectEdition.bottom) ;
	Parent->ScreenToClient(point) ;
	Parent->ScreenToClient(point2) ;

  // on reporte la modif du rectangle dans rectEditDico
  // pour gérer les changements du curseur par NSTreeWindow::EvMouseMove
  // Note : on doit passer le rectangle en coordonnées Client
  if (_pTreeAEditer)
    _pTreeAEditer->rectEditDico = NS_CLASSLIB::TRect(point, point2) ;

  //
  // Positionnement du contrôle Edit (la taille inclus la scrollbar)
  //
  SetWindowPos(0, point.X(), point.Y(), rectEdition.right - rectEdition.left, rectEdition.bottom
                - rectEdition.top ,SWP_NOZORDER) ;
  delete pIc ;
}

//-----------------------------------------------------------------------
// ajuster le rectangle d'édition chaque fois qu'on tape une lettre
//
// Cette fonction ne travaille que sur right et bottom
//-----------------------------------------------------------------------
void
NSEditDico::AjusterRectangleEdition()
{
  TIC* pIc = new TIC("DISPLAY", 0, 0) ;
  TEXTMETRIC metric ;
  pIc->GetTextMetrics(metric) ;
  LONG largeurChar = metric.tmMaxCharWidth ;
  // LONG hauteurChar = metric.tmHeight;
  int LargeurMax = (int)largeurChar*10 ; //10
  delete pIc ;
  int longueur  = 10 ; //GetTextLen();
  int nbBloc    = (longueur / 5 ) + 1 ; //nombre de blocs
  int LargeurCalculee = 5 * nbBloc * (int)largeurChar ;
  NS_CLASSLIB::TRect rectEdition   = GetWindowRect() ;
  NS_CLASSLIB::TRect rectBoiteMere = Parent->GetWindowRect() ; //boîte mère
  _rect = rectEdition ;
  if (LargeurCalculee > LargeurMax)
  	rectEdition.right  = rectBoiteMere.right - 50 ;
  else
  	if ((rectEdition.left + LargeurCalculee) < (rectBoiteMere.right - 10))
    	rectEdition.right  = rectEdition.left + LargeurCalculee ;

  NS_CLASSLIB::TPoint point(rectEdition.left, rectEdition.top) ;
  Parent->ScreenToClient(point) ;
  //
  // Positionnement du contrôle Edit
  //
  SetWindowPos(0, point.X(), point.Y(), rectEdition.right - rectEdition.left, rectEdition.bottom
                - rectEdition.top ,SWP_NOZORDER) ;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void
NSEditDico::EvChar(uint key, uint repeatCount, uint flags)
{
	_pDico->getDicoDialog()->setEdit(this, _sTypeLexique) ;
	Invalidate() ;

  short iCtrlPressedStatus = GetAsyncKeyState(VK_CONTROL) ;
  bool  bCtrlPressed = (iCtrlPressedStatus & 0x8000) != 0 ;

	if ((VK_RETURN == key) || (VK_TAB == key))
  {
    if (false == bCtrlPressed)
			//pour enlever le Beep ne pas appliquer TEdit::EvChar(key, repeatCount, flags)
    	return ;
  }

	TEdit::EvChar(key, repeatCount, flags) ;
	UpdateDico() ;

  // Ctrl-Z -> key == 26
  if (26 != key)
    addToUndoBuffer() ;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void
NSEditDico::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	Invalidate() ;
	_pDico->getDicoDialog()->setEdit(this, _sTypeLexique) ;

  // The "natural behavior" of Escape key is to kill the tree window.
  // It is mandatory to intercept it!
  //
  if (VK_ESCAPE != key)
  {
	  TEdit::EvKeyDown(key, repeatCount, flags) ;
	  Parent->SendNotification(Attr.Id, 200, HWindow) ;
  }

	// flêche basse
	//
	if (VK_DOWN == key)
	{
  	// Si on est en mode TEXTE LIBRE, passer à la ligne suivante du TL
    //
    if (_bTexteLibre)
    {
    }
    // Si on n'est pas en mode TEXTE LIBRE, soit on passe au noeud suivant
    // soit on descend d'un élément dans le pDicoDialog
    //
    else
      goToNextNode() ;
	}
	// flêche haute
  //
	else if (VK_UP == key)
	{
		// Si on est en mode TEXTE LIBRE, passer à la ligne préc. du TL
    //
    if (_bTexteLibre)
    {
    }
    // Si on n'est pas en mode TEXTE LIBRE, soit on passe au noeud précédent
    // soit on monte d'un élément dans le pDicoDialog
    //
    else
    	goToPreviousNode() ;
  }
  else if ((VK_INSERT == key) || (VK_RETURN == key))
	{
  	short iCtrlPressedStatus = GetAsyncKeyState(VK_CONTROL) ;
    bool  bCtrlPressed = (iCtrlPressedStatus & 0x8000) != 0 ;
    if ((VK_RETURN == key) && (true == bCtrlPressed))
    	TEdit::EvKeyDown(key, repeatCount, flags) ;
    else
      TEdit::EvKeyDown(key, repeatCount, flags) ;

    // Moved to KeyUp
    // else
		//	_pDico->getDicoDialog()->InsererElementLexique() ;
    // return ;
	}
	else if (VK_CONTROL == key)
	{
		_sTypeLexique = string("_") ;
    _pDico->getDicoDialog()->setEdit(this, _sTypeLexique) ;
	}
  // Ctrl-A select all
  else if (GetKeyState(VK_CONTROL) < 0)
  {
    if (GetKeyState(VK_SHIFT) < 0)
    {
      if (('Z' == key) || ('z' == key))
        redoOneStep() ;
    }
    else
    {
      if      (('A' == key) || ('a' == key))
        CmEditSelectAll() ;
      else if (('Z' == key) || ('z' == key))
        undoOneStep() ;
      else if (('G' == key) || ('g' == key))
        setBold() ;
      else if (('N' == key) || ('n' == key))
        setRegular() ;
    }
  }
  else if (VK_DELETE == key)
    addToUndoBuffer() ;

  // The "natural behavior" of Escape key is to kill the tree window.
  // It is mandatory to intercept it!
  //
  else if (VK_ESCAPE == key)
  {
    if (_pTreeAEditer && (false == _pTreeAEditer->bDirty))
    {
      NSDialog* pDialog = getNSDialog() ;
      if (pDialog)
        pDialog->CmCancel() ;
    }
    else
    {
      addToUndoBuffer() ;
      SetText("") ;
    }
  }
}

void
NSEditDico::goToNextNode()
{
  if ((NSTreeWindow*) NULL == _pTreeAEditer)
    return ;

  if (_pTreeAEditer->ModifierTexteLibre)
  {
    if (_pTreeAEditer->pNSTreeNodeEnCours->getGlobalText() == string(""))
      _pTreeAEditer->ModifierTexteLibre = false ;
    return ;
  }

  //si ammorce vide alors passer au NStrreNode suivant
  if (_pDico->getDicoDialog()->getAmmorce() == string(""))
  {
    _pTreeAEditer->NStreeNodeSuivant(true) ;

    // Don't do anything after NStreeNodeSuivant() since it contains KillDico()
    //
    // _pTreeAEditer->SortieEditionDico() ;
  }
  //sinon récupérer un élément dans le lexique
  else
    _pDico->getDicoDialog()->ElementSuivant() ;
}

void
NSEditDico::goToPreviousNode()
{
  if ((NSTreeWindow*) NULL == _pTreeAEditer)
    return ;

  //si ammorce vide alors passer au NStrreNode suivant
  if (_pTreeAEditer->ModifierTexteLibre)
  {
    if (_pTreeAEditer->pNSTreeNodeEnCours->getGlobalText() == string(""))
      _pTreeAEditer->ModifierTexteLibre = false ;
    return ;
  }

  if (_pDico->getDicoDialog()->getAmmorce() == string(""))
  {
    _pTreeAEditer->NStreeNodePrcedent() ;
    _pTreeAEditer->SortieEditionDico() ;
  }
  //sinon récupérer un élément dans le lexique
  else
    _pDico->getDicoDialog()->ElementPrecedent() ;
}

//---------------------------------------------------------------------// taper du texte libre
//---------------------------------------------------------------------
void
NSEditDico::TextLibre()
{
	int oldBuffLen = GetTextLen() ;
	char far* oldBuff = new char[oldBuffLen+1] ;
	GetText(oldBuff, oldBuffLen+1) ;
	string sContenu = string(oldBuff) ;
	delete[] oldBuff ;

  if (string("") == sContenu)
  {
    // Do this in order that emptyed element can be deleted
    //
    if (_pTreeAEditer)
      _pTreeAEditer->DicoGetTexteLibre(&sContenu) ;

    goToNextNode() ;
		return ;
  }

	strip(sContenu) ;

  // It is allowed to have just a "spacer" in order to separate two chapters
  //
	if (string("") == sContenu)
		sContenu = string(" ") ;

	//
	// enlever \r\n du texte
	//
  if ((NSTreeWindow*) NULL == _pTreeAEditer)
  {
		EnleverRetourChariot(&sContenu) ;
		strip(sContenu, stripRight) ;
	}
  else
  	RemoveSoftLineBreaks(&sContenu) ;

	// texte libre
	if (_pTreeAEditer)
    _pTreeAEditer->DicoGetTexteLibre(&sContenu) ;
}

//--------------------------------------------------------------
//enlever \n\r de pContenu et les remplacer par " "
//--------------------------------------------------------------
void
NSEditDico::EnleverRetourChariot(string* pContenu, uint* pStartSel, uint* pEndSel)
{
	//
  // enlever \r\n du texte
	//
  size_t pos = pContenu->find("\r\n") ;
  while (NPOS != pos)
	{
		if (pStartSel)
    	if ((*pStartSel) > pos)
      	(*pStartSel)-- ;

    if (pEndSel)
    	if ((*pEndSel) > pos)
      	(*pEndSel)-- ;

    pContenu->replace(pos, 2, " ") ;
    pos = pContenu->find("\r\n") ;
	}

	pos = pContenu->find("\n") ;
	while (NPOS != pos)
	{
  	pContenu->replace(pos, 1, " ") ;
    pos = pContenu->find("\n") ;
	}

	pos = pContenu->find("\r") ;
	while (NPOS != pos)
	{
		pContenu->replace(pos, 1, " ") ;
    pos = pContenu->find("\r") ;
	}
}

//--------------------------------------------------------------
//enlever les \r\r\n de pContenu et les remplacer par " "
//--------------------------------------------------------------
void
NSEditDico::RemoveSoftLineBreaks(string* pContenu, uint* pStartSel, uint* pEndSel)
{
	//
  // enlever \r\n du texte
	//
  size_t pos = pContenu->find("\r\r\n") ;
  while (NPOS != pos)
	{
		if (pStartSel)
    	if ((*pStartSel) > pos)
      	(*pStartSel) -= 2 ;

    if (pEndSel)
    	if ((*pEndSel) > pos)
      	(*pEndSel) -= 2 ;

    pContenu->replace(pos, 3, " ") ;
    pos = pContenu->find("\r\r\n") ;
	}
}

//----------------------------------------------------------------
//mettre pContenu sous forme de paragraphe
//----------------------------------------------------------------
void
NSEditDico::Paragraphe(string* pContenu)
{
	if (((string*) NULL == pContenu) || (string("") == *pContenu))
		return ;

	// uint curLine;
  int  cxScroll ;
  string sChaineSource = *pContenu ;
  TIC* pIc = new TIC("DISPLAY", 0, 0) ;
  uint   startSel, endSel ;
  GetSelection(startSel, endSel) ;

  // Pour recalculer les nouveaux sauts de ligne
  // on reprend la chaine à plat (les CRLF sont remplacés par des espaces)
  // EnleverRetourChariot(pContenu, &startSel, &endSel) ;
  RemoveSoftLineBreaks(pContenu, &startSel, &endSel) ;
  string sChaine = *pContenu ;

  //
  // Prise de la largeur éditable : largeur de l'Edit moins scrollbar
  //
  //TRect rectEdition   = GetWindowRect();
  NS_CLASSLIB::TRect rectEdition = GetClientRect() ;

  // largeur de la scrollbar
  if (_bTexteLibre)
  	cxScroll = ::GetSystemMetrics(SM_CXVSCROLL) ;
  else
  	cxScroll = 0 ;

  int     largeurEdit = rectEdition.right - rectEdition.left - cxScroll ;
  int     longueurChaine ;
  int     longueur ;
  int     enCoursAvant ;
  size_t  debLigne = 0 ;
  size_t  enCours = 0 ;
  string sTexte ;
  //
  bool Tourner = true ;
  while (Tourner)
  {
  	longueurChaine = 0 ;
    while ((enCours < strlen(sChaine.c_str())) &&
             (longueurChaine < largeurEdit)) //on risque de dépasser le contrôle édit
    {
    	if ((enCours < strlen(sChaine.c_str()) - 1) &&
          ('\r' == sChaine[enCours]) && ('\n' == sChaine[enCours+1]))
      {
      	enCours += 2 ;
      	break ;
      }

    	sTexte = string(sChaine, debLigne, enCours - debLigne + 1) ;
      NS_CLASSLIB::TSize size = pIc->GetTextExtent(sTexte.c_str(), strlen(sTexte.c_str())) ;
      longueurChaine = size.X() ;
      enCours++ ;
    }

    longueur = strlen(sChaine.c_str()) ;
    // Premier blanc
    if (longueurChaine >= largeurEdit)
    {
    	// on conserve la position du dernier caractère
      // (celui qui dépasse la largeur de la zone éditable)
      enCoursAvant = enCours - 1 ;

      // on recule jusqu'au premier blanc ou jusqu'au début de la ligne
      while ((enCours > debLigne) && (sTexte[enCours - debLigne] != ' '))
      	enCours-- ;

      if (enCours == debLigne) // cas de la ligne sans espace
      {
        enCours = enCoursAvant ;
        sChaine = string(sChaine, 0, enCours) + string("\r\r\n") + string(sChaine, enCours, longueur - enCours + 1) ;
        if (size_t(startSel) >= enCours)
        	startSel += 3 ;
        if (size_t(endSel) >= enCours)
        	endSel += 3 ;
      }
      else // !! dans ce cas on saute l'espace (on reprend à enCours + 1)
      {
      	sChaine = string(sChaine, 0, enCours) + string("\r\r\n") + string(sChaine, enCours + 1, longueur - enCours) ;
        if (startSel >= (enCours + 1))
        	startSel += 2 ;
        if (endSel >= (enCours + 1))
        	endSel += 2 ;
      }

      debLigne = enCours + 3 ;
      enCours = debLigne ;
    }
    else if (enCours >= strlen(sChaine.c_str()))
    	Tourner = false ;
	}
  delete pIc ;

	// on affiche le nouveau texte
	if (sChaine != sChaineSource)
		SetText(sChaine.c_str()) ;

	*pContenu = sChaine ;

	SetSelection(startSel, endSel) ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSEditDico::UpdateDico()
{
try
{
  char szThis[20] ;
  sprintf(szThis, "%p", HWindow) ;
  string sMsg = string("EditDico ") + string(szThis) + string(" UpdateDico: Entering.") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  //
  // Texte total
  //
  string sContenu = string("") ;
  int oldBuffLen = GetTextLen() ;
  if (oldBuffLen > 0)
  {
    char far* oldBuff = new char[oldBuffLen+1] ;
    GetText(oldBuff, oldBuffLen+1) ;
    sContenu = string(oldBuff) ;
    delete[] oldBuff ;
  }
  bool bCacherLexique = false ;
  //
  // Mettre ce texte sous forme de paragraphe
  //
  Paragraphe(&sContenu) ;

  // Si on est en texte libre, on teste si le texte commence encore par
  // la chaine qui a causé la fermeture du lexique
  //
  if (_bTexteLibre)
  {
  	size_t iTailleAmmorce = strlen(_sAmmorceLibre.c_str()) ;
    if ((strlen(sContenu.c_str()) >= iTailleAmmorce) &&
            (string(sContenu, 0, iTailleAmmorce) == _sAmmorceLibre))
    	return ;
	}

	int OouF ;
	if (string("") == sContenu)
	{
  	OouF = 1 ;
    bCacherLexique = true ;
	}
	else
		OouF = _pDico->getDicoDialog()->DonneAmmorce(&sContenu) ;

  //
  // Code lexique
  //
  if (1 == OouF)
  {
  	if (_bTexteLibre) //passage d'un edit texte libre à un edit code lexique
    	switchFromFreeToLexique(&sContenu) ;

    _bTexteLibre = false ;

    if (bCacherLexique)
    {
      sMsg = string("EditDico ") + string(szThis) + string(" UpdateDico: Hiding Lexique.") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
      _pDico->getDicoDialog()->cacherDialogue() ;
    }
    else
    {
      if (false == isEmpty())
      {
    	  _bGardeFocus = true ; // car pDico->pDicoDialog->ShowWindow(SW_SHOW) fait perdre le focus à this
			  _pDico->getDicoDialog()->montrerDialogue() ;
      }
    }
    SetFocus() ;
    _bGardeFocus = false ;
  }
  //
  // Texte libre
  //
  // On n'entre dans cette routine qu'au changement de mode (lexique -> TL)
  //
  else if ((2 == OouF) && (false == _bTexteLibre))
  {
    _bTexteLibre = true ;
    //
    // Montrer les scrollbar
    //
    ShowScrollBar(SB_VERT, TRUE) ;
    //
    // Cacher pDicoDialog
    //
		_pDico->getDicoDialog()->cacherDialogue() ;
    //
    // On se fixe une taille statique maximum par défaut
    //
    NS_CLASSLIB::TRect rectEdition = GetWindowRect() ;
    //
    // Ajuster le contrôle Edit à la dimension des textes libres
    //
    AjusterRectangleEdition("") ;
    //
    // Recalculer les paragraphes en fonction de la nouvelle taille
    //
    Paragraphe(&sContenu);
    //
    // Retenir l'ammorce qui a fait basculer d'un mode à l'autre
    //
    _sAmmorceLibre = sContenu ;
	}

  sMsg = string("EditDico ") + string(szThis) + string(" UpdateDico: Leaving.") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
}
catch (...)
{
  erreur("Exception NSEditDico::UpdateDico", standardError, 0) ;
}
}

//-------------------------------------------------------------------------
// 					Réponse à un message ok ou doubleClick
//-------------------------------------------------------------------------
void
NSEditDico::ElementSelectionne()
{
  if (_pTreeAEditer)
	  _pTreeAEditer->DicoGetCodeLexique() ;
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
void
NSEditDico::EvKeyUp(uint key, uint repeatcount, uint flags)
{
	// THandle NextControl = 0;
	switch (key)
	{
  	//return
    case VK_RETURN	:
    {
    	short iCtrlPressedStatus = GetAsyncKeyState(VK_CONTROL) ;
      bool  bCtrlPressed = (iCtrlPressedStatus & 0x8000) != 0 ;
  		if ((false == _bTexteLibre) || (false == bCtrlPressed))
      {
        TEdit::EvKeyUp(key, repeatcount, flags) ;

        if (_pDico->getDicoDialog()->hasSelectedElement())
          _pDico->getDicoDialog()->InsererElementLexique() ;
        else
    		  TextLibre() ;
      }
      else
      	TEdit::EvKeyUp(key, repeatcount, flags) ;
      break ;
    }
    //tabulation
    case VK_TAB	:
    {
      //
      // demander à la boîte de dialogue mère d'activer le controle
      // suivant pControle sinon le premier
      //
      NSDialog* pDialog = getNSDialog() ;
      if (pDialog)
        pDialog->ActiveControlSuivant(getControl()) ;
      else
      {
        NSBBMUEView* pMueVie = getMUEView() ;
        if (pMueVie)
        {
          if (GetKeyState(VK_SHIFT) < 0)
            pMueVie->SetFocusToPreviousControl((TControl*) _pTreeAEditer) ;
          else
            pMueVie->SetFocusToNextControl((TControl*) _pTreeAEditer) ;
        }
      }
      break ;
    }
    default	:
    	TEdit::EvKeyUp(key, repeatcount, flags) ;
	}
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
uint
NSEditDico::EvGetDlgCode(MSG far* /* msg */)
{
	uint retVal = (uint)DefaultProcessing() ;
  // retVal |= DLGC_WANTALLKEYS ;
	return retVal ;
}

//--------------------------------------------------------------------
//différents traitements sur les textes
//--------------------------------------------------------------------
void
NSEditDico::CmEditCut()
{
	TEdit::CmEditCut() ;
  UpdateDico() ;

  addToUndoBuffer() ;
}

void
NSEditDico::CmEditCopy()
{
	TEdit::CmEditCopy() ;
}

void
NSEditDico::CmEditPaste()
{
	TEdit::CmEditPaste() ;
  UpdateDico() ;

  addToUndoBuffer() ;
}

void
NSEditDico::CmEditDelete()
{
	TEdit::CmEditDelete() ;
  UpdateDico() ;

  addToUndoBuffer() ;
}

void
NSEditDico::CmEditClear()
{
	TEdit::CmEditClear() ;

  addToUndoBuffer() ;
}

void
NSEditDico::CmEditUndo()
{
	undoOneStep() ;
}

void
NSEditDico::CmEditSelectAll()
{
  int iTextSize = GetTextLen() ;
  if (iTextSize < 1)
    return ;

  SetSelection(0, iTextSize) ;

  addToUndoBuffer() ;
}

//-----------------------------------------------------------
//curseur de la souris
//-----------------------------------------------------------
void
NSEditDico::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TEdit::EvLButtonDown(modKeys, point) ;
}

//-----------------------------------------------------------
//curseur de la souris
//-----------------------------------------------------------
void
NSEditDico::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TEdit::EvLButtonUp(modKeys, point) ;
}

void
NSEditDico::EvRButtonDown(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
try
{
  NS_CLASSLIB::TPoint lp = point ;

  string sCut    = string("Cut") ;
  string sCopy   = string("Copy") ;
  string sPaste  = string("Paste") ;
  string sDelete = string("Delete") ;
  string sUndo   = string("Undo") ;
  string sRedo   = string("Redo") ;
  string sEmphas = string("Emphasized") ;
  string sStanda = string("Standard") ;
  if (_pDico)
  {
    NSSuper* pSuper = pContexte->getSuperviseur() ;
    sCut    = pSuper->getText("treeViewManagement", "cut") ;
    sCopy   = pSuper->getText("treeViewManagement", "copy") ;
    sPaste  = pSuper->getText("treeViewManagement", "paste") ;
    sDelete = pSuper->getText("treeViewManagement", "delete") ;
    sUndo   = pSuper->getText("treeViewManagement", "undo") ;
    sRedo   = pSuper->getText("treeViewManagement", "redo") ;
    sEmphas = pSuper->getText("treeViewManagement", "emphasized") ;
    sStanda = pSuper->getText("treeViewManagement", "standard") ;
  }

  uint startSel, endSel ;
  GetSelection(startSel, endSel) ;

  OWL::TPopupMenu *menu = new OWL::TPopupMenu() ;

  menu->AppendMenu(MF_STRING, CM_EDITCUT, sCut.c_str()) ;
  menu->AppendMenu(MF_STRING, CM_EDITCOPY, sCopy.c_str()) ;
  menu->AppendMenu(MF_STRING, CM_EDITPASTE, sPaste.c_str()) ;
  menu->AppendMenu(MF_STRING, CM_EDITDELETE, sDelete.c_str()) ;
  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
  menu->AppendMenu(MF_STRING, CM_EMPHASIZED, sEmphas.c_str()) ;
  menu->AppendMenu(MF_STRING, CM_STANDARD, sStanda.c_str()) ;
  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
  menu->AppendMenu(MF_STRING, CM_ANNULER, sUndo.c_str()) ;
  menu->AppendMenu(MF_STRING, CM_EDITREDO, sRedo.c_str()) ;

  ClientToScreen(lp) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  delete menu ;
}
catch (...)
{
  erreur("Exception NSEditDico::EvRButtonDown.", standardError, 0) ;
}
}

//-------THandle-------------------------------------------------------------
// le champ edit perd le focus
//--------------------------------------------------------------------
void
NSEditDico::EvKillFocus(THandle hWndGetFocus)
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  char szThis[20] ;
  sprintf(szThis, "%p", HWindow) ;
  string sMsgHeader = string("EditDico ") + string(szThis) + string(" EvKillFocus: ") ;

  char szFocus[20] ;
  sprintf(szFocus, "%p", (HWND) hWndGetFocus) ;
  string sMsg = sMsgHeader + string("Entering (focus gained by ") + string(szFocus) + string(")") ;

  if (_bGardeFocus)
    sMsg += string(" garde focus = true") ;
  else
    sMsg += string(" garde focus = false") ;

  pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  if (_bBeingDestroyed)
  {
    sMsg = sMsgHeader + string("Doing nothing because being destroyed.") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // Must accept loosing focus without condition
  //
  if (_bYouMustAcceptToLooseFocus)
  {
    _bYouMustAcceptToLooseFocus = false ;

    sMsg = sMsgHeader + string("Accepting the KillFocus (forced).") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    TEdit::EvKillFocus(hWndGetFocus) ;

    return ;
  }

    //
	//	Quand un contrôle est activé il reçoit :
	//								WM_SETFOCUS
	//								WM_KILLFOCUS
	//                      WM_PAINT
	//								WM_SETFOCUS
  // il faut ignorer les killFocus tant que ce contrôle n'a pas reçu WM_PAINT
  //
	if (false == _bPaint)
  {
    sMsg = sMsgHeader + string("Leaving (not painted yet).") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
		return ;
  }

  // Force keeping focus
  //
	if (_bGardeFocus)
	{
  	SetFocus() ;
    // _bGardeFocus = false ;

    sMsg = sMsgHeader + string("Keeping focus (and leaving).") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    return ;
	}

  // This is gaining focus; nothing to do
  //
	if (hWndGetFocus == HWindow)
  {
    sMsg = sMsgHeader + string("Gaining focus thyself, leaving.") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
		return ;
  }

  if (_pTreeAEditer)
  {
    // If the treewindow is not in "editing mode", it is useless to keep the focus
    //
    if (false == _pTreeAEditer->bEditionDico)
    {
      TWindow focusWnd(hWndGetFocus) ;
      string sTitle = string(focusWnd.Title) ;
      if (string("") != sTitle)
      {
        sMsg = sMsgHeader + string("Focused windows title: ") + sTitle ;
        pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
      }

      // sMsg = sMsgHeader + string("Leaving (treewindow is not in edit mode).") ;
      sMsg = sMsgHeader + string("Accepting the KillFocus (treewindow is not in edit mode).") ;
      pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

      TEdit::EvKillFocus(hWndGetFocus) ;

      if (_pDico->getDicoDialog())
      {
        char szDico[20] ;
        sprintf(szDico, "%p", _pDico->getDicoDialog()->HWindow) ;
        sMsg = sMsgHeader + string("Hidding Dico ") + string(szDico) ;
        pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

        _pDico->getDicoDialog()->cacherDialogue() ;
      }

      //
      // An asynchronous call must be done there, because this objet
      // will be killed by the tree
      //
      if (false == _bKillSent)
      {
        char szTree[20] ;
        sprintf(szTree, "%p", _pTreeAEditer->HWindow) ;
        sMsg = sMsgHeader + string("Sending DicoLostFocus to tree ") + string(szTree) ;
        pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

        _pTreeAEditer->PostMessage(WM_COMMAND, IDC_DICO_LOST_FOCUS, (WPARAM) HWindow) ;
      // pSuper->getApplication()->PumpWaitingMessages() ;
        _bKillSent = true ;
      }

      // The treewindow will also receive a KillFocus ; we tell it that it has
      // to kill this NSEditDico

		  return ;
    }

    // If the treewindow got the focus while in edit mode, we keep it
    //
    if (hWndGetFocus == _pTreeAEditer->HWindow)
    {
      sMsg = sMsgHeader + string("Focus gained by TreeWindow, leaving.") ;
      pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
		  return ;
    }

    NSControle* pTreeControl = _pTreeAEditer->getControl() ;
    if (pTreeControl)
    {
      NSDialog* pDialog = pTreeControl->getNSDialog() ;
      if (pDialog)
      {
        // If the dialog box got the focus, we keep it
        //
        if (hWndGetFocus == pDialog->HWindow)
        {
          sMsg = sMsgHeader + string("Focus gained by NSDialog, leaving.") ;
          pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
		      return ;
        }
      }

      NSBBMUEView* pMueView = pTreeControl->getMUEView() ;
      if (pMueView)
      {
        // If the Mue View got the focus, we keep it
        //
        if (hWndGetFocus == pMueView->HWindow)
        {
          sMsg = sMsgHeader + string("Focus gained by MueView, leaving.") ;
          pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
		      return ;
        }

        // Check Pane Splitter's parent
        //
        NSPaneSplitter* pPaneSplitter = pMueView->getPaneSplitter() ;
        if (pPaneSplitter && pPaneSplitter->Parent &&
                            (hWndGetFocus == pPaneSplitter->Parent->HWindow))
        {
          sMsg = sMsgHeader + string("Focus gained by PaneSplitter's parent, leaving.") ;
          pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
		      return ;
        }
      }
    }
  }

  // Focus gained by DicoDialog or child, leaving
  //
  if (_pDico->getDicoDialog() &&
                ((hWndGetFocus == _pDico->getDicoDialog()->HWindow) ||
                  _pDico->getDicoDialog()->IsChild(hWndGetFocus)))
  {
    sMsg = sMsgHeader + string("Focus gained by DicoDialog or child, leaving.") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // Trying to guess who gained focus
  //
  TWindow focusWnd(hWndGetFocus) ;

  char szClassName[255] ;
  focusWnd.GetClassName(szClassName, sizeof(szClassName)) ;

/*
  if ('\0' != szClassName[0])
  {
    // Focus gained by the MDIClient space, nothing to do
    //
    if (string("MDIClient") == string(szClassName))
    {
      sMsg = sMsgHeader + string("Focus gained by MDIClient, leaving.") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
      return ;
    }
  }
*/

  // Accepting to lose focus and hidding DicoDialog
  //
  string sTitle = string(focusWnd.Title) ;
  if (string("") != sTitle)
    sMsg = sMsgHeader + string("Focused windows title: ") + sTitle ;
  else if ('\0' != szClassName[0])
    sMsg = sMsgHeader + string("Focused windows ClassName: ") + string(szClassName) ;
  pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  if (_pDico->getDicoDialog())
  {
    char szDico[20] ;
    sprintf(szDico, "%p", _pDico->getDicoDialog()->HWindow) ;
    sMsg = sMsgHeader + string("Hidding Dico ") + string(szDico) ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    _pDico->getDicoDialog()->cacherDialogue() ;
  }

  sMsg = sMsgHeader + string("Accepting the KillFocus.") ;
  pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  NSEditDicoGlobal::EvKillFocus(hWndGetFocus) ;

  if (_pTreeAEditer)
  {
    char szTree[20] ;
    sprintf(szTree, "%p", _pTreeAEditer->HWindow) ;
    sMsg = sMsgHeader + string("Sending DicoLostFocus to tree ") + string(szTree) ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
    //
    // An asynchronous call must be done there, because this objet
    // will be killed by the tree
    //
    _pTreeAEditer->PostMessage(WM_COMMAND, IDC_DICO_LOST_FOCUS, (WPARAM) HWindow) ;
  }

  sMsg = sMsgHeader + string("Leaving.") ;
  pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
}

//------------------------------------------------------------------
//redimensionnement du champ édit
//------------------------------------------------------------------
void
NSEditDico::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& rect)
{
	TWindow::Paint(dc, erase, rect) ;
	_bPaint = true ;
}

//------------------------------------------------------------------
//redimensionnement du champ édit
//------------------------------------------------------------------
void
NSEditDico::EvPaint()
{
	//TWindow::EvPaint();
	DefaultProcessing() ;
	_bPaint = true ;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
void
NSEditDico::EvSetFocus(HWND hWndLostFocus)
{
  char szThis[20] ;
  sprintf(szThis, "%p", HWindow) ;
  string sMsg = string("EditDico ") + string(szThis) + string(" EvSetFocus: ") ;

  char szFocus[20] ;
  sprintf(szFocus, "%p", (HWND) hWndLostFocus) ;
  sMsg += string(" (focus lost by ") + string(szFocus) + string(")") ;

  if (hWndLostFocus == HWindow)
  {
    sMsg += string(" doing nothing ") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
    return ;
  }

  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	NSEditDicoGlobal::EvSetFocus(hWndLostFocus) ;

	if (_bPaint && (false == _bTexteLibre) && (false == isEmpty()))
	{
    bool bPreviousGardeFocus = _bGardeFocus ;
  	_bGardeFocus = true ; // car pDico->pDicoDialog->ShowWindow(SW_SHOW) fait perdre le focus à this
    _pDico->getDicoDialog()->montrerDialogue() ;
    _bGardeFocus = bPreviousGardeFocus ;
	}

  // It is necessary to do that because the NSTreeWindow object doesn't
  // receive the EvSetFocus when NSEditDico gets it
  //
  if (_pTreeAEditer)
    _pTreeAEditer->setLoosingFocus(false) ;
}

void
NSEditDico::EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	// SetCursor(NULL, IDC_IBEAM);
  TEdit::EvMouseMove(modKeys, point) ;
}

void
NSEditDico::switchFromFreeToLexique(string* pContent)
{
	if ((string*) NULL == pContent)
		return ;

	TIC* pIc = new TIC("DISPLAY", 0, 0) ;
  //
  //redonner les dimensions pour un code lexique : prendre la taille du texte tapé  + ...
  //
  NS_CLASSLIB::TRect rectEdition = GetWindowRect() ;

  NS_CLASSLIB::TSize size = pIc->GetTextExtent(pContent->c_str(), strlen(pContent->c_str())) ;
  delete pIc ;

  int longueurChaine = size.X() + 5 ;
  int hauteur        = size.Y() + 5 ;

  rectEdition.right  = rectEdition.left + 268 ;
  rectEdition.bottom = rectEdition.top  + 16 ;

  if (rectEdition.right < longueurChaine)
    rectEdition.right = rectEdition.left + longueurChaine + 5 ;

  if (rectEdition.bottom < hauteur)
    rectEdition.bottom = rectEdition.top  + hauteur + 5 ;

  NS_CLASSLIB::TPoint point(rectEdition.left, rectEdition.top) ;
  NS_CLASSLIB::TPoint point2(rectEdition.right, rectEdition.bottom) ;
  Parent->ScreenToClient(point) ;
  Parent->ScreenToClient(point2) ;

  // on reporte la modif du rectangle dans rectEditDico
  // pour pouvoir gérer les changements de curseur
  // (voir NSTreeWindow::EvMouseMove)
  // Note : ne pas oublier de prendre les coordonnées Client
  if (_pTreeAEditer)
    _pTreeAEditer->rectEditDico = NS_CLASSLIB::TRect(point, point2) ;

  //
  // Positionnement du contrôle Edit
  //
  SetWindowPos(0, point.X(), point.Y(),
                     rectEdition.right - rectEdition.left,
                     rectEdition.bottom - rectEdition.top ,SWP_NOZORDER) ;
  //
  // Supprimer la scrollbar verticale
  //
  ShowScrollBar(SB_VERT, FALSE) ;

  _sAmmorceLibre = string("") ;
}

void
NSEditDico::setBold()
{
  string sBeforeSelection = string("") ;
  string sSelection       = string("") ;
  string sAfterSelection  = string("") ;

  bool bGotBlocks = getThreeBlocks(sBeforeSelection, sSelection, sAfterSelection) ;
  if (false == bGotBlocks)
    return ;

  // Is there a bold opening markup before the selection?
  //
  bool isBeginingBefore = false ;
  if (string("") != sBeforeSelection)
  {
    size_t iLastStart = sBeforeSelection.find_last_of(START_BOLD) ;
    size_t iLastEnd   = sBeforeSelection.find_last_of(END_BOLD) ;

    if (iLastStart > iLastEnd)
      isBeginingBefore = true ;
  }

  // Is there a bold closing markup after the selection?
  //
  bool isEndigAfter = false ;
  if (string("") != sAfterSelection)
  {
    size_t iFirstStart = sAfterSelection.find(START_BOLD) ;
    size_t iFirstEnd   = sAfterSelection.find(END_BOLD) ;

    if (iFirstEnd < iFirstStart)
      isEndigAfter = true ;
  }

  // Remove all the opening or closing markups inside the selection
  //
  size_t iStartMark = sSelection.find(START_BOLD) ;
  size_t iEndMark   = sSelection.find(END_BOLD) ;
  while ((NPOS != iStartMark) || (NPOS != iEndMark))
  {
    size_t iMark = min(iStartMark, iEndMark) ;
    int    iMarkSize = strlen(START_BOLD) ;
    if (iMark == iEndMark)
      iMarkSize = strlen(END_BOLD) ;

    string sNewSelection = string("") ;
    if (iMark > 0)
      sNewSelection = string(sSelection, 0, iMark) ;
    if (iMark < strlen(sSelection.c_str()) - iMarkSize)
      sNewSelection += string(sSelection, iMark + iMarkSize, strlen(sSelection.c_str()) - iMark - iMarkSize) ;

    sSelection = sNewSelection ;

    iStartMark = sSelection.find(START_BOLD) ;
    iEndMark   = sSelection.find(END_BOLD) ;
  }

  if (false == isBeginingBefore)
    sBeforeSelection += START_BOLD ;

  if (false == isEndigAfter)
    sAfterSelection = END_BOLD + sAfterSelection ;

  addToUndoBuffer() ;

  setThreeBlocks(sBeforeSelection, sSelection, sAfterSelection) ;
}

void
NSEditDico::setRegular()
{
  string sBeforeSelection = string("") ;
  string sSelection       = string("") ;
  string sAfterSelection  = string("") ;

  bool bGotBlocks = getThreeBlocks(sBeforeSelection, sSelection, sAfterSelection) ;
  if (false == bGotBlocks)
    return ;

  // Is there a bold opening markup before the selection?
  //
  bool isBeginingBefore = false ;
  if (string("") != sBeforeSelection)
  {
    size_t iLastStart = sBeforeSelection.find_last_of(START_BOLD) ;
    size_t iLastEnd   = sBeforeSelection.find_last_of(END_BOLD) ;

    if (iLastStart > iLastEnd)
      isBeginingBefore = true ;
  }

  // Is there a bold closing markup after the selection?
  //
  bool isEndigAfter = false ;
  if (string("") != sAfterSelection)
  {
    size_t iFirstStart = sAfterSelection.find(START_BOLD) ;
    size_t iFirstEnd   = sAfterSelection.find(END_BOLD) ;

    if (iFirstEnd < iFirstStart)
      isEndigAfter = true ;
  }

  // Remove all the opening or closing markups inside the selection
  //
  size_t iStartMark = sSelection.find(START_BOLD) ;
  size_t iEndMark   = sSelection.find(END_BOLD) ;
  while ((NPOS != iStartMark) || (NPOS != iEndMark))
  {
    size_t iMark = min(iStartMark, iEndMark) ;
    int    iMarkSize = strlen(START_BOLD) ;
    if (iMark == iEndMark)
      iMarkSize = strlen(END_BOLD) ;

    string sNewSelection = string("") ;
    if (iMark > 0)
      sNewSelection = string(sSelection, 0, iMark) ;
    if (iMark < strlen(sSelection.c_str()) - iMarkSize)
      sNewSelection += string(sSelection, iMark + iMarkSize, strlen(sSelection.c_str()) - iMark - iMarkSize) ;

    sSelection = sNewSelection ;

    iStartMark = sSelection.find(START_BOLD) ;
    iEndMark   = sSelection.find(END_BOLD) ;
  }

  if (true == isBeginingBefore)
    sBeforeSelection += START_BOLD ;

  if (true == isEndigAfter)
    sAfterSelection = END_BOLD + sAfterSelection ;

  addToUndoBuffer() ;

  setThreeBlocks(sBeforeSelection, sSelection, sAfterSelection) ;
}

void
NSEditDico::addToUndoBuffer()
{
  Invalidate() ;

  uint iStarSel   = 0 ;
  uint iEndSel    = 0 ;
  string sContent = string("") ;

  int oldBuffLen = GetTextLen() ;
  if (oldBuffLen > 0)
  {
    GetSelection(iStarSel, iEndSel) ;
	  char far* oldBuff = new char[oldBuffLen + 1] ;
	  GetText(oldBuff, oldBuffLen+1) ;
    sContent = string(oldBuff) ;
    delete[] oldBuff ;
  }

  // expand the buffer
  //
  if (_undoBuffer.empty() || (_undoBuffer.end() == _undoCursor))
  {
    _undoBuffer.push_back(new NSEditStep(sContent, iStarSel, iEndSel)) ;
    _undoCursor = _undoBuffer.end() ;
  }
  // replace a previous record
  //
  else
  {
    (*_undoCursor)->replaceStep(sContent, iStarSel, iEndSel) ;
    _undoCursor++ ;
  }
}

void
NSEditDico::undoOneStep()
{
  if (_undoBuffer.empty() || (_undoBuffer.begin() == _undoCursor))
    return ;

  // If the cursor is at end, it means that current state is at back, and
  // we have to go back 2 times to get the previous state
  //
  if (_undoBuffer.end() == _undoCursor)
  {
    _undoCursor-- ;
    if (_undoBuffer.begin() == _undoCursor)
    {
      SetText("") ;
      SetSelection(0, 0) ;
    }
  }

  _undoCursor-- ;

  string sContent = (*_undoCursor)->getContent() ;
  SetText(sContent.c_str()) ;
  SetSelection((*_undoCursor)->getMinSel(), (*_undoCursor)->getMaxSel()) ;
}

void
NSEditDico::redoOneStep()
{
  if (_undoBuffer.end() == _undoCursor)
    return ;

  _undoCursor++ ;

  if (_undoBuffer.end() == _undoCursor)
    return ;

  string sContent = (*_undoCursor)->getContent() ;
  SetText(sContent.c_str()) ;
  SetSelection((*_undoCursor)->getMinSel(), (*_undoCursor)->getMaxSel()) ;
}

bool
NSEditDico::getThreeBlocks(string& sBeforeSelection, string& sSelection, string& sAfterSelection)
{
  sBeforeSelection = string("") ;
  sSelection       = string("") ;
  sAfterSelection  = string("") ;

  int oldBuffLen = GetTextLen() ;
  if (oldBuffLen <= 0)
    return false ;

  uint iStarSel ;
  uint iEndSel ;
  GetSelection(iStarSel, iEndSel) ;
  if (iStarSel == iEndSel)
    return false ;

  char far* oldBuff = new char[oldBuffLen + 1] ;
  GetText(oldBuff, oldBuffLen+1) ;
  string sContent = string(oldBuff) ;

  if (iStarSel > 0)
    sBeforeSelection = string(sContent, 0, iStarSel) ;

  if (iEndSel < strlen(sContent.c_str()) - 1)
    sAfterSelection = string(sContent, iEndSel, strlen(sContent.c_str()) - iEndSel) ;

  if (iEndSel > iStarSel)
    sSelection = string(sContent, iStarSel, iEndSel - iStarSel) ;

  return true ;
}

void
NSEditDico::setThreeBlocks(string sBeforeSelection, string sSelection, string sAfterSelection)
{
  string sContent = sBeforeSelection + sSelection + sAfterSelection ;
  SetText(sContent.c_str()) ;

  int iStartSel = strlen(sBeforeSelection.c_str()) ;
  int iEndSel   = iStartSel ;
  if (string("") != sSelection)
    iEndSel += strlen(sSelection.c_str()) ;

  SetSelection(iStartSel, iEndSel) ;
}

NSControle*
NSEditDico::getControl()
{
  if ((NSTreeWindow*) NULL == _pTreeAEditer)
    return (NSDialog*) 0 ;

  return _pTreeAEditer->getControl() ;
}

NSDialog*
NSEditDico::getNSDialog()
{
  NSControle* pTreeControl = getControl() ;
  if ((NSControle*) NULL == pTreeControl)
    return (NSDialog*) 0 ;

  return pTreeControl->getNSDialog() ;
}

NSBBMUEView*
NSEditDico::getMUEView()
{
  NSControle* pTreeControl = getControl() ;
  if ((NSControle*) NULL == pTreeControl)
    return (NSDialog*) 0 ;

  return pTreeControl->getMUEView() ;
}

// -------------------------------------------------------------------------
//                               NSEditStep
// -------------------------------------------------------------------------

NSEditStep::NSEditStep(string sContent, int iMinSel, int iMaxSel)
{
  _iMinSel  = iMinSel ;
  _iMaxSel  = iMaxSel ;
  _sContent = sContent ;
}

NSEditStep::~NSEditStep()
{
}

NSEditStep::NSEditStep(const NSEditStep& rv)
{
  _iMinSel  = rv._iMinSel ;
  _iMaxSel  = rv._iMaxSel ;
  _sContent = rv._sContent ;
}

NSEditStep&
NSEditStep::operator=(const NSEditStep& src)
{
  if (&src == this)
    return *this ;

  _iMinSel  = src._iMinSel ;
  _iMaxSel  = src._iMaxSel ;
  _sContent = src._sContent ;

  return *this ;
}

void
NSEditStep::replaceStep(string sContent, int iMinSel, int iMaxSel)
{
  _iMinSel  = iMinSel ;
  _iMaxSel  = iMaxSel ;
  _sContent = sContent ;
}
