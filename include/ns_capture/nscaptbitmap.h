// -----------------------------------------------------------------------------// nscaptbimap.h
// -----------------------------------------------------------------------------
// Bitmaps utilisés pour la capture d'écran
// Bitmaps used for screen capture
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: fabrice $
// $Date: 2005/05/24 17:41:04 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------// Copyright Nautilus, 2005
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
#ifndef __NSCAPTBITMAP_H__
# define __NSCAPTBITMAP_H__

# include <owl\gdiobjec.h>
# include <vector>
# include <string>

//# include "partage\nsglobal.h"
//# include "partage\NTArray.h"
# include "ns_capture\nscaptexport.h"

class _NSCAPTURECLASSE NSPixel
{
 protected:
  NS_CLASSLIB::TPoint point ;
  NS_CLASSLIB::TColor color ;

 public:
  NSPixel(NS_CLASSLIB::TPoint Point, NS_CLASSLIB::TColor Color) ;
  NSPixel(int X, int Y, NS_CLASSLIB::TColor Color) ;
  NSPixel(const NSPixel& rv) ;
  ~NSPixel() ;

  NSPixel& operator=(const NSPixel& src) ;

  NS_CLASSLIB::TPoint getPoint()  { return point ; }
  int                 X()         { return point.X() ; }
  int                 Y()         { return point.Y() ; }
  NS_CLASSLIB::TColor getColor()  { return color ; }

  char *              getTuple() ;
} ;


// définition d'un array de pixels
typedef vector<NSPixel *>         PixArray ;
typedef PixArray::iterator        pixelIter ;
typedef PixArray::const_iterator  pixelCIter ;


class _NSCAPTURECLASSE PixelArray : public PixArray
{
 public:
  // Constructeurs - Destructeur
  PixelArray() ;
  PixelArray(const PixelArray& rv) ;
  ~PixelArray() ;

  NS_CLASSLIB::TRect ArrayRect ;

  bool ScreenCapture(TWindowDC * pWinDC, ClassLib::TRect * pSrcRect) ;
  bool Affiche(TDC * pDestDC, NS_CLASSLIB::TRect * pDstRect, NS_CLASSLIB::TPoint * pSrcPt) ;

  void ajouter(NSPixel * pNewPix) ;

  NS_CLASSLIB::TColor getColor(NS_CLASSLIB::TPoint pt, bool * pExist) ;

  int getBottom() { return ArrayRect.Bottom() ; }
  int getTop()    { return ArrayRect.Top() ;    }
  int getLeft()   { return ArrayRect.Left() ;   }
  int getRight()  { return ArrayRect.Right() ;  }

  void vider() ;
  PixelArray& operator=(const PixelArray& src) ;
} ;


//class NSOCRbloc ;class _NSCAPTURECLASSE NSOCRbloc{
 public:
  NS_CLASSLIB::TRect  BlocRect ;
  NS_CLASSLIB::TRect  BlocOCR ;
  string              sTexte ;
  NS_CLASSLIB::TColor bgColor ;
  NS_CLASSLIB::TColor fgColor ;

  NSOCRbloc(NS_CLASSLIB::TRect Bloc) ;
  NSOCRbloc(NS_CLASSLIB::TRect Bloc,  NS_CLASSLIB::TColor bColor, NS_CLASSLIB::TColor fColor = NS_CLASSLIB::TColor::Black) ;
  NSOCRbloc(const NSOCRbloc& rv) ;
  ~NSOCRbloc() ;

  NSOCRbloc& operator=(const NSOCRbloc& src) ;
} ;


// définition d'un array de modèles
typedef vector<NSOCRbloc *>       OCRArray ;
typedef OCRArray::iterator        blocIter ;
typedef OCRArray::const_iterator  blocCIter ;


class _NSCAPTURECLASSE OCRBlocsArray : public OCRArray
{
 public:
  // Constructeurs - Destructeur
  OCRBlocsArray() : OCRArray() {}
  OCRBlocsArray(const OCRBlocsArray& rv) ;
  ~OCRBlocsArray() ;

  void    vider() ;
  OCRBlocsArray& operator=(const OCRBlocsArray& src) ;
} ;
class _NSCAPTURECLASSE NSBitmap{
 public:
  // Constructeurs - Destructeur
  NSBitmap() ;
  NSBitmap(const NSBitmap& rv) ;
  ~NSBitmap() ;

  bool ScreenCapture(TWindowDC * pWinDC, ClassLib::TRect * pSrcRect) ;
  bool Affiche(TDC * pDestDC, NS_CLASSLIB::TRect * pDstRect, NS_CLASSLIB::TPoint * pSrcPt, int iZoomFactor = 1) ;

  bool trouveBloc(NSOCRbloc * pBloc, NS_CLASSLIB::TPoint * pPoint) ;

  bool enableCapture(ClassLib::TRect * pRect, NS_CLASSLIB::TColor * pBgColor) ;
  void nearBlank(ClassLib::TRect * pRect, NS_CLASSLIB::TColor * pBgColor, bool * pLeft, bool * pTop, bool * pRight, bool * pBottom) ;
  void existEdges(ClassLib::TRect * pRect, NS_CLASSLIB::TColor * pBgColor, bool * pLeft, bool * pTop, bool * pRight, bool * pBottom) ;

  NS_CLASSLIB::TColor getColor(int X, int Y, bool * pExist) ;
  NS_CLASSLIB::TColor getColor(NS_CLASSLIB::TPoint pt, bool * pExist) ;

  int getBottom() { return pBitmap->Height() ; }
  int getTop()    { return 0 ; }
  int getLeft()   { return 0 ; }
  int getRight()  { return pBitmap->Width() ; }

  NSBitmap& operator=(const NSBitmap& src) ;

 protected:
  TDib * pBitmap ;
} ;


#endif // !__NSCAPTBITMAP_H__

