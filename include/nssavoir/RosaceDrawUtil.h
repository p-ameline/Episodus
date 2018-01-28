// -----------------------------------------------------------------------------
// RosaceDrawUtil.h
// -----------------------------------------------------------------------------
// Outils pour dessiner et gerer des rosaces
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// Author : Tabun
// $Date: 2009/07/23 17:05:29 $
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


#include <string>
#include <windows.h>
#include <owl/static.h>


// Fonction servant a transformer un point en coordonée polaire polaire
// NS_CLASSLIB::TPoint& pPoint : point a transformer en coordoné polaire
// NS_CLASSLIB::TPoint& center : Centre des coordonnées polaire
// int* piRadius : radius a remplir
// int* iDegreAngle : angle a remplir
void		calculateParametersForPoint(NS_CLASSLIB::TPoint& pPoint,NS_CLASSLIB::TPoint& center, int* piRadius, int* iDegreAngle);


// Fonction servant a transformer un point exprimé en polaire en coordonée en cartesien
// int piRadius : radius du point a transformer
// int iDegreAngle : angle du point a transformer
// NS_CLASSLIB::TPoint& center : centre des coordonnées polaire
// NS_CLASSLIB::TPoint& pPoint : Point resultant de l'operation
void		radiusTocar(int piRadius, int iDegreAngle,NS_CLASSLIB::TPoint& center, NS_CLASSLIB::TPoint& pPoint);

// Dessine un texte sur un cercle
// HDC 							: Device graphique sur lequel ecrire
// char* texte  		: texte à afficher en cercle
// NS_CLASSLIB 			: TPoint& center point representant le centre du cercle
// int radius 			: A quel distance du texte commencer l'ecriture
// int start_angle  : Angle ou on doit commencer l'ecriture
// int val_angle		: valeur de l'angle
void    drawCircleTexte(HDC hdc, std::string to_aff, NS_CLASSLIB::TPoint& center, int radius, int start_angle, int val_angle, NS_CLASSLIB::TRect& RectAPeindre );
