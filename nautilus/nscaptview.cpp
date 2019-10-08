// -----------------------------------------------------------------------------// nscaptview.cpp
// -----------------------------------------------------------------------------
// Document/Vues Texte capturé
// -----------------------------------------------------------------------------
// $Revision: 1.6 $
// $Author: pameline $
// $Date: 2015/02/07 11:34:44 $
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2005
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------


#if !defined(OWL_LISTWIND_H)
# include <owl/listwind.h>
#endif

#include <owl/uihelper.h>

#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>
#include <fstream.h>

#include <owl/button.h>

#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nautilus\ns_html.h"
#include "nautilus\nscompub.h"		// pour les formules
#include "nautilus\nsresour.h"      // pour AutoWordDialog
#include "nautilus\nsvisual.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nscsdoc.h"

#include "nautilus\nsepicap.h"
#include "nautilus\nssoapview.h"
#include "nautilus\nscaptview.h"
// #include "nautilus\nscr_ana.h"
#include "nsldv\nsldvuti.h"
#include "nsbb\nspanesplitter.h"
#include "nautilus\nsdocview.h"

#include "nautilus\nsSOAP.rh"

// -----------------------------------------------------------------------------
//
// METHODES DE NSCAPT Document / Vue
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//
// Class NSCaptDocument
//
// -----------------------------------------------------------------------------

/*
// -----------------------------------------------------------------------------
// Constructeur
// -----------------------------------------------------------------------------
NSCaptDocument::NSCaptDocument(TDocument* parent, NSContexte* pCtx, string sTxt, string sCaptPath)
  : TDocument(parent),
    NSRoot(pCtx)
{
try
{
	string sTitle = pContexte->getSuperviseur()->getText("windowsTitles", "RawCapture") ;
	SetTitle(sTitle.c_str()) ;

	pEpisodus = pContexte->getSuperviseur()->getEpisodus() ;

	if (sTxt != "")
		newCapture(sTxt, sCaptPath) ;
}
catch (...)
{
	erreur("Exception NSCaptDocument ctor.", standardError, 0) ;
}
}


// -----------------------------------------------------------------------------
// constructeur
// -----------------------------------------------------------------------------
NSCaptDocument::NSCaptDocument(TDocument *parent, NSContexte *pCtx, NSOCRbloc* pOcrBloc, string sCaptPath)
  : TDocument(parent),
    NSRoot(pCtx)
{
try
{
  string sTitle = pContexte->getSuperviseur()->getText("windowsTitles", "RawCapture") ;
  SetTitle(sTitle.c_str()) ;

  pEpisodus = pContexte->getSuperviseur()->getEpisodus() ;

  if (pOcrBloc)
    newCapture(pOcrBloc, sCaptPath) ;
}
catch (...)
{
  erreur("Exception NSCaptDocument ctor.", standardError, 0) ;
}
}
*/

// -----------------------------------------------------------------------------
// constructeur
// -----------------------------------------------------------------------------
NSCaptDocument::NSCaptDocument(TDocument *parent, NSContexte *pCtx)
               :TDocument(parent),
                NSRoot(pCtx)
{
try
{
  string sTitle = pContexte->getSuperviseur()->getText("windowsTitles", "RawCapture") ;
  SetTitle(sTitle.c_str()) ;

  pEpisodus = pContexte->getEpisodus() ;
}
catch (...)
{
  erreur("Exception NSCaptDocument ctor.", standardError, 0) ;
}
}

void
NSCaptDocument::PatChanged()
{
	aCaptures.vider() ;
}

void
NSCaptDocument::newCapture(NSCaptureArray* pCaptureArray, string sTxt, string sCaptPath, string sClassif, string sUnite)
{
try
{
  if (sTxt == "")
    return ;

  string sTexte = sTxt ;

/* avant _MUE
  // Remplace les \r\n par des \r\r\n : uniquement utile pour envoyer dans un edit
  size_t posCr = sTexte.find("\r\n") ;
  while (posCr != string::npos)
  {
    if ((posCr > 0) && (sTexte[posCr-1] != '\n'))
      sTexte = string(sTexte, 0, posCr) + string(1, '\r') + string(sTexte, posCr, strlen(sTexte.c_str()) - posCr) ;
    posCr = sTexte.find("\r\n", posCr + 2) ;
  }
*/

// Traitement des retours chariot
	size_t iCR = sTexte.find('\n') ;
	while (string::npos != iCR)
	{
		string sSubText = string(sTexte, 0, iCR) ;
		strip(sSubText, stripBoth, '\r') ;
		strip(sSubText, stripBoth) ;
		sTexte = string(sTexte, iCR+1, strlen(sTexte.c_str())-iCR-1) ;
		strip(sTexte, stripBoth, '\r') ;
		strip(sTexte, stripBoth) ;

		if (string("") != sSubText)
		{
    	// Capture inside newCaptureArray : we don't yet use it
      //
    	if (pCaptureArray == &(pEpisodus->newCaptureArray))
			{
				NSCapture* pCapt = new NSCapture(pContexte,
                                        sCaptPath,
                                        sSubText,
                                        FromMailBox,
                                        sClassif,
                                        sUnite) ;
        pCaptureArray->ajouter(pCapt);
			}
      // Capture outside newCaptureArray : we analyse it
      //
			else
			{
      	analysedCapture* pAnaCap = new analysedCapture(pContexte, sSubText, pCaptureArray, FromMailBox, sCaptPath, sClassif, sUnite) ;
    		aCaptures.push_back(pAnaCap) ;

        analysedCapture* pSecondary = (analysedCapture*) 0 ;
        NSCapture* pCapt = pAnaCap->analyse(&pSecondary) ;

        processNewCapture(pCapt, pAnaCap, pSecondary) ;

      	refreshViewsNewLigne() ;
			}
		}

		iCR = sTexte.find('\n') ;
	}

	if (string("") != sTexte)
	{
		if (pCaptureArray == &(pEpisodus->newCaptureArray))
		{
			NSCapture* pCapt = new NSCapture(pContexte,
                                        sCaptPath,
                                        sTexte,
                                        FromMailBox,
                                        sClassif,
                                        sUnite) ;
			pCaptureArray->ajouter(pCapt);
		}
    else
		{
    	analysedCapture* pAnaCap = new analysedCapture(pContexte, sTexte, pCaptureArray, FromMailBox, sCaptPath, sClassif, sUnite) ;
			aCaptures.push_back(pAnaCap) ;

      analysedCapture* pSecondary = NULL ;
      NSCapture* pCapt = pAnaCap->analyse(&pSecondary) ;

      processNewCapture(pCapt, pAnaCap, pSecondary) ;

    	refreshViewsNewLigne() ;
		}
	}
}
catch (...)
{
	erreur("Exception NSCaptDocument::newCapture (from text).", standardError, 0) ;
}
}

void
NSCaptDocument::newCapture(NSCaptureArray* pCaptureArray, NSOCRbloc* pBloc, string sCaptPath, string sClassif, string sUnite)
{
try
{
  if (((NSOCRbloc*) NULL == pBloc) || (string("") == pBloc->sTexte))
    return ;

	analysedCapture* pAnaCap = new analysedCapture(pContexte, pBloc, pCaptureArray, FromMailBox, sCaptPath, sClassif, sUnite) ;
  aCaptures.push_back(pAnaCap) ;

  analysedCapture* pSecondary = (analysedCapture*) 0 ;
  NSCapture* pCapt = pAnaCap->analyse(&pSecondary) ;

  processNewCapture(pCapt, pAnaCap, pSecondary) ;

	refreshViewsNewLigne() ;
}
catch (...)
{
	erreur("Exception NSCaptDocument::newCapture (from OCR).", standardError, 0) ;
}
}

void
NSCaptDocument::newCapture(NSCaptureArray* pCaptArray, NSCapture* pCapture)
{
try
{
	analysedCapture* pAnaCap = new analysedCapture(pContexte, pCaptArray, pCapture) ;
	aCaptures.push_back(pAnaCap) ;

  analysedCapture* pSecondary = (analysedCapture*) 0 ;
  NSCapture* pCapt = pAnaCap->analyse(&pSecondary) ;

  processNewCapture(pCapt, pAnaCap, pSecondary) ;

	refreshViewsNewLigne() ;
}
catch (...)
{
	erreur("Exception NSCaptDocument::newCapture (from capture).", standardError, 0) ;
}
}

void
NSCaptDocument::processNewCapture(NSCapture* pCapt, analysedCapture* pPrimeAnalizedCapt, analysedCapture* pSecAnalizedCapt)
{
	if (pCapt)
	{
		if (pCapt->getClassifier() != string(""))
			pContexte->getEpisodus()->addToSOAP(pCapt) ;
	}

  if (pSecAnalizedCapt)
  	aCaptures.push_back(pSecAnalizedCapt) ;
}

string
NSCaptDocument::getText(size_t iIndice)
{
  if ((aCaptures.empty()) || (iIndice > aCaptures.size()))
    return string("") ;

  return (aCaptures[iIndice])->getText() ;
}

void
NSCaptDocument::refreshViewsNewLigne()
{
	if (aCaptures.empty())
		return ;

	bool bWindowFound = false ;

	TView * pView = GetViewList() ;
  if (pView)
  {
    do
    {
      NSMultiCaptView * pMultiView = TYPESAFE_DOWNCAST(pView, NSMultiCaptView) ;
      if (pMultiView)
      {
        pMultiView->addedLines() ;
        bWindowFound = true ;
      }
      pView = NextView(pView) ;
    }
    while (pView) ;
	}

  if (false == bWindowFound)
  	openView() ;

	for (AnaCaptIter cptIt = aCaptures.begin() ; aCaptures.end() != cptIt ; cptIt++)
		(*cptIt)->setDisplayed(true) ;
}

void
NSCaptDocument::openView()
{
	TView * pView = GetViewList() ;
  if (pView)
  {
    do
    {
      NSMultiCaptView * pMultiView = TYPESAFE_DOWNCAST(pView, NSMultiCaptView) ;
      if (pMultiView)
        return ;
      pView = NextView(pView) ;
    }
    while (pView) ;
	}

  if (aCaptures.empty())
		return ;

	// We only create if aCaptures is not empty or limited to first and second name
  //
	int iElementsFound  = 0 ;
  int iArrayTotalSize = 0 ;
  for (AnaCaptIter cptIt = aCaptures.begin() ; cptIt != aCaptures.end() ; cptIt++)
  {
  	NSCaptureArray* pCaptArray = (*cptIt)->getCaptureArray() ;

  	if (pCaptArray && (false == pCaptArray->empty()))
    {
    	iArrayTotalSize += pCaptArray->size() ;

  		string sResult ;
  		if (string::npos != UINT(pCaptArray->trouveChemin(string("LNOM01"), &sResult)))
    		iElementsFound++ ;
    	if (string::npos != UINT(pCaptArray->trouveChemin(string("LNOM21"), &sResult)))
    		iElementsFound++ ;
    	if (string::npos != UINT(pCaptArray->trouveChemin(string("LCODO1"), &sResult)))
    		iElementsFound++ ;
    }
  }

	if (iElementsFound == iArrayTotalSize)
		return ;

	NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(this, "Multi Capt Management") ;
}

NSCaptDocument::~NSCaptDocument(){
	// if (pEpisodus && (pEpisodus->pCaptureDoc) == this)
	//	pEpisodus->pCaptureDoc = NULL ;
}


// -----------------------------------------------------------------------------
//
// Class NSCaptView
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Table de réponses de la classe NSCaptView
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE2(NSCaptView, TView, TEdit)
  EV_VN_ISWINDOW,
  EV_WM_RBUTTONDOWN,
  //EV_LVN_GETDISPINFO(IDSOAPWindow, DispInfoSOAP),
  EV_WM_SIZE,
  EV_WM_SETFOCUS,
  EV_WM_MOUSEMOVE,
  EV_WM_DESTROY,
  // EV_COMMAND(CM_EPISO_TRANSMIT, transmit),
  // EV_COMMAND(IDC_EPISOD_CLASSIF, EpisodSetClassif),
  EV_COMMAND(IDC_PRINCIPE1,   EvPosit1),
  EV_COMMAND(IDC_PRINCIPE2,   EvPosit2),  EV_COMMAND(IDC_PRINCIPE3,   EvPosit3),
  EV_COMMAND(IDC_PRINCIPE4,   EvPosit4),
  EV_COMMAND(IDC_PRINCIPE5,   EvPosit5),
  EV_COMMAND(IDC_PRINCIPE6,   EvPosit6),
  EV_COMMAND(IDC_PRINCIPE7,   EvPosit7),
  EV_COMMAND(IDC_PRINCIPE8,   EvPosit8),
  EV_COMMAND(IDC_PRINCIPE9,   EvPosit9),
  EV_COMMAND(IDC_CODE,        EvCode),
END_RESPONSE_TABLE ;


// -----------------------------------------------------------------------------
// Constructeur
// -----------------------------------------------------------------------------
NSCaptView::NSCaptView(NSCaptDocument &doc, TWindow *parent)
           :TView(doc), TEdit(parent, GetNextViewId(), "", 0, 0, 0, 0, 0, true)
{
  TEdit::FormatLines(false) ;
  pDoc = &doc ;
  bSetupToolBar = true ;
}

// -----------------------------------------------------------------------------
// Destructeur
// -----------------------------------------------------------------------------
NSCaptView::~NSCaptView()
{
}

// -----------------------------------------------------------------------------
// GetWindow renvoie this (à redéfinir car virtual dans TView)
// -----------------------------------------------------------------------------
TWindow *
NSCaptView::GetWindow()
{
  return ((TWindow*)this) ;
}

// -----------------------------------------------------------------------------
// Appel de la fonction de remplissage de la vue
// -----------------------------------------------------------------------------
void
NSCaptView::SetupWindow()
{
  SetWindowPosit() ;

  TEdit::SetupWindow() ;
  TEdit::SetText((pDoc->getText(0)).c_str()) ;
}

void
NSCaptView::EvDestroy()
{
  if (pDoc->pContexte->getEpisodus()->bAutoSave)
    RecordWindowPosit() ;
}

void
NSCaptView::SetWindowPosit()
{
  string sPosition = pDoc->pContexte->getEpisodus()->sPosCapture ;

  if (string("") == sPosition)
    return ;

  int     X, Y, W, H ;
  string  sTaille ;
  size_t  debut = 0 ;

  size_t  positVide = sPosition.find("|") ;  // chaîne vide  int     nbCoords = 0 ;

  vector<string> Coordonnees ;  // contient les coordonnées sous forme de string

  while (string::npos != positVide)
  {
    Coordonnees.push_back(string(sPosition, debut, positVide - debut)) ;
    nbCoords++ ;
    debut = positVide + 1 ;
    positVide = sPosition.find("|", debut + 1) ;
  }
  Coordonnees.push_back(string(sPosition, debut, strlen(sPosition.c_str()) - debut)) ;
  nbCoords++ ;

  //récupérer les coordonnées
  vector<string>::iterator i ;
  i = Coordonnees.begin() ;
  X = StringToDouble(*i) ;
  i++ ;
  Y = StringToDouble(*i) ;
  i++ ;
  W  = StringToDouble(*i) ;
  i++ ;
  H = StringToDouble(*i) ;

  // cas en icone ou plein ecran
  if (nbCoords > 4)
  {
    i++;
    sTaille = *i ;
  }
  else
    sTaille = "N" ;

  Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER) ;
  if (sTaille == "I")    Parent->Show(SW_SHOWMINIMIZED) ;
  else if (sTaille == "Z")
    Parent->Show(SW_SHOWMAXIMIZED) ;
  else
    Parent->Show(SW_SHOWNORMAL) ;
}

voidNSCaptView::RecordWindowPosit()
{
  string sPosSoap = pDoc->pContexte->getEpisodus()->sPosCapture ;

  NS_CLASSLIB::TRect rectDlg = Parent->GetWindowRect() ; // coordonnées par % à l'écran
  int X, Y ;
  // int W, H ;
  NS_CLASSLIB::TPoint point(rectDlg.X(), rectDlg.Y()) ;
  Parent->Parent->ScreenToClient(point) ;
  X = point.X() ;
  Y = point.Y() ;
//  W = rectDlg.Width() ;
//  H = rectDlg.Height() ;

  double TamponX = (double)X ;
  double TamponY = (double)Y ;
  double TamponW = (double)rectDlg.Width() ;
  double TamponH = (double)rectDlg.Height() ;
  string sPosition = DoubleToString(&TamponX, 0, 0) + "|" + DoubleToString(&TamponY, 0, 0) + "|" + DoubleToString(&TamponW, 0, 0) + "|" + DoubleToString(&TamponH, 0, 0) ;

  // On regarde si la fenetre est en icone ou en plein ecran
  if ((Parent->IsIconic()) || (Parent->IsZoomed()))
  {
    size_t poslast ;

    sPosition = sPosSoap ;
    poslast = sPosition.find_last_of("|") ;
    // si le '|' n'est pas le dernier caractere
    if ((poslast != string::npos) && (poslast < (strlen(sPosition.c_str()) - 1)))
    {
      // si la fenetre était déja en icone ou en zoom :
      // on reprend l'ancienne taille
      if ((sPosition[poslast + 1] == 'I') || (sPosition[poslast + 1] == 'Z'))
        sPosition = string(sPosition, 0, poslast) ;

      if (Parent->IsIconic())
        sPosition += "|I" ;
      else
        sPosition += "|Z" ;
    }
    else
    {
      erreur("Coordonnée éronnée.", standardError, 0, 0) ;
      // on remet dans ce cas la fenetre a une taille par défaut
      sPosition = "0|0|100|100" ;
    }
  }
  pDoc->pContexte->getEpisodus()->sPosCapture = sPosition ;
}


boolNSCaptView::VnIsWindow(HWND hWnd)
{
  return (HWindow == hWnd) ;
}


// -----------------------------------------------------------------------------
// clic droit de la souris sur un noeud
// -----------------------------------------------------------------------------
void
NSCaptView::EvRButtonDown(uint /*modkeys*/, NS_CLASSLIB::TPoint& point)
{
try
{
  uint iStartPos, iEndPos ;
  TEdit::GetSelection(iStartPos, iEndPos) ;
  if (iStartPos == iEndPos)
    return ;

  char * pResult = new char[iEndPos - iStartPos] ;
  TEdit::GetSubText(pResult, iStartPos, iEndPos) ;
  sSelection = string(pResult) ;
  delete[] pResult ;

  if (!pDoc || !(pDoc->pEpisodus) || !(pDoc->pEpisodus->pPrincipes))
    return ;

  PrinciplesArray * pPrincipes = pDoc->pEpisodus->pPrincipes ;

  if (pPrincipes->empty())
    return ;

  NS_CLASSLIB::TPoint lp = point ;
  TPopupMenu * menu = new TPopupMenu() ;

  PrinciplesIter prIt = pPrincipes->begin() ;
  for (int i = 1 ; prIt != pPrincipes->end() ; prIt++, i++)
  {
    switch (i)
    {
      case 1 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE1, (*prIt)->sLibelle.c_str()) ; break ;
      case 2 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE2, (*prIt)->sLibelle.c_str()) ; break ;
      case 3 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE3, (*prIt)->sLibelle.c_str()) ; break ;
      case 4 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE4, (*prIt)->sLibelle.c_str()) ; break ;
      case 5 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE5, (*prIt)->sLibelle.c_str()) ; break ;
      case 6 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE6, (*prIt)->sLibelle.c_str()) ; break ;
      case 7 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE7, (*prIt)->sLibelle.c_str()) ; break ;
      case 8 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE8, (*prIt)->sLibelle.c_str()) ; break ;
      case 9 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE9, (*prIt)->sLibelle.c_str()) ; break ;
    }
  }
  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
  menu->AppendMenu(MF_STRING, IDC_CODE, "Autre") ;

  ClientToScreen(lp) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  delete menu ;
}
catch (...)
{
	erreur("Exception NSCaptView::EvRButtonDown.", standardError, 0) ;
}
}


voidNSCaptView::EvPosit(int iPos)
{
try
{
	PrinciplesArray * pPrincipes = pDoc->pEpisodus->pPrincipes ;
	if (pPrincipes->empty())
		return ;

	PrinciplesIter prIt = pPrincipes->begin() ;
	for (int i = 1 ; (pPrincipes->end() != prIt) && (i < iPos) ; prIt++, i++)
    ;
	if (pPrincipes->end() == prIt)
		return ;

	string sChemin = string("GCONS1/") + (*prIt)->sCase ;

#ifndef _EXT_CAPTURE
	NSCapture * pCapt = new NSCapture(pDoc->pContexte, sChemin, sSelection, FromOutside, (*prIt)->sClassification) ;
#else
	NSCapture * pCapt = new NSCapture(sChemin, sSelection, FromOutside, (*prIt)->sClassification) ;
#endif

	NSEpisodus * pEpiso = pDoc->pContexte->getEpisodus() ;

	// Ajout de l'objet à l'array de capture / Adding the object to capture array
	if (pCapt)
	{
		pEpiso->newCaptureArray.ajouter(pCapt) ;

		// Ajout au réservoir de SOAP
		// Adding to SOAPing tank
		if (pCapt && (pCapt->getClassifier() != string("")))
			pEpiso->addToSOAP(pCapt) ;
	}

	// On prévient le système qu'une nouvelle capture a été effectuée
	// Warning the system that a new capture occured
	/* bool bCapt = */ pDoc->pContexte->captureData(&(pEpiso->newCaptureArray)) ;
}
catch (...)
{
	erreur("Exception NSCaptView::EvPosit.", standardError, 0) ;
}
}

voidNSCaptView::EvCode()
{
try
{
	if (string("") == sSelection)
		return ;

#ifndef _EXT_CAPTURE
	NSCapture * pCapt = new NSCapture(pDoc->pContexte, string(""), sSelection, FromOutside, string("")) ;
#else
	NSCapture * pCapt = new NSCapture("", sSelection, "") ;
#endif

	NSEpisodus* pEpiso = pDoc->pContexte->getEpisodus() ;

	// Ajout de l'objet à l'array de capture / Adding the object to capture array	if (pCapt)
	{
		pEpiso->newCaptureArray.ajouter(pCapt) ;

		// Ajout au réservoir de SOAP
		// Adding to SOAPing tank
		if (pCapt && (pCapt->getClassifier() != string("")))
			pEpiso->addToSOAP(pCapt) ;
	}

	// On prévient le système qu'une nouvelle capture a été effectuée
	// Warning the system that a new capture occured
	//
	/* bool bCapt = */ pDoc->pContexte->captureData(&(pEpiso->newCaptureArray)) ;
}
catch (...)
{
	erreur("Exception NSCaptView::EvCode.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------// fonction EVSetFocus// -----------------------------------------------------------------------------
void
NSCaptView::EvSetFocus(HWND hWndLostFocus)
{
	TMyApp * pMyApp = pDoc->pContexte->getSuperviseur()->getApplication() ;

	//TWindowView::EvSetFocus(hWndLostFocus) ;
  if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
  {
    SetupToolBar() ;
    pMyApp->SetToolBarWindow(GetWindow()) ;
  }
  TEdit::EvSetFocus(hWndLostFocus) ;
}


voidNSCaptView::SetupToolBar()
{
	TMyApp * pMyApp = pDoc->pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;

	//pMyApp->cb2->Insert(*new TButtonGadget(IDC_EPISOD_CLASSIF, IDC_EPISOD_CLASSIF, TButtonGadget::Command));
	//pMyApp->cb2->Insert(*new TButtonGadget(CM_EPISO_TRANSMIT, CM_EPISO_TRANSMIT, TButtonGadget::Command));
	pMyApp->getSecondaryControlBar()->LayoutSession() ;
}


// -----------------------------------------------------------------------------
//
// Class FlexArray
//
// -----------------------------------------------------------------------------
/*
FlexArray::FlexArray(FlexArray &rv)
  : ArrayOfFlex()
{
try
{
  if (rv.empty())
    return ;

  for (FlexIter It = rv.begin() ; It != rv.end() ; It++)
    push_back(new flexObject(*(*It))) ;
}
catch (...)
{
  erreur("Exception FlexArray copy ctor.", standardError, 0) ;
}
}

FlexArray::~FlexArray()
{
  vider() ;
}


FlexArray&
FlexArray::operator=(FlexArray src)
{
try
{
  vider() ;

  if (src.empty())
    return (*this) ;

  for (FlexIter It = src.begin() ; It != src.end() ; It++)
    push_back(new flexObject(*(*It))) ;

  return (*this) ;
}
catch (...)
{
  erreur("Exception FlexArray = operator.", standardError, 0) ;
  return (*this) ;
}
}


void
FlexArray::vider()
{
  if (empty())
    return ;

  for (FlexIter It = begin() ; It != end() ; )
  {
    delete (*It) ;
    erase(It) ;
  }
}
*/



/*
// -----------------------------------------------------------------------------
//
// Class AnaCaptArray
//
// -----------------------------------------------------------------------------

AnaCaptArray::AnaCaptArray(AnaCaptArray &rv)
  : AnaCaptVector()
{
try
{
  if (rv.empty())
    return ;

  for (AnaCaptIter It = rv.begin() ; It != rv.end() ; It++)
    push_back(new analysedCapture(*(*It))) ;
}
catch (...)
{
  erreur("Exception AnaCaptArray copy ctor.", standardError, 0) ;
}
}


AnaCaptArray::~AnaCaptArray()
{
  vider() ;
}

AnaCaptArray&
AnaCaptArray::operator=(AnaCaptArray src)
{
try
{
  vider() ;

  if (src.empty())
    return (*this) ;

  for (AnaCaptIter It = src.begin() ; It != src.end() ; It++)
    push_back(new analysedCapture(*(*It))) ;

  return (*this) ;
}
catch (...)
{
  erreur("Exception AnaCaptArray = operator.", standardError, 0) ;
  return (*this) ;
}
}


void
AnaCaptArray::vider()
{
  if (empty())
    return ;

  for (AnaCaptIter It = begin() ; It != end() ; )
  {
    delete (*It) ;
    erase(It) ;
  }
}
*/



// -----------------------------------------------------------------------------
//
// Class NSCaptLine
//
// -----------------------------------------------------------------------------

NSCaptLine::NSCaptLine()
{
  pCapturedElement    = 0 ;
  iStartPosInElement  = 0 ;
  iEndPosInElement    = 0 ;
  iStatus             = StatusInit ;
  pTextLine           = 0 ;
}


NSCaptLine::NSCaptLine(const NSCaptLine& rv)
{
try
{
  pCapturedElement    = rv.pCapturedElement ;
  iStartPosInElement  = rv.iStartPosInElement ;
  iEndPosInElement    = rv.iEndPosInElement ;
  iStatus             = rv.iStatus ;
  pTextLine           = (NSLigne*) 0 ;
  if (rv.pTextLine)
    pTextLine = new NSLigne(*(rv.pTextLine)) ;
}
catch (...)
{
	erreur("Exception NSCaptLine copy ctor.", standardError, 0) ;
}
}

NSCaptLine::~NSCaptLine()
{
  if (pTextLine)
    delete pTextLine ;
}

string
NSCaptLine::getText()
{
  if ((NSLigne*) NULL == pTextLine)
    return string("") ;

  return pTextLine->Texte ;
}

size_t
NSCaptLine::getTextLen()
{
  if ((NSLigne*) NULL == pTextLine)
    return 0 ;
  return strlen(pTextLine->Texte.c_str()) ;
}

void
NSCaptLine::setText(string sText)
{
  if ((NSLigne*) NULL == pTextLine)
    return ;
  pTextLine->Texte = sText ;
}

NS_CLASSLIB::TRect
NSCaptLine::getBox() const
{
  if ((NSLigne*) NULL == pTextLine)
    return NS_CLASSLIB::TRect(0, 0, 0, 0) ;

  return pTextLine->Boite ;
}

NSCaptLine&
NSCaptLine::operator=(const NSCaptLine& src)
{
try
{
  if (&src == this)
		return *this ;

  pCapturedElement    = src.pCapturedElement ;
  iStartPosInElement  = src.iStartPosInElement ;
  iEndPosInElement    = src.iEndPosInElement ;
  iStatus             = src.iStatus ;
  pTextLine           = (NSLigne*) 0 ;
  if (src.pTextLine)
    pTextLine = new NSLigne(*(src.pTextLine)) ;

  return (*this) ;
}
catch (...)
{
	erreur("Exception NSCaptLine = operator.", standardError, 0) ;
	return (*this) ;
}
}

// -----------------------------------------------------------------------------
//
// Class CaptLineArray
//
// -----------------------------------------------------------------------------

CaptLineArray::CaptLineArray(const CaptLineArray& rv)
              :CaptLineVector()
{
try
{
	if (rv.empty())
		return ;

	for (CaptLineConstIter It = rv.begin() ; rv.end() != It ; It++)
		push_back(new NSCaptLine(*(*It))) ;
}
catch (...)
{
	erreur("Exception CaptLineArray copy ctor.", standardError, 0) ;
}
}

CaptLineArray::~CaptLineArray()
{
	vider() ;
}

CaptLineArray&
CaptLineArray::operator=(const CaptLineArray& src)
{
try
{
  if (&src == this)
		return *this ;

  vider() ;

  if (src.empty())
    return (*this ) ;

  for (CaptLineConstIter It = src.begin() ; src.end() != It ; It++)
    push_back(new NSCaptLine(*(*It))) ;

  return (*this) ;
}
catch (...)
{
	erreur("Exception CaptLineArray = operator.", standardError, 0) ;
	return (*this) ;
}
}

CaptLineIter
CaptLineArray::getLine(size_t iNumLine)
{
  if (empty())
    return NULL ;

  CaptLineIter It = begin() ;
  for (size_t i = 0 ; (i < iNumLine) && (It != end()) ; It++, i++)
    ;

  if (end() == It)
    return NULL ;

  return It ;
}

void
CaptLineArray::vider()
{
  if (empty())
    return ;

  for (CaptLineIter It = begin() ; end() != It ; )
  {
    delete (*It) ;
    erase(It) ;
  }
}

// -----------------------------------------------------------------------------
//
// Class NSMultiCaptView
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSMultiCaptView, NSMUEView)
  EV_WM_RBUTTONDOWN,
  EV_WM_LBUTTONDOWN,
  EV_WM_LBUTTONUP,
  EV_WM_LBUTTONDBLCLK,
  EV_WM_MOUSEMOVE,
  EV_WM_SETFOCUS,
  EV_WM_KEYDOWN,
  EV_WM_VSCROLL,
  EV_WM_HSCROLL,
  EV_COMMAND(IDC_PRINCIPE1,   EvPosit1),
  EV_COMMAND(IDC_PRINCIPE2,   EvPosit2),  EV_COMMAND(IDC_PRINCIPE3,   EvPosit3),
  EV_COMMAND(IDC_PRINCIPE4,   EvPosit4),
  EV_COMMAND(IDC_PRINCIPE5,   EvPosit5),
  EV_COMMAND(IDC_PRINCIPE6,   EvPosit6),
  EV_COMMAND(IDC_PRINCIPE7,   EvPosit7),
  EV_COMMAND(IDC_PRINCIPE8,   EvPosit8),
  EV_COMMAND(IDC_PRINCIPE9,   EvPosit9),
  EV_COMMAND(IDC_CODE,        EvCode),
  EV_COMMAND(IDC_OCR_LEARN,   EvOcrLearn),
END_RESPONSE_TABLE ;

NSMultiCaptView::NSMultiCaptView(NSCaptDocument& doc, TWindow *parent, string sConcern)
                :NSMUEView(doc.pContexte, &doc, parent, "Capture", "CaptDoc", sConcern)
{
	// Initialise le style avec des ascenseurs horizontaux et verticaux
  Attr.Style    = Attr.Style | WS_HSCROLL | WS_VSCROLL | CS_OWNDC ;
  pCaptDoc      = &doc ;
  pToolBar      = 0 ;
  bSetupToolBar = true ;

  initParams() ;

  setViewName() ;
}

NSMultiCaptView::~NSMultiCaptView()
{
}

void
NSMultiCaptView::setViewName()
{
	_sViewName = pContexte->getSuperviseur()->getText("captureManagement", "captureViewTitle") ;

  addConcernTitle() ;
}

// -----------------------------------------------------------------------------
// GetWindow renvoie this (à redéfinir car virtual dans TView)
// -----------------------------------------------------------------------------
TWindow *
NSMultiCaptView::GetWindow()
{
  return ((TWindow *)this) ;
}

// -----------------------------------------------------------------------------// Appel de la fonction de remplissage de la vue// -----------------------------------------------------------------------------
void
NSMultiCaptView::SetupWindow()
{
  NSMUEView::SetupWindow() ;

  Parent->SetCaption(_sViewName.c_str()) ;

  initLines() ;
  if (aLignes.empty())
    return ;

  // initHistory() ;
  InitialiseLignes() ;
}

void
NSMultiCaptView::EvSetFocus(HWND hWndLostFocus)
{
  NSMUEView::EvSetFocus(hWndLostFocus) ;
  _pPaneSplitter->SetFrameTitle(getFunction(), _sViewName) ;
  CreateCaret(false /* isGray */, 2 /* width */, 10 /* height */) ;
  setCaretPosition() ;
  ShowCaret() ;
}

void
NSMultiCaptView::EvKillFocus(HWND hWndGetFocus)
{
  DestroyCaret() ;
}

void
NSMultiCaptView::initLines()
{
  aLignes.vider() ;

  if (!pCaptDoc)
    return ;
  if (pCaptDoc->aCaptures.empty())
    return ;

  AnaCaptIter cptIt = pCaptDoc->aCaptures.begin() ;
  for ( ; cptIt != pCaptDoc->aCaptures.end() ; cptIt++)
    convertLine(cptIt) ;

  setCaretPosition() ;
}

void
NSMultiCaptView::addedLines()
{
  if (!pCaptDoc || (pCaptDoc->aCaptures.empty()))
    return ;

  bool bFirstLines ;
  if (aLignes.empty())
    bFirstLines = true ;
  else
    bFirstLines = false ;

  for (AnaCaptIter cptIt = pCaptDoc->aCaptures.begin() ; cptIt != pCaptDoc->aCaptures.end() ; cptIt++)
  	if (false == (*cptIt)->isDisplayed())
    	convertLine(cptIt) ;

  InitialiseLignes() ;

  if (bFirstLines)
    setCaretPosition() ;
}

void
NSMultiCaptView::convertLine(AnaCaptIter cptIt)
{
try
{
  string sText = (*cptIt)->getText() ;

  size_t posPrev  = 0 ;
  size_t posCr    = sText.find("\r") ;
  size_t iTextLen = strlen(sText.c_str()) ;
  while (posCr != string::npos)
  {
    string sTextLine = string(sText, posPrev, posCr - posPrev) ;
    posPrev = posCr + 1 ;
    if (posPrev >= iTextLen)
      return ;

    NSCaptLine* pCaptLine = new NSCaptLine ;
    pCaptLine->pCapturedElement   = *cptIt ;
    pCaptLine->iStartPosInElement = posPrev ;
    pCaptLine->iEndPosInElement   = posCr - 1 ;

    pCaptLine->pTextLine = new NSLigne ;
    pCaptLine->setText(sTextLine) ;
    aLignes.push_back(pCaptLine) ;

    if ('\n' == sText[posPrev])
    {
      posPrev++ ;
      if (posPrev >= iTextLen)
        return ;
    }
    posCr = sText.find("\r", posPrev) ;
  }

  string sTextLine = string(sText, posPrev, iTextLen - posPrev) ;

  NSCaptLine* pCaptLine = new NSCaptLine ;
  pCaptLine->pCapturedElement   = *cptIt ;
  pCaptLine->iStartPosInElement = posPrev ;
  pCaptLine->iEndPosInElement   = iTextLen - 1 ;

  pCaptLine->pTextLine = new NSLigne ;
  pCaptLine->setText(sTextLine) ;

  aLignes.push_back(pCaptLine) ;
}
catch (...)
{
	erreur("Exception NSMultiCaptView::convertLine.", standardError, 0) ;
}
}

void
NSMultiCaptView::initParams()
{
  PremLigne         = 1 ;
  HautGcheFenetre.x = HautGcheFenetre.y = 0 ;
  LargeurPolice     = 0 ;

  bSelected         = false ;
  bMouseSelecting   = false ;
  firstSelPoint.setLigne(0) ;
  firstSelPoint.setPos(0) ;
  lastSelPoint.setLigne(0) ;
  lastSelPoint.setPos(0) ;
  bDragDrop         = false ;

  captCaret.setLigne(0) ;
  captCaret.setPos(0) ;

  // Mise en place des polices dans le tableau - Storing fonts in their array
  // Rappel : NSFont(facename, height, width, escapement, orientation, weight,
  //                 pitchAndFamily, italic, underline, strikeout, charSet,
  //                 outputPrecision, clipPrecision, quality)
  // Police 0 : Eléments non utilisés - Font 0 : not used elements
	StylesPolice.push_back(new NSFont("Helvetica", 14, 0, 0, 0, 0, 0, 0, 0, 0, 1)) ;
    // Police 1 : Eléments utilisés - Font 0 : used elements
	StylesPolice.push_back(new NSFont("Helvetica",14,0,0,0,0,0,0,0,0,1)) ;
    // Police 2 : Eléments en cours - Font 0 : current elements
	StylesPolice.push_back(new NSFont("Helvetica",14,0,0,0,0,700,0,0,0,1)) ;
}

void
NSMultiCaptView::InitialiseLignes()
{
try
{
  if (aLignes.empty())
    return ;

  RectangleGlobal.left    = 0 ;
  RectangleGlobal.top     = 0 ;
  RectangleGlobal.right   = 0 ;
  RectangleGlobal.bottom  = 0 ;

	// On récupère un "contexte d'information" concernant l'écran
	TIC * pDC = new TIC("DISPLAY", NULL, NULL) ;
	pDC->SetMapMode(MM_LOMETRIC) ;

  NS_CLASSLIB::TRect RectangleTexte ;
  RectangleTexte.Set(RectangleGlobal.Left() + 5, RectangleGlobal.Top() - 5, RectangleGlobal.Right(), RectangleGlobal.Bottom()) ;

	// On initialise le point de référence en haut et à gauche
  NS_CLASSLIB::TPoint PointRef ;
	PointRef.x = RectangleTexte.Left() ;
  PointRef.y = RectangleTexte.Top() ;

  for (CaptLineIter it = aLignes.begin() ; it != aLignes.end() ; it++)
  {
    NSFont PoliceVoulue ;
    bool bOkFont = getFont((*it)->iStatus, &PoliceVoulue) ;
    if (bOkFont)
    {
      if (PoliceVoulue.logFont.lfHeight >= 0)
      {
        NS_CLASSLIB::TPoint	PointTransfert ;
        PointTransfert.x = PoliceVoulue.logFont.lfHeight ;
        PointTransfert.y = PoliceVoulue.logFont.lfWidth ;
        pDC->DPtoLP(&PointTransfert) ;
        PoliceVoulue.logFont.lfHeight = PointTransfert.x ;
        PoliceVoulue.logFont.lfWidth  = PointTransfert.y ;
      }

      TFont* pPolice = new TFont(&(PoliceVoulue.logFont)) ;
      pDC->SelectObject(*pPolice) ;
      TEXTMETRIC  TextMetric ;
      LONG        lPolicetmHeight = 0 ;
      if (pDC->GetTextMetrics(TextMetric))
      {
        if ((LargeurPolice == 0) || (LargeurPolice > TextMetric.tmAveCharWidth))
          LargeurPolice = TextMetric.tmAveCharWidth ;
        lPolicetmHeight = TextMetric.tmHeight + TextMetric.tmDescent ;
      }

      // On prévoit l'espace au dessus du paragraphe
      // PointRef.Offset(0, -StylePara.EspaceDessus);
      // Initialisation de tous les paramètres déjà connus
      (*it)->pTextLine->Boite.left    = RectangleTexte.Left() ;
      (*it)->pTextLine->Boite.top	    = PointRef.y ;
      (*it)->pTextLine->Boite.bottom  = PointRef.y ;

      string sTexte = (*it)->getText() ;
      NS_CLASSLIB::TSize TailleChaine = pDC->GetTextExtent(sTexte.c_str(), strlen(sTexte.c_str())) ;
      (*it)->pTextLine->Boite.right = (*it)->pTextLine->Boite.left + TailleChaine.cx ;

      LONG lHauteur = lPolicetmHeight ;
      if (lHauteur == 0)
        lHauteur = TailleChaine.cy ;
      (*it)->pTextLine->Boite.bottom -= lHauteur ;
      PointRef.Offset(0, -lHauteur) ;

      delete pPolice ;

      // Mise à jour de RectangleGlobal
      if (RectangleGlobal.right < (*it)->pTextLine->Boite.right)
        RectangleGlobal.right = (*it)->pTextLine->Boite.right ;
      RectangleGlobal.bottom = (*it)->pTextLine->Boite.bottom ;
    }
  }
	delete pDC ;
}
catch (...)
{
	erreur("Exception NSMultiCaptView::InitialiseLignes.", standardError, 0) ;
}
}

bool
NSMultiCaptView::getFont(NSCaptLine::CAPTSTATUS iStatus, NSFont* pFont)
{
  if (!pFont)
    return false ;

  switch (iStatus)
  {
    case NSCaptLine::StatusCaptured : // Item déjà traité
                                      *pFont = *(StylesPolice[0]) ;
                                      return true ;
    case NSCaptLine::StatusNotFound : // Item dont le traitement a échoué
                                      *pFont = *(StylesPolice[2]) ;
                                      return true ;
    default                         : // Autre
                                      *pFont = *(StylesPolice[2]) ;
                                      return true ;
  }

/*
    // Item déjà traité
    if      ((*cptIter)->iStatus == NSCaptLine::StatusCaptured)
      PoliceVoulue = *(StylesPolice[0]) ;

    // Item dont le traitement a échoué
    else if ((*cptIter)->iStatus == NSCaptLine::StatusNotFound)
      PoliceVoulue = *(StylesPolice[2]) ;

    // Autre
    else
      PoliceVoulue = *(StylesPolice[1]) ;
*/
}

void
NSMultiCaptView::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  CAPTHIT capt = HitTest(point) ;
  if (!(capt.getInside()))
  {
    bSelected = false ;
    Invalidate() ;
    return ;
  }

  if (inSelectedArea(capt))
    selectSentence(capt) ;
  else
  {
    selectWord(capt) ;

    // Si on a fait double click sur un blanc on sélectionne la phrase
    if (!bSelected)
      selectSentence(capt) ;
  }

  Invalidate() ;
  return ;
}


// -----------------------------------------------------------------------------
// click gauche de la souris
// Mouse left button click
// -----------------------------------------------------------------------------
void
NSMultiCaptView::EvLButtonDown(uint /*modkeys*/, NS_CLASSLIB::TPoint& point)
{
try
{
	// If the click is outside the text, return
  //
  bool bCaretBefore = true ;
	CAPTHIT capt = HitTest(point, &bCaretBefore) ;
	if (!(capt.getInside()))
	{
		bSelected = false ;
		Invalidate() ;
		return ;
	}
	// Test drag and drop first, because it is outside "caret management"
  //
	if (bDragDrop)
  	return ;

	// If there is already a selected zone then :
  // 1) click is inside this zone, and we begin a drag and drop
  // 2) click is outside this zone, and we begin a new selection
  //
	if (bSelected)
	{
		if (inSelectedArea(capt))
		{
			bDragDrop = true ;
			return ;
		}
    else
    	Invalidate() ;
	}

	captCaret = capt ;
  if (!bCaretBefore)
  	captCaret.nextPos() ;
	bSelected = false ;

	bMouseSelecting = true ;
	selectingRefPoint = captCaret ;

	firstSelPoint = captCaret ;
	lastSelPoint = captCaret ;
	setCaretPosition() ;
	return ;
}
catch (...)
{
	erreur("Exception NSMultiCaptView::EvLButtonDown.", standardError, 0) ;
}
}

void
NSMultiCaptView::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point)
{
try
{
	bMouseSelecting = false ;

  if (bDragDrop)
  	bDragDrop = false ;

  bool bCaretBefore ;
  CAPTHIT capt = HitTest(point, &bCaretBefore) ;
  if (!(capt.getInside()))
  {
    // bSelected = false ;
    Invalidate() ;
    return ;
  }
  captCaret = capt ;
  if (!bCaretBefore)
		captCaret.nextPos() ;
  setCaretPosition() ;
}
catch (...)
{
	erreur("Exception NSMultiCaptView::EvLButtonDown.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// traitement de l'événement généré par le mouvement de la souris
// -----------------------------------------------------------------------------
// void	NSLdvView::EvMouseMove(unint modKeys, point)
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void
NSMultiCaptView::EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point)
{
try
{
	// If the click is outside the text, return
  //
  bool bCaretBefore ;
	CAPTHIT capt = HitTest(point, &bCaretBefore) ;
	if (!(capt.getInside()))
		return ;

	if (bMouseSelecting)
	{
  	captCaret = capt ;
    if (!bCaretBefore)
  		captCaret.nextPos() ;

    bSelected = true ;   // To enter in updateSelection
    updateSelection() ;

  	setCaretPosition() ;
    Invalidate() ;
	}
}
catch (...)
{
	erreur("Exception NSMultiCaptView EvMouseMove.", standardError, 0) ;
}
}


// -----------------------------------------------------------------------------
// clic droit de la souris sur un noeud
// -----------------------------------------------------------------------------
void
NSMultiCaptView::EvRButtonDown(uint /*modkeys*/, NS_CLASSLIB::TPoint& point)
{
try
{
  CAPTHIT capt = HitTest(point) ;
	if (!(capt.getInside()))
		return ;

  if (!bSelected)
  {
    selectWord(capt) ;
    if (!bSelected)
      selectSentence(capt) ;

    if (bSelected)
      Invalidate() ;
  }

  // Clic droit dans la zone sélectionnée
  // Right-click in the selected area
  if (inSelectedArea(capt))
  {
    string sSelection = getSelText() ;
    if (sSelection == "")
      return ;

    NSEpisodus * pEpiso = pContexte->getEpisodus() ;

    PrinciplesArray * pPrincipes = pEpiso->pPrincipes ;
    if (!pPrincipes || pPrincipes->empty())
      return ;

    NS_CLASSLIB::TPoint lp = point ;
    TPopupMenu * menu = new TPopupMenu() ;

    PrinciplesIter prIt = pPrincipes->begin() ;
    for (int i = 1 ; prIt != pPrincipes->end() ; prIt++, i++)
    {
      switch (i)
      {
        case 1 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE1, (*prIt)->sLibelle.c_str()) ; break ;
        case 2 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE2, (*prIt)->sLibelle.c_str()) ; break ;
        case 3 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE3, (*prIt)->sLibelle.c_str()) ; break ;
        case 4 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE4, (*prIt)->sLibelle.c_str()) ; break ;
        case 5 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE5, (*prIt)->sLibelle.c_str()) ; break ;
        case 6 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE6, (*prIt)->sLibelle.c_str()) ; break ;
        case 7 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE7, (*prIt)->sLibelle.c_str()) ; break ;
        case 8 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE8, (*prIt)->sLibelle.c_str()) ; break ;
        case 9 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE9, (*prIt)->sLibelle.c_str()) ; break ;
      }
    }
    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
    string sTitle = pContexte->getSuperviseur()->getText("captureMenus", "Other") ;
    menu->AppendMenu(MF_STRING, IDC_CODE, sTitle.c_str()) ;

    analysedCapture * pCaptured = getSelCapture() ;
    if (pCaptured && pCaptured->getOcrBloc())
    {
      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      sTitle = pContexte->getSuperviseur()->getText("captureMenus", "OCRLearn") ;
      menu->AppendMenu(MF_STRING, IDC_OCR_LEARN, sTitle.c_str()) ;
    }

    ClientToScreen(lp) ;
    menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
    delete menu ;
  }
}
catch (...)
{
	erreur("Exception NSMultiCaptView::EvRButtonDown.", standardError, 0) ;
}
}


void
NSMultiCaptView::EvChar(uint key, uint repeatCount, uint flags)
{
}


void
NSMultiCaptView::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	WORD wScrollNotify  = 0xFFFF ;
  bool bVScroll       = true ;

  switch (key)
	{
/*
    case 'a'      : break ;
    case 'A'      : // Control-A : Tout sélectionner
                    if (GetKeyState(VK_CONTROL) < 0)
                      SelectAll() ;
                    break ;
    case 'c'      : break ;
    case 'C'      : // Control-C : copier vers le presse-papier
                    if (GetKeyState(VK_CONTROL) < 0)
                      CopyToClipboard() ;
                    break ;
*/
    case VK_UP    :

    	// wScrollNotify = SB_LINEUP ;
      bVScroll      = false ;

      // Shift pressed : selection
      if (GetKeyState(VK_SHIFT) < 0)
      	selecting() ;
      else
      	bSelected = false ;

      captCaret = upPoint(captCaret) ;

      updateSelection() ;

      break ;

    case VK_PRIOR : wScrollNotify = SB_PAGEUP ;
                    break ;
    case VK_NEXT  : wScrollNotify = SB_PAGEDOWN ;
                    break ;
    case VK_DOWN  :

    	// wScrollNotify = SB_LINEDOWN ;
      bVScroll      = false ;

      // Shift pressed : selection
      if (GetKeyState(VK_SHIFT) < 0)
      	selecting() ;
      else
      	bSelected = false ;

      captCaret = downPoint(captCaret) ;

      updateSelection() ;

      break ;

    case VK_HOME  :

    	// wScrollNotify = SB_TOP ;
      bVScroll      = false ;

      // Shift pressed : selection
      if (GetKeyState(VK_SHIFT) < 0)
      	selecting() ;
      else
      	bSelected = false ;

      captCaret = firstLinePoint(captCaret) ;

      updateSelection() ;

      break ;

    case VK_END   :

    	// wScrollNotify = SB_BOTTOM ;
      bVScroll      = false ;

      // Shift pressed : selection
      if (GetKeyState(VK_SHIFT) < 0)
      	selecting() ;
      else
      	bSelected = false ;

      captCaret = lastLinePoint(captCaret) ;

      updateSelection() ;

      break ;

    case VK_RIGHT :

    	// wScrollNotify = SB_LINERIGHT ;
      bVScroll      = false ;

      // Shift pressed : selection
      if (GetKeyState(VK_SHIFT) < 0)
      	selecting() ;
      else
      	bSelected = false ;

      captCaret = nextPoint(captCaret) ;

      updateSelection() ;

			break ;

    case VK_LEFT  :

    	// wScrollNotify = SB_LINELEFT ;
      bVScroll      = false ;

      // Shift pressed : selection
      if (GetKeyState(VK_SHIFT) < 0)
      	selecting() ;
      else
      	bSelected = false ;

			captCaret = previousPoint(captCaret) ;

      updateSelection() ;

			break ;
  }

  if (wScrollNotify != 0xFFFF)
  {
    if (bVScroll)
      SendMessage(WM_VSCROLL, MAKELONG(wScrollNotify, 0), 0L) ;
    else
      SendMessage(WM_HSCROLL, MAKELONG(wScrollNotify, 0), 0L) ;
  }

	TWindowView::EvKeyDown(key, repeatCount, flags) ;
  setCaretPosition() ;
  Invalidate() ;
}


void
NSMultiCaptView::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  if (aLignes.empty())
    return ;

	int iAncDC = dc.SaveDC() ;

	// Fixe la métrique et l'origine
	dc.SetMapMode(MM_LOMETRIC) ;

	// Stockage des points supérieurs gauches et inférieurs droits du rectangle
  NS_CLASSLIB::TPoint Points[2] ;
	Points[0] = RectAPeindre.TopLeft() ;
	Points[1] = RectAPeindre.BottomRight() ;

	// Transformation de ces points en coordonnées logiques
	dc.DPtoLP(Points, 2) ;

	// Calcul du rectangle du document qui correspond au RectAPeindre de l'écran
	// ATTENTION : dans le mode MM_LOMETRIC les coordonnées en ordonnées sont négatives
	NS_CLASSLIB::TRect* pRectDocument = new NS_CLASSLIB::TRect(Points[0], Points[1]) ;
	*pRectDocument = pRectDocument->Offset(HautGcheFenetre.x, HautGcheFenetre.y) ;
	pRectDocument->Normalized() ;

	// On demande à toutes les lignes concernées de se repeindre

  // repeinte de la zone d'affichage
  UINT iNumLigne = 0 ;
  for (CaptLineIter it = aLignes.begin() ; it != aLignes.end() ; it++, iNumLigne++)
  {
    NS_CLASSLIB::TRect BoiteNormale = (*it)->getBox() ;
		BoiteNormale.Normalized();
    //  ATTENTION : la méthode Touches de TRect ne fonctionne pas
    //	if (pRectDocument->Touches(BoiteNormale))
		if ((BoiteNormale.right >= pRectDocument->left) && (BoiteNormale.left <= pRectDocument->right) && (BoiteNormale.bottom <= pRectDocument->top) && (BoiteNormale.top >= pRectDocument->bottom))
    {
      string sTexte = (*it)->getText() ;

      // Choix de la police
      NSFont PoliceVoulue ;
      bool bOkFont = getFont((*it)->iStatus, &PoliceVoulue) ;
      if (bOkFont)
      {
        if (PoliceVoulue.logFont.lfHeight >= 0)
        {
          NS_CLASSLIB::TPoint	PointTransfert ;
          PointTransfert.x = PoliceVoulue.logFont.lfHeight ;
          PointTransfert.y = PoliceVoulue.logFont.lfWidth ;
          dc.DPtoLP(&PointTransfert) ;
          PoliceVoulue.logFont.lfHeight = PointTransfert.x ;
          PoliceVoulue.logFont.lfWidth  = PointTransfert.y ;
        }

        TFont * pPolice = new TFont(&(PoliceVoulue.logFont)) ;
        dc.SelectObject(*pPolice) ;

        // Calcul de la taille effective de la ligne
        SIZE lpSize ;
        BOOL Continuer = ::GetTextExtentPoint32(HDC(dc), sTexte.c_str(), strlen(sTexte.c_str()), &lpSize) ;
        if (Continuer)
        {
          // Calcul du point écran où afficher
          NS_CLASSLIB::TPoint PointAffichage ;
          NS_CLASSLIB::TRect  rectBox = (*it)->getBox() ;
          PointAffichage.x = rectBox.left ;
          PointAffichage.y = rectBox.top ;
          PointAffichage -= HautGcheFenetre ;

          // Si cette ligne contient des caractères sélectionnés
          // If this line contains selected chars
          if (bSelected && (int(iNumLigne) >= firstSelPoint.getLigne()) && (int(iNumLigne) <= lastSelPoint.getLigne()))
          {
            size_t iInitialLength = strlen(sTexte.c_str()) ;

            // Caractères non sélectionnées en début de ligne
            // Non selected chars in the begining of the line
            if ((int(iNumLigne) == firstSelPoint.getLigne()) && (firstSelPoint.getPos() > 0))
            {
              string sTextBegin = string(sTexte, 0, firstSelPoint.getPos()) ;
              SIZE lpSizeBefore ;
              BOOL Good = ::GetTextExtentPoint32(HDC(dc), sTextBegin.c_str(), strlen(sTextBegin.c_str()), &lpSizeBefore) ;
              if (Good)
              {
                dc.SetTextJustification(0, 0) ;
                dc.SetTextColor((*it)->pTextLine->CouleurTexte) ;                dc.TextOut(PointAffichage, sTextBegin.c_str()) ;
                PointAffichage.x += lpSizeBefore.cx ;

                sTexte = string(sTexte, firstSelPoint.getPos(), strlen(sTexte.c_str()) - firstSelPoint.getPos()) ;
              }
            }

            // Caractères sélectionnés
            string sTextAfter = "" ;
            if ((int(iNumLigne) == lastSelPoint.getLigne()) && (lastSelPoint.getPos() < iInitialLength - 1))
            {
              size_t  iCurrentLen = strlen(sTexte.c_str()) ;
              size_t  iSepar = lastSelPoint.getPos() - (iInitialLength - iCurrentLen) ;
              sTextAfter  = string(sTexte, iSepar + 1, iCurrentLen-iSepar - 1) ;
              sTexte      = string(sTexte, 0, iSepar + 1) ;
            }

            TColor bkColor = dc.GetBkColor() ;

            dc.SetTextJustification(0, 0) ;
            dc.SetBkColor(TColor::SysHighlight) ;
            dc.SetTextColor(TColor::SysHighlightText) ;            dc.TextOut(PointAffichage, sTexte.c_str()) ;

            dc.SetBkColor(bkColor) ;

            if (sTextAfter != "")
            {
              SIZE lpSizeText ;
              BOOL Good = ::GetTextExtentPoint32(HDC(dc), sTexte.c_str(), strlen(sTexte.c_str()), &lpSizeText) ;

              if (Good)
              {
                PointAffichage.x += lpSizeText.cx ;
                dc.SetTextJustification(0, 0) ;
                dc.SetTextColor((*it)->pTextLine->CouleurTexte) ;                dc.TextOut(PointAffichage, sTextAfter.c_str()) ;
              }
            }
          }
          else
          {
            dc.SetTextJustification(0, 0) ;
            dc.SetTextColor((*it)->pTextLine->CouleurTexte) ;            dc.TextOut(PointAffichage, sTexte.c_str()) ;
          }
        }
        dc.RestoreFont() ;
        delete pPolice ;
      }    }
	}
	delete pRectDocument ;
	dc.RestoreDC(iAncDC) ;
}


// -----------------------------------------------------------------------------
// Scroll vertical
// -----------------------------------------------------------------------------
void
NSMultiCaptView::EvVScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)
{
	if (aLignes.empty())
		return ;

	const NSCaptLine * pLigne ;
	int		iTailleScroll ;
	NS_CLASSLIB::TPoint * pPointRef ;

	// On sort tout de suite si on est sur les lignes extrèmes
	if (((scrollCode == SB_LINEDOWN) && (PremLigne == aLignes.size())) || ((scrollCode == SB_PAGEDOWN) && (PremLigne == aLignes.size())) || ((scrollCode == SB_LINEUP) && (PremLigne == 1)) || ((scrollCode == SB_PAGEUP) && (PremLigne == 1)))
		return ;

	// On récupère un "contexte d'information" concernant l'écran
	TIC * pDC = new TIC("DISPLAY", NULL, NULL) ;
	pDC->SetMapMode(MM_LOMETRIC) ;

	if (scrollCode == SB_LINEDOWN)
	{
  	// Scroll vers le haut (flêche basse)

		// Calcul de la quantité à scroller (hauteur de la première ligne)
		pLigne = aLignes[PremLigne - 1] ;
    NS_CLASSLIB::TRect rectBox = pLigne->getBox() ;
		iTailleScroll = rectBox.Height() ;
		// Conversion en pixels
		pPointRef = new NS_CLASSLIB::TPoint(0, iTailleScroll) ;
		pDC->LPtoDP(pPointRef) ;
  	// Demande de scroll
		ScrollWindow(0, -pPointRef->y) ;
		delete pPointRef ;
    pPointRef = 0 ;
		// Ajustement des variables PremLigne et HautGcheFenetre
		PremLigne++ ;
		HautGcheFenetre.y += iTailleScroll ;
		// UpdateWindow() ;
	}
	else if (scrollCode == SB_PAGEDOWN)
  {
	  // Scroll vers le haut (page basse)
    NS_CLASSLIB::TRect rectClient = GetClientRect() ;
    int cyScroll = ::GetSystemMetrics(SM_CYHSCROLL) ;
    int hauteurLignes = 0 ;
    UINT numLigne = PremLigne ; // numéro de la première ligne visible
    int hauteurFen = rectClient.bottom - rectClient.top - cyScroll ; // hauteur fenetre en pixels
    int hBoite = 0, hLigne = 0 ;
    int tailleScroll = 0 ;
    while ((numLigne < aLignes.size()) && (hauteurLignes < hauteurFen))
    {
      pLigne = aLignes[numLigne - 1] ;
      NS_CLASSLIB::TRect rectBox = pLigne->getBox() ;
      hBoite = rectBox.Height() ;
      NS_CLASSLIB::TPoint py(0, hBoite) ;
      pDC->LPtoDP(&py) ;
      hLigne = py.y ;

      tailleScroll += hBoite ;
      hauteurLignes += hLigne ;
      numLigne++ ;
    }
    PremLigne = numLigne ;
    ScrollWindow(0, -hauteurLignes) ;
    HautGcheFenetre.y += tailleScroll ;
  }
	else if (scrollCode == SB_LINEUP)
	{
  	// Scroll vers le bas (flêche haute)

		// Calcul de la quantité à scroller (hauteur de l'avant-première ligne)
		pLigne = aLignes[PremLigne - 2] ;
    NS_CLASSLIB::TRect rectBox = pLigne->getBox() ;
		iTailleScroll = rectBox.Height() ;
		// Conversion en pixels
		pPointRef = new NS_CLASSLIB::TPoint(0, iTailleScroll) ;
		pDC->LPtoDP(pPointRef) ;
		// Demande de scroll
		ScrollWindow(0, pPointRef->y) ;
		delete pPointRef ;
    pPointRef = 0 ;
		// Ajustement des variables PremLigne et HautGcheFenetre
		PremLigne-- ;
		HautGcheFenetre.y -= iTailleScroll ;
		// UpdateWindow() ;
	}
	else if (scrollCode == SB_PAGEUP)
  {
	  // Scroll vers le bas (page haute)
    NS_CLASSLIB::TRect rectClient = GetClientRect() ;
    int cyScroll = ::GetSystemMetrics(SM_CYHSCROLL) ;
    int hauteurLignes = 0 ;
    int numLigne = PremLigne ; // numéro de la première ligne visible
    int hauteurFen = rectClient.bottom - rectClient.top - cyScroll ; // hauteur fenetre en pixels
    int hBoite = 0, hLigne = 0 ;
    int tailleScroll = 0 ;
    while ((numLigne > 1) && (hauteurLignes < hauteurFen))
    {
      pLigne = aLignes[numLigne - 2] ;  // Ligne précédente
      NS_CLASSLIB::TRect rectBox = pLigne->getBox() ;
      hBoite = rectBox.Height() ;
      NS_CLASSLIB::TPoint py(0, hBoite) ;
      pDC->LPtoDP(&py) ;
      hLigne = py.y ;
      tailleScroll += hBoite ;
      hauteurLignes += hLigne ;
      numLigne-- ;
    }
    PremLigne = numLigne ;
    ScrollWindow(0, hauteurLignes) ;
    HautGcheFenetre.y -= tailleScroll ;
  }
	delete pDC ;
  pDC = 0 ;
	SetScrollPos(SB_VERT, PremLigne) ;
}


// -----------------------------------------------------------------------------
// Scroll horizontal
// -----------------------------------------------------------------------------
void
NSMultiCaptView::EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)
{
	NS_CLASSLIB::TPoint* pPointRef;

	// On sort tout de suite si on est sur les lignes extrèmes
	if (((scrollCode == SB_LINERIGHT) && (HautGcheFenetre.x == RectangleGlobal.Width())) || ((scrollCode == SB_PAGERIGHT) && (HautGcheFenetre.x == RectangleGlobal.Width())) || ((scrollCode == SB_LINELEFT) && (HautGcheFenetre.x == 0)) || ((scrollCode == SB_PAGELEFT) && (HautGcheFenetre.x == 0)))
		return ;

	// On récupère un "contexte d'information" concernant l'écran
	TIC* pDC = new TIC("DISPLAY", NULL, NULL) ;
	pDC->SetMapMode(MM_LOMETRIC) ;

	// Scroll vers la gauche (flêche droite)
	if (scrollCode == SB_LINERIGHT)
	{
		// Conversion en pixels
		pPointRef = new NS_CLASSLIB::TPoint(LargeurPolice, 0) ;
		pDC->LPtoDP(pPointRef) ;
		// Demande de scroll
		ScrollWindow(-pPointRef->x, 0) ;
		delete pPointRef ;
    pPointRef = 0 ;
		// Ajustement de HautGcheFenetre
		HautGcheFenetre.x += LargeurPolice ;
	}

	// Scroll vers la gauche (page droite)
	if (scrollCode == SB_PAGERIGHT)
	{
		// Conversion en pixels
    int k = 1 ;
    pPointRef = new NS_CLASSLIB::TPoint(LargeurPolice, 0) ;
    pDC->LPtoDP(pPointRef) ;
    while ((k <= 10) && ((HautGcheFenetre.x + LargeurPolice) <= RectangleGlobal.Width()))
    {
      // Demande de scroll
      ScrollWindow(-pPointRef->x, 0) ;
      // Ajustement de HautGcheFenetre
      HautGcheFenetre.x += LargeurPolice ;
      k++ ;
    }
    delete pPointRef ;
    pPointRef = 0 ;
	}

	// Scroll vers la droite (flêche gauche)
	else if (scrollCode == SB_LINELEFT)
	{
		// Conversion en pixels
		pPointRef = new NS_CLASSLIB::TPoint(LargeurPolice, 0) ;
		pDC->LPtoDP(pPointRef) ;
		// Demande de scroll
		ScrollWindow(pPointRef->x, 0) ;
		delete pPointRef ;
    pPointRef = 0 ;
		// Ajustement de HautGcheFenetre
		HautGcheFenetre.x -= LargeurPolice ;
	}

	// Scroll vers la droite (page gauche)
	if (scrollCode == SB_PAGELEFT)
	{
		// Conversion en pixels
    int k = 1 ;
    pPointRef = new NS_CLASSLIB::TPoint(LargeurPolice, 0) ;
    pDC->LPtoDP(pPointRef) ;
    while ((k <= 10) && ((HautGcheFenetre.x - LargeurPolice) >= 0))
    {
      // Demande de scroll
      ScrollWindow(pPointRef->x, 0) ;
      // Ajustement de HautGcheFenetre
      HautGcheFenetre.x -= LargeurPolice ;
      k++ ;
    }
		delete pPointRef ;
    pPointRef = 0 ;
	}

	delete pDC ;
  pDC = 0 ;
  if (LargeurPolice > 0)
    SetScrollPos(SB_HORZ, HautGcheFenetre.x / LargeurPolice) ;
}


// -----------------------------------------------------------------------------
// Molette
// -----------------------------------------------------------------------------
LRESULT
NSMultiCaptView::EvMouseWheel(WPARAM wParam, LPARAM lParam)
{
  // WORD    fwKeys  = LOWORD(wParam) ;           // key flags
  short   zDelta  = (short) HIWORD(wParam) ;   // wheel rotation
  // short   xPos    = (short) LOWORD(lParam) ;   // horizontal position of pointer
  // short   yPos    = (short) HIWORD(lParam) ;   // vertical position of pointer
  UINT    scrollCode;

  // A positive value indicates that the wheel was rotated forward, away from the user
  if (zDelta > 0)
    scrollCode = SB_LINEUP ;
  // a negative value indicates that the wheel was rotated backward, toward the user.
  else if (zDelta < 0)
    scrollCode = SB_LINEDOWN ;
  for (int i = 0 ; i < 4 ; i++)
    EvVScroll(scrollCode, 0, 0) ;
  return 0 ;
}


void
NSMultiCaptView::reloadView(string sReason)
{
}


void
NSMultiCaptView::concernChanged(string sConcern)
{
}


size_t
NSMultiCaptView::getMaxLine()
{
	if (aLignes.empty())
		return 0 ;

  return aLignes.size() ;
}


CAPTHIT
NSMultiCaptView::HitTest(NS_CLASSLIB::TPoint& point, bool * pCaretBefore)
{
  CAPTHIT hitPos ;
  hitPos.setLigne(-1) ;
  hitPos.setPos(0) ;
  hitPos.setInside(false) ;

try
{
  if (aLignes.empty())
    return hitPos ;

	NS_CLASSLIB::TPoint ptClick = point ;

	// On récupère un "contexte d'information" concernant l'écran
	TIC * pDC = new TIC("DISPLAY", NULL, NULL) ;
	pDC->SetMapMode(MM_LOMETRIC) ;

  ptClick = screenPointToCaptPoint(ptClick, pDC) ;

	// On cherche si ce point se situe sur une ligne
	// Attention, les ordonnées sont < 0
  int ligneEnCours = 1 ;

  CaptLineIter cptIter = aLignes.begin() ;

  // Attention, y est négatif en MapMode MM_LOMETRIC
  // Warning, y is negative when using MM_LOMETRIC MapMode
  for (ligneEnCours = 0 ; cptIter != aLignes.end() ; cptIter++, ligneEnCours++)
  {
    NS_CLASSLIB::TRect rectBox = (*cptIter)->getBox() ;
    if ((ptClick.x >= rectBox.left) && (ptClick.x <= rectBox.right) && (ptClick.y <= rectBox.top) && (ptClick.y >= rectBox.bottom))
      break ;
  }

  // We are not in any line "box", we now check if we are somewhere
  //
  if (cptIter == aLignes.end())
  {
  	cptIter = aLignes.begin() ;
    NS_CLASSLIB::TRect rectBox = (*cptIter)->getBox() ;
    //
    // Above first line
    //
    if (ptClick.y > rectBox.top)
    {
    	delete pDC ;
    	return hitPos ;
		}

  	for (ligneEnCours = 0 ; cptIter != aLignes.end() ; cptIter++, ligneEnCours++)
  	{
    	rectBox = (*cptIter)->getBox() ;
    	if ((ptClick.y <= rectBox.top) && (ptClick.y >= rectBox.bottom))
      	break ;
  	}
    hitPos.setLigne(ligneEnCours) ;
    //
    // On the right of a line
    //
    if ((cptIter != aLignes.end()) && (ptClick.x > rectBox.right))
		{
    	string sTexte = (*cptIter)->getText() ;
			hitPos.setPos(strlen(sTexte.c_str())) ;
		}
		delete pDC ;
		return hitPos ;
  }
  hitPos.setLigne(ligneEnCours) ;
  hitPos.setInside(true) ;

  // Dans cette ligne, on cherche à quelle position on est
  NS_CLASSLIB::TRect rectBox = (*cptIter)->getBox() ;
  NS_CLASSLIB::TPoint pointInBox = NS_CLASSLIB::TPoint(ptClick.x - rectBox.left, ptClick.y - rectBox.top) ;
  NSFont PoliceVoulue ;
  if (!(getFont((*cptIter)->iStatus, &PoliceVoulue)))
  {
    delete pDC ;
    return hitPos ;
  }

  if (PoliceVoulue.logFont.lfHeight >= 0)
  {
    NS_CLASSLIB::TPoint	PointTransfert ;
    PointTransfert.x = PoliceVoulue.logFont.lfHeight ;
    PointTransfert.y = PoliceVoulue.logFont.lfWidth ;
    pDC->DPtoLP(&PointTransfert) ;
    PoliceVoulue.logFont.lfHeight = PointTransfert.x ;
    PoliceVoulue.logFont.lfWidth  = PointTransfert.y ;
  }

  TFont * pPolice = new TFont(&(PoliceVoulue.logFont)) ;
  pDC->SelectObject(*pPolice) ;
  TEXTMETRIC  TextMetric ;
  // LONG        lPolicetmHeight = 0 ;
  if (pDC->GetTextMetrics(TextMetric))
  {
    if ((LargeurPolice == 0) || (LargeurPolice > TextMetric.tmAveCharWidth))
      LargeurPolice = TextMetric.tmAveCharWidth ;
    // lPolicetmHeight = TextMetric.tmHeight + TextMetric.tmDescent ;
  }

  size_t  iMinLetterIndex = 0 ;
  long    lMinPosition    = 0 ;

  string  sTexte = (*cptIter)->getText() ;
  NS_CLASSLIB::TSize TailleChaine = pDC->GetTextExtent(sTexte.c_str(), strlen(sTexte.c_str())) ;
  size_t  iMaxLetterIndex = strlen(sTexte.c_str()) ;
  long    lMaxposition    = TailleChaine.X() ;
  bool bTourner = true ;

  if ((pointInBox.x < lMinPosition) || (pointInBox.x > lMaxposition))
    bTourner = false ;

  // On se positionne entre Min et Max, à peu près sur la lettre voulue
  // le nouveau point remplace min ou max selon sa position réelle
  // We make a test between Min and Max, as close as possible from the target
  // char, then we see if the actual reached point replaces min or max
  while (bTourner)
  {
    double dDelta = double(pointInBox.x - lMinPosition) / double(lMaxposition - lMinPosition) ;
    double dIndex = dDelta * double(iMaxLetterIndex - iMinLetterIndex) + double(iMinLetterIndex) ;
    size_t iNewIndex = dtoi(dIndex) ;
    if ((iNewIndex == iMinLetterIndex) && (iMinLetterIndex > 0))
      iNewIndex++ ;
    if (iNewIndex == iMaxLetterIndex)
      iNewIndex-- ;

    sTexte = string((*cptIter)->getText(), 0, iNewIndex + 1) ;
    NS_CLASSLIB::TSize TailleChaine = pDC->GetTextExtent(sTexte.c_str(), strlen(sTexte.c_str())) ;
    if (TailleChaine.X() == pointInBox.x)
    {
      hitPos.setPos(iNewIndex) ;
      bTourner = false ;
    }
    else if (TailleChaine.X() > pointInBox.x)
    {
      iMaxLetterIndex = iNewIndex ;
      lMaxposition    = TailleChaine.X() ;
      if ((iMaxLetterIndex == iMinLetterIndex) || (iMaxLetterIndex == iMinLetterIndex + 1))
      {
        hitPos.setPos(iMaxLetterIndex) ;
        bTourner = false ;
      }
    }
    else if (TailleChaine.X() < pointInBox.x)
    {
      iMinLetterIndex = iNewIndex ;
      lMinPosition    = TailleChaine.X() ;

      if ((iMaxLetterIndex == iMinLetterIndex) || (iMaxLetterIndex == iMinLetterIndex + 1))
      {
        hitPos.setPos(iMaxLetterIndex) ;
        bTourner = false ;
      }
    }
  }

  if (pCaretBefore)
  {
  	// default value : Caret is before the char
    //
  	*pCaretBefore = true ;

  	size_t iSelected = hitPos.getPos() ;
    sTexte = (*cptIter)->getText() ;
    if (iSelected < strlen(sTexte.c_str()))
		{
			sTexte = string((*cptIter)->getText(), 0, iSelected + 1) ;
    	NS_CLASSLIB::TSize TailleChaine = pDC->GetTextExtent(sTexte.c_str(), strlen(sTexte.c_str())) ;
    	lMaxposition = TailleChaine.X() ;

    	sTexte = string(1, sTexte[iSelected]) ;
    	TailleChaine = pDC->GetTextExtent(sTexte.c_str(), strlen(sTexte.c_str())) ;
    	lMinPosition = lMaxposition - TailleChaine.X() ;

    	if (pointInBox.x - lMinPosition > lMaxposition - pointInBox.x)
      	*pCaretBefore = false ;
		}
  }

  delete pPolice ;
  delete pDC ;

  return hitPos ;
}
catch (...)
{
	erreur("Exception NSMultiCaptView::HitTest.", standardError, 0) ;
	return hitPos ;
}
}


bool
NSMultiCaptView::inSelectedArea(CAPTHIT hitPoint)
{
  if ((!bSelected) || (hitPoint.getLigne() < 0))
    return false ;

  if ((hitPoint.getLigne() < firstSelPoint.getLigne()) ||
  	  (hitPoint.getLigne() > lastSelPoint.getLigne()) ||
      ((hitPoint.getLigne() == firstSelPoint.getLigne()) && (hitPoint.getPos() < firstSelPoint.getPos())) ||
      ((hitPoint.getLigne() == lastSelPoint.getLigne()) && (hitPoint.getPos() > lastSelPoint.getPos())))
    return false ;

  return true ;
}


void
NSMultiCaptView::setCaretPosition()
{
try
{
  if (captCaret.getLigne() < 0)
    return ;

  CaptLineIter iterLigne = aLignes.getLine(captCaret.getLigne()) ;
  if (!iterLigne || (iterLigne == aLignes.end()))
    return ;

  NS_CLASSLIB::TRect rectBox = (*iterLigne)->getBox() ;

  // Basic position : left side of the box
  NS_CLASSLIB::TPoint	caretPoint(rectBox.X() - HautGcheFenetre.x, rectBox.Y() - HautGcheFenetre.y) ;

  // On récupère un "contexte d'information" concernant l'écran
  TIC * pDC = new TIC("DISPLAY", NULL, NULL) ;
  pDC->SetMapMode(MM_LOMETRIC) ;

  if (captCaret.getPos() == 0)
  {
    pDC->LPtoDP(&caretPoint) ;
    SetCaretPos(caretPoint) ;
    delete pDC ;
    return ;
  }

  NSFont PoliceVoulue ;
  if (!(getFont((*iterLigne)->iStatus, &PoliceVoulue)))
    return ;

  // On regarde où se situe captcaret
  if (PoliceVoulue.logFont.lfHeight >= 0)
  {
    NS_CLASSLIB::TPoint	PointTransfert ;
    PointTransfert.x = PoliceVoulue.logFont.lfHeight ;
    PointTransfert.y = PoliceVoulue.logFont.lfWidth ;
    pDC->DPtoLP(&PointTransfert) ;
    PoliceVoulue.logFont.lfHeight = PointTransfert.x ;
    PoliceVoulue.logFont.lfWidth  = PointTransfert.y ;
  }

  TFont* pPolice = new TFont(&(PoliceVoulue.logFont)) ;
  pDC->SelectObject(*pPolice) ;

  SIZE    lpSize ;
  string  sTexte = string((*iterLigne)->getText(), 0, captCaret.getPos()) ;
  BOOL Continuer = ::GetTextExtentPoint32(pDC->GetHDC(), sTexte.c_str(), strlen(sTexte.c_str()), &lpSize) ;
  if (Continuer)
  {
    caretPoint.x += lpSize.cx ;
    pDC->LPtoDP(&caretPoint) ;
    SetCaretPos(caretPoint) ;
  }

  delete pPolice ;
  delete pDC ;

  return ;
}
catch (...)
{
	erreur("Exception NSMultiCaptView::setCaretPosition.", standardError, 0) ;
	return ;
}
}

//
// Update firstSelPoint and lastSelPoint from selectingRefPoint and captCaret
//
void
NSMultiCaptView::updateSelection()
{
	if (!bSelected)
		return ;

	//
	// Warning : since the caret is before selectingRefPoint, if we are
	//           selecting on the left, selectingRefPoint must be excluded from
	//           the selected area
	//

  // nothing selected
	if (captCaret == selectingRefPoint)
	{
    bSelected = false ;

		firstSelPoint = selectingRefPoint ;
		lastSelPoint  = selectingRefPoint ;
	}
  // something selected
	else
  {
  	bSelected = true ;

  	// Selecting beyond refPoint
  	if (captCaret > selectingRefPoint)
		{
			firstSelPoint = selectingRefPoint ;
			lastSelPoint  = previousPoint(captCaret) ;
		}
		// Selecting before refPoint
		else
		{
			firstSelPoint = captCaret ;
			lastSelPoint  = previousPoint(selectingRefPoint) ;
		}
	}
}

void
NSMultiCaptView::selecting()
{
	if (!bSelected)
		selectingRefPoint = captCaret ;

	bSelected = true ;
}

void
NSMultiCaptView::selectWord(CAPTHIT hitPoint)
{
  bSelected = false ;

  if (hitPoint.getLigne() < 0)
    return ;

  CaptLineIter cptIter = aLignes.getLine(hitPoint.getLigne()) ;
  if (cptIter == NULL)
    return ;

  string  sTexte      = (*cptIter)->getText() ;
  size_t  iTextLen    = strlen(sTexte.c_str()) ;
  if ((hitPoint.getPos() < iTextLen) && (sTexte[hitPoint.getPos()] == ' '))
    return ;

  bSelected = true ;
  firstSelPoint.setLigne(hitPoint.getLigne()) ;
  lastSelPoint.setLigne(hitPoint.getLigne()) ;

  if (hitPoint.getPos() >= iTextLen)
  {
    firstSelPoint.setPos(0) ;
    lastSelPoint.setPos(iTextLen - 1) ;
    return ;
  }

  firstSelPoint.setPos(hitPoint.getPos()) ;
  while ((firstSelPoint.getPos() > 0) && (sTexte[firstSelPoint.getPos()-1] != ' '))
  	firstSelPoint.precPos() ;

  lastSelPoint.setPos(hitPoint.getPos()) ;
  while ((lastSelPoint.getPos() < iTextLen-1) && (sTexte[lastSelPoint.getPos()+1] != ' '))
    lastSelPoint.nextPos() ;
}


void
NSMultiCaptView::selectSentence(CAPTHIT hitPoint)
{
  bSelected = false ;

  if (hitPoint.getLigne() < 0)
    return ;

  CaptLineIter cptIter = aLignes.getLine(hitPoint.getLigne()) ;
  if (cptIter == NULL)
    return ;

  string  sTexte      = (*cptIter)->getText() ;
  size_t  iTextLen    = strlen(sTexte.c_str()) ;

  bSelected = true ;
  firstSelPoint.setLigne(hitPoint.getLigne()) ;
  lastSelPoint.setLigne(hitPoint.getLigne()) ;

  if (hitPoint.getPos() >= iTextLen)
  {
    firstSelPoint.setPos(0) ;
    lastSelPoint.setPos(iTextLen - 1) ;
    return ;
  }

  firstSelPoint.setPos(hitPoint.getPos()) ;
  while ((firstSelPoint.getPos() > 0) && (sTexte[firstSelPoint.getPos()-1] != '.'))
    firstSelPoint.precPos() ;

  lastSelPoint.setPos(hitPoint.getPos()) ;
  while ((lastSelPoint.getPos() < iTextLen-1) && (sTexte[lastSelPoint.getPos()+1] != '.'))
    lastSelPoint.nextPos() ;
}


void
NSMultiCaptView::selectParagraph(CAPTHIT hitPoint)
{
}


void
NSMultiCaptView::selectCaptureSet(CAPTHIT hitPoint)
{
}


void
NSMultiCaptView::selectAll(CAPTHIT hitPoint)
{
}

CAPTHIT
NSMultiCaptView::previousPoint(CAPTHIT refPoint)
{
	CAPTHIT point = refPoint ;
  size_t iPtPos = point.getPos() ;

	if (iPtPos > 0)
		point.setPos(iPtPos - 1) ;
	else
	{
  	int iPtLigne = point.getLigne() ;
    if (iPtLigne > 0)
    {
    	iPtLigne-- ;
			point.setLigne(iPtLigne) ;
			CaptLineIter caretIter = aLignes.getLine(iPtLigne) ;
      //
      // Warning : since caret is before the point, the last point of a line
      //           is beyond the last caracter of the line
      //
			if ((caretIter != NULL) && (caretIter != aLignes.end()))
				point.setPos((*caretIter)->getTextLen()) ;
		}
	}
  return point ;
}

CAPTHIT
NSMultiCaptView::nextPoint(CAPTHIT refPoint)
{
	CAPTHIT point = refPoint ;

	CaptLineIter caretIter = aLignes.getLine(point.getLigne()) ;
  if ((caretIter == NULL) || (caretIter == aLignes.end()))
		return point ;

	size_t iPtPos = point.getPos() ;

	// dernier caractère de la ligne ?
	if (iPtPos >= (*caretIter)->getTextLen())
	{
		// dernière ligne
		if (captCaret.getLigne() < int(getMaxLine()) - 1)
		{
			point.nextLigne() ;
			point.resetPos() ;
		}
	}
  else
  	point.nextPos() ;

	return point ;
}

CAPTHIT
NSMultiCaptView::firstLinePoint(CAPTHIT refPoint)
{
	CAPTHIT point = refPoint ;

	point.resetPos() ;

	return point ;
}

CAPTHIT
NSMultiCaptView::lastLinePoint(CAPTHIT refPoint)
{
	CAPTHIT point = refPoint ;

	CaptLineIter caretIter = aLignes.getLine(point.getLigne()) ;
  if ((caretIter == NULL) || (caretIter == aLignes.end()))
		return point ;

	point.setPos((*caretIter)->getTextLen()) ;

	return point ;
}

CAPTHIT
NSMultiCaptView::upPoint(CAPTHIT refPoint)
{
	CAPTHIT point = refPoint ;

	int iLigne = point.getLigne() ;
  if (iLigne == 0)
		return point ;

	CaptLineIter caretIter = aLignes.getLine(iLigne - 1) ;
  if ((caretIter == NULL) || (caretIter == aLignes.end()))
		return point ;

	NS_CLASSLIB::TPoint	caretPoint ;
	GetCaretPos(caretPoint) ;

  NS_CLASSLIB::TRect BoiteNormale = (*caretIter)->getBox() ;
	BoiteNormale.Normalized();
  NS_CLASSLIB::TPoint	centerPoint(BoiteNormale.Left() + (BoiteNormale.Right() - BoiteNormale.Left()) / 2, BoiteNormale.Top() + (BoiteNormale.Bottom() - BoiteNormale.Top()) / 2) ;
  centerPoint = captPointToScreenPoint(centerPoint) ;

	caretPoint = NS_CLASSLIB::TPoint(caretPoint.X(), centerPoint.Y()) ;

  bool bCaretBefore ;
	CAPTHIT capt = HitTest(caretPoint, &bCaretBefore) ;
  if (!(capt.getInside()))
  {
  	int iCaptLigne = capt.getLigne() ;
  	if ((iCaptLigne == -1) || (iCaptLigne >= int(getMaxLine()) - 1))
  		return point ;

    if (capt.getPos() > 0)
			return lastLinePoint(capt) ;
  }

  point = capt ;
  if (!bCaretBefore)
  	point.nextPos() ;

	return point ;
}

CAPTHIT
NSMultiCaptView::downPoint(CAPTHIT refPoint)
{
	CAPTHIT point = refPoint ;

	int iLigne = point.getLigne() ;
  if (iLigne >= int(getMaxLine()) - 1)
		return point ;

	CaptLineIter caretIter = aLignes.getLine(iLigne + 1) ;
  if ((caretIter == NULL) || (caretIter == aLignes.end()))
		return point ;

	NS_CLASSLIB::TPoint	caretPoint ;
	GetCaretPos(caretPoint) ;

  NS_CLASSLIB::TRect BoiteNormale = (*caretIter)->getBox() ;
	BoiteNormale.Normalized();
  NS_CLASSLIB::TPoint	centerPoint(BoiteNormale.Left() + (BoiteNormale.Right() - BoiteNormale.Left()) / 2, BoiteNormale.Top() + (BoiteNormale.Bottom() - BoiteNormale.Top()) / 2) ;
  centerPoint = captPointToScreenPoint(centerPoint) ;

	caretPoint = NS_CLASSLIB::TPoint(caretPoint.X(), centerPoint.Y()) ;

  bool bCaretBefore ;
	CAPTHIT capt = HitTest(caretPoint, &bCaretBefore) ;
  if (!(capt.getInside()))
  {
  	int iCaptLigne = capt.getLigne() ;
  	if ((iCaptLigne == -1) || (iCaptLigne >= int(getMaxLine()) - 1))
  		return point ;

    if (capt.getPos() > 0)
			return lastLinePoint(capt) ;
  }

  point = capt ;
  if (!bCaretBefore)
  	point.nextPos() ;

	return point ;
}

string
NSMultiCaptView::getSelText()
{
  string sSelection = "" ;

	UINT i = 0 ;
  if (firstSelPoint.getLigne() >= 0)
  	i = firstSelPoint.getLigne() ;

  for ( ; int(i) <= lastSelPoint.getLigne() ; i++)
  {
    CaptLineIter cptIter = aLignes.getLine(i) ;
    if (cptIter != NULL)
    {
      string sTexte = (*cptIter)->getText() ;
      if ((int(i) == firstSelPoint.getLigne()) && (int(i) == lastSelPoint.getLigne()))
        sSelection = string(sTexte, firstSelPoint.getPos(), lastSelPoint.getPos() - firstSelPoint.getPos() + 1) ;
      else if (int(i) == firstSelPoint.getLigne())
        sSelection = string(sTexte, firstSelPoint.getPos(), strlen(sTexte.c_str()) - firstSelPoint.getPos()) ;
      else if (int(i) == lastSelPoint.getLigne())
        sSelection += string(sTexte, 0, lastSelPoint.getPos() + 1) ;
      else
        sSelection += sTexte ;
    }
  }

  return sSelection ;
}

analysedCapture *
NSMultiCaptView::getSelCapture()
{
  analysedCapture * pCaptured = NULL ;

	UINT i = 0 ;
  if (firstSelPoint.getLigne() >= 0)
  	i = firstSelPoint.getLigne() ;

  for ( ; int(i) <= lastSelPoint.getLigne() ; i++)
  {
    CaptLineIter cptIter = aLignes.getLine(i) ;
    if (!pCaptured)
      pCaptured = (*cptIter)->pCapturedElement ;
    else if (pCaptured != (*cptIter)->pCapturedElement)
      return NULL ;
  }
  return pCaptured ;
}

void
NSMultiCaptView::EvPosit(int iPos)
{
try
{
	if (false == bSelected)
		return ;

	string sSelection = getSelText() ;
	if (sSelection == "")
		return ;

	NSEpisodus * pEpiso = pContexte->getEpisodus() ;

	PrinciplesArray * pPrincipes = pEpiso->pPrincipes ;
	if (pPrincipes->empty())
		return ;

	PrinciplesIter prIt = pPrincipes->begin() ;
	for (int i = 1 ; (prIt != pPrincipes->end()) && (i < iPos) ; prIt++, i++)
    ;
	if (prIt == pPrincipes->end())
		return ;

	string sChemin = string("GCONS1/") + (*prIt)->sCase ;

#ifndef _EXT_CAPTURE
	NSCapture * pCapt = new NSCapture(pContexte, sChemin, sSelection, FromOutside, (*prIt)->sClassification) ;
#else
	NSCapture * pCapt = new NSCapture(sChemin, sSelection, FromOutside, (*prIt)->sClassification) ;
#endif

	// Ajout de l'objet à l'array de capture / Adding the object to capture array
	if (pCapt)
	{
		pEpiso->newCaptureArray.ajouter(pCapt) ;
		// Ajout au réservoir de SOAP
		// Adding to SOAPing tank
		// if (pCapt && (pCapt->sClassifier != ""))
		//	pEpiso->addToSOAP(pCapt) ;
	}

	// On prévient le système qu'une nouvelle capture a été effectuée
	// Warning the system that a new capture occured
	/* bool bCapt = */ pContexte->captureData(&(pEpiso->newCaptureArray)) ;
}
catch (...)
{
	erreur("Exception NSMultiCaptView::EvPosit.", standardError, 0) ;
}
}


voidNSMultiCaptView::EvCode()
{
try
{
	if (false == bSelected)
		return ;

	string sSelection = getSelText() ;
	if (string("") == sSelection)
		return ;

#ifndef _EXT_CAPTURE
	NSCapture * pCapt = new NSCapture(pContexte, string(""), sSelection, FromOutside, string("")) ;
#else
	NSCapture * pCapt = new NSCapture("", sSelection, FromOutside, "") ;
#endif

	NSEpisodus * pEpiso = pContexte->getEpisodus() ;

	// Ajout de l'objet à l'array de capture / Adding the object to capture array	if (pCapt)
	{
		pEpiso->newCaptureArray.ajouter(pCapt) ;

		// Ajout au réservoir de SOAP
		// Adding to SOAPing tank
		if (pCapt && (pCapt->getClassifier() != string("")))
			pEpiso->addToSOAP(pCapt) ;
	}

	// On prévient le système qu'une nouvelle capture a été effectuée
	// Warning the system that a new capture occured
	/* bool bCapt = */ pContexte->captureData(&(pEpiso->newCaptureArray)) ;
}
catch (...)
{
	erreur("Exception NSMultiCaptView::EvCode.", standardError, 0) ;
}
}


void
NSMultiCaptView::EvOcrLearn()
{
try
{
	if (false == bSelected)
		return ;

	analysedCapture* pCaptured = getSelCapture() ;
	if (((analysedCapture*) NULL == pCaptured) ||
      ((NSOCRbloc*)       NULL == pCaptured->getOcrBloc()))
		return ;

#ifndef _EXT_CAPTURE
	NSCapture * pCapt = new NSCapture(pContexte, pCaptured->getOcrBloc()) ;
#else
	NSCapture * pCapt = new NSCapture(pCaptured->getOcrBloc()) ;
#endif

	// Ajout de l'objet à l'array de capture / Adding the object to capture array
	if (pCapt)
	{
		NSEpisodus * pEpiso = pContexte->getEpisodus() ;

		pEpiso->newCaptureArray.ajouter(pCapt) ;

		// Ajout au réservoir de SOAP
		// Adding to SOAPing tank
		if (pCapt && (pCapt->getClassifier() != string("")))
			pEpiso->addToSOAP(pCapt) ;
	}
}
catch (...)
{
	erreur("Exception NSMultiCaptView::EvOcrLearn.", standardError, 0) ;
}
}

NS_CLASSLIB::TPoint
NSMultiCaptView::screenPointToCaptPoint(NS_CLASSLIB::TPoint& point, TIC* pDC)
{
	NS_CLASSLIB::TPoint captPoint(0, 0) ;

try
{
	bool bManageDC ;
  TIC * pScreenDC ;
  if (!pDC)
	{
		// On récupère un "contexte d'information" concernant l'écran
		pScreenDC = new TIC("DISPLAY", NULL, NULL) ;
    pScreenDC->SetMapMode(MM_LOMETRIC) ;
		bManageDC = true ;
	}
	else
	{
		pScreenDC = pDC ;
  	bManageDC = false ;
	}

  captPoint = point ;

	// On transforme les unités physiques d'écran en unités logiques
	pScreenDC->DPtoLP(&captPoint) ;

	if (bManageDC)
		delete pScreenDC ;

	// On positionne le point dans la page
	captPoint.x += HautGcheFenetre.x ;
	captPoint.y += HautGcheFenetre.y ;

	return captPoint ;
}
catch (...)
{
	erreur("Exception NSMultiCaptView::screenPointToCaptPoint.", standardError, 0) ;
  return captPoint ;
}
}

NS_CLASSLIB::TPoint
NSMultiCaptView::captPointToScreenPoint(NS_CLASSLIB::TPoint& point, TIC* pDC)
{
	NS_CLASSLIB::TPoint screenPoint = point ;

try
{
	// On positionne le point dans la page
	screenPoint.x -= HautGcheFenetre.x ;
	screenPoint.y -= HautGcheFenetre.y ;

	bool bManageDC ;
  TIC * pScreenDC ;
  if (!pDC)
	{
		// On récupère un "contexte d'information" concernant l'écran
		pScreenDC = new TIC("DISPLAY", NULL, NULL) ;
    pScreenDC->SetMapMode(MM_LOMETRIC) ;
		bManageDC = true ;
	}
	else
	{
  	pScreenDC = pDC ;
  	bManageDC = false ;
	}

	// On transforme les unités physiques d'écran en unités logiques
	pScreenDC->LPtoDP(&screenPoint) ;

	if (bManageDC)
		delete pScreenDC ;

	return screenPoint ;
}
catch (...)
{
	erreur("Exception NSMultiCaptView::captPointToScreenPoint.", standardError, 0) ;
  return screenPoint ;
}
}

