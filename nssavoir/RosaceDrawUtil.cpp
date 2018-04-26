// -----------------------------------------------------------------------------
// RosaceDrawUtil.cpp
// -----------------------------------------------------------------------------
// Outils pour dessiner et gerer des rosaces
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// Author : Tabun
// $Date: 2009/07/04 09:41:02 $
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

#include "nssavoir\RosaceDrawUtil.h"

/*
extern "C"
{
	#include <math.h>
}
*/

#include <cmath>


// Fonction servant a transformer un point en coordonée polaire polaire
// NS_CLASSLIB::TPoint& pPoint : point a transformer en coordoné polaire
// NS_CLASSLIB::TPoint& center : Centre des coordonnées polaire
// int* piRadius : radius a remplir
// int* iDegreAngle : angle a remplir
//
void calculateParametersForPoint(NS_CLASSLIB::TPoint& pPoint,NS_CLASSLIB::TPoint& center, int* piRadius, int* iDegreAngle)
{
try
{
  double dDeltaX = double( pPoint.X() - center.X() ) ;
  double dDeltaY = double( center.Y() - pPoint.Y() ) ;

  // Pythagore
  double dRadius 	= pow( pow(dDeltaX, 2) + pow(dDeltaY, 2) , 0.5 ) ;
  if (0L == dRadius)
  {
    *iDegreAngle = 0 ;
    *piRadius    = 0 ;
    return ;
  }

  double dCosine = dDeltaX / dRadius ;
  double dSine   = dDeltaY / dRadius ;

  double dCosRadAngle = acos(dCosine) ;	// result in range 0 to pi
  double dSinRadAngle = asin(dSine) ;		// result in range -pi/2 to pi/2

  // double dPI = 3.14159265358979 ;
  double dPI = M_PI ; 

  if (dSinRadAngle < 0)
    dCosRadAngle = ( 2 * dPI ) - dCosRadAngle ;

  *iDegreAngle	= int( dCosRadAngle * double(180) / dPI) ;
  *piRadius		= int( dRadius ) ;
}
catch(...)
{
}
}

// Fonction servant a transformer un point exprimé en polaire en coordonée en cartesien
// int piRadius : radius du point a transformer
// int iDegreAngle : angle du point a transformer
// NS_CLASSLIB::TPoint& center : centre des coordonnées polaire
// NS_CLASSLIB::TPoint& pPoint : Point resultant de l'operation
//
void radiusTocar(int piRadius, int iDegreAngle,NS_CLASSLIB::TPoint& center, NS_CLASSLIB::TPoint& pPoint)
{
	try
  {
    // double dPI = 3.14159265358979 ;
    double dPI = M_PI ;

    double iradeAngle	=  2 * dPI - ( ((iDegreAngle) * dPI) / double(180));
    pPoint.x = center.x + (int)(cos((double)iradeAngle)* piRadius);
    pPoint.y = center.y + (int)(sin((double)iradeAngle)* piRadius);
  }
  catch(...)
  {
  }
}


// Dessine un texte sur un cercle
// HDC 							: Device graphique sur lequel ecrire
// char* texte  		: texte à afficher en cercle
// NS_CLASSLIB 			: TPoint& center point representant le centre du cercle
// int radius 			: A quel distance du texte commencer l'ecriture
// int start_angle  : Angle ou on doit commencer l'ecriture (en degrés)
// int val_angle		: valeur de l'angle (en degrés)
//
void drawCircleTexte(HDC hdc, std::string to_aff, NS_CLASSLIB::TPoint& center, int radius, int start_angle, int val_angle, NS_CLASSLIB::TRect& RectAPeindre )
{
try
{
  bool bIncrease = false ;     // sert a savoir si on n'est pas trop serré
  int nb_char = strlen(to_aff.c_str()) ;
  if (0 == nb_char)
    return ;

  HFONT hfnt, hfntPrev;

  PLOGFONT plf = (PLOGFONT) LocalAlloc(LPTR, sizeof(LOGFONT)) ;
  // lstrcpy(plf->lfFaceName, "Arial");
  lstrcpy(plf->lfFaceName, "Courier New");
  plf->lfWeight        =/* FW_NORMAL*/ FW_LIGHT 	;
  plf->lfOutPrecision  = OUT_TT_PRECIS;
  plf->lfQuality       = PROOF_QUALITY	;
  plf->lfClipPrecision = CLIP_LH_ANGLES;

  SetBkMode(hdc, TRANSPARENT) ;

  // Get the angle per character value
  //
  double inc = (double) val_angle / (double) nb_char ;
  if (inc > 4)
  {
    bIncrease = true ;
    nb_char++ ;
    inc = (double) val_angle / (double) nb_char ;
  }

 // le ratio est trop grand on reduit et on recentre le text
/* double rpecent = val_angle / 10;
 int i = 0;
 while (inc > 7)
 {
    val_angle = val_angle -  rpecent;
    inc = ( ((double)val_angle/ (double)nb_char) );
 }

 start_angle =  start_angle + (i /2)* rpecent;
 */

  // Upper part of the circle
  //
  if (start_angle + val_angle < 180)
  {
    double anglet = start_angle + val_angle ;
    for (int i = 0 ; i < nb_char ; i++)
    {
      if (true == bIncrease)
        anglet -= inc ;
      plf->lfEscapement = ceil(((anglet - 90) * 10)) ;
      hfnt = CreateFontIndirect(plf) ;
      hfntPrev = (HFONT)SelectObject(hdc, hfnt) ;
      NS_CLASSLIB::TPoint pPoint(0, 0) ;
      radiusTocar(radius + 8, anglet, center, pPoint) ;
      LPSTR temp = " " ;
      *temp = to_aff[i] ;
      TextOut(hdc, pPoint.x, pPoint.y, temp, lstrlen(temp)) ;
      SelectObject(hdc, hfntPrev) ;
      DeleteObject(hfnt) ;
      if (false == bIncrease)
        anglet -= inc ;
    }
  }
  // Lower part of the circle
  //
  else
  {
    int anglet = start_angle ;
    for (int i = 0 ; i < nb_char ; i++)
    {
      if (true == bIncrease)
        anglet += inc ;
      plf->lfEscapement = ceil((anglet - 270 ) * 10) ;
      hfnt = CreateFontIndirect(plf) ;
      hfntPrev = (HFONT)SelectObject(hdc, hfnt) ;
      NS_CLASSLIB::TPoint pPoint(0, 0) ;
      radiusTocar(radius - 19, anglet, center, pPoint) ;
      LPSTR temp = " " ;
      *temp = to_aff[i] ;
      TextOut(hdc, pPoint.x, pPoint.y, temp, lstrlen(temp)) ;
      SelectObject(hdc, hfntPrev) ;
      DeleteObject(hfnt) ;
      if (false == bIncrease)
        anglet += inc ;
    }
  }

  SetBkMode(hdc, OPAQUE) ;
  SetBkMode(hdc, OPAQUE) ;
  LocalFree((LOCALHANDLE) plf) ;
}
catch (...)
{
}

}

