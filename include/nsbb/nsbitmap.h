//**************************************************************************
//  NOM du FICHIER  NSBITMAP.H
//
//  Mise en place de la classe NSBitmap qui correspond aux bitmaps
//  interactifs contenus dans les boîte de dialogue
//
//  Auteur : KRZISCH Ph.
// Date de création :                  mise à jour :
//**************************************************************************
#ifndef __NSBITMAP_H
#define __NSBITMAP_H


#ifndef __CONTROL_H
#include <owl\control.h>
#endif

#ifndef __POINT_H
#include <owl\point.h>
#endif

#ifndef __ARRAYS_H
#include <classlib\arrays.h>
#endif

#ifndef __DC_H
#include <owl\dc.h>
#endif

#ifndef __GDIOBJEC_H
#include <owl\gdiobjec.h>
#endif


#include "nsbb\nsbb_dlg.h"

/*#ifndef __WINGDI_H
#include <owl\wingdi.h>
#endif
*/
// Fonction qui sert à enregistrer la classe NSBitmap
BOOL NSBitmapRegister();
BOOL NSBitmapButRegister();


//--------------------------------------------------------------------------
// Déclaration de DefRegion
// Struct qui sert à initialiser une région d'un NSBitmap.
// Son principal intérêt est son constructeur qui à partir d'une string
// construit tous les arguments nécessaire à la définition d'une TRegion.
//--------------------------------------------------------------------------
struct DefRegion {
  string                szName;           // Nom de la région
  NS_CLASSLIB::TPoint*  pVertices;      // Matrice des différents sommets
  int                   nbVertices;         // Nb. de sommets

 // Constructeurs
  DefRegion(const char* name="", const NS_CLASSLIB::TPoint* pPoints = NULL);
  DefRegion(const string info, const char* separateurs = "/:(,)");

 // Destructeur
  ~DefRegion();
  void AnalyseChaine(string sChaine, string* pNom, int* pNbVert, const char* separateurs);
  void RemplirMatrice(string sChaine, const char* separateurs);

 // Surcharge d'opérateurs
  DefRegion& operator=(const DefRegion& src);
  int operator==(const DefRegion& x);
};
//--------------------------------------------------------------------------
// Déclaration de DefRegionArray
// Classe qui sert à définir les régions d'un NSBitmap sous forme de matrice
// dynamique de DefRegion.
//--------------------------------------------------------------------------
typedef vector <DefRegion*> DefArray;
typedef DefArray::iterator IterDefRegion;

class DefRegionArray : public DefArray
{
	public :
	// Constructeurs
	DefRegionArray() : DefArray() {}
	DefRegionArray(DefRegionArray& rv);
	// Destructeur
	virtual ~DefRegionArray();
   void vider();
};



//--------------------------------------------------------------------------
// Déclaration de NSRegion
// Classe qui sert de région pour un NSBitmap. Elle est initalisée
// grâce à une DefRegion.
//--------------------------------------------------------------------------
class NSBitmap;      // Définition de type forward

class NSBitmapRegion : public TRegion {

  NSBitmap* pBitmap;   // Adresse de la fenêtre NSBitmap
  string szName;        // Nom de la région
  int iState;          // Etat coché ou non ou gray

  public :

 // Constructeurs
  NSBitmapRegion(NSBitmap* Aparent, DefRegion& defRegion, int fillMode =
					  ALTERNATE);
 // Destructeur
  ~NSBitmapRegion() {}
};
//--------------------------------------------------------------------------
// Déclaration de NSRegionArray
// Classe qui sert à gérer les régions d'un NSBitmap sous forme de matrice
// dynamique de NSRegions.
//--------------------------------------------------------------------------

typedef vector <NSBitmapRegion*> RegionArray;
typedef RegionArray::iterator IterNSBitmapRegion;

class NSRegionArray : public RegionArray
{
	public :
	// Constructeurs
	NSRegionArray() : RegionArray() {}
	NSRegionArray(NSRegionArray& rv);
	// Destructeur
	virtual ~NSRegionArray();
   void vider();
};



//--------------------------------------------------------------------------
// Déclaration de NSBitmap
// Dérivé de TControl
// Classe NSBitmap est un bitmap qui doit à partir d'une matrice de
// "régions" simuler un ensemble de boutons.
// Ces régions définissent les zônes actives du bitmap.
//--------------------------------------------------------------------------

// Messages d'obtention et de positionnement de la position courante
// du curseur sur le bitmap

#define NSB_SETPOS WM_USER+10
#define NSB_GETPOS WM_USER+11

class NSBitmap : public OWL::TControl {


   public:

	NSRegionArray* pRegionArray;  // Matrice dynamique de NSRegions

   // Position courante du curseur
   #if defined __WIN32__
		long   nXcurr, nYcurr;
   #else
   	int  	 nXcurr, nYcurr;
   #endif

	BOOL  		bCapture;               // Indicateur de capture du focus
	int   		IDBmp;                  // ID de la ressource bitmap
   TDib* 		pDib;                   // Pointeur sur un DIB
   NSControle* 	pControle;


  // Constructeurs
	NSBitmap(TWindow* Aparent, int idCtrl, int idBmp,
				DefRegionArray* regionArray,
				int xpos = 0, int ypos = 0, BOOL capture = FALSE);

   //copie
   NSBitmap(const NSBitmap& src);
   // Surcharges des opérateurs
   NSBitmap& operator=(const NSBitmap& src) ;



  // Destructeur
	~NSBitmap();
   WNDTYPE donneType() { return(isBtn); };
  // Fonctions diverses
	 virtual UINT Transfer(void* buffer, TTransferDirection direction);


	protected :
	// virtual void InvertRgn(NSBitmapRegion& region);
	 virtual void Affiche(TDC& Dc);

  // Chargement du bitmap
	 BOOL LoadBitmapResource(TResId ResId);
	 BOOL LoadBitmapFile(const char* name);

  // Réponses aux messages
	 void EvKeyDown(UINT key, UINT repeatCount, UINT flags);
	 void EvSetFocus(HWND hWndLostFocus);
	 void EvKillFocus(HWND hWndGetFocus);
	 void EvPaint();
	 void EvMouseMove(UINT modKeys, NS_CLASSLIB::TPoint& point);
	 void EvLButtonDown(UINT modKeys, NS_CLASSLIB::TPoint& point);
	 void EvLButtonUp(UINT modKeys, NS_CLASSLIB::TPoint& point);
	 void EvLButtonDblClk(UINT modKeys, NS_CLASSLIB::TPoint& point);
	 UINT EvGetDlgCode(MSG far* msg);
  DECLARE_RESPONSE_TABLE(NSBitmap) ;};
#endif
