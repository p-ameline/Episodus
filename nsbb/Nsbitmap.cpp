//**************************************************************************
//  NOM du FICHIER  NSBITMAP.CPP
//
//  Mise en place de la classe NSBitmap qui est un contrôle bitmap
//  interactif.
//  On définit un ensemble de régions qui partitionnent le bitmap et qui
//  doivent simuler le fonctionnement de boutons.
//  On transfert une matrice de booléans qui correspondent aux régions
//  cochées.
//
//  Auteur : KRZISCH Ph.
// Date de création : août 94                 mise à jour :
//**************************************************************************

#include <stdlib.h>              // atoi, div
//#include "..\include\nsutil.h"   // ptrInit et strAnalyse
//#include "ns_vector.h"
#include "nsutil\nscache.h"
#include "partage\nsglobal.h"
#include "nsbb\nsbitmap.h"


BOOL    ptrInit(char** strPtr, int length, char car = ' ');
BOOL    ptrInit(char** strPtr, const char* strInit);

// Extern défini dans owl.cpp
extern LRESULT FAR PASCAL _export InitWndProc(HWND, UINT, WPARAM, LPARAM);
extern HINSTANCE hInst;


//--------------------------------------------------------------------------
// Implémentation de DefRegionArray
//--------------------------------------------------------------------------

DefRegionArray::DefRegionArray(DefRegionArray& rv)
					:DefArray()
{
	 for (IterDefRegion i = rv.begin(); i != rv.end(); i++)
   	push_back(new DefRegion(*(*i)));
}

//---------------------------------------------------------------------------
//  Vidange de l'array
//---------------------------------------------------------------------------
void
DefRegionArray::vider()
{
	for (IterDefRegion i = begin(); i != end(); )
   {
   	delete *i;
      erase(i);
   }
}

DefRegionArray::~DefRegionArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Function: DefRegion::DefRegion(string szName,
//                                 const TPoint pPoints)
//  Arguments:
//            szName   ->  Nom de la région
//            pPoints  ->  Matrice de Tpoints définissant les sommets
//  Description:
//       Construit une DefRegion.
//  Returns:
//       RIEN
//---------------------------------------------------------------------------
DefRegion::DefRegion(const char* name, const TPoint* pPoints)
{
  nbVertices = sizeof(pPoints)/ sizeof(TPoint);

  if ( nbVertices > 2 ) {
	 szName =  string(1,*name);
	 pVertices = new TPoint[sizeof(pPoints)/ sizeof(TPoint)];
	 for(int i = 0; i < nbVertices; i++)
		pVertices[i] = *new TPoint(pPoints[i]);
  }
  else {
	//  Message d'erreur puisque pour définir une région il faut plus de
	//  2 sommets.
  }
}
//---------------------------------------------------------------------------
//  Function: DefRegion::DefRegion(const string info,
//                                 const char* separateurs = "/(,)"
//  Arguments:
//        info         ->  String de configuration récupérée dans la
//                         ressource WINDOWS
//        separateurs  ->  Liste des séparateurs utilisés
//  Description:
//       Construit une DefRegion à partir d'une chaîne (string BORLAND)
//       en analysant cette chaîne. Il faut préciser la liste des séparateurs
//       utilisée.( Par défaut : "/(,)" )
//       Les informations de configuration d'une région se présente de la
//       manière suivante :
//       "Rouge:103,10/58,76/102,109/115,98"
//  Returns:
//       RIEN
//---------------------------------------------------------------------------
DefRegion::DefRegion(const string info, const char* separateurs)
{
  nbVertices = 0;
  int i, j, x, y;


  //analyser la chaine
  AnalyseChaine(info, &szName, &nbVertices, separateurs);


  if (nbVertices > 2)
  {
    // Matrice des différents sommets
	 pVertices = new TPoint[nbVertices];
    RemplirMatrice(info, separateurs);
  }
  else {
	//  Message d'erreur puisque pour définir une région il faut plus de
	//  2 sommets et définir un point 2 entiers.
  }
}

//------------------------------------------------------------------------
//	extraire de sChaine le nom de la région et le nombre de vertices
// exemple : 	sChaine = "Rouge:103,10/58,76/102,109/115,98"
//             *pNom = "Rouge"
//					*pNbVert = 4 : 3 + 1
//------------------------------------------------------------------------
void
DefRegion::AnalyseChaine(string sChaine, string* pNom, int* pNbVert,
                          const char* separateurs)
{
	size_t debut = 0;
   size_t posit = sChaine.find(separateurs[0]);

   if(posit == NPOS)
    	*pNbVert = 0;

   else
   {
    	while ((posit != NPOS) && (posit < strlen(sChaine.c_str())))
    	{
      	debut = posit + 1;
         (*pNbVert)++;
      	posit = sChaine.find(separateurs[0], debut+1);
    	}
      (*pNbVert) = (*pNbVert) + 1;
    }

	//nom de la region
   *pNom = string("");
   size_t posNom = sChaine.find(separateurs[1]);
   if(posNom != NPOS)
   	*pNom = string(sChaine, 0, posNom);
}

//------------------------------------------------------------------------
//	extraire de sChaine les sommets et les mettre dans pVertices
// exemple : 	sChaine = "Rouge:103,10/58,76/102,109/115,98"
//								(103,10) (58,76) (102,109) (115,98)
//------------------------------------------------------------------------
void
DefRegion::RemplirMatrice(string sChaine, const char* separateurs)
{
   int i, j, x, y;
   string X;
   string Y;
	size_t posVirgule = sChaine.find(separateurs[3]); //position de la virgule
   size_t posSeparateur = sChaine.find(separateurs[0]);

   size_t positDeuxPoint = sChaine.find(separateurs[1]);

   string sInfo = sChaine;

   if((posVirgule != NPOS) && (posSeparateur != NPOS) && (positDeuxPoint != NPOS))
   {
      X = string(sChaine, positDeuxPoint + 1, posVirgule - positDeuxPoint - 1);
      Y = string(sChaine, posVirgule + 1, posSeparateur - posVirgule - 1 );

   	x = atoi(X.c_str());
		y = atoi(Y.c_str());
		pVertices[0] = *new TPoint(x, y);
   }
   i = 1;

   while(posSeparateur != NPOS )
   {
      if(i == 1)
      {
      	//sInfo = 58,76/102,109/115,98
   		sInfo = string(sChaine, posSeparateur + 1, strlen(sChaine.c_str()));
      }

      posSeparateur = sInfo.find(separateurs[0]);
      posVirgule = sInfo.find(separateurs[3]); //position de la virgule

      X = string(sInfo, 0, posVirgule);
      Y = string(sInfo, posVirgule + 1, posSeparateur - posVirgule - 1 );

   	x = atoi(X.c_str());
		y = atoi(Y.c_str());
		pVertices[i] = *new TPoint(x, y);
      i++;
      sInfo = string(sInfo, posSeparateur + 1, strlen(sChaine.c_str()));
      posSeparateur = sInfo.find(separateurs[0]);

   }

   //dernier point
   posVirgule = sInfo.find(separateurs[3]); //position de la virgule
   X = string(sInfo, 0, posVirgule);
   Y = string(sInfo, posVirgule + 1, strlen(sInfo.c_str()));
   x = atoi(X.c_str());
   y = atoi(Y.c_str());
   pVertices[i] = *new TPoint(x, y);

}

//---------------------------------------------------------------------------
//  Function: DefRegion::~DefRegion()
//  Arguments:
//            AUCUN
//  Description:
//            Destructeur
//  Returns:
//       RIEN
//---------------------------------------------------------------------------
DefRegion::~DefRegion()
{
  for(int i =0; i < nbVertices; i++)
	 delete &(pVertices[i]);
  delete pVertices;
}
//---------------------------------------------------------------------------
//  Function: DefRegion::DefRegion::operator=(DefRegion src)
//  Arguments:
//            src -> DefRegion que l'on veut affecter.
//  Description:
//            Opérateur =
//  Returns:
//            Référence à une DefRegion
//---------------------------------------------------------------------------
DefRegion&
DefRegion::operator=(const DefRegion& src)
{
  if (this == &src)
    return *this ;

  szName = src.szName ;

  if (pVertices)
    delete[] pVertices ;
  pVertices = new TPoint[src.nbVertices] ;
  for (int i = 0; i < src.nbVertices; i++)
    pVertices[i] = *new TPoint(src.pVertices[i]) ;

  nbVertices = src.nbVertices ;

  return *this ;
}
//---------------------------------------------------------------------------
//  Function: DefRegion::operator==(const DefRegion& x)
//  Arguments:
//            x -> DefRegion que l'on veut comparer à *this
//  Description:
//            Opérator==
//  Returns:
//---------------------------------------------------------------------------
int DefRegion::operator==(const DefRegion& x)
{
  if( (!(x.szName == szName)) && (x.nbVertices == nbVertices)) {
	 for(int i = 0; i < nbVertices; i++) {
		if(x.pVertices[i] != pVertices[i])
		  return 0;
	 }
	 return 1;
  }
  return 0;
}

//--------------------------------------------------------------------------
// Implémentation de DefRegionArray
//--------------------------------------------------------------------------

NSRegionArray::NSRegionArray(NSRegionArray& rv)
					:RegionArray()
{
	 for (IterNSBitmapRegion i = rv.begin(); i != rv.end(); i++)
   	push_back(new NSBitmapRegion(*(*i)));
}

//---------------------------------------------------------------------------
//  Vidange de l'array
//---------------------------------------------------------------------------
void
NSRegionArray::vider()
{
	for (IterNSBitmapRegion i = begin(); i != end(); )
   {
   	delete *i;
      erase(i);
   }
}

NSRegionArray::~NSRegionArray()
{
	vider();
}

//--------------------------------------------------------------------------
// Implémentation de NSBitmapRegion
//--------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Function: NSBitmapRegion::NSBitmapRegion(NSBitmap* Aparent,
//                                           DefRegion& defRegion,
//										 int fillMode)
//  Arguments:
//        Aparent    ->  Adresse du NSBitmap qui crée l'objet
//        defRegion  ->  Référence de la matrice de TPoint qui correspondent
//                       aux sommets de la région que l'on définit
//        fillMode   ->  ALTERNATE ou WINDING
//  Description:
//       Construit une NSRegion en positionnant le pointeur sur le Bitmap
//       parent et passe au constructeur de TRegion les différents paramètres
//       nécessaires.
//  Returns:
//       RIEN
//---------------------------------------------------------------------------
NSBitmapRegion::NSBitmapRegion(NSBitmap* Aparent, DefRegion& defRegion,
										 int fillMode)
: TRegion(defRegion.pVertices, sizeof(defRegion.pVertices)/sizeof(TPoint),
			 fillMode),
  pBitmap(Aparent), iState(0)
{
  szName = defRegion.szName;
}
//--------------------------------------------------------------------------
// Implémentation de l'enregistrement d'une classe
//--------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Function: BOOL NSBitmapRegister()
//
//  Arguments:
//          AUCUN
//  Description:
//            Vérifie si la classe NSBitmap n'est pas déjà enregistrée.
//            Si elle ne l'est pas l'enregistre.
//  Returns:
//          TRUE   si la classe n'était pas enregistrée
//          FALSE  sinon
//---------------------------------------------------------------------------
BOOL NSBitmapRegister()
{
  WNDCLASS  windowClass;
  BOOL gc;
  static char className[] = "NSBitmap";

  // Only check for globally registered classes if not in an NT WoW box,
  // since WoW plays nasty games with class registration.
#if defined(__WIN32__)
  gc = ::GetClassInfo(0, "NSBitmap", &windowClass);
#else
  static BOOL isWoW = BOOL(::GetWinFlags()&0x4000);
  if (!isWoW)
	 gc = ::GetClassInfo(0, "NSBitmap", &windowClass);
  else
	 gc = 0;
#endif

  if (!gc && !::GetClassInfo(0, "NSBitmap", &windowClass)) {
	 memset(&windowClass, 0, sizeof windowClass);
	 windowClass.cbClsExtra = 0;
	 windowClass.cbWndExtra = 0;
	 windowClass.hInstance = *::Module;
	 windowClass.hIcon = 0;
	 windowClass.hCursor = ::LoadCursor(0, IDC_CROSS);
	 windowClass.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	 windowClass.lpszMenuName = 0;
	 windowClass.lpszClassName = className;
	 windowClass.style = CS_HREDRAW|CS_VREDRAW;
	 windowClass.lpfnWndProc =::DefWindowProc;  //InitWndProc;
	 return ::RegisterClass(&windowClass);
  }



  return TRUE;
}


//---------------------------------------------------------------------------
//  Function: BOOL NSBitmapButRegister()
//
//  Arguments:
//          AUCUN
//  Description:
//            Vérifie si la classe NSBitmap n'est pas déjà enregistrée.
//            Si elle ne l'est pas l'enregistre.
//  Returns:
//          TRUE   si la classe n'était pas enregistrée
//          FALSE  sinon
//---------------------------------------------------------------------------
BOOL NSBitmapButRegister()
{
  WNDCLASS  windowClass;
  BOOL gc;
  static char className[] = "NSBitBut";

  // Only check for globally registered classes if not in an NT WoW box,
  // since WoW plays nasty games with class registration.
#if defined(__WIN32__)
  gc = ::GetClassInfo(0, "NSBitBut", &windowClass);
#else
  static BOOL isWoW = BOOL(::GetWinFlags()&0x4000);
  if (!isWoW)
	 gc = ::GetClassInfo(0, "NSBitBut", &windowClass);
  else
	 gc = 0;
#endif

  if (!gc && !::GetClassInfo(0, "NSBitBut", &windowClass)) {
	 memset(&windowClass, 0, sizeof windowClass);
	 windowClass.cbClsExtra = 0;
	 windowClass.cbWndExtra = 0;
	 windowClass.hInstance = *::Module;
	 windowClass.hIcon = 0;
	 windowClass.hCursor = ::LoadCursor(0, IDC_CROSS);
	 windowClass.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	 windowClass.lpszMenuName = 0;
	 windowClass.lpszClassName = className;
	 windowClass.style = CS_HREDRAW|CS_VREDRAW;
	 windowClass.lpfnWndProc =::DefWindowProc;  //InitWndProc;
	 return ::RegisterClass(&windowClass);
  }
  return TRUE;
}
//--------------------------------------------------------------------------
// Implémentation de NSBitmap
//--------------------------------------------------------------------------

// Déclaration de la table de réponse
DEFINE_RESPONSE_TABLE1(NSBitmap, TControl)
  EV_WM_PAINT,
  EV_WM_LBUTTONDOWN,
  EV_WM_LBUTTONUP,
  EV_WM_LBUTTONDBLCLK,
  EV_WM_MOUSEMOVE,
  EV_WM_KEYDOWN,
  EV_WM_SETFOCUS,
  EV_WM_KILLFOCUS,
  EV_WM_GETDLGCODE,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Function: NSBitmap::NSBitmap(TWindow* Aparent, int idCtrl, int idBmp,
//                               DefRegion regionArray[],
//                               int xpos = 0, int ypos = 0,
//                               BOOL capture = FALSE)
//  Arguments:
//           Aparent     -> Parent de la fenêtre
//           idCtrl      -> ID de la ressource pour le contrôle
//           idBmp       -> ID de la ressource pour le bitmap
//           regionArray -> Matrice des DefRegion pour créer les régions
//           xpos
//           ypos        -> Position initiale du curseur
//           capture     -> Capture du focus
//  Description:
//       Positionne toutes les données membres.
//       Crée un pointeur sur un TBitmap à partie de la ressource bitmap
//       d'ID idBmp.
//  Returns:
//
//---------------------------------------------------------------------------
NSBitmap::NSBitmap(TWindow* Aparent, int idCtrl, int idBmp,
						 DefRegionArray* regionArray,
						 int xpos , int ypos, BOOL capture)

			: TControl(Aparent,idCtrl), nXcurr(xpos), nYcurr(ypos), bCapture(capture),
           IDBmp(idBmp)
{
	// Création des NSBitmapRegions
  	pRegionArray = new NSRegionArray;

  	for (IterDefRegion i = regionArray->begin(); i != regionArray->end(); i++)
	  	pRegionArray->push_back(new NSBitmapRegion(this, *(*i)));
}



//---------------------------------------------------------------------------
//  Function: NSBitmap::~NSBitmap()
//
//  Arguments:
//            AUCUN
//  Description:
//            Détruit NSBitmapRegionArray.
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
NSBitmap::~NSBitmap()
{
  delete pRegionArray;
 /* if(pControle)
  {
      delete pControle;
  		pControle = 0;
  }*/
}


//---------------------------------------------------------------------------
//  Description:	Constructeur copie
//---------------------------------------------------------------------------
NSBitmap::NSBitmap(const NSBitmap& src)
             :TControl((TWindow *)src.GetParent(), src.GetId(), src.GetModule())
{
   pControle = src.pControle;
   bCapture  = src.bCapture;               // Indicateur de capture du focus
	IDBmp = src.IDBmp;                  // ID de la ressource bitmap
   pDib = src.pDib;                   // Pointeur sur un DIB
}

//---------------------------------------------------------------------------
//  Description:	Surcharge de l'opérateur d'affectation
//---------------------------------------------------------------------------
NSBitmap&
NSBitmap::operator=(const NSBitmap& src)
{
  if (this == &src)
    return *this ;

  pControle = src.pControle ;
  bCapture  = src.bCapture ;               // Indicateur de capture du focus
  IDBmp     = src.IDBmp ;                  // ID de la ressource bitmap
  pDib      = src.pDib ;                   // Pointeur sur un DIB

  return *this ;
}

//---------------------------------------------------------------------------
//  Function: UINT NSBitmap::Transfer(void* buffer, TTransferDirection direction)
//
//  Arguments:
//            buffer    ->  Adresse du buffer de transfert
//            direction ->  Direction de transfert
//  Description:
//        Transfert le nom des régions cochées sous forma de structures
//        NSBitmap::TransferStruct
//  Returns:
//        Taille des données transferées
//  Remarque :
//    Cette taille n'est pas utilisée cela est maintenu pour des raisons
//    de compatibilité avec OWL.
//---------------------------------------------------------------------------
UINT NSBitmap::Transfer(void* buffer, TTransferDirection direction)
{
  return 0;
}
//---------------------------------------------------------------------------
//  Function: void NSBitmap::InvertRgn(NSBitmapRegion& region)
//
//  Arguments:
//            region -> region que l'on veut inverser
//  Description:
//            Inverse les couleurs d'une région
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
/*
void NSBitmap::InvertRgn(NSBitmapRegion& region)
{
  TClientDC Dc;
  Dc.InvertRgn(region);
}
*/
//---------------------------------------------------------------------------
//  Function: void NSBitmap::Affiche(TDC& Dc)
//                                 RefAdd
//  Arguments:
//            Dc -> DC valide dans le contexte
//  Description:
//       Affiche le curseur de la fenêtre i.e. dessine une croix.
//       Ne crée pas de DC pour les opérations de tracé celui-ci est
//       disponible dans les fonctions appelantes.
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
void NSBitmap::Affiche(TDC& Dc)
{
  TPen pen = TPen(CreatePen(PS_SOLID, 3, RGB(0,0,0)));
  int xpos=nXcurr, ypos=nXcurr;    // Position courante dans le bitmap

// Mode pour préserver le fond en cas de dessin
  Dc.SetROP2(R2_NOT);

// Sélection du crayon
  Dc.SelectObject(pen);

// Tracé d'une croix
  Dc.MoveTo(xpos-10, ypos-10);
  Dc.LineTo(xpos+10, ypos+10);
  Dc.MoveTo(xpos-10, ypos+10);
  Dc.LineTo(xpos+10, ypos-10);

// Rétablissement du crayon d'origine
  Dc.RestorePen();

// Mise à jour de la position courante
  nXcurr = xpos;
  nYcurr = ypos;
}

//---------------------------------------------------------------------------
//  Function: void NSBitmap::LoadBitmapResource(WORD ResId)
//
//  Arguments:
//            ResId -> ID de la ressource bitmap
//  Description:
//            Charge un bitmap à partir d'une ressource
//  Returns:
//            TRUE   si le chargement s'est bien passé
//            FALSE  sinon
//---------------------------------------------------------------------------
BOOL NSBitmap::LoadBitmapResource(TResId ResId)
{
	TDib* newDib;
  	try
   {
	 	newDib = new TDib(HINSTANCE(*(GetModule())), ResId);
  	}
  	catch (/*TGdiBase::*/TXGdi& x)
   {
	 	MessageBox("On ne peut accéder à la ressource bitmap",
					 	GetModule()->GetName(),
						MB_OK);
   	return FALSE;
  	}
  	pDib = newDib;
  	return TRUE;
}
//---------------------------------------------------------------------------
//  Function: void NSBitmap::LoadBitmapFile(const char* name)
//
//  Arguments:
//            name  -> Nom du fichier contenant le bitmap
//  Description:
//      Charge un bitmap à partir d'un fichier.
//      Teste si le fichier passé est un WINDOWS 3.0 DI (ou PM 1.x) bitmap
//
//  Returns:
//            TRUE   si tout c'est bien passé
//            FALSE  sinon
//---------------------------------------------------------------------------
BOOL NSBitmap::LoadBitmapFile(const char* name)
{
	TDib* newDib = 0;
  	try
  	{
	 	newDib = new TDib(name);
  	}
  	catch (/*TGdiBase::*/TXGdi& x)
   {
	 	MessageBox("On ne peut ouvrir le fichier bitmap",
					 								GetModule()->GetName(), MB_OK);
	 	return FALSE;
  	}
  	pDib = newDib;
  	return TRUE;
}
//---------------------------------------------------------------------------
//  Function: void NSBitmap::Paint(TDC& Dc, BOOL erase, TRect& rect)
//
//  Arguments:
//           Dc    ->  Contexte de périphérique
//           erase ->  Indique si le fond doit-être effacé
//           rect  ->  Rectangle qui circonscrit la zône qui doit-être
//                     repeinte
//  Description:
//       Appelée par les classes de base en réponse au message WM_PAINT
//       via evPaint() pour réafficher le contenu de la fenêtre.
//       Crée pDragDC si il n'est pas NULL et le détruit à la fin.
//  Returns:
//        RIEN
//---------------------------------------------------------------------------
void NSBitmap:: EvPaint()
{
  TPaintDC  Dc(*this);
  TRect&   rect = *(TRect*)&Dc.Ps.rcPaint;

  //TRect rect = GetClientRect();

// Efface le pointeur
  Affiche(Dc);

// Trace le tour de fenêtre en gris si la fenêtre à le focus
  if(GetFocus() == HWindow)
	 Dc.FrameRect(rect, TBrush(::GetStockObject(GRAY_BRUSH)));
  else {
	 TBrush br(GetSysColor(COLOR_WINDOW));
	 Dc.FrameRect(rect, br);
  }

// Affiche le bitmap
//------------------
 /*
  TMemoryDC memDC(Dc);
  TBitmap bitmap(GetModule()->GetInstance(), IDBmp);
  TDib dib(GetModule()->GetInstance(), TResId(IDBmp));
  TBitmap bitmap(dib, &TPalette((HPALETTE)::GetStockObject(DEFAULT_PALETTE)));
  memDC.SelectObject(bitmap);
  memDC.SetMapMode(Dc.GetMapMode());

  int x = bitmap.Width();
  int y = bitmap.Height();

 // Dc.DPtoLP(&pt, 1);

  Dc.BitBlt(0,0, x, y, memDC, 0,0, SRCCOPY);
  memDC.RestoreBitmap();
  x = DeleteObject(bitmap);
  y = DeleteObject(dib);
*/
  Dc.SetDIBitsToDevice(rect, TPoint(0,0), *pDib);
// Affiche le pointeur
  Affiche(Dc);
}
//---------------------------------------------------------------------------
//  Function: void NSBitmap::EvKeyDown(UINT key, UINT repeatCount, UINT flags)
//
//  Arguments:
//          key         ->
//          repeatCount ->
//          flags       ->
//  Description:
//     Réponse à l'enfoncement d'une touche. Ne tient pas compte que
//     des flèches.
//  Returns:
//
//---------------------------------------------------------------------------
void NSBitmap::EvKeyDown(UINT key, UINT repeatCount, UINT flags)
{
  	static int nInc;     // Incrément de déplacement
  	TRect rect;

  	// Position du curseur
  	#if defined __WIN32__
  		long xpos, ypos;
   #else
      int  xpos, ypos;
   #endif
  	xpos = nXcurr;
  	ypos = nYcurr;

	// Création du DC
  	TClientDC	dragDC(*this);

  	if(key == VK_DOWN || key == VK_UP || key == VK_RIGHT || key == VK_LEFT)
   {
	 	Affiche(dragDC);    // Efface le curseur
	 	if(repeatCount == 1) // Si l'état précédent était appuyé
			nInc = 1;
	 	else
			nInc += repeatCount;
  		switch(key)
   	{
	 		case VK_DOWN  : ypos += nInc;
						    	 break;
	 		case VK_UP    : ypos -= nInc;
						  		 break;
	 		case VK_RIGHT : xpos += nInc;
						  		 break;
	 		case VK_LEFT  : xpos -= nInc;
						  		 break;
  		}
  		rect = GetClientRect();  // Teste les bornes
  		ypos = max(ypos, long(0));
  		ypos = min(ypos, rect.bottom);
  		xpos = max(xpos, long(0));
  		xpos = min(xpos, rect.right);
  		nXcurr = xpos;
  		nYcurr = ypos;

 		// Affiche le curseur
      Affiche(dragDC);
  	}
}

//---------------------------------------------------------------------------
//  Function: void EvSetFocus(HWND hWndLostFocus)
//
//  Arguments:
//           hWndLostFocus -> Handle de la fenêtre qui a perdu le focus
//  Description:
//           Répond gain du focus.
//  Returns:
//           RIEN
//---------------------------------------------------------------------------
void NSBitmap::EvSetFocus(HWND hWndLostFocus)
{
  TRect rect = GetClientRect();
  TBrush br(/*GetStockObject(BLACK_BRUSH)*/GetSysColor(COLOR_WINDOW));
  TClientDC dragDC(*this);

  dragDC.FrameRect(rect, br);
}
//---------------------------------------------------------------------------
//  Function: void NSBitmap::EvKillFocus(HWND hWndGetFocus)
//
//  Arguments:
//            hWndGetFocus ->  Handle de la fenêtre qui vient d'obtenir
//                             le focus
//  Description:
//            Réponse à la perte du focus.
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
void NSBitmap::EvKillFocus(HWND hWndGetFocus)
{
  TRect rect = GetClientRect();
  TBrush br(GetStockObject(GRAY_BRUSH));
  TClientDC dragDC(*this);

  dragDC.FrameRect(rect, br);
}
//---------------------------------------------------------------------------
//  Function: void NSBitmap::EvMouseMove(UINT modKeys, TPoint& point)
//
//  Arguments:
//            modKeys ->
//            point   ->
//  Description:
//            Réponse au mouvement de la souris
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
 void NSBitmap::EvMouseMove(UINT modKeys, TPoint& point)
{
  TClientDC dragDC(*this);

  if(bCapture){
	 Affiche(dragDC);
	 nXcurr = point.x;
	 nYcurr = point.y;
	 Affiche(dragDC);
  }
}
//---------------------------------------------------------------------------
//  Function: void NSBitmap::EvLButtonDown(UINT modKeys, TPoint& point)
//
//  Arguments:
//          modKeys  ->
//          point    ->
//  Description:
//        Appui sur le bouton gauche de la souris.
//        Crée un DC mais ne le détruit pas. Cette destruction doit être
//        effectuée dans EvLButtonUp, i.e. quand il y relâchement du bouton.
//  Returns:
//        RIEN
//---------------------------------------------------------------------------
void NSBitmap::EvLButtonDown(UINT modKeys, TPoint& point)
{
  ::SetFocus(HWindow);
  ::SetCapture(HWindow);
  bCapture = TRUE;
  ::ShowCursor(FALSE);

  TClientDC dragDC(*this);

  Affiche(dragDC);
  nXcurr = point.x;
  nYcurr = point.y;
  Affiche(dragDC);
}
//---------------------------------------------------------------------------
//  Function: void NSBitmap::EvLButtonUp(UINT modKeys, TPoint& point)
//
//  Arguments:
//           modKeys ->
//           point   -> Point correspondant à la position du curseur
//  Description:
//
//  Returns:
//
//---------------------------------------------------------------------------
void NSBitmap::EvLButtonUp(UINT modKeys, TPoint& point)
{
  if(bCapture) {
	 ReleaseCapture();
	 bCapture = FALSE;
	 ::ShowCursor(TRUE);
  }
}
//---------------------------------------------------------------------------
//  Function: void NSBitmap::EvLButtonDblClk(UINT modKeys, TPoint& point)
//
//  Arguments:
//            modKeys  ->
//            point    ->
//  Description:
//
//  Returns:
//
//---------------------------------------------------------------------------
void NSBitmap::EvLButtonDblClk(UINT modKeys, TPoint& point)
{
}
//---------------------------------------------------------------------------
//  Function: UINT NSBitmap::EvGetDlgCode(MSG far* msg)
//
//  Arguments:
//            msg ->
//  Description:
//            Indique quelles sont les touches gérées par la fenêtre.
//  Returns:
//
//---------------------------------------------------------------------------
/* Version pour un Edit
 UINT
TEdit::EvGetDlgCode(MSG far* msg)
{
  UINT retVal = (UINT)TStatic::EvGetDlgCode(msg);
  if (!IsValid(FALSE))
	 retVal |= DLGC_WANTTAB;
  return retVal;
}
*/
UINT NSBitmap::EvGetDlgCode(MSG far*)
{
  return DLGC_WANTARROWS;
}

