//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#include <owl\owlpch.h>
#include <owl\dc.h>
#include <owl\inputdia.h>
#include <owl\chooseco.h>
#include <owl\gdiobjec.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>
#include <winsys\color.h>
#include <iostream.h>
#include <cstring.h>

#ifdef _INCLUS  #include "partage\Nsglobal.h"
  #include "nsbb\nsutidlg.h"
#endif

#include "partage\nsdivfct.h"#include "nautilus\nscr_anx.h"
#include "nautilus\nscrvue.h"

// -------------------------------------------------------------------------// ------------------------- METHODES DE NSLigne ---------------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------//  Constructeur par défaut
//---------------------------------------------------------------------------
NSLigne::NSLigne()
        :Boite(0, 0, 0, 0), BoitePrnt(0, 0, 0, 0), CouleurTexte(0, 0, 0)
{
	iNumPhrase 	  = 0 ;
	Texte 		    = string("") ;
	numPage 	    = 0 ;
	numLigne 	    = 0 ;
	DerniereLigne = true ;
  numParagraphe = 1 ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSLigne::~NSLigne()
{}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSLigne::NSLigne(const NSLigne& rv)
{
	iNumPhrase	  = rv.iNumPhrase ;
	Texte 		  	= rv.Texte ;
	numPage 		  = rv.numPage ;
	numLigne 	  	= rv.numLigne ;
	DerniereLigne = rv.DerniereLigne ;
	CouleurTexte  = rv.CouleurTexte ;
	Boite			    = rv.Boite ;
  BoitePrnt	  	= rv.BoitePrnt ;

  numParagraphe 	= rv.numParagraphe ;  StyleParagraphe = rv.StyleParagraphe ;
}

//---------------------------------------------------------------------------//  Fonction	 : NSLigne::EffacerTexte()
//
//  Description : Efface simplement le texte (garde le contexte)
//---------------------------------------------------------------------------
void
NSLigne::EffacerTexte()
{
	Texte = string("") ;
}

//---------------------------------------------------------------------------//  Fonction	 : NSLigne::Reinitialiser()
//
//  Description : Remet entièrement à zéro
//---------------------------------------------------------------------------
void
NSLigne::Reinitialiser()
{
	iNumPhrase 	  = 0 ;
	Texte 		    = string("") ;
	numPage 	    = 0 ;
	numLigne 	    = 0 ;
	DerniereLigne = true ;

	numParagraphe = 1 ;
  StyleParagraphe.Reinitialiser() ;
}

//---------------------------------------------------------------------------//  Fonction	 : NSLigne::peindre(TPoint* Origine)
//
//  Paramètres	 : dc		  : contexte de périphérique en cours
//						Origine : position par rapport au document du coin
//									 supérieur gauche de l'écran
//                pVue	  : vue du document à imprimer
//						support : écran ou imprimante
//
//  Description : Affichage de la ligne à l'écran
//---------------------------------------------------------------------------
bool
NSLigne::Peindre(TDC& dc, NS_CLASSLIB::TPoint* pOrigine, NSRefVue* pVue, int support, bool bInvert)
{
try
{
	SIZE  	            lpSize;
	NS_CLASSLIB::TPoint	PointAffichage, PointTransfert;
	TFont*	            pPolice;
	NSFont	            NSPolice;
	BOOL		            Continuer;

    //int	iAncDC = dc.SaveDC();

	//
	// Récupération du style de paragraphe qui formate la ligne
	//
  NSStyleParagraphe Paragraphe ;
  if (numParagraphe > 0)
    Paragraphe = *(pVue->_StylesParagraphe[numParagraphe-1]) ;
  else
    Paragraphe = StyleParagraphe ;
  //
	// Sélection de la police prévue par le style de paragraphe en fct du support
	//
	switch (support)
	{
    case NSLIGNEPNT_ECRAN :

      NSPolice = *(pVue->_StylesPolice[Paragraphe.NumPolice-1]);
      //
      // La taille de la police est stockée en
      // unité physiques d'écran, il faut donc
      // la transformer en unités logiques
      //
      PointTransfert.x = NSPolice.logFont.lfHeight;
      PointTransfert.y = NSPolice.logFont.lfWidth;
      dc.DPtoLP(&PointTransfert);
      NSPolice.logFont.lfHeight = PointTransfert.x;
      NSPolice.logFont.lfWidth  = PointTransfert.y;
      break;

    case NSLIGNEPNT_PRNT  :
      NSPolice = *(pVue->_StylesPolImp[Paragraphe.NumPolice-1]);
      //
      // La taille de la police est stockée en
      // points, il faut donc la transformer en
      // unités logiques (0,1 mm)
      //
      NSPolice.logFont.lfHeight = NSPolice.logFont.lfHeight * 254 / 72;
      NSPolice.logFont.lfWidth  = NSPolice.logFont.lfWidth  * 254 / 72;
      break;
  }
	pPolice = new TFont(&(NSPolice.logFont));
	dc.SelectObject(*pPolice);

	//
	// Calcul de la taille effective de la ligne
	//
	Continuer = GetTextExtentPoint(HDC(dc), Texte.c_str(), strlen(Texte.c_str()), &lpSize);
	if (!Continuer)
	{
        dc.RestoreFont();
		delete pPolice;
		return false;
	}
	//
	// Calcul du point écran où afficher
	//
    NS_CLASSLIB::TPoint PointBD;

	PointAffichage.x = Boite.left;
	PointAffichage.y = Boite.top;
	PointAffichage -= *pOrigine;

    PointBD.x = Boite.right;
	PointBD.y = Boite.bottom;
	PointBD -= *pOrigine;

    NS_CLASSLIB::TRect RectangleTexte(PointAffichage, PointBD);

	//
	// Préparation de l'affichage suivant le type de justification
	//
	UINT EspaceVide = Boite.Width() - lpSize.cx;
	dc.SetTextJustification(0, 0);
	if 	  (Paragraphe.Alignement == TA_RIGHT)
		PointAffichage.x += EspaceVide;
	else if (Paragraphe.Alignement == TA_CENTER)
		PointAffichage.x += EspaceVide / 2;
	else if ((Paragraphe.Alignement != TA_LEFT) && (!DerniereLigne))
	{
		// Recherche du nombre d'"entre mots"
		UINT NbBlancs = 0;
		UINT pos = 0;
		do
		{
			pos = Texte.find(' ', pos);
			if (pos != NPOS)
			{
				while (Texte[pos] == ' ')
						pos++;
				NbBlancs++;
			}
		}
		while (pos != NPOS);
		// Calcul de la taille à donner aux blancs pour justifier
		dc.SetTextJustification(EspaceVide, NbBlancs);
	}
	dc.TextOut(PointAffichage, Texte.c_str());
	dc.RestoreFont();

    if (bInvert)
        dc.InvertRect(RectangleTexte);

	delete pPolice;
	return true;
}
catch (...)
{
	erreur("Exception NSLigne::Peindre.", standardError, 0) ;
	return true ;
}
}

//---------------------------------------------------------------------------//  Fonction	 : NSLigne::PeindreRectangle(TPoint* Origine)
//
//  Description : Affichage de la boite qui contient la ligne
//---------------------------------------------------------------------------
bool
NSLigne::PeindreRectangle(TDC& dc, NS_CLASSLIB::TPoint* pOrigine, NSRefVue* pVue, int support)
{
try
{
	SIZE  	            lpSize;
	NS_CLASSLIB::TPoint	PointTransfert;
	TFont*	            pPolice;
	NSFont	            NSPolice;
	BOOL		        Continuer;
	//
	// Récupération du style de paragraphe qui formate la ligne
	//
  NSStyleParagraphe Paragraphe;
  if (numParagraphe > 0)
    Paragraphe = *(pVue->_StylesParagraphe[numParagraphe-1]);
  else
    Paragraphe = StyleParagraphe;
	//
	// Sélection de la police prévue par le style de paragraphe en fct du support
	//
	switch (support)
	{
		case NSLIGNEPNT_ECRAN :

      NSPolice = *(pVue->_StylesPolice[Paragraphe.NumPolice-1]) ;
      //
      // La taille de la police est stockée en
      // unité physiques d'écran, il faut donc
      // la transformer en unités logiques
      //
      PointTransfert.x = NSPolice.logFont.lfHeight;
      PointTransfert.y = NSPolice.logFont.lfWidth;
      dc.DPtoLP(&PointTransfert);
      NSPolice.logFont.lfHeight = PointTransfert.x;
      NSPolice.logFont.lfWidth  = PointTransfert.y;
      break;

		case NSLIGNEPNT_PRNT  :

      NSPolice = *(pVue->_StylesPolImp[Paragraphe.NumPolice-1]);
      //
      // La taille de la police est stockée en
      // points, il faut donc la transformer en
      // unités logiques (0,1 mm)
      //
      NSPolice.logFont.lfHeight = NSPolice.logFont.lfHeight * 254 / 72;
      NSPolice.logFont.lfWidth  = NSPolice.logFont.lfWidth  * 254 / 72;      break;
	}
	pPolice = new TFont(&(NSPolice.logFont));
	dc.SelectObject(*pPolice);

	//
	// Calcul de la taille effective de la ligne
	//
	Continuer = GetTextExtentPoint(HDC(dc), Texte.c_str(), strlen(Texte.c_str()), &lpSize);
	if (!Continuer)
	{
		delete pPolice;
		return false;
	}
	//
	// Calcul du rectangle écran où afficher
	//
  NS_CLASSLIB::TPoint PointHG, PointBD;

	PointHG.x = Boite.left;
	PointHG.y = Boite.top;
	PointHG -= *pOrigine;
  PointBD.x = Boite.right;
	PointBD.y = Boite.bottom;
	PointBD -= *pOrigine;
  //
  // Affichage de la boite en rouge clair
  // Les couleurs sont dans winsys/color.h
  //
  TBrush Pinceau(TColor::LtRed);

  dc.FillRect(PointHG.x, PointHG.y, PointBD.x, PointBD.y, Pinceau);

	return true;
}
catch (...)
{
  erreur("Exception NSLigne::PeindreRectangle.", standardError, 0);
  return true;
}
}

//---------------------------------------------------------------------------//  Fonction	 : NSLigne::PeindreCurseur(TPoint* Origine)
//
//  Description : Affichage d'un curseur au niveau de la ligne
//---------------------------------------------------------------------------
bool
NSLigne::PeindreCurseur(TDC& dc, NS_CLASSLIB::TPoint* pOrigine, NSRefVue* pVue, int support, bool bPeindre)
{
try
{
	SIZE  	            lpSize;
	NS_CLASSLIB::TPoint	PointTransfert;
	TFont*	            pPolice;
	NSFont	            NSPolice;
	BOOL		            Continuer;
  //
	// Récupération du style de paragraphe qui formate la ligne
	//
  NSStyleParagraphe Paragraphe;
  if (numParagraphe > 0)
    Paragraphe = *(pVue->_StylesParagraphe[numParagraphe-1]);
  else
    Paragraphe = StyleParagraphe;
	//
	// Sélection de la police prévue par le style de paragraphe en fct du support
	//
	switch (support)
	{
    case NSLIGNEPNT_ECRAN :

      NSPolice = *(pVue->_StylesPolice[Paragraphe.NumPolice-1]);
      //
      // La taille de la police est stockée en
      // unité physiques d'écran, il faut donc
      // la transformer en unités logiques
      //
      PointTransfert.x = NSPolice.logFont.lfHeight;
      PointTransfert.y = NSPolice.logFont.lfWidth;
      dc.DPtoLP(&PointTransfert);
      NSPolice.logFont.lfHeight = PointTransfert.x;
      NSPolice.logFont.lfWidth  = PointTransfert.y;
      break;

		case NSLIGNEPNT_PRNT  :

      NSPolice = *(pVue->_StylesPolImp[Paragraphe.NumPolice-1]);
      //
      // La taille de la police est stockée en
      // points, il faut donc la transformer en
      // unités logiques (0,1 mm)
      //
      NSPolice.logFont.lfHeight = NSPolice.logFont.lfHeight * 254 / 72;
      NSPolice.logFont.lfWidth  = NSPolice.logFont.lfWidth  * 254 / 72;
      break;
  }
	pPolice = new TFont(&(NSPolice.logFont));
	dc.SelectObject(*pPolice);

	//
	// Calcul de la taille effective de la ligne
	//
	Continuer = GetTextExtentPoint(HDC(dc), Texte.c_str(), strlen(Texte.c_str()), &lpSize);
	if (!Continuer)
	{
    dc.RestoreFont();
		delete pPolice;
		return false;
	}
	//
	// Calcul du rectangle du curseur
	//
  NS_CLASSLIB::TPoint PointHG, PointBD;

  // Attention les ordonnées sont négatives
	PointHG.x = Boite.left - 50;
	PointHG.y = Boite.bottom + ((Boite.top - Boite.bottom) / 2) + 5;
	PointHG -= *pOrigine;
  PointBD.x = Boite.left - 40;
	PointBD.y = Boite.bottom + ((Boite.top - Boite.bottom) / 2) - 5;
	PointBD -= *pOrigine;
  //
  // Affichage de la boite en rouge clair
  // Les couleurs sont dans winsys/color.h
  //
  TBrush PinceauRouge(TColor::LtRed);
  TBrush PinceauBlanc(TColor::White);

  if (bPeindre)
    dc.FillRect(PointHG.x, PointHG.y, PointBD.x, PointBD.y, PinceauRouge) ;
  else
    dc.FillRect(PointHG.x, PointHG.y, PointBD.x, PointBD.y, PinceauBlanc) ;

  dc.RestoreFont() ;
  delete pPolice ;
	return true ;
}
catch (...)
{
  erreur("Exception NSLigne::PeindreCurseur.", standardError, 0);
  return true;
}
}

//---------------------------------------------------------------------------
//  Fonction	 : NSLigne::operator=(NSLigne src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSLigne&
NSLigne::operator=(const NSLigne& src)
{
  if (this == &src)
		return *this ;

	iNumPhrase    = src.iNumPhrase;	Texte         = src.Texte;
	numPage       = src.numPage;
	numLigne 	  	= src.numLigne;
	DerniereLigne = src.DerniereLigne;
	CouleurTexte  = src.CouleurTexte;
	Boite         = src.Boite;
  BoitePrnt	  	= src.BoitePrnt;

  numParagraphe 	= src.numParagraphe;
  StyleParagraphe = src.StyleParagraphe;

	return *this;
}

//---------------------------------------------------------------------------//  Fonction	 : NSLigne::operator == (const NSLigne& o)
//
//  Description : Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSLigne::operator == (const NSLigne& o)
{
  if ((iNumPhrase	   == o.iNumPhrase)	   &&
      (Texte 		     == o.Texte) 		   &&
      (numPage 	     == o.numPage) 	   &&
      (numLigne 	   == o.numLigne) 	   &&
      (DerniereLigne == o.DerniereLigne) &&
      (numParagraphe == o.numParagraphe) &&
      (CouleurTexte  == o.CouleurTexte)  &&
      (Boite		     == o.Boite))
    return 1 ;
  return 0 ;
}

