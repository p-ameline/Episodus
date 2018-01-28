// -----------------------------------------------------------------------------
// nsHealthTeamMemberInterface.h
// -----------------------------------------------------------------------------
// Interface Visuelle de Gestion d'un Membre de l'Equipe de Santé
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 17:05:29 $
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à [rappeler les
// caractéristiques techniques de votre logiciel].
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
// This software is a computer program whose purpose is to [describe
// functionalities and technical features of your software].
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

#ifndef __GRAPHIC_HEALTH_TEAM__
# define __GRAPHIC_HEALTH_TEAM__

# include <iostream>
# include <string>
# include <owl/static.h>
# include <owl/groupbox.h>
# include <owl/tooltip.h>
# include <owl/button.h>
# include "nsbb/nsutidlg.h"
# include "nsbb/NSTip.h"
# include "partage/NTArray.h"
# include "nssavoir/nsHealthTeam.h"
# include "nssavoir/nsHealthTeamInterface.h"
# include "nssavoir/nsHealthTeamMemberInterface.h"

# define IDC_MEN_1				561
# define IDC_MANDATE_SUP	562
# define IDC_MEN_ADD			563
# define IDC_MEN_ALL			564
# define IDC_ADDTEAM			565
# define IDC_VIEWTEAM			566

/*
void		calculateParametersForPoint(NS_CLASSLIB::TPoint& pPoint,NS_CLASSLIB::TPoint& center, int* piRadius, int* iDegreAngle);
void		radiusTocar(int piRadius, int iDegreAngle,NS_CLASSLIB::TPoint& center, NS_CLASSLIB::TPoint& pPoint);
*/

// Dessine un texte sur un cercle
// HDC 							: Device graphique sur lequel ecrire
// char* texte  		: texte à afficher en cercle
// NS_CLASSLIB 			: TPoint& center point representant le centre du cercle
// int radius 			: A quel distance du texte commencer l'ecriture
// int start_angle  : Angle ou on doit commencer l'ecriture
// int val_angle		: valeur de l'angle

void    drawCircleTexte(HDC hdc, string texte, NS_CLASSLIB::TPoint& center, int radius, int start_angle, int val_angle, NS_CLASSLIB::TRect& RectAPeindre) ;

enum HTType
{
	HT_Petal, // support
  HT_Icon,  // icone
} ;

// -----------------------------------------------------------------------------
// Cartesian Element
// -----------------------------------------------------------------------------
// Represente un objet localisé par un une ordonnée et une abcisse
// -----------------------------------------------------------------------------
class Cartesian
{
 public:

  Cartesian() ;
  Cartesian(int x, int y) ;
  Cartesian(NS_CLASSLIB::TPoint* point) ;
  ~Cartesian() ;

  NS_CLASSLIB::TPoint* getCoordinate() ;

  void setCoordinate(NS_CLASSLIB::TPoint& point) ;
  void setCoordinate(int x, int y) ;

  int getX()		{ return (pPoint->x) ; }
  int getY()		{ return (pPoint->y) ; }

 protected:

 	NS_CLASSLIB::TPoint* pPoint ;
} ;

// -----------------------------------------------------------------------------
// Polar Element
// -----------------------------------------------------------------------------
// Represente un objet localisé par un angle et un radius
// -----------------------------------------------------------------------------
class Polar
{
 public:

 	Polar(NS_CLASSLIB::TPoint& center, int sangle, int radius) ;
  ~Polar() ;

  void setRadius(double ang) ;
  void setAngle(double ang) ;
  double getAngle() ;
  double getRadius() ;

 protected:

  double							_radius ;
  double  						_angle ;
  NS_CLASSLIB::TPoint _pCenter ;
} ;

// -----------------------------------------------------------------------------
// Pie Element
// -----------------------------------------------------------------------------
// Represente un objet camembert
// -----------------------------------------------------------------------------
class PolarPie : public Polar
{
 public:

 	PolarPie(NS_CLASSLIB::TPoint& center, int sangle, int angle, int radius)
  	: Polar(center,sangle, radius), _PieAngle(angle)
  {}

 protected:

 	double _PieAngle ;
} ;

// -----------------------------------------------------------------------------
// Health Element
// -----------------------------------------------------------------------------
// Abstraction d'un objet graphique servant dans l'epquipe de santé
// -----------------------------------------------------------------------------
class HealthTeamElement : public NSRoot
{
 public:

  // HealthTeamElement* parent : parent de l'objet
  HealthTeamElement(NSContexte* con) ;

  // Destructeur de l'objet
  ~HealthTeamElement() ;

  // Dessine l'objet à l'ecran
  virtual void Dessine(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) = 0 ;

  // ajoute un element qui sera contenu dans l'objet
  virtual void AjouteElement(HealthTeamElement* elem) {} ;

  // Efface un element de l'objet
  virtual bool RemoveElement(HealthTeamElement* elem) { return false ; } ;

  //	Charge un objet a partir d'un fichier de configuration
  // name : represente l'entre dans le fichier servant à charger le fichier
  virtual void 	InitColorFormSkin(string name) ;

  // recherche un element dans la liste
  virtual int Find(HealthTeamElement* temp) { return 0 ; }

  virtual HealthTeamElement* operator[](size_t i) { return NULL ; }

  virtual void AdjustElementOnScreen() = 0 ;

  // indique si le point est dans l'objet
  // NS_CLASSLIB::TPoint& point : Position que l'on veut tester
  //  HealthTeamElement* elemHits : Element testé
  virtual bool isIn(NS_CLASSLIB::TPoint& point, HealthTeamElement** elemHits, HTType type) = 0 ;

  // Cherche un element en fonction de l'angle et la distance
  virtual bool FindElement(int angle, string distance, HealthTeamElement** temp) = 0 ;

  // suprime els elements
  virtual void Reinit() = 0 ;

  // indique le plan d'affichage
  void 	 setPlan(int pl) { _plan = pl ; }

  // Indique si deux element sont du même type et sont egaux
  virtual bool IsEqual(HealthTeamElement* elem) = 0 ;

 protected:

  int											_plan ;  					// position dans le plan d'affichage
  ClassLib::TColor* 			_pBackColor ;     // Couleur de base l'objet

 public:

  // Retourne l'ordre d'affichage de l'objet
  inline
  int getPlan() { return _plan ; }
} ;

// -----------------------------------------------------------------------------
// HealthTeamVect Element
// -----------------------------------------------------------------------------
// Vecteur d'Element Graphique
// Vide les elements lors de la destruction de l'objet
// -----------------------------------------------------------------------------
typedef NTArray<HealthTeamElement>  HealthTeamVect ;

// -----------------------------------------------------------------------------
// HealthElements Element
// -----------------------------------------------------------------------------
// Represente une collection d'objet (graphique) a grouper
// -----------------------------------------------------------------------------
class HealthElements : public HealthTeamElement
{
 public:

  HealthElements(NSContexte* con)
  	: HealthTeamElement(con)
  {}

  //  Ajoute des lements dans la liste en les ordonnant
  //  en fonction de leur plan d'affichage
  virtual void AjouteElement(HealthTeamElement* elem) ;

  // Dessine tous les elements de la liste
  void Dessine(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

  virtual void Reinit() ;

  // Test si l'objet est sur un des objets
  virtual bool isIn(NS_CLASSLIB::TPoint& point, HealthTeamElement** elemHits, HTType typ) ;
  virtual bool FindElement(int angle, string distance, HealthTeamElement** temp) ;
  virtual bool HealthElements::IsEqual(HealthTeamElement* elem) ;
  virtual bool RemoveElement(HealthTeamElement* elem) ;
  virtual int Find(HealthTeamElement* temp) ;
  virtual HealthTeamElement* operator[] (size_t i) ;

  virtual void AdjustElementOnScreen() ;

 protected:

 	HealthTeamVect _elements ;
} ;

// -----------------------------------------------------------------------------
// Icon Element
// -----------------------------------------------------------------------------
// Represente un icon : un icon représentant un pair/mandat
// -----------------------------------------------------------------------------
class IconElement : public HealthTeamElement, public Cartesian
{
 public:

  IconElement(NSContexte* con, NSMemberMandatePair* pair, int x, int y) ;

  void Dessine(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

  bool isIn(NS_CLASSLIB::TPoint& point, HealthTeamElement** elemHits, HTType type) ;
  bool FindElement(int angle, string distance, HealthTeamElement** temp) { return false ; }

  NSMemberMandatePair* getPair()   { return mandat ; }
  NSHealthTeamMember*  getMember() { return mandat->getMember() ; }
  NSHealthTeamMandate* getMandat() { return mandat->getMandate() ; }
  string               getName()   { return mandat->getName() ; }

  void setParent(HealthTeamElement* par) { _parent = par ; }
  HealthTeamElement*   getParent() { return _parent ; }

  bool IsEqual(HealthTeamElement* elem) { return false ; }

  OWL::TIcon* getIco() { return _ico ; }

  void Reinit() {}

  void AdjustElementOnScreen() {}

  bool gras ;

 protected:

  NSMemberMandatePair* mandat ;
  HealthTeamElement*   _parent ;
  OWL::TIcon*          _ico ;
} ;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Partie d'un petal de la rosace
// Correspondant a une distance et un angle
// -----------------------------------------------------------------------------
class SubPetal : public HealthElements, public PolarPie
{
 public:

  // Constructeur pour les rosaces
  // NSContexte* con 	: le contexte global
  // string skin :  Chapitre pour recuperer les couleurs du sous petal
  // plan 						: ordre d'affichage
  SubPetal(NSContexte* con, string skin, int plan) ;

  SubPetal(NSContexte* con, NS_CLASSLIB::TPoint& Center, int startangle, int valangle, int radius, int plan, string skinname = "") ;
  ~SubPetal() ;

  // Test si l'objet est sur un des objets
  virtual bool isIn(NS_CLASSLIB::TPoint& point, HealthTeamElement** elemHits, HTType type) ;

  bool FindElement(int angle, string distance,HealthTeamElement** temp) ;
  bool SubPetal::IsEqual(HealthTeamElement* elem) ;
  void AdjustElementOnScreen() ;

 public:

  void Dessine(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;
  void Reinit() {}

 public:

  int 	getAngleView()  { return _angle_view ; }
  void  setAngleView(int angle) { _angle_view = angle ; }

  void setDistance(string dist) { _distance = dist ; }
  string getDistance()          { return _distance ; }

 protected:

  int  				 _angle_view ;   //	angle du petal
  string       _distance ;    // distance du petal
} ;

// -----------------------------------------------------------------------------
// Rosace Center
// -----------------------------------------------------------------------------
// Classe pour tracer le centre (le patient )
// -----------------------------------------------------------------------------
class RosaceCenter : public HealthElements, public Polar
{
 public:

  RosaceCenter() ;
  RosaceCenter(NSContexte* cons,NS_CLASSLIB::TPoint& Center, int radius, string skinname = "") ;

  void Dessine(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;
  bool isIn(NS_CLASSLIB::TPoint& point, HealthTeamElement** elemHits, HTType type) ;
  bool FindElement(int angle, string distance, HealthTeamElement** temp) ;

  virtual void AdjustElementOnScreen() ;

 public:

  int 	getAngleView()          { return _angle_view ; }
  void  setAngleView(int angle) { _angle_view = angle ; }

  void setDistance(string dist) { _distance = dist ; }
  string getDistance()          { return _distance ; }

  bool IsEqual(HealthTeamElement* elem) ;
//  	void Reinit() {}

 protected:

  int     _angle_view ;       //	angle du petal
  string  _distance ;    // distance du petal
} ;

// -----------------------------------------------------------------------------
// Petal Center
// -----------------------------------------------------------------------------
// representant un groupe de sous-petals
// -----------------------------------------------------------------------------
class Petal : public PolarPie, public HealthElements
{
 public:

 	Petal(NSContexte* con, NS_CLASSLIB::TPoint Center, double startAng, double angle, int size) ;
  ~Petal() ;

  bool isIn(NS_CLASSLIB::TPoint& point, HealthTeamElement** elemHits, HTType type) ;
  bool FindElement(int angle, string distance, HealthTeamElement** temp) ;
  void Dessine(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

 public:

  inline
  void setName(string name) { _name = name ; }

 protected:

  string    _name ;
  void Reinit() ;

 public:

  string    _label ;
} ;

// -----------------------------------------------------------------------------
// HealthDrag Center
// -----------------------------------------------------------------------------
// representant une classe servant un drag and drop
// -----------------------------------------------------------------------------
class HealthDrag
{
 public:

  NS_CLASSLIB::TPoint 			 _drag_point ;   // point ou l'icone est charge
  NS_CLASSLIB::TPoint 			 _drop_point ;   // point ou l'icone est laché
  IconElement* 							 _drag_elem ;    // Icone saisie
  HealthTeamElement*				 _drag_region ;  // icone déplacer

 public:

  bool	noMove() { return (_drag_point == _drop_point) ; }
  void Reinit() {}

 public:

  string _label ;
} ;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class _CLASSMODE RosaceManager : public NSRoot
{
 public:

 	RosaceManager(NSContexte *pCtx, NSMoralPerson *pMPTeam) ;
  RosaceManager(NSContexte *pCtx, NSHealthTeam *pHTeam) ;
  ~RosaceManager() ;

  NSPatPathoArray *						getPatho()				{ return (_tManager->getPatho()) ; }
  NSHealthTeamMemberArray *		getMembers()			{ return (_tManager->getMembers()) ; }
  NSMoralPerson *							getMPTeam()				{ return (_tManager->getMPTeam()) ; }
  NSHealthTeam *							getHTeam()				{ return (_tManager->getHTeam()) ; }

  NSTeamManager*							getManager()      { return _tManager ; }

  bool												isMPTeamManager()		{ if (_tManager->getMPTeam() != NULL) return true ; return false ; }
  bool												isHTeamManager()	{ if (_tManager->getHTeam() != NULL) return true ; return false ; }

  void												addMember(NSHealthTeamMember *pMember, NSHealthTeamMember::memberType mType)			{ _tManager->addMember(pMember, mType) ; }

  void												addMandate(NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)											{ _tManager->addMandate(pMe, pMa) ; }
  void												modMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)		{ _tManager->modMandate(pCtx, pMe, pMa) ; }
  void												closeMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)	{ _tManager->closeMandate(pCtx, pMe, pMa) ; }

 	void                        reset(NSMoralPerson *pMPTeam) ;
	void                        reset(NSHealthTeam *pHTeam) ;

 protected:

// 	NSTeam *										_team ;
  NSTeamManager *							_tManager ;
} ;

// -----------------------------------------------------------------------------
// RosaceDrawer  Center
// -----------------------------------------------------------------------------
// representant une classe servant pour le drag and drop
// -----------------------------------------------------------------------------
class _CLASSMODE RosaceDrawer : public OWL::TButton, public NSRoot
{
 public:

	RosaceDrawer(NSContexte *pCtx, TWindow* parent, int resId, RosaceManager *rManager, bool bVerbose = false) ;
  RosaceDrawer(NSContexte *pCtx, TWindow *parent, int Id, const char far* text, int X, int Y, int W, int H, RosaceManager *rManager, bool bVerbose = true) ;

  ~RosaceDrawer() ;

 	void  ObjectInit() ;
  void	SetupWindow() ;

  static int 					rosace_size ;
  static int 					first_petal_size ;
  static int 					second_petal_size ;
//  NSHealthTeamManager*        getManager() { return _pHTManager ; }
	RosaceManager *			getManager() { return _tManager ; }

 protected:

	void 		InitRosace() ;
  void 		EvPaint() ;
  void 		Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;
  void 		EvLButtonDblClk(UINT modKeys, NS_CLASSLIB::TPoint& point) ; // Double click bouton gauche souris
  void 		EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point) ;    	// déplacement de la souris
	void 		EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ; 	// bouton gauche de souris enfoncé
  void 		EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ; 	// bouton droit de souris enfoncé
  void 		EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point) ;    	// bouton gauche de souris levé
  void 		EvKeyDown(uint key, uint repeatCount, uint flags) ; 					// Touche enfoncé
  void 		EvTimer(uint timerId) ;
  void		InitialiseMandat() ;
  void    CmViewList() ; // Voir la liste des mandats d'un membre
  void		CmAddMember() ;
  void		CmSupMember() ;
  void		CmSupMandat() ;
  void		CmAddTeam() ;
  void		CmViewTeam() ;
  void    CmViewAll() ;

 protected:

	int                       _height ;      	// hauteur
  int                       _width ;       	// largeur
	HealthElements*           _Draws ; 				// Elements of the box
  NS_CLASSLIB::TPoint       _ptCenter ;  		// Centre de la fenetre
  int                       _max_radius ;		// taille maximum des radius a tracer
  OWL::TImageList*          _ImgDragDrop ; 	// utiliser pour le drag and drop
  bool                      _bLButtonDown ;	// indique si le bouton gauche de la souris est enfoncé
  NSTitleTip*               _pToolTip ;     // toolTip
  NSMemberMandatePairArray* _mandats ; 			// list member mandat

//  NSHealthTeamManager*        _pHTManager ;   // manager
	RosaceManager *           _tManager ;

  HealthDrag                _dragInformation ;
  IconElement*              _popBuffer ;    // Voir en details le membre
  NS_CLASSLIB::TPoint       _PosMenu ;

  NS_CLASSLIB::TPoint       _LastMouseMovePos ;

  bool                      _bVerboseMode ;

 DECLARE_RESPONSE_TABLE(RosaceDrawer) ;
} ;

int  RosaceDrawer::rosace_size 				=	0 ;
int  RosaceDrawer::first_petal_size 	= 0 ;
int  RosaceDrawer::second_petal_size 	= 0 ;

class _CLASSMODE GraphicHealthTeam : public NSUtilDialog // public TMDIChild, public NSRoot
{
 public:
  GraphicHealthTeam(/*TMDIClient& parent */ TWindow *pParentWindow, NSContexte *pCtx, NSMoralPerson *pMPTeam) ;
  GraphicHealthTeam(TWindow *parent, NSContexte *pCtx, NSHealthTeam *pHTeam) ;
  ~GraphicHealthTeam() ;
  void  SetupWindow() ;
  void	 CmOk() ;
  void	 CmCancel() ;
  void	 CmHelp() ;

 protected:

 	OWL::TButton*  rosace ;
  OWL::TButton*  pOkButton ;
  OWL::TButton*  pCancelButton ;
  OWL::TButton*  pHelpButton ;
  RosaceManager* manager ;

 DECLARE_RESPONSE_TABLE(GraphicHealthTeam) ;
} ;

#endif // __GRAPHIC_HEALTH_TEAM__

