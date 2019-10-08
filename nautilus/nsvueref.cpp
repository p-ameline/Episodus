// -----------------------------------------------------------------------------// nsvueref.cpp
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// $Revision: 1.7 $
// $Author: pameline $
// $Date: 2013/05/01 21:29:44 $
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
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
#include <owl\owlpch.h>#include <owl\dc.h>
#include <owl\inputdia.h>
#include <owl\chooseco.h>
#include <owl\gdiobjec.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>
#include <owl\printer.h>
#include <classlib\arrays.h>
#include <iostream.h>
#include <cstring.h>
#include <owl/controlb.h>
#include <owl/buttonga.h>
#include <owl/oleview.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"

#include "nautilus\nsvueref.h"
#include "nautilus\nscrdoc.h"		// pour récupérer sFichDecod
#include "nautilus\nsmdiframe.h"

#include "nsbb\nsattvaltools.h"

#define MAXSCREENRES 10000

// -----------------------------------------------------------------------------
//
// class NSRefVue
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSRefVue, TWindowView)
  // EV_WM_SYSCOMMAND,
  // EV_WM_SYSKEYDOWN,
  EV_WM_KEYDOWN,
  EV_WM_VSCROLL,
  EV_WM_HSCROLL,
  EV_MESSAGE(WM_MOUSEWHEEL, EvMouseWheel),
  EV_COMMAND(CM_IMPRIME, CmPublier),
  EV_COMMAND(CM_COMPOSE, CmComposer),  EV_COMMAND(CM_VISUAL, CmVisualiser),
END_RESPONSE_TABLE ;

// -----------------------------------------------------------------------------
// Function    : NSRefVue::NSRefVue(NSRefDocument& doc, TWindow* parent)
// Arguments   : doc 	: NSRefDocument à afficher
// Description : Constructeur
// Retour      : Rien
// -----------------------------------------------------------------------------
NSRefVue::NSRefVue(NSRefDocument& doc, NSContexte* pCtx, TWindow* parent)         :TWindowView(doc, parent), NSRoot(pCtx)
{
  Attr.AccelTable = REFVIEW_ACCEL ;

	_pDoc = &doc ;

	_HautGcheFenetre.x = _HautGcheFenetre.y = 0 ;
	_ToolBar 	 = (OWL::TControlBar*) 0 ;
	_pBigBoss  = (NSSmallBrother*) 0 ;
	_ImpDriver = string("") ;
	_ImpDevice = string("") ;
	_ImpOutput = string("") ;
	_LargeurPolice = 0 ;

  _iLigneEnCours = 0 ;
  _iLignePrec    = 0 ;
  _iLigneSelect  = 0 ;

  _pMUEViewMenu = (TMenuDescr*) 0 ;

  // Get an information DC for the screen
  //
	_pTIC = new TIC("DISPLAY", NULL, NULL) ;

  // Switch to a mode where each logical unit is mapped to one device pixel.
  // (Positive x is to the right, positive y is down)
  //
  _pTIC->SetMapMode(MM_TEXT) ;

  // Get screen resolution in pixels
  //
  _iScreenResW = _pTIC->GetDeviceCaps(HORZRES) ;
  _iScreenResH = _pTIC->GetDeviceCaps(VERTRES) ;

  string ps = string("Detected screen resolution: ") + IntToString(_iScreenResW) + string("*") + IntToString(_iScreenResH) + string(" pixels") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubSteps) ;

  // Switch to a mode where each logical unit is mapped to 0.1 millimeter.
  // (Positive x is to the right, positive y is up)
  //
	_pTIC->SetMapMode(MM_LOMETRIC) ;

  _iFontForScreenResH = MAXSCREENRES ;
  _iFontForScreenResW = MAXSCREENRES ;
}

// -----------------------------------------------------------------------------
// Function    : NSRefVue::~NSRefVue()
// Description : Destructeur
// -----------------------------------------------------------------------------
NSRefVue::~NSRefVue()
{
	delete _pTIC ;
  _pTIC = (TIC*) 0 ;

  if (_pMUEViewMenu)
  	delete _pMUEViewMenu ;

  if (_pBigBoss)
  {
    delete _pBigBoss ;
    _pBigBoss = (NSSmallBrother*) 0 ;
  }

  if (0 != _hAccelerator)
  {
    DestroyAcceleratorTable(_hAccelerator) ;
    _hAccelerator = 0 ;
  }
}

// -----------------------------------------------------------------------------// Fonction    : NSRefVue::SetupWindow()// Arguments   : Aucun
// Description : Initialise la fenêtre
// Retour      : Rien
// -----------------------------------------------------------------------------
void NSRefVue::SetupWindow()
{
	// Préparation des éléments de mise en page du document
	InitAspect('N') ;
}

bool
NSRefVue::PreProcessMsg(MSG &msg)
{
  PRECONDITION(GetHandle()) ;
  return _hAccelerator ? ::TranslateAccelerator(GetHandle(), _hAccelerator, &msg) : false ;
}

// -----------------------------------------------------------------------------
// Function    : NSRefVue::InitAspect(char type)
// Arguments   : Aucun
// Description : Initialise les paraètres de mise en page du document
// Returns     : true/false
// -----------------------------------------------------------------------------
bool
NSRefVue::InitAspect(char type)
{
	// Récupère les données concernant l'imprimante
	if (!InitialiseImpData())
  	return false ;

	// Initialise les styles de paragraphes et les polices à partir de N.MOD
	if (!InitialiseMode(type))
  	return false ;

	if (!InitialisePolicesImp())
  	return false ;

	return true ;
}


// -----------------------------------------------------------------------------// Function    : NSRefVue::Paint(TDC& dc, bool, TRect& RectAPeindre)// Arguments   :	dc 						: device contexte de la fenêtre
//					  		RectAPeindre	: Rectangle à repeindre
// Description : Peint/repeint tout/une partie de la fenêtre
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSRefVue::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
	// Iterates through the array of line objects.
	const   NSLigne Ligne ;
  int     minLigne, maxLigne ;
	NS_CLASSLIB::TRect*  pRectDocument ;
	NS_CLASSLIB::TRect	  BoiteNormale ;
	NS_CLASSLIB::TPoint  Points[2] ;
	int	  iAncDC ;
	iAncDC = dc.SaveDC() ;

	// Fixe la métrique et l'origine
	dc.SetMapMode(MM_LOMETRIC) ;

	// Stockage des points supérieurs gauches et inférieurs droits du rectangle
	Points[0] = RectAPeindre.TopLeft() ;
	Points[1] = RectAPeindre.BottomRight() ;

	// Transformation de ces points en coordonnées logiques
	dc.DPtoLP(Points, 2) ;

	// Calcul du rectangle du document qui correspond au RectAPeindre de
	// l'écran
	// ATTENTION : dans le mode MM_LOMETRIC les coordonnées en ordonnées
	//				  sont négatives
	pRectDocument = new NS_CLASSLIB::TRect(Points[0], Points[1]) ;
	*pRectDocument = pRectDocument->Offset(_HautGcheFenetre.x, _HautGcheFenetre.y) ;
	pRectDocument->Normalized() ;

	// On demande à toutes les lignes concernées de se repeindre
  NSLignesArrayIter iter ;
  int ligneEnCours = 1 ;
	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;

  // calcul des lignes min et max de sélection
  if ((_iLigneEnCours + _iLigneSelect) < _iLigneEnCours)
  {
  	minLigne = _iLigneEnCours + _iLigneSelect;
    maxLigne = _iLigneEnCours;
  }
  else
  {
  	minLigne = _iLigneEnCours;
    maxLigne = _iLigneEnCours + _iLigneSelect;
  }

  // repeinte de la zone d'affichage
  for (iter = _Lignes.begin() ; _Lignes.end() != iter ; iter++, ligneEnCours++)
	{
		BoiteNormale = (*iter)->Boite ;
		BoiteNormale.Normalized() ;
		if ((BoiteNormale.right  >= pRectDocument->left)  &&
			 	(BoiteNormale.left   <= pRectDocument->right) &&
			 	(BoiteNormale.bottom <= pRectDocument->top)   &&
			 	(BoiteNormale.top 	 >= pRectDocument->bottom))
		//  ATTENTION : la méthode Touches de TRect ne fonctionne pas
		//	if (pRectDocument->Touches(BoiteNormale))
    {
    	UINT iPhrase = (*iter)->iNumPhrase;
      NSCRPhrArrayIter it;
      for (it = pCRDoc->_aCRPhrases.begin(); (pCRDoc->_aCRPhrases.end() != it) && ((*it)->iNumero != iPhrase); it++)
      	;
      if (pCRDoc->_aCRPhrases.end() != it)
      {
      	if ((*it)->aLesions.empty())
        	(*iter)->PeindreCurseur(dc, &_HautGcheFenetre, this, NSLIGNEPNT_ECRAN, false) ;
        else
        	(*iter)->PeindreCurseur(dc, &_HautGcheFenetre, this, NSLIGNEPNT_ECRAN) ;
      }

      // Affichage du bloc sélectionné en inverse vidéo
      if ((ligneEnCours >= minLigne) && (ligneEnCours <= maxLigne))
      	(*iter)->Peindre(dc, &_HautGcheFenetre, this, NSLIGNEPNT_ECRAN, true) ;
      else
      	(*iter)->Peindre(dc, &_HautGcheFenetre, this, NSLIGNEPNT_ECRAN) ;
    }
	}
	delete pRectDocument ;
  pRectDocument = 0 ;
	dc.RestoreDC(iAncDC) ;
}

// -----------------------------------------------------------------------------
// Function    : NSRefVue::PaintPrnt(TDC& dc, bool, TRect& RectAPeindre)
// Arguments   : dc 				: device contexte de la fenêtre
//					  	 RectAPeindre : Rectangle à repeindre
// Description : Peint/repeint sur imprimante
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSRefVue::PaintPrnt(TDC& dc, bool, int page, NS_CLASSLIB::TRect& RectAPeindre)
{
	// Iterates through the array of line objects.
  //
	int iAncDC = dc.SaveDC() ;

	// Fixe la métrique ; par défaut c'est le pixel, mais le mode MM_LOMETRIC
	// est beaucoup plus commode
	dc.SetMapMode(MM_LOMETRIC) ;

	// Stockage des points supérieurs gauches et inférieurs droits du rectangle
  //
  NS_CLASSLIB::TPoint Points[2] ;

	Points[0] = RectAPeindre.TopLeft() ;
	Points[1] = RectAPeindre.BottomRight() ;

	// Transformation de ces points en coordonnées logiques
  //
	dc.DPtoLP(Points, 2) ;

	// Calcul du rectangle du document qui correspond au RectAPeindre de
	// l'imprimante
	// ATTENTION : dans le mode MM_LOMETRIC les coordonnées en ordonnées
	//				     sont négatives
  //
	NS_CLASSLIB::TRect* pRectDocument = new NS_CLASSLIB::TRect(Points[0], Points[1]) ;
	*pRectDocument = pRectDocument->Offset(_HautGcheFenetre.x, _HautGcheFenetre.y) ;
	pRectDocument->Normalized() ;

	// On demande à toutes les lignes concernées de se repeindre
  //
  if (false == _LignesPrnt.empty())
    for (NSLignesArrayIter iter = _LignesPrnt.begin() ; _LignesPrnt.end() != iter ; iter++)
    {
      NS_CLASSLIB::TRect BoiteNormale = (*iter)->Boite ;
      BoiteNormale.Normalized() ;
      if (((*iter)->numPage == page) &&
          (BoiteNormale.right  >= pRectDocument->left)  &&
          (BoiteNormale.left   <= pRectDocument->right) &&
          (BoiteNormale.bottom <= pRectDocument->top)   &&
          (BoiteNormale.top 	 >= pRectDocument->bottom))
        //  ATTENTION : la méthode Touches de TRect ne fonctionne pas
        //	if (pRectDocument->Touches(BoiteNormale))
        (*iter)->Peindre(dc, &_HautGcheFenetre, this, NSLIGNEPNT_PRNT) ;
    }

	delete pRectDocument ;
	dc.RestoreDC(iAncDC) ;
}

#ifdef UNICODE
  #define GETDEFAULTPRINTER "GetDefaultPrinterW"
#else
  #define GETDEFAULTPRINTER "GetDefaultPrinterA"
#endif

// Size of internal buffer used to hold "printername,drivername,portname"
// string. You may have to increase this for huge strings.
#define MAXBUFFERSIZE 250

// -----------------------------------------------------------------------------
// Fonction    : NSRefVue::InitialiseImpData(void)
// Arguments   : Aucun
// Description : Initialise les variables concernant l'imprimante
// Retourne    : true/false
// -----------------------------------------------------------------------------
bool
NSRefVue::InitialiseImpData()
{
	string ps = string("Entering InitialiseImpData") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubSteps) ;

  _RectangleGlobal.left 	= 0 ;
	_RectangleGlobal.top  	= 0 ;
	_RectangleGlobal.right  = 2100 ;
	_RectangleGlobal.bottom = 2970 ;

  return true ;

  OSVERSIONINFO osv ;

  // What version of Windows are you running?
  osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osv);

  char szDevice[MAXBUFFERSIZE] ;
  char szDriver[MAXBUFFERSIZE] ;
  char szOutput[MAXBUFFERSIZE] ;

  // If Windows NT, use the GetDefaultPrinter API for Windows 2000,
  // or GetProfileString for version 4.0 and earlier.
  if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT)
  {
    ps = string("Detected platform Win 32 NT4 or later") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

    if (osv.dwMajorVersion >= 5) // Windows 2000 or later (use explicit call)
    {
      ps = string("Detected platform Windows 2000 or later") ;
      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

      TModule* pWinspoolModule = new TModule("winspool.drv", TRUE) ;
      if (!pWinspoolModule)
      {
        ps = string("Can't load winspool.drv") ;
        pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
        ps = string("InitialiseImpData returns false") ;
        pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubSteps) ;
        return false ;
      }

      BOOL (FAR* fnGetDefaultPrinter)( LPTSTR pszBuffer, LPDWORD pcchBuffer ) ;
      (FARPROC) fnGetDefaultPrinter = pWinspoolModule->GetProcAddress(GETDEFAULTPRINTER) ;
      if (!fnGetDefaultPrinter)
      {
        delete pWinspoolModule ;

        ps = string("Can't get default printer function from winspool.drv") ;
        pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
        ps = string("InitialiseImpData returns false") ;
        pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubSteps) ;
        return false ;
      }

      char  cBuffer[MAXBUFFERSIZE] ;
      DWORD wSize = MAXBUFFERSIZE ;

      bool bFlag = (*fnGetDefaultPrinter)(cBuffer, &wSize) ;
      if (!bFlag)
      {
        delete pWinspoolModule ;

        ps = string("Default printer function failed") ;
        pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
        ps = string("InitialiseImpData returns false") ;
        pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubSteps) ;
        return false ;
      }

      delete pWinspoolModule ;

      ps = string("Detected default printer: ") + string(cBuffer) ;
      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

      strcpy(szDevice, cBuffer) ;
      strcpy(szDriver, "winspool") ;
      strcpy(szOutput, "unknown") ;
    }

    else // NT4.0 or earlier
    {
      char cBuffer[MAXBUFFERSIZE] ;

      ps = string("Detected platform NT4 or earlier") ;
      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

      // Retrieve the default string from Win.ini (the registry).
      // String will be in form "printername,drivername,portname".
      if (GetProfileString("windows", "device", ",,,", cBuffer, MAXBUFFERSIZE) <= 0)
        return false ;

      char *szPtrDevice, *szPtrDriver, *szPtrOutput ;

      if (!(szPtrDevice = strtok(cBuffer, ",")) ||
          !(szPtrDriver = strtok(NULL, ", "))		 ||
          !(szPtrOutput = strtok(NULL, ", ")))
        return false ;

      strcpy(szDevice, szPtrDevice) ;
      strcpy(szDriver, szPtrDriver) ;
      strcpy(szOutput, szPtrOutput) ;
    }
  }
  // If Windows 95 or 98, use EnumPrinters.
  else
  {
	  char 	 szPrinter[64] ;
	  // Récupération de l'imprimante par défaut dans WIN.INI
	  GetProfileString("windows", "device", "", szPrinter, 64) ;

    char *szPtrDevice, *szPtrDriver, *szPtrOutput ;

    if (!(szPtrDevice = strtok(szPrinter, ",")) ||
      !(szPtrDriver = strtok(NULL, ", "))		 ||
      !(szPtrOutput = strtok(NULL, ", ")))
      return false ;

    strcpy(szDevice, szPtrDevice) ;
    strcpy(szDriver, szPtrDriver) ;
    strcpy(szOutput, szPtrOutput) ;
  }

	// Stockage des références de l'imprimante utilisée
	_ImpDriver = string(szDriver) ;
	_ImpDevice = string(szDevice) ;
	_ImpOutput = string(szOutput) ;
	//ptrInit(&ImpDriver, szDriver) ;
	//ptrInit(&ImpDevice, szDevice) ;
	//ptrInit(&ImpOutput, szOutput) ;

	// Prise d'un DC d'information
	TIC* pDICImprimante = new TIC(szDriver, szDevice, szOutput) ;

	// Initialisation du rectangle de délimitation d'une page à partir de
	// la taille de la feuille de papier
	pDICImprimante->SetMapMode(MM_LOMETRIC) ;
	_RectangleGlobal.left 	 = 0 ;
	_RectangleGlobal.top  	 = 0 ;

	// ATTENTION : GetDeviceCaps donne une taille en millimètres.
	//             Dans le mode MM_LOMETRIC, les y sont en négatif.
	_RectangleGlobal.right  = pDICImprimante->GetDeviceCaps(HORZSIZE) * 10 ;
	_RectangleGlobal.bottom = - pDICImprimante->GetDeviceCaps(VERTSIZE) * 10 ;
	delete pDICImprimante ;
  pDICImprimante = 0 ;
	return true ;
}

// -----------------------------------------------------------------------------
// Fonction    : NSRefVue::InitialisePolicesImp(void)
// Arguments   : Aucun
// Description : Initialise les variables concernant l'imprimante
// Retourne    : true/false
// -----------------------------------------------------------------------------
bool
NSRefVue::InitialisePolicesImp()
{
  for (FontIter i = _StylesPolice.begin() ; _StylesPolice.end() != i ; i++)
		_StylesPolImp.push_back(new NSFont(**i)) ;

  return true ;

  // si on a pas d'imprimante sélectionnée on sort
  if ((string("") == _ImpDriver) || (string("") == _ImpDevice) || (string("") == _ImpOutput))
  	return false ;

	NSFont* 	          pPoliceImprimante ;
	NSFont	            PoliceSouhaitee ;
	TFont*	            pPolice, pPoliceEcran ;
	NS_CLASSLIB::TPoint	PointTransfert ;

	// Création d'un objet de contexte d'information pour l'imprimante
	TIC* pDICImprimante = new TIC(_ImpDriver.c_str(), _ImpDevice.c_str(), _ImpOutput.c_str()) ;
	if (NULL == pDICImprimante)
		return false ;

	TIC* pDICEcran = new TIC("DISPLAY", NULL, NULL) ;
	if (NULL == pDICEcran)
	{
		delete pDICImprimante ;
		return false ;
	}

	// Pour le DIC d'imprimante, Twips et Twips Logiques sont identiques
	pDICImprimante->SetMapMode(MM_TWIPS) ;

	// On passe le DIC d'écran en mode topographique "Twips Logiques"
	pDICEcran->SetMapMode(MM_ANISOTROPIC) ;
	NS_CLASSLIB::TSize* pTaille = new NS_CLASSLIB::TSize(1440, 1440) ;
	pDICEcran->SetWindowExt(*pTaille) ;
	pTaille->cx = pDICEcran->GetDeviceCaps(LOGPIXELSX) ;
	pTaille->cy = pDICEcran->GetDeviceCaps(LOGPIXELSY) ;
	pDICEcran->SetViewportExt(*pTaille) ;
	delete pTaille ;
  pTaille = 0 ;

	// Pour chaque police demandée,
	// on récupère la police imprimante qui correspond
	for (FontIter i = _StylesPolice.begin() ; _StylesPolice.end() != i ; i++)
	{
		PoliceSouhaitee = *(*i) ;

		// La taille de la police, exprimée en points doit être convertie
		// en unité physiques (elle est d'abord * 20 pour avoir des twips)
		PoliceSouhaitee.logFont.lfHeight = PoliceSouhaitee.logFont.lfHeight * 20 ;
		PoliceSouhaitee.logFont.lfWidth 	= PoliceSouhaitee.logFont.lfWidth  * 20 ;

		// Création d'une TFont à partir de la NSFont contenue dans l'Array
		pPolice = new TFont(&(PoliceSouhaitee.logFont)) ;

		// Sélection de la Police pour le DC imprimante
		pDICImprimante->SelectObject(*pPolice) ;
		delete pPolice ;
    pPolice = 0 ;

		// Récupération de ses paramètres réels
		// la taille est récupérée par GetTextMetrics : donc en unité logique
		pPoliceImprimante = new NSFont(pDICImprimante) ;
		PointTransfert.x = pPoliceImprimante->logFont.lfHeight ;
		PointTransfert.y = pPoliceImprimante->logFont.lfWidth ;

		// Comme on veut des unités physiques d'écran, on convertit ces TWIPS
		// avec le DIC d'écran
		pDICEcran->LPtoDP(&PointTransfert) ;
		pPoliceImprimante->logFont.lfHeight = PointTransfert.x ;
		pPoliceImprimante->logFont.lfWidth  = PointTransfert.y ;

		// Ajout de l'objet à l'Array
		_StylesPolImp.push_back(new NSFont(*pPoliceImprimante)) ;
		delete pPoliceImprimante ;
    pPoliceImprimante = 0 ;
	}

	delete pDICImprimante ;
	delete pDICEcran ;
	return true ;
}

// -----------------------------------------------------------------------------// Fonction	   : NSRefVue::InitialiseMode(char type)// Arguments 	 : type : catégorie de modèle (N pour CR, M pour Magic)
// Description : Initialise le style du document à partir du fichier N.MOD
// Retourne	   : true/false
// -----------------------------------------------------------------------------
bool
NSRefVue::InitialiseMode(char type)
{
	char	 		   buffer[201] ;
	short	 		   etatEnCours ;
	size_t	 		   recherche ;

	NSFont*	 		       pPolice     = (NSFont*) 0 ;
	NSStyleParagraphe* pParagraphe = (NSStyleParagraphe*) 0 ;
	NSCadreDecor* 	   pCadre      = (NSCadreDecor*) 0 ;
  NSPage*			       pPage       = (NSPage*) 0 ;

  string			       sPathMod = pContexte->PathName("FPER") ;
  string             sFichierMod = string("") ;

	ifstream inFile ;
	//
	// Tentative d'ouverture du fichier spécifique à l'utilisateur
	/******
	strcpy(nomFichier, "N") ;
	nomFichier[0] = type ;
	strcat(nomFichier, pDoc->pSuper->pUtilisateur->pDonnees->indice) ;
	strcat(nomFichier, ".MOD") ;
  sFichierMod = sPathMod + string(nomFichier) ;
	inFile.open(sFichierMod.c_str()) ;
	//
	// Si la tentative a échoué, ouverture du fichier standard
	//
	if (!inFile)
	{
    *****/

    string sNomFichier = string("N.MOD") ;
    sNomFichier[0] = type ;
    sFichierMod = sPathMod + sNomFichier ;
    inFile.open(sFichierMod.c_str()) ;
    if (!inFile)
    {
      string sErrorMsg = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
      sErrorMsg += string(" ") + sFichierMod ;

      pContexte->getSuperviseur()->trace(&sErrorMsg, 1, NSSuper::trWarning) ;
      erreur(sErrorMsg.c_str(), standardError, 0, GetHandle()) ;
      return false ;
    }
		/* if (!inFile)
			return InitialiseModeDefaut(); */
	// }

	// etatEnCours : 1 = Cadre, 2 = Police, 3 = Paragraphe, 4 = Page
	etatEnCours = 0;

	// Lecture du fichier	inFile.unsetf(ios::skipws) ;
	//while (inFile >> Chaine)
	while (inFile.getline(buffer, 200))
	{
		string Chaine = buffer ;

		// Enlève les blancs au début et à la fin de la Chaine
    strip(Chaine) ;

		// Commentaire ou ligne vierge : on ne fait rien
		if ((strlen(Chaine.c_str()) == 0) ||
			 ((Chaine[0] == '/') && (Chaine[1] == '/')))
		{
		}

		// Si on rencontre un délimiteur de Section
		else if ((Chaine[0] == '[') && (Chaine[strlen(Chaine.c_str())-1] == ']'))
		{
			// On en termine avec l'élément précédent
			// (les polices sont mises en place par la fct de traitement)
			switch (etatEnCours)
			{
      	case 1 :
          if ((NSCadreDecor*) NULL == pCadre)
          {
            _Cadres.push_back(new NSCadreDecor(*pCadre)) ;
            delete pCadre ;
            pCadre = (NSCadreDecor*) 0 ;
          }
          pCadre = 0 ;
          break ;
        case 3 :
          if (pParagraphe)
          {
            _StylesParagraphe.push_back(new NSStyleParagraphe(*pParagraphe)) ;
            delete pParagraphe ;
          }
          pParagraphe = 0 ;
          break ;
        case 4 :
          if (pPage)
          {
            _Pages.push_back(new NSPage(*pPage)) ;
            delete pPage ;
          }
          pPage = 0 ;
          break ;
      }

			// On analyse le type de section
			if (Chaine.find("Cadre") != NPOS)
			{
				pCadre = new NSCadreDecor ;
				etatEnCours = 1 ;
        pCadre->InitTitre(&Chaine) ;
			}
			else if (Chaine.find("Polices") != NPOS)
			{
				/* pPolice = new NSFont(); */
				if 	  (Chaine.find("par défaut") != NPOS)
					etatEnCours = 2 ;
				else if ((string("") != _ImpDevice) && Chaine.find(_ImpDevice) != NPOS)
				{
					_StylesPolice.vider() ;
					etatEnCours = 2 ;
				}
				else
        {
          etatEnCours = 0 ;   // don't take into account

          // Is it a screen resolution dedicated font selection?
          //
          string sMajChaine = pseumaj(Chaine) ;
          size_t iPos = sMajChaine.find("SCREEN") ;

          if ((NPOS != iPos) && isCloserScreen(Chaine, iPos))
          {
            _StylesPolice.vider() ;
            etatEnCours = 2 ;
          }
        }
			}
			else if (Chaine.find("Paragraphe") != NPOS)
			{
				pParagraphe = new NSStyleParagraphe ;
				etatEnCours = 3 ;
			}
      else if (Chaine.find("Mise en page") != NPOS)
			{
				pPage = new NSPage ;
				etatEnCours = 4 ;
        pPage->InitNumPage(&Chaine) ;
			}
			else
				etatEnCours = 0 ;
		}

		// Sinon traitement de la ligne
		else if (etatEnCours > 0)
		{
      string ChaineNom 	   = Chaine ;
      string ChaineContenu = string("") ;

			recherche = Chaine.find('=') ;
			if ((NPOS != recherche) && (recherche < strlen(Chaine.c_str()) - 1))
			{
				// On sépare la chaine en Nom et Contenu
				//	Ex : "Font1=Times New Roman,120"
				// -> Nom : "Font1" et Contenu = "Times New Roman,120"
				ChaineNom 	  = string(Chaine, 0, recherche) ;
				ChaineContenu = string(Chaine, recherche+1, strlen(Chaine.c_str())-(recherche+1)) ;
      }

			// On traite suivant la section
			switch (etatEnCours)
			{
      	case 1 :	if (ChaineNom.find("Taille") != NPOS)
        						pCadre->InitTaille(&ChaineContenu) ;
        					else if (ChaineNom.find("DebutTexte") != NPOS)
                  {
                  	pCadre->finTexte() ;
                    pCadre->debutTexte() ;
                  }
                  else if (ChaineNom.find("FinTexte") != NPOS)
                  	pCadre->finTexte() ;
                  else if (ChaineNom.find("Texte") != NPOS)
                  	pCadre->InitTexte(&ChaineContenu) ;
                  else if (ChaineNom.find("Police") != NPOS)
                  	pCadre->InitPolice(&ChaineContenu) ;
                  else if (ChaineNom.find("Retrait") != NPOS)
                  	pCadre->InitRetrait(&ChaineContenu) ;
                  else if (ChaineNom.find("Espacement") != NPOS)
                  	pCadre->InitEspacement(&ChaineContenu) ;
                  else if (ChaineNom.find("Alignement") != NPOS)
                  	pCadre->InitAlignement(&ChaineContenu) ;
                  else if (ChaineNom.find("Tabulations") != NPOS)                  	pCadre->InitTabulations(&ChaineContenu) ;
                  break ;
        case 2 :	InitModePolice(&ChaineNom, &ChaineContenu) ;
        					break ;
        case 3 :	if (ChaineNom.find("Police") != NPOS)
        						pParagraphe->InitPolice(&ChaineContenu) ;
        					else if (ChaineNom.find("Retrait") != NPOS)
                  	pParagraphe->InitRetrait(&ChaineContenu) ;
                  else if (ChaineNom.find("Espacement") != NPOS)
                  	pParagraphe->InitEspacement(&ChaineContenu) ;
                  else if (ChaineNom.find("Alignement") != NPOS)
                  	pParagraphe->InitAlignement(&ChaineContenu) ;
                  else if (ChaineNom.find("Tabulations") != NPOS)
                  	pParagraphe->InitTabulations(&ChaineContenu) ;
                  break ;
        case 4 :	if (ChaineNom.find("MargesTexte") != NPOS)
        						pPage->InitMargesTxt(&ChaineContenu) ;
                  else if (ChaineNom.find("Marges") != NPOS)
                  	pPage->InitMarges(&ChaineContenu) ;
                  else if (ChaineNom.find("Decor") != NPOS)
                  	pPage->InitDecor(&ChaineNom, &ChaineContenu) ;
                  break ;
      }
		}
  }

	// On en termine avec l'élément précédent
	switch (etatEnCours)
	{
  	case 1 :	_Cadres.push_back(new NSCadreDecor(*pCadre)) ;
            	delete pCadre ;
            	pCadre = 0 ;
            	break ;
    case 3 :	_StylesParagraphe.push_back(new NSStyleParagraphe(*pParagraphe)) ;
            	delete pParagraphe ;
            	pParagraphe = 0 ;
            	break ;
    case 4 :	_Pages.push_back(new NSPage(*pPage)) ;
            	delete pPage ;
            	pPage = 0 ;
            	break ;
  }
	inFile.close() ;
	return true ;
}

//
// Get a String in the form "Screen 1920,1080" and determine if it is closer
// from actual screen than the current selection (if yes, return true)
//
// When returning true, updates _iFontForScreenResH and _iFontForScreenResW
//
bool
NSRefVue::isCloserScreen(const string sChapter, size_t iScreenPos)
{
  if (string("") == sChapter)
    return false ;

  size_t iChapterLen = strlen(sChapter.c_str()) ;

  if ((NPOS == iScreenPos) || (iScreenPos >= iChapterLen - 8))
    return false ;

  // Res should be in the form "W,H"
  //
  string sRes = string(sChapter, iScreenPos + 7, iChapterLen - 1) ;
  strip(sRes, stripRight, ']') ;
  strip(sRes) ;

  string sResW = string("") ;
  string sResH = string("") ;
  splitString(sRes, &sResW, &sResH, ',') ;
  strip(sResW) ;
  strip(sResH) ;

  if ((string("") == sResW) && (string("") == sResH))
    return false ;

  int iResW = 0 ;
  int iResH = 0 ;

  if (string("") == sResW)
    iResW = _iFontForScreenResW + 1 ;
  else
    iResW = StringToInt(sResW) ;

  if (string("") == sResH)
    iResH = _iFontForScreenResH + 1 ;
  else
    iResH = StringToInt(sResH) ;

  // If screen target is set and is larger than actual screen, it is not OK
  //
  if (((iResW < MAXSCREENRES) && (iResW > _iScreenResW)) ||
      ((iResH < MAXSCREENRES) && (iResH > _iScreenResH)))
    return false ;

  if (getExcessSurface(iResW, iResH) < getExcessSurface(_iFontForScreenResW, _iFontForScreenResH))
  {
    if (iResW < MAXSCREENRES)
      _iFontForScreenResW = iResW ;
    else
      _iFontForScreenResW = MAXSCREENRES ;

    if (iResH < MAXSCREENRES)
      _iFontForScreenResH = iResH ;
    else
      _iFontForScreenResH = MAXSCREENRES ;

    return true ;
  }
  else
    return false ;
}

// Get surface in excess between a given resolution and actual resolution
//
int
NSRefVue::getExcessSurface(int iResW, int iResH) const
{
  // If given resolution is greater than actual screen resolution, no way
  //
  if (((iResW < MAXSCREENRES) && (iResW > _iScreenResW)) ||
      ((iResH < MAXSCREENRES) && (iResH > _iScreenResH)))
    return -1 ;

  if (iResW >= MAXSCREENRES)
    iResW = 0 ;

  if (iResH >= MAXSCREENRES)
    iResH = 0 ;

  return ((_iScreenResW - iResW) * (_iScreenResH - iResH)) ;
}

// -----------------------------------------------------------------------------// Function    : NSRefVue::InitModePolice(pPolice, ChaineNom, ChaineContenu)// Arguments   :	pPolice 		 : pointeur sur la NSFont à initialiser
//				  			ChaineNom 	 : Nom de la Font (forme "Font4" pour la 4ème)
//				  			ChaineContenu : Descriptif de la Font
// Description : Initialise le style du document à partir du fichier N.MOD
// Returns     : true/false
// -----------------------------------------------------------------------------
//?????????????????????? a revoir
bool
NSRefVue::InitModePolice(string* pChaineNom, string* pChaineContenu)
{
try
{
	UINT    indice, place ;
	size_t  i, j, k ;
	size_t  positionDeb, positionFin, positionNew ;
	NSFont* pPolice ;
	char    facename[LF_FACESIZE] ;
	int	    height = 0, width = 0, escapement = 0, orientation = 0, weight = FW_NORMAL ;
	BYTE    pitchAndFamily = DEFAULT_PITCH|FF_DONTCARE ;
	BYTE    italic = false, underline = false, strikeout = false, charSet = 1 ;
	BYTE    outputPrecision = OUT_DEFAULT_PRECIS ;
	BYTE    clipPrecision	= CLIP_DEFAULT_PRECIS ;
	BYTE    quality			= DEFAULT_QUALITY ;

	// Si le nom n'est pas bon, on sort
	if (string(*pChaineNom, 0, 4) != "Font")
  	return false ;

	// Les caractères situés juste après "Font" donnent l'indice de la police
	if ((strlen(pChaineNom->c_str()) > 4) && (isdigit((*pChaineNom)[4])))
	{
		indice = UINT((*pChaineNom)[4]) - UINT('0') ;
		if ((strlen(pChaineNom->c_str()) > 5) && (isdigit((*pChaineNom)[5])))
		{
			indice = 10 * indice ;
			indice += UINT((*pChaineNom)[5]) - UINT('0') ;
		}
	}

	// S'il n'y a pas de caractères, la police se place en queue de tableau
	else
		//indice = StylesPolice.ArraySize();
    indice = _StylesPolice.size() ;

	// Analyse du descriptif de police
	place = 1 ;
	positionDeb = 0 ;
	positionFin = 0 ;
	positionNew = 0 ;
	while (NPOS != positionNew)
	{
		// Plante quand positionDeb = 10
		//positionNew = ChaineContenu.find((const string) ",", positionDeb);
		for (positionNew = positionDeb ;
				(positionNew < strlen(pChaineContenu->c_str())) &&
				((*pChaineContenu)[positionNew] != ',') ; positionNew++)
    	;
		if (positionNew == strlen(pChaineContenu->c_str()))
			positionNew = NPOS ;

		if (positionNew == NPOS)
			positionFin = strlen(pChaineContenu->c_str()) ;
		else
			positionFin = positionNew ;

		// Traitement du nom de police
		if (place == 1)
		{
			i = min(UINT(LF_FACESIZE-1), positionFin - positionDeb) ;
			for (j = positionDeb ; j < i ; j++)
      	facename[j-positionDeb] = (*pChaineContenu)[j] ;
			facename[j-positionDeb] = '\0' ;
		}

		// Traitement des éléments numériques
		else if ((place > 1) && (positionFin - positionDeb + 1 < 5))
		{
			i = 0 ;
			for (j = positionDeb ; j < positionFin ; j++)
				i = (10 * i) + donneDigit((*pChaineContenu)[j]) ;

			switch (place)
			{
        case 2 	:	height 	    = i ; break ;
        case 3 	:	width  	    = i ; break ;
        case 4 	:	escapement  = i ; break ;
        case 5 	:	orientation = i ; break ;
        case 6 	:	weight 	    = i ; break ;
        default	:	if ((i >= 0) && (i < 256))
            			{
                		switch (place)
                		{
                      case  7 : pitchAndFamily  = (BYTE)i ; break ;
                      case  8 : italic 	        = (BYTE)i ; break ;
                      case  9 : underline 	    = (BYTE)i ; break ;
                      case 10 : strikeout 	    = (BYTE)i ; break ;
                      case 11 : charSet 		    = (BYTE)i ; break ;
                      case 12 : outputPrecision = (BYTE)i ; break ;
                      case 13 : clipPrecision   = (BYTE)i ; break ;
                      case 14 : quality		    	= (BYTE)i ; break ;
                		}
            			}
			}
		}
		if (positionNew != NPOS)
			positionDeb = positionFin + 1 ;
		place++ ;
	}

	// Appel du constructeur de NSFont
	pPolice = new NSFont(facename, height, width, escapement, orientation,
							  weight, pitchAndFamily, italic, underline, strikeout,
							  charSet, outputPrecision, clipPrecision, quality) ;

	// Mise de la police à sa place dans le tableau
  FontIter iter;
  for(iter = _StylesPolice.begin(), k=0 ; (_StylesPolice.end() != iter) && (k < indice-1) ; iter++, k++)
		;

	_StylesPolice.insert(iter, new NSFont(*pPolice)) ;
	delete pPolice ;

	return true ;
}
catch (...)
{
  erreur("Exception NSRefVue::InitModePolice.", standardError, 0) ;
  return false ;
}
}


// -----------------------------------------------------------------------------
// Description : Renvoie un pointeur sur la page de numéro numPage
// Arguments   : numPage : numéro de la page à chercher
// Retour      : Pointeur ou 0 si la page n'existe pas
// -----------------------------------------------------------------------------
NSPage *
NSRefVue::donnePage(unsigned int numPage)
{
	if (_Pages.empty())
		return 0 ;

  NSStylPageArrayIter i ;
	for (i = _Pages.begin() ; (_Pages.end() != i) && ((*i)->numeroPage != numPage) ; i++)
  	;
	if (_Pages.end() != i)
		return *i ;
	else
		return (NSPage *) 0 ;
}

// -----------------------------------------------------------------------------
// Description : Renvoie un pointeur sur le décor de titre titreDecor
// Arguments   : titreDecor : titre du décor à chercher
// Retour      : Pointeur ou 0 si la page n'existe pas
// -----------------------------------------------------------------------------
NSCadreDecor*
NSRefVue::donneDecor(string* titreDecor)
{
	if (_Cadres.empty())
		return 0 ;

  CadreDecoIter i;
  for (i = _Cadres.begin(); (_Cadres.end() != i) && (strcmp((*i)->Titre.c_str(), titreDecor->c_str()) != 0); i++)
  	;

	if (_Cadres.end() != i)
		return *i ;
	else
		return (NSCadreDecor*) 0 ;
}

//---------------------------------------------------------------------------//  Function: 		NSRefVue::metDecor(TRect* rectPage, int ecranImpri)
//
//  Arguments : 	rectPage   : Cadre de la page
//						indicePage : indice de la page
//						ecranImpri : 1 = ecran ; 2 = imprimante
//
//  Description : Affiche les décors de la page
//
//  Retour :		Rien
//---------------------------------------------------------------------------
void
NSRefVue::metDecor(NS_CLASSLIB::TRect* rectPage, TDC* pDC, unsigned numPage, int ecranImpri)
{
    NSCadreDecor*      pCadreDecor;
    NS_CLASSLIB::TRect Cadrage;    long			   lGche, lDrte, lHaut, lBas;
    long			   TraiteLigne, LargeurPossible, LargeurGauche ;
	NSLigne*		   pLigne;
    unsigned		   iNumPhrase = 0;
	size_t		       Curseur, NouBlanc, PreBlanc ;
    NS_CLASSLIB::TSize TailleChaine;
    //    // Recherche de la page de numéro numPage
    //
	NSPage* pStylePage = donnePage(numPage);
    if ((pStylePage == 0) || (pStylePage->Decors.empty()))
		return;
    //
    // Passage en revue de tous ses décors
    //
    NSDecoArrayIter iter;

	for (iter = pStylePage->Decors.begin(); iter != pStylePage->Decors.end(); iter++)
    {
        //
        // Recherche du décor correspondant
        //
   	    pCadreDecor = donneDecor(&((*iter)->titreCadre));
        if ((pCadreDecor != 0) && !(pCadreDecor->DecorArray.empty()))
        {
      	    //
            // Détermination du cadre dans lequel s'inscrit le décor
            //
            if ((*iter)->coinHautGcheX > 0)
            {
         	    lGche = rectPage->Left() + (*iter)->coinHautGcheX;
                lDrte = lGche + pCadreDecor->tailleHorizontale;
            }
            else if ((*iter)->AlignementX == TA_LEFT)
            {
         	    lGche = rectPage->Left();
                lDrte = lGche + pCadreDecor->tailleHorizontale;
            }
            else if ((*iter)->AlignementX == TA_RIGHT)
            {         	    lDrte = rectPage->Right();
                lGche = lDrte - pCadreDecor->tailleHorizontale;
            }
            else if ((*iter)->AlignementX == TA_CENTER)
            {
         	    lGche = rectPage->Left() + (rectPage->Width() - pCadreDecor->tailleHorizontale) / 2;
                lDrte = lGche + pCadreDecor->tailleHorizontale;
            }
            else if ((*iter)->AlignementX == TA_JUSTIFIED)
            {
         	    lGche = rectPage->Left();
                lDrte = rectPage->Right();
            }

            if ((*iter)->coinHautGcheY > 0)
            {
         	    lHaut = rectPage->Top() - (*iter)->coinHautGcheY;
                lBas  = lHaut - pCadreDecor->tailleVerticale;
            }
            else if ((*iter)->AlignementY == TA_LEFT)
            {
         	    lHaut = rectPage->Top();
                lBas  = lHaut - pCadreDecor->tailleVerticale;
            }
            else if ((*iter)->AlignementY == TA_RIGHT)
            {
         	    lBas  = rectPage->Bottom();
                lHaut = lBas + pCadreDecor->tailleVerticale;
            }
            else if ((*iter)->AlignementY == TA_CENTER)
            {
         	    lHaut = rectPage->Top() + (rectPage->Height() - pCadreDecor->tailleVerticale) / 2;
                lBas  = lHaut - pCadreDecor->tailleVerticale;
            }
            else if ((*iter)->AlignementY == TA_JUSTIFIED)
            {
         	    lHaut = rectPage->Top();
                lBas  = rectPage->Bottom();
            }
            Cadrage.Set(lGche, lHaut, lDrte, lBas);
            //
            // Tracé ligne par ligne
            //
            string 				sTexte;
            NSStyleParagraphe StylePara;

            NS_CLASSLIB::TPoint PointRef;
            PointRef.x = Cadrage.Left();
            PointRef.y = Cadrage.Top();

            NSStylDecoArrayIter decoIter;

            for (decoIter = pCadreDecor->DecorArray.begin(); decoIter != pCadreDecor->DecorArray.end(); decoIter++)
            {
         	    sTexte 	 = (*decoIter)->Texte;
              StylePara = (*decoIter)->styleParagraphe;
            // On prévoit l'espace au dessus du paragraphe
            PointRef.Offset(0, -StylePara.EspaceDessus);
            // Calcul de la largeur utilisable sans le retrait gauche
                    LargeurGauche = Cadrage.Width() - StylePara.RetraitDroit;
                    TraiteLigne = 1;
                    Curseur = 0; PreBlanc = 0;
            while (TraiteLigne > 0)
            {
              // Création d'une ligne vierge
              pLigne = new NSLigne();
              // Initialisation de tous les paramètres déjà connus
              pLigne->iNumPhrase = iNumPhrase;
              pLigne->numPage = numPage;
              if (TraiteLigne == 1)
                pLigne->Boite.left = Cadrage.Left()+StylePara.RetraitPremLigne;
              else
                pLigne->Boite.left = Cadrage.Left()+StylePara.RetraitGauche;
              pLigne->Boite.right   = LargeurGauche;
              pLigne->Boite.top	  	 = PointRef.y;
              pLigne->Boite.bottom  = PointRef.y;
              // pLigne->CouleurTexte  = *(StylePara.pCouleurTexte);
              pLigne->numParagraphe 	= 0;
                        pLigne->StyleParagraphe = StylePara;

              pLigne->numLigne		 = TraiteLigne;
              // Traitement des chaines de taille non nulle
              if (strlen(sTexte.c_str()) > 0)
                        {
                // Détermination de la largeur utilisable
                LargeurPossible = UINT(pLigne->Boite.Width());
                // Recherche du segment de chaîne qui "tient"
                NouBlanc = Curseur;
                do
                {
                  PreBlanc = NouBlanc;
                  NouBlanc = sTexte.find(' ', PreBlanc+1);
                  if (NouBlanc == NPOS)
                    break;
                  TailleChaine = pDC->GetTextExtent(string(sTexte, Curseur, NouBlanc-Curseur).c_str(),
                                 int(NouBlanc-Curseur));
                }
                while (TailleChaine.cx < LargeurPossible);
                // Si on est en fin de chaîne,
                            // on regarde si le dernier mot "tient"
                if (NouBlanc == NPOS)
                {
                  TailleChaine = pDC->GetTextExtent(string(sTexte, Curseur, strlen(sTexte.c_str())-Curseur).c_str(),
                                 int(strlen(sTexte.c_str())-Curseur));
                  if (TailleChaine.cx < LargeurPossible)
                    PreBlanc = strlen(sTexte.c_str());
                }
                // On met le segment de chaine "qui tient"
                      // dans la chaine Texte de la ligne en cours
                pLigne->Texte = string(sTexte, Curseur, PreBlanc-Curseur);
                // On calcule la vraie taille de cette chaine
                            strip(pLigne->Texte);
                TailleChaine = pDC->GetTextExtent(pLigne->Texte.c_str(),
                               int(strlen(pLigne->Texte.c_str())));
                // On initialise Boite : rectangle exinscrit à la ligne
                if (TailleChaine.cy < StylePara.HauteurLigne)
                  TailleChaine.cy = StylePara.HauteurLigne;
                pLigne->Boite.bottom -= TailleChaine.cy;
                PointRef.Offset(0, -TailleChaine.cy);
                if (PreBlanc < strlen(sTexte.c_str()))
                {
                  Curseur = PreBlanc + 1;
                  TraiteLigne++;
                  pLigne->DerniereLigne = false;
                }
                else
                {
                  TraiteLigne = 0;
                  pLigne->DerniereLigne = true;
                }
              }              // Traitement des chaines vides
              else
              {
                pLigne->Texte = "";
                // On initialise Boite : rectangle exinscrit à la ligne
                TailleChaine = pDC->GetTextExtent(" ", 1);
                if (TailleChaine.cy < StylePara.HauteurLigne)
                  TailleChaine.cy = StylePara.HauteurLigne;
                pLigne->Boite.bottom -= TailleChaine.cy;
                PointRef.Offset(0, -TailleChaine.cy);
                // On précise que le traitement est terminé
                TraiteLigne = 0;
                pLigne->DerniereLigne = true;
              }
              // On ajoute la ligne à Lignes
              _Lignes.push_back(new NSLigne(*pLigne));
              delete pLigne;
              pLigne = 0;
            }
            // On prévoit l'espace au dessous du paragraphe
            PointRef.Offset(0, -StylePara.EspaceDessous);
         }
      }
   }
}

voidNSRefVue::SelectAll()
{
  // on sélectionne toutes les lignes du CR
  _iLignePrec    = _iLigneEnCours ;
  _iLigneEnCours = 1 ;
  _iLigneSelect  = _Lignes.size() - 1 ;
  Invalidate() ;
}

void
NSRefVue::CopyToClipboard()
{
  if ((!::OpenClipboard(NULL)) || (0 == _iLigneEnCours))
    return ;

  // calcul des lignes min et max du bloc sélectionné
  //
  int minLigne, maxLigne ;
  if ((_iLigneEnCours + _iLigneSelect) < _iLigneEnCours)
  {
    minLigne = _iLigneEnCours + _iLigneSelect ;
    maxLigne = _iLigneEnCours ;
  }
  else
  {
    minLigne = _iLigneEnCours ;
    maxLigne = _iLigneEnCours + _iLigneSelect ;
  }

  // On copie le texte sélectionné dans une string
  //
  string sTextSelect = string("") ;

  int    numLigne    = 1 ;
  if (false == _Lignes.empty())
    for (NSLignesArrayIter iter = _Lignes.begin() ; _Lignes.end() != iter ; iter++)
    {
      if ((numLigne >= minLigne) && (numLigne <= maxLigne))
        sTextSelect += (*iter)->Texte + string("\r\n") ;
      numLigne++ ;
    }

  size_t len = strlen(sTextSelect.c_str()) ;
  char* cData = new char[len + 1] ;
  strcpy(cData, sTextSelect.c_str()) ;

  // on vide le clipboard
  //
  ::EmptyClipboard() ;

  // Allocate a global memory object for the text.
  //
  HGLOBAL hgCopy = GlobalAlloc(GMEM_DDESHARE, (len + 1) * sizeof(char)) ;
  if (NULL == hgCopy)
  {
    ::CloseClipboard() ;
    delete[] cData ;
    return ;
  }

  // Lock the handle and copy the text to the buffer.
  //
  LPSTR lpCopy = (LPSTR) GlobalLock(hgCopy) ;
  memcpy(lpCopy, cData, len * sizeof(char)) ;
  lpCopy[len] = '\0' ;
  GlobalUnlock(hgCopy) ;

  // on copie dans le clipboard
  //
  ::SetClipboardData(CF_TEXT, hgCopy) ;

  ::CloseClipboard() ;
  delete[] cData ;
}

voidNSRefVue::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	WORD wScrollNotify = 0xFFFF;
	bool bVScroll = true;

	switch (key)	{
		case 'a':
			break ;
		case 'A':
			// Control-A : Tout sélectionner
			if (GetKeyState(VK_CONTROL) < 0)
				SelectAll() ;
			break ;

		case 'c':			break ;
		case 'C':
			// Control-C : copier vers le presse-papier
			if (GetKeyState(VK_CONTROL) < 0)
				CopyToClipboard() ;
			break ;

		case VK_UP:      if (GetKeyState(VK_SHIFT) < 0)
      {
        // sélectionner vers le haut
        if ((_iLigneEnCours) && ((_iLigneEnCours + _iLigneSelect) > 1))
        {
          _iLigneSelect-- ;
          Invalidate() ;
        }
      }
      else
        wScrollNotify = SB_LINEUP ;
      break ;

    case VK_PRIOR:      wScrollNotify = SB_PAGEUP ;
      break ;

    case VK_NEXT:      wScrollNotify = SB_PAGEDOWN ;
      break ;

    case VK_DOWN:      if (GetKeyState(VK_SHIFT) < 0)
      {
        // sélectionner vers le bas
        if ((_iLigneEnCours) && ((_iLigneEnCours + _iLigneSelect) < _Lignes.size()))
        {
          _iLigneSelect++ ;
          Invalidate() ;
        }
      }
      else
        wScrollNotify = SB_LINEDOWN ;
      break ;

    case VK_HOME:      wScrollNotify = SB_TOP;
      break;

    case VK_END:
      wScrollNotify = SB_BOTTOM;
      break;

    case VK_RIGHT:
      wScrollNotify = SB_LINERIGHT;
      bVScroll = false;
      break;

    case VK_LEFT:
      wScrollNotify = SB_LINELEFT;
      bVScroll = false;
      break;
  }

	if (wScrollNotify != -1)
	{
		if (bVScroll)
			SendMessage(WM_VSCROLL, MAKELONG(wScrollNotify, 0), 0L);
		else
			SendMessage(WM_HSCROLL, MAKELONG(wScrollNotify, 0), 0L);
	}

	TWindowView::EvKeyDown(key, repeatCount, flags);
}
voidNSRefVue::EvSysCommand(uint cmdType, NS_CLASSLIB::TPoint& point)
{
    // MessageBox("EvSysCommand");
}

voidNSRefVue::EvSysKeyDown(uint key, uint repeatCount, uint flags)
{
    // MessageBox("EvSysKeyDown");
}

//---------------------------------------------------------------------------//  Function: NSRefVue::EvVScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)
//
//  Arguments:   dc 				: device contexte de la fenêtre
//					  RectAPeindre : Rectangle à repeindre
//
//  Description: Peint/repeint tout/une partie de la fenêtre
//
//  Returns:	  Rien
//---------------------------------------------------------------------------
void
NSRefVue::EvVScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)
{
	if (_Lignes.empty())
		return ;

	const NSLigne* pLigne = (NSLigne*) 0 ;
	int		         iTailleScroll ;
	NS_CLASSLIB::TPoint* pPointRef ;
	//
	// On sort tout de suite si on est sur les lignes extrèmes
	//
	if (((scrollCode == SB_LINEDOWN) && (_PremLigne == _Lignes.size())) ||
      ((scrollCode == SB_PAGEDOWN) && (_PremLigne == _Lignes.size())) ||
      ((scrollCode == SB_LINEUP)   && (_PremLigne == 1)) ||
      ((scrollCode == SB_PAGEUP)   && (_PremLigne == 1)))
		return ;
	//
	// On récupère un "contexte d'information" concernant l'écran
	//
	TIC* pDC = new TIC("DISPLAY", NULL, NULL) ;
	pDC->SetMapMode(MM_LOMETRIC) ;
	//
	// Scroll vers le haut (flêche basse)
	//
	if (SB_LINEDOWN == scrollCode)
	{
		//
		// Calcul de la quantité à scroller (hauteur de la première ligne)
		//
		pLigne = _Lignes[_PremLigne-1] ;
		iTailleScroll = pLigne->Boite.Height() ;
		//
		// Conversion en pixels
		//
		pPointRef = new NS_CLASSLIB::TPoint(0, iTailleScroll) ;
		pDC->LPtoDP(pPointRef) ;
		//
		// Demande de scroll
		//
		ScrollWindow(0, -pPointRef->y) ;
		delete pPointRef ;
		pPointRef = 0 ;
		//
		// Ajustement des variables PremLigne et HautGcheFenetre
		//
		_PremLigne++ ;
		_HautGcheFenetre.y += iTailleScroll ;
		//UpdateWindow();
	}
	//
	// Scroll vers le haut (page basse)	//
	else if (SB_PAGEDOWN == scrollCode)
	{
    NS_CLASSLIB::TRect rectClient = GetClientRect();
    int cyScroll      = ::GetSystemMetrics(SM_CYHSCROLL) ;
    int hauteurLignes = 0 ;
    int numLigne      = _PremLigne; // numéro de la première ligne visible
    int hauteurFen    = rectClient.bottom - rectClient.top - cyScroll; // hauteur fenetre en pixels
    int hBoite = 0, hLigne = 0;
    int tailleScroll  = 0;

    while ((numLigne < _Lignes.size()) && (hauteurLignes < hauteurFen))
    {
      pLigne = _Lignes[numLigne - 1] ;
      hBoite = pLigne->Boite.Height() ;
      NS_CLASSLIB::TPoint py(0, hBoite) ;
      pDC->LPtoDP(&py) ;
      hLigne = py.y ;

      tailleScroll  += hBoite ;
      hauteurLignes += hLigne ;
      numLigne++ ;
    }

    _PremLigne = numLigne ;
    ScrollWindow(0, -hauteurLignes) ;
    _HautGcheFenetre.y += tailleScroll ;
  }
	//
	// Scroll vers le bas (flêche haute)
	//
	else if (SB_LINEUP == scrollCode)
	{
		//
		// Calcul de la quantité à scroller (hauteur de l'avant-première ligne)
		//
		pLigne = _Lignes[_PremLigne-2];
		iTailleScroll = pLigne->Boite.Height();
		//
		// Conversion en pixels
		//
		pPointRef = new NS_CLASSLIB::TPoint(0, iTailleScroll);
		pDC->LPtoDP(pPointRef);
		//
		// Demande de scroll
		//
		ScrollWindow(0, pPointRef->y) ;
		delete pPointRef ;
    pPointRef = 0 ;
		//
		// Ajustement des variables PremLigne et HautGcheFenetre
		//
		_PremLigne-- ;
		_HautGcheFenetre.y -= iTailleScroll ;
		//UpdateWindow();
	}
	//
	// Scroll vers le bas (page haute)
	//
	else if (SB_PAGEUP == scrollCode)
  {
    NS_CLASSLIB::TRect rectClient = GetClientRect() ;
    int cyScroll = ::GetSystemMetrics(SM_CYHSCROLL) ;
    int hauteurLignes = 0 ;
    int numLigne      = _PremLigne ; // numéro de la première ligne visible
    int hauteurFen    = rectClient.bottom - rectClient.top - cyScroll ; // hauteur fenetre en pixels
    int hBoite = 0, hLigne = 0 ;
    int tailleScroll  = 0 ;

    while ((numLigne > 1) && (hauteurLignes < hauteurFen))
    {
      pLigne = _Lignes[numLigne - 2];  // Ligne précédente
      hBoite = pLigne->Boite.Height();
      NS_CLASSLIB::TPoint py(0, hBoite);
      pDC->LPtoDP(&py);
      hLigne = py.y;

      tailleScroll += hBoite;
      hauteurLignes += hLigne;
      numLigne--;
    }

    _PremLigne = numLigne;
    ScrollWindow(0, hauteurLignes);
    _HautGcheFenetre.y -= tailleScroll;
	}

	delete pDC ;
	pDC = 0 ;
	SetScrollPos(SB_VERT, _PremLigne) ;
}

//---------------------------------------------------------------------------
//  Function: NSRefVue::EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)
//
//  Arguments:   dc 				: device contexte de la fenêtre
//					  RectAPeindre : Rectangle à repeindre
//
//  Description: Peint/repeint tout/une partie de la fenêtre
//
//  Returns:	  Rien
//---------------------------------------------------------------------------
void
NSRefVue::EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)
{
	NS_CLASSLIB::TPoint* pPointRef;
	//
	// On sort tout de suite si on est sur les lignes extrèmes
	//
	if (((scrollCode == SB_LINERIGHT) && (_HautGcheFenetre.x == _RectangleGlobal.Width())) ||
      ((scrollCode == SB_PAGERIGHT) && (_HautGcheFenetre.x == _RectangleGlobal.Width())) ||
		  ((scrollCode == SB_LINELEFT)  && (_HautGcheFenetre.x == 0)) ||
      ((scrollCode == SB_PAGELEFT)  && (_HautGcheFenetre.x == 0)))
		return ;
	//
	// On récupère un "contexte d'information" concernant l'écran
	//
	TIC* pDC = new TIC("DISPLAY", NULL, NULL) ;
	pDC->SetMapMode(MM_LOMETRIC) ;
	//
	// Scroll vers la gauche (flêche droite)
	//
	if (SB_LINERIGHT == scrollCode)
	{
		//
		// Conversion en pixels
		//
		pPointRef = new NS_CLASSLIB::TPoint(_LargeurPolice, 0);
		pDC->LPtoDP(pPointRef);
		//
		// Demande de scroll
		//
		ScrollWindow(-pPointRef->x, 0) ;
		delete pPointRef ;
    pPointRef = 0 ;
		//
		// Ajustement de HautGcheFenetre
		//
		_HautGcheFenetre.x += _LargeurPolice ;
	}
  //
	// Scroll vers la gauche (page droite)
	//
	if (SB_PAGERIGHT == scrollCode)
	{
		//
		// Conversion en pixels
		//
    int k = 1 ;
    pPointRef = new NS_CLASSLIB::TPoint(_LargeurPolice, 0) ;
    pDC->LPtoDP(pPointRef) ;

    while ((k <= 10) && ((_HautGcheFenetre.x + _LargeurPolice) <= _RectangleGlobal.Width()))
    {
      //
      // Demande de scroll
      //
      ScrollWindow(-pPointRef->x, 0) ;
      //
      // Ajustement de HautGcheFenetre
      //
      _HautGcheFenetre.x += _LargeurPolice ;
      k++ ;
    }

		delete pPointRef;
    pPointRef = 0;
	}
	//
	// Scroll vers la droite (flêche gauche)
	//
	else if (SB_LINELEFT == scrollCode)
	{
		//
		// Conversion en pixels
		//
		pPointRef = new NS_CLASSLIB::TPoint(_LargeurPolice, 0) ;
		pDC->LPtoDP(pPointRef) ;
		//
		// Demande de scroll
		//
		ScrollWindow(pPointRef->x, 0) ;
		delete pPointRef ;
    pPointRef = 0 ;
		//
		// Ajustement de HautGcheFenetre
		//
		_HautGcheFenetre.x -= _LargeurPolice ;
	}
  //
	// Scroll vers la droite (page gauche)
	//
	if (SB_PAGELEFT == scrollCode)
	{
		//
		// Conversion en pixels
		//
    int k = 1 ;
    pPointRef = new NS_CLASSLIB::TPoint(_LargeurPolice, 0) ;
    pDC->LPtoDP(pPointRef) ;

    while ((k <= 10) && ((_HautGcheFenetre.x - _LargeurPolice) >= 0))
    {
      //
      // Demande de scroll
      //
      ScrollWindow(pPointRef->x, 0) ;
      //
      // Ajustement de HautGcheFenetre
      //
      _HautGcheFenetre.x -= _LargeurPolice ;
      k++ ;
    }

		delete pPointRef ;
    pPointRef = 0 ;
	}

	delete pDC ;
  pDC = 0 ;
  if (_LargeurPolice > 0)
		SetScrollPos(SB_HORZ, _HautGcheFenetre.x / _LargeurPolice) ;
}

LRESULT
NSRefVue::EvMouseWheel(WPARAM wParam, LPARAM lParam)
{
  // WORD    fwKeys  = LOWORD(wParam) ;           // key flags
  short   zDelta  = (short) HIWORD(wParam) ;   // wheel rotation
  // short   xPos    = (short) LOWORD(lParam) ;   // horizontal position of pointer
  // short   yPos    = (short) HIWORD(lParam) ;   // vertical position of pointer

  UINT scrollCode ;

  // A positive value indicates that the wheel was rotated forward,
  // away from the user
  //
  if (zDelta > 0)
    scrollCode = SB_LINEUP ;

  // a negative value indicates that the wheel was rotated backward,
  // toward the user.
  //
  else if (zDelta < 0)
    scrollCode = SB_LINEDOWN ;

  for (int i = 0; i < 4; i++)
    EvVScroll(scrollCode, 0, 0) ;

  return 0 ;
}

boolNSRefVue::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;

	NS_CLASSLIB::TPoint ptClick = point ;
	//
	// On récupère un "contexte d'information" concernant l'écran
	//
	TIC* pDC = new TIC("DISPLAY", NULL, NULL) ;
	pDC->SetMapMode(MM_LOMETRIC) ;
	//
	// On transforme les unités physiques d'écran en unités logiques
	//
	pDC->DPtoLP(&ptClick) ;
	delete pDC ;
	//
	// On positionne le point dans la page
	//
	ptClick.x += _HautGcheFenetre.x ;
	ptClick.y += _HautGcheFenetre.y ;
	//
	// On cherche si ce point se situe sur une ligne
	// Attention, les ordonnées sont < 0
	//
  NSLignesArrayIter iter ;
  int ligneEnCours = 1 ;
  bool tourner = true ;
  if (false == _Lignes.empty())
  	for (iter = _Lignes.begin(); (_Lignes.end() != iter) && (tourner) ; )
  	{
  		if ((ptClick.x >= (*iter)->Boite.left) && (ptClick.x <= (*iter)->Boite.right) &&
			    (ptClick.y <= (*iter)->Boite.top)  && (ptClick.y >= (*iter)->Boite.bottom) )
    		tourner = false ;
    	else
    	{
    		iter++ ;
      	ligneEnCours++ ;
    	}
		}
	//
	// Si ce point se situe sur une ligne, on cherche la phrase correspondante
	//
	if (false == tourner)
	{
  	// cas du Shift-click : on sélectionne un bloc
    if (GetKeyState(VK_SHIFT) < 0)
    {
    	if (_iLigneEnCours)
      {
      	_iLigneSelect = ligneEnCours - _iLigneEnCours ;
        Invalidate() ;
      }
      return true ;
    }
    // on désélectionne le bloc ou la ligne, et on change éventuellement de ligne
    _iLignePrec = _iLigneEnCours ;

    if (_iLigneSelect != 0) // (s'il y a un bloc)
    {
    	// on remplace le bloc par la nouvelle ligne
      _iLigneEnCours = ligneEnCours ;
    }
    else
    {
    	if (_iLigneEnCours != ligneEnCours)
      	_iLigneEnCours = ligneEnCours ;
      else // pour pouvoir désélectionner une ligne
      	_iLigneEnCours = 0 ;
    }

    // on efface le bloc, et on repeind
    _iLigneSelect = 0 ;
    Invalidate() ;
    return true ;
	}

	return false ;
}

// Fonction de réponse à la commande Composer////////////////////////////////////////////////////////////////void
NSRefVue::CmComposer()
{
	_pDoc->Composer();
}

// Fonction de réponse à la commande Publier////////////////////////////////////////////////////////////////
void
NSRefVue::CmPublier()
{
	_pDoc->Publier();
}

// Fonction de réponse à la commande Visualiser////////////////////////////////////////////////////////////////
void
NSRefVue::CmVisualiser()
{
	_pDoc->Visualiser();
}

// Fonction CmImprime (non implementee)////////////////////////////////////////////////////////////////
void
NSRefVue::CmImprime()
{
	NSPrinter* pPrinter = _pDoc->pContexte->getSuperviseur()->getApplication()->getFrame()->Printer ;
	if (pPrinter)
	{
		NSCRPrintout printout("Echocardiographie", this) ;
		//printout.SetBanding(true);
		//pPrinter->Print(this, printout, false);
		pPrinter->Print(_pDoc->pContexte->GetMainWindow(), printout, false) ;
	}
}

//---------------------------------------------------------------------------//  Fonction : 	EcranToLignes(TPoint* pPoint)
//
//  Description : Convertion d'un point de l'écran en point de l'espace des
//						Lignes.
//
//  Returns:	  	Numéro de page dans l'espace des Lignes
//---------------------------------------------------------------------------
UINT
NSRefVue::EcranToLignes(NS_CLASSLIB::TPoint* pPoint)
{
  if (NULL == pPoint)
    return 0 ;

	//
	// On transforme les unités physiques d'écran en unités logiques
	//
	_pTIC->DPtoLP(pPoint) ;
	//
	// On positionne le point dans la page
	//
	pPoint->x += _HautGcheFenetre.x ;
	pPoint->y += _HautGcheFenetre.y ;
	//
	// On calcule la page en cours
	//
  UINT i;
  if (_RectangleGlobal.Height() != 0)
		i = pPoint->x / _RectangleGlobal.Height() ;
  else
    i = 0 ;

	return i ;
}

//---------------------------------------------------------------------------//  Fonction : 	LignesToEcran(TPoint* pPoint)
//
//  Description : Convertion d'un point de l'espace des Lignes en point écran
//
//  Returns:	  	True si le point est à l'écran, False sinon
//---------------------------------------------------------------------------
bool
NSRefVue::LignesToEcran(NS_CLASSLIB::TPoint* pPoint)
{
	//
	// On recale le point en coordonnées de l'écran
	//
	pPoint->x -= _HautGcheFenetre.x ;
	pPoint->y -= _HautGcheFenetre.y ;
	//
	// On transforme les unités logiques en unités physiques d'écran
	//
	return _pTIC->LPtoDP(pPoint) ;
}

voidNSRefVue::initMUEViewMenu(string sMenuName){#ifdef N_TIERS	if (_pMUEViewMenu)
  	delete _pMUEViewMenu ;

	nsMenuIniter menuIter(_pDoc->pContexte) ;
	_pMUEViewMenu = new OWL::TMenuDescr ;
  menuIter.initMenuDescr(_pMUEViewMenu, sMenuName) ;
#endif

	return ;}voidNSRefVue::activateMUEViewMenu(){#ifdef N_TIERS	if (NULL == _pMUEViewMenu)
		return ;

	TMyApp* pMyApp = _pDoc->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->GetMainWindow()->SetMenuDescr(*_pMUEViewMenu) ;
#endif
	return ;}
//---------------------------------------------------------------------------//  Fonction : 	NSRefVue::InitialiseLignes()
//
//  Description : Devrait etre virtuelle pure, mais celà pose des problèmes
//						aux Templates ; alors elle est déclarée mais ne fait rien.
//
//  Returns:	  	Rien
//---------------------------------------------------------------------------
bool
NSRefVue::InitialiseLignes()
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Méthodes de NSCRPrintout
//
NSCRPrintout::NSCRPrintout(const char* title, NSRefVue* pCRVue)
             :TPrintout(title)
{
  _pCRView = pCRVue ;
}

void
NSCRPrintout::BeginDocument(int startPage, int endPage, unsigned flags)
{
	TPrintout::BeginDocument(startPage, endPage, flags);
}

void
NSCRPrintout::BeginPrinting()
{
	TPrintout::BeginPrinting();
}

void
NSCRPrintout::EndDocument()
{
	TPrintout::EndDocument();
}

void
NSCRPrintout::EndPrinting()
{
	TPrintout::EndPrinting();
}

void
NSCRPrintout::SetPrintParams(TPrintDC* pDC, NS_CLASSLIB::TSize pageSize)
{
  NSLigne*			      pLigne;
	string 			        Chaine, ChaineLigne;
	NSStyleParagraphe   StylePara;
	TFont*				      pPolice;
	UINT				        NumeroPara, NumeroPolice, NumeroPage;
	size_t				      Curseur, NouBlanc, PreBlanc, NbrBlancs;
	NS_CLASSLIB::TPoint PointRef, PointTransfert, PageLogique;
	ifstream 			      inFile;
	UINT				        TraiteLigne, LargeurPossible, LargeurGauche, LargeurChaine;
	NS_CLASSLIB::TSize	TailleChaine;
	NSFont		 		      PoliceVoulue;
  NSPage*				      pStylePage, stylePage;
  string				      sFichierCR;

	TPrintout::SetPrintParams(pDC, pageSize) ;
	//
	// Remise à zéro des LignesPrnt
	//
	_pCRView->_LignesPrnt.vider() ;
	//
	// Tentative d'ouverture du fichier qui contient le compte rendu
	//
  NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pCRView->GetDoc()) ;  if ((NSCRDocument*) NULL == pCRDoc)    return ;
  sFichierCR = pCRDoc->getTextFile() ;	inFile.open(sFichierCR.c_str()) ;
	if (!inFile)
		return;
	int iAncDC = pDC->SaveDC() ;
	//
	// Fixe la métrique ; par défaut c'est le pixel, mais le mode MM_LOMETRIC
	// est beaucoup plus commode
	//
	pDC->SetMapMode(MM_LOMETRIC) ;
	//
	// On convertit la taille de la page des unités physiques (pixel) aux unités
	// logiques (0,1 mm)
	//
	PageLogique.x = pageSize.cx ;
	PageLogique.y = pageSize.cy ;
	pDC->DPtoLP(&PageLogique) ;
	//
	// On initialise le style du paragraphe et la police qui correspond
	//
	NumeroPara 	 = 1 ;
	StylePara 	 = *(_pCRView->_StylesParagraphe[NumeroPara-1]) ;
	NumeroPolice = StylePara.NumPolice ;
	//
	// La taille de la police est stockée en points, on la ramène en multiple
	// de 0,1 mm
	//
	PoliceVoulue = *(_pCRView->_StylesPolice[NumeroPolice-1]) ;
	PoliceVoulue.logFont.lfHeight = PoliceVoulue.logFont.lfHeight * 254 / 72 ;
	PoliceVoulue.logFont.lfWidth  = PoliceVoulue.logFont.lfWidth  * 254 / 72 ;
	//
	// On sélectionne la police dans le DC
	//
	pPolice = new TFont(&(PoliceVoulue.logFont));
	pDC->SelectObject(*pPolice);
	//
	// On initialise le point de référence en haut et à gauche
	//
	NumeroPage = 1;
  pStylePage = _pCRView->donnePage(NumeroPage) ;
  if (pStylePage == 0)
    pStylePage = _pCRView->donnePage(0);

  if (pStylePage != 0)
    stylePage = *pStylePage;
  else
    stylePage.loadDefault();

  NS_CLASSLIB::TRect	RectanglePage, RectangleTexte;
  RectanglePage.Set(stylePage.MargeGauche,
                      -stylePage.MargeHaute,
                      PageLogique.x-stylePage.MargeDroite,
                      PageLogique.y+stylePage.MargeBasse);

  RectangleTexte.Set(RectanglePage.Left() + stylePage.MargeTxtGch,
                      RectanglePage.Top() - stylePage.MargeTxtHaut,
                      RectanglePage.Right() - stylePage.MargeTxtDte,
                      RectanglePage.Bottom() + stylePage.MargeTxtBas);
  //
	// On initialise le point de référence en haut et à gauche
	//
	PointRef.x = RectangleTexte.Left() ;
  PointRef.y = RectangleTexte.Top() ;
	//
	// Lecture du fichier paragraphe par paragraphe
	//
	while (getline(inFile, Chaine, '\n'))
	{
		if (strlen(Chaine.c_str()) > 0)
		{
			//
			// Si le paragraphe commence par un indicateur de lésions, on saute
			//
			if (Chaine[0] == '<')
			{
				UINT k = 1;
				while (Chaine[k] != '>')
					k++;
				k++;
				Chaine = string(Chaine, k, strlen(Chaine.c_str()) - k);
			}
    }
    if (strlen(Chaine.c_str()) > 0)
		{
			//
			// Si le paragraphe commence par un changement de style
			//
			if (Chaine[0] == char(27))
			{
				NumeroPara 	 = UINT(Chaine[1]) - UINT('0');
				StylePara 	 = *(_pCRView->_StylesParagraphe[NumeroPara-1]);
				NumeroPolice = StylePara.NumPolice;
				delete pPolice;
        pPolice = 0;
				PoliceVoulue = *(_pCRView->_StylesPolice[NumeroPolice-1]);
        PoliceVoulue.logFont.lfHeight = PoliceVoulue.logFont.lfHeight * 254 / 72;
				PoliceVoulue.logFont.lfWidth  = PoliceVoulue.logFont.lfWidth  * 254 / 72;
				pPolice = new TFont(&(PoliceVoulue.logFont));
				Chaine = string(Chaine, 2, strlen(Chaine.c_str()) - 2);
				pDC->SelectObject(*pPolice);
			}
			//
			// On ote les éventuels début/fin de paragraphe
			//
			size_t i = Chaine.find(char(27)) ;
			while (NPOS != i)
			{
        if (0 == i)
          Chaine = string(Chaine, 2, strlen(Chaine.c_str()) - 2);
        else if (i >= strlen(Chaine.c_str()) - 2)
          Chaine = string(Chaine, 0, i);
        else
          Chaine = string(Chaine, 0, i) +
                                string(Chaine, i+2, strlen(Chaine.c_str())-i-2);
					i = Chaine.find(char(27));
			}
			i = Chaine.find(char(28));
			while (NPOS != i)
			{
        if (0 == i)
          Chaine = string(Chaine, 2, strlen(Chaine.c_str()) - 2) ;
        else if (i >= strlen(Chaine.c_str()) - 2)
          Chaine = string(Chaine, 0, i) ;
        else
          Chaine = string(Chaine, 0, i) +
                          string(Chaine, i+2, strlen(Chaine.c_str())-i-2) ;
        i = Chaine.find(char(28));
			}
		}
		//
		// On prévoit l'espace au dessus du paragraphe
		//
		PointRef.Offset(0, -StylePara.EspaceDessus) ;
		//
		// Calcul de la largeur utilisable sans le retrait gauche
		//
		LargeurGauche = RectangleTexte.Width() - StylePara.RetraitDroit ;
		//
		// On traite le paragraphe, ligne par ligne
		//
		Curseur     = 0 ;
    PreBlanc    = 0 ;
		TraiteLigne = 1 ;
		while (TraiteLigne > 0)
		{
      //
      // Création d'une ligne vierge
      //
      pLigne = new NSLigne() ;
      //
      // Initialisation de tous les paramètres déjà connus
      //
      pLigne->numPage = NumeroPage ;
      if (TraiteLigne == 1)
        pLigne->Boite.left  = StylePara.RetraitPremLigne ;
      else
        pLigne->Boite.left  = StylePara.RetraitGauche ;
      pLigne->Boite.right   = LargeurGauche ;
      pLigne->Boite.top	  	= PointRef.y ;
      pLigne->Boite.bottom  = PointRef.y ;
      // pLigne->CouleurTexte  = *(StylePara.pCouleurTexte);
      pLigne->numParagraphe = NumeroPara ;
      pLigne->numLigne		  = TraiteLigne ;
      //
      // Traitement des chaines de taille non nulle
      //
      if (strlen(Chaine.c_str()) > 0)
      {
        //
        // Détermination de la largeur utilisable
        //
        LargeurPossible = UINT(pLigne->Boite.Width());
        //
        // Recherche du segment de chaîne qui "tient" dans cette largeur        //
        NouBlanc = Curseur;
        do        {
          PreBlanc = NouBlanc;
          NouBlanc = Chaine.find(' ', PreBlanc+1);
          if (NPOS == NouBlanc)
            break;
          TailleChaine = pDC->GetTextExtent(string(Chaine, Curseur, NouBlanc-Curseur).c_str(),
                               int(NouBlanc-Curseur));
        }
        while (TailleChaine.cx < LargeurPossible);
        //
        // Si on est en fin de chaîne, on regarde si le dernier mot "tient"
        //
        if (NPOS == NouBlanc)
        {
          TailleChaine = pDC->GetTextExtent(string(Chaine, Curseur, strlen(Chaine.c_str())-Curseur).c_str(),
                               int(strlen(Chaine.c_str())-Curseur));
          if (TailleChaine.cx < LargeurPossible)
            PreBlanc = strlen(Chaine.c_str());
        }
        //
        // On met le segment de chaine "qui tient" dans la chaine Texte
        // de la ligne en cours
        //
        pLigne->Texte = string(Chaine, Curseur, PreBlanc-Curseur);
        //
        // On calcule la vraie taille de cette chaine
        //
        strip(pLigne->Texte) ;
        TailleChaine = pDC->GetTextExtent(pLigne->Texte.c_str(),
                                      int(strlen(pLigne->Texte.c_str()))) ;
        //
        // On initialise Boite : rectangle exinscrit à la ligne
        //
        if (TailleChaine.cy < StylePara.HauteurLigne)
          TailleChaine.cy = StylePara.HauteurLigne;
        pLigne->Boite.bottom -= TailleChaine.cy;
        PointRef.Offset(0, -TailleChaine.cy);

        //
        //
        //
        if (PreBlanc < strlen(Chaine.c_str()))
        {
          Curseur = PreBlanc + 1;
          TraiteLigne++;
          pLigne->DerniereLigne = false;
        }
        else
        {
          TraiteLigne = 0;
          pLigne->DerniereLigne = true;
        }
      }
      //
      // Traitement des chaines vides
      //
      else
      {
        pLigne->Texte = "";
        //
        // On initialise Boite : rectangle exinscrit à la ligne
        //
        TailleChaine = pDC->GetTextExtent(" ", 1);
        if (TailleChaine.cy < StylePara.HauteurLigne)
          TailleChaine.cy = StylePara.HauteurLigne;
        pLigne->Boite.bottom -= TailleChaine.cy;
        PointRef.Offset(0, -TailleChaine.cy);
        //
        // On précise que le traitement est terminé
        //
        TraiteLigne = 0;
        pLigne->DerniereLigne = true;
      }
          //
      // Si la ligne déborde de la page, elle passe en page suivante
      //
      if (abs(PointRef.y) > abs(RectangleTexte.Height()))
      {
        NumeroPage++ ;
        PointRef.y = 0 ;
        pLigne->numPage = NumeroPage ;
        pLigne->Boite.top	   = PointRef.y ;
        pLigne->Boite.bottom = -TailleChaine.cy ;
      }
      //
      // On ajoute la ligne à Lignes
      //
      _pCRView->_LignesPrnt.push_back(new NSLigne(*pLigne)) ;
      delete pLigne ;
      pLigne = 0 ;
		}
		//
		// On prévoit l'espace au dessous du paragraphe
		//
		PointRef.Offset(0, -StylePara.EspaceDessous);
		Chaine = "";
	}
	delete pPolice ;
  pPolice =0 ;
	inFile.close() ;
	pDC->RestoreDC(iAncDC) ;
	return ;
}

voidNSCRPrintout::PrintPage(int page, NS_CLASSLIB::TRect& rect, unsigned)
{
  // Conditionally scale the DC to the window so the printout will
  // resemble the window
  //
  int    prevMode;
  NS_CLASSLIB::TSize oldVExt, oldWExt;
  //
  // Demande à ce rectangle, dans cette page, de se peindre
  //
  _pCRView->PaintPrnt(*DC, false, page, rect) ;
}

// Do not enable page range in the print dialog since only one page is// available to be printed
//
void
NSCRPrintout::GetDialogInfo(int& minPage, int& maxPage,
										 int& selFromPage, int& selToPage)
{
  minPage = 1 ;
  maxPage = 1 ;
  selFromPage = selToPage = 1 ;
}

bool
NSCRPrintout::HasPage(int pageNumber)
{
	return pageNumber == 1 ;
}

