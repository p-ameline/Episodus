// -----------------------------------------------------------------------------// nsepisodview.h
// -----------------------------------------------------------------------------
// Document/Vues Capture Données Utilisateur
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 16:45:28 $
// -----------------------------------------------------------------------------
// FLP - 08/2001
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


#ifndef __NSEPISODVIEW_H__
# define __NSEPISODVIEW_H__

# include <owl\docmanag.h>
# include <owl\docview.h>
# include <owl\mdichild.h>
# include <owl\decframe.h>
# include <owl\statusba.h>
# include <owl\controlb.h>
# include <owl\buttonga.h>
# include <owl\applicat.h>
# include <owl\richedpr.h>
# include <owl\choosefo.h>
# include <owl\listwind.h>

# include "nautilus\nsepicap.h"

# include "nautilus\nsdocref.h"
# include "nautilus\nautilus.rh"
# include "nautilus\richedap.h"
# include "nsbb\nsednum.h"

# define MAXCARS 100000      // Limite du texte pour RichEdit

# define _NSEPISODVIEWCLASS _USERCLASS
# define IDC_ESPISODVIEW     0x0042


// -----------------------------------------------------------------------------
//
// class NSEpisodDocument
// classe générique de documents texte nautilus
//
// -----------------------------------------------------------------------------

class  NSEpisodDocument : public TDocument, public NSRoot
{
 public:
  EpisodusModel* pModel ;

  NSEpisodDocument(TDocument *parent, EpisodusModel *pMod = 0, NSContexte *pCtx = 0) ;
  ~NSEpisodDocument() ;

  // Méthodes virtuelles de TDocument
  bool Open(int mode, const char far *path = 0)   { return true ; }
  bool Close()                                    { return true ; }
  bool IsOpen()                                   { return true ; }
} ;


class NSEpisodListWindow ;


class NSEpisodView : public OWL::TWindowView
{
 public:
  // Constructeur - Destructeur
  NSEpisodView(NSEpisodDocument& doc, TWindow *parent = 0) ;
  ~NSEpisodView() ;

  // Redéfiniton des fonctions virtuelles de TView et TRichEdit
  static LPCSTR   StaticName()                  { return "NSEpisodView" ; }
  const char      *GetViewName()                { return StaticName() ; }
  TWindow         *GetWindow() ;
  bool            CanClose() ;                  // de TEditFile
  //bool          CanClear() ;                  // de TEditFile
  bool            SetDocTitle(const char far* docname, int index)   { return OWL::TWindowView::SetDocTitle(docname, index) ; }

  void            SetupColumns() ;
  void            AfficheListe() ;
  void            AfficheModel() ;
  void            DispInfoEpisodus(TLwDispInfoNotify& dispInfo) ;
  void            EvSetFocus(HWND hWndLostFocus) ;

  string          donneContexte(string sContexte) ;

  NSEpisodDocument* getDoc() { return pDocEpisod ; }

 protected:
  // Remplissage de la vue
  void 	FillViewData() ;
  void 	SetupWindow() ;
  void  SetupTxtBar() ;
  void  InitFormatFont() ;
  void  SetupToolBar() ;

  // view event handlers
  bool 	VnIsWindow(HWND hWnd) ;

  // Fonctions de réponse aux evts
  void    EvSize(uint sizeType, ClassLib::TSize &size) ;

  void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  void    EvChar(uint key, uint repeatCount, uint flags) ;

  // fonction d'apprentissage
  void    EpisodSetClassif() ;
  void    EpisodSetContext() ;

  // Variables
  NSEpisodDocument    *pDocEpisod ;     // Pointeur sur document EPISOD
  OWL::TControlBar    *pToolBar ;	      // Barre d'outils
  OWL::TPrinter       *pPrinter ;       // Printer
  NSEpisodListWindow  *pMyListWindow ;  // ListWindow

  bool	              bSetupToolBar ;

 DECLARE_RESPONSE_TABLE(NSEpisodView) ;
} ;


class NSEpisodListWindow : public TListWindow
{
 public:
  // constructors / destructor
  NSEpisodListWindow() ;
  NSEpisodListWindow(NSEpisodView *parent, int id, int x, int y, int w, int h, TModule* module = 0) ;
  ~NSEpisodListWindow() {}

  // fonction d'apprentissage
  void    EpisodSetClassif() ;
  void    EpisodSetContext() ;
  void    EpisodSetFormat() ;
  void    EpisodSetUnit() ;

  void    EpisodSetAnalyse(int iAnalyseType) ;
  void    EpisodSetAnalyseP() ;
  void    EpisodSetAnalyseO() ;
  void    EpisodSetAnalysePO() ;
  void    EpisodSetAnalyseL() ;
  void    EpisodSetAnalyseN() ;
  void    EpisodSetAnalyseRP() ;
  void    EpisodSetAnalyseRO() ;

  void    CreateSubModel() ;

  string  donneContexte(string sContexte) ;

  // réponse aux événements
  bool    VnIsWindow(HWND hWnd) ;
  void    EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  void    EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
  void    EvSetFocus(HWND hWndLostFocus) ;
  void    DispInfoEpisodus(TLwDispInfoNotify& dispInfo) ;

  void    SetupToolBar() ;
  void    SetupWindow() ;
  void    AfficheListe() ;
  void    AfficheModel() ;

  bool    bSetupToolBar ;

  NSEpisodView *pParentWindow ;

 DECLARE_RESPONSE_TABLE(NSEpisodListWindow) ;
} ;


class NSEpisodScreenView : public OWL::TWindowView
{
 public:
  NS_CLASSLIB::TPoint pointHautGauche ;
  NSBitmap*           pScreenMap ;
  NS_CLASSLIB::TPoint mousePoint ;
  OCRBlocsArray       Blocs ;

  // Constructeur - Destructeur
  NSEpisodScreenView(NSEpisodDocument& doc, TWindow *parent = 0) ;
  ~NSEpisodScreenView() ;

  // Redéfiniton des fonctions virtuelles de TView et TRichEdit
  static LPCSTR   StaticName()        { return "NSEpisodScreenView" ; }
  const char      *GetViewName()      { return StaticName() ; }
  TWindow         *GetWindow();
  bool            CanClose();         // de TEditFile
  //bool          CanClear() ;        // de TEditFile
  bool            SetDocTitle(const char far* docname, int index) { return OWL::TWindowView::SetDocTitle(docname, index) ; }

  void Paint(TDC&, bool, NS_CLASSLIB::TRect& RectangleAPeindre) ;
  void EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
  void EvSetFocus(HWND hWndLostFocus) ;
  void OCRstart() ;
  void defineBloc() ;
  NSEpisodDocument* getDoc()          { return pDocEpisod ; }

 protected:
  // Remplissage de la vue
  void 	FillViewData() ;
  void 	SetupWindow() ;
  void  SetupTxtBar() ;
  void  InitFormatFont() ;
  void  SetupToolBar() ;

  // view event handlers
  bool 	VnIsWindow(HWND hWnd) ;

  // Fonctions de réponse aux evts
  void  EvSize(uint sizeType, ClassLib::TSize &size) ;
  void  EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  void  EvChar(uint key, uint repeatCount, uint flags) ;

  // Variables
  NSEpisodDocument    *pDocEpisod ;	  // Pointeur sur document EPISOD
  OWL::TControlBar    *pToolBar ;	    // Barre d'outils

  bool	              bSetupToolBar ;

 DECLARE_RESPONSE_TABLE(NSEpisodScreenView) ;
} ;

#endif // !__NSEPISODVIEW_H__

