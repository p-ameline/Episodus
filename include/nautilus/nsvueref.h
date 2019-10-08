//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSVUEREF_H
#define __NSVUEREF_H

// #include <owl\owlpch.h>
#include <owl\dc.h>
#include <owl\inputdia.h>
#include <owl\chooseco.h>
#include <owl\gdiobjec.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>
#include <owl\listbox.h>
#include <owl\controlb.h>
#include <owl\printer.h>
#include <classlib\arrays.h>

#include "ns_sgbd\nsannexe.h"
#include "dcodeur\nstxstyl.h"
#include "dcodeur\nstxstar.h"
#include "nsbb\nsbbsmal.h"
#include "nautilus\nsdocref.h"

#include "dcodeur\nscr_ana.h"#include "nautilus\nautilus.rh"

class  NSRefVue : public TWindowView, public NSRoot
{
	protected:

    NSRefDocument* 	  	_pDoc ;

    NS_CLASSLIB::TPoint _HautGcheFenetre ;   // Coordonnées "Document" du                                               // point origine de l'écran
    UINT				        _PremLigne ;		   // numéro de la ligne du haut
    LONG				        _LargeurPolice ;
    OWL::TControlBar*   _ToolBar ;
    OWL::TMenu*			    _pMenuLigne ;
    OWL::TPopupMenu*    _pPopupMenuLigne ;

    TIC* 				        _pTIC ;

    TMenuDescr*         _pMUEViewMenu ;
    HACCEL              _hAccelerator ;

    NSSmallBrother*   	_pBigBoss ;

    int                 _iScreenResH, _iScreenResW ;
    int                 _iFontForScreenResH, _iFontForScreenResW ;

    bool isCloserScreen(const string sChapter, size_t iScreenPos) ;
    int  getExcessSurface(int iResW, int iResH) const ;

	public:

    NSFontArray			       _StylesPolice, _StylesPolImp ;
    NSStyleParagrapheArray _StylesParagraphe ;

    // Descriptif des cadres
    NSCadreDecorArray		   _Cadres ;

    // Descriptif des pages (cadres décor, cadres de texte)
    NSPageArray				     _Pages ;

    // Variables
    NS_CLASSLIB::TRect	   _RectangleGlobal ;
    NSRectArray		  	     _RectDecors ;
    NS_CLASSLIB::TColor	   _CouleurFond ;

    // Lignes de texte, issues du traitement des pages et des cadres
    NSLigneArray		       _Lignes, _LignesPrnt ;
    int 				           _iLigneEnCours ;
    int                    _iLignePrec ;
    int                    _iLigneSelect ;

    long				           _MargeDte, _MargeGche, _MargeHaute, _MargeBasse ;

    string				         _ImpDriver, _ImpDevice, _ImpOutput ;

    NSRefVue(NSRefDocument& doc, NSContexte* pCtx, TWindow* parent = 0);

    TDocument docutemp;

    NSRefVue() : TWindowView(docutemp), NSRoot(NULL) { }
    ~NSRefVue();

    // Fonction pour récupérer le pointeur sur le document
    NSRefDocument* GetDoc() { return _pDoc ; }

    // Fonctions de réponse aux messages    void    Paint(TDC&, bool, NS_CLASSLIB::TRect& RectangleAPeindre);
    void    PaintPrnt(TDC& dc, bool, int page, NS_CLASSLIB::TRect& RectAPeindre);
    void    EvKeyDown(uint key, uint repeatCount, uint flags);
    void    EvSysCommand(uint cmdType, NS_CLASSLIB::TPoint& point);
    void    EvSysKeyDown(uint key, uint repeatCount, uint flags);
    bool    EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point);
    void    EvVScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl);
    void    EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl);
    LRESULT EvMouseWheel(WPARAM wparam, LPARAM lparam);
    void    CopyToClipboard();
    void    SelectAll();

    void CmPublier();
    void CmComposer();
    void CmVisualiser();

    void CmImprime();

    // Implémentation des méthodes virtuelles pures de TView
    static LPCSTR StaticName() { return "NSRefVue"; }
    const char* GetViewName() { return StaticName(); }
    TWindow*    GetWindow()    { return (TWindow*) this; }

    // Overrides TWindows' in order to process our accelerators table
    bool PreProcessMsg(MSG &msg) ;

    // Fonction d'initialisation
    void SetupWindow();
    bool Initialisation();
    bool InitAspect(char type);
    bool InitialiseMode(char type);
    bool InitialiseImpData();
    bool InitialisePolicesImp();
    virtual bool InitialiseLignes();  	// Devrait etre virtuelle pure, mais
													// celà pose problème aux Templates
    bool InitModePolice(string* pChaineNom, string* pChaineContenu);

    NSPage* donnePage(unsigned int numPage);

    NSCadreDecor* donneDecor(string* titreDecor);
    void		  metDecor(NS_CLASSLIB::TRect* rectPage, TDC* pDC, unsigned indicePage, int ecranImpri);

    // Fonctions de conversion des coordonnées Lignes en coord. écran
    UINT EcranToLignes(NS_CLASSLIB::TPoint* pPoint);
    bool LignesToEcran(NS_CLASSLIB::TPoint* pPoint);

		void initMUEViewMenu(string sMenuName) ;
    void activateMUEViewMenu() ;

	DECLARE_RESPONSE_TABLE(NSRefVue) ;
};

class  NSCRPrintout : public TPrintout {

	public:

		NSCRPrintout(const char* title, NSRefVue* pCRVue) ;

		void BeginDocument(int startPage, int endPage, unsigned flags) ;
		void BeginPrinting() ;
		void EndDocument() ;
		void EndPrinting() ;
		void GetDialogInfo(int& minPage, int& maxPage,
														int& selFromPage, int& selToPage) ;
		void SetPrintParams(TPrintDC* dc, NS_CLASSLIB::TSize pageSize) ;
		void PrintPage(int page, NS_CLASSLIB::TRect& rect, unsigned flags) ;
		void SetBanding(bool b) { Banding = b ; }
		bool HasPage(int pageNumber) ;

	protected:

		NSRefVue* _pCRView ;
};

#endif // __NSCRVUE_H
