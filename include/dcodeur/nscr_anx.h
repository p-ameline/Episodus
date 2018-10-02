//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSCR_ANX_H
#define __NSCR_ANX_H

#include <owl\owlpch.h>
// #include <owl\dc.h>
// #include <owl\inputdia.h>
// #include <owl\chooseco.h>
// #include <owl\gdiobjec.h>
// #include <owl\docmanag.h>
// #include <owl\filedoc.h>
// #include <owl\listbox.h>

// #include "nautilus\nsannexe.h"
#include "dcodeur\nstxstyl.h"
// #include "nautilus\nsdocref.h"

#if defined(_DKD_DLL)
	#define _NSDKDCLASSE __export
#else
	#define _NSDKDCLASSE __import
#endif

#define NSLIGNEPNT_ECRAN 1
#define NSLIGNEPNT_PRNT  2

class NSRefVue;

class _NSDKDCLASSE NSLigne
{
  public:

    UINT 	    iNumPhrase;			// Num�ro de la phrase de rattachement

    string	    Texte; 				// string qui contient le contenu de la ligne
    UINT		numPage; 			// Num�ro de la page qui contient la ligne
    UINT		numLigne; 			// Num�ro de la ligne dans le paragraphe
                    // et indicateur de derni�re ligne
    bool		DerniereLigne; 	// (utile pour les paragraphes justifi�s)

    NS_CLASSLIB::TRect  	Boite, BoitePrnt; // Boite dans laquelle est contenue la ligne
    NS_CLASSLIB::TColor 	CouleurTexte;

    UINT		numParagraphe; 	// Num�ro du style de paragraphe utilis�
                    // pour cette ligne
    NSStyleParagraphe StyleParagraphe ; // paragraphe

    // Constructeur, constructeur copie et destructeur
    NSLigne() ;
    NSLigne(const NSLigne& rv) ;
    ~NSLigne() ;

    // Fonction d'affichage
    bool Peindre(TDC &dc, NS_CLASSLIB::TPoint* pOrigine, NSRefVue* pVue, int support, bool bInvert = false);
    bool PeindreRectangle(TDC &dc, NS_CLASSLIB::TPoint* pOrigine, NSRefVue* pVue, int support);
    bool PeindreCurseur(TDC &dc, NS_CLASSLIB::TPoint* pOrigine, NSRefVue* pVue, int support, bool bPeindre = true);

    // Fonctions d'initialisation
    void EffacerTexte() ;   // Efface simplement le texte (garde le contexte)
    void Reinitialiser() ;  // Remet enti�rement � z�ro

    // Op�rateurs d'affectation et de comparaison
    NSLigne& operator=(const NSLigne& src) ;
    int operator == (const NSLigne& o) ;
    int operator != (const NSLigne& o) { return !(*this==o) ; }
};

#endif // __NSCR_ANX_H

