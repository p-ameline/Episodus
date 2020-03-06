// -----------------------------------------------------------------------------
// NSRightsManager.cpp
// -----------------------------------------------------------------------------
// Manager des droits des utilisateurs
// -----------------------------------------------------------------------------
// $Revision: 1.6 $
// $Author: pameline $
// $Date: 2015/02/07 14:22:14 $
// -----------------------------------------------------------------------------
// FLP - juin 2004
// CH  - mai 2004
// -----------------------------------------------------------------------------
/*
   Copyright (C) 2004, Nautilus

This file is part of Episodus.

Episodus is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

Episodus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Episodus; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.
*/
#include <owl\dc.h>
#include "partage\nsdivfct.h"
#include "nssavoir\nsRightsManager.h"
#include "nautilus\nssuper.h"
#include "nssavoir\nssavoir.h"
#include "nssavoir\nssavoir.rh"
#include "nssavoir\RosaceDrawUtil.h"

extern "C"
{
#include <math.h>
}

// rosace's drawing size in percent
// (computed from the max radius possible in the widow)
const int OccupationRosacePercent = 88;

bool
infRightPos(NSRightPosition *s, NSRightPosition *b)
{
	if (s->getDistance() == b->getDistance())
    	return (s->getSpeciality() == sameSpec) ;

	return (s->getDistance() < b->getDistance()) ;
}

// -----------------------------------------------------------------------------
// class NSRightPosition
// -----------------------------------------------------------------------------
NSRightPosition::NSRightPosition(NSContexte *pCtx, DistanceType distance, SpecialityType speciality, int theColor, string sTitle)
				:NSRoot(pCtx)
{
	sPositionTitle = sTitle ;
	dist           = distance ;
	spec           = speciality ;
  color          = theColor ;
}

NSRightPosition::NSRightPosition(const NSRightPosition& rv)
                :NSRoot(rv.pContexte)
{
	sPositionTitle = rv.sPositionTitle ;
	dist 			= rv.dist ;
	spec			= rv.spec ;
	color			= rv.color ;
}

NSRightPosition::~NSRightPosition()
{
}

void
NSRightPosition::draw(TDC* pDc, bool /* erase */, NS_CLASSLIB::TRect& /* RectAPeindre */, TBrush** pBrushes, NSRightPosition* pSelectedPos)
{
try
{
	if (!pDc || !pBrushes || (iDeltaAngle == 0))
    	return ;

    // pContexte->getSuperviseur()->Delay(100) ;

    NS_CLASSLIB::TColor* pPenColor ;
    // if (iZOrder > 5)
    //	pPenColor = new TColor(NS_CLASSLIB::TColor::Black) ;
    if (pSelectedPos == this)
    	pPenColor = new TColor(NS_CLASSLIB::TColor::LtRed) ;
    else
    	pPenColor = new TColor(NS_CLASSLIB::TColor::Gray) ;

    TPen thePen(*pPenColor, 1) ;
    ::SelectObject(pDc->GetHDC(), thePen) ;

    TBrush* pTheBrush = 0 ;
    if ((color > 0) && (color <= 3))
    	pTheBrush = pBrushes[color] ;
    else
    	pTheBrush = pBrushes[0] ;

    if (iDeltaAngle == 360)
    {
    	NS_CLASSLIB::TRect patientRect(	ptCenter.X() - iRadius,
    									ptCenter.Y() + iRadius,
    									ptCenter.X() + iRadius,
    									ptCenter.Y() - iRadius) ;
        ::SelectObject(pDc->GetHDC(), *pTheBrush) ;
		pDc->Ellipse(patientRect) ;
        return ;
    }

    if (pTheBrush)
    	::SelectObject(pDc->GetHDC(), *pTheBrush) ;

    /*

    pDc->MoveTo(ptCenter) ;
    pDc->AngleArc(ptCenter.X(), ptCenter.Y(), iRadius, iStartAngle, iDeltaAngle) ;
    pDc->LineTo(ptCenter) ;

    // Remplissage
    int 	iHalfAngle = iStartAngle + ( iDeltaAngle / 2 ) ;
    double	dRadAngle = double(iHalfAngle) * 3.1415926 / double(180)  ;

    double dCos = cos(dRadAngle) ;
    double dSin = sin(dRadAngle) ;
    // double dXin = ( (double)iRadius / (double)2 ) * cos((double)iHalfAngle)  ;
    // double dYin = ( (double)iRadius / (double)2 ) * sin((double)iHalfAngle)  ;
    double dXin = ( (double)4 * (double)iRadius / (double)5 ) * dCos  ;
    double dYin = ( (double)4 * (double)iRadius / (double)5 ) * dSin  ;

    NS_CLASSLIB::TPoint pointIn ;
    pointIn.x = ptCenter.x + int(dXin) ;
    pointIn.y = ptCenter.y - int(dYin) ;

    if (pTheBrush)
    	pDc->ExtFloodFill(pointIn, *pPenColor, FLOODFILLBORDER) ;

    */

    NS_CLASSLIB::TRect boudingRect(	ptCenter.X() - iRadius,
    								ptCenter.Y() + iRadius,
                                    ptCenter.X() + iRadius,
                                    ptCenter.Y() - iRadius) ;

    double dRadStartAngle = double(iStartAngle) * 3.1415926 / double(180)  ;
    double dStartX = (double)iRadius * cos(dRadStartAngle) ;
    double dStartY = (double)iRadius * sin(dRadStartAngle) ;

    double dRadEndAngle = double(iStartAngle + iDeltaAngle) * 3.1415926 / double(180)  ;
    double dEndX = (double)iRadius * cos(dRadEndAngle) ;
    double dEndY = (double)iRadius * sin(dRadEndAngle) ;

    NS_CLASSLIB::TPoint pointStart(ptCenter.x + int(dStartX), ptCenter.y - int(dStartY)) ;
    NS_CLASSLIB::TPoint pointEnd(ptCenter.x + int(dEndX), ptCenter.y - int(dEndY)) ;

    pDc->Pie(boudingRect, pointStart, pointEnd) ;
}
catch (...)
{
	erreur("Exception NSRightPosition::draw", standardError, 0) ;
}
}

bool
NSRightPosition::isBoxOnPoint(int iPointRadius, int iPointDegreAngle)
{
	if ((iPointRadius <= iRadius ) &&
    	(iPointDegreAngle >= iStartAngle) &&
        (iPointDegreAngle <= iStartAngle + iDeltaAngle))
    return true ;

  return false ;
}

void
NSRightPosition::setNextColor()
{
	color-- ;
  if (color <= 0)
    color = 3 ;
}

NSRightPosition&
NSRightPosition::operator=(const NSRightPosition& src)
{
  if (this == &src)
    return *this ;

  sPositionTitle = src.sPositionTitle ;
  dist           = src.dist ;
  spec           = src.spec ;
  color          = src.color ;

  return *this ;
}

bool
NSRightPosition::operator==(const NSRightPosition& src)
{
  return ((sPositionTitle == src.sPositionTitle) && (dist == src.dist) && (spec == src.spec)) ;
}

bool
NSRightPosition::operator!=(const NSRightPosition& src)
{
	return !((sPositionTitle == src.sPositionTitle) && (dist == src.dist) && (spec == src.spec)) ;
}

// -----------------------------------------------------------------------------
// class NSPetal
// -----------------------------------------------------------------------------

NSPetal::NSPetal(NSContexte* pCtx, int iCtAngle, PetalType iCtType, PetalLevel iCtLevel, int iRights[], std::string petal_title)
        :NSRightVector(), NSRoot(pCtx)
{

	iAngle 	= iCtAngle ;
  iType 	= iCtType ;
  iLevel 	= iCtLevel ;
  if (petal_title != "")
    sPetalTitle = petal_title ;
  else
  {
    char test[3] ;
    itoa(iCtAngle, test, 10) ;
    std::string sPetalSearch = "angle" + std::string(test) ;
    sPetalTitle = pCtx->getSuperviseur()->getText("rosaceManagement", sPetalSearch) ;
    if (iCtLevel == physicalLevel)
      sPetalTitle += "\n " + pCtx->getSuperviseur()->getText("rosaceManagement", "phys") ;
    else
      sPetalTitle += "\n " + pCtx->getSuperviseur()->getText("rosaceManagement", "moral") ;
  }

  std::string proche  = "\n" + pCtx->getSuperviseur()->getText("rosaceManagement", "distance1") ;
  std::string eloign  = "\n" + pCtx->getSuperviseur()->getText("rosaceManagement", "distance2") ;
  std::string samespe = "\n" + pCtx->getSuperviseur()->getText("rosaceManagement", "samespe") ;
  std::string diffspe = "\n" + pCtx->getSuperviseur()->getText("rosaceManagement", "diffspe") ;

  if 		(iCtType == patientType)
  {
    push_back(new NSRightPosition(pContexte, nearDist, sameSpec, iRights[0],sPetalTitle )) ;
    return ;
  }

  if 		(iCtType == classicType)
  {
    push_back(new NSRightPosition(pContexte, nearDist, sameSpec, iRights[0],sPetalTitle + proche)) ;
    push_back(new NSRightPosition(pContexte, farDist, sameSpec, iRights[2],sPetalTitle+ eloign)) ;
  }
  else if (iCtType == focusType)
  {
    push_back(new NSRightPosition(pContexte, nearDist, sameSpec, iRights[0],sPetalTitle + proche + samespe)) ;
    push_back(new NSRightPosition(pContexte, nearDist, difSpec, iRights[1],sPetalTitle+ proche + diffspe)) ;
    push_back(new NSRightPosition(pContexte, farDist, sameSpec, iRights[2],sPetalTitle +eloign + samespe)) ;
    push_back(new NSRightPosition(pContexte, farDist, difSpec, iRights[3],sPetalTitle+ eloign + diffspe)) ;
  }

  sort(begin(), end(), infRightPos) ;
}

NSPetal::NSPetal(const NSPetal& rv)
        :NSRightVector(), NSRoot(rv.pContexte)
{
try
{
	iAngle = rv.iAngle ;
  iType  = rv.iType ;
  iLevel = rv.iLevel ;

  if (false == rv.empty())
    for (NSRightsConstIter it = rv.begin() ; rv.end() != it ; it++)
			push_back(*it) ;

  sort(begin(), end(), infRightPos) ;
}
catch (...)
{
	erreur("Exception NSPetal ctor.", standardError, 0) ;
}
}

void
NSPetal::resetPetal(string sRights)
{
}

string
NSPetal::getRightsString()
{
	if (empty())
		return string("") ;

	char bitColorSameSpec  = 0x00 ;
  char bitColorOtherSpec = 0x10 ;

  if (iLevel == moralLevel)
  {
    bitColorSameSpec  += 0x20 ;
    bitColorOtherSpec += 0x20 ;
  }

  bool bExistSameSpec  = false ;
  bool bExistOtherSpec = false ;

	int		iDecalage	= 0 ;

  for (DistanceType iDist = patDist ; iDist <= farDist ; iDist = DistanceType(iDist + 1))
  {
    for (SpecialityType iSpec = sameSpec ; iSpec <= difSpec ; iSpec = SpecialityType(iSpec + 1))
    {
      NSRightPosition* pRPos = getRP(iDist, iSpec) ;
      if ((NULL != pRPos) && (pRPos->getColor() != 0))
      {
        switch (pRPos->getDistance())
				{
					case patDist :
          case nearDist :
						iDecalage = 2 ;
						break ;
          case farDist :
						iDecalage = 0 ;
						break ;
				}

        if (pRPos->getSpeciality() == sameSpec)
        {
          bExistSameSpec = true ;
          if (iDecalage > 0)
            bitColorSameSpec += (char)(pRPos->getColor() << iDecalage) ;
          else
            bitColorSameSpec += (char) pRPos->getColor() ;
        }
        else
        {
          bExistOtherSpec = true ;
          if (iDecalage > 0)
            bitColorOtherSpec += (char)(pRPos->getColor() << iDecalage) ;
          else
            bitColorOtherSpec += (char) pRPos->getColor() ;
        }
      }
    }
  }

  string sReturn = string("") ;
  if (bExistSameSpec)
    sReturn += string(1, bitColorSameSpec) ;
  if (bExistOtherSpec)
    sReturn += string(1, bitColorOtherSpec) ;

  return sReturn ;
}

NSRightPosition*
NSPetal::getRP(DistanceType	distType, SpecialityType specType)
{
	if (empty())
		return NULL ;

	NSRightsIter iter = begin() ;
	for ( ; iter != end() &&
            		(((*iter)->getSpeciality() != specType) ||
                     ((*iter)->getDistance() != distType)) ; iter++) ;

	if (iter == end())
		return NULL ;

	return *iter ;
}

void
NSPetal::vider()
{
	if (empty())
		return ;

	for (NSRightsIter i = begin() ; i != end() ; )
	{
		delete *i ;
		erase(i) ;
	}
}

int
NSPetal::getTrigoAngle()
{
	int iTrigoAngle = 180 - (15 * iAngle)  ;
  if (iTrigoAngle < 0)
    iTrigoAngle += 360 ;

  return iTrigoAngle ;
}

void
NSPetal::prepareRights(NS_CLASSLIB::TPoint* pCenter, int iRadius, int iDeltaAngle)
{
  if (empty() || !pCenter)
    return ;

  PetalUnity iUnity ;
  //
  // Familly = simplex petal
  //
  if ((iAngle > 18) && (iAngle < 24))
    iUnity = simplex ;
  else
    iUnity = duplex ;

  NSRightsIter itRights = begin() ;

  // Patient -> center circle
  //
  if (iAngle == 0)
  {
    (*itRights)->iZOrder     = 9 ;

    (*itRights)->iStartAngle = 0 ;
    (*itRights)->iDeltaAngle = 360 ;
    (*itRights)->iRadius     = iRadius / 5 ;
		(*itRights)->ptCenter    = *pCenter ;

    return ;
  }

  int	iTrigoAngle = getTrigoAngle() ;
	int iHalfPetalAngle ;

  for (; itRights != end(); itRights++)
  {
    (*itRights)->ptCenter	= *pCenter ;

 		//
    // Long petal (lone petal or large petal of a twin)
    //
    if ((iUnity == simplex) || (iLevel == moralLevel))
    {
      if 		((*itRights)->getDistance() == nearDist)
      {
        (*itRights)->iRadius	= 3 * iRadius / 4 ;
        (*itRights)->iZOrder	= 5 ;
      }
      else if ((*itRights)->getDistance() == farDist)
      {
        (*itRights)->iRadius	= iRadius ;
        (*itRights)->iZOrder	= 4 ;
      }

      iHalfPetalAngle = iDeltaAngle / 2 ;
    }
    //
    // Small petal : "above" petal in a twin
    //
    else
    {
      if 		((*itRights)->getDistance() == nearDist)
      {
        (*itRights)->iRadius 	= 3 * iRadius / 4 ;
        (*itRights)->iZOrder	= 7 ;
      }
      else if ((*itRights)->getDistance() == farDist)
      {
        (*itRights)->iRadius 	= 6 * iRadius / 7 ;
        (*itRights)->iZOrder	= 6 ;
      }

      iHalfPetalAngle = iDeltaAngle / 3 ;
    }

    //
    // No speciality : use the whole width
    //
    if 		(iType == classicType)
    {
      (*itRights)->iStartAngle = iTrigoAngle - iHalfPetalAngle ;
      (*itRights)->iDeltaAngle = 2 * iHalfPetalAngle ;
    }
    //
    // With speciality : use half the width
    //
		else if (iType == focusType)
		{
			(*itRights)->iDeltaAngle	= iHalfPetalAngle ;

			if 		((*itRights)->getSpeciality() == sameSpec)
				(*itRights)->iStartAngle = iTrigoAngle ;
			else if ((*itRights)->getSpeciality() == difSpec)
				(*itRights)->iStartAngle = iTrigoAngle - iHalfPetalAngle ;
		}
	}
}

void
NSPetal::draw(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre, int iZo, TBrush** pBrushes, NSRightPosition* pSelectedPos)
{
try
{
	if (empty())
    	return ;

  NSRightsIter itRights = begin() ;
  for (; itRights != end(); itRights++)
    if ((*itRights)->iZOrder == iZo)
      (*itRights)->draw(pDc, erase, RectAPeindre, pBrushes, pSelectedPos) ;

  return ;
}
catch (...)
{
	erreur("Exception NSPetal::draw", standardError, 0) ;
}
}

bool
NSPetal::isBoxOnPoint(NSRightPosition** pClickedRight, int iPointRadius, int iPointDegreAngle, int iZo)
{
	*pClickedRight = 0 ;

	if (empty())
    return false ;

  NSRightsIter itRights = begin() ;
  for (; itRights != end(); itRights++)
    if (((*itRights)->iZOrder == iZo) && ((*itRights)->isBoxOnPoint(iPointRadius, iPointDegreAngle)))
    {
      *pClickedRight = *itRights ;
      return true ;
    }

  return false ;
}

NSPetal::~NSPetal()
{
	vider() ;
}

NSPetal&
NSPetal::operator=(const NSPetal& rv)
{
try
{
  if (this == &rv)
    return *this ;

	if (false == rv.empty())
		for (NSRightsConstIter it = rv.begin() ; rv.end() != it ; it++)
			push_back(*it) ;

  iAngle 	= rv.iAngle ;
  iType 	= rv.iType ;
  iLevel 	= rv.iLevel ;

  sort(begin(), end(), infRightPos) ;

  return *this ;
}
catch (...)
{
	erreur("Exception NSPetal = operator.", standardError, 0) ;
  return *this ;
}
}

// -----------------------------------------------------------------------------
// fonction globale de tri :
//			Permet de trier les NSLdvGoalInfo par ordre de date
// -----------------------------------------------------------------------------
bool
infPetal(NSPetal *s, NSPetal *b)
{
	if (s->getAngle() == b->getAngle())
    return (s->getLevel() == physicalLevel) ;

	return (s->getAngle() < b->getAngle()) ;
}


// -----------------------------------------------------------------------------
// class NSRosace
// -----------------------------------------------------------------------------

//
// Old rosace
//
NSRosace::NSRosace(NSContexte *pCtx, string sRights, NSHealthTeamMandate* pMdt)
         :NSPetalVector(), NSRoot(pCtx)
{
	pMandate        = pMdt ;
	sRightsAsString = sRights ;
  pSelectedRP     = 0 ;
  iRadius         = 0 ;
  bNeedPainting   = true ;

  sReferenceSpeciality    = "" ;
  sReferenceSpecialityLib = "" ;
  sReferenceJob           = "" ;
	sReferenceJobLib        = "" ;

  bReadOnly	= true ;

  resetRosace(sRightsAsString) ;
}

//
// New rosace
//
NSRosace::NSRosace(NSContexte *pCtx, NSHealthTeamMandate* pMdt)
         :NSPetalVector(), NSRoot(pCtx)
{
  pMandate 				= pMdt ;
  sRightsAsString = string("") ;
  pSelectedRP     = 0 ;
  iRadius         = 0 ;
  bNeedPainting   = true ;

  sReferenceSpeciality    = string("") ;
  sReferenceSpecialityLib	= string("") ;
  sReferenceJob           = string("") ;
  sReferenceJobLib        = string("") ;

  bReadOnly = false ;

  _synchro = false ;
  _personnal_note = false ;

  createPetals() ;
}

NSRosace::NSRosace(const NSRosace& rv)
         :NSPetalVector(), NSRoot(rv.pContexte)
{
try
{
	pMandate				= rv.pMandate ;
	sRightsAsString = rv.sRightsAsString ;
	pSelectedRP			= 0 ;

	iRadius					= rv.iRadius ;
	bNeedPainting		= true ;

	sReferenceSpeciality    = rv.sReferenceSpeciality ;
	sReferenceSpecialityLib	= rv.sReferenceSpecialityLib ;
	sReferenceJob           = rv.sReferenceJob ;
	sReferenceJobLib        = rv.sReferenceJobLib ;

  _synchro        = rv._synchro;
  _personnal_note = rv._personnal_note;

	resetRosace(sRightsAsString) ;
}
catch (...)
{
	erreur("Exception NSRosace copy ctor", standardError, 0) ;
}
}

NSRosace::~NSRosace()
{
	vider() ;
}

bool
NSRosace::createPetals()
{
try
{
	//
  // First, the header
  //
	sRightsAsString = string("\x10") ;

  // Colors : - green
  //
  string sSameSpecAllGreen  = string("\x0F\x2F") ;
  string sOtherSpecAllGreen = string("\x1F\x3F") ;

  // Patient petals ( \x42 = 01000010 )
  //
  string sPatientStandard 	= string("\x20") + string("3") + string("\x42\x0C") + string(1, char(GROUPEND)) ;

  //
  // Doctors
  //
  // Focus petals  ( \x23 = 00100011 )
	string sDocFocusAllGreen	= string("\x23") + string("6") + string("\x23") + sSameSpecAllGreen + sOtherSpecAllGreen + string(1, char(GROUPEND)) ;
  // Simple petals ( \x13 = 00010011 )
  string sDocSimpleAllGreen	= string("\x23") + string("4") + string("\x13") + sSameSpecAllGreen + string(1, char(GROUPEND)) ;

  //
  // Health professionals
  //
  // Focus petals
  //
  // PS same job
	string sPsFocusSameJobAllGreen	= string("\x27") + string("6") + string("\x23") + sSameSpecAllGreen + sOtherSpecAllGreen + string(1, char(GROUPEND)) ;
  // PS standard
  string sPsFocusAllGreen					= string("\x29") + string("6") + string("\x23") + sSameSpecAllGreen + sOtherSpecAllGreen + string(1, char(GROUPEND)) ;
  //
  // Simple petals
  //
  string sPsSimpleAllGreen				= string("\x29") + string("4") + string("\x13") + sSameSpecAllGreen + string(1, char(GROUPEND)) ;

  //
  // Other professionals
  //
  // Simple petals
  //

  string sSimpleRedRed = string("\x05\x25") ;   // 5 = 0101

  // Pro non PS
  string sProSameJobMedical	= string("\x2D") + string("4") + string("\x13") + sSimpleRedRed + string(1, char(GROUPEND)) ;
  // Social
  string sSocialMedical			= string("\x2F") + string("4") + string("\x13") + sSimpleRedRed + string(1, char(GROUPEND)) ;
  // Administrative
  string sAdminMedical			= string("\x31") + string("4") + string("\x13") + sSimpleRedRed + string(1, char(GROUPEND)) ;

  //
  // Familly ( \x12 = 00010010 )
  //
  string sSimpleNoDomainYellowRed = string("\x09") ;  // 9 = 1001

  string sFamilyMedical			= string("\x35") + string("3") + string("\x12") + sSimpleNoDomainYellowRed + string(1, char(GROUPEND)) ;

  // No mandate
  //
	if (NULL == pMandate)
	{
		sRightsAsString +=	sPatientStandard +
    										sDocSimpleAllGreen + sPsSimpleAllGreen +
                        sSocialMedical + sAdminMedical + sFamilyMedical ;
		resetRosace(sRightsAsString) ;
		return true ;
	}
	//
  // The patient
  //
	if (pMandate->getAngle() == 0)
		sRightsAsString +=	sPatientStandard +
    										sDocSimpleAllGreen + sPsSimpleAllGreen +
                        sSocialMedical + sAdminMedical + sFamilyMedical ;
	//
	// A doctor
	//
	if (pMandate->getAngle() == 3)
		sRightsAsString +=	sPatientStandard +
      									sDocFocusAllGreen + sPsFocusAllGreen +
                        sSocialMedical + sAdminMedical + sFamilyMedical ;
	//
	// A health professional
	//
	if (pMandate->getAngle() == 9)
  	sRightsAsString +=	sPatientStandard +
    										sDocFocusAllGreen +	sPsFocusSameJobAllGreen + sPsFocusAllGreen +
                        sSocialMedical + sAdminMedical + sFamilyMedical ;
	//
	// Another professional
	//
	if ((pMandate->getAngle() == 15) || (pMandate->getAngle() == 17))
  	sRightsAsString +=	sPatientStandard +
    										sDocSimpleAllGreen + sPsSimpleAllGreen +
                        sProSameJobMedical + sSocialMedical + sAdminMedical + sFamilyMedical ;
	//
	// A family member
	//
	if (pMandate->getAngle() == 21)
  	sRightsAsString +=	sPatientStandard +
    										sDocSimpleAllGreen + sPsSimpleAllGreen +
                        sSocialMedical + sAdminMedical + sFamilyMedical ;

	if (pMandate->getProfession() != "")
  	sRightsAsString += 	string("M") +
    										string(1, char(strlen(pMandate->getProfession().c_str())+1)) +
                        pMandate->getProfession() + string("\xFF") ;

  if (pMandate->getSpeciality() != "")
  	sRightsAsString += 	string("S") +
    										string(1, char(strlen(pMandate->getSpeciality().c_str())+1)) +
                        pMandate->getSpeciality() + string("\xFF") ;

	resetRosace(sRightsAsString) ;

	return true ;

/*
    bool	bMax = true ;
    int		iColors[4] ;
	// Doctor
    //
    std::string med = pContexte->getSuperviseur()->getText("rosaceManagement","angle3");
    if (bMax)
    //if (hasMandatForAngle(3) || hasMandatForAngle(9))
    {
    	iColors[0] = 3 ; iColors[1] = 3 ; iColors[2] = 2 ; iColors[3] = 1 ;
        push_back(new NSPetal(pContexte, 3, focusType, physicalLevel, iColors,"Personne physique")) ;
        iColors[0] = 3 ; iColors[1] = 2 ; iColors[2] = 2 ; iColors[3] = 1 ;
    	push_back(new NSPetal(pContexte, 3, focusType, moralLevel, iColors,"Personne Morale")) ;
    }
    else
    {
        iColors[0] = 3 ; iColors[1] = 3 ; iColors[2] = 0 ; iColors[3] = 0 ;
    	push_back(new NSPetal(pContexte, 3, classicType, physicalLevel, iColors,"Personne physique")) ;
        iColors[0] = 2 ; iColors[1] = 2 ; iColors[2] = 0 ; iColors[3] = 0 ;
    	push_back(new NSPetal(pContexte, 3, classicType, moralLevel, iColors,"Personne Morale")) ;
    }
    //
    // Health Professional
    //
    // Same view angle = 7 ; Other view angle = 9
    //
     std::string  prof_sant = pContexte->getSuperviseur()->getText("rosaceManagement","angle9");
    if (bMax)
    //if (hasMandatForAngle(9) || hasMandatForAngle(3))
    {
    	iColors[0] = 3 ; iColors[1] = 3 ; iColors[2] = 2 ; iColors[3] = 1 ;
    	push_back(new NSPetal(pContexte, 9, focusType, physicalLevel, iColors,prof_sant)) ;
        iColors[0] = 3 ; iColors[1] = 3 ; iColors[2] = 2 ; iColors[3] = 1 ;
    	push_back(new NSPetal(pContexte, 9, focusType, moralLevel, iColors,prof_sant)) ;
        if (bMax)
        // if (hasMandatForAngle(9))
        {
        	iColors[0] = 3 ; iColors[1] = 3 ; iColors[2] = 2 ; iColors[3] = 1 ;
        	push_back(new NSPetal(pContexte, 7, focusType, physicalLevel, iColors,prof_sant)) ;
            iColors[0] = 3 ; iColors[1] = 3 ; iColors[2] = 2 ; iColors[3] = 1 ;
    		push_back(new NSPetal(pContexte, 7, focusType, moralLevel, iColors,prof_sant)) ;
        }
    }
    else
    {
    	iColors[0] = 3 ; iColors[1] = 3 ; iColors[2] = 0 ; iColors[3] = 0 ;
    	push_back(new NSPetal(pContexte, 9, classicType, physicalLevel, iColors,prof_sant)) ;
        iColors[0] = 2 ; iColors[1] = 2 ; iColors[2] = 0 ; iColors[3] = 0 ;
    	push_back(new NSPetal(pContexte, 9, classicType, moralLevel, iColors,prof_sant)) ;
    }
    //
    // Professional (non health)
    //
    // Same view angle = 13 ; Social = 15 ; Admin = 17
    //
    std::string  soc = pContexte->getSuperviseur()->getText("rosaceManagement","angle15");
    std::string  admi = pContexte->getSuperviseur()->getText("rosaceManagement","angle17");
    if (bMax)
    // if (hasMandatForAngle(15) || hasMandatForAngle(17))
    {
    	iColors[0] = 2 ; iColors[1] = 2 ; iColors[2] = 2 ; iColors[3] = 2 ;
    	push_back(new NSPetal(pContexte, 13, classicType, physicalLevel, iColors,"angle21")) ;
        iColors[0] = 2 ; iColors[1] = 2 ; iColors[2] = 2 ; iColors[3] = 2 ;
    	push_back(new NSPetal(pContexte, 13, classicType, moralLevel, iColors,"angle21")) ;
    }
    iColors[0] = 2 ; iColors[1] = 2 ; iColors[2] = 2 ; iColors[3] = 1 ;
    push_back(new NSPetal(pContexte, 15, classicType, physicalLevel, iColors,soc)) ;
    iColors[0] = 2 ; iColors[1] = 2 ; iColors[2] = 2 ; iColors[3] = 1 ;
    push_back(new NSPetal(pContexte, 15, classicType, moralLevel, iColors,soc)) ;
    iColors[0] = 2 ; iColors[1] = 2 ; iColors[2] = 2 ; iColors[3] = 1 ;
    push_back(new NSPetal(pContexte, 17, classicType, physicalLevel, iColors,admi)) ;
    iColors[0] = 2 ; iColors[1] = 2 ; iColors[2] = 2 ; iColors[3] = 1 ;
    push_back(new NSPetal(pContexte, 17, classicType, moralLevel, iColors,admi)) ;
    //
	// Familly
    //
    std::string  fam_lab = pContexte->getSuperviseur()->getText("rosaceManagement","angle21");
    iColors[0] = 2 ; iColors[1] = 1 ; iColors[2] = 1 ; iColors[3] = 1 ;
    push_back(new NSPetal(pContexte, 21, classicType, physicalLevel, iColors,fam_lab)) ;
    //
	// Patient
    //
     std::string  fam_pat = pContexte->getSuperviseur()->getText("rosaceManagement","angle0");
    iColors[0] = 1 ; iColors[1] = 1 ; iColors[2] = 1 ; iColors[3] = 1 ;
    push_back(new NSPetal(pContexte, 0, classicType, physicalLevel, iColors,fam_pat)) ;

    return true ;
*/
}
catch (...)
{
	erreur("Exception NSRosace::createPetals", standardError, 0) ;
  return false ;
}
}

bool
NSRosace::updateRightsString()
{
	return false ;
}

string
NSRosace::getRightsString()
{
  updateRightsString() ;
  return sRightsAsString ;
}

bool
NSRosace::setRightsString(string sNewRights)
{
  sRightsAsString = sNewRights ;
  if (createPetals())
    return true ;

  sRightsAsString = string("") ;
  return false ;
}

void
NSRosace::calculateParameters(NS_CLASSLIB::TRect* pContainerRect)
{
	if ((NS_CLASSLIB::TRect*) NULL == pContainerRect)
    return ;

  // Normalizing
  //
  int iHeigth = min(abs(pContainerRect->Height()), abs(pContainerRect->Width())) ;
  iRadius	= ( iHeigth / 2 ) - 1 ;

  ptCenter.x = pContainerRect->Left() + iRadius + 1 ;
  ptCenter.y = pContainerRect->Top() + iRadius + 1 ;

  // Real size rosace occupation
  iRadius = (iRadius * OccupationRosacePercent) / 100 ;
}

/*
void
NSRosace::calculateParametersForPoint(NS_CLASSLIB::TPoint* pPoint, int* piRadius, int* iDegreAngle)
{
	double dDeltaX = double( pPoint->X() - ptCenter.X() ) ;
    double dDeltaY = double( ptCenter.Y() - pPoint->Y() ) ;

    // Pythagore
    double dRadius 	= pow( pow(dDeltaX, 2) + pow(dDeltaY, 2) , 0.5 ) ;
    double dCosine 	= dDeltaX / dRadius ;
    double dSine 	= dDeltaY / dRadius ;

    double dCosRadAngle = acos(dCosine) ;	// result in range 0 to pi
    double dSinRadAngle = asin(dSine) ;		// result in range -pi/2 to pi/2

    double dPI = 3.14159265358979 ;

    if (dSinRadAngle < 0)
    	dCosRadAngle = ( 2 * dPI ) - dCosRadAngle ;

    *iDegreAngle	= int( dCosRadAngle * double(180) / dPI) ;
    *piRadius		= int( dRadius ) ;
} */

void
NSRosace::prepare(NS_CLASSLIB::TRect* pContainerRect)
{
	if ((NS_CLASSLIB::TRect*) NULL == pContainerRect)
    return ;

  if (empty())
    return ;

  calculateParameters(pContainerRect) ;

  for (NSPetalsIter petalIt = begin() ; petalIt != end() ; petalIt++)
    (*petalIt)->prepareRights(&ptCenter, iRadius, 20) ;

  //
  // Sorting the rosace : angle + level
  //
  sort(begin(), end(), infPetal) ;
}

void
NSRosace::draw(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
try
{
	if ((TDC*) NULL == pDc)
		return ;

	bNeedPainting = false ;

	if (empty())
		return ;

	int iPreviousDC = pDc->SaveDC() ;
	if (iPreviousDC == 0)
	{
		erreur("Exception NSRosace::draw", standardError, 0) ;
		return ;
	}

	TBrush* brushes[4] ;
	brushes[0] = new TBrush(NS_CLASSLIB::TColor::White) ;
	brushes[1] = new TBrush(NS_CLASSLIB::TColor::LtRed);
	brushes[2] = new TBrush(NS_CLASSLIB::TColor::LtYellow);
	brushes[3] = new TBrush(NS_CLASSLIB::TColor::LtGreen) ;

	pDc->FillRect(RectAPeindre, *(brushes[0])) ;

	TPen thePen(NS_CLASSLIB::TColor::Black, 1) ;
	pDc->SelectObject(thePen) ;

	for (int i = 0 ; i < MAXRIGHTSZORDER ; i++)
		for (NSPetalsIter petalIt = begin() ; petalIt != end() ; petalIt++)
			(*petalIt)->draw(pDc, erase, RectAPeindre, i, brushes, pSelectedRP) ;

	for (int i = 0 ; i < 4 ; i++)
		delete brushes[i] ;

	string sTextToDraw = pContexte->getSuperviseur()->getText("HealthTeamManagement", "medicalDoctor") ;
	drawCircleTexte(*pDc, sTextToDraw, ptCenter, iRadius +14, 94, 80, RectAPeindre) ;
  sTextToDraw = pContexte->getSuperviseur()->getText("HealthTeamManagement", "healthProfessionals") ;
	drawCircleTexte(*pDc, sTextToDraw, ptCenter , iRadius + 14, 0, 88, RectAPeindre) ;
  sTextToDraw = pContexte->getSuperviseur()->getText("HealthTeamManagement", "familly") ;
	drawCircleTexte(*pDc, sTextToDraw, ptCenter, iRadius +14, 196, 50, RectAPeindre) ;
  sTextToDraw = pContexte->getSuperviseur()->getText("HealthTeamManagement", "administration") ;
	drawCircleTexte(*pDc, sTextToDraw, ptCenter, iRadius +14, 250, 60, RectAPeindre) ;
  sTextToDraw = pContexte->getSuperviseur()->getText("HealthTeamManagement", "socialWorkers") ;
	drawCircleTexte(*pDc, sTextToDraw, ptCenter, iRadius+14, 310, 43, RectAPeindre) ;

	pDc->RestoreDC(iPreviousDC) ;
}
catch (...)
{
	erreur("Exception NSRosace::draw", standardError, 0) ;
}
}

string
NSRosace::buildRosaceString()
{
  if (empty())
    return string("") ;

  // entête : version 1
  //
  string sRights = "" ; //"\x01" ;
  int header = 0x10 ;
  header  += (_synchro == true) ? 0x08 : 0;
  header  += (_personnal_note == true) ?  0x04 : 0;
  sRights += string(1, char(header)) ;

  string sAngleRights = "" ;

  int		iPreviousAngle 		= -1 ;
  char 	cAngleParameters	= 0x00 ;

  // Assuming petals are sorted (angle + level)
  //
  for (NSPetalsIter petalIt = begin() ; petalIt != end() ; petalIt++)
  {
    int iPetalAngle = (*petalIt)->getAngle() ;
    //
    // Closing the previous angle petal(s)'s rights
    //
    if ((iPreviousAngle != -1) && (iPetalAngle != iPreviousAngle))
    {
      if (sAngleRights != "")
      {
        size_t iLength = strlen(sAngleRights.c_str()) ;

        sAngleRights =	string(1, char(iPreviousAngle + 0x20)) +
                				string(1, char(iLength + 2 + 0x30)) +
                                string(1, cAngleParameters) +
                				sAngleRights +
                                string(1, char(GROUPEND)) ;

        sRights += sAngleRights ;
			}
      sAngleRights 		= "" ;
      cAngleParameters	= 0x00 ;
    }
    iPreviousAngle = iPetalAngle ;

    sAngleRights += (*petalIt)->getRightsString() ;

    if 		((*petalIt)->getLevel() == physicalLevel)
      cAngleParameters |= PHYSICALACCESS ;
    else if ((*petalIt)->getLevel() == moralLevel)
      cAngleParameters |= MORALACCESS ;

    if 		((*petalIt)->getType() == classicType)
      cAngleParameters |= SIMPLEPETAL ;
    else if ((*petalIt)->getType() == focusType)
      cAngleParameters |= FOCUSPETAL ;
    else if ((*petalIt)->getType() == patientType)
      cAngleParameters |= PATIENTPETAL ;
  }

  if (sAngleRights != "")
  {
    size_t iLength = strlen(sAngleRights.c_str()) ;

    sAngleRights =	string(1, char(iPreviousAngle + 0x20)) +
                        string(1, char(iLength + 2 + 0x30)) +
                        string(1, cAngleParameters) +
                        sAngleRights +
                        string(1, char(GROUPEND)) ;

    sRights += sAngleRights ;
  }

  return sRights ;
}

void
NSRosace::resetRosace(string sRights)
{
	if (false == empty())
		vider() ;

	if (string("") == sRights)
		return ;

  string sLanguage = pContexte->getUserLanguage() ;

	size_t 	iCursor = 0 ;
	size_t 	iRigLen = strlen(sRights.c_str()) ;

	//
	// Gestion de l'entête
	//
  char entete = sRights[iCursor] ;

  _synchro        = ((entete & 0x08) == 0x08) ;
  _personnal_note = ((entete & 0x04) == 0x04) ;

 /*	if (sRights[iCursor] != '\x01')
		return ;    */

	iCursor++ ;

	// Gestion des pétales et infos
	//
	while (iCursor < iRigLen)
	{
		// Petal
		//
		unsigned char cHeader = (unsigned char) sRights[iCursor] ;

		iCursor++ ;
		if (iCursor >= iRigLen)
			return ;

    // Group length
    //
		size_t iPetalLength = sRights[iCursor] ;
    if (iPetalLength < 0x30)
      return ;
    iPetalLength -= 0x30 ;

		if (iCursor + iPetalLength >= iRigLen)
			return ;
		if (sRights[iCursor + iPetalLength] != char(GROUPEND))
			return ;

		iCursor++ ;

		if (cHeader < 0x40)
		{
			// Angle value
			//
			int iPetalAngle = cHeader - 0x20 ;

			// Petal(s) type
			//
			char cPetalType = sRights[iCursor] ;

			int iComplexity = cPetalType ; // cPetalType & MASK2GROUP ;
			PetalType iType = PTnotDef ;
			if 			((iComplexity & MASK4START) == PATIENTPETAL)
				iType = patientType ;
			else if ((iComplexity & MASK4START) == FOCUSPETAL)
				iType = focusType ;
			else if ((iComplexity & MASK4START) == SIMPLEPETAL)
				iType = classicType ;

			if (iType == PTnotDef)
				return ;

			int iAccessType = cPetalType & MASK4GROUP ;

			bool bExistPhy	= ((iAccessType & PHYSICALACCESS) == PHYSICALACCESS) ;
			bool bExistMoral	= ((iAccessType & MORALACCESS) == MORALACCESS) ;
			if (bExistPhy && bExistMoral && (iPetalLength < 4))
				return ;

      iCursor++ ;

      bool bRightsForPersonFound = false ;
      bool bRightsForDomainFound = false ;

      int iColorsForPerson[4] ;
      int iColorsForDomain[4] ;

      //
      //
      colorsFromChar('\0', iColorsForPerson) ;
      colorsFromChar('\0', iColorsForDomain) ;

      // Petals
      //
      size_t iPetalNb = 1 ;
      while ((iPetalNb < iPetalLength-1) && (iCursor < strlen(sRights.c_str())))
      {
        // Domain
        //
        if ((sRights[iCursor] & 0x20) == 0x20)
        {
          if (colorsFromChar(sRights[iCursor], iColorsForDomain))
            bRightsForDomainFound = true ;
        }
        // Person
        //
        else
        {
          if (colorsFromChar(sRights[iCursor], iColorsForPerson))
            bRightsForPersonFound = true ;
        }
        iPetalNb++ ;
        iCursor++ ;
      }

			if (bRightsForPersonFound)
				push_back(new NSPetal(pContexte, iPetalAngle, iType, physicalLevel, iColorsForPerson, "")) ;

			if (bRightsForDomainFound)
        push_back(new NSPetal(pContexte, iPetalAngle, iType, moralLevel, iColorsForDomain, "")) ;

			if (sRights[iCursor] != char(GROUPEND))
				return ;
			iCursor++ ;
		}
    else
    {
    	string sText = string(sRights, iCursor, iPetalLength-1) ;
      iCursor += iPetalLength ;

    	switch(cHeader)
      {
      	case 'M' :
        	sReferenceJob = sText ;
          pContexte->getDico()->donneLibelle(sLanguage, &sReferenceJob, &sReferenceJobLib) ;
          break ;
        case 'S' :
        	sReferenceSpeciality = sText ;
          pContexte->getDico()->donneLibelle(sLanguage, &sReferenceSpeciality, &sReferenceSpecialityLib) ;
          break ;
      }
    }
	}
}

bool
NSRosace::colorsFromChar(char cPetalColors, int iRights[])
{
  // reset
  //
  if ('\0' == cPetalColors)
  {
    for (int i = 0 ; i < 4 ; i++)
      iRights[i] = 0 ;

    return true ;
  }

  // Define if it is "same specialty petal" or "another specialty petal"
  //

  // Remember:
  // - iRights[0] : r = 1 ; same specialty
  // - iRights[1] : r = 1 ; other specialty
  // - iRights[2] : r = 2 ; same specialty
  // - iRights[3] : r = 2 ; other specialty

  // Other specialty
  //
  if ((cPetalColors & 0x10) == 0x10)
  {
    iRights[1] = (cPetalColors & MASK3GROUP) >> 2 ;  
    iRights[3] = cPetalColors & MASK4GROUP ;
  }
  // Same specialty
  //
  else
  {
    iRights[0] = (cPetalColors & MASK3GROUP) >> 2 ;
    iRights[2] = cPetalColors & MASK4GROUP ;
  }

  return true ;
}

//le decodage de la petale famille
void
NSRosace::decodePetalFamily(char /* sDecodingChar */, NSPetal* /* pPetalRight */)
{
/*
  NSRightPosition	*pRightPos = new NSRightPosition() ;
  //la famille ocupe les dernieres 4 bits : 2 pour chaque distance

  int iColor  = -1 ;
  iColor = (sDecodingChar & MASK3GROUP) ;       //proche
  if ( iColor >= 0)   //0 est sans couleur (erreur)
  {
    pRightPos->setFonction(family) ;
    pRightPos->setDistance(nearDist) ;
    pRightPos->setAccess(nominative) ;
    pRightPos->setColor(iColor) ;
    pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;
  }

  iColor = (sDecodingChar & MASK4GROUP) ;       //lointaine
  if ( iColor >= 0)   //0 est sans couleur (erreur)
  {
    pRightPos->setFonction(family) ;
    pRightPos->setDistance(farDist) ;
    pRightPos->setAccess(nominative) ;
    pRightPos->setColor(iColor) ;
    pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;
  }
  delete pRightPos ;
*/
}


void
NSRosace::setProperty(FonctionType /* fonction */, SpecialityType /* spec */, AccessType /* acces */, DistanceType /* dist */, int /* color */, NSRightPosition* /* pRightPos */)
{
/*
  pRightPos->setFonction(fonction) ;
  pRightPos->setDistance(dist) ;
  pRightPos->setSpeciality(spec) ;
  pRightPos->setAccess(acces) ;
  pRightPos->setColor(color) ;
*/
}


// le decodage des pétales medecin, pers. de santé et pers. social
void
NSRosace::decodePetal(char /* sDecodingChar */, int /* iPosPetale */, NSPetal* /* pPetalRight */)
{
/*
	NSRightPosition *pRightPos = new NSRightPosition() ;

  // --000000 : même spécialté, distance proche
  // 00--0000 : même spécialité distance éloignée
  // 0000--00 : autre spécialité distance proche
  // 000000-- : autre spécialité distance éloignée

  switch (iPosPetale)
  {
  	case 2	:	// médecin - accès nominatif (personne physique)
            	setProperty(medecin, 					sameSpec, nominative, nearDist,	((sDecodingChar & MASK1GROUP)	>> 6),	pRightPos) ;
            	pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(medecin, 					sameSpec,	nominative, farDist, 	((sDecodingChar & MASK2GROUP) >> 4),	pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(medecin, 					difSpec, 	nominative, nearDist, ((sDecodingChar & MASK3GROUP)	>> 2),	pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(medecin, 					difSpec, 	nominative, farDist,	(sDecodingChar & MASK4GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              break ;

    case 3	: // médecin - accès par domaine (personne morale)
              setProperty(medecin, 					sameSpec,	domain, 		nearDist,	((sDecodingChar & MASK1GROUP)	>> 6),	pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(medecin, 					sameSpec,	domain, 		farDist, 	((sDecodingChar & MASK2GROUP)	>> 4), 	pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(medecin, 					difSpec,	domain, 		nearDist, ((sDecodingChar & MASK3GROUP)	>> 2), 	pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(medecin, 					difSpec,	domain,			farDist, 	(sDecodingChar & MASK4GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              break ;

    case 4	: // personnel de santé - accés nominatif (personne physique)
              setProperty(medPers, 					sameSpec, nominative, nearDist, (sDecodingChar & MASK1GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(medPers, 					sameSpec, nominative, farDist, 	(sDecodingChar & MASK2GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(medPers, 					difSpec, 	nominative, nearDist, (sDecodingChar & MASK3GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(medPers, 					difSpec, 	nominative, farDist, 	(sDecodingChar & MASK4GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              break ;

    case 5	: // personnel de santé - accés par domaine (personne morale)
              setProperty(medPers, 					sameSpec, domain, 		nearDist, (sDecodingChar & MASK1GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(medPers, 					sameSpec, domain, 		farDist,	(sDecodingChar & MASK2GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(medPers, 					difSpec, 	domain, 		nearDist, (sDecodingChar & MASK3GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(medPers, 					difSpec, 	domain, 		farDist,	(sDecodingChar & MASK4GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              break ;

    case 6	: // personnel social - accès nominatif (personne physique)
              setProperty(socPers, 					sameSpec,	nominative, nearDist,	(sDecodingChar & MASK1GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(socPers, 					sameSpec,	nominative, farDist, 	(sDecodingChar & MASK2GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

          		//setProperty(socPers, 					difSpec, 	nominative, nearDist, (sDecodingChar & MASK3GROUP), 				pRightPos) ;
            	//pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

            	//setProperty(socPers, 					difSpec, 	nominative, farDist, 	(sDecodingChar & MASK4GROUP), 				pRightPos) ;
            	//pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

            break;

    case 7	:	// personnel social - accès par domaine (personne morale)
              setProperty(socPers, 					sameSpec, domain, 		nearDist, (sDecodingChar & MASK1GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

              setProperty(socPers, 					sameSpec, domain, 		farDist, 	(sDecodingChar & MASK2GROUP), 				pRightPos) ;
              pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

							//setProperty(socPers, 					difSpec, 	domain, 		nearDist, (sDecodingChar & MASK3GROUP), 				pRightPos) ;
            	//pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

            	//setProperty(socPers, 					difSpec, 	domain, 		farDist, 	(sDecodingChar & MASK4GROUP), 				pRightPos) ;
            	//pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

            	break ;

    // case 8 et 9. La petale qui est entre PS et personnel social
    // personnel de santé ou pesonnel social pour traiter. La prise en compte du métier
    case 8	:	// personnel de santé ou personnel social (métier différent de l'auteur) - accès nominatif (personne physique)
            	setProperty(persSameFonction, sameSpec, nominative, nearDist,	(sDecodingChar & MASK1GROUP),					pRightPos) ;
            	pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

            	setProperty(persSameFonction, sameSpec, nominative, farDist, 	(sDecodingChar & MASK2GROUP), 				pRightPos) ;
            	pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

            	setProperty(persSameFonction, difSpec, nominative, 	nearDist, (sDecodingChar & MASK3GROUP), 				pRightPos) ;
            	pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

            	setProperty(persSameFonction, difSpec, nominative, 	farDist, 	(sDecodingChar & MASK4GROUP), 				pRightPos) ;
            	pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

            	break ;

    case 9	:	// personnel de santé ou personnel social (métier différent de l'auteur) - accès par domaine (personne morale)
							setProperty(persSameFonction,	sameSpec, domain, 		nearDist, (sDecodingChar & MASK1GROUP), 				pRightPos) ;
            	pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

            	setProperty(persSameFonction, sameSpec, domain, 		farDist, 	(sDecodingChar & MASK2GROUP), 				pRightPos) ;
            	pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

            	setProperty(persSameFonction, difSpec, domain, 			nearDist, (sDecodingChar & MASK3GROUP), 				pRightPos) ;
            	pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

            	setProperty(persSameFonction, difSpec, domain, 			farDist, 	(sDecodingChar & MASK4GROUP), 				pRightPos) ;
            	pPetalRight->push_back(new NSRightPosition(*pRightPos)) ;

            	break ;

  }
  delete pRightPos ;
*/
}


// -----------------------------------------------------------------------------
// dans le code qu'on recuppère on a :
//   * de 0-9 les droits d'accés
//   * 2 codes lexique : code métier + code specialité de l'utilisateur qui a
//     construit la rosace
// -----------------------------------------------------------------------------
void
NSRosace::translateRights(char* /* sCodRight */, NSPetal* /* pRights */)
{
/*
  // le 1er char contient : 00--0000 -> version
  //                       	0000000- -> type utilisateur: pour afficher la 3eme petale qui se trouve entre Pers. Santé et Pers. Soc.
  bProfNonMedecin =  sCodRight[0] & (0x01) ;
  iVersion 				= (sCodRight[0] & MASK4START) >> 4 ;

  iIsExtended 		= (sCodRight[0] & MASK3GROUP) >> 2 ;

  // le patient est dans les premiers 4 bits
  // on utilise seulement les 2 premiers, ils en restent 2 libres
  NSRightPosition *pRightPos = new NSRightPosition() ;

  int iPat 				= (sCodRight[1] & MASK1GROUP) >> 6 ;
  pRightPos->setDistance(patDist) ;
  pRightPos->setColor(iPat) ;

  pRights->push_back(new NSRightPosition(*pRightPos)) ;
  delete pRightPos ;

  // decodage de la petale famille
  decodePetalFamily(sCodRight[1], pRights) ;
  int len = 0 ;
  if (iIsExtended > 0)
  	len = 9 ;
  else
  	len = 7 ;
  for (int i = 2 ; i < len ; i++)        // 0-9 caracters pour les droits
  	decodePetal(sCodRight[i], i, pRights) ;
*/
}


char
*NSRosace::codifRights(NSPetal* /* pRight */)
{
	return "" ;
/*
	char 	*sCode 		= new char[10] ;
  int 	iCharNo 	= 0 ;
  char 	charCode 	= 0x00 ;

  // ajout version
  charCode = (char)(charCode | iVersion) ;

  //ajout type user
  if (bProfNonMedecin)
  	charCode = (char)(charCode | 0x01) ;

  sCode[0] = charCode ;
  iCharNo++ ;

  char bitColor[4];
  for (int i = 0 ; i > 4 ; i++)
  	bitColor[i] = 0x00 ;

  int 	iColor = -1 ;
  bool	bExtended = false ;

  NSRightsIter iter ;
  for (iter = pRight->begin() ; iter != pRight->end() ; iter++)
	{
    charCode = 0x00 ;
    int 					iCount = 0 ;
    FonctionType	fonction = (*iter)->getFonction() ;
    switch (fonction)
    {
    	case patient					: iColor = (*iter)->getColor() ;
                              if (iColor == 0)
                              {
                                erreur("erreur sur le droit d'acces du patient", standardError, 0, 0) ;
                                return "" ;
                              }
                             	bitColor[0] = (char)(iColor << 6) ;
                             	iCount = 1 ;
                             	break ;

      case family 					:	iColor = (*iter)->getColor() ;
                							if (iColor == 0)
                              {
                                erreur("erreur sur le droit d'acces de la famille", standardError, 0, 0) ;
                                return "" ;
                              }

                              if ((*iter)->getDistance() == nearDist)		// occupe les bits 4 et 5
                              	bitColor[2] = char(iColor << 2) ;
                              else                                    	// occupe les bits 6 et 7
                              {
                              	bitColor[3] = char(iColor) ;
                                iCount = 3 ;
                              }
                              break ;

      case medecin 					: // 2eme et 3eme char
                              iColor = (*iter)->getColor() ;
                              if ( iColor == 0)
                              {
                                erreur("erreur sur le droit d'acces des medecins", standardError, 0, 0) ;
                                return "" ;
                              }
                              if ((*iter)->getSpeciality() == sameSpec)
                              {
                                if ((*iter)->getDistance() == nearDist)
                                {
                                  bitColor[iCount] = (char)(iColor << 6) ;
                                  iCount++ ;
                                }
                                else
                                {
                                  bitColor[iCount] = (char)(iColor << 4) ;
                                  iCount++ ;
                                }
                              }
                              else
                              {
                                if ((*iter)->getDistance() == farDist)
                                {
                                  bitColor[iCount] = (char)(iColor << 2) ;
                                  iCount++ ;
                                }
                                else
                                {
                                  bitColor[2] = (char)(iColor) ;
                                  iCount++ ;
                                }
                              }
                              break ;

      case medPers 					: // occupe les bits 0 et 1 - 4eme et 5eme char
                              iColor = (*iter)->getColor() ;
                              if (iColor == 0)
                              {
                                erreur("erreur sur le droit d'acces pour le personnel santé", standardError, 0, 0) ;
                                return "" ;
                              }
                              if ((*iter)->getSpeciality() == sameSpec)
                              {
                                if ((*iter)->getDistance() == nearDist)
                                {
                                  bitColor[iCount] = (char)(iColor << 6) ;
                                  iCount++ ;
                                }
                                else
                                {
                                  bitColor[iCount] = (char)(iColor << 4) ;
                                  iCount++ ;
                                }
                              }
                              else
                              {
                                if ((*iter)->getDistance() == farDist)
                                {
                                  bitColor[iCount] = (char)(iColor << 2) ;
                                  iCount++ ;
                                }
                                else
                                {
                                  bitColor[2] = (char)(iColor) ;
                                  iCount++ ;
                                }
                              }
                              break ;

      case socPers 					: // occupe les bits 0 et 1 - 6eme et 7eme char
                              iColor = (*iter)->getColor() ;
                              if (iColor == 0)
                              {
                                erreur("erreur sur le droit d'acces pour les acteurs sociaux", standardError, 0, 0) ;
                                return "" ;
                              }

                              if ((*iter)->getDistance() == nearDist)    	// occupe le bit 0 et 1
                              	bitColor[0] = (char)(iColor << 6) ;
                              else                                    		// occupe le bit 2 et 3
                              {
                                bitColor[4] = (char)(iColor) ;
                                iCount = 3 ;
                              }
                              break ;

      case persSameFonction	:	// occupe les bits 0 et 1 - 8eme et 9eme char
                              iColor = (*iter)->getColor() ;
                              if (iColor == 0)
                              {
                                erreur("erreur sur le droit d'acces pour le personnel santé", standardError, 0, 0) ;
                                return "" ;
                              }
                              if ((*iter)->getSpeciality() == sameSpec)
                              {
                                if ((*iter)->getDistance() == nearDist)
                                {
                                  bitColor[iCount] = (char)(iColor << 6) ;
                                  iCount++ ;
                                }
                                else
                                {
                                  bitColor[iCount] = (char)(iColor << 4) ;
                                  iCount++ ;
                                }
                              }
                              else
                              {
                                if ((*iter)->getDistance() == farDist)
                                {
                                  bitColor[iCount] = (char)(iColor << 2) ;
                                  iCount++ ;
                                }
                                else
                                {
                                  bitColor[2] = (char)(iColor) ;
                                  iCount++ ;
                                }
                              }
                              bExtended = true ;
                              break ;
    }

    if (iCount == 3)
    {
    	sCode[iCharNo] = (char)(bitColor[0] | bitColor[1] | bitColor[2] | bitColor[3]) ;
      iCharNo++ ;

      for (int i = 0 ; i > 4 ; i++)
      	bitColor[i] = 0x00 ;

      iCount == 0 ;
    }
	}

  if (bExtended)
  	sCode[0] = (char)(sCode[0] | MASK3GROUP) ;

  return sCode ;
*/
}


int
NSRosace::getRightsPetal(string sUserSpec, string sCreatorSpec, DistanceType dist, char sCodePetal)
{
	if (sUserSpec == "")
	{
  	if (dist == nearDist)
    	return (sCodePetal & MASK1GROUP) >> 6 ;
    else
    	return (sCodePetal & MASK2GROUP) >> 4 ;
	}

  if (sUserSpec == sCreatorSpec)
	{
  	if (dist == nearDist)
    	return (sCodePetal & MASK1GROUP) >> 6 ;
    else
    	return (sCodePetal & MASK2GROUP) >> 4 ;
	}
  else
	{
  	if (dist == farDist)
    	return (sCodePetal & MASK3GROUP) >> 2 ;
    else
    	return (sCodePetal & MASK4GROUP) ;
	}
}


int
NSRosace::getRightsForUser(DistanceType dist, AccessType acces, QuarterType quarter, string sFonction, string sSpeciality, char *sCodeRights)
{
	string 	sFonctLexic 			= "" ;
  	string 	sSpecialityLexic	= "" ;
  	char 		*sRights 					= new char[10] ;
  	for (int i = 0 ; i < 10 ; i++)
  		sRights[i] = sCodeRights[i] ;
  	iIsExtended = ((char)(sRights[0]) & MASK3GROUP) >> 2 ;

  	for (int i = 0 ; i < 5 ; i++)
  		sFonctLexic[i] = sCodeRights[9 + i] ;
  	sFonctLexic[6] = '\0' ;

  	for (int i = 0 ; i < 5 ; i++)
  		sSpecialityLexic[i] = sCodeRights[14 + i] ;
  	sSpecialityLexic[6] = '\0' ;
  	if (dist == patDist)
  		return (sRights[0] & MASK1GROUP) >> 6 ;
  	int iColor = -1 ;

  	switch (quarter)
  	{
    	case medecins 			:
        	if (acces == nominative)
            	iColor = getRightsPetal(sSpeciality, sSpecialityLexic, dist, sRights[2]) ;
            else
            	iColor = getRightsPetal(sSpeciality, sSpecialityLexic, dist, sRights[3]) ;
            break ;

    	case medicalPersons	:
    		if ((sFonctLexic == sFonction) && (iIsExtended ==1))
            {
            	if (acces == nominative)
                	iColor = getRightsPetal(sSpeciality, sSpecialityLexic, dist, sRights[4]) ;
                else
                	iColor = getRightsPetal(sSpeciality, sSpecialityLexic, dist, sRights[5]) ;
            }
            else
            {
            	if (acces == nominative)
                	iColor = getRightsPetal(sSpeciality, sSpecialityLexic, dist, sRights[8]) ;
                else
                	iColor = getRightsPetal(sSpeciality, sSpecialityLexic, dist, sRights[9]) ;
            }
        	break ;

    case socialPersons	:	if ((sFonctLexic == sFonction) && (iIsExtended == 1))
    											{
            								if (acces == nominative)
                							iColor = getRightsPetal(sSpeciality, sSpecialityLexic, dist, sRights[6]) ;
            								else
                							iColor = getRightsPetal(sSpeciality, sSpecialityLexic, dist, sRights[7]) ;
													}
        									else
													{
                          	if (acces == nominative)
                          		iColor = getRightsPetal(sSpeciality, sSpecialityLexic, dist, sRights[8]) ;
                          	else
                          		iColor = getRightsPetal(sSpeciality, sSpecialityLexic, dist, sRights[9]) ;
													}
                      		break ;
    case familyPersons  :	if (dist == nearDist)
            								iColor =  (sRights[1] & MASK1GROUP) >> 6 ;
        									else
            								iColor = (sRights[1] & MASK2GROUP) >> 4 ;
    											break ;
  }
  return iColor ;
}

void
NSRosace::vider()
{
	if (empty())
		return ;

	for (NSPetalsIter i = begin() ; i != end() ; )
	{
		delete *i ;
		erase(i) ;
	}
}

//
// Gives the position of the point pPoint in the rosace
//
NSRightPosition*
NSRosace::getRightFromPoint(NS_CLASSLIB::TPoint* pPoint)
{
	//
	// Radius and Angle of the point
  //
  int iPointRadius = 0 ;
  int iPointDegreAngle = 0 ;
  // calculateParametersForPoint(pPoint, &iPointRadius, &iPointDegreAngle) ;ptCenter
  calculateParametersForPoint(*pPoint,ptCenter, &iPointRadius, &iPointDegreAngle) ;
  return getRightFromPoint(iPointRadius, iPointDegreAngle) ;
}

//
// Gives the position of the point(iRadius, iDegreAngle) in the rosace
//
NSRightPosition*
NSRosace::getRightFromPoint(int iRadius, int iDegreAngle)
{
	NSRightPosition* pClickedRight = 0 ;

	for (int i = MAXRIGHTSZORDER ; i >= 0 ; i--)
    for (NSPetalsIter petalIt = begin() ; petalIt != end() ; petalIt++)
      if ((*petalIt)->isBoxOnPoint(&pClickedRight, iRadius, iDegreAngle, i))
        return pClickedRight ;
        
	return NULL ;
}

void
NSRosace::EvLButtonDown(uint /* modKeys */, NS_CLASSLIB::TPoint& point, NS_CLASSLIB::TRect* /* pRosaceRect */)
{
	if (empty())
		return ;

  NSRightPosition* pSelPosit = getRightFromPoint(&point) ;

  if (NULL == pSelPosit)
    return ;

  if (pSelPosit == pSelectedRP)
  {
    pSelectedRP->setNextColor() ;
    bNeedPainting = true ;
  }
  else
  {
    pSelectedRP = pSelPosit ;
    bNeedPainting = true ;
  }
}

void
NSRosace::EvLButtonUp(uint /* modKeys */, NS_CLASSLIB::TPoint& /* point */, NS_CLASSLIB::TRect* /* pRosaceRect */)
{
}

NSRosace&
NSRosace::operator=(const NSRosace& rv)
{
try
{
  if (this == &rv)
    return *this ;

  if (false == rv.empty())
		for (NSPetalsConstIter it = rv.begin() ; rv.end() != it ; it++)
			push_back(*it) ;

  sRightsAsString = rv.sRightsAsString ;

  return *this ;
}
catch (...)
{
	erreur("Exception NSRosace = operator", standardError, 0) ;
  return *this ;
}
}

DEFINE_RESPONSE_TABLE1(NSRosaceGroupBox, TButton)
  EV_WM_PAINT,
  EV_WM_LBUTTONDOWN,
  EV_WM_MOUSEMOVE,
  EV_WM_LBUTTONDBLCLK,
  // EV_WM_TIMER,
  // EV_NOTIFY_AT_CHILD(WM_LBUTTONDBLCLK, ButtonDblClk),
  // EV_NOTIFY_AT_CHILD(WM_LBUTTONDOWN, ButtonDblClk),
END_RESPONSE_TABLE;

//// Constructeur
//
NSRosaceGroupBox::NSRosaceGroupBox(NSContexte *pCtx, TWindow* parent, int resId, NSRosace* pRose)
				 :TButton(parent, resId), NSRoot(pCtx)
{
  DisableTransfer() ;
  _pRosace = pRose ;

  // initialisation des tooltip
  _pToolTip = new NSTitleTip(this, pContexte) ;
  _pToolTip->Create() ;
  _pToolTip->Hide() ;
  _mouse_on = NULL ;
}

//
// Destructeur
//
NSRosaceGroupBox::~NSRosaceGroupBox()
{
 //	KillTimer(ROSACE_TIMER);
}

void
NSRosaceGroupBox::SetupWindow()
{
	TButton::SetupWindow() ;

  if (_pRosace)
  {
    NS_CLASSLIB::TRect rosaceRect = GetClientRect() ;
    _pRosace->prepare(&rosaceRect) ;
  }
 //  SetTimer(ROSACE_TIMER, 250);
}

string
NSRosaceGroupBox::getRigthsString()
{
	if ((NSRosace*) NULL == _pRosace)
    return string("") ;

	return _pRosace->buildRosaceString() ;
}

void
NSRosaceGroupBox::EvPaint()
{
  TPaintDC Dc(*this) ;
  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&Dc.Ps.rcPaint ;

  Paint(Dc, true, rect) ;

  DefaultProcessing() ;
}

void
NSRosaceGroupBox::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  TButton::Paint(dc, erase, RectAPeindre) ;

  if (_pRosace)
    _pRosace->draw(&dc, erase, RectAPeindre) ;
}voidNSRosaceGroupBox::EvMouseMove(uint /* modKeys */, NS_CLASSLIB::TPoint& point){  _pos_tip = point ;  _time    = 0 ;  NSRightPosition* temp = (NSRightPosition*) 0 ;  if (_pRosace)    temp = _pRosace->getRightFromPoint(&_pos_tip) ;  if (temp && _mouse_on && (*temp == *_mouse_on))    return ;  _mouse_on = temp ;  _pToolTip->Hide() ;  if (_mouse_on)
  {
    NS_CLASSLIB::TPoint haut(_pos_tip.x + 16, _pos_tip.y + 16) ;
    NS_CLASSLIB::TPoint bas(_pos_tip.x + 120, _pos_tip.y + 120) ;
    NS_CLASSLIB::TRect rectDoc(haut,bas) ;
    std::string toaff = _mouse_on->sPositionTitle ;
    _pToolTip->Show(rectDoc, &toaff, 0) ;  }  else    _pToolTip->Hide() ;}
voidNSRosaceGroupBox::EvLButtonDblClk(UINT /* modKeys */, NS_CLASSLIB::TPoint& /* point */)
{
	//
  //
  //
}

/*
void
NSRosaceGroupBox::EvTimer(uint timerId)
{
}   */

void
NSRosaceGroupBox::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	NS_CLASSLIB::TRect rosaceRect = GetClientRect() ;

  if (_pRosace)
  {
	  _pRosace->EvLButtonDown(modKeys, point, &rosaceRect) ;
    if (_pRosace->bNeedPainting)
      Invalidate() ;
  }
}

void
NSRosaceGroupBox::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	NS_CLASSLIB::TRect rosaceRect = GetClientRect() ;
  if (_pRosace)
	  _pRosace->EvLButtonUp(modKeys, point, &rosaceRect) ;
}

voidNSRosaceGroupBox::ButtonDblClk()
{
}

//***********************************************************************//
//							Classe RightsDialog
//***********************************************************************//
DEFINE_RESPONSE_TABLE1(RightsDialog, NSUtilDialog)
	EV_COMMAND(IDOK,	    CmOk),
	EV_COMMAND(IDCANCEL,	CmCancel),
END_RESPONSE_TABLE;

RightsDialog::RightsDialog(TWindow* parent, NSContexte* pCtx, string* pRightsStr)
             :NSUtilDialog(parent, pCtx, "RIGHTS_CHOICE", pNSResModule)
{
	_pRightsString = pRightsStr ;

  if (((NSContexte*) NULL == pCtx) || (NULL == pCtx->getPatient()))
    return ;

  NSHealthTeamMandate* pMdt = NULL ;

  NSHealthTeam* pHT = pCtx->getPatient()->getHealthTeam() ;
  if (pHT)
  {
    NSHealthTeamMember* pHTMember = pHT->getUserAsMember(pCtx) ;
    if (pHTMember)
    {
      NSHTMMandateArray aMandates ;
      pHTMember->getActiveMandates(&aMandates) ;
      if (false == aMandates.empty())
      {
        NSHTMMandateIter mandateIter = aMandates.begin() ;
        pMdt = *mandateIter ;
        //
        // We have to empty aMandates in order to avoid mandates deletion
        //
        for ( ; mandateIter != aMandates.end() ; )
          aMandates.erase(mandateIter) ;
      }
    }
  }

  if (string("") == *_pRightsString)
    _pRosace = new NSRosace(pCtx, pMdt) ;
  else
    _pRosace = new NSRosace(pCtx, *_pRightsString, pMdt) ;

	_pRightsInterface = new NSRosaceGroupBox(pCtx, this, IDC_ROSACE, _pRosace) ;
}

RightsDialog::~RightsDialog()
{
  delete _pRightsInterface ;
  delete _pRosace ;
}

void
RightsDialog::SetupWindow()
{
	TDialog::SetupWindow() ;
}

void
RightsDialog::CmOk()
{
  *_pRightsString = _pRightsInterface->getRigthsString() ;

	CloseWindow(IDOK) ;
}

void
RightsDialog::CmCancel()
{
  Destroy(IDCANCEL) ;
}

