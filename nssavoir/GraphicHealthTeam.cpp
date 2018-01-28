// -----------------------------------------------------------------------------
// GraphicHealthTeam.cpp
// -----------------------------------------------------------------------------
// Interface Graphique de Gestion d'un Membre de l'Equipe de Santé
// -----------------------------------------------------------------------------
// $Revision: 1.12 $
// $Author: pameline $
// $Date: 2013/12/15 15:43:25 $
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

#include "nssavoir\GraphicHealthTeam.h"
#include "nssavoir\RosaceDrawUtil.h"
#include "nssavoir\healthteam.rh"
#include "nssavoir\nssavoir.h"
#include "nssavoir\nshealthteam.h"
#include "nsbb\ns_skins.h"
#include "nsbb\nslistwind.h"
#include "nsbb\NSHTTeamDialog.h"
#include "nsdn\nsdocnoy.h"
#include "nsepisod\nsToDo.h"

#include <owl\gdiobjec.h>
#include <windows.h>

#include "nsdn\nsdochis.h"
#include "nautilus\nshistdo.h"

extern "C"
{
#include <math.h>
#include <stdio.h>
}


// -----------------------------------------------------------------------------
// Cartesian Element
// -----------------------------------------------------------------------------
//
// Represente un objet localisé par un une ordonnée et une abcisse
//
// -----------------------------------------------------------------------------

Cartesian::Cartesian()
{
	pPoint = new  NS_CLASSLIB::TPoint() ;
}


Cartesian::Cartesian(int x, int y)
{
	pPoint = new NS_CLASSLIB::TPoint(x, y) ;
}


Cartesian::Cartesian(NS_CLASSLIB::TPoint* point)
{
	pPoint = new NS_CLASSLIB::TPoint(*point) ;
}


Cartesian::~Cartesian()
{
	if (pPoint != NULL)
  {
  	delete (pPoint) ;
    pPoint = NULL ;
  }
}

NS_CLASSLIB::TPoint *
Cartesian::getCoordinate()
{
	return  pPoint ;
}


void
Cartesian::setCoordinate(NS_CLASSLIB::TPoint& point)
{
	pPoint->x = point.x ;
  pPoint->y = point.y ;
}


void
Cartesian::setCoordinate(int x, int y)
{
	pPoint->x = x ;
  pPoint->y = y ;
}


// -----------------------------------------------------------------------------
// Polar Element
// -----------------------------------------------------------------------------
//
// Represente un objet localisé par un angle et un radius
//
// -----------------------------------------------------------------------------

Polar::Polar(NS_CLASSLIB::TPoint& center, int sangle, int radius)
{
	_angle  = sangle ;
  _radius = radius ;
  _pCenter = NS_CLASSLIB::TPoint(center) ;
}


Polar::~Polar()
{
}


void
Polar::setRadius(double ang)
{
	_radius = ang ;
}


void
Polar::setAngle(double ang)
{
	_angle = ang ;
}


double
Polar::getAngle()
{
	return _angle ;
}


double
Polar::getRadius()
{
	return _radius ;
}



// -----------------------------------------------------------------------------
// Health Element
// -----------------------------------------------------------------------------
//
// Represente un objet graphique de l'health team
//
// -----------------------------------------------------------------------------

HealthTeamElement::HealthTeamElement(NSContexte* con) : NSRoot(con)
{
	_pBackColor= NULL ;
}


HealthTeamElement::~HealthTeamElement()
{
}


// -----------------------------------------------------------------------------
// Importe la couleur a utiliser pour tracer l'element
// name = : nom du chapitre (present dans skin.ini);
// -----------------------------------------------------------------------------
void
HealthTeamElement::InitColorFormSkin(string name)
{
	nsSkin* pSkin = pContexte->getSkins()->donneSkin(name) ;
  if (pSkin)
  {
  	_pBackColor        = pSkin->getBackColor() ;
  }
}



// -----------------------------------------------------------------------------
// Health Elements
// -----------------------------------------------------------------------------
//
// Liste d'elements représentant l'health team
//
// -----------------------------------------------------------------------------

void
HealthElements::AjouteElement(HealthTeamElement* elem)
{
  if (_elements.empty())
  	_elements.push_back(elem) ;
  else
  {
    bool insert = false ;
    int pelem = elem->getPlan() ; // recupere le plan d'affichage pour odonner la liste
    HealthTeamVect::iterator iter ;
    for (iter = _elements.begin() ; iter != _elements.end() ; iter++)
      if  (((*iter) != NULL) && ( (*iter)->getPlan() >= pelem) )          // insert avant
      {
        insert = true ;
        _elements.insert(iter,elem) ;
        break ;
      }
    if (insert == false)
    	_elements.push_back(elem) ;

  }

}


bool
HealthElements::isIn(NS_CLASSLIB::TPoint& point, HealthTeamElement** elemHits, HTType type)
{
  if (_elements.empty())
  	return false ;
  HealthTeamVect::reverse_iterator iter ;
  for (iter = _elements.rbegin() ; iter != _elements.rend() ; iter++)
		if  ((*iter) != NULL)
    	if ( (*iter)->isIn(point, elemHits, type) == true)
      	return true ;
  return false ;
}


void
HealthElements::AdjustElementOnScreen()
{
	if (_elements.empty())
  	return ;
  HealthTeamVect::reverse_iterator iter ;
  for (iter = _elements.rbegin() ; iter != _elements.rend() ; iter++)
  	if  ((*iter) != NULL)
    	(*iter)->AdjustElementOnScreen() ;
}


bool
HealthElements::FindElement(int angle, string distance, HealthTeamElement** temp)
{
	if (_elements.empty())
  	return false ;
  HealthTeamVect::reverse_iterator iter ;
  for (iter = _elements.rbegin() ; iter != _elements.rend() ; iter++)
    if  ((*iter) != NULL)
    {
      if ((*iter)->FindElement(angle, distance, temp) == true)
        return true ;
    }
	return false ;
}

int
HealthElements::Find(HealthTeamElement* temp)
{
	if (_elements.empty())
  	return -1 ;
  int i = 0 ;
  HealthTeamVect::iterator iter ;
  for (iter = _elements.begin() ; iter != _elements.end() ; iter++)
  if  ((*iter) != NULL)
  {
    if ((*iter) == temp)
    	return i ;
    i++ ;
  }
  return -1 ;
}

HealthTeamElement *
HealthElements::operator[](size_t i)
{
	if (i < _elements.size())
  	return _elements[i] ;
  return NULL ;
}

void
HealthElements::Dessine(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
	try
  {
    if (_elements.empty())
    	return ;

    HealthTeamVect::iterator iter ;
    for (iter = _elements.begin() ; iter != _elements.end() ; iter++)
      if  ((*iter) != NULL)
        (*iter)->Dessine(pDc, erase, RectAPeindre) ;
  }
  catch (...)
  {
  }
}


bool
HealthElements::IsEqual(HealthTeamElement* /*elem*/)
{
	return false ;
}

bool
HealthElements::RemoveElement(HealthTeamElement* elem)
{
  if (_elements.empty())
  	return false ;

  HealthTeamVect::iterator iter ;
  for (iter = _elements.begin() ; iter != _elements.end() ; iter++)
    if  (((*iter) != NULL) && ( (*iter) == elem))
    {
      _elements.erase(iter) ;
      return true ;
    }

	return false ;
}

void
HealthElements::Reinit()
{
  _elements.clear() ;
}

// -----------------------------------------------------------------------------
// Petal method
// -----------------------------------------------------------------------------

Petal::Petal(NSContexte* con, NS_CLASSLIB::TPoint	 Center, double startAng, double angle, int radius)
	: HealthElements(con) ,
  	PolarPie(Center, startAng, angle,  radius)
{
	_plan = 1 ;
}


Petal::~Petal()
{
}


bool
Petal::isIn(NS_CLASSLIB::TPoint& point, HealthTeamElement** elemHits, HTType type)
{
	int angle = 0 ;
  int radius = 0 ;
  double size = RosaceDrawer::rosace_size ;
	calculateParametersForPoint(point,_pCenter,&radius, &angle) ;
  double angle_end = _angle + _PieAngle ;

  if ((radius <= _radius) && (radius >= size) && (angle >= _angle ) && (angle <= angle_end))
  {
  	return HealthElements::isIn(point, elemHits, type) ;
  }
  return false ;
}


bool
Petal::FindElement(int angle, string distance, HealthTeamElement** temp)
{
  if (_elements.empty())
  	return false ;

  HealthTeamVect::reverse_iterator iter ;
  for (iter = _elements.rbegin() ; iter != _elements.rend() ; iter++)
  	if  ((*iter) != NULL)
    {
    	if ((*iter)->FindElement(angle, distance, temp) == true)
      	return true ;
    }
  return false ;
}


void
Petal::Dessine(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  try
  {
    drawCircleTexte(pDc->GetHDC(), _label, _pCenter, _radius + 20,  _angle, _PieAngle, RectAPeindre) ;
    HealthElements::Dessine(pDc, erase, RectAPeindre) ;
  }
  catch (...)
  {
  }
}


void
Petal::Reinit()
{
  if (_elements.empty())
  	return ;

  HealthTeamVect::reverse_iterator iter ;
  for (iter = _elements.rbegin() ; iter != _elements.rend() ; iter++)
    if  ((*iter) != NULL)
    {
    	(*iter)->Reinit() ;
    }
  return ;
}



// -----------------------------------------------------------------------------
// Rosace method
// -----------------------------------------------------------------------------

RosaceCenter::RosaceCenter(NSContexte* cons, NS_CLASSLIB::TPoint&	Center, int radius, string skin)
	:	HealthElements(cons),
  	Polar(Center, 0, radius)
{
	_pBackColor = NULL ;
	if (skin != "")
		InitColorFormSkin(skin) ;
	_plan = 4 ;
}


void
RosaceCenter::Dessine(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
	try
  {
		if (!pDc)
    	return ;

		TBrush* temp ;
		if (_pBackColor != NULL)
			temp =  new TBrush(*_pBackColor) ;
    else
    	temp =  new TBrush(NS_CLASSLIB::TColor::LtRed) ;
    NS_CLASSLIB::TRect patientRect(_pCenter.X() - _radius, _pCenter.Y() + _radius, _pCenter.X() + _radius, _pCenter.Y() - _radius) ;
    ::SelectObject(pDc->GetHDC(), *temp) ;

    pDc->Ellipse(patientRect) ;
    HealthElements::Dessine(pDc, erase, RectAPeindre) ;
    delete temp ;
  }
  catch (...)
  {
  }
}


// -----------------------------------------------------------------------------
// indique si le point est dans l'objet
// -----------------------------------------------------------------------------
bool
RosaceCenter::isIn(NS_CLASSLIB::TPoint& point, HealthTeamElement** elemHits, HTType type)
{
	int angle = 0 ;
  int radius = 0 ;
  calculateParametersForPoint(point, _pCenter, &radius, &angle) ;
  if (type == HT_Petal)
  {
  	if (radius <= _radius)
    {
      *elemHits = this ;
      return true ;
    }
  }
  else
  {
  	if (radius <= _radius)
  		return 	HealthElements::isIn(point, elemHits, type) ;
    return false ;
  }
  return false ;
}


void
RosaceCenter::AdjustElementOnScreen()
{
	NS_CLASSLIB::TPoint point(10, 10) ;
	radiusTocar((int)(_radius /3), 0, _pCenter, point) ;
	if (_elements.empty())
		return ;
	HealthTeamVect::iterator iter ;
	for (iter = _elements.begin() ; iter != _elements.end() ; iter ++)
	{
  	((IconElement*)(*iter))->setCoordinate(point) ;
  }
}


bool
RosaceCenter::FindElement(int /*angle*/, string distance, HealthTeamElement** temp)
{
	// Question ouverte : Un paitent doit t'il avoir un angle ?
	if (/*(_angle_view == angle) &&*/ (distance == _distance))       //	angle du petal
  {
    *temp = this ;
    return true ;
  }
  return false ;
}


bool
RosaceCenter::IsEqual(HealthTeamElement* elem)
{
  RosaceCenter* temp = dynamic_cast<RosaceCenter*>(elem) ;
  if (temp != NULL)
  {
    if ((_angle_view == temp->getAngleView()) && (temp->getDistance() == _distance))
      return true ;
    return false ;
  }
  return false ;
}



// -----------------------------------------------------------------------------
// SuPetal Methods
// -----------------------------------------------------------------------------

SubPetal::SubPetal(NSContexte* cons, NS_CLASSLIB::TPoint& Center, int startangle, int valangle, int radius, int plan, string skinname)
	: HealthElements(cons),
  	PolarPie(Center, startangle, valangle, radius)
{
  _plan = plan ;
  _pBackColor = NULL ;
	if (skinname != "")
		InitColorFormSkin(skinname) ;
}


SubPetal::~SubPetal()
{
}


void
SubPetal::Dessine(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
try
{
  if (!pDc)
    return ;

  TBrush* temp ;
  if (_pBackColor != NULL)
    temp = new TBrush(*_pBackColor) ;
  else
    temp = new TBrush(NS_CLASSLIB::TColor::LtRed) ;
  NS_CLASSLIB::TRect boudingRect(_pCenter.X() - _radius, _pCenter.Y() + _radius, _pCenter.X() + _radius, _pCenter.Y() - _radius) ;

  double dRadStartAngle = double(_angle) * 3.1415926 / double(180) ;
  double dStartX = (double)_radius * cos(dRadStartAngle) ;
  double dStartY = (double)_radius * sin(dRadStartAngle) ;

  double dRadEndAngle = double(_angle + _PieAngle) * 3.1415926 / double(180) ;
  double dEndX = (double)_radius * cos(dRadEndAngle) ;
  double dEndY = (double)_radius * sin(dRadEndAngle) ;

  NS_CLASSLIB::TPoint pointStart(_pCenter.x + int(dStartX), _pCenter.y - int(dStartY)) ;
  NS_CLASSLIB::TPoint pointEnd(_pCenter.x + int(dEndX), _pCenter.y - int(dEndY)) ;
  ::SelectObject(pDc->GetHDC(), *temp) ;
  pDc->Pie(boudingRect, pointStart, pointEnd) ;


  if (_elements.empty())
    return ;
  HealthTeamVect::iterator iter ;
  for (iter = _elements.begin() ; iter != _elements.end() ; iter++)
    if  ((*iter) != NULL)
      (*iter)->Dessine(pDc, erase, RectAPeindre) ;
  delete (temp) ;
}
catch (...)
{
}
}


bool
SubPetal::isIn(NS_CLASSLIB::TPoint& point, HealthTeamElement** elemHits, HTType type)
{
	int			angle		= 0 ;
  int			radius	= 0 ;
  double	size		= RosaceDrawer::rosace_size ;
  double	first		= RosaceDrawer::first_petal_size ;
  double	second	= RosaceDrawer::second_petal_size ;

	if (type == HT_Petal)
  {
		calculateParametersForPoint(point, _pCenter, &radius, &angle) ;
  	double angle_end = _PieAngle + _angle ;

  	double in 	= (_radius == first) ? size  : first ;
  	double out 	= (_radius == first) ? first : second ;

  	if ((radius >=  in) && (radius <= out) && (angle >= _angle ) && (angle <= angle_end))
    {
      string name = (in == size) ? "proche" : "eloigné" ;
      //erreur(name.c_str(),0,0) ;
      *elemHits = this ;
      return true ;
    }
  }
  else
  {
  	calculateParametersForPoint(point, _pCenter, &radius, &angle) ;
    double angle_end = _PieAngle + _angle ;
    double in = (_radius == first) ? size : first ;
  	double out = (_radius == first) ? first : second ;
  	if ((radius >=  in) && (radius <= out) && (angle >= _angle ) && (angle <= angle_end))
    {
      return HealthElements::isIn(point, elemHits, type) ;
    }
  }
  return false ;
}


bool
SubPetal::FindElement(int angle, string distance, HealthTeamElement** temp)
{
	if ((_angle_view == angle) && (distance == _distance))
  {
    *temp = this ;
    return true ;
  }
  return false ;
}


bool
SubPetal::IsEqual(HealthTeamElement* elem)
{
  SubPetal* temp = dynamic_cast<SubPetal *>(elem) ;
  if (temp != NULL)
  {
    if ((_angle_view == temp->getAngleView()) && (temp->getDistance() == _distance))
      return true ;
    return false ;
  }
  return false ;
}

void
SubPetal::AdjustElementOnScreen()
{
  double size   = RosaceDrawer::rosace_size ;
  double first  = RosaceDrawer::first_petal_size ;
  double second = RosaceDrawer::second_petal_size ;
  double in     = (_radius == first) ? size : first ;
  double out    = (_radius == first) ? first : second ;
  int test_angle  = (int)((_angle + _angle + _PieAngle) / 2) ;
  int test_radius = (int)((in + out) /2) ;

  NS_CLASSLIB::TPoint point(0, 0) ;
  radiusTocar(test_radius, test_angle, _pCenter, point) ;

  int radius = 0 ;
  int angle  = 0 ;
  calculateParametersForPoint(point, _pCenter, &radius, &angle) ;

  radiusTocar(test_radius, test_angle, _pCenter, point) ;
  if (_elements.empty())
    return ;
  HealthTeamVect::iterator iter ;
  for (iter = _elements.begin() ; iter != _elements.end() ; iter++)
  {
    ((IconElement*)(*iter))->setCoordinate(point) ;
  }
}

// -----------------------------------------------------------------------------
// Icons Element
// -----------------------------------------------------------------------------

IconElement::IconElement(NSContexte* con, NSMemberMandatePair* pair, int x, int y)
            :HealthTeamElement(con), Cartesian(x, y)
{
	_plan = 5 ;
  mandat = pair ;
  gras = false ;
  _ico = NULL ;
  nsSkin* pSkin = pContexte->getSkins()->donneSkin("HealthTeamIco") ;
  if (pSkin)
  {
  	_ico        = pSkin->getIcon() ;
  }
  else
  {
  	_ico = new OWL::TIcon(0, IDI_EXCLAMATION) ;
  }
}


void
IconElement::Dessine(TDC* pDc, bool /*erase*/, NS_CLASSLIB::TRect& /*RectAPeindre*/)
{
try
{
  if (!pDc)
    return ;

  if (gras)
  {
    NS_CLASSLIB::TPoint center(getX() + 8, getY() + 8) ;
    TBrush* temp = new TBrush(NS_CLASSLIB::TColor::LtRed) ;
    NS_CLASSLIB::TRect patientRect(center.X() - 15, center.Y() + 15, center.X() + 15, center.Y() - 15) ;
    ::SelectObject(pDc->GetHDC(), *temp) ;

    pDc->Ellipse(patientRect) ;
    delete temp ;
  }

  if (_ico != NULL)
    pDc->DrawIcon(getX(), getY(), *_ico, 16, 16) ;
}
catch(...)
{
}
}


bool
IconElement::isIn(NS_CLASSLIB::TPoint& point, HealthTeamElement** elemHits, HTType type)
{
	if (type == HT_Icon)
  {
		if ((point.x >= this->getX()) && (point.x <= getX() + 16) && (point.y >= getY()) && (point.y <= getY() + 16))
  	{
    	*elemHits = this ;
    	return true ;
  	}
  }
  return false ;
}



// -----------------------------------------------------------------------------
// Rosace Drawer
// -----------------------------------------------------------------------------
// Bouton sur lequel on trace l'Equipe de Santé
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(RosaceDrawer, TButton)
	EV_WM_LBUTTONDBLCLK,
  EV_WM_PAINT,
  EV_WM_LBUTTONDOWN,                                  // bouton gauche enfoncé
  EV_WM_RBUTTONDOWN,                                  // bouton droit enfoncé
	EV_WM_LBUTTONUP,                                    // bouton gauche relaché
  EV_WM_MOUSEMOVE,                                    // mouvement de la souris
  EV_COMMAND(IDC_MEN_1,      	CmViewList),
  EV_COMMAND(IDC_MEN_ADD,    	CmAddMember),
  EV_COMMAND(IDC_MANDATE_SUP, CmSupMandat),
  EV_COMMAND(IDC_MEN_ALL, 		CmViewAll),
  EV_COMMAND(IDC_ADDTEAM,			CmAddTeam),
  EV_COMMAND(IDC_VIEWTEAM,		CmViewTeam),
//  EV_WM_KEYUP,
  EV_WM_KEYDOWN,
END_RESPONSE_TABLE ;


RosaceDrawer::RosaceDrawer(NSContexte *pCtx, TWindow *parent, int resId, RosaceManager *pManager, bool bVerbose)
             :TButton(parent, resId), NSRoot(pCtx)
{
  _tManager	    = pManager ;
	_bVerboseMode = bVerbose ;
  
	ObjectInit() ;
}

RosaceDrawer::RosaceDrawer(NSContexte *pCtx, TWindow *parent, int Id, const char far* text, int X, int Y, int W, int H, RosaceManager *pManager, bool bVerbose)
             :TButton(parent, Id, text, X, Y, W, H), NSRoot(pCtx)
{
  _tManager	    = pManager ;
  _bVerboseMode = bVerbose ;

	ObjectInit() ;
}

void
RosaceDrawer::ObjectInit()
{
	_Draws				= new HealthElements(pContexte) ;
  _ImgDragDrop	= new OWL::TImageList(NS_CLASSLIB::TSize(16, 16), ILC_COLOR16, 0, 0) ;
	_ImgDragDrop->SetBkColor(NS_CLASSLIB::TColor::White) ;
  _bLButtonDown = 0 ;
  _pToolTip			= new NSTitleTip(this, pContexte) ;
  _pToolTip->Create() ;
	_pToolTip->Hide() ;
/*
  _pHTManager		= new NSHealthTeamManager(pContexte->getPatient()->pHealthTeam) ; // Initialisation du manager
  _mandats			= new NSMemberMandatePairArray(_pHTManager->getMembers(), pContexte) ;
*/
  _mandats			= new NSMemberMandatePairArray(_tManager->getMembers(), pContexte) ;

  _popBuffer		= NULL ;
  _bLButtonDown = false ;
  _LastMouseMovePos = NS_CLASSLIB::TPoint(0, 0) ;
}

void
RosaceDrawer::EvKeyDown(uint key, uint /* repeatCount */, uint /* flags */)
{
  switch(key)
  {
    case VK_TAB			:	// NextSection() ;
      								// this->SetFocus() ;
        							break ;
    case VK_RETURN	:
    case VK_DOWN  	:	break ;
    case VK_DELETE	:	break ;
    case VK_INSERT	:	break ;
    default       	:	break ;
  }
}


void
RosaceDrawer::InitialiseMandat()
{
	if (_mandats->empty())
  	return ;
  NSMemberMandatePairIter iter ;
  int i = 0 ;
	for (iter = _mandats->begin() ; iter != _mandats->end() ; iter++)
  {
    int angle = (*iter)->getMandate()->getAngle()	;
    string dist = (*iter)->getMandate()->getSDistance() ;
    HealthTeamElement *temp = NULL ;
    _Draws->FindElement(angle, dist, &temp) ;
    if (temp != NULL)
    	temp->AjouteElement(new IconElement(pContexte, (*iter), 10 + (i * 25), 30)) ;
   	i++ ;
  }
}

void
RosaceDrawer::CmViewList()
{
  if (NULL == _popBuffer)
    return ;

  HealthTeamMemberInterface	*pNewMemberInterface = new HealthTeamMemberInterface(this, pContexte, _popBuffer->getMember()) ;
  pNewMemberInterface->Execute() ;
  delete pNewMemberInterface ;
}

void
RosaceDrawer::CmViewAll()
{
	HealthTeamInterface health(this, pContexte) ;
 	health.Execute() ;
}

void
RosaceDrawer::InitRosace()
{
	// recupere differentes couleur d'un objet
	int size = (_max_radius * 20 ) / 100 ;

  _Draws->Reinit() ;

  RosaceDrawer::rosace_size = size ;
 	RosaceCenter* quaida = new RosaceCenter(pContexte, _ptCenter, size, "HealthTeamCenter") ;
 	quaida->setDistance("0") ;
  quaida->setAngleView(0) ;
  _Draws->AjouteElement(quaida) ;

  int size2 = (_max_radius * 64 ) / 100 ; // Taille des plus petit petal
  int size3 = (_max_radius * 92 ) / 100 ; // Taille des plus grand petal

  RosaceDrawer::first_petal_size  = size2 ;
  RosaceDrawer::second_petal_size = size3 ;

  // medecin
	Petal* medecin = new Petal(pContexte, _ptCenter, 90, 88, size3) ;
  medecin->_label = pContexte->getSuperviseur()->getText("HealthTeamManagement", "medicalDoctor") ;
  medecin->setName("medecin") ;
  SubPetal* far_med = new SubPetal(pContexte, _ptCenter, 90, 88, size3, 1, "HealthTeamDoctorFar") ;
  far_med->setDistance("2") ;
  far_med->setAngleView(3) ;
  medecin->AjouteElement(far_med) ;
  SubPetal* near_med = new SubPetal(pContexte, _ptCenter, 90, 88, size2, 2, "HealthTeamDoctorNear") ;
  near_med->setDistance("1") ;
  near_med->setAngleView(3) ;
 	medecin->AjouteElement(near_med) ;
  _Draws->AjouteElement(medecin) ;

 	// personnel soigant
  Petal* ps = new Petal(pContexte, _ptCenter, 0, 88, size3) ;
  ps->setName("personnel soigant") ;
  ps->_label = pContexte->getSuperviseur()->getText("HealthTeamManagement", "healthProfessionals") ;
  SubPetal* ps_far = new SubPetal(pContexte, _ptCenter, 0, 88, size3, 1 , "HealthTeamSoignantFar") ;
  ps_far->setDistance("2") ;
  ps_far->setAngleView(9) ;
  ps->AjouteElement(ps_far) ;
  SubPetal* ps_near = new SubPetal(pContexte, _ptCenter, 0, 88, size2, 2, "HealthTeamSoignantNear") ;
  ps_near->setDistance("1") ;
  ps_near->setAngleView(9) ;
  ps->AjouteElement(ps_near) ;
  _Draws->AjouteElement(ps) ;

  // social
  Petal* social = new Petal(pContexte, _ptCenter, 315, 43, size3) ;
  social->_label = pContexte->getSuperviseur()->getText("HealthTeamManagement", "socialWorkers") ;
  social->setName("social") ;
  SubPetal* social_far = new SubPetal(pContexte, _ptCenter, 315, 43, size3, 1, "HealthTeamSocialFar") ;
  social_far->setDistance("2") ;
  social_far->setAngleView(15) ;
  social->AjouteElement(social_far) ;
  SubPetal* social_near = new SubPetal(pContexte, _ptCenter, 315, 43, size2, 2, "HealthTeamSocialNear") ;
  social_near->setDistance("1") ;
  social_near->setAngleView(15) ;
  social->AjouteElement(social_near) ;
  _Draws->AjouteElement(social) ;

  // personne administrateur
  Petal* adm = new Petal(pContexte, _ptCenter, 270, 43, size3) ;
  adm->_label = pContexte->getSuperviseur()->getText("HealthTeamManagement", "administration") ;
  adm->setName("administration") ;
  SubPetal* adm_far = new SubPetal(pContexte, _ptCenter, 270, 43, size3, 1, "HealthTeamAdmFar") ;
  adm_far->setDistance("2") ;
  adm_far->setAngleView(17) ;
  adm->AjouteElement(adm_far) ;
  SubPetal* adm_near = new SubPetal(pContexte, _ptCenter, 270, 43, size2, 2, "HealthTeamAdmNear") ;
  adm_near->setDistance("1") ;
  adm_near->setAngleView(17) ;
  adm->AjouteElement(adm_near) ;
  _Draws->AjouteElement(adm) ;

  // famille
  Petal* fam = new Petal(pContexte, _ptCenter, 180, 88, size3) ;
  fam->_label = pContexte->getSuperviseur()->getText("HealthTeamManagement", "familly") ;
  fam->setName("famille") ;
  SubPetal* fam_far = new SubPetal(pContexte, _ptCenter, 180, 88, size3, 1, "HealthTeamFamilyFar") ;
  fam_far->setDistance("2") ;
  fam_far->setAngleView(21) ;
  fam->AjouteElement(fam_far) ;
  SubPetal* fam_near = new SubPetal(pContexte, _ptCenter, 180, 88, size2, 2, "HealthTeamFamilyNear") ;
  fam_near->setDistance("1") ;
  fam_near->setAngleView(21) ;
  fam->AjouteElement(fam_near) ;
  _Draws->AjouteElement(fam) ;
}


void
RosaceDrawer::EvPaint()
{
  TPaintDC Dc(*this) ;
  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&Dc.Ps.rcPaint ;
  Paint(Dc, true, rect) ;
//  DefaultProcessing() ;
}


void
RosaceDrawer::EvMouseMove(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
  if (_LastMouseMovePos == point)
    return ;

  _LastMouseMovePos = point ;

	if (_bLButtonDown)
  {
		_ImgDragDrop->DragMove(point.x, point.y) ;
    _pToolTip->Hide() ;
  }
  else
  {
    HealthTeamElement* elemHits = NULL ;
    _Draws->isIn(point, &elemHits, HT_Icon) ;
    if   (elemHits != NULL)
    {
      IconElement* pIElem = dynamic_cast<IconElement *>(elemHits) ;
      if ((pIElem != NULL))
      {
        //temp->gras = true;
        //_SelectedIco = temp;
        string	name2Display	= "[personne inconnue]" ;
        string  elemName			= pIElem->getName() ;
        if (elemName != "")
        	name2Display = elemName ;
//        string toaff = pIElem->getName() ;
        NS_CLASSLIB::TPoint haut(point.x + 16, point.y + 16) ;
        NS_CLASSLIB::TPoint bas(point.x + 150, point.y + 150) ;
        NS_CLASSLIB::TRect rectDoc(haut, bas) ;
        NS_CLASSLIB::TPoint hautHover(point.x - 5, point.y - 5) ;
        NS_CLASSLIB::TPoint basHover(point.x + 5, point.y + 5) ;
        NS_CLASSLIB::TRect rectHover(hautHover, basHover) ;
        _pToolTip->Show(rectDoc, &name2Display, 0, -1, &rectHover) ;
	      //  Invalidate() ;
      }
    // _pToolTip->Hide() ;
    }
    else
    {
      /*if (_SelectedIco != NULL) // deselectionné l'icone
      {
        _SelectedIco->gras = false ;
        _SelectedIco = NULL ;
        Invalidate() ;
      }           */
     	_pToolTip->Hide() ;
    }
  }
}


void
RosaceDrawer::EvRButtonDown(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
try
{
	HealthTeamElement *elemHits = NULL ;
  _Draws->isIn(point, &elemHits, HT_Icon) ;
  if (elemHits != NULL)
  {
  	IconElement *temp = dynamic_cast<IconElement *>(elemHits) ;
    if (temp != NULL)
    {
    	_popBuffer = temp ;
      NS_CLASSLIB::TRect  clientRect = GetClientRect() ;
      NS_CLASSLIB::TPoint ptOrigine  = clientRect.BottomRight() ;
			OWL::TPopupMenu *zoomMenu = new OWL::TPopupMenu() ;

			zoomMenu->AppendMenu(MF_STRING, IDC_MEN_1, "Voir la liste des mandats de ce membre") ;
			zoomMenu->AppendMenu(MF_STRING, IDC_MANDATE_SUP, "Supprimer ce mandat") ;
			if (temp->getMember()->getType() == NSHealthTeamMember::team)
      	zoomMenu->AppendMenu(MF_STRING, IDC_VIEWTEAM, "Voir cette équipe") ;

			ClientToScreen(point) ;
			zoomMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;
			delete zoomMenu ;
    }
  }
  else // ouverture du popup general
  {
  	NS_CLASSLIB::TRect  clientRect = GetClientRect() ;
    NS_CLASSLIB::TPoint ptOrigine  = clientRect.BottomRight() ;
    _PosMenu = point ;
		OWL::TPopupMenu *zoomMenu = new OWL::TPopupMenu() ;

		zoomMenu->AppendMenu(MF_STRING, IDC_MEN_ADD, "Ajouter un membre") ;
    zoomMenu->AppendMenu(MF_STRING, IDC_ADDTEAM, "Ajouter une équipe") ;
    zoomMenu->AppendMenu(MF_STRING, IDC_MEN_ALL, "Voir la liste des mandats") ;

		ClientToScreen(point) ;
		zoomMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;
		delete zoomMenu ;
  }
}
catch (...)
{
	// TODO
  // error
}
}

void
RosaceDrawer::EvLButtonDown(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	_bLButtonDown = true ;        																			// on indique que le bouton gauche est enfoné
  HealthTeamElement *elemHits = NULL ;
	_Draws->isIn(point, &elemHits, HT_Icon) ;	                         	// On cherche si un icone a été saisie
  if (elemHits != NULL)                                           		// Si c'est le cas on le met
  {
    IconElement* temp = dynamic_cast<IconElement *>(elemHits) ;
    if (temp != NULL)
    {
      HealthTeamElement *elemRegion = NULL ;
      _Draws->isIn(point, &elemRegion, HT_Petal) ;
      _dragInformation._drag_region = elemRegion ;

     	_ImgDragDrop->Add(/*OWL::TIcon(0, IDI_EXCLAMATION)*/ *temp->getIco()) ;
     	_ImgDragDrop->BeginDrag(0, 16, 16) ;
     	_ImgDragDrop->DragEnter(*this, point.x, point.y) ;
     	_dragInformation._drag_elem  = temp ;
     	_dragInformation._drag_point = point ;
    }
  }
  else
    _dragInformation._drag_point = point ;
 // DefaultProcessing() ;
}


void
RosaceDrawer::EvLButtonUp(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
try
{
	if (false == _bLButtonDown)
		return ;

	_ImgDragDrop->DragLeave(this->HWindow) ;
	_ImgDragDrop->EndDrag() ;
	_bLButtonDown = false ;
	if (_dragInformation._drag_elem != NULL)
	{
		_dragInformation._drop_point = point ;
		if (_dragInformation.noMove() == false)   // Il n'y a pas eut de déplacement
		{
    	// Est ce que la destination est une rosace
    	// Si oui le deplacer, sinon le laisser sur place
      HealthTeamElement* testing = NULL ;
      if ((_Draws->isIn(_dragInformation._drop_point, &testing, HT_Petal) == true) && (_dragInformation._drag_elem != NULL))
      	_dragInformation._drag_elem->setCoordinate(point.x - 16, point.y - 16) ;
      HealthTeamElement* elemDropRegion = NULL ;
      _Draws->isIn(point, &elemDropRegion, HT_Petal) ;      // On recupere la region d'arriver
      if ((elemDropRegion != NULL) && (_dragInformation._drag_region->IsEqual(elemDropRegion) == false))                         // si elle n'est pas null et qu'elles sont différentes
      {
      	// on rajoute l'element dans la region de destination
        elemDropRegion->AjouteElement(_dragInformation._drag_elem) ;
        // on retire l'element de la region de destination
        _dragInformation._drag_region->RemoveElement(_dragInformation._drag_elem) ;

        int angle = -1 ;
        string distance = "-1" ;
        // Recupere les informations sur le degre et sur l'angle  de la regions
        SubPetal* sup = dynamic_cast<SubPetal *>(elemDropRegion) ;
        if (sup != NULL)
        {
        	angle    = sup->getAngleView() ;
          distance = sup->getDistance() ;
        }
        else
        {
        	RosaceCenter* cen = dynamic_cast<RosaceCenter *>(elemDropRegion) ;
          if (cen != NULL)
          {
          	angle    = cen->getAngleView() ;
            distance = cen->getDistance() ;
          }
        }

        HealthTeamMandateInterface test(this, pContexte, _dragInformation._drag_elem->getName(), _dragInformation._drag_elem->getMandat(), angle, distance) ;
        test.Execute() ;
        getManager()->modMandate(pContexte, _dragInformation._drag_elem->getMember(), _dragInformation._drag_elem->getMandat()) ;
      }
      Invalidate(false) ;
    }
  }
  
  _dragInformation._drag_elem = NULL ;
}
catch(...)
{
}
}

void
RosaceDrawer::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  TButton::Paint(dc, erase, RectAPeindre) ;
  if (erase == true)
  	_Draws->Dessine(&dc, erase, RectAPeindre) ;
}

void
RosaceDrawer::EvLButtonDblClk(UINT /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	HealthTeamElement* elemHits = NULL ;
	_Draws->isIn(point, &elemHits, HT_Icon) ;
  if (NULL == elemHits)
		return ;

	IconElement* temp = dynamic_cast<IconElement *>(elemHits) ;
	if (temp != NULL)
	{
  	HealthTeamMandateInterface test(this, pContexte,temp->getName(), temp->getMandat()) ;
    test.Execute() ;
	}
}

void
RosaceDrawer::CmAddMember()
{
try
{
	NSHealthTeamMember *pMember ;
  string sPids = "" ;

  NSTPersonListDialog indep((TWindow *)this, pidsUtilisat, true, pContexte, 0, false, string(""), pNSResModule) ;
  int	iDialogReturn = indep.Execute() ;
  if (iDialogReturn != IDOK)
  	return ;

  if (indep.bCreer)
  {
    NSPersonInfo *pPersonInfo = pContexte->getPersonArray()->createPerson(pContexte, pidsCorresp) ;
    if (NULL != pPersonInfo)
    {
      sPids = pPersonInfo->getNss() ;  // On recupre le personne id
      pMember = new NSHealthTeamMember(sPids) ;
      if (pMember->isValid())
        getManager()->addMember(pMember, NSHealthTeamMember::person) ;                // On ajoute le membre
    }
    else
    	return ;
  }
  else
  {
    NSPersonInfo *temp = indep.pPersonSelect ;
    sPids = temp->getNss() ;
    if (string("") != sPids)
    {
      pMember = new NSHealthTeamMember(sPids) ;
      if (pMember->isValid())
        getManager()->addMember(pMember, NSHealthTeamMember::person) ;
    }
    else
    {
      // TODO
      // Rajouter l'erreur
    }
  }

  NSPersonInfo *pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, sPids, pidsCorresp) ;
  if (pPersonInfo)
  {
  	string sMemberName = pPersonInfo->getCivilite() ;
    NSMemberMandatePair* man = new NSMemberMandatePair(pContexte, pMember, new NSHealthTeamMandate()) ;
    man->setName(sMemberName) ;

    IconElement *tempIconE = new IconElement(pContexte, man, _PosMenu.x, _PosMenu.y) ;
    if (tempIconE)
    {
    	HealthTeamElement *elemRegion = NULL ;
      _Draws->isIn(_PosMenu, &elemRegion, HT_Petal) ;

      if (elemRegion)
      	elemRegion->AjouteElement(tempIconE) ;

     	int angle = -1 ;
     	string distance = "" ;

     	SubPetal *tempSubP = dynamic_cast<SubPetal *>(elemRegion) ;
     	if (tempSubP)
     	{
     		angle     = tempSubP->getAngleView() ;
        distance  = tempSubP->getDistance() ;
      }

     	RosaceCenter *tempRosC = dynamic_cast<RosaceCenter *>(elemRegion) ;
     	if (tempRosC)
     	{
        angle     = tempRosC->getAngleView() ;
        distance  = tempRosC->getDistance() ;
     	}

      NSHealthTeamMandate *mand = new NSHealthTeamMandate() ;
      HealthTeamMandateInterface test(this, pContexte, sMemberName, mand, angle, distance) ;
      test.Execute() ;
      getManager()->addMandate(pMember, mand) ;
    }
  }

  if (_bVerboseMode)
	{
  	NSToDoTask* pTask = new NSToDoTask ;

		pTask->setWhatToDo(string("RefreshHealthTeamInformation")) ;
		pTask->setParam1(string("MEMBER_NEW")) ;
    pTask->setParam2(string("ROSACE")) ;

  	pTask->sendMe(pContexte->getSuperviseur()) ;
  }
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception RosaceDrawer::CmAddMember: ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("Exception RosaceDrawer::CmAddMember.", standardError, 0) ;
}
}

void
RosaceDrawer::CmSupMember()
{
}

void
RosaceDrawer::CmSupMandat()
{
  if (NULL == _popBuffer)
    return ;

	HealthTeamElement* region = NULL ;

  NS_CLASSLIB::TPoint point = *_popBuffer->getCoordinate() ;
  _Draws->isIn(point, &region, HT_Petal) ;
  if (NULL == region)
  {
    _popBuffer = NULL ;
    return ;
  }

  region->RemoveElement(_popBuffer) ;
  getManager()->closeMandate(pContexte, _popBuffer->getMember(), _popBuffer->getMandat()) ;

  if ((_mandats) && (false == _mandats->empty()))
  {
    NSMemberMandatePairArray::iterator iter ;
    for (iter = _mandats->begin() ; _mandats->end() != iter ; iter++)
      if ((*iter) == _popBuffer->getPair())
      {
        delete ((*iter)) ;
        _mandats->erase(iter) ;
        break ;
      }
  }

  NSToDoTask* pTask = new NSToDoTask ;

  pTask->setWhatToDo(string("RefreshHealthTeamInformation")) ;
  pTask->setParam1(string("MEMBER_REMOVED")) ;
  pTask->setParam2(string("ROSACE")) ;

  pTask->sendMe(pContexte->getSuperviseur()) ;
}


void
RosaceDrawer::CmAddTeam()
{
  string sObjectID = "" ;
  NSTTeamListDialog	teamListDialog((TWindow *)this, true, pContexte, pNSResModule) ;
  int	iDialogReturn = teamListDialog.Execute() ;
  if (iDialogReturn == IDOK)
  {
  	if (teamListDialog.bCreer)
    {
  		NSTeamGraphManager *pTeamManager = new NSTeamGraphManager(pContexte->getSuperviseur()) ;
    	NSMoralPerson	*pMPTeam = pTeamManager->createTeam(pContexte) ;
      if (pMPTeam != NULL)
      {
  			GraphicHealthTeam	*gHTInterface	= new GraphicHealthTeam(this, pContexte, pMPTeam) ;
  			int	iGHTReturn = gHTInterface->Execute() ;
  			delete gHTInterface ;

      	pTeamManager->setTeamGraph(pMPTeam->getPatho()) ;
        sObjectID = pTeamManager->getRootObject() ;
      }
      delete pTeamManager ;
    }
    else
    {
    	NSTeamDisplay *pTeam = teamListDialog.pSelectedTeam ;
      if (pTeam != NULL)
      	sObjectID	= pTeam->getID() ;
    }
  }
  else
  	return ;

  if (sObjectID != "")
  {
  	NSPatPathoArray	*ppt = new NSPatPathoArray(pContexte->getSuperviseur()) ;
  	NSTeamGraphManager *pTeamManager = new NSTeamGraphManager(pContexte->getSuperviseur()) ;
  	pTeamManager->getTeamGraph(sObjectID, ppt) ;
    string	sTeamName	= pTeamManager->getTeamName(ppt) ;
    NSHealthTeamMember *pMember = new NSHealthTeamMember(sObjectID) ;
    NSMemberMandatePair *mmPair = new NSMemberMandatePair(pContexte, pMember, new NSHealthTeamMandate()) ;
    mmPair->setName(sTeamName) ;

    IconElement	*iconE = new IconElement(pContexte, mmPair, _PosMenu.x, _PosMenu.y) ;
    if (iconE != NULL)
    {
    	HealthTeamElement *htElemRegion = NULL ;
      _Draws->isIn(_PosMenu, &htElemRegion, HT_Petal) ;

      if (htElemRegion != NULL)
      	htElemRegion->AjouteElement(iconE) ;

      int     angle	= -1 ;
      string  dist	= "" ;
      SubPetal *subP = dynamic_cast<SubPetal *>(htElemRegion) ;
      if (subP != NULL)
      {
      	angle = subP->getAngleView() ;
        dist  = subP->getDistance() ;
      }

      RosaceCenter	*rosC = dynamic_cast<RosaceCenter *>(htElemRegion) ;
      if (rosC != NULL)
      {
      	angle = rosC->getAngleView() ;
        dist  = rosC->getDistance() ;
      }

      NSHealthTeamMandate	*mandate = new NSHealthTeamMandate() ;
      HealthTeamMandateInterface	mandateI(this, pContexte, sTeamName, mandate, angle, dist) ;
      mandateI.Execute() ;

      getManager()->addMember(pMember, NSHealthTeamMember::team) ;
      getManager()->addMandate(pMember, mandate) ;
      // TODO FIXME
      // il manque l'appel au service - FLP
    }
    // delete ppt ;
    // delete pTeamManager ;
  }
}

void
RosaceDrawer::CmViewTeam()
{
	HealthTeamElement *region = (HealthTeamElement*) 0 ;
  if (_popBuffer)
  {
	 	NS_CLASSLIB::TPoint point = *_popBuffer->getCoordinate() ;
   	_Draws->isIn(point, &region, HT_Petal) ;

   	if (region)
   	{
  		NSTeamGraphManager TeamManager(pContexte->getSuperviseur()) ;
      NSPatPathoArray ppt(pContexte->getSuperviseur()) ;
    	bool bMPTeam = TeamManager.getTeamGraph(_popBuffer->getMember()->getID(), &ppt) ;
      if (bMPTeam)
      {
      	NSMoralPerson MPTeam(pContexte, &ppt) ;
  			GraphicHealthTeam	gHTInterface(this, pContexte, &MPTeam) ;
  			int	iGHTReturn = gHTInterface.Execute() ;
      }
		}
  }
}

void
RosaceDrawer::SetupWindow()
{
	TButton::SetupWindow() ;
  NS_CLASSLIB::TRect rosaceRect = GetClientRect() ;
  if (!&rosaceRect)
  	return ;

  _height = rosaceRect.Height() ;
  _width  = rosaceRect.Width() ;
  int iHeigth = min(abs(_height), abs(_width)) ;
  _max_radius	= (iHeigth / 2) - 1 ;

  _ptCenter.x = rosaceRect.Left() + _max_radius + 1 ;
  _ptCenter.y = rosaceRect.Top() + _max_radius + 1 ;

  InitRosace() ;
  InitialiseMandat() ;
  _Draws->AdjustElementOnScreen() ;
}


RosaceDrawer::~RosaceDrawer()
{
	if (_Draws)
  	delete (_Draws) ;

  if (_ImgDragDrop)
  {
  	_ImgDragDrop->RemoveAll() ;
		delete _ImgDragDrop ;
  }

  if (_pToolTip)
    delete _pToolTip ;

  if (_mandats)
    delete _mandats ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
RosaceManager::RosaceManager(NSContexte *pCtx, NSMoralPerson *pMPTeam)
              :NSRoot(pCtx)
{
	_tManager = new NSMoralPersonManager(pMPTeam) ;
}

RosaceManager::RosaceManager(NSContexte *pCtx, NSHealthTeam *pHTeam)
	:	NSRoot(pCtx)
{
	_tManager	= new NSHealthTeamManager(pHTeam) ;
}

RosaceManager::~RosaceManager()
{
  if (_tManager)
		delete _tManager ;
}

void
RosaceManager::reset(NSMoralPerson *pMPTeam)
{
	if (_tManager)
		delete _tManager ;

	_tManager = new NSMoralPersonManager(pMPTeam) ;
}

void
RosaceManager::reset(NSHealthTeam *pHTeam)
{
	if (_tManager)
		delete _tManager ;

	_tManager	= new NSHealthTeamManager(pHTeam) ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(GraphicHealthTeam, TDialog)
	EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(IDHELP, CmHelp),
END_RESPONSE_TABLE ;


GraphicHealthTeam::GraphicHealthTeam(TWindow *pParentWindow, NSContexte *pCtx, NSMoralPerson *pMPTeam)
	: NSUtilDialog(pParentWindow, pCtx, "IDD_GRAPHIC_HEALTH_TEAM", pNSResModule)
{
  manager	= new RosaceManager(pContexte, pMPTeam) ;
	rosace	= new RosaceDrawer(pContexte, this, IDC_ROSACE, manager) ;

  pOkButton     = new OWL::TButton(this, IDOK) ;
  pCancelButton = new OWL::TButton(this, IDCANCEL) ;
	pHelpButton   = new OWL::TButton(this, IDHELP) ;
}


GraphicHealthTeam::GraphicHealthTeam(TWindow *parent, NSContexte *pCtx, NSHealthTeam *pHTeam)
	: NSUtilDialog(parent, pCtx, "IDD_GRAPHIC_HEALTH_TEAM", pNSResModule)
{
	manager	= new RosaceManager(pContexte, pHTeam) ;
  rosace	= new RosaceDrawer(pContexte, this, IDC_ROSACE, manager) ;

  pOkButton     = new OWL::TButton(this, IDOK) ;
  pCancelButton = new OWL::TButton(this, IDCANCEL) ;
	pHelpButton   = new OWL::TButton(this, IDHELP) ;
}


void
GraphicHealthTeam::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	string sLocalisationText = pContexte->getSuperviseur()->getText("generalLanguage", "team") ;

	if (((RosaceDrawer *)rosace)->getManager()->isMPTeamManager())
  {
  	string sTeamName = sLocalisationText + string(": ") + ((RosaceDrawer *)rosace)->getManager()->getMPTeam()->getLabel() ;
  	SetCaption(sTeamName.c_str()) ;
	}

	sLocalisationText = pContexte->getSuperviseur()->getText("generalLanguage", "ok") ;
  pOkButton->SetCaption(sLocalisationText.c_str()) ;

  sLocalisationText = pContexte->getSuperviseur()->getText("generalLanguage", "cancel") ;
  pCancelButton->SetCaption(sLocalisationText.c_str()) ;

  sLocalisationText = pContexte->getSuperviseur()->getText("generalLanguage", "help") ;
  pHelpButton->SetCaption(sLocalisationText.c_str()) ;
}


/*
void
GraphicHealthTeam::EvClose()
{
        TWindow::EvClose();
}
*/


GraphicHealthTeam::~GraphicHealthTeam()
{
	if (NULL != rosace)
  {
    delete (rosace) ;
    rosace = NULL ;
  }

  delete pOkButton ;
  delete pCancelButton ;
	delete pHelpButton ;
}

void
GraphicHealthTeam::CmOk()
{
	if (((RosaceDrawer *)rosace)->getManager()->isHTeamManager())
  {
    NSNoyauDocument noyau((TDocument*) 0, 0, 0, pContexte, false, true) ;
    bool            bContinuer = true ;

    NSHISTODocument *pDocHis = pContexte->getPatient()->getDocHis() ;
    DocumentIter iterDoc = pDocHis->getVectDocument()->begin() ;
    while ((pDocHis->getVectDocument()->end() != iterDoc) && bContinuer)
    {
      NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
      (*iterDoc)->initFromPatPatho(&PPT) ;

      if (false == PPT.empty())
      {
        PatPathoIter iter = PPT.begin() ;
        if ((*iter)->getLexique() == string("LEQUI1"))
          bContinuer = false ;
        else
          iterDoc++ ;
      }
      else
        iterDoc++ ;
    }

    if (false == bContinuer)
    {
    	(noyau._pDocInfo) = new NSDocumentInfo(pContexte) ; // *((*iterDoc)->pDonnees) ;

      NSDocumentData InfoData ;
      (*iterDoc)->initFromData(&InfoData) ;
      noyau._pDocInfo->setData(&InfoData) ;

    	noyau.setPatPatho(((RosaceDrawer *)rosace)->getManager()->getPatho()) ;
    	noyau.enregistrePatPatho() ;
    }
  }
  else if (((RosaceDrawer *)rosace)->getManager()->isMPTeamManager())
  {
  	// fabFIXME
    // il faut enregistrer l'equipe médicale (en faits il faut la modifier)
  }
  else
  {
  	// erreur on ne sait pas quel genre d'équipe on traite
  }
  NSUtilDialog::CmOk() ;
}

void
GraphicHealthTeam::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}


void
GraphicHealthTeam::CmHelp()
{
	NSUtilDialog::CmHelp() ;
}

