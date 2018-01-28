//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSCRVUE_H
#define __NSCRVUE_H

// #include <owl\owlpch.h>
#include <owl\dc.h>
#include <owl\inputdia.h>
#include <owl\chooseco.h>
#include <owl\gdiobjec.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>
#include <owl\listbox.h>
#include <classlib\arrays.h>

#include "nautilus\nsannexe.h"
#include "nautilus\nstxstyl.h"
#include "nautilus\nstxstar.h"
#include "nautilus\nsdocref.h"
#include "nautilus\nsvueref.h"
#include "nautilus\nscrdoc.h"
#include "nautilus\nscr_ana.h"

#define	IDC_CORRIGER      10
#define IDC_REPRENDRE_CR  20
#define IDC_SUPPRIMER     30
#define IDC_COPY_ARBRE	  40
#define IDC_TLIB_ORDO     50
#define IDC_MEDIC_ALD     60
#define IDC_MEDIC_NONALD  70

class NSCRReadOnlyView : public NSRefVue
{
  public:

  	string	_NomDuFichier ;
    string 	_sLocalisationSelectionne ;  //localisation d'un élément
                                         //patpatho correspondant à
                                         //ligne du CR

    bool 		_bSetupToolBar ;
    bool    _bFermerDocument ;           //mis à true quand on ferme un document
    bool    _bEnConclusion ;             //mis à true pendant la conclusion
    bool    _bEnCodage ;                 //mis à true pendant le codage

		NSCRReadOnlyView(NSCRDocument& doc, TWindow* parent = 0) ;

		~NSCRReadOnlyView() ;

    void ReprendreCR() ;
    bool ReprendreCompteRendu() ;
		bool nouveauCR() ;

    void CorrigerCR() ;
		bool corrigeCR() ;
    void CopyArbre() ;
    void SupprimerLigne() ;
    bool SupprimerLigneCR() ;
    void TLibreOrdonnance() ;
    void MedicEnALD() ;
    void MedicHorsALD() ;

		// Fonctions de réponse aux messages
		void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void EvSetFocus(THandle hWndLostFocus /* may be 0 */) ;
		void CmEnregistre() ;
    void CmQuickSave() ;
    void CmFileClose() ;
		// Implémentation des méthodes virtuelles pures de TView		static LPCSTR StaticName() { return "NSCRReadOnlyView"; }
		const char* GetViewName()   {return StaticName();}
    TWindow*    GetWindow()     { return (TWindow*) this; }

		// Fonction d'initialisation
		void SetupWindow() ;
    void SetupToolBar() ;

		int  Initialisation() ;

		bool InitialiseLignes() ;
    void VaA27(string *pChaine, size_t *pik, NSCRPhraLesArray *pLesBuf, NSCRPhraLesArray *pLesPath) ;

    void Conclusion() ;
    void Compta() ;
    void Codage() ;
    bool CanClose() ;
    void EvClose() ;
    void EvDestroy() ;
    bool EvQueryEndSession() ;

	DECLARE_RESPONSE_TABLE(NSCRReadOnlyView) ;
};

#endif // __NSCRVUE_H

