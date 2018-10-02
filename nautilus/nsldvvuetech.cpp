// -----------------------------------------------------------------------------
//                  Ligne de vie - Vue du modèle Document/Vue
// -----------------------------------------------------------------------------

#include <vcl.h>
#include <owl/owlpch.h>
#include <owl/dc.h>
#include <owl/docview.h>
#include <winsys/color.h>

#include "nssavoir\nsBdmDriver.h"
#include "nautilus\nssuper.h"
#include "dcodeur\nsdkd.h"
#include "dcodeur\nsgen.h"

#include "nautilus\nautilus.rh"
#include "nautilus\nsadmiwd.h"
#include "nsbb\nstlibre.h"

#include "nsepisod\nsepidiv.h"

#include "nautilus\nshistdo.h"
#include "nautilus\nstrihis.h"
#include "nautilus\nsepicap.h"

#include <windows.h>
#include <owl\scrollba.h>

#include "nssavoir\nsgraphe.h"
#include "nssavoir\nsconver.h"

#include "nsbb\nsattvaltools.h"
#include "nautilus\nsldvvue.rh"
#include "nautilus\nshisto.rh"
#include "nsbb\nsbb.rh"
#include "nautilus\nsldvvue.h"
#include "nautilus\nsldvvuetech.h"
#include "nsldv\nsmdiframe_base.h"

#include "nautilus\nsldvgoal.h"

#define VSCROLLBAR_ID 301

long NSLdvWindowToon::lLocalObjectCount = 0 ;
long NSLdvCommandPannel::lObjectCount = 0 ;
long NSLdvCommandPannelInterface::lObjectCount = 0 ;
long NSLdvChronoLine::lObjectCount = 0 ;
long NSLdvChronoLineInterface::lObjectCount = 0 ;
long NSLdvCurveYAxis::lObjectCount = 0 ;
long NSLdvCurveYAxisInterface::lObjectCount = 0 ;
long NSLdvCurveBackgroundQuadri::lObjectCount = 0 ;
long NSLdvCurveBackground::lObjectCount = 0 ;
long NSCurvesProperty::lObjectCount = 0 ;
long NSCurvesPropertyWindow::lObjectCount = 0 ;
long NSLdvCurve::lObjectCount = 0 ;
long NSLdvCurvesManagementPannel::lObjectCount = 0 ;
long NSLdvCurvesManagementPannelInterface::lObjectCount = 0 ;
long NsObjectIcon::lObjectCount = 0 ;
long NSLdvViewIconsShowWindow::lObjectCount = 0 ;
long NSLdvViewMagnifier::lObjectCount = 0 ;

// -----------------------------------------------------------------------------
//                              NSLdvWindowToon
// -----------------------------------------------------------------------------

NSLdvWindowToon::NSLdvWindowToon(NSContexte* pCtx, NSLdvWindowToon* parent, NSLdvView* pTheView)
                :NSLdvToon(pCtx, pTheView)
{
  lLocalObjectCount++ ;

	_toonCategory = windowToon ;

  _pInterface   = 0 ;
  _pParent      = parent ;

  _bLeftSet   = false ;
  _bRightSet  = false ;
  _bTopSet    = false ;
  _bBottomSet = false ;

  _iLeftValue   = 0 ;
  _iRightValue  = 0 ;
  _iTopValue    = 0 ;
  _iBottomValue = 0 ;

  _bWidthSet    = false ;
  _bFixedWidth  = false ;
  _iWidthValue  = 0 ;
  _bHeightSet   = false ;
  _bFixedHeight = false ;
  _iHeightValue = 0 ;

  registerAsToon() ;
}


NSLdvWindowToon::NSLdvWindowToon(const NSLdvWindowToon& rv)
                :NSLdvToon(rv.pContexte, rv._pView)
{
  lLocalObjectCount++ ;

	initialiser(&rv) ;
}

void
NSLdvWindowToon::initialiser(const NSLdvWindowToon* pSrc)
{
  NSLdvToon::initialiser((NSLdvToon*)(pSrc)) ;

  _pInterface   = pSrc->_pInterface ;
  _pParent      = pSrc->_pParent ;
	// wBox        = pSrc->wBox ;
	// wVisibleBox = pSrc->wVisibleBox ;
  _sSkinName    = pSrc->_sSkinName ;

  _bLeftSet     = pSrc->_bLeftSet ;
  _bRightSet    = pSrc->_bRightSet ;
  _bTopSet      = pSrc->_bTopSet ;
  _bBottomSet   = pSrc->_bBottomSet ;

  _iLeftValue   = pSrc->_iLeftValue ;
  _iRightValue  = pSrc->_iRightValue ;
  _iTopValue    = pSrc->_iTopValue ;
  _iBottomValue = pSrc->_iBottomValue ;

  _bWidthSet    = pSrc->_bWidthSet ;
  _bFixedWidth  = pSrc->_bFixedWidth ;
  _iWidthValue  = pSrc->_iWidthValue ;
  _bHeightSet   = pSrc->_bHeightSet ;
  _bFixedHeight = pSrc->_bFixedHeight ;
  _iHeightValue = pSrc->_iHeightValue ;
}

void
NSLdvWindowToon::registerAsToon()
{
  if ((NULL == _pParent) && (NULL == _pView))
    return ;

  if (NULL == _pParent)
  {
    _pView->_aToons.push_back(this) ;
    return ;
  }

  _pParent->_aToons.push_back(this) ;
}

bool
NSLdvWindowToon::Touches(NS_CLASSLIB::TRect rect)
{
  NS_CLASSLIB::TRect visibleBox = donneVisibleRectangle() ;

	return ((rect.Right()  >= visibleBox.Left())  &&
          (rect.Left()   <= visibleBox.Right()) &&
          (rect.Bottom() >= visibleBox.Top())   &&
          (rect.Top()    <= visibleBox.Bottom())) ;
}

bool
NSLdvWindowToon::Contains(const NS_CLASSLIB::TPoint& point)
{
  NS_CLASSLIB::TRect visibleBox = donneVisibleRectangle() ;

  return visibleBox.Contains(point) ;
}

NS_CLASSLIB::TRect
NSLdvWindowToon::donneRectangle()
{
  NS_CLASSLIB::TRect theoreticalBox(_iLeftValue, _iTopValue, _iRightValue, _iBottomValue) ;
  return theoreticalBox ;
}

NS_CLASSLIB::TRect
NSLdvWindowToon::donneVisibleRectangle()
{
  NS_CLASSLIB::TRect VisibleBox(0, 0, 0, 0) ;

  if (_pInterface && (true == _pInterface->IsWindow()))
  {
    VisibleBox = _pInterface->GetClientRect() ;
  	return VisibleBox ;
  }

  if (true == isPosValid())
    VisibleBox = donneRectangle() ;

  return VisibleBox ;
}

void
NSLdvWindowToon::resetPos()
{
  _bLeftSet   = false ;
  _bRightSet  = false ;
  _bTopSet    = false ;
  _bBottomSet = false ;

  _iLeftValue   = -1 ;
  _iRightValue  = -1 ;
  _iTopValue    = -1 ;
  _iBottomValue = -1 ;

  if (false == _bFixedWidth)
  {
    _bWidthSet   = false ;
    _iWidthValue = -1 ;
  }
  if (false == _bFixedHeight)
  {
    _bHeightSet   = false ;
    _iHeightValue = -1 ;
  }
}

bool
NSLdvWindowToon::isPosValid()
{
  return _bLeftSet && _bRightSet && _bTopSet && _bBottomSet ;
}

void
NSLdvWindowToon::initDimensions()
{
try
{
  nsSkin* pSkin = pContexte->getSkins()->donneSkin(_sSkinName) ;
  if (NULL == pSkin)
  	return ;

  // Absolute values
  //
  nsBoxPosition* pBoxPos = pSkin->getBoxPosition() ;
  if (pBoxPos)
  {
    if (string("PIX") == pBoxPos->getWidthUnit())
    {
      _iWidthValue = pBoxPos->getWidthValue() ;
      _bWidthSet   = true ;
      _bFixedWidth = true ;
    }
    if (string("PIX") == pBoxPos->getHeightUnit())
    {
      _iHeightValue = pBoxPos->getHeightValue() ;
      _bHeightSet   = true ;
      _bFixedHeight = true ;
    }
    if ((true == _bWidthSet) && (true == _bHeightSet))
      return ;
  }

  // Skin dependant values
  //
  OWL::TDib* pDrawing = pSkin->getBackBmp() ;
  if (NULL == pDrawing)
    return ;

  int iDibHeight = pDrawing->Height() ;
  if (iDibHeight < 0)
  	iDibHeight = - iDibHeight ;

  int iDibWidth  = pDrawing->Width() ;
  if (iDibWidth < 0)
  	iDibWidth = - iDibWidth ;

  if (pSkin->getBackDraw() == nsSkin::DrawAsForm)
  {
    _iWidthValue = iDibWidth ;
    _bWidthSet   = true ;
    _bFixedWidth = true ;

    _iHeightValue = iDibHeight ;
    _bHeightSet   = true ;
    _bFixedHeight = true ;
  }

  if (NULL == pBoxPos)
    return ;

  if (string(FOCUS_SKIN) == pBoxPos->getWidthUnit())
  {
    _iWidthValue = iDibWidth ;
    _bWidthSet   = true ;
    _bFixedWidth = true ;
  }
  if (string(FOCUS_SKIN) == pBoxPos->getHeightUnit())
  {
    _iHeightValue = iDibHeight ;
    _bHeightSet   = true ;
    _bFixedHeight = true ;
  }
}
catch (...)
{
  string sErrorText = string("Exception NSLdvWindowToon::initDimensions.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

bool
NSLdvWindowToon::initBoxFromSkin()
{
/*
	nsSkin* pSkin = pContexte->getSkins()->donneSkin(sSkinName) ;

  if ((NULL == pSkin) || !pSkin->getBackBmp() || (pSkin->getBackDraw() != nsSkin::DrawAsForm))
  	return false ;

	OWL::TDib* pDrawing = pSkin->getBackBmp() ;

  int iDibHeight = pDrawing->Height() ;
  if (iDibHeight < 0)
  	iDibHeight = - iDibHeight ;

  int iDibWidth  = pDrawing->Width() ;
  if (iDibWidth < 0)
  	iDibWidth = - iDibWidth ;

  // Full width: only dib's height is taken into account
  //
  if (toonShape == toonFullWidth)
  {
  	wVisibleBox = NS_CLASSLIB::TRect(0, 0, 10000, iDibHeight) ;
    return true ;
  }
  //
  // Full height: only dib's width is taken into account
  //
  if (toonShape == toonFullHeight)
  {
  	wVisibleBox = NS_CLASSLIB::TRect(0, 0, iDibWidth, 100000) ;
    return true ;
  }
  //
  // Limited: both width and height are taken into account
  //
  if (toonShape == toonSizeLimited)
  {
  	wVisibleBox = NS_CLASSLIB::TRect(0, 0, iDibWidth, iDibHeight) ;
    return true ;
  }
*/

  return false ;
}

void
NSLdvWindowToon::calculateWidthValue()
{
  if ((false == _bLeftSet) || (false == _bRightSet) || (true == _bWidthSet))
    return ;

  _iWidthValue = _iRightValue - _iLeftValue ;
  _bWidthSet   = true ;
}

void
NSLdvWindowToon::calculateHeightValue()
{
  if ((false == _bTopSet) || (false == _bBottomSet) || (true == _bHeightSet))
    return ;

  _iHeightValue = _iTopValue - _iBottomValue ;
  _bHeightSet   = true ;
}

void
NSLdvWindowToon::calculateLeftValue()
{
  if ((false == _bRightSet) || (false == _bWidthSet) || (true == _bLeftSet))
    return ;

  _iLeftValue = _iRightValue - _iWidthValue ;
  _bLeftSet   = true ;
}

void
NSLdvWindowToon::calculateRightValue()
{
  if ((false == _bLeftSet) || (false == _bWidthSet) || (true == _bRightSet))
    return ;

  _iRightValue = _iLeftValue + _iWidthValue ;
  _bRightSet   = true ;
}

void
NSLdvWindowToon::calculateTopValue()
{
  if ((false == _bHeightSet) || (false == _bBottomSet) || (true == _bTopSet))
    return ;

  _iTopValue = _iBottomValue + _iHeightValue ;
  _bTopSet   = true ;
}

void
NSLdvWindowToon::calculateBottomValue()
{
  if ((false == _bTopSet) || (false == _bHeightSet) || (true == _bBottomSet))
    return ;

  _iBottomValue = _iTopValue - _iHeightValue ;
  _bBottomSet   = true ;
}

void
NSLdvWindowToon::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal)
{
try
{
	NS_CLASSLIB::TRect screenRect = _pView->getGlobalPhysicalRect(*pRectARepeindre) ;
  draw(pDc, &screenRect) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvWindowToon::draw.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvWindowToon::draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
	skinDraw(pDc, pRectARepeindre) ;
}

void
NSLdvWindowToon::skinDraw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
try
{
	if ((string("") == _sSkinName) || (NULL == _pInterface) || (false == _pInterface->IsWindow()))
  	return ;

	nsSkin* pSkin = pContexte->getSkins()->donneSkin(_sSkinName) ;

  if (NULL == pSkin)
		return ;

  if (nsSkin::DrawAsForm == pSkin->getBackDraw())
    skinDrawForm(pDc, pRectARepeindre) ;
  if (nsSkin::DrawTile   == pSkin->getBackDraw())
    skinDrawTile(pDc, pRectARepeindre) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvWindowToon::skinDraw.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvWindowToon::skinDrawForm(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
try
{
  if ((string("") == _sSkinName) || (NULL == _pInterface) || (false == _pInterface->IsWindow()))
  	return ;

	nsSkin* pSkin = pContexte->getSkins()->donneSkin(_sSkinName) ;

  if (NULL == pSkin)
		return ;

  NS_CLASSLIB::TRect clientRect = _pInterface->GetClientRect() ;

	OWL::TDib* pBackgrnd = pSkin->getBackBmp() ;
  if (!pBackgrnd)
		return ;

  NS_CLASSLIB::TPoint dibOrigin ;
  // bool bBottomUp ;

  //
  // Bottom up dib : origin lies at the lower left corner
  //
  if (pBackgrnd->Height() > 0)
  {
  	// bBottomUp = true ;
    dibOrigin = NS_CLASSLIB::TPoint(0, 0) ;
  }
  //
  // top down dib : origin lies at the upper left corner
  //
  else
  {
  	// bBottomUp = false ;
    dibOrigin = NS_CLASSLIB::TPoint(0, 0) ;
  }

  pDc->SetDIBitsToDevice(clientRect, dibOrigin, *pBackgrnd) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvWindowToon::skinDrawForm.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvWindowToon::skinDrawTile(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
try
{
  if ((string("") == _sSkinName) || (NULL == _pInterface) || (false == _pInterface->IsWindow()))
  	return ;

	nsSkin* pSkin = pContexte->getSkins()->donneSkin(_sSkinName) ;

  if (NULL == pSkin)
		return ;

  NS_CLASSLIB::TRect clientRect = _pInterface->GetClientRect() ;

	OWL::TDib* pBackgrnd = pSkin->getBackBmp() ;
  if (!pBackgrnd)
		return ;

  NS_CLASSLIB::TPoint dibOrigin ;
  bool bBottomUp ;

  //
  // Bottom up dib : origin lies at the lower left corner
  //
  if (pBackgrnd->Height() > 0)
  {
  	bBottomUp = true ;
    dibOrigin = NS_CLASSLIB::TPoint(0, 0) ;
  }
  //
  // top down dib : origin lies at the upper left corner
  //
  else
  {
  	bBottomUp = false ;
    dibOrigin = NS_CLASSLIB::TPoint(0, 0) ;
  }

  int iTop  = clientRect.Top() ;
  int iLeft = clientRect.Left() ;
  int x = 0 ;
  int y = 0 ;
  while (y < clientRect.Height())
  {
  	while (x < clientRect.Width())
    {
      int iWidthToPaint  = pBackgrnd->Width() ;
      if (x + iWidthToPaint > clientRect.Width())
        iWidthToPaint = clientRect.Width() - x ;

      int iHeightToPaint = pBackgrnd->Height() ;
      if (y + iHeightToPaint > clientRect.Height())
        iHeightToPaint = clientRect.Height() - y ;

    	NS_CLASSLIB::TRect drawRect = NS_CLASSLIB::TRect(x + iLeft, y + iTop, x + iLeft + iWidthToPaint, y + iTop + iHeightToPaint) ;
      pDc->SetDIBitsToDevice(drawRect, dibOrigin, *pBackgrnd) ;

      x += pBackgrnd->Width() ;
    }
    if (bBottomUp)
    	y += pBackgrnd->Height() ;
    else
    	y -= pBackgrnd->Height() ;

    x = 0 ;
  }

	// ---------------------------------------------------------------------------
	// Restauration des objets initiaux (pen et brush)
	// ::SelectObject(pDc->GetHDC(), oldPen);
	// ::SelectObject(pDc->GetHDC(), oldBrush);
}
catch (...)
{
  string sErrorText = string("Exception NSLdvWindowToon::skinDrawTile.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

/*
void
NSLdvWindowToon::skinDraw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
	if (string("") == sSkinName)
  	return ;

	nsSkin* pSkin = pContexte->getSkins()->donneSkin(sSkinName) ;

  if (NULL == pSkin)
		return ;

	NS_CLASSLIB::TRect clientRect = pView->GetClientRect() ;

	OWL::TDib* pBackgrnd = pSkin->getBackBmp() ;
  if (!pBackgrnd)
		return ;

	wBox = NS_CLASSLIB::TRect(0, 0, clientRect.Width(), pBackgrnd->Height()) ;

  NS_CLASSLIB::TPoint dibOrigin ;
  bool bBottomUp ;

  //
  // Bottom up dib : origin lies at the lower left corner
  //
  if (pBackgrnd->Height() > 0)
  {
  	bBottomUp = true ;
    dibOrigin = NS_CLASSLIB::TPoint(0, 0) ;
  }
  //
  // top down dib : origin lies at the upper left corner
  //
  else
  {
  	bBottomUp = false ;
    dibOrigin = NS_CLASSLIB::TPoint(0, 0) ;
  }

  NS_CLASSLIB::TRect winRect = wVisibleBox ;

  int iTop  = wVisibleBox.Top() ;
  int iLeft = wVisibleBox.Left() ;
  int x = 0 ;
  int y = 0 ;
  while (y < wVisibleBox.Height())
  {
  	while (x < wVisibleBox.Width())
    {
      int iWidthToPaint  = pBackgrnd->Width() ;
      if (x + iWidthToPaint > winRect.Width())
        iWidthToPaint = winRect.Width() - x ;

      int iHeightToPaint = pBackgrnd->Height() ;
      if (y + iHeightToPaint > winRect.Height())
        iHeightToPaint = winRect.Height() - y ;

    	NS_CLASSLIB::TRect drawRect = NS_CLASSLIB::TRect(x + iLeft, y + iTop, x + iLeft + iWidthToPaint, y + iTop + iHeightToPaint) ;
      pDc->SetDIBitsToDevice(drawRect, dibOrigin, *pBackgrnd) ;

      x += pBackgrnd->Width() ;
    }
    if (bBottomUp)
    	y += pBackgrnd->Height() ;
    else
    	y -= pBackgrnd->Height() ;

    x = 0 ;
  }

	// ---------------------------------------------------------------------------
	// Restauration des objets initiaux (pen et brush)
	// ::SelectObject(pDc->GetHDC(), oldPen);
	// ::SelectObject(pDc->GetHDC(), oldBrush);
}
*/

bool
NSLdvWindowToon::MoveWindow(bool repaint)
{
  if (false == isPosValid())
    return false ;

  NS_CLASSLIB::TRect ToonRect = donneRectangle()  ;
  if (NULL == _pParent)
    ToonRect = _pView->getGlobalPhysicalRect(ToonRect) ;
  else
  {
    ToonRect = _pParent->getRelativeRect(ToonRect) ;
    ToonRect = _pParent->getRectForDraw(&ToonRect) ;
  }

  bool bMoveResult = MoveWindow(ToonRect, repaint) ;

  if (true == bMoveResult)
    setScrollParams() ;

  return bMoveResult ;
}

bool
NSLdvWindowToon::MoveWindow(const NS_CLASSLIB::TRect& rect, bool repaint)
{
  if ((NULL == _pInterface) || (false == _pInterface->IsWindow()))
    return false ;

  return _pInterface->MoveWindow(rect, repaint) ;
}

void
NSLdvWindowToon::EvaluateWinToonsPos()
{
  if ((NULL == _pInterface) || (false == _pInterface->IsWindow()))
    return ;

  _pView->EvaluateWinToonsPos(_pInterface, &_aToons) ;
}

int
NSLdvWindowToon::getYValueFromChildYValue(int iLdVYValue, TWindow* pChild)
{
  if ((NULL == pChild) || (false == pChild->IsWindow()))
    return iLdVYValue ;

  NS_CLASSLIB::TRect screenRect      = _pInterface->GetWindowRect() ;
  NS_CLASSLIB::TRect childScreenRect = pChild->GetWindowRect() ;

  int iReturnValue = iLdVYValue + screenRect.Bottom() - childScreenRect.Bottom() ;

  // If it is a working area, take scrolling into account
  //
  if (toonWorkingArea == _toonType)
  {
    NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(this, NSLdvViewArea) ;
    if (pWorkArea)
      iReturnValue += pWorkArea->getPosInfDroit() ;
  }

  return iReturnValue ;
}

int
NSLdvWindowToon::getWindowYValueFromLdVYValue(int iLdVYValue)
{
	// First, for windows, Y = 0 is on top of the screen, while it is on the
  // bottom of the chronoLine for the LdV. We have to reverse it.
  //
  NS_CLASSLIB::TRect clientRect = GetClientRect() ;
  int iWinYValue = clientRect.Height() - iLdVYValue ;

  // Then, the LdV can be scrolled of a lPosInfDroit quantity
  //
  // iWinYValue += pView->getPosInfDroit() ;

  return iWinYValue ;
}

NS_CLASSLIB::TRect
NSLdvWindowToon::getScreenRectFromLdvRect(NVLdVRect* pLdvRect)
{
  NS_CLASSLIB::TPoint ptTopLeft = _pView->getGlobalPhysicalPoint(pLdvRect->TopLeft()) ;
	NS_CLASSLIB::TPoint ptBotRigh = _pView->getGlobalPhysicalPoint(pLdvRect->BottomRight()) ;

  NS_CLASSLIB::TRect screenRect(ptTopLeft.x, ptTopLeft.y, ptBotRigh.x, ptBotRigh.y) ;

  return screenRect ;
}

NS_CLASSLIB::TRect
NSLdvWindowToon::getScreenRect(NS_CLASSLIB::TRect *pFixedRect)
{
  return NS_CLASSLIB::TRect(0, 0, 0, 0) ;
}

// Window's reference frame has top-left as origin, while ldv has bottom-left
//
NS_CLASSLIB::TPoint
NSLdvWindowToon::getPointForDraw(NS_CLASSLIB::TPoint *pLdvPoint)
{
  NS_CLASSLIB::TPoint windowsFramePoint = *pLdvPoint ;

  if ((NULL == _pInterface) || (false == _pInterface->IsWindow()))
  {
    windowsFramePoint.y = _iHeightValue - windowsFramePoint.y ;
    return windowsFramePoint ;
  }

  NS_CLASSLIB::TRect clientRect = _pInterface->GetClientRect() ;
  windowsFramePoint.y = clientRect.Height() - windowsFramePoint.y ;

  return windowsFramePoint ;
}

NS_CLASSLIB::TRect
NSLdvWindowToon::getRectForDraw(NS_CLASSLIB::TRect *pLdvRect)
{
  NS_CLASSLIB::TPoint ptTopLeft = getPointForDraw(&(pLdvRect->TopLeft())) ;
	NS_CLASSLIB::TPoint ptBotRigh = getPointForDraw(&(pLdvRect->BottomRight())) ;

  return NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
}

NS_CLASSLIB::TPoint
NSLdvWindowToon::getPointForDraw(NVLdVPoint ldvPoint)
{
  NS_CLASSLIB::TPoint physicalPoint(0, 0) ;

  if ((NULL == _pView) || (NULL == _pInterface) || (false == _pInterface->IsWindow()))
    return physicalPoint ;

  // For time, the reference is given by the right side of Chronoline
  //
  NSLdvChronoLine* pChronoLine = _pView->getLdVChronoLine() ;
  if (NULL == pChronoLine)
    return physicalPoint ;

  NS_CLASSLIB::TRect ChronoRect = pChronoLine->donneVisibleRectangle() ;
  NS_CLASSLIB::TRect ThisRect   = donneVisibleRectangle() ;
  int iDeltaRight = ThisRect.Right() - ChronoRect.Right() ;

	// On place le point en bas à droite
	// Instantiating the point as clientRect bottom right point
  //
	NS_CLASSLIB::TRect clientRect = _pInterface->GetClientRect() ;
	physicalPoint = clientRect.BottomRight() ;

	// Translation du point
	// Translating the point

	NVLdVRect ldvRect(_pView,
										ldvPoint.getX(),
										_pView->getDateHeureInfDroit(),
										0, // lPosInfDroit,
										ldvPoint.getY()) ;

	physicalPoint.x -= _pView->getPhysicalWidthFromTimeUnit(ldvRect.Width()) ;
  physicalPoint.x -= iDeltaRight ;

  if (physicalPoint.x > numeric_limits<int>::max())
  	physicalPoint.x = numeric_limits<int>::max() ;
  if (physicalPoint.x < - numeric_limits<int>::max())
  	physicalPoint.x = - numeric_limits<int>::max() ;

	physicalPoint.y += ldvRect.Height() ;

  if (physicalPoint.y > numeric_limits<int>::max())
  	physicalPoint.y = numeric_limits<int>::max() ;
  if (physicalPoint.y < - numeric_limits<int>::max())
  	physicalPoint.y = - numeric_limits<int>::max() ;

	return physicalPoint ;
}

NS_CLASSLIB::TRect
NSLdvWindowToon::getRectForDraw(NVLdVRect ldvRect)
{
	NS_CLASSLIB::TPoint ptTopLeft = getPointForDraw(ldvRect.TopLeft()) ;
	NS_CLASSLIB::TPoint ptBotRigh = getPointForDraw(ldvRect.BottomRight()) ;

  return NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
}

NS_CLASSLIB::TPoint
NSLdvWindowToon::getRelativePoint(NS_CLASSLIB::TPoint ldvPoint)
{
  // Local reference frame is bottom-left
  NS_CLASSLIB::TRect  viewRect = donneRectangle() ;
  NS_CLASSLIB::TPoint ptResult = NS_CLASSLIB::TPoint(ldvPoint.X() - viewRect.Left(), ldvPoint.Y() - viewRect.Bottom()) ;

  return ptResult ;
}

NS_CLASSLIB::TRect
NSLdvWindowToon::getRelativeRect(NS_CLASSLIB::TRect ldvRect)
{
	NS_CLASSLIB::TPoint ptTopLeft = getRelativePoint(ldvRect.TopLeft()) ;
	NS_CLASSLIB::TPoint ptBotRigh = getRelativePoint(ldvRect.BottomRight()) ;

  return NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
}

TWindow*
NSLdvWindowToon::getParentInterface()
{
  if (NULL == _pParent)
    return _pView ;

  return _pParent->getInterface() ;
}

/*
NSLdvVerticalScrollBar*
NSLdvWindowToon::getVerticalScroll()
{
  if (aToons.empty())
    return NULL ;

  for (ToonsIter toonIter = aToons.begin() ; aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonVerticalScroll == (*toonIter)->getType())
    {
      NSLdvVerticalScrollBar *pVScroll = TYPESAFE_DOWNCAST(*toonIter, NSLdvVerticalScrollBar) ;
      if (NULL != pVScroll)
        return pVScroll ;
    }
  }

  return NULL ;
}
*/

// -----------------------------------------------------------------------------
// ------------------- METHODES DE NSLdvCommandPannel --------------------------
// -----------------------------------------------------------------------------

NSLdvCommandPannel::NSLdvCommandPannel(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvView* pTheView)
                   :NSLdvWindowToon(pCtx, pLdvParent, pTheView)
{
  lObjectCount++ ;

	_iZOrder   = LEVEL_COMMANDS ;
  _toonType  = toonPannel ;
  // toonShape = toonFullWidth ;
  _sSkinName = string("LdvCommandPannel") ;

  initDimensions() ;
  // if (!initBoxFromSkin())
	//	wVisibleBox = pView->getDateTimeArea() ;
  // else
  //	pView->setDateTimeArea(wVisibleBox) ;

  _pInterface = new NSLdvCommandPannelInterface(pContexte, getParentInterface(), this) ;

	// pSeverityThresholdSlider = 0 ;
  // pDrugsDisplayOnOff       = 0 ;
  // pCompressedMode          = 0 ;
}


NSLdvCommandPannel::NSLdvCommandPannel(const NSLdvCommandPannel& rv)
                   :NSLdvWindowToon(rv.pContexte, rv.getToonParent(), rv._pView)
{
  lObjectCount++ ;

	initialiser((NSLdvWindowToon*)(&rv)) ;
}


NSLdvCommandPannel::~NSLdvCommandPannel()
{
  lObjectCount-- ;

	// if (NULL != pSeverityThresholdSlider)
	// 	delete pSeverityThresholdSlider ;
	// if (NULL != pDrugsDisplayOnOff)
  //	delete pDrugsDisplayOnOff ;
  // if (NULL != pCompressedMode)
	//	delete pCompressedMode ;
}

void
NSLdvCommandPannel::SetupWindow()
{
}

void
NSLdvCommandPannel::EvRButtonDown()
{
}

/*
void
NSLdvCommandPannel::draw(TDC* pDc, NVLdVRect* pRectARepeindre)
{
  NS_CLASSLIB::TRect screenRect = pView->getGlobalPhysicalRect(*pRectARepeindre) ;
  draw(pDc, &screenRect) ;
}

void
NSLdvCommandPannel::draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
	skinDraw(pDc, pRectARepeindre) ;
}
*/

void
NSLdvCommandPannel::dysplayStatusMessage(string* pMessage)
{
try
{
	if (!pMessage || (*pMessage == string("")))
		return ;

  string sLangue  = pContexte->getUserLanguage() ;

	// Get the lib of "years" (plural of "year")
  //
  string sCodeLexAn = string("2DAT33") ;
  NSPathologData Data ;
  pContexte->getDico()->trouvePathologData(sLangue, &sCodeLexAn, &Data) ;
  GENRE iGenre ;
  Data.donneGenre(&iGenre) ;
  Data.donneGenrePluriel(&iGenre) ;
  string sLibel ;
  NSGenerateur* pGene = pContexte->getDico()->getGenerateur() ;
  pGene->donneLibelleAffiche(&sLibel, &Data, iGenre) ;

  string sCodeLexMois = string("2DAT21") ;
  pContexte->getDico()->trouvePathologData(sLangue, &sCodeLexMois, &Data) ;
  Data.donneGenre(&iGenre) ;
  Data.donneGenrePluriel(&iGenre) ;
  string sLibelMois ;
  pGene->donneLibelleAffiche(&sLibelMois, &Data, iGenre) ;

	NVLdVTemps tNaissance = _pView->getDateNaissance() ;

  string sCurDate = string(*pMessage, 6, 4) + string(*pMessage, 3, 2) + string(*pMessage, 0, 2) ;
  int iAge = donne_age(sCurDate, tNaissance.donneDate()) ;

  if (iAge >= 2)
  {
    string sAge = string("?") ;
    if (iAge >= 0)
      sAge = IntToString(iAge) ;
    *pMessage = *pMessage + string("  ->  ") + sAge + string(" ") + sLibel ;
  }
  else
  {
    iAge = donne_age_mois(sCurDate, tNaissance.donneDate()) ;
    *pMessage = *pMessage + string("  ->  ") + IntToString(iAge) + string(" ") + sLibelMois ;
  }

  TWindowDC* pWinDC = new TWindowDC(_pView->HWindow) ; // HWnd
  // drawText(pWinDC, *pMessage, "LdvCommandPannelMouseTime", &wBox) ;
	delete pWinDC ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvCommandPannel::dysplayStatusMessage.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvCommandPannel::dysplayPatientInformation()
{
	char  sAge[20] ;
	sAge[0] = '\0';

	string sLangue  = pContexte->getUserLanguage() ;
	string sTitre   = pContexte->getPatient()->donneTitre(sLangue) ;

	NVLdVTemps tNaissance = _pView->getDateNaissance() ;
	if (false == tNaissance.estVide())
	{
  	// Get the lib of "years" (plural of "year")
    //
  	string sCodeLexAn = string("2DAT33") ;
    NSPathologData Data ;
    pContexte->getDico()->trouvePathologData(sLangue, &sCodeLexAn, &Data) ;
    GENRE iGenre ;
    Data.donneGenre(&iGenre) ;
    Data.donneGenrePluriel(&iGenre) ;
    string sLibel ;
    NSGenerateur* pGene = pContexte->getDico()->getGenerateur() ;
    pGene->donneLibelleAffiche(&sLibel, &Data, iGenre) ;

    string sCodeLexMois = string("2DAT21") ;
    pContexte->getDico()->trouvePathologData(sLangue, &sCodeLexMois, &Data) ;
    Data.donneGenre(&iGenre) ;
    Data.donneGenrePluriel(&iGenre) ;
    string sLibelMois ;
    pGene->donneLibelleAffiche(&sLibelMois, &Data, iGenre) ;

    string sCurrentDate = donne_date_duJour() ;
		int    iCurrentAge  = donne_age(sCurrentDate, tNaissance.donneDate()) ;

		if (iCurrentAge >= 2)
			sTitre += string(" (") + IntToString(iCurrentAge) + string(" ") + sLibel + string(")") ;
		else if (iCurrentAge >= 0)
		{
    	iCurrentAge = donne_age_mois(sCurrentDate, tNaissance.donneDate()) ;
      sTitre += string(" (") + IntToString(iCurrentAge) + string(" ") + sLibelMois + string(")") ;
    }
  }

  TWindowDC* pWinDC = new TWindowDC(_pView->HWindow) ; // HWnd
  // drawText(pWinDC, sTitre, "LdvCommandPannelPatInfo", &wBox) ;
	delete pWinDC ;
}

void
NSLdvCommandPannel::drawText(TDC* pDc, string sText, string sSkinName, NS_CLASSLIB::TRect* pRect)
{
try
{
	nsSkin* pSkin = pContexte->getSkins()->donneSkin(sSkinName) ;

  NS_CLASSLIB::TRect displayRect = *pRect ;

  if (pSkin)
  {
  	// NS_CLASSLIB::TRect* pLocalRect = pSkin->getPosition() ;
  	// if (pLocalRect)
    //	displayRect = NS_CLASSLIB::TRect(wBox.Left() + pLocalRect->Left(),
    //                                   wBox.Top() + pLocalRect->Top(),
    //                                   wBox.Right(), wBox.Bottom()) ;

    ClassLib::TColor* pFontColor = pSkin->getFontColor() ;
    if (pFontColor)
    	pDc->SetTextColor(*pFontColor) ;

    ClassLib::TColor* pBackColor = pSkin->getFontBackColor() ;
    if (pBackColor)
    	pDc->SetBkColor(*pBackColor) ;

    LOGFONT* pLogFont = pSkin->getLogFont() ;
    if (pLogFont)
    	pDc->SelectObject(OWL::TFont(pLogFont)) ;
  }
  else
		pDc->FillRect(displayRect, NS_CLASSLIB::TColor::White) ;

	pDc->DrawText(sText.c_str(), -1, displayRect, DT_CENTER) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvCommandPannel::drawText.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvCommandPannel::setScrollParams()
{
}

int
NSLdvCommandPannel::getSeverityThreshold()
{
  if (NULL == _pInterface)
    return 0 ;

  NSLdvCommandPannelInterface* pPannel = TYPESAFE_DOWNCAST(_pInterface, NSLdvCommandPannelInterface) ;
  if (pPannel)
    return pPannel->getSeverityThreshold() ;

  return 0 ;
}

// -----------------------------------------------------------------------------
// --------------- METHODES DE NSLdvCommandPannelInterface ------------------
// -----------------------------------------------------------------------------

#define ID_SEVERITY_SLIDER 201

DEFINE_RESPONSE_TABLE1(NSLdvCommandPannelInterface, TWindow)
  EV_WM_RBUTTONDOWN,
  EV_CHILD_NOTIFY_ALL_CODES(ID_SEVERITY_SLIDER, UpdateThresholdSlider),
END_RESPONSE_TABLE ;

NSLdvCommandPannelInterface::NSLdvCommandPannelInterface(NSContexte* pCtx, TWindow* parent, NSLdvCommandPannel* pToon)
                            :TWindow(parent), NSRoot(pCtx)
{
  lObjectCount++ ;

  _pToon = pToon ;

  SetCaption(_pToon->_sSkinName.c_str()) ;

  NS_CLASSLIB::TRect rectDTArea = _pToon->getView()->getDateTimeArea() ;
  _pSeverityThresholdSlider = new TVSlider(this, ID_SEVERITY_SLIDER, rectDTArea.Left() + rectDTArea.Width(), 2, 20, pToon->getHeightValue() - 4) ;
}

NSLdvCommandPannelInterface::NSLdvCommandPannelInterface(const NSLdvCommandPannelInterface& rv)
                            :TWindow(rv.GetParent()), NSRoot(rv.pContexte)
{
  lObjectCount++ ;

  _pToon = rv.getToon() ;
}

NSLdvCommandPannelInterface::~NSLdvCommandPannelInterface()
{
  lObjectCount-- ;

  delete _pSeverityThresholdSlider ;
}

void
NSLdvCommandPannelInterface::SetupWindow()
{
  TWindow::SetupWindow() ;

  _pSeverityThresholdSlider->SetRuler(10, true) ;
  _pSeverityThresholdSlider->SetRange(-1, 100) ;
  _pSeverityThresholdSlider->SetPosition(-1) ;
}

void
NSLdvCommandPannelInterface::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  _pToon->skinDraw(&dc, &RectAPeindre) ;
}

void
NSLdvCommandPannelInterface::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  TWindow::EvRButtonDown(modKeys, point) ;
}

int
NSLdvCommandPannelInterface::getSeverityThreshold()
{
  if (NULL == _pSeverityThresholdSlider)
    return 0 ;

  return _pSeverityThresholdSlider->GetPosition() ;
}

void
NSLdvCommandPannelInterface::UpdateThresholdSlider(WPARAM code)
{
  if ((NULL == _pToon) || (NULL == _pToon->getView()))
    return ;

  int iPos = _pSeverityThresholdSlider->GetPosition() ;
  int iThr = _pToon->getView()->getSeverityThreshold() ;

  if (iPos == iThr)
    return ;

  _pToon->getView()->UpdateSeverityThreshold(iThr) ;
}

NSLdvCommandPannelInterface&
NSLdvCommandPannelInterface::operator=(const NSLdvCommandPannelInterface& src)
{
  if (this == &src)
    return *this ;

  _pToon = src.getToon() ;

  return *this ;
}

// -----------------------------------------------------------------------------
// ------------------- METHODES DE NSLdvChronoLine --------------------------
// -----------------------------------------------------------------------------

NSLdvChronoLine::NSLdvChronoLine(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvView* pTheView)
                :NSLdvWindowToon(pCtx, pLdvParent, pTheView)
{
  lObjectCount++ ;

  _bUnitIsChanging = false ;

	_iZOrder   = LEVEL_COMMANDS ;
  _toonType  = toonChronoLine ;
  // toonShape = toonFullWidth ;
  _sSkinName = string("LdvChronoLine") ;

  initDimensions() ;

/*
  if (!initBoxFromSkin())
  {
  	TEXTMETRIC tm;

		TWindowDC* pWinDC = new TWindowDC(pView->GetParent()) ; // HWnd

		int iSimpleLineHeight = 0 ;
		if (pWinDC->GetTextMetrics(tm))
			iSimpleLineHeight = tm.tmHeight + tm.tmInternalLeading + tm.tmExternalLeading ;

		int iHauteur = iSimpleLineHeight * 2 + 10 ;

		delete pWinDC;

    // wVisibleBox = NS_CLASSLIB::TRect(0, 0, 10000, iHauteur) ;
	}
*/

  _pInterface = new NSLdvChronoLineInterface(pContexte, getParentInterface(), this) ;
}

NSLdvChronoLine::NSLdvChronoLine(const NSLdvChronoLine& rv)
                :NSLdvWindowToon(rv.pContexte, rv.getToonParent(), rv._pView)
{
  lObjectCount++ ;

  _bUnitIsChanging = rv._bUnitIsChanging ;
	initialiser((NSLdvWindowToon*)(&rv)) ;
}

NSLdvChronoLine::~NSLdvChronoLine()
{
  lObjectCount-- ;
}

void
NSLdvChronoLine::EvRButtonDown()
{
}

void
NSLdvChronoLine::SetupWindow()
{
}

void
NSLdvChronoLine::setScrollParams()
{
}

void
NSLdvChronoLine::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal)
{
	NS_CLASSLIB::TRect screenRect = _pView->getGlobalPhysicalRect(*pRectARepeindre) ;

  draw(pDc, &screenRect) ;
}

void
NSLdvChronoLine::draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
try
{
  // Recalculate wVisibleBox
  //
  // wVisibleBox = NS_CLASSLIB::TRect(0, pView->GetClientRect().Height() - wVisibleBox.Height(), pView->GetClientRect().Width(), wVisibleBox.Height()) ;

  NVLdVTemps minLeftDateTime ;
  minLeftDateTime.initFromDate(string("19000101000000")) ;

	skinDraw(pDc, pRectARepeindre) ;

	NVLdVTemps tDateHeureInfGauche = _pView->getDateHeureInfDroit() ;

  int iTimeUnitWidth = _pView->getTimeUnitFromPhysicalWidth(_pInterface->GetClientRect().Width()) ;

  pixUnit iViewXUnit = _pView->getXunit() ;
	switch (iViewXUnit)
	{
  	case pixSeconde :
			tDateHeureInfGauche.ajouteSecondes(-iTimeUnitWidth) ;
			break ;
  	case pixMinute :
			tDateHeureInfGauche.ajouteMinutes(-iTimeUnitWidth) ;
			break ;
		case pixHeure :
			tDateHeureInfGauche.ajouteHeures(-iTimeUnitWidth) ;
			break ;
		case pixJour :
			tDateHeureInfGauche.ajouteJours(-iTimeUnitWidth) ;
			break ;
		case pixSemaine :
			tDateHeureInfGauche.ajouteJours(-iTimeUnitWidth * 7) ;
			break ;
		case pixMois :
			tDateHeureInfGauche.ajouteMois(-iTimeUnitWidth) ;
			break ;
  }

  if (tDateHeureInfGauche < minLeftDateTime)
  {
    tDateHeureInfGauche = minLeftDateTime ;
    _pView->setRightTimeFromLeftTime(tDateHeureInfGauche) ;
  }

  switch (iViewXUnit)
	{
  	case pixSeconde :
			drawChronoStepMinutes(pDc, pRectARepeindre, tDateHeureInfGauche) ;
			drawChronoContextHours(pDc, pRectARepeindre, tDateHeureInfGauche) ;
			break ;

  	case pixMinute :
			drawChronoStepHours(pDc, pRectARepeindre, tDateHeureInfGauche) ;
			drawChronoContextDays(pDc, pRectARepeindre, tDateHeureInfGauche) ;
			break ;

		case pixHeure :
			drawChronoStepDays(pDc, pRectARepeindre, tDateHeureInfGauche) ;
			drawChronoContextMonths(pDc, pRectARepeindre, tDateHeureInfGauche) ;
			break ;

		case pixJour :
			drawChronoStepMonths(pDc, pRectARepeindre, tDateHeureInfGauche) ;
			drawChronoContextYears(pDc, pRectARepeindre, tDateHeureInfGauche) ;
			break ;

		case pixSemaine :
			drawChronoStepYears(pDc, pRectARepeindre, tDateHeureInfGauche) ;
			break ;

		case pixMois :
			drawChronoStepDecads(pDc, pRectARepeindre, tDateHeureInfGauche) ;
			drawChronoContextDecads(pDc, pRectARepeindre, tDateHeureInfGauche) ;
			break ;
  }
}
catch (...)
{
  string sErrorText = string("Exception NSLdvChronoLine::draw.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvChronoLine::drawChronoStep(TDC *pDc, NVLdVTemps tGauche, NVLdVTemps tDroite, string sTexte, bool bRed)
{
try
{
	int iSimpleLineHeight = getLineHeight() ;

	NVLdVRect textDisplay   = NVLdVRect(_pView, tGauche, tDroite, long(iSimpleLineHeight), long(iSimpleLineHeight * 2)) ;
  NVLdVRect periodDisplay = NVLdVRect(_pView, tGauche, tDroite, long(iSimpleLineHeight * 2), long(iSimpleLineHeight * 2 + 5)) ;

  // NS_CLASSLIB::TRect textRect = pView->getGlobalPhysicalRect(textDisplay) ;
  // NS_CLASSLIB::TRect stepRect = pView->getGlobalPhysicalRect(periodDisplay) ;

  NS_CLASSLIB::TRect textRect = getRectForDraw(textDisplay) ;
  NS_CLASSLIB::TRect stepRect = getRectForDraw(periodDisplay) ;

	pDc->DrawText(sTexte.c_str(), -1, textRect, DT_CENTER) ;
  pDc->FillRect(textRect.X(), textRect.Y(), textRect.X() + 1, textRect.Y() + 5, OWL::TBrush(_pView->getBlackBrushHandle())) ;

	if (bRed)
  	pDc->FillRect(stepRect, OWL::TBrush(_pView->getRedBrushHandle())) ;
  else
  	pDc->FillRect(stepRect, OWL::TBrush(_pView->getBlackBrushHandle())) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvChronoLine::drawChronoStep.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}


// -----------------------------------------------------------------------------
// affichage de la chronoline
// -----------------------------------------------------------------------------
// La chronoline est divisé en deux parties. La partie supérieure est la partie
// que nous appelerons step (unité la plus représentative), la partie inférieure
// est la partie que nous appelerons context (contexte de l'unité la plus
// représentative).
// -----------------------------------------------------------------------------
// Par exemple si on est à l'echelle : 1 pixel == 1 heure alors l'affichage de
// * la partie supérieure (step) sera par jour (24 pixels de largeur)
// * la partie inférieure (context) sera par mois (28 à 31 jours)
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// affichage de la partie supérieure de la chronoline (step) par minutes
// -----------------------------------------------------------------------------
// NSLdvView::drawChronoStepMinutes(TDC *, NVLdVRect *, NVLdVTemps)
// -----------------------------------------------------------------------------
// échelle : 1 pixel == 1 seconde
// -----------------------------------------------------------------------------

void
NSLdvChronoLine::drawChronoStepMinutes(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche)
{
try
{
	NVLdVTemps  tDroite = _pView->getDateHeureInfDroit() ;
	NVLdVTemps  tGauche = NVLdVTemps(	tDroite.donneAns(),
																		tDroite.donneMois(),
																		tDroite.donneJours(),
																		tDroite.donneHeures(),
																		tDroite.donneMinutes(),
																		0) ;
	bool        bRed = false ;

	while (tDroite > tDateHeureInfGauche)
	{
    int iMinute = tGauche.donneMinutes() ;
    string sText = IntToString(iMinute) ;

    drawChronoStep(pDc, tGauche, tDroite, sText, bRed) ;

		bRed = !bRed ;
		tDroite = tGauche ;
		tGauche.ajouteMinutes(-1) ;
	}
}
catch (...)
{
  erreur("Exception NSLdvChronoLine drawChronoStepDays.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// affichage de la partie supérieure de la chronoline (step) par heure
// -----------------------------------------------------------------------------
// NSLdvView::drawChronoStepHours(TDC *, NVLdVRect *, NVLdVTemps)
// -----------------------------------------------------------------------------
// échelle : 1 pixel == 1 minute
// -----------------------------------------------------------------------------
void
NSLdvChronoLine::drawChronoStepHours(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche)
{
try
{
	NVLdVTemps  tDroite = _pView->getDateHeureInfDroit() ;
	NVLdVTemps  tGauche = NVLdVTemps(	tDroite.donneAns(),
																		tDroite.donneMois(),
																		tDroite.donneJours(),
																		tDroite.donneHeures(),
																		0,
																		0) ;
	bool        bRed = false ;

	while (tDroite > tDateHeureInfGauche)
	{
		int iHour = tGauche.donneHeures() ;
    string sText = IntToString(iHour) ;

    drawChronoStep(pDc, tGauche, tDroite, sText, bRed) ;

		bRed = !bRed ;
		tDroite = tGauche ;
		tGauche.ajouteHeures(-1) ;
	}
}
catch (...)
{
  erreur("Exception NSLdvChronoLine drawChronoStepDays.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------// affichage de la partie supérieure de la chronoline (step) par jour
// -----------------------------------------------------------------------------
// NSLdvView::drawChronoStepDays(TDC *, NVLdVRect *, NVLdVTemps)
// -----------------------------------------------------------------------------
// échelle : 1 pixel == 1 heure
// -----------------------------------------------------------------------------

void
NSLdvChronoLine::drawChronoStepDays(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche)
{
try
{
	NVLdVTemps  tDroite = _pView->getDateHeureInfDroit() ;
	NVLdVTemps  tGauche = NVLdVTemps(	tDroite.donneAns(),
																		tDroite.donneMois(),
																		tDroite.donneJours(),
																		0,
																		0,
																		0) ;
	bool        bRed = false ;

	while (tDroite > tDateHeureInfGauche)
	{
		int iJour = tGauche.donneJours() ;
		string sText = IntToString(iJour) ;

    drawChronoStep(pDc, tGauche, tDroite, sText, bRed) ;

		bRed = !bRed ;
		tDroite = tGauche ;
		tGauche.ajouteJours(-1) ;
	}
}
catch (...)
{
  erreur("Exception NSLdvChronoLine drawChronoStepDays.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// affichage de la partie supérieure de la chronoline (step) par mois
// -----------------------------------------------------------------------------
// NSLdvView::drawChronoStepMonths(TDC *, NVLdVRect *, NVLdVTemps)
// -----------------------------------------------------------------------------
// échelle : 1 pixel == 1 jour
// -----------------------------------------------------------------------------
void
NSLdvChronoLine::drawChronoStepMonths(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche)
{
try
{
	NVLdVTemps tDroite = _pView->getDateHeureInfDroit() ;
	NVLdVTemps tGauche = NVLdVTemps(tDroite.donneAns(), tDroite.donneMois(), 1, 0, 0, 0) ;

	bool        bRed = false ;

	while (tDroite > tDateHeureInfGauche)
	{
		int iMois = tGauche.donneMois() ;
		string sText = IntToString(iMois) ;

    drawChronoStep(pDc, tGauche, tDroite, sText, bRed) ;

		bRed = !bRed ;
		tDroite = tGauche ;
		tGauche.ajouteMois(-1) ;
	}
}
catch (...)
{
	erreur("Exception NSLdvChronoLine drawChronoStepMonths.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// affichage de la partie supérieure de la chronoline (step) par année
// -----------------------------------------------------------------------------
// NSLdvView::drawChronoStepYears(TDC *, NVLdVRect *, NVLdVTemps)
// -----------------------------------------------------------------------------
// échelle : 1 pixel == 1 semaine
// -----------------------------------------------------------------------------
void
NSLdvChronoLine::drawChronoStepYears(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche)
{
try
{
	NVLdVTemps  tDroite = _pView->getDateHeureInfDroit() ;
	NVLdVTemps  tGauche = NVLdVTemps(tDroite.donneAns(), 1, 1, 0, 0, 0) ;

	bool        bRed = false ;

	while (tDroite > tDateHeureInfGauche)
	{
		int iAnnee = tGauche.donneAns() ;
		string sText = IntToString(iAnnee) ;

    drawChronoStep(pDc, tGauche, tDroite, sText, bRed) ;

		bRed = !bRed ;
		tDroite = tGauche ;
		tGauche.ajouteAns(-1) ;
	}
}
catch (...)
{
	erreur("Exception NSLdvChronoLine drawChronoStepYears.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// affichage de la partie supérieure de la chronoline (step) par décennie
// -----------------------------------------------------------------------------
// NSLdvView::drawChronoStepDecads(TDC *, NVLdVRect *, NVLdVTemps)
// -----------------------------------------------------------------------------
// échelle : 1 pixel == 1 mois
// -----------------------------------------------------------------------------
void
NSLdvChronoLine::drawChronoStepDecads(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche)
{
try
{
	NVLdVTemps  tDroite = _pView->getDateHeureInfDroit() ;
	NVLdVTemps  tGauche = NVLdVTemps(tDroite.donneAns(), 1, 1, 0, 0, 0) ;

	bool        bRed = false ;

	while (tDroite > tDateHeureInfGauche)
	{
		int iAnnee = tGauche.donneAns() % 10 ;
		string sText = IntToString(iAnnee) ;

    drawChronoStep(pDc, tGauche, tDroite, sText, bRed) ;

		bRed = !bRed ;
		tDroite = tGauche ;
		tGauche.ajouteAns(-1) ;
	}
}
catch (...)
{
	erreur("Exception NSLdvChronoLine drawChronoStepDecads.", standardError, 0) ;
}
}

void
NSLdvChronoLine::drawChronoContext(TDC *pDc, NVLdVTemps tGauche, NVLdVTemps tDroite, string sTexte)
{
	int iSimpleLineHeight = getLineHeight() ;

	NVLdVRect rectDisplay = NVLdVRect(_pView, tGauche, tDroite, long(0), long(iSimpleLineHeight)) ;

  NS_CLASSLIB::TPoint ptTopLeft = getPointForDraw(rectDisplay.TopLeft()) ;
  NS_CLASSLIB::TPoint ptBotRigh = getPointForDraw(rectDisplay.BottomRight()) ;

	// NS_CLASSLIB::TPoint ptTopLeft = pView->getGlobalPhysicalPoint(rectDisplay.TopLeft());
	// NS_CLASSLIB::TPoint ptBotRigh = pView->getGlobalPhysicalPoint(rectDisplay.BottomRight());

  pDc->DrawText(sTexte.c_str(), -1, NS_CLASSLIB::TRect(ptTopLeft.x, ptTopLeft.y, ptBotRigh.x, ptBotRigh.y), DT_CENTER) ;
}

// -----------------------------------------------------------------------------
// affichage de la partie inférieure de la chronoline (context) par heure
// -----------------------------------------------------------------------------
// NSLdvView::drawChronoContextMonths(TDC *, NVLdVRect *, NVLdVTemps)
// -----------------------------------------------------------------------------
// échelle : 1 pixel == 1 minute
// -----------------------------------------------------------------------------
void
NSLdvChronoLine::drawChronoContextHours(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche)
{
try
{
	string sLang = pContexte->getUserLanguage() ;

	NVLdVTemps tDroite = _pView->getDateHeureInfDroit() ;
	NVLdVTemps tGauche = NVLdVTemps(tDroite.donneAns(), tDroite.donneMois(), tDroite.donneJours(), tDroite.donneHeures(), 1, 0) ;

	while (tDroite > tDateHeureInfGauche)
	{
		if (tGauche < tDateHeureInfGauche)
			tGauche = tDateHeureInfGauche ;

    int     iHour   = tGauche.donneHeures() ;
    int     iJour   = tGauche.donneJours() ;
		int     iMois   = tGauche.donneMois() ;
		string  sMois   = donne_mois(iMois, sLang) ;
		int     iAnnees = tGauche.donneAns() ;
		char    pcDate[20] ;
		sprintf(pcDate, "%d %s %d %d:00", iJour, sMois.c_str(), iAnnees, iHour) ;

    string sTexte = string(pcDate) ;
    drawChronoContext(pDc, tGauche, tDroite, sTexte) ;

		tDroite = tGauche ;
		tGauche.ajouteHeures(-1) ;
	}
}
catch (...)
{
	erreur("Exception NSLdvChronoLine drawChronoContextDays.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// affichage de la partie inférieure de la chronoline (context) par jour
// -----------------------------------------------------------------------------
// NSLdvView::drawChronoContextMonths(TDC *, NVLdVRect *, NVLdVTemps)
// -----------------------------------------------------------------------------
// échelle : 1 pixel == 1 minute
// -----------------------------------------------------------------------------
void
NSLdvChronoLine::drawChronoContextDays(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche)
{
try
{
	string sLang = pContexte->getUserLanguage() ;

	NVLdVTemps tDroite = _pView->getDateHeureInfDroit() ;
	NVLdVTemps tGauche = NVLdVTemps(tDroite.donneAns(), tDroite.donneMois(), tDroite.donneJours(), 1, 0, 0) ;

	while (tDroite > tDateHeureInfGauche)
	{
		if (tGauche < tDateHeureInfGauche)
			tGauche = tDateHeureInfGauche ;

    int     iJour   = tGauche.donneJours() ;
		int     iMois   = tGauche.donneMois() ;
		string  sMois   = donne_mois(iMois, sLang) ;
		int     iAnnees = tGauche.donneAns() ;
		char    pcDate[20] ;
		sprintf(pcDate, "%d %s %d", iJour, sMois.c_str(), iAnnees) ;

    string sTexte = string(pcDate) ;
    drawChronoContext(pDc, tGauche, tDroite, sTexte) ;

		tDroite = tGauche ;
		tGauche.ajouteJours(-1) ;
	}
}
catch (...)
{
	erreur("Exception NSLdvChronoLine drawChronoContextDays.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// affichage de la partie inférieure de la chronoline (context) par mois
// -----------------------------------------------------------------------------
// NSLdvView::drawChronoContextMonths(TDC *, NVLdVRect *, NVLdVTemps)
// -----------------------------------------------------------------------------
// échelle : 1 pixel == 1 heure
// -----------------------------------------------------------------------------
void
NSLdvChronoLine::drawChronoContextMonths(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche)
{
try
{
	string sLang = pContexte->getUserLanguage() ;

	NVLdVTemps tDroite = _pView->getDateHeureInfDroit() ;
	NVLdVTemps tGauche = NVLdVTemps(tDroite.donneAns(), tDroite.donneMois(), 1, 0, 0, 0) ;

	while (tDroite > tDateHeureInfGauche)
	{
		if (tGauche < tDateHeureInfGauche)
			tGauche = tDateHeureInfGauche ;

		int     iMois   = tGauche.donneMois() ;
		string  sMois   = donne_mois(iMois, sLang) ;
		int     iAnnees = tGauche.donneAns() ;
		char    pcDate[20] ;
		sprintf(pcDate, "%s %d", sMois.c_str(), iAnnees) ;

    string sTexte = string(pcDate) ;
    drawChronoContext(pDc, tGauche, tDroite, sTexte) ;

		tDroite = tGauche ;
		tGauche.ajouteMois(-1) ;
	}
}
catch (...)
{
	erreur("Exception NSLdvChronoLine drawChronoContextMonths.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// affichage de la partie inférieure de la chronoline (context) par année
// -----------------------------------------------------------------------------
// NSLdvView::drawChronoContextYears(TDC *, NVLdVRect *, NVLdVTemps)
// -----------------------------------------------------------------------------
// échelle : 1 pixel == 1 jour
// -----------------------------------------------------------------------------
void
NSLdvChronoLine::drawChronoContextYears(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche)
{
try
{
	NVLdVTemps tDroite = _pView->getDateHeureInfDroit() ;
	NVLdVTemps tGauche = NVLdVTemps(tDroite.donneAns(), 1, 1, 0, 0, 0) ;

	while (tDroite > tDateHeureInfGauche)
	{
		if (tGauche < tDateHeureInfGauche)
			tGauche = tDateHeureInfGauche ;

		int     iAnnees = tGauche.donneAns() ;
		char    pcDate[5] ;
		sprintf(pcDate, "%d", iAnnees) ;

    string sTexte = string(pcDate) ;
    drawChronoContext(pDc, tGauche, tDroite, sTexte) ;

		tDroite = tGauche ;
		tGauche.ajouteAns(-1) ;
	}
}
catch (...)
{
	erreur("Exception NSLdvChronoLine drawChronoContextYears.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// affichage de la partie inférieure de la chronoline (context) par décenies
// -----------------------------------------------------------------------------
// NSLdvView::drawChronoContextDecads(TDC *, NVLdVRect *, NVLdVTemps)
// -----------------------------------------------------------------------------
// échelle : 1 pixel == 1 mois
// -----------------------------------------------------------------------------
void
NSLdvChronoLine::drawChronoContextDecads(TDC *pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche)
{
try
{
	NVLdVTemps tDroite = _pView->getDateHeureInfDroit() ;
	NVLdVTemps tGauche = NVLdVTemps((tDroite.donneAns() / 10) * 10, 1, 1, 0, 0, 0) ;

	while (tDroite > tDateHeureInfGauche)
	{
		if (tGauche < tDateHeureInfGauche)
			tGauche = tDateHeureInfGauche ;

		int     iAnnees = tGauche.donneAns() ;
		char    pcDate[5] ;
		sprintf(pcDate, "%d", iAnnees) ;

    string sTexte = string(pcDate) ;
    drawChronoContext(pDc, tGauche, tDroite, sTexte) ;

		tDroite = tGauche ;
		tGauche.ajouteAns(-10) ;
	}
}
catch (...)
{
	erreur("Exception NSLdvChronoLine drawChronoContextDecads.", standardError, 0) ;
}
}

void
NSLdvChronoLine::refresh()
{
  NSTimeZoomLevel* pTimeInfo = _pView->getCurrentZoomInfo() ;

  NSLdvChronoLineInterface* pChrono = TYPESAFE_DOWNCAST(_pInterface, NSLdvChronoLineInterface) ;
  if (NULL == pChrono)
    return ;

  pChrono->getSlider()->SetRange(pTimeInfo->getLowerLimit(), pTimeInfo->getUpperLimit() - 1) ;
  pChrono->getSlider()->SetPosition(pTimeInfo->getUppRate()) ;
}

int
NSLdvChronoLine::getLineHeight()
{
	// return (wVisibleBox.Height() - 10) / 2 ;

  int iInterfaceHeight = 0 ;

  if (_pInterface && (true == _pInterface->IsWindow()))
  {
    NS_CLASSLIB::TRect clientRect = _pInterface->GetClientRect() ;
    iInterfaceHeight = clientRect.Height() ;
  }
  else
    iInterfaceHeight = _iHeightValue ;

  return (iInterfaceHeight - 10) / 2 ;
}

int
NSLdvChronoLine::getPpuRatePosition()
{
  if (NULL == _pInterface)
    return 0 ;

  NSLdvChronoLineInterface* pChrono = TYPESAFE_DOWNCAST(_pInterface, NSLdvChronoLineInterface) ;
  if (pChrono)
    return pChrono->getPpuRatePosition() ;

  return 0 ;
}

void
NSLdvChronoLine::unitChanging()
{
  _bUnitIsChanging = true ;
}

void
NSLdvChronoLine::unitChanged()
{
  _bUnitIsChanging = false ;
}

void
NSLdvChronoLine::initPpuRateInterface(NSTimeZoomLevel* pTimeInfo)
{
  if (NULL == pTimeInfo)
    return ;

  NSLdvChronoLineInterface* pChrono = TYPESAFE_DOWNCAST(_pInterface, NSLdvChronoLineInterface) ;
  if (NULL == pChrono)
    return ;

  pChrono->getSlider()->SetRange(pTimeInfo->getLowerLimit(), pTimeInfo->getUpperLimit() - 1) ;
  pChrono->getSlider()->SetPosition(pTimeInfo->getUppRate()) ;
}

// -----------------------------------------------------------------------------
// ----------------- METHODES DE NSLdvChronoLineInterface --------------------
// -----------------------------------------------------------------------------

#define ID_SLIDER 201

DEFINE_RESPONSE_TABLE1(NSLdvChronoLineInterface, TWindow)
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(CM_LDV_PIXSECOND,    CmLdvPixSecond),    // pixel == 1 seconde
	EV_COMMAND(CM_LDV_PIXMINUTE,    CmLdvPixMinute),    // pixel == 1 minute
	EV_COMMAND(CM_LDV_PIXHOUR,      CmLdvPixHour),      // pixel == 1 heure
	EV_COMMAND(CM_LDV_PIXDAY,       CmLdvPixDay),       // pixel == 1 jour
	EV_COMMAND(CM_LDV_PIXWEEK,      CmLdvPixWeek),      // pixel == 1 semaine
	EV_COMMAND(CM_LDV_PIXMONTH,     CmLdvPixMonth),     // pixel == 1 mois
  EV_CHILD_NOTIFY_ALL_CODES(ID_SLIDER, UpdateTimeSlider),
END_RESPONSE_TABLE ;

NSLdvChronoLineInterface::NSLdvChronoLineInterface(NSContexte* pCtx, TWindow* parent, NSLdvChronoLine* pToon)
                         :TWindow(parent), NSRoot(pCtx)
{
  lObjectCount++ ;

  _pToon   = pToon ;
  _pSlider = new THSlider(this, ID_SLIDER, 5, pToon->getHeightValue() - 15, 200, 15) ;

  SetCaption(_pToon->_sSkinName.c_str()) ;
}

NSLdvChronoLineInterface::NSLdvChronoLineInterface(const NSLdvChronoLineInterface& rv)
                         :TWindow(rv.GetParent()), NSRoot(rv.pContexte)
{
  lObjectCount++ ;

  _pToon = rv.getToon() ;
}

NSLdvChronoLineInterface::~NSLdvChronoLineInterface()
{
  lObjectCount-- ;

  delete _pSlider ;
}

void
NSLdvChronoLineInterface::SetupWindow()
{
  TWindow::SetupWindow() ;

  _pSlider->SetRuler(1, true) ;

  NSTimeZoomLevel* pTimeInfo = _pToon->getView()->getCurrentZoomInfo() ;
  if (pTimeInfo)
  {
    _pSlider->SetRange(pTimeInfo->getLowerLimit(), pTimeInfo->getUpperLimit() - 1) ;
    _pSlider->SetPosition(pTimeInfo->getUppRate()) ;
  }
}

void
NSLdvChronoLineInterface::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  if (_pToon)
    _pToon->draw(&dc, &RectAPeindre) ;
}

void
NSLdvChronoLineInterface::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  OWL::TPopupMenu *zoomMenu = new OWL::TPopupMenu() ;

  string sMenuText = pContexte->getSuperviseur()->getText("LigneDeVieMenus", "1pixelPerSecond") ;
  zoomMenu->AppendMenu(MF_STRING, CM_LDV_PIXSECOND, sMenuText.c_str()) ;
  sMenuText = pContexte->getSuperviseur()->getText("LigneDeVieMenus", "1pixelPerMinute") ;
  zoomMenu->AppendMenu(MF_STRING, CM_LDV_PIXMINUTE, sMenuText.c_str()) ;
  sMenuText = pContexte->getSuperviseur()->getText("LigneDeVieMenus", "1pixelPerHour") ;
  zoomMenu->AppendMenu(MF_STRING, CM_LDV_PIXHOUR,  sMenuText.c_str()) ;
  sMenuText = pContexte->getSuperviseur()->getText("LigneDeVieMenus", "1pixelPerDay") ;
  zoomMenu->AppendMenu(MF_STRING, CM_LDV_PIXDAY,   sMenuText.c_str()) ;
  sMenuText = pContexte->getSuperviseur()->getText("LigneDeVieMenus", "1pixelPerWeek") ;
  zoomMenu->AppendMenu(MF_STRING, CM_LDV_PIXWEEK,  sMenuText.c_str()) ;
  sMenuText = pContexte->getSuperviseur()->getText("LigneDeVieMenus", "1pixelPerMonth") ;
  zoomMenu->AppendMenu(MF_STRING, CM_LDV_PIXMONTH, sMenuText.c_str()) ;

  ClientToScreen(point) ;
  zoomMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;
  delete zoomMenu ;

  TWindow::EvRButtonDown(modKeys, point) ;
}

void
NSLdvChronoLineInterface::CmLdvPixSecond()
{
	_pToon->getView()->setXunit(pixSeconde) ;
  _pToon->getView()->Invalidate(true) ;
}

void
NSLdvChronoLineInterface::CmLdvPixMinute()
{
  _pToon->getView()->setXunit(pixMinute) ;
  _pToon->getView()->Invalidate(true) ;
}

void
NSLdvChronoLineInterface::CmLdvPixHour()
{
  _pToon->getView()->setXunit(pixHeure) ;
  _pToon->getView()->Invalidate(true) ;
}

void
NSLdvChronoLineInterface::CmLdvPixDay()
{
  _pToon->getView()->setXunit(pixJour) ;
  _pToon->getView()->Invalidate(true) ;
}

void
NSLdvChronoLineInterface::CmLdvPixWeek()
{
  _pToon->getView()->setXunit(pixSemaine) ;
  _pToon->getView()->Invalidate(true) ;
}

void
NSLdvChronoLineInterface::CmLdvPixMonth()
{
  _pToon->getView()->setXunit(pixMois) ;
  _pToon->getView()->Invalidate(true) ;
}

int
NSLdvChronoLineInterface::getPpuRatePosition()
{
  if (NULL == _pSlider)
    return 0 ;

  int iPos = _pSlider->GetPosition() ;

  return iPos ;
}

void
NSLdvChronoLineInterface::UpdateTimeSlider(WPARAM code)
{
  if ((NULL == _pToon) || (_pToon->isUnitChanging()) || (NULL == _pToon->getView()))
    return ;

  int iPos = _pSlider->GetPosition() ;
  int iPpu = _pToon->getView()->getPpuRate() ;

  if (iPos == iPpu)
    return ;

  _pToon->getView()->UpdatePixelPerUnitRate(getPpuRatePosition()) ;
}

NSLdvChronoLineInterface&
NSLdvChronoLineInterface::operator=(const NSLdvChronoLineInterface& src)
{
  if (this == &src)
    return *this ;

  _pToon = src.getToon() ;

  return *this ;
}

// -----------------------------------------------------------------------------
//                       NSLdvVerticalScrollBar VSCROLLBAR_ID
// -----------------------------------------------------------------------------

/*

NSLdvVerticalScrollBar::NSLdvVerticalScrollBar(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvView* pTheView)
                       :NSLdvWindowToon(pCtx, pLdvParent, pTheView)

{
	toonType = toonVerticalScroll ;

  sSkinName = string("LdvVerticalScrollBar") ;

  initDimensions() ;

  pInterface = new NSLdvVerticalScrollBarInterface(pContexte, getParentInterface(), this) ;
}

void
NSLdvVerticalScrollBar::SetupWindow()
{
}

void
NSLdvVerticalScrollBar::setVisibleBox()
{
  // int iRight  = pView->getWorkingSpaceRight() ;
  // int iTop    = pView->getWorkingSpaceTop() ;
  // int iBottom = pView->getWorkingSpaceBottom() ;

  int iWidth = 10 ;

  // wVisibleBox = NS_CLASSLIB::TRect(iRight - iWidth, iTop, iRight, iBottom) ;
}

void
NSLdvVerticalScrollBar::draw(TDC* pDc, NVLdVRect* pRectARepeindre)
{
	NS_CLASSLIB::TRect screenRect = pView->getGlobalPhysicalRect(*pRectARepeindre) ;

  draw(pDc, &screenRect) ;
}
*/

// void
// NSLdvVerticalScrollBar::draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
// {
/*
	setVisibleBox() ;

  int iWorkingSpaceHeight = pView->getWorkingSpaceHeight() ;
  int iCurrentYRange      = pView->getGlobalBox().Height() ;

  // Every toon fits inside the workingspace
  //
	if (iCurrentYRange <= iWorkingSpaceHeight)
  {
  	pInterface->Show(SW_HIDE) ;
    return ;
  }

  int iRight = pView->getWorkingSpaceRight() ;
  int iTop   = pView->getWorkingSpaceTop() ;

  int iWidth = 10 ;
  // pInterface->SetWindowPos(0, wVisibleBox.Left(), wVisibleBox.Top(), wVisibleBox.Width(), wVisibleBox.Height(), SWP_NOZORDER) ;

  // Since we start from the bottom, we must "reverse" nPos
  //
  int iMaxDeltaPos = iCurrentYRange - iWorkingSpaceHeight ;
  int iThumbPos    = iMaxDeltaPos - pView->getPosInfDroit() ;

  SCROLLINFO scInfo ;
	scInfo.cbSize = (UINT) sizeof(scInfo) ;
	scInfo.fMask = SIF_ALL ;
	scInfo.nMin  = 0 ;
	scInfo.nMax  = iCurrentYRange ;
	scInfo.nPage = (UINT) (iWorkingSpaceHeight) ;
	scInfo.nPos  = iThumbPos ;

  OWL::TScrollBar* pScroll = TYPESAFE_DOWNCAST(pInterface, OWL::TScrollBar) ;
  if (NULL != pScroll)
    pScroll->SetScrollInfo(&scInfo, TRUE) ;

  pInterface->Show(SW_SHOW) ;
*/
// }

/*
void
NSLdvVerticalScrollBar::EvRButtonDown()
{
}

void
NSLdvVerticalScrollBar::FixeRange(int max, int taille)
{
}

void
NSLdvVerticalScrollBar::PositionneCurseur()
{
}

// -----------------------------------------------------------------------------
// ----------------- METHODES DE NSLdvChronoLineInterface --------------------
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSLdvVerticalScrollBarInterface, OWL::TScrollBar)
  EV_WM_PAINT,
END_RESPONSE_TABLE ;

NSLdvVerticalScrollBarInterface::NSLdvVerticalScrollBarInterface(NSContexte* pCtx, TWindow* parent, NSLdvVerticalScrollBar* pToon)
                                :OWL::TScrollBar(parent, VSCROLLBAR_ID, 0, 0, 0, 0, false), NSRoot(pCtx)
{
  _pToon = pToon ;

  SetCaption(_pToon->sSkinName.c_str()) ;
}

NSLdvVerticalScrollBarInterface::NSLdvVerticalScrollBarInterface(NSLdvVerticalScrollBarInterface& rv)
                                :OWL::TScrollBar((TWindow*)(rv.GetParent()), VSCROLLBAR_ID, 0, 0, 0, 0, false), NSRoot(rv.pContexte)
{
  _pToon = rv.getToon() ;
}

NSLdvVerticalScrollBarInterface::~NSLdvVerticalScrollBarInterface()
{
}

void
NSLdvVerticalScrollBarInterface::SetupWindow()
{
  OWL::TScrollBar::SetupWindow() ;
}

void
NSLdvVerticalScrollBarInterface::EvPaint()
{
  TPaintDC dc(*this) ;
}

void
NSLdvVerticalScrollBarInterface::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  _pToon->draw(&dc, &RectAPeindre) ;

  NSLdvWindowToon* pParentToon = _pToon->getToonParent() ;
  if ((NULL == pParentToon) || (NSLdvToon::toonWorkingArea != pParentToon->getType()))
  {
    OWL::TScrollBar::Paint(dc, true, RectAPeindre) ;
    return ;
  }

  NSLdvViewArea *pWorkingArea = TYPESAFE_DOWNCAST(pParentToon, NSLdvViewArea) ;
  if (NULL == pWorkingArea)
  {
    OWL::TScrollBar::Paint(dc, true, RectAPeindre) ;
    return ;
  }

  int iWorkingSpaceHeight = pWorkingArea->getWorkingSpaceHeight() ;
  int iCurrentYRange      = pWorkingArea->getGlobalBox()->Height() ;

  // Every toon fits inside the workingspace
  //
	if (iCurrentYRange <= iWorkingSpaceHeight)
  {
  	Show(SW_HIDE) ;
    return ;
  }

  int iRight = pWorkingArea->getWorkingSpaceRight() ;
  int iTop   = pWorkingArea->getWorkingSpaceTop() ;

  int iWidth = 10 ;
  // pInterface->SetWindowPos(0, wVisibleBox.Left(), wVisibleBox.Top(), wVisibleBox.Width(), wVisibleBox.Height(), SWP_NOZORDER) ;

  // Since we start from the bottom, we must "reverse" nPos
  //
  int iMaxDeltaPos = iCurrentYRange - iWorkingSpaceHeight ;
  int iThumbPos    = iMaxDeltaPos - pWorkingArea->getPosInfDroit() ;

  SCROLLINFO scInfo ;
	scInfo.cbSize = (UINT) sizeof(scInfo) ;
	scInfo.fMask = SIF_ALL ;
	scInfo.nMin  = 0 ;
	scInfo.nMax  = iCurrentYRange ;
	scInfo.nPage = (UINT) (iWorkingSpaceHeight) ;
	scInfo.nPos  = iThumbPos ;

  SetScrollInfo(&scInfo, TRUE) ;
  Show(SW_SHOW) ;

  // OWL::TScrollBar::Paint(dc, true, RectAPeindre) ;
}

void
NSLdvVerticalScrollBarInterface::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  OWL::TScrollBar::EvRButtonDown(modKeys, point) ;
}

void
NSLdvVerticalScrollBarInterface::SBLineDown()
{
  if ((NULL == _pToon) || (NULL == _pToon->getToonParent()))
    return ;

  NSLdvViewArea *pWorkingArea = TYPESAFE_DOWNCAST(_pToon->getToonParent(), NSLdvViewArea) ;
  if (NULL != pWorkingArea)
    pWorkingArea->increasePosInfDroit(-5) ;
}

void
NSLdvVerticalScrollBarInterface::SBLineUp()
{
  if ((NULL == _pToon) || (NULL == _pToon->getToonParent()))
    return ;

  NSLdvViewArea *pWorkingArea = TYPESAFE_DOWNCAST(_pToon->getToonParent(), NSLdvViewArea) ;
  if (NULL != pWorkingArea)
    pWorkingArea->increasePosInfDroit(5) ;
}

void
NSLdvVerticalScrollBarInterface::SBPageDown()
{
}

void
NSLdvVerticalScrollBarInterface::SBPageUp()
{
}

NSLdvVerticalScrollBarInterface&
NSLdvVerticalScrollBarInterface::operator=(NSLdvVerticalScrollBarInterface& src)
{
  if (this == &src)
    return *this ;

  _pToon = src.getToon() ;

  return *this ;
}

bool
NSLdvVerticalScrollBarInterface::PreProcessMsg(MSG& msg)
{
  switch(msg.message)
  {
    case WM_PAINT:
    {
      // NS_CLASSLIB::TPoint pt1(LOWORD(msg.lParam), HIWORD(msg.lParam)) ;
      // NS_CLASSLIB::TPoint pt2(LOWORD(msg.wParam), HIWORD(msg.wParam)) ;
      // NS_CLASSLIB::TRect  rect1(pt1, pt2) ;

      TPaintDC dc(*this) ;
      NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&dc.Ps.rcPaint ;
      Paint(dc, true, rect) ;

      DefWindowProc(msg.message, msg.wParam, msg.lParam) ;

      break ;
    }
    case WM_NCPAINT:
      DefWindowProc(msg.message, msg.wParam, msg.lParam) ;
      break ;
  }

  return TWindow::PreProcessMsg(msg) ;
}
*/

// -----------------------------------------------------------------------------
//                       NSLdvCurveYAxis
// -----------------------------------------------------------------------------

NSLdvCurveYAxis::NSLdvCurveYAxis(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvView* pView, NSLdvCurve* pTheCurve)
                :NSLdvWindowToon(pCtx, pLdvParent, pView)
{
  lObjectCount++ ;

	pCurve = pTheCurve ;

	_iZOrder   = LEVEL_COMMANDS ;
  _toonType  = toonYAxis ;
  // toonShape = toonFullHeight ;
  _sSkinName = string("LdvCurvesYAxis") ;

  initDimensions() ;

	_dLowAxisValue   = double(0) ;
  _dHighAxisValue  = double(0) ;

  _dLowActualValue = double(0) ;
  _dHigActualValue = double(0) ;

  _bIncludeZero    = true ;

  if (!initBoxFromSkin())
  {
    // int iBottom = pView->getPixelPosAboveChronoline(0) ;
    // int iHeight = pCurve->getCurvesPannel()->getYAxisPixelRange() ;

    // wVisibleBox = NS_CLASSLIB::TRect(0, iBottom + iHeight, 100, iBottom) ;
	}

  _pInterface = new NSLdvCurveYAxisInterface(pContexte, getParentInterface(), this) ;
}

NSLdvCurveYAxis::NSLdvCurveYAxis(const NSLdvCurveYAxis& rv)
                :NSLdvWindowToon(rv.pContexte, rv.getToonParent(), rv._pView)
{
  lObjectCount++ ;

  pCurve          = rv.pCurve ;

  _dLowActualValue = rv._dLowActualValue ;
  _dHigActualValue = rv._dHigActualValue ;

	_dLowAxisValue   = rv._dLowAxisValue ;
  _dHighAxisValue  = rv._dHighAxisValue ;

  _bIncludeZero    = rv._bIncludeZero ;
}

NSLdvCurveYAxis::~NSLdvCurveYAxis()
{
  lObjectCount-- ;

  if (_pInterface)
    delete _pInterface ;
}

void
NSLdvCurveYAxis::SetupWindow()
{
}

void
NSLdvCurveYAxis::setScrollParams()
{
}

void
NSLdvCurveYAxis::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal)
{
  NSLdvViewArea* pWorkArea = _pView->getActiveWorkingArea() ;
  if (NULL == pWorkArea)
    return ;

  NS_CLASSLIB::TRect screenRect = pWorkArea->getScrollablePhysicalRect(*pRectARepeindre) ;

  draw(pDc, &screenRect) ;
}

void
NSLdvCurveYAxis::draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
	drawAxis(pDc, pRectARepeindre) ;
}

bool
NSLdvCurveYAxis::MoveWindow(bool repaint)
{
  bool bMoveResult = NSLdvWindowToon::MoveWindow(repaint) ;

  if (pCurve->getFirstPoint())
    pCurve->reinit(false) ;

  return bMoveResult ;
}

void
NSLdvCurveYAxis::setVisible()
{
  _bVisible = true ;
  if (_pInterface)
    _pInterface->Show(SW_SHOW) ;
}

void
NSLdvCurveYAxis::setHidden()
{
  _bVisible = false ;
  if (_pInterface)
    _pInterface->Show(SW_HIDE) ;
}

void
NSLdvCurveYAxis::EvRButtonDown()
{
}

void
NSLdvCurveYAxis::drawAxis(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
try
{
	if (aTicks.empty())
  	return ;

  int iMaxTextHeight = 0 ;
  int iMaxTextWidth  = 0 ;

  vector<string>::iterator iter = aTicks.begin() ;
	for ( ; iter != aTicks.end() ; iter++)
	{
  	NS_CLASSLIB::TSize textSize = pDc->GetTextExtent((*iter).c_str(), strlen((*iter).c_str())) ;
    iMaxTextWidth  = max(textSize.X(), iMaxTextWidth) ;
    iMaxTextHeight = max(textSize.Y(), iMaxTextHeight) ;
  }

  // Need to resize the wVisibleBox ?
  //
  // if (wVisibleBox.Width() != iMaxTextWidth + 10)
  // 	wVisibleBox.Set(wVisibleBox.Left(), wVisibleBox.Top(),
  //              wVisibleBox.Left() + iMaxTextWidth + 10, wVisibleBox.Bottom()) ;

  // Erase the area
  //
  // NS_CLASSLIB::TRect windowsBox(wVisibleBox.Left(),
  //                              getWindowYValueFromLdVYValue(wVisibleBox.Top()),
  //                              wVisibleBox.Right(),
  //                              getWindowYValueFromLdVYValue(wVisibleBox.Bottom())) ;
  NS_CLASSLIB::TRect windowsBox = donneVisibleRectangle() ;
	pDc->FillRect(windowsBox, NS_CLASSLIB::TColor::White) ;

  // Draw ticks
  //
	iter = aTicks.begin() ;
	for ( ; iter != aTicks.end() ; iter++)
	{
		double dTickValue  = StringToDouble(*iter) ;
    int    iYPos       = giveYCoordinateForValue(dTickValue) ;
    int    iWinYPos    = getWindowYValueFromLdVYValue(iYPos) ;

    // Drawing the tick's text
    //
    int    iTextTop    = iWinYPos - iMaxTextHeight / 2 ;
    int    iTextBottom = iTextTop + iMaxTextHeight ;
    int    iTextLeft   = windowsBox.Left() ;
    int    iTextRight  = windowsBox.Left() + iMaxTextWidth ;

    NS_CLASSLIB::TRect textRect(iTextLeft, iTextTop, iTextRight, iTextBottom) ;
    pDc->DrawText((*iter).c_str(), -1, textRect, DT_RIGHT | DT_VCENTER) ;

    // Drawing the Tick's line
    //
    int iLeftTickLine  = windowsBox.Left() + iMaxTextWidth + 1 ;
    int iRightTickLine = windowsBox.Left() + iMaxTextWidth + 10 ;
    NS_CLASSLIB::TPoint ptLeft(iLeftTickLine, iWinYPos) ;
    pDc->MoveTo(ptLeft) ;
    NS_CLASSLIB::TPoint ptRight(iRightTickLine, iWinYPos) ;
    pDc->LineTo(ptRight) ;
  }

  // Draw the vertical separator
  //
  NS_CLASSLIB::TPoint ptTop = windowsBox.TopRight() ;
  pDc->MoveTo(ptTop) ;
  NS_CLASSLIB::TPoint ptBottom = windowsBox.BottomRight() ;
  pDc->LineTo(ptBottom) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvCurveYAxis::drawAxis.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

//
// Return 0 for _dLowAxisValue and lYAxisPixelRange for _dHighAxisValue
//
int
NSLdvCurveYAxis::giveYCoordinateForValue(double dValue, bool bLimitedToAxisLimits)
{
	double dValueToCompute = dValue ;

  // Special cases
  //
  if (dValueToCompute == numeric_limits<double>::max())
  {
  	if (bLimitedToAxisLimits)
    	dValueToCompute = _dHighAxisValue ;
    else
			return numeric_limits<int>::max() ;
	}
  if (dValue == - numeric_limits<double>::max())
	{
  	if (bLimitedToAxisLimits)
    	dValueToCompute = _dLowAxisValue ;
    else
			return - numeric_limits<int>::max() ;
  }

  if (bLimitedToAxisLimits)
  {
  	if (dValueToCompute < _dLowAxisValue)
    	dValueToCompute = int(_dLowAxisValue) ;
    if (dValueToCompute > _dHighAxisValue)
      dValueToCompute = int(_dHighAxisValue) ;
  }

  double dDeltaScale = _dHighAxisValue - _dLowAxisValue ;
  //
  // Since we will divide by dDeltaScale, better check it is != 0
  //
  if (dDeltaScale == double(0))
  	return 0 ;

  double dDeltaValue = dValueToCompute - _dLowAxisValue ;

	// int    iDeltaY         = pCurve->getCurvesPannel()->getYAxisPixelRange() ;
  NS_CLASSLIB::TRect windowsBox = donneVisibleRectangle() ;
  int    iDeltaY         = windowsBox.Height() ;
  int    iDeltaYForValue = iDeltaY * dDeltaValue / dDeltaScale ;

  return iDeltaYForValue ;
}

int
NSLdvCurveYAxis::giveWorkingAreaYCoordinateForValue(double dValue, bool bLimitedToAxisLimits)
{
  int iInternalCoordinate = giveYCoordinateForValue(dValue, bLimitedToAxisLimits) ;

  if ((NULL == _pInterface) || (false == _pInterface->IsWindow()) || (NULL == _pParent))
    return iInternalCoordinate ;

  int iAreaCoordinate = _pParent->getYValueFromChildYValue(iInternalCoordinate, _pInterface) ;

  return iAreaCoordinate ;
}

void
NSLdvCurveYAxis::initAxisValues(NSLdvCurvePoint* pFirstPoint)
{
  if (NULL == pFirstPoint)
    return ;

	_dLowActualValue = numeric_limits<double>::max() ;
  // _dHighAxisValue = numeric_limits<double>::min() ; // this give the lower exponent (E-x)
  _dHigActualValue = - numeric_limits<double>::max() ;

  NSLdvCurvePoint* pPoint = pFirstPoint ;

	while (pPoint)
  {
    if (pPoint->_dValue > _dHigActualValue)
    	_dHigActualValue = pPoint->_dValue ;
    if (pPoint->_dValue < _dLowActualValue)
    	_dLowActualValue = pPoint->_dValue ;

    pPoint = pPoint->getNextBrother() ;
  }

  // To be modified
  //
  _dLowAxisValue  = _dLowActualValue ;
  _dHighAxisValue = _dHigActualValue ;

  // Taking the background into account (background interval plus 10% margin)
  //
  NSLdvCurveBackground* pCurveBackground = pCurve->getBackground() ;

  if (pCurveBackground &&
      ((pCurveBackground->_bExistMinLimit || pCurveBackground->_bExistMaxLimit)))
  {
  	double dBgMargin = double(0) ;
    if (pCurveBackground->_bExistMinLimit && pCurveBackground->_bExistMaxLimit)
    	dBgMargin = 0.1 * (pCurveBackground->_dMaxLimit - pCurveBackground->_dMinLimit) ;
    else
		{
    	double dBgMinMargin = double(0) ;
      double dBgMaxMargin = double(0) ;
    	if (pCurveBackground->_bExistMinLimit)
      	dBgMinMargin = 0.1 * fabs(pCurveBackground->_dMinLimit) ;
      if (pCurveBackground->_bExistMaxLimit)
      	dBgMaxMargin = 0.1 * fabs(pCurveBackground->_dMaxLimit) ;

      dBgMargin = dBgMinMargin + dBgMaxMargin ;
    }

  	if (pCurveBackground->_bExistMinLimit)
    {
    	double dBgLowLowLimit = pCurveBackground->_dMinLimit - dBgMargin ;
      if (dBgLowLowLimit < _dLowAxisValue)
    		_dLowAxisValue = dBgLowLowLimit ;

      double dBgLowHighLimit = pCurveBackground->_dMinLimit + dBgMargin ;
      if (dBgLowHighLimit > _dHighAxisValue)
    		_dHighAxisValue = dBgLowHighLimit ;
    }

		if (pCurveBackground->_bExistMaxLimit)
    {
    	double dBgHighHighLimit = pCurveBackground->_dMaxLimit + dBgMargin ;
      if (dBgHighHighLimit > _dHighAxisValue)
    		_dHighAxisValue = dBgHighHighLimit ;

      double dBgHighLowLimit = pCurveBackground->_dMaxLimit - dBgMargin ;
      if (dBgHighLowLimit < _dLowAxisValue)
    		_dLowAxisValue = dBgHighLowLimit ;
    }
  }

	adjustAxisMinMaxValues(_dLowAxisValue, _dHighAxisValue) ;

  computeTicks(_dLowAxisValue, _dHighAxisValue, 20) ;
}

void
NSLdvCurveYAxis::adjustAxisMinMaxValues(double& axisMinVal, double& axisMaxVal)
{
	if ((axisMinVal == double(0)) && (axisMaxVal == double(0)))
  {
  	_dHighAxisValue = 1 ;
    return ;
  }

  double dCommonValue ;
  int    iCommon10log ;
  bool bIsCommonValue = getCommonValue(axisMinVal, axisMaxVal, dCommonValue, iCommon10log) ;

  	//
    // Example : min = 1.345 max = 1.352
    //           -> commonValue 1.3 common10log -1
    //           roundDown -> minAxis = 1.3 roundUp -> maxAxis = 1.4
    //           nice
    //
    // Example : min = 90 max = 95
    //           -> commonValue 90 common10log 1
    //           roundDown -> minAxis = 90 roundUp -> maxAxis = 100
    //           not nice because min values are stuck to the bottom line
    //           so, remove one power10 of common10log
    //           -> minAxis = 80 maxAxis = 100 : nice
    //

  if (!bIsCommonValue)
  	//
    // Example : min = 1.345 max = 2.512
    //           -> commonValue 1.3 common10log -1
    //           roundDown -> minAxis = 1.3 roundUp -> maxAxis = 1.4
    //           nice
    //
    iCommon10log = min(getPrecision(axisMinVal), getPrecision(axisMaxVal)) ;

  double dRoundedValue ;

  dRoundedValue = roundDown(axisMinVal, -iCommon10log) ;
  if (axisMinVal == dRoundedValue)
  	axisMinVal -= pow(double(10), double(iCommon10log)) ;
  else
  	axisMinVal = dRoundedValue ;

  dRoundedValue = roundUp(axisMaxVal, -iCommon10log) ;
  if (axisMaxVal == dRoundedValue)
  	axisMaxVal += pow(double(10), double(iCommon10log)) ;
  else
  	axisMaxVal = dRoundedValue ;

  if (_bIncludeZero)
  {
  	if (axisMinVal > 0)
    	axisMinVal = 0 ;
    else if (axisMaxVal < 0)
    	axisMaxVal = 0 ;
  }

  return ;
}

void
NSLdvCurveYAxis::emptyTicks()
{
	if (aTicks.empty())
		return ;

	vector<string>::iterator iter;
	for (iter = aTicks.begin(); iter != aTicks.end();)
  	aTicks.erase(iter) ;
}

void
NSLdvCurveYAxis::computeTicks(double ticMinVal, double ticMaxVal, int maxTicks)
{
	emptyTicks() ;

  if (ticMaxVal <= ticMinVal)
		return ;

	double xStep = quantaRoundUp((ticMaxVal - ticMinVal) / maxTicks) ;
	int numfracdigits = numFracDigits(xStep) ;

	// Compute x starting point so it is a multiple of xStep.
	double xStart = xStep * ceil(ticMinVal / xStep) ;

	// Label the axis.  The labels are quantized so that
	// they don't have excess resolution.
	for (double xpos = xStart; xpos <= ticMaxVal; xpos += xStep)
  	aTicks.push_back(DoubleToString(&xpos, 0, numfracdigits)) ;
}

//
// Given a number, round up to the nearest power of ten
// times 1, 2, or 5.
//
// Note: The argument must be strictly positive.
//
double
NSLdvCurveYAxis::quantaRoundUp(double val)
{
	int exponent = (int) floor(log10(val)) ;

  val *= pow(10, -exponent) ;
  if (val > 5.0)
  	val = 10.0 ;
	else if (val > 2.0)
  	val = 5.0 ;
	else if (val > 1.0)
		val = 2.0 ;

	val *= pow(10, exponent) ;
	return val ;
}

//
// Return the number of fractional digits required to display the
// given number.  No number larger than 15 is returned (if
// more than 15 digits are required, 15 is returned).
//
int
NSLdvCurveYAxis::numFracDigits(double num)
{
	int numdigits = 0 ;
  while ((numdigits <= 15) && (num != floor(num)))
	{
		num *= 10.0 ;
		numdigits += 1 ;
	}
	return numdigits ;
}

//
// Return the common value for val1 and val2
// ie if val1 = 1.23 and val2 = 1.25 then common = 1.2
//
// iCommon10log is limited to -15
//
bool
NSLdvCurveYAxis::getCommonValue(double val1, double val2, double& common, int& iCommon10log)
{
	if (val1 == val2)
	{
  	common = val1 ;
    iCommon10log = getPrecision(common) + 1 ;
    return true ;
  }

	double dMax = max(val1, val2) ;
  double dMin = min(val1, val2) ;

  if ((dMax > double(0)) && (dMin < double(0)))
  	return false ;

  double dLog = log10(dMax) ;
  int	   iLog = int(dLog) ;
  double dMult = pow(double(10), double(iLog)) ;
  double dResuMax ;
  double dResuMin ;

  bool   bResult = false ;

  common = double(0) ;

  // Comparing the integer value
  //
  while (iLog >= -15)
  {
    dResuMax = floor(dMax / dMult) ;
    dResuMin = floor(dMin / dMult) ;

    if (dResuMax == dResuMin)
    {
    	bResult = true ;

    	common = dResuMax * dMult ;
      iCommon10log = iLog ;

      dMax = dMax - (dResuMax * dMult) ;
      dMin = dMin - (dResuMin * dMult) ;

    	dMult = dMult / 10 ;
    	iLog-- ;
    }
    else
    	return bResult ;
  }
  return bResult ;
}

int
NSLdvCurveYAxis::getPrecision(double dVal)
{
	int    iPrecision = 0 ;
  double dTruncatedVal = trunc(dVal, 0) ;

  // Means it is an integer : look for the number of zeros
  //
  if (dTruncatedVal == dVal)
  {
  	while ((iPrecision > -15) && (dTruncatedVal == dVal))
    {
    	iPrecision-- ;
      dTruncatedVal = trunc(dVal, iPrecision) ;
    }
    iPrecision++ ;
  }
  //
  // Means it in the forme X.Y: look for the precision
  //
  else
  {
  	while ((iPrecision < 15) && (dTruncatedVal == dVal))
    {
    	iPrecision++ ;
      dTruncatedVal = trunc(dVal, iPrecision) ;
    }
    iPrecision-- ;
  }

  return iPrecision ;
}

//
// Rounds the given number up to the supplied number of decimal places
//
// roundUp(76.9,0) returns 77
// roundUp(31415.92654, -2) returns 31500
//
double
NSLdvCurveYAxis::roundUp(double dVal, int iPrecision)
{
	return ceil(dVal * pow(double(10), double(iPrecision))) / pow(double(10), double(iPrecision)) ;
}

//
// Rounds a number down
//
// ROUNDDOWN(3.14159, 3) returns 31.141
// ROUNDDOWN(31415.92654, -2) returns 31.400
//
double
NSLdvCurveYAxis::roundDown(double dVal, int iPrecision)
{
	return floor(dVal * pow(double(10), double(iPrecision))) / pow(double(10), double(iPrecision)) ;
}

//
// Removes the fractional part of a number to the specified precision
// without rounding the number
//
// TRUNC(123.456, 2) returns 123.45
// TRUNC(9899.435, –2) returns 9800
//
double
NSLdvCurveYAxis::trunc(double dVal, int iPrecision)
{
	int iVal = int(dVal * pow(double(10), double(iPrecision))) ;
	return double(iVal) / pow(double(10), double(iPrecision)) ;
}

NSLdvCurveYAxis&
NSLdvCurveYAxis::operator=(const NSLdvCurveYAxis& src)
{
	if (this == &src)
	  return *this ;

  pCurve          = src.pCurve ;

  _dLowActualValue = src._dLowActualValue ;
  _dHigActualValue = src._dHigActualValue ;

	_dLowAxisValue   = src._dLowAxisValue ;
  _dHighAxisValue  = src._dHighAxisValue ;

  _bIncludeZero    = src._bIncludeZero ;

  return *this ;
}

// -----------------------------------------------------------------------------
// ----------------- METHODES DE NSLdvCurveYAxisInterface --------------------
// -----------------------------------------------------------------------------

NSLdvCurveYAxisInterface::NSLdvCurveYAxisInterface(NSContexte* pCtx, TWindow* parent, NSLdvCurveYAxis* pToon)
                         :TWindow(parent), NSRoot(pCtx)
{
  lObjectCount++ ;

  _pToon = pToon ;

  SetCaption(_pToon->_sSkinName.c_str()) ;
}

NSLdvCurveYAxisInterface::NSLdvCurveYAxisInterface(const NSLdvCurveYAxisInterface& rv)
                         :TWindow(rv.GetParent()), NSRoot(rv.pContexte)
{
  lObjectCount++ ;

  _pToon = rv.getToon() ;
}

NSLdvCurveYAxisInterface::~NSLdvCurveYAxisInterface()
{
  lObjectCount-- ;
}

void
NSLdvCurveYAxisInterface::SetupWindow()
{
  TWindow::SetupWindow() ;
}

void
NSLdvCurveYAxisInterface::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  _pToon->draw(&dc, &RectAPeindre) ;
}

void
NSLdvCurveYAxisInterface::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  TWindow::EvRButtonDown(modKeys, point) ;
}

NSLdvCurveYAxisInterface&
NSLdvCurveYAxisInterface::operator=(const NSLdvCurveYAxisInterface& src)
{
  if (this == &src)
    return *this ;

  _pToon = src.getToon() ;

  return *this ;
}

// -----------------------------------------------------------------------------
//                       NSLdvCurveBackgroundQuadri
// -----------------------------------------------------------------------------

NSLdvCurveBackgroundQuadri::NSLdvCurveBackgroundQuadri(NSLdvView* pVw)
{
  lObjectCount++ ;

	_pView = pVw ;

  color = NS_CLASSLIB::TColor(NS_CLASSLIB::TColor::White) ;

	_tpsLowerLeft.init() ;
	_dValueLowerLeft    = double(0) ;
  _sUnitSensLowerLeft = string("") ;

  _tpsLowerRight.init() ;
	_dValueLowerRight    = double(0) ;
	_sUnitSensLowerRight = string("") ;

	_tpsUpperLeft.init() ;
	_dValueUpperLeft    = double(0) ;
	_sUnitSensUpperLeft = string("") ;

  _tpsUpperRight.init() ;
	_dValueUpperRight    = double(0) ;
	_sUnitSensUpperRight = string("") ;
}

NSLdvCurveBackgroundQuadri::NSLdvCurveBackgroundQuadri(NSLdvView* pVw, NS_CLASSLIB::TColor qColor, NVLdVTemps beginDate, NVLdVTemps endDate, double lowerValue, string lowerUnit, double upperValue, string upperUnit)
{
  lObjectCount++ ;

	_pView = pVw ;

  color = qColor ;

	string sLowerUnitSens ;
  _pView->pContexte->getDico()->donneCodeSens(&lowerUnit, &sLowerUnitSens) ;

  _tpsLowerLeft        = beginDate ;
	_dValueLowerLeft     = lowerValue ;
  _sUnitSensLowerLeft  = sLowerUnitSens ;

  _tpsLowerRight       = endDate ;
	_dValueLowerRight    = lowerValue ;
	_sUnitSensLowerRight = sLowerUnitSens ;

  string sUpperUnitSens ;
  _pView->pContexte->getDico()->donneCodeSens(&upperUnit, &sUpperUnitSens) ;

	_tpsUpperLeft        = beginDate ;
	_dValueUpperLeft     = upperValue ;
	_sUnitSensUpperLeft  = sUpperUnitSens ;

  _tpsUpperRight       = endDate ;
	_dValueUpperRight    = upperValue ;
	_sUnitSensUpperRight = sUpperUnitSens ;
}

NSLdvCurveBackgroundQuadri::~NSLdvCurveBackgroundQuadri()
{
  lObjectCount-- ;
}

NSLdvCurveBackgroundQuadri::NSLdvCurveBackgroundQuadri(const NSLdvCurveBackgroundQuadri& rv)
{
  lObjectCount++ ;

	_pView = rv._pView ;

  color = rv.color ;

  _tpsLowerLeft        = rv._tpsLowerLeft ;
	_dValueLowerLeft     = rv._dValueLowerLeft ;
  _sUnitSensLowerLeft  = rv._sUnitSensLowerLeft ;

  _tpsLowerRight       = rv._tpsLowerRight ;
	_dValueLowerRight    = rv._dValueLowerRight ;
	_sUnitSensLowerRight = rv._sUnitSensLowerRight ;

	_tpsUpperLeft        = rv._tpsUpperLeft ;
	_dValueUpperLeft     = rv._dValueUpperLeft ;
	_sUnitSensUpperLeft  = rv._sUnitSensUpperLeft ;

  _tpsUpperRight       = rv._tpsUpperRight ;
	_dValueUpperRight    = rv._dValueUpperRight ;
	_sUnitSensUpperRight = rv._sUnitSensUpperRight ;
}

NSLdvCurveBackgroundQuadri&
NSLdvCurveBackgroundQuadri::operator=(const NSLdvCurveBackgroundQuadri& src)
{
	if (this == &src)
		return *this ;

	_pView = src._pView ;

  color = src.color ;

  _tpsLowerLeft        = src._tpsLowerLeft ;
	_dValueLowerLeft     = src._dValueLowerLeft ;
  _sUnitSensLowerLeft  = src._sUnitSensLowerLeft ;

  _tpsLowerRight       = src._tpsLowerRight ;
	_dValueLowerRight    = src._dValueLowerRight ;
	_sUnitSensLowerRight = src._sUnitSensLowerRight ;

	_tpsUpperLeft        = src._tpsUpperLeft ;
	_dValueUpperLeft     = src._dValueUpperLeft ;
	_sUnitSensUpperLeft  = src._sUnitSensUpperLeft ;

  _tpsUpperRight       = src._tpsUpperRight ;
	_dValueUpperRight    = src._dValueUpperRight ;
	_sUnitSensUpperRight = src._sUnitSensUpperRight ;

  return *this ;
}

// -----------------------------------------------------------------------------
//                       NSLdvCurveBackground
// -----------------------------------------------------------------------------

NSLdvCurveBackground::NSLdvCurveBackground(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSLdvCurve* pTheCurve)
                     :NSLdvTimeToon(pCtx, pWorkArea, true)
{
  lObjectCount++ ;

	_iZOrder  = LEVEL_BACKGROUND ;
  _toonType = toonBackground ;

	_pCurve   = pTheCurve ;

	_bExistMaxLimit = false ;
	_dMaxLimit      = double(0) ;
	_sMaxLimitUnit  = string("") ;

	_bExistMinLimit = false ;
	_dMinLimit      = double(0) ;
	_sMinLimitUnit  = string("") ;

  _bInterceptRButtonDown = false ;
}

NSLdvCurveBackground::NSLdvCurveBackground(const NSLdvCurveBackground& rv)
                     :NSLdvTimeToon(rv.pContexte, rv._pWorkingArea, rv._bVerticalScrollEnabled)
{
  lObjectCount++ ;

	_pCurve   = rv._pCurve ;
  _aQuadris = rv._aQuadris ;

  _bExistMaxLimit = rv._bExistMaxLimit ;
	_dMaxLimit      = rv._dMaxLimit ;
	_sMaxLimitUnit  = rv._sMaxLimitUnit ;

	_bExistMinLimit = rv._bExistMinLimit ;
	_dMinLimit      = rv._dMinLimit ;
	_sMinLimitUnit  = rv._sMinLimitUnit ;
}

NSLdvCurveBackground::~NSLdvCurveBackground()
{
  lObjectCount-- ;
}

void
NSLdvCurveBackground::SetupWindow()
{
}

void
NSLdvCurveBackground::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal)
{
  NSLdvViewArea* pWorkArea = _pView->getActiveWorkingArea() ;
  if (NULL == pWorkArea)
    return ;

	NS_CLASSLIB::TRect screenRect = pWorkArea->getScrollablePhysicalRect(*pRectARepeindre) ;

  draw(pDc, &screenRect) ;
}

void
NSLdvCurveBackground::draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
	drawQuadris(pDc, pRectARepeindre) ;
}

void
NSLdvCurveBackground::EvRButtonDown()
{
}

void
NSLdvCurveBackground::drawQuadris(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
	if (_aQuadris.empty())
  	return ;

	for (NSLdvQuadriIter QIt = _aQuadris.begin() ; _aQuadris.end() != QIt ; QIt++ )
  	drawAQuadri(pDc, pRectARepeindre, *QIt) ;
}

TRegion
NSLdvCurveBackground::getQuadriAsARegion(NSLdvCurveBackgroundQuadri* pQuadri, bool* pSuccess)
{
	if (pSuccess)
  	*pSuccess = true ;

	// We ask for Y values limited to axis' reference frame
  //
	bool bSuccess ;
  int iYforLowerLeft  = _pCurve->giveYCoordinateForValue(pQuadri->_dValueLowerLeft,  &bSuccess, pQuadri->_sUnitSensLowerLeft, true) ;
  int iYforUpperLeft  = _pCurve->giveYCoordinateForValue(pQuadri->_dValueUpperLeft,  &bSuccess, pQuadri->_sUnitSensUpperLeft, true) ;
  int iYforUpperRight = _pCurve->giveYCoordinateForValue(pQuadri->_dValueUpperRight, &bSuccess, pQuadri->_sUnitSensUpperRight, true) ;
  int iYforLowerRight = _pCurve->giveYCoordinateForValue(pQuadri->_dValueLowerRight, &bSuccess, pQuadri->_sUnitSensLowerRight, true) ;

  if ((bSuccess == false) || ((iYforLowerLeft == iYforUpperLeft) && (iYforUpperRight == iYforLowerRight)))
  {
  	if (pSuccess)
  		*pSuccess = false ;
    return TRegion() ;
  }

	NS_CLASSLIB::TPoint aQuadPts[5] ;

  NSLdvViewArea* pWorkArea = _pView->getActiveWorkingArea() ;
  if (NULL == pWorkArea)
    return TRegion(aQuadPts, 5, ALTERNATE) ;

  NVLdVPoint ldvPoint(_pView) ;

  ldvPoint.setX(pQuadri->_tpsLowerLeft) ;
  ldvPoint.setY(iYforLowerLeft) ;
  aQuadPts[0] = pWorkArea->getScrollablePhysicalPoint(ldvPoint) ;

  ldvPoint.setX(pQuadri->_tpsUpperLeft) ;
  ldvPoint.setY(iYforUpperLeft) ;
  aQuadPts[1] = pWorkArea->getScrollablePhysicalPoint(ldvPoint) ;

  ldvPoint.setX(pQuadri->_tpsUpperRight) ;
  ldvPoint.setY(iYforUpperRight) ;
  aQuadPts[2] = pWorkArea->getScrollablePhysicalPoint(ldvPoint) ;

  ldvPoint.setX(pQuadri->_tpsLowerRight) ;
  ldvPoint.setY(iYforLowerRight) ;
  aQuadPts[3] = pWorkArea->getScrollablePhysicalPoint(ldvPoint) ;

  aQuadPts[4] = aQuadPts[0] ;

  return TRegion(aQuadPts, 5, ALTERNATE) ;
}

void
NSLdvCurveBackground::drawAQuadri(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NSLdvCurveBackgroundQuadri* pQuadri)
{
try
{
	if (NULL == pDc)
		return ;

	bool bSuccess ;
	TRegion region = getQuadriAsARegion(pQuadri, &bSuccess) ;

  if (false == bSuccess)
		return ;

	int iAncDC = pDc->SaveDC() ;
  pDc->FillRgn(region, pQuadri->color) ;
  pDc->RestoreDC(iAncDC) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvCurveBackground::drawAQuadri.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvCurveBackground::addQuadri(NSLdvCurveBackgroundQuadri* pQuadri)
{
	_aQuadris.push_back(new NSLdvCurveBackgroundQuadri(*pQuadri)) ;
}

void
NSLdvCurveBackground::eraseQuadri()
{
	_aQuadris.vider() ;
}

NSLdvCurveBackground&
NSLdvCurveBackground::operator=(const NSLdvCurveBackground& src)
{
	if (this == &src)
		return *this ;

	_pCurve         = src._pCurve ;
  _aQuadris       = src._aQuadris ;

	_bExistMaxLimit = src._bExistMaxLimit ;
	_dMaxLimit      = src._dMaxLimit ;
	_sMaxLimitUnit  = src._sMaxLimitUnit ;

	_bExistMinLimit = src._bExistMinLimit ;
	_dMinLimit      = src._dMinLimit ;
	_sMinLimitUnit  = src._sMinLimitUnit ;

  return *this ;
}

// -----------------------------------------------------------------------------
//                       NSLdvCurvesManagementPannel
// -----------------------------------------------------------------------------

const int ID_CurvesList = 0x100 ;

NSLdvCurvesManagementPannel::NSLdvCurvesManagementPannel(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvView* pView)
                            :NSLdvWindowToon(pCtx, pLdvParent, pView)
{
  lObjectCount++ ;

	_iZOrder   = LEVEL_COMMANDS ;
  _toonType  = toonCurvePannel ;
  // toonShape = toonSizeLimited ;
  _sSkinName = string("LdvCurvesManagementPannel") ;

  initDimensions() ;

  if (!initBoxFromSkin())
  {
  	NS_CLASSLIB::TRect wRect = _pView->GetClientRect() ;
  	NS_CLASSLIB::TRect pannelRect = _pView->getDateTimeArea() ;
		// wVisibleBox = NS_CLASSLIB::TRect(600, 0, 800, pannelRect.Height()) ;
  }

  _pInterface = new NSLdvCurvesManagementPannelInterface(pContexte, getParentInterface(), this) ;

  _pCurvesListManager = new NSCurvesProperty(pContexte, this, _pView) ;

  iSortedColumn = -1 ;
  bNaturallySorted = false ;
}

NSLdvCurvesManagementPannel::NSLdvCurvesManagementPannel(const NSLdvCurvesManagementPannel& rv)
                            :NSLdvWindowToon(rv.pContexte, rv.getToonParent(), rv._pView)
{
  lObjectCount++ ;

	_sSkinName = rv._sSkinName ;

	initialiser((NSLdvWindowToon*)(&rv)) ;
}


NSLdvCurvesManagementPannel::~NSLdvCurvesManagementPannel()
{
  lObjectCount-- ;
}

void
NSLdvCurvesManagementPannel::setScrollParams()
{
}

void
NSLdvCurvesManagementPannel::CmNew()
{
try
{
	NSLdvCurve* pNewCurve = new NSLdvCurve(this) ;
  searchNextCurveParams(&(pNewCurve->_color), &(pNewCurve->_pointAspect), &(pNewCurve->_pointRadius), &(pNewCurve->_dotWidth)) ;

	NSNewCurveDlg* pNewCurvDialog = new NSNewCurveDlg(_pView, pContexte, pNewCurve) ;
  int iExecReturn = pNewCurvDialog->Execute() ;
  delete pNewCurvDialog ;

	if (IDOK != iExecReturn)
  {
  	delete pNewCurve ;
    return ;
  }

  NSLdvViewArea* pActiveWorkingArea = _pView->getActiveWorkingArea() ;
  if (pActiveWorkingArea)
  {
    if (pNewCurve->getYAxis() && pNewCurve->getYAxis()->getInterface())
      pNewCurve->getYAxis()->getInterface()->Create() ;
    _pView->EvaluateWinToonsPos(pActiveWorkingArea->getInterface(), pActiveWorkingArea->getToonsArray()) ;

    _pView->addToToonsArray(pActiveWorkingArea->getToonsArray(), pNewCurve->getBackground()) ;
  }

  pNewCurve->init() ;

  _aCurves.push_back(pNewCurve) ;

  _pCurvesListManager->displayElement(pNewCurve) ;
  CmPutUp(pNewCurve->getCompoundLabel()) ;

  _pView->Invalidate(true) ;
}
catch (...)
{
	erreur("Exception NSLdvCurvesManagementPannel::CmNew", standardError, 0) ;
}
}

bool
NSLdvCurvesManagementPannel::CmSupres(string sLabelToSuppress)
{
	if (string("") == sLabelToSuppress)
		return false ;

  if (_aCurves.empty())
		return false ;

  ArrayOfCurvesIter curveIter = _aCurves.begin() ;
  for (; (_aCurves.end() != curveIter) && ((*curveIter)->getCompoundLabel() != sLabelToSuppress); curveIter++) ;

  if (_aCurves.end() == curveIter)
  	return false ;

	// Suppress the curve
  //
  (*curveIter)->close() ;
  delete *curveIter ;
  _aCurves.erase(curveIter) ;

  // PutUp next curve
  //
  if (false == _aCurves.empty())
  {
  	if (_aCurves.end() == curveIter)
    	curveIter-- ;
    CmPutUp((*curveIter)->getCompoundLabel()) ;
  }
  else
  	_pView->Invalidate(true) ;

  return true ;
}

void
NSLdvCurvesManagementPannel::CmPutUp(string sLabelToPutUp)
{
	if (string("") == sLabelToPutUp)
		return ;

  if (_aCurves.empty())
		return ;

	ArrayOfCurvesIter curveIter = _aCurves.begin() ;
  for (; _aCurves.end() != curveIter ; curveIter++)
  {
  	if ((*curveIter)->getCompoundLabel() == sLabelToPutUp)
    {
    	(*curveIter)->getYAxis()->setVisible() ;
      (*curveIter)->getBackground()->setVisible() ;
    }
    else
    {
    	(*curveIter)->getYAxis()->setHidden() ;
      (*curveIter)->getBackground()->setHidden() ;
    }
  }

  _pCurvesListManager->selectItem(sLabelToPutUp) ;

  _pView->Invalidate(true) ;
}

void
NSLdvCurvesManagementPannel::CmModify(string sLabelToModify)
{
	if (string("") == sLabelToModify)
		return ;

  if (_aCurves.empty())
		return ;

	ArrayOfCurvesIter curveIter = _aCurves.begin() ;
  for (; (_aCurves.end() != curveIter) && ((*curveIter)->getCompoundLabel() != sLabelToModify); curveIter++) ;

  if (_aCurves.end() == curveIter)
  	return ;

  string sPreviousUnitSens ;
  pContexte->getDico()->donneCodeSens(&((*curveIter)->getUnit()), &sPreviousUnitSens) ;

  NSNewCurveDlg* pNewCurvDialog = new NSNewCurveDlg(_pView, pContexte, *curveIter, true) ;
  int iExecReturn = pNewCurvDialog->Execute() ;
  delete pNewCurvDialog ;

  if (IDOK != iExecReturn)
    return ;

  string sNewUnitSens ;
  pContexte->getDico()->donneCodeSens(&((*curveIter)->getUnit()), &sNewUnitSens) ;

  (*curveIter)->reinit(sPreviousUnitSens != sNewUnitSens) ;

  _pView->Invalidate(true) ;
}

void
NSLdvCurvesManagementPannel::autoCreateNewCurve(string sLexiqueCode)
{
	if (sLexiqueCode == string(""))
		return ;

	// If a curve already exists, it is not useful to draw another one
  //
	NSLdvCurve* pExistingCurve = searchExistingCurveForThisConcept(sLexiqueCode) ;
  if (pExistingCurve)
		return ;

	NSLdvCurve* pNewCurve = new NSLdvCurve(this) ;

	pNewCurve->setItemValue(sLexiqueCode) ;

  searchNextCurveParams(&(pNewCurve->_color), &(pNewCurve->_pointAspect), &(pNewCurve->_pointRadius), &(pNewCurve->_dotWidth)) ;

  pNewCurve->init() ;

  // If there is no point, we don't add it to the list
  //
  if (NULL == pNewCurve->getFirstPoint())
  {
    delete pNewCurve ;
    return ;
  }

  _aCurves.push_back(pNewCurve) ;

  NSLdvViewArea* pActiveWorkingArea = _pView->getActiveWorkingArea() ;
  if (pActiveWorkingArea)
  {
    if (pNewCurve->getYAxis() && pNewCurve->getYAxis()->getInterface())
      pNewCurve->getYAxis()->getInterface()->Create() ;
    _pView->EvaluateWinToonsPos(pActiveWorkingArea->getInterface(), pActiveWorkingArea->getToonsArray()) ;

    _pView->addToToonsArray(pActiveWorkingArea->getToonsArray(), pNewCurve->getBackground()) ;
  }

  _pCurvesListManager->displayElement(pNewCurve) ;
  CmPutUp(pNewCurve->getCompoundLabel()) ;

  _pView->Invalidate(true) ;
}

void
NSLdvCurvesManagementPannel::searchNextCurveParams(NS_CLASSLIB::TColor* pColor, int* pPtAspect, int* pPtRadius, int* pLineWidth)
{
  if (pColor)
    findNextColor(pColor) ;

  if (pPtAspect)
    *pPtAspect = NSLdvCurve::paCircle ;

  if (pPtRadius)
    *pPtRadius = findCommonPointRadius() ;

  if (pLineWidth)
    *pLineWidth = findCommonLineWidth() ;
}

NSLdvCurve*
NSLdvCurvesManagementPannel::searchExistingCurveForThisConcept(string sLexique)
{
	if (_aCurves.empty())
		return NULL ;

	string sCodeSens ;
	pContexte->getDico()->donneCodeSens(&sLexique, &sCodeSens) ;

  ArrayOfCurvesIter curveIter = _aCurves.begin() ;
  for ( ; _aCurves.end() != curveIter ; curveIter++)
  {
  	string sCurveLex = (*curveIter)->getItemValue() ;
    string sCurveLexSens ;
		pContexte->getDico()->donneCodeSens(&sCurveLex, &sCurveLexSens) ;

    if (sCurveLexSens == sCodeSens)
    	return *curveIter ;
  }

  return NULL ;
}

NSLdvCurve*
NSLdvCurvesManagementPannel::searchExistingCurveForThisColor(int iRed, int iGreen, int iBlue)
{
	if (_aCurves.empty())
		return (NSLdvCurve*) 0 ;

  ArrayOfCurvesIter curveIter = _aCurves.begin() ;
  for (; curveIter != _aCurves.end() ; curveIter++)
  	if (((*curveIter)->_color.Red()   == iRed)   &&
        ((*curveIter)->_color.Green() == iGreen) &&
        ((*curveIter)->_color.Blue()  == iBlue))
  		return *curveIter ;

	return (NSLdvCurve*) 0 ;
}

int
NSLdvCurvesManagementPannel::findCommonLineWidth()
{
  if (_aCurves.empty())
		return 2 ;

  int iMinLineWidth = 1000 ;
  int iMaxLineWidth = -1 ;

  ArrayOfCurvesIter curveIter = _aCurves.begin() ;
  for (; curveIter != _aCurves.end() ; curveIter++)
  {
    if ((*curveIter)->getDotWidth() > iMaxLineWidth)
      iMaxLineWidth = (*curveIter)->getDotWidth() ;

    if ((*curveIter)->getDotWidth() < iMinLineWidth)
      iMinLineWidth = (*curveIter)->getDotWidth() ;
  }

  return ((iMinLineWidth + iMaxLineWidth) / 2) ;
}

int
NSLdvCurvesManagementPannel::findCommonPointRadius()
{
  if (_aCurves.empty())
		return 3 ;

  int iMinRadius = 1000 ;
  int iMaxRadius = -1 ;

  ArrayOfCurvesIter curveIter = _aCurves.begin() ;
  for (; curveIter != _aCurves.end() ; curveIter++)
  {
    if ((*curveIter)->getDotWidth() > iMaxRadius)
      iMaxRadius = (*curveIter)->getPointRadius() ;

    if ((*curveIter)->getDotWidth() < iMinRadius)
      iMinRadius = (*curveIter)->getPointRadius() ;
  }

  return ((iMinRadius + iMaxRadius) / 2) ;
}

void
NSLdvCurvesManagementPannel::findNextColor(NS_CLASSLIB::TColor* pColor)
{
  // algorithm: we take 'legal' values as combination of RGB values in the range
	// 0, 51, 102, 153, 204, 255 (see http://en.wikipedia.org/wiki/RGB_color_model)
  //
	// We take a combination at random, then check if it is not already used
  // If already in use, we select a new random color

  int aColors[6] ;
  for (int i = 0; i < 6; i++)
  	aColors[i] = 51 * i ;

	int iRed   = 255 ;
  int iGreen = 255 ;
  int iBlue  = 255 ;

	bool bChooseColor = true ;
  while (bChooseColor)
  {
  	iRed   = aColors[random(6)] ;
    iGreen = aColors[random(6)] ;
    iBlue  = aColors[random(6)] ;

    // White is not allowed
    //
    if ((iRed != 255) || (iGreen != 255) || (iBlue != 255))
    {
    	NSLdvCurve* pColorCurve = searchExistingCurveForThisColor(iRed, iGreen, iBlue) ;
      if (NULL == pColorCurve)
      	bChooseColor = false ;
    }
  }
	*pColor = NS_CLASSLIB::TColor(iRed, iGreen, iBlue) ;
}

void
NSLdvCurvesManagementPannel::EvRButtonDown()
{
}

void
NSLdvCurvesManagementPannel::reinit()
{
  if (_aCurves.empty())
		return ;

  ArrayOfCurvesIter curveIter = _aCurves.begin() ;
  for (; _aCurves.end() != curveIter ; curveIter++)
    (*curveIter)->reset() ;
}

void
NSLdvCurvesManagementPannel::disconnectCurves()
{
  if (_aCurves.empty())
		return ;

  ArrayOfCurvesIter curveIter = _aCurves.begin() ;
  for (; _aCurves.end() != curveIter ; curveIter++)
    (*curveIter)->disconnect() ;
}

void
NSLdvCurvesManagementPannel::SetupWindow()
{
	// pListeWindow->MoveWindow(wVisibleBox, true) ;

  // Setting the lYAxisPixelRange value: space left between the main Panel and
  // the chronoLine
  //
  NS_CLASSLIB::TRect screenRect = _pView->GetClientRect() ;

  _lYAxisPixelRange = screenRect.Height() ;

  // NSLdvWindowToon* pPannel = pView->getLdVWinToon(toonPannel) ;
  // if (pPannel)
  //	lYAxisPixelRange -= pPannel->wVisibleBox.Height() ;

  // NSLdvWindowToon* pChrono = pView->getLdVWinToon(toonChronoLine) ;
  // if (pChrono)
  //	lYAxisPixelRange -= pChrono->wVisibleBox.Height() ;
}

void
NSLdvCurvesManagementPannel::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal)
{
  NS_CLASSLIB::TRect screenRect = _pView->getGlobalPhysicalRect(*pRectARepeindre) ;

  draw(pDc, &screenRect) ;
}

void
NSLdvCurvesManagementPannel::draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
	skinDraw(pDc, pRectARepeindre) ;
}

void
NSLdvCurvesManagementPannel::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
}

void
NSLdvCurvesManagementPannel::LVNColumnclick(TLwNotify& lwn)
{
}

void
NSLdvCurvesManagementPannel::sortByName()
{
}

void
NSLdvCurvesManagementPannel::sortByEnding()
{
}

void
NSLdvCurvesManagementPannel::sortByBegining()
{
}

// -----------------------------------------------------------------------------
// --------------- METHODES DE NSLdvCommandPannelInterface ------------------
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSLdvCurvesManagementPannelInterface, TWindow)
  EV_WM_RBUTTONDOWN,
END_RESPONSE_TABLE ;

NSLdvCurvesManagementPannelInterface::NSLdvCurvesManagementPannelInterface(NSContexte* pCtx, TWindow* parent, NSLdvCurvesManagementPannel* pToon)
                                     :TWindow(parent), NSRoot(pCtx)
{
  lObjectCount++ ;

  _pToon = pToon ;

  SetCaption(_pToon->_sSkinName.c_str()) ;
}

NSLdvCurvesManagementPannelInterface::NSLdvCurvesManagementPannelInterface(const NSLdvCurvesManagementPannelInterface& rv)
                                     :TWindow(rv.GetParent()), NSRoot(rv.pContexte)
{
  lObjectCount++ ;

  _pToon = rv.getToon() ;
}

NSLdvCurvesManagementPannelInterface::~NSLdvCurvesManagementPannelInterface()
{
  lObjectCount-- ;
}

void
NSLdvCurvesManagementPannelInterface::SetupWindow()
{
  TWindow::SetupWindow() ;
}

void
NSLdvCurvesManagementPannelInterface::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  _pToon->skinDraw(&dc, &RectAPeindre) ;

/*
  uint iChildrenNb = NumChildren() ;
  if (iChildrenNb <= 0)
    return ;

  TWindow* pChild = GetFirstChild() ;
  int i = 0 ;
  while ((NULL != pChild) && (i < iChildrenNb))
  {
    pChild->Paint(dc, erase, RectAPeindre) ;
    pChild = pChild->Next() ;
    i++ ;
  }
*/
}

void
NSLdvCurvesManagementPannelInterface::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  TWindow::EvRButtonDown(modKeys, point) ;
}

NSLdvCurvesManagementPannelInterface&
NSLdvCurvesManagementPannelInterface::operator=(const NSLdvCurvesManagementPannelInterface& src)
{
  if (this == &src)
    return *this ;

  _pToon = src.getToon() ;

  return *this ;
}

// -----------------------------------------------------------------------------
//                                 NSLdvCurve
// -----------------------------------------------------------------------------

NSLdvCurve::NSLdvCurve(NSLdvCurvesManagementPannel* pPannel)
{
  lObjectCount++ ;

	_pCurvesPannel  = pPannel ;

  _sItemLabel     = string("") ;
  _sUnitLabel     = string("") ;
  _sCompoundLabel = string("") ;

	_sItemValue     = string("") ;
	_sPath          = string("") ;
	_sUnit          = string("") ;

	_pFirstPoint    = (NSLdvCurvePoint*) 0 ;

  _color          = NS_CLASSLIB::TColor::Black ;
  _dotWidth       = 1 ;
  _dotStyle       = PS_SOLID ;

  _pointAspect    = paCircle ;
  _pointRadius    = 2 ;

  _bAdjustPointColorToValue = true ;

  _pAxis       = new NSLdvCurveYAxis(_pCurvesPannel->pContexte, _pCurvesPannel->getView()->getActiveWorkingArea(), _pCurvesPannel->getView(), this) ;
  _pBackground = new NSLdvCurveBackground(_pCurvesPannel->pContexte, _pCurvesPannel->getView()->getActiveWorkingArea(), this) ;
}

NSLdvCurve::NSLdvCurve(const NSLdvCurve& rv)
{
  lObjectCount++ ;

	_pCurvesPannel  = rv._pCurvesPannel ;

  _sItemLabel     = rv._sItemLabel ;
  _sUnitLabel     = rv._sUnitLabel ;
  _sCompoundLabel = rv._sCompoundLabel ;

	_sItemValue     = rv._sItemValue ;
	_sPath          = rv._sPath ;
	_sUnit          = rv._sUnit ;

	_pFirstPoint    = rv._pFirstPoint ;

  _color          = rv._color ;
  _dotWidth       = rv._dotWidth ;
  _dotStyle       = rv._dotStyle ;

  _pointAspect    = rv._pointAspect ;
  _pointRadius    = rv._pointRadius ;

  _bAdjustPointColorToValue = rv._bAdjustPointColorToValue ;

  if (rv._pAxis)
  	_pAxis = new NSLdvCurveYAxis(*(rv._pAxis)) ;
  else
  	_pAxis = (NSLdvCurveYAxis*) 0 ;

	if (rv._pBackground)
  	_pBackground = new NSLdvCurveBackground(*(rv._pBackground)) ;
  else
  	_pBackground = (NSLdvCurveBackground*) 0 ;
}

NSLdvCurve::~NSLdvCurve()
{
  lObjectCount-- ;

  NSLdvViewArea* pActiveWorkingArea = (NSLdvViewArea*) 0 ;
  if (_pAxis || _pBackground)
    pActiveWorkingArea = _pCurvesPannel->getView()->getActiveWorkingArea() ;

	if (_pAxis)
  {
    if (pActiveWorkingArea)
  	  _pCurvesPannel->getView()->removeFromToonsArray(pActiveWorkingArea->getToonsArray(), _pAxis) ;
    delete _pAxis ;
  }

  if (_pBackground)
  {
    if (pActiveWorkingArea)
  	  _pCurvesPannel->getView()->removeFromToonsArray(pActiveWorkingArea->getToonsArray(), _pBackground) ;
    delete _pBackground ;
  }
}

void
NSLdvCurve::init()
{
  NSContexte *pContexte = _pCurvesPannel->pContexte ;

	// Setting the labels
	//
  string sLang = pContexte->getUserLanguage() ;
  pContexte->getDico()->donneLibelle(sLang, &_sItemValue, &_sItemLabel) ;
  if (string("") != _sUnit)
  	pContexte->getDico()->donneLibelle(sLang, &_sUnit, &_sUnitLabel) ;
  setCompoundLabel() ;

	//
	// Getting history for this data
  //
	NSHistoryValManagementArray histoManager(pContexte) ;

  histoManager.ChargeHistoryValueItem(_sItemValue) ;
  histoManager.ChargeCaptureValueItem(_sItemValue) ;
  if (histoManager.empty())
  	return ;

	sort(histoManager.begin(), histoManager.end(), sortByDateInf) ;

  NSHistoryValManagementIter histoIt ;

  // If unit has not been chosen, we take the most selected
  //
  if (string("") == _sUnit)
  {
  	ClasseStringVector aUnitCounter ;
    iterClassString stringIt ;
  	for (histoIt = histoManager.begin(); histoManager.end() != histoIt ; histoIt++)
		{
    	// Unit counter
    	//
    	string sDataUnit = (*histoIt)->getUnit() ;
    	string sDataUnitSens ;
    	pContexte->getDico()->donneCodeSens(&sDataUnit, &sDataUnitSens) ;

    	if (false == aUnitCounter.empty())
    	{
    		stringIt = aUnitCounter.begin() ;
      	for (; (aUnitCounter.end() != stringIt) && ((*stringIt)->getItem() != sDataUnitSens) ; stringIt++) ;
      	if (stringIt == aUnitCounter.end())
      		aUnitCounter.push_back(new classString(sDataUnitSens, 1)) ;
      	else
      		(*stringIt)->setColumn((*stringIt)->getColumn() + 1) ;
    	}
    	else
    		aUnitCounter.push_back(new classString(sDataUnitSens, 1)) ;
    }

    int iCurrentCounter = 0 ;
  	for (stringIt = aUnitCounter.begin(); stringIt != aUnitCounter.end() ; stringIt++)
    {
    	if ((*stringIt)->getColumn() > iCurrentCounter)
      {
      	iCurrentCounter = (*stringIt)->getColumn() ;
        setUnit((*stringIt)->getItem() + string("1")) ;
      }
    }
  }

  // Creating the points as toons
  //
  NSLdvCurvePoint* pPreviousPoint = NULL ;

  NSLdvViewArea* pActiveWorkingArea = _pCurvesPannel->getView()->getActiveWorkingArea() ;
  if (NULL == pActiveWorkingArea)
    return ;

  // Creating the points
  //
  for (histoIt = histoManager.begin() ; histoIt != histoManager.end(); histoIt++)
	{
    NSLdvCurvePoint* pPoint = new NSLdvCurvePoint(pContexte, pActiveWorkingArea, this, *histoIt) ;

    if (NULL == pPreviousPoint)
    	_pFirstPoint = pPoint ;
    else
    {
    	pPreviousPoint->setNextBrother(pPoint) ;
      pPoint->_pPreviousBrother = pPreviousPoint ;
    }

    _pCurvesPannel->getView()->addToToonsArray(pActiveWorkingArea->getToonsArray(), pPoint) ;

    pPreviousPoint = pPoint ;
  }

  // Setting the points to current unit (and hidding them when not possible)
  //
  pointsSetUnit() ;

  // Setting background information
  //
  NSFrameInformationArray *pFrames = _pCurvesPannel->getView()->getDoc()->getFrames() ;
  if (pFrames)
  {
    for (int i = 0 ; i < FRAMECOUNT ; i++)
    {
      NSFrameInformation* pFrameInfo = pFrames->getFrameInformation(getFrameForIndex(i)) ;
      if (pFrameInfo)
        initHistoryOfValuedGoals(pFrameInfo->getGoalsArray()) ;
    }
  }

  buildBackgroundQuadrisFromGoals() ;

  // Initing the Axis
  //
  _pAxis->initAxisValues(_pFirstPoint) ;

  // Ending points initialization
  //
	pointsSetup() ;
}

void
NSLdvCurve::reset()
{
  // Delete existing points
  //
  close() ;

  init() ;
}

void
NSLdvCurve::reinit(bool bUnitChanged)
{
  disconnect() ;
  reconnect() ;

	if (bUnitChanged)
  	pointsSetUnit() ;

	// Initing the Axis
  //
  _pAxis->initAxisValues(_pFirstPoint) ;

  // Setting points according to changes
  //
  pointsSetup() ;

  // reinit background
  //
  _pBackground->eraseQuadri() ;
  buildBackgroundQuadrisFromGoals() ;
}

void
NSLdvCurve::disconnect()
{
  NSLdvView* pView = _pCurvesPannel->getView() ;
  if (NULL == pView)
    return ;

  NSLdvViewArea* pActiveWorkingArea = pView->getActiveWorkingArea() ;
  if (NULL == pActiveWorkingArea)
    return ;

  ArrayOfToons* pToonsArray = pActiveWorkingArea->getToonsArray() ;
  if (NULL == pToonsArray)
    return ;

  // Disconnect points
  //
  NSLdvCurvePoint* pPoint = _pFirstPoint ;
  while (pPoint)
  {
  	NSLdvCurvePoint* pNextPoint = pPoint->getNextBrother() ;
    if (pActiveWorkingArea)
  	  pView->removeFromToonsArray(pToonsArray, pPoint) ;
    pPoint = pNextPoint ;
  }

  // Disconnect components
  //
  if (_pBackground)
    pView->removeFromToonsArray(pToonsArray, _pBackground) ;

  if (_pAxis)
    pView->removeFromToonsArray(pToonsArray, _pAxis) ;
}

void
NSLdvCurve::reconnect()
{
  NSLdvView* pView = _pCurvesPannel->getView() ;
  if (NULL == pView)
    return ;

  NSLdvViewArea* pActiveWorkingArea = pView->getActiveWorkingArea() ;
  if (NULL == pActiveWorkingArea)
    return ;

  // Reconnect components
  //
  if (_pBackground)
    pView->addToToonsArray(pActiveWorkingArea->getToonsArray(), _pBackground) ;

  if (_pAxis)
    pView->addToToonsArray(pActiveWorkingArea->getToonsArray(), _pAxis) ;

  // Reconnect points
  //
  NSLdvCurvePoint* pPoint = _pFirstPoint ;
  while (pPoint)
  {
  	NSLdvCurvePoint* pNextPoint = pPoint->getNextBrother() ;
    if (pActiveWorkingArea)
  	  pView->addToToonsArray(pActiveWorkingArea->getToonsArray(), pPoint) ;
    pPoint = pNextPoint ;
  }
}

void
NSLdvCurve::pointsSetUnit()
{
	if (NULL == _pFirstPoint)
		return ;
  if (string("") == _sUnit)
  	return ;

  NSContexte *pContexte = _pCurvesPannel->pContexte ;
  NSSuper    *pSuper    = pContexte->getSuperviseur() ;

  string sNewUnitSens ;
  pContexte->getDico()->donneCodeSens(&_sUnit, &sNewUnitSens) ;

  bool bConvertOpen = false ;
  NSCV NsCv(pSuper) ;

  bool bOddConvert  = false ;
  bool bSomeUnconvertibleUnit = false ;

	NSLdvCurvePoint* pPoint = _pFirstPoint ;
  while (pPoint)
  {
  	if (sNewUnitSens != pPoint->_sUnitSens)
    {
    	// Main value with good unit
      //
    	if      (pPoint->_value.getUnit() == sNewUnitSens)
    	{
    		double dValue = StringToDouble(pPoint->_value.getValue()) ;
    		pPoint->setActualValues(dValue, sNewUnitSens) ;
        pPoint->setVisible() ;
    	}
      // Secondary value with good unit
      //
    	else if (pPoint->_value.getUnit2() == sNewUnitSens)
    	{
    		double dValue = StringToDouble(pPoint->_value.getValue2()) ;
    		pPoint->setActualValues(dValue, sNewUnitSens) ;
        pPoint->setVisible() ;
    	}
      // Conversion needed
      //
      else
      {
    		// Opening the conversion database
      	//
    		if ((false == bConvertOpen) && (false == bOddConvert))
      	{
        	DBIResult Resultat = NsCv.open() ;
        	if (DBIERR_NONE != Resultat)
        	{
        		string sErrorMessage = pSuper->getText("unitConversion", "errorOpeningConversionDatabase") ;
        		erreur(sErrorMessage.c_str(), standardError, Resultat) ;
          	bOddConvert = true ;
        	}
        	else
        		bConvertOpen = true ;
      	}
      	if (bConvertOpen)
      	{
          bool bSuccess = false ;

          // Try to convert the primary value
          //
      		double dVal = StringToDouble(pPoint->_value.getValue()) ;

      		if (NsCv.ConvertirUnite(&dVal, _sUnit, pPoint->_value.getUnit(), _sItemValue))
        	{
          	// Successfull conversion
        		//
        		pPoint->setActualValues(dVal, sNewUnitSens) ;
          	pPoint->setVisible() ;

            bSuccess = true ;
        	}

          // Try to convert the secondary value
          //
          if ((false == bSuccess) && (pPoint->_value.getUnit2() != string("")))
          {
          	dVal = StringToDouble(pPoint->_value.getValue2()) ;

      			if (NsCv.ConvertirUnite(&dVal, _sUnit, pPoint->_value.getUnit2(), _sItemValue))
        		{
              // Successfull conversion
        			//
        			pPoint->setActualValues(dVal, sNewUnitSens) ;
          		pPoint->setVisible() ;

            	bSuccess = true ;
        		}
          }
        	// Unsuccessfull conversion
        	//
        	if (false == bSuccess)
        	{
        		pPoint->resetActualValues() ;
        		pPoint->setHidden() ;

          	if (false == bSomeUnconvertibleUnit)
          	{
        			string sErrorMessage = pSuper->getText("LigneDeVieCurves", "cannotConvertUnit") ;
              string sLang = pContexte->getUserLanguage() ;
              string sUnitLabel ;
              if ((string("") == _sUnit) || (string("200001") == _sUnit))
              	sUnitLabel = string("\"\"") ;
              else
								pContexte->getDico()->donneLibelle(sLang, &_sUnit, &sUnitLabel) ;
              string sPointUnit = pPoint->_value.getUnit() ;
              string sPointUnitLabel ;
              if ((string("") == sPointUnit) || (string("200001") == sPointUnit))
              	sPointUnitLabel = string("\"\"") ;
              else
								pContexte->getDico()->donneLibelle(sLang, &sPointUnit, &sPointUnitLabel) ;
              sErrorMessage += string(" (") + sPointUnitLabel + string(" -> ") + sUnitLabel + string(")");
        			erreur(sErrorMessage.c_str(), standardError, 0) ;
          		bSomeUnconvertibleUnit = true ;
          	}
        	}
      	}
      	if (bOddConvert)
      	{
      		pPoint->resetActualValues() ;
        	pPoint->setHidden() ;
      	}
      }
    }

    pPoint = pPoint->getNextBrother() ;
  }

  if (bConvertOpen)
  	NsCv.close() ;
}

void
NSLdvCurve::pointsSetup()
{
	NSLdvCurvePoint* pPoint = _pFirstPoint ;
  while (pPoint)
  {
  	pPoint->SetupWindow() ;
    pPoint = pPoint->getNextBrother() ;
  }
}

void
NSLdvCurve::close()
{
	// First step : remove the NSLdvCurvePoint(s) from ldv toons array
  //              and delete them
  //
  NSLdvViewArea* pActiveWorkingArea = _pCurvesPannel->getView()->getActiveWorkingArea() ;

  NSLdvCurvePoint* pPoint = _pFirstPoint ;
  while (pPoint)
  {
  	NSLdvCurvePoint* pNextPoint = pPoint->getNextBrother() ;
    if (pActiveWorkingArea)
  	  _pCurvesPannel->getView()->removeFromToonsArray(pActiveWorkingArea->getToonsArray(), pPoint) ;
    delete pPoint ;
    pPoint = pNextPoint ;
  }
}

int
NSLdvCurve::giveYCoordinateForValue(double dValue, bool* pSuccess, string sValueUnitSens, bool bLimitedToAxisLimits)
{
	if (pSuccess)
		*pSuccess = true ;

	if (sValueUnitSens == string(""))
    return _pAxis->giveWorkingAreaYCoordinateForValue(dValue, bLimitedToAxisLimits) ;

  string sCurveUnitSens ;
  _pCurvesPannel->pContexte->getDico()->donneCodeSens(&_sUnit, &sCurveUnitSens) ;

  if (sValueUnitSens == sCurveUnitSens)
		return _pAxis->giveWorkingAreaYCoordinateForValue(dValue, bLimitedToAxisLimits) ;

  NSCV NsCv(_pCurvesPannel->pContexte->getSuperviseur()) ;
	DBIResult Resultat = NsCv.open() ;
  if (DBIERR_NONE != Resultat)
  {
  	string sErrorMessage = _pCurvesPannel->pContexte->getSuperviseur()->getText("unitConversion", "errorOpeningConversionDatabase") ;
    erreur(sErrorMessage.c_str(), standardError, Resultat) ;
    if (pSuccess)
			*pSuccess = false ;
		return -1 ;
  }

  // Try to convert the value
  //
  double dConvertedVal = dValue ;

  bool bSuccCv = NsCv.ConvertirUnite(&dConvertedVal, _sUnit, sValueUnitSens, _sItemValue) ;

  NsCv.close() ;

	if (bSuccCv)
  	return _pAxis->giveWorkingAreaYCoordinateForValue(dConvertedVal, bLimitedToAxisLimits) ;
  else
  {
  	if (pSuccess)
			*pSuccess = false ;
  	return -1 ;
  }
}

void
NSLdvCurve::setUnit(string sUnitToSet)
{
	_sUnit = sUnitToSet ;
	string sLang = _pCurvesPannel->pContexte->getUserLanguage() ;
  _pCurvesPannel->pContexte->getDico()->donneLibelle(sLang, &_sUnit, &_sUnitLabel) ;
  setCompoundLabel() ;
}

void
NSLdvCurve::setCompoundLabel()
{
	_sCompoundLabel = _sItemLabel ;
  if (string("") != _sUnitLabel)
  	_sCompoundLabel += string(" (") + _sUnitLabel + string(")");
}

void
NSLdvCurve::initHistoryOfValuedGoals(ArrayGoals *pGoals)
{
	if ((NULL == pGoals) || (pGoals->empty()))
		return ;

	string sItemSens ;
  _pCurvesPannel->pContexte->getDico()->donneCodeSens(&_sItemValue, &sItemSens) ;

	for (ArrayGoalIter goalIter = pGoals->begin() ; pGoals->end() != goalIter ; goalIter++ )
  {
  	if ((*goalIter)->containsGoalsOnValue())
    {
  		string sGoalItemSens ;
			_pCurvesPannel->pContexte->getDico()->donneCodeSens(&((*goalIter)->getLexique()), &sGoalItemSens) ;

    	if (sGoalItemSens == sItemSens)
      	_aHistoryOfValuedGoals.push_back(*goalIter) ;
    }
  }
}

void
NSLdvCurve::buildBackgroundQuadrisFromGoals()
{
	if (_aHistoryOfValuedGoals.empty())
		return ;

	_pBackground->_bExistMaxLimit = false ;
	_pBackground->_dMaxLimit      = - numeric_limits<double>::max() ;
	_pBackground->_sMaxLimitUnit  = string("") ;

	_pBackground->_bExistMinLimit = false ;
	_pBackground->_dMinLimit      = numeric_limits<double>::max() ;
	_pBackground->_sMinLimitUnit  = string("") ;

	// Voir http://www.pitt.edu/~nisg/cis/web/cgi/rgb.html
  //
	NS_CLASSLIB::TColor paleBlue ;
  nsSkin* pBlueSkin = _pCurvesPannel->pContexte->getSkins()->donneSkin("CurveBackgroundBlue") ;
	if (pBlueSkin && pBlueSkin->getBackColor())
  	paleBlue = *(pBlueSkin->getBackColor()) ;
  else
  	paleBlue = NS_CLASSLIB::TColor(240, 255, 240) ;

  NS_CLASSLIB::TColor paleGreen ;
  nsSkin* pGreenSkin = _pCurvesPannel->pContexte->getSkins()->donneSkin("CurveBackgroundGreen") ;
	if (pGreenSkin && pGreenSkin->getBackColor())
  	paleGreen = *(pGreenSkin->getBackColor()) ;
  else
  	paleGreen = NS_CLASSLIB::TColor(220, 255, 220) ;

  NS_CLASSLIB::TColor paleYellow ;
  nsSkin* pYellowSkin = _pCurvesPannel->pContexte->getSkins()->donneSkin("CurveBackgroundYellow") ;
	if (pYellowSkin && pYellowSkin->getBackColor())
  	paleYellow = *(pYellowSkin->getBackColor()) ;
  else
  	paleYellow = NS_CLASSLIB::TColor(255, 250, 220) ;

	NS_CLASSLIB::TColor paleRed ;
  nsSkin* pRedSkin = _pCurvesPannel->pContexte->getSkins()->donneSkin("CurveBackgroundRed") ;
	if (pRedSkin && pRedSkin->getBackColor())
  	paleRed = *(pRedSkin->getBackColor()) ;
  else
  	paleRed = NS_CLASSLIB::TColor(255, 240, 220) ;

	// This algorithm mimics ObjectifViewerDlg::repaintColorBar algorithm
  //
	// The only possible algoritm is to define the center color (hearth)
	// then move up from there, using existing values as color transitions
  // then start again from the center and go down (in the same way we went up)

  //
  // Every goals' value is converted to the curve's unit
  //

  NVLdVTemps minDate ;
  minDate.setNoLimit() ;

  NVLdVTemps maxDate ;
  maxDate.init() ;

	vector<NSLdvGoal*>::iterator goalIt = _aHistoryOfValuedGoals.begin() ;
  for ( ; _aHistoryOfValuedGoals.end() != goalIt ; goalIt++)
  {
  	// First, take begin and end dates
    //
    if ((false == (*goalIt)->_tOuvertLe.estVide()) ||
        (false == (*goalIt)->_tDateOuverture.estVide()) ||
        (false == (*goalIt)->canBeFullyConverted(_sUnit)))
    {
    	NVLdVTemps beginDate ;
      if (false == (*goalIt)->_tOuvertLe.estVide())
      	beginDate = (*goalIt)->_tOuvertLe ;
      else
      	beginDate = (*goalIt)->_tDateOuverture ;

      NVLdVTemps endDate ;
      if      (false == (*goalIt)->_tFermeLe.estVide())
      	endDate = (*goalIt)->_tFermeLe ;
      else if (false == (*goalIt)->_tDateFermeture.estVide())
      	endDate = (*goalIt)->_tDateFermeture ;
      else
      	endDate.setNoLimit() ;

      if (beginDate < minDate)
      	minDate = beginDate ;

      if (endDate > maxDate)
      	maxDate = endDate ;

  		// looking for the color of the central quadri
  		//
      NS_CLASSLIB::TColor centerColor ;
      // Next up color
      NS_CLASSLIB::TColor upperColor = NS_CLASSLIB::TColor::White ;
      double dUpperValue = numeric_limits<double>::max() ;
      // Next down color
      NS_CLASSLIB::TColor lowerColor = NS_CLASSLIB::TColor::White ;
      double dLowerValue = - numeric_limits<double>::max() ;

      if ((*goalIt)->_bValMinIdeal || (*goalIt)->_bValMaxIdeal)
      {
      	centerColor = paleBlue ;

        if ((*goalIt)->_bValMinIdeal)
        {
        	lowerColor = paleGreen ;
          dLowerValue = (*goalIt)->_dValMinIdeal ;
          _pCurvesPannel->getView()->convertUnit(&dLowerValue, _sUnit, (*goalIt)->_sUniteValMinIdeal, _sItemValue, false) ;
        }
        if ((*goalIt)->_bValMaxIdeal)
        {
        	upperColor = paleGreen ;
          dUpperValue = (*goalIt)->_dValMaxIdeal ;
          _pCurvesPannel->getView()->convertUnit(&dUpperValue, _sUnit, (*goalIt)->_sUniteValMaxIdeal, _sItemValue, false) ;
        }
      }
      else if ((*goalIt)->_bValMinConseille || (*goalIt)->_bValMaxConseille)
      {
      	centerColor = paleGreen ;

        if ((*goalIt)->_bValMinConseille)
        {
        	lowerColor = paleYellow ;
          dLowerValue = (*goalIt)->_dValMinConseille ;
          _pCurvesPannel->getView()->convertUnit(&dLowerValue, _sUnit, (*goalIt)->_sUniteValMinConseille, _sItemValue, false) ;
        }
        if ((*goalIt)->_bValMaxConseille)
        {
        	upperColor = paleYellow ;
          dUpperValue = (*goalIt)->_dValMaxConseille ;
          _pCurvesPannel->getView()->convertUnit(&dUpperValue, _sUnit, (*goalIt)->_sUniteValMaxConseille, _sItemValue, false) ;
        }
      }
      else if ((*goalIt)->_bValMinAutorise || (*goalIt)->_bValMaxAutorise)
      {
      	centerColor = paleYellow ;

        if ((*goalIt)->_bValMinAutorise)
        {
        	lowerColor = paleRed ;
          dLowerValue = (*goalIt)->_dValMinAutorise ;
          _pCurvesPannel->getView()->convertUnit(&dLowerValue, _sUnit, (*goalIt)->_sUniteValMinAutorise, _sItemValue, false) ;
        }
        if ((*goalIt)->_bValMaxAutorise)
        {
        	upperColor = paleRed ;
          dUpperValue = (*goalIt)->_dValMaxAutorise ;
          _pCurvesPannel->getView()->convertUnit(&dUpperValue, _sUnit, (*goalIt)->_sUniteValMaxAutorise, _sItemValue, false) ;
        }
      }
      else
      	return ;

      bool bCenterDrawn = false ;

      // Quadri bottom when going up
      double dBottomValue = dLowerValue ;

      // Quadri top when going down (since we first go up, the center will
      // already have been drawn when going down, this is the reason why
      // we also start from dLowerValue)
      double dTopValue = dLowerValue ;

      // Drawing center quadri
      //
      if (lowerColor == upperColor)
      {
      	NSLdvCurveBackgroundQuadri bgQuadri(_pCurvesPannel->getView(), centerColor, beginDate, endDate, dLowerValue, _sUnit, dUpperValue, _sUnit) ;
      	_pBackground->addQuadri(&bgQuadri) ;
        bCenterDrawn = true ;

        dBottomValue = dUpperValue ;
      }

      // Going up
      //

      // Transition toward yellow
      if ((*goalIt)->_bValMaxConseille)
      {
      	// If center was not drawn, it means that we don't have a bValMinConseille
        // So, we either have :
        // - a bValMinIdeal :    we must draw a blue central quadri
        // - a bValMinAutorise : we must draw a green central quadri
        // - no Min information : we must draw a green lower part quadri
      	if (false == bCenterDrawn)
        {
        	upperColor = centerColor ;
        	bCenterDrawn = true ;
        }

        double dTopOfTheBoxValue = (*goalIt)->_dValMaxConseille ;
        _pCurvesPannel->getView()->convertUnit(&dTopOfTheBoxValue, _sUnit, (*goalIt)->_sUniteValMaxConseille, _sItemValue, false) ;

        NSLdvCurveBackgroundQuadri bgQuadri(_pCurvesPannel->getView(), upperColor, beginDate, endDate, dBottomValue, _sUnit, dTopOfTheBoxValue, _sUnit) ;
        _pBackground->addQuadri(&bgQuadri) ;

        dBottomValue = dTopOfTheBoxValue ;
        upperColor = paleYellow ;
      }
      // Transition toward red
      if ((*goalIt)->_bValMaxAutorise)
      {
        if (false == bCenterDrawn)
        {
        	upperColor = centerColor ;
        	bCenterDrawn = true ;
        }

        double dTopOfTheBoxValue = (*goalIt)->_dValMaxAutorise ;
        _pCurvesPannel->getView()->convertUnit(&dTopOfTheBoxValue, _sUnit, (*goalIt)->_sUniteValMaxAutorise, _sItemValue, false) ;

        NSLdvCurveBackgroundQuadri bgQuadri(_pCurvesPannel->getView(), upperColor, beginDate, endDate, dBottomValue, _sUnit, dTopOfTheBoxValue, _sUnit) ;
        _pBackground->addQuadri(&bgQuadri) ;

        dBottomValue = dTopOfTheBoxValue ;
        upperColor = paleRed ;
      }

      // Upper part
      //
      if (false == bCenterDrawn)
      {
      	upperColor = centerColor ;
        bCenterDrawn = true ;
      }

      NSLdvCurveBackgroundQuadri bgQuadri(_pCurvesPannel->getView(), upperColor, beginDate, endDate, dBottomValue, _sUnit, numeric_limits<double>::max(), _sUnit) ;
      _pBackground->addQuadri(&bgQuadri) ;

      // Going down
      //

      // Transition toward yellow
      if ((*goalIt)->_bValMinConseille)
      {
      	double dBottomOfTheBoxValue = (*goalIt)->_dValMinConseille ;
        _pCurvesPannel->getView()->convertUnit(&dBottomOfTheBoxValue, _sUnit, (*goalIt)->_sUniteValMinConseille, _sItemValue, false) ;

      	if ((dTopValue != - numeric_limits<double>::max()) && (lowerColor != centerColor))
        {
        	NSLdvCurveBackgroundQuadri bgQuadri(_pCurvesPannel->getView(), lowerColor, beginDate, endDate, dBottomOfTheBoxValue, _sUnit, dTopValue, _sUnit) ;
        	_pBackground->addQuadri(&bgQuadri) ;
        }

        dTopValue = dBottomOfTheBoxValue ;
        lowerColor = paleYellow ;
      }

      // Transition toward red
      if ((*goalIt)->_bValMinAutorise)
      {
      	double dBottomOfTheBoxValue = (*goalIt)->_dValMinAutorise ;
        _pCurvesPannel->getView()->convertUnit(&dBottomOfTheBoxValue, _sUnit, (*goalIt)->_sUniteValMinAutorise, _sItemValue, false) ;

      	if ((dTopValue != - numeric_limits<double>::max()) && (lowerColor != centerColor))
        {
        	NSLdvCurveBackgroundQuadri bgQuadri(_pCurvesPannel->getView(), lowerColor, beginDate, endDate, dBottomOfTheBoxValue, _sUnit, dTopValue, _sUnit) ;
        	_pBackground->addQuadri(&bgQuadri) ;
        }

        dTopValue = dBottomOfTheBoxValue ;
        lowerColor = paleYellow ;
      }

      // Lower part
      //
      if ((dTopValue != - numeric_limits<double>::max()) && (lowerColor != centerColor))
      {
      	NSLdvCurveBackgroundQuadri bgQuadri(_pCurvesPannel->getView(), lowerColor, beginDate, endDate, - numeric_limits<double>::max(), _sUnit, dTopValue, _sUnit) ;
        _pBackground->addQuadri(&bgQuadri) ;
      }

      // Updating min and max limits
      //
      if (numeric_limits<double>::max() != dBottomValue)
      {
      	if (false == _pBackground->_bExistMaxLimit)
      	{
					_pBackground->_dMaxLimit      = dBottomValue ;
        	_pBackground->_bExistMaxLimit = true ;
      	}
      	else
      		if (dBottomValue > _pBackground->_dMaxLimit)
        		_pBackground->_dMaxLimit = dBottomValue ;
      }

      if (-numeric_limits<double>::max() != dTopValue)
      {
  			if (false == _pBackground->_bExistMinLimit)
      	{
					_pBackground->_dMinLimit      = dTopValue ;
        	_pBackground->_bExistMinLimit = true ;
      	}
      	else
      		if (dTopValue < _pBackground->_dMinLimit)
        		_pBackground->_dMinLimit = dTopValue ;
      }
    }
  }

  NS_CLASSLIB::TRect YAxisBox = _pAxis->donneVisibleRectangle() ;
  _pBackground->fixeRectangle(minDate, maxDate, YAxisBox.Bottom(), YAxisBox.Top()) ;
}

NSLdvCurve&
NSLdvCurve::operator=(const NSLdvCurve& src)
{
  if (this == &src)
		return *this ;

	_pCurvesPannel  = src._pCurvesPannel ;

  _sItemLabel     = src._sItemLabel ;
  _sUnitLabel     = src._sUnitLabel ;
  _sCompoundLabel = src._sCompoundLabel ;

	_sItemValue     = src._sItemValue ;
	_sPath          = src._sPath ;
	_sUnit          = src._sUnit ;

	_pFirstPoint    = src._pFirstPoint ;

  _color          = src._color ;
  _dotWidth       = src._dotWidth ;
  _dotStyle       = src._dotStyle ;

  _pointAspect    = src._pointAspect ;
  _pointRadius    = src._pointRadius ;

  _bAdjustPointColorToValue = src._bAdjustPointColorToValue ;

  if (_pAxis)
  {
    NSLdvViewArea* pActiveWorkingArea = _pCurvesPannel->getView()->getActiveWorkingArea() ;
    if (pActiveWorkingArea)
  	  _pCurvesPannel->getView()->removeFromToonsArray(pActiveWorkingArea->getToonsArray(), _pAxis) ;
  	delete _pAxis ;
  }

  if (src._pAxis)
  	_pAxis = new NSLdvCurveYAxis(*(src._pAxis)) ;
  else
  	_pAxis = (NSLdvCurveYAxis*) 0 ;

  return *this ;
}

// -----------------------------------------------------------------------------
//                            NSCurvesPropertyWindow
// -----------------------------------------------------------------------------

NSCurvesProperty::NSCurvesProperty(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvView* pTheView)
                 :NSLdvWindowToon(pCtx, pLdvParent, pTheView)
{
  lObjectCount++ ;

  _iZOrder   = LEVEL_COMMANDS ;
  _sSkinName = string("LdvCurvesList") ;

  initDimensions() ;

  _pInterface = new NSCurvesPropertyWindow(pContexte, getParentInterface(), this, ID_CurvesList, 0, 0, 0, 0) ;
}

NSCurvesProperty::NSCurvesProperty(const NSCurvesProperty& rv)
                 :NSLdvWindowToon(rv.pContexte, rv.getToonParent(), rv._pView)
{
  lObjectCount++ ;

  initialiser((NSLdvWindowToon*)(&rv)) ;
}

NSCurvesProperty::~NSCurvesProperty()
{
  lObjectCount-- ;
}

void
NSCurvesProperty::SetupWindow()
{
}

void
NSCurvesProperty::setScrollParams()
{
}

void
NSCurvesProperty::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal)
{
}

void
NSCurvesProperty::draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre)
{
}

void
NSCurvesProperty::EvRButtonDown()
{
}

void
NSCurvesProperty::CmNew()
{
  NSLdvCurvesManagementPannel* pPannel = getCurveManagementPannel() ;
  if (NULL == pPannel)
    return ;

  pPannel->CmNew() ;
}

bool
NSCurvesProperty::CmSupres(string sLabelToSuppress)
{
  NSLdvCurvesManagementPannel* pPannel = getCurveManagementPannel() ;
  if (NULL == pPannel)
    return false ;

  return pPannel->CmSupres(sLabelToSuppress) ;
}

void
NSCurvesProperty::CmPutUp(string sLabelToPutUp)
{
  NSLdvCurvesManagementPannel* pPannel = getCurveManagementPannel() ;
  if (NULL == pPannel)
    return ;

  pPannel->CmPutUp(sLabelToPutUp) ;
}

void
NSCurvesProperty::CmModify(string sLabelToPutUp)
{
  NSLdvCurvesManagementPannel* pPannel = getCurveManagementPannel() ;
  if (NULL == pPannel)
    return ;

  pPannel->CmModify(sLabelToPutUp) ;
}

void
NSCurvesProperty::displayElement(NSLdvCurve* pCurve)
{
  NSCurvesPropertyWindow* pList = getInterfaceAsList() ;
  if (NULL == pList)
    return ;

  pList->displayElement(pCurve) ;
}

void
NSCurvesProperty::selectItem(string sItemLabel)
{
  NSCurvesPropertyWindow* pList = getInterfaceAsList() ;
  if (NULL == pList)
    return ;

  pList->selectItem(sItemLabel) ;
}

NSLdvCurvesManagementPannel*
NSCurvesProperty::getCurveManagementPannel()
{
  if (NULL == _pParent)
    return 0 ;

  NSLdvCurvesManagementPannel* pPannel = TYPESAFE_DOWNCAST(_pParent, NSLdvCurvesManagementPannel) ;
  return pPannel ;
}

NSCurvesPropertyWindow*
NSCurvesProperty::getInterfaceAsList()
{
  if (NULL == _pInterface)
    return 0 ;

  NSCurvesPropertyWindow* pList = TYPESAFE_DOWNCAST(_pInterface, NSCurvesPropertyWindow) ;
  return pList ;
}

NSCurvesProperty&
NSCurvesProperty::operator=(const NSCurvesProperty& src)
{
  if (this == &src)
    return *this ;

  return *this ;
}

#define MENU_INSER 101
#define MENU_SUPPR 102
#define MENU_MODIF 103
#define MENU_PUTUP 104

DEFINE_RESPONSE_TABLE1(NSCurvesPropertyWindow, TListWindow)
  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  // EV_WM_LBUTTONUP,
  EV_WM_PAINT,
  EV_COMMAND(MENU_INSER, newCurve),
  EV_COMMAND(MENU_SUPPR, suppress),
  EV_COMMAND(MENU_MODIF, modify),
  EV_COMMAND(MENU_PUTUP, putUp),
  // EV_NOTIFY_AT_CHILD(WM_LBUTTONDBLCLK, ButtonDblClk),
  // EV_NOTIFY_AT_CHILD(WM_RBUTTONDOWN, RButtonDown),
  // EV_NOTIFY_AT_CHILD(LVN_GETDISPINFO, LvnGetDispInfo),
END_RESPONSE_TABLE ;

NSCurvesPropertyWindow::NSCurvesPropertyWindow(NSContexte* pCtx, TWindow* parent, NSCurvesProperty* pToon, int id, int x, int y, int w, int h, TModule *module)
                       :TListWindow(parent, id, x, y, w, h, module), NSRoot(pCtx)
{
  lObjectCount++ ;

  iRes = id ;
  // Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;

  // Inactivate HScroll, activate report
  ModifyStyle(WS_HSCROLL, LVS_REPORT|LVS_SHOWSELALWAYS) ;

  _pToon = pToon ;

  SetCaption(_pToon->_sSkinName.c_str()) ;

  DisableTransfer() ;
  // SetFlag(wfAutoCreate) ;
}

void
NSCurvesPropertyWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;

  string sTitle = pContexte->getSuperviseur()->getText("LigneDeVieCurves", "biometricalCurves") ;

  // int iListWidth = pCurvesPannel->wVisibleBox.Width() - 10 ;
  // InsertColumn(0, TListWindColumn((char far*) sTitle.c_str()/* , iListWidth*/)) ;

  int iListWidth = _pToon->getWidthValue() - 30 ;
  InsertColumn(0, TListWindColumn((char far*) sTitle.c_str(), iListWidth)) ;
}

void
NSCurvesPropertyWindow::EvPaint()
{  TListWindow::EvPaint() ;}

void
NSCurvesPropertyWindow::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  TListWindow::Paint(dc, erase, RectAPeindre) ;
}

void
NSCurvesPropertyWindow::displayElement(NSLdvCurve* pCurve)
{
	InsertItem(TListWindItem(pCurve->getCompoundLabel().c_str(), 0)) ;
}

void
NSCurvesPropertyWindow::suppress()
{
	string sSelectedLabel = string("") ;
  int iIndex = getLabelOfSelectedItem(&sSelectedLabel) ;
  if (-1 == iIndex)
  	return ;

	bool bReallyDeleted = ((NSCurvesProperty *)(_pToon))->CmSupres(sSelectedLabel) ;
	if (bReallyDeleted)
  	DeleteAnItem(iIndex) ;
}

void
NSCurvesPropertyWindow::putUp()
{
	string sSelectedLabel = string("") ;
  int iIndex = getLabelOfSelectedItem(&sSelectedLabel) ;
  if (-1 == iIndex)
  	return ;

  ((NSCurvesProperty *)(_pToon))->CmPutUp(sSelectedLabel) ;
}

void
NSCurvesPropertyWindow::modify()
{
	string sSelectedLabel = string("") ;
  int iIndex = getLabelOfSelectedItem(&sSelectedLabel) ;
  if (iIndex == -1)
  	return ;

  ((NSCurvesProperty *)(_pToon))->CmModify(sSelectedLabel) ;
}

void
NSCurvesPropertyWindow::newCurve()
{
	((NSCurvesProperty *)(_pToon))->CmNew() ;
}

// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSCurvesPropertyWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  	putUp() ;  else
  	newCurve() ;

  TListWindow::EvLButtonDblClk(modKeys, point) ;
}

void
NSCurvesPropertyWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;
  int indexItem = HitTest(info) ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  string sAdd = pSuper->getText("generalLanguageForLists", "add") ;

  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;		string sModify = pSuper->getText("generalLanguageForLists", "modify") ;
    string sSuppre = pSuper->getText("generalLanguageForLists", "remove") ;    string sPutUp  = pSuper->getText("LigneDeVieCurves", "putFront") ;    // création d'un menu popup    NS_CLASSLIB::TPoint lp = point ;

    OWL::TPopupMenu *pPopupMenu = new OWL::TPopupMenu() ;

    pPopupMenu->AppendMenu(MF_STRING, MENU_PUTUP, sPutUp.c_str()) ;
    pPopupMenu->AppendMenu(MF_STRING, MENU_MODIF, sModify.c_str()) ;
    pPopupMenu->AppendMenu(MF_STRING, MENU_SUPPR, sSuppre.c_str()) ;
    pPopupMenu->AppendMenu(MF_SEPARATOR, 0, 0);
    pPopupMenu->AppendMenu(MF_STRING, MENU_INSER, sAdd.c_str()) ;

    ClientToScreen(lp) ;
  	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  	delete pPopupMenu ;  }
  else
  {
    // création d'un menu popup
    NS_CLASSLIB::TPoint lp = point ;

    OWL::TPopupMenu *pPopupMenu = new OWL::TPopupMenu() ;

    pPopupMenu->AppendMenu(MF_STRING, MENU_INSER, sAdd.c_str()) ;

    ClientToScreen(lp) ;
  	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  	delete pPopupMenu ;
  }

  TListWindow::EvRButtonDown(modkeys, point) ;
}

void
NSCurvesPropertyWindow::ButtonDblClk()
{
}

void
NSCurvesPropertyWindow::RButtonDown()
{
}

void
NSCurvesPropertyWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if      (key == VK_DELETE)
  	suppress() ;
  else if (key == VK_INSERT)
  {
  	if (GetKeyState(VK_SHIFT) < 0)
    	putUp() ;
    else
    	newCurve() ;
  }
  else
  	TListWindow::EvKeyDown(key, repeatCount, flags) ;
}

void
NSCurvesPropertyWindow::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& pt)
{
	NSContexte *pContexte = pContexte ;
  NSSuper    *pSuper    = pContexte->getSuperviseur() ;

  if (pSuper->bDragAndDrop)
  {
  	pSuper->DragDrop->DragLeave( *this ) ;
    pSuper->DragDrop->EndDrag() ;
    delete pSuper->DragDrop ;
    pSuper->DragDrop = 0 ;
    pSuper->bDragAndDrop = false ;
  }

  TListWindow::EvLButtonUp(modKeys, pt) ;
}

// -----------------------------------------------------------------------------
// Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSCurvesPropertyWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

	for (int i = 0 ; i < count ; i++)  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;}

int
NSCurvesPropertyWindow::getLabelOfSelectedItem(string* pLabelString)
{
	int iIndex = IndexItemSelect() ;
  if (iIndex == -1)
  {
  	string sWarning = pContexte->getSuperviseur()->getText("generalLanguageForLists", "anElementMustBeSelected") ;
    erreur(sWarning.c_str(), warningError, 0) ;
    if (NULL == pLabelString)
    	*pLabelString = string("") ;
    return iIndex ;
  }

  if (pLabelString)
  {
  	char szItemText[2048] ;

  	int iReturnedTextSize = GetItemText(iIndex, 0, szItemText, 2048) ;
  	if (iReturnedTextSize > 0)
  		*pLabelString = string(szItemText) ;
  	else
  	 	*pLabelString = string("") ;
  }

  return iIndex ;
}

void
NSCurvesPropertyWindow::selectItem(string sItemLabel)
{
	int iNbItem = GetItemCount() ;
  for (int iIndex = 0 ; iIndex < iNbItem ; iIndex++)
  {
  	char szItemText[2048] ;

  	int iReturnedTextSize = GetItemText(iIndex, 0, szItemText, 2048) ;
  	if (iReturnedTextSize > 0)
    {
    	if (sItemLabel == string(szItemText))
      {
      	SetItemState(iIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;
        return ;
      }
    }
  }
}

void
NSCurvesPropertyWindow::EvSetFocus(HWND hWndLostFocus)
{
	SetBkColor(0x00fff0f0) ; // 0x00bbggrr
  SetTextBkColor(0x00fff0f0) ;
  Invalidate();

  // pCurvesPannel->focusFct() ;

  int count = GetItemCount() ;

  for (int i = 0 ; i < count ; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    	return ;

	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;

  TListWindow::EvSetFocus(hWndLostFocus) ;
}

void
NSCurvesPropertyWindow::EvKillFocus(HWND hWndGetFocus)
{
	SetBkColor(0x00ffffff) ; // 0x00bbggrr
  SetTextBkColor(0x00ffffff) ;
  Invalidate();
  TListWindow::EvKillFocus(hWndGetFocus) ;
}

void
NSCurvesPropertyWindow::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
}

/*
bool
NSCurvesPropertyWindow::PreProcessMsg(MSG& msg)
{
  switch(msg.message)
  {
    case WM_PAINT:
    {
      // NS_CLASSLIB::TPoint pt1(LOWORD(msg.lParam), HIWORD(msg.lParam)) ;
      // NS_CLASSLIB::TPoint pt2(LOWORD(msg.wParam), HIWORD(msg.wParam)) ;
      // NS_CLASSLIB::TRect  rect1(pt1, pt2) ;

      TPaintDC dc(*this) ;
      NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&dc.Ps.rcPaint ;
      Paint(dc, true, rect) ;

      DefWindowProc(msg.message, msg.wParam, msg.lParam) ;

      break ;
    }
    case WM_NCPAINT:
      DefWindowProc(msg.message, msg.wParam, msg.lParam) ;
      break ;
  }

  return TWindow::PreProcessMsg(msg) ;
}
*/

NsObjectIcon::NsObjectIcon(NSLdvObjetView* pObject)
{
  lObjectCount++ ;

  _pObject    = pObject ;
}

NsObjectIcon::NsObjectIcon(NSLdvObjetView* pObject, NS_CLASSLIB::TRect rect)
{
  lObjectCount++ ;

  _pObject    = pObject ;
  _VisibleBox = rect ;
}

NsObjectIcon::~NsObjectIcon()
{
  lObjectCount-- ;
}

// -----------------------------------------------------------------------------
// ------------------------- NSLdvViewIconsShowWindow --------------------------
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSLdvViewIconsShowWindow, TWindow)
	EV_WM_TIMER,
  EV_WM_LBUTTONDOWN,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(IDC_OUVRIR,     Ouvrir),
  EV_COMMAND(CM_EDITER,      Editer),
/*
  EV_COMMAND(IDC_OTER,       Oter),
  EV_COMMAND(IDC_PROPRIETE,  Proprietes),
  EV_COMMAND(IDC_PARAMETRES, Parametres),
  EV_COMMAND(IDC_DETRUIRE,   Detruire),
*/
  EV_COMMAND(IDC_XML_EXPORT, XmlExport),
/*
  EV_COMMAND(IDC_TRANSFERT,  Transfert),
  EV_COMMAND(IDC_FORCESIGN,  Reassign),
*/
END_RESPONSE_TABLE;

NSLdvViewIconsShowWindow::NSLdvViewIconsShowWindow(NSContexte* pCtx, TWindow* parent)
                         :TWindow(parent), NSRoot(pCtx)
{
  lObjectCount++ ;

  _bTimerEnabled      = false ;
  _pParent            = (NSLdvViewAreaWindow*) parent ;

  _ActiveObject       = (NsObjectIcon*) 0 ;
  _ActiveObjectStatus = statusNone ;

  _bJustIcons         = false ;
  _bJustPileUp        = false ;

  if (_bJustIcons)
    _pToolTip         = new NSTitleTip(this, pContexte) ;
  else
    _pToolTip         = (NSTitleTip*) 0 ;

  _pTextFont          = (OWL::TFont*) 0 ;

  _iLeftSideWidth     = 0 ;
  _iRightSideWidth    = 0 ;

  _iExpectedLeftSideWidth  = 0 ;
  _iExpectedRightSideWidth = 0 ;
  _iExpectedHeight         = 0 ;

  _iMaxLeftExcursion  = 0 ;
  _iMaxRightExcursion = 0 ;

  _bSizeIsSet = false ;

  _iNbTimerTicks = 0 ;
}

NSLdvViewIconsShowWindow::~NSLdvViewIconsShowWindow()
{
  lObjectCount-- ;

  // Empty objects vector
  //
  resetObjects() ;

  DisableTimer() ;

  if (_pToolTip)
    delete _pToolTip ;
  if (_pTextFont)
    delete _pTextFont ;
}

void
NSLdvViewIconsShowWindow::SetupWindow()
{
  TWindow::SetupWindow() ;

  SetBkgndColor(ClassLib::TColor::Transparent) ;

  EnableTimer() ;

  if (_bJustIcons && _pToolTip)
  {
    _pToolTip->Create() ;
	  _pToolTip->Hide() ;
  }
  else
    _pTextFont = new OWL::TFont("MS Sans Serif", -9) ;

  if (_aObjects.size() < 3)
    _bJustPileUp = true ;

  setSize() ;
}

void
NSLdvViewIconsShowWindow::setSize()
{
try
{
  if (_aObjects.empty())
    return ;

  TWindowDC* pWinDC = new TWindowDC(HWindow) ;

  _iExpectedLeftSideWidth  = _iLeftSideWidth ;
  _iExpectedRightSideWidth = _iRightSideWidth ;
  _iExpectedHeight         = 0 ;

  if (false == _bJustIcons)
    pWinDC->SelectObject(*_pTextFont) ;

  NS_CLASSLIB::TRect clientRect = GetClientRect() ;

  int iTheoricalIconWidth  = ICONWIDTH ;
  int iTheoricalIconHeight = ICONWIDTH ;

  // Evaluate total width to display icons side by side
  //
  int iTotalIconsWidth   = iTheoricalIconWidth * _aObjects.size() ;
  int iInBetweenInterval = (getDiameter() - iTotalIconsWidth) / (_aObjects.size() - 1) ;

  vector<NsObjectIcon*>::iterator objectsIterator = _aObjects.begin() ;

  // Max of y(s)
  //
  int iTopSpaceHeight = clientRect.Height() - _iBottomMargin - iTheoricalIconHeight ;
  int iClientWidth    = clientRect.Width() ;

  // Radius is half of icons centers area (width minus half icon left and half icon right)
  int iClientRadius   = (getDiameter() - iTheoricalIconWidth)  / 2 ;

  int iX = 0 ;
  int iY = 0 ;

  int iPreviousY = 0 ;

  for ( ; _aObjects.end() != objectsIterator ; objectsIterator++)
  {
    bool bTextAtRightOfIcon = true ;

    // Get y from x
    //

    // If icons only, use ellipse coordinates to get y from x
    //
    int iCenteredX    = iX - iClientRadius ;
    double iRadiusPw2 = double(1) - (double(iCenteredX * iCenteredX) / double(iClientRadius * iClientRadius)) ;
    double iHeightPw2 = iRadiusPw2 * double(iTopSpaceHeight * iTopSpaceHeight) ;
    double dHeight    = sqrt(iHeightPw2) ;
    int iCalculatedY  = int(dHeight) ;

    if (_bJustIcons && (false == _bJustPileUp))
      iY = iCalculatedY ;
    else if (_bJustPileUp)
    {
      if (iPreviousY > 0)
        iY += iTheoricalIconHeight + 1 ; // space between icons
      else
        iPreviousY = 1 ;

      if (0 == _iRightSideWidth)
        bTextAtRightOfIcon = false ;
    }
    else
    {
      // Here, iY has not been set and has previous value

      // curve going up
      //
      if (iCalculatedY > iY)
      {
        // Make certain that this icon is fully above the other
        if (iCalculatedY < iY + iTheoricalIconHeight)
          iY += iTheoricalIconHeight ;
        else
          iY = iCalculatedY ;

        // If drawing elliptically, put text left for icons on growing curve
        //
        if (false == _bJustPileUp)
          bTextAtRightOfIcon = false ;
      }
      else if (iCalculatedY < iY)
      {
        // Make certain that this icon is fully under the other
        if (iCalculatedY > iY - iTheoricalIconHeight)
          iY -= iTheoricalIconHeight ;
        else
          iY = iCalculatedY ;
      }
      else if (0 == iY)
        bTextAtRightOfIcon = false ;  // for first icon, at left
    }

    // Draw icon
    //
    int iTrueY = iTopSpaceHeight - iY ;

    int iTrueX = iX + iTheoricalIconWidth / 2 ;
    if (false == _bJustPileUp)
      iTrueX += _iLeftSideWidth - iClientRadius ;

    NS_CLASSLIB::TPoint ptTopLeft = NS_CLASSLIB::TPoint(iTrueX, iTrueY) ;

    // Don't really draw, just get visible box
    //
    NS_CLASSLIB::TRect objectVisibleBox ;
    int iIconWidth = (*objectsIterator)->getObject()->drawIcon(pWinDC, &ptTopLeft, &objectVisibleBox, true) ;

    (*objectsIterator)->setBox(objectVisibleBox) ;

    if (objectVisibleBox.Top() < 0)
    {
      int iProposedHeight = clientRect.Height() - objectVisibleBox.Top() ;
      if (iProposedHeight > _iExpectedHeight)
        _iExpectedHeight = iProposedHeight ;
    }

    // Draw text
    //
    if (false == _bJustIcons)
    {
      //
      //
      string sObjectLabel = (*objectsIterator)->getObject()->_pObjet->getTitre() ;

      NS_CLASSLIB::TSize TailleChaine = pWinDC->GetTextExtent(sObjectLabel.c_str(),
                                               strlen(sObjectLabel.c_str())) ;

      NS_CLASSLIB::TRect txtBox ;
      if (bTextAtRightOfIcon)
      {
        objectVisibleBox.right += TailleChaine.X() + 2 ;
        txtBox = objectVisibleBox ;
        txtBox.left += iTheoricalIconWidth + 2 ;

        // Too large to fit; ask for more
        if (objectVisibleBox.Right() > clientRect.Right())
        {
          int iWidthExcess = objectVisibleBox.Right() - clientRect.Right() ;
          int iNeededWidth = _iRightSideWidth + iWidthExcess ;
          if (iNeededWidth > _iExpectedRightSideWidth)
            _iExpectedRightSideWidth = iNeededWidth ;
        }
      }
      else
      {
        objectVisibleBox.left -= TailleChaine.X() + 2 ;
        txtBox = objectVisibleBox ;
        txtBox.right -= iTheoricalIconWidth + 2 ;

        // Too large to fit; ask for more
        if (objectVisibleBox.Left() < 0)
        {
          int iNeededWidth = _iLeftSideWidth - objectVisibleBox.Left() ;
          if (iNeededWidth > _iExpectedLeftSideWidth)
            _iExpectedLeftSideWidth = iNeededWidth ;
        }
      }
    }

    if (false == _bJustPileUp)
      iX += iIconWidth + iInBetweenInterval ;
  }

  if (false == _bJustIcons)
    pWinDC->RestoreFont() ;


  delete pWinDC ;

  resize() ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewIconsShowWindow::setSize.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

int
NSLdvViewIconsShowWindow::getDiameter()
{
  if (_aObjects.empty())
    return 0 ;

  size_t iObjNb = _aObjects.size() ;

  return iObjNb * ICONWIDTH + (iObjNb - 1) * 4 ;
}

void
NSLdvViewIconsShowWindow::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectToPaint)
{
try
{
  if (_bSizeIsSet)
    TWindow::Paint(dc, erase, RectToPaint) ;

  _iExpectedLeftSideWidth  = _iLeftSideWidth ;
  _iExpectedRightSideWidth = _iRightSideWidth ;
  _iExpectedHeight         = 0 ;

  if (_aObjects.empty())
    return ;

  if (false == _bJustIcons)
    dc.SelectObject(*_pTextFont);

  NS_CLASSLIB::TRect clientRect = GetClientRect() ;

  int iTheoricalIconWidth  = ICONWIDTH ;
  int iTheoricalIconHeight = ICONWIDTH ;

  // Evaluate total width to display icons side by side
  //
  int iTotalIconsWidth   = iTheoricalIconWidth * _aObjects.size() ;
  int iInBetweenInterval = (getDiameter() - iTotalIconsWidth) / (_aObjects.size() - 1) ;

  vector<NsObjectIcon*>::iterator objectsIterator = _aObjects.begin() ;

  // Max of y(s)
  //
  int iTopSpaceHeight = clientRect.Height() - _iBottomMargin - iTheoricalIconHeight ;
  int iClientWidth    = clientRect.Width() ;

  // Radius is half of icons centers area (width minus half icon left and half icon right)
  int iClientRadius   = (getDiameter() - iTheoricalIconWidth)  / 2 ;

  int iX = 0 ;
  int iY = 0 ;
  int iEllipseY = 0 ;

  int iPreviousY = 0 ;

  for ( ; _aObjects.end() != objectsIterator ; objectsIterator++)
  {
    bool bTextAtRightOfIcon = true ;

    // Get y from x
    //

    // If icons only, use ellipse coordinates to get y from x
    //
    int iCenteredX    = iX - iClientRadius ;
    double iRadiusPw2 = double(1) - (double(iCenteredX * iCenteredX) / double(iClientRadius * iClientRadius)) ;
    double iHeightPw2 = iRadiusPw2 * double(iTopSpaceHeight * iTopSpaceHeight) ;
    double dHeight    = sqrt(iHeightPw2) ;
    int iCalculatedY  = int(dHeight) ;

    if (_bJustIcons && (false == _bJustPileUp))
      iY = iCalculatedY ;
    else if (_bJustPileUp)
    {
      if (iPreviousY > 0)
        iY += iTheoricalIconHeight + 1 ; // space between icons
      else
        iPreviousY = 1 ;

      if (0 == _iRightSideWidth)
        bTextAtRightOfIcon = false ;
    }
    else
    {
      // Here, iY has not been set and has previous value

      // curve going up
      //
      if (iCalculatedY > iEllipseY)
      {
        // Make certain that this icon is fully above the other
        // if (iCalculatedY < iY + iTheoricalIconHeight)
          iY += iTheoricalIconHeight ;
        // else
        //  iY = iCalculatedY ;

        // If drawing elliptically, put text left for icons on growing curve
        //
        if (false == _bJustPileUp)
          bTextAtRightOfIcon = false ;
      }
      else if (iCalculatedY < iEllipseY)
      {
        // Make certain that this icon is fully under the other
        // if (iCalculatedY > iY - iTheoricalIconHeight)
          iY -= iTheoricalIconHeight ;
        // else
        //  iY = iCalculatedY ;
      }
      else if (0 == iY)
        bTextAtRightOfIcon = false ;  // for first icon, at left

      iEllipseY = iCalculatedY ;
    }

    // Draw icon
    //
    int iTrueY = iTopSpaceHeight - iY ;

    int iTrueX = iX + iTheoricalIconWidth / 2 ;
    if (false == _bJustPileUp)
      iTrueX += _iLeftSideWidth - iClientRadius ;

    NS_CLASSLIB::TPoint ptTopLeft = NS_CLASSLIB::TPoint(iTrueX, iTrueY) ;

    // Really draw if _bSizeIsSet is true; else just get visible box
    //
    NS_CLASSLIB::TRect objectVisibleBox ;
    int iIconWidth = (*objectsIterator)->getObject()->drawIcon(&dc, &ptTopLeft, &objectVisibleBox, !_bSizeIsSet) ;

    (*objectsIterator)->setBox(objectVisibleBox) ;

    if (objectVisibleBox.Top() < 0)
    {
      int iProposedHeight = clientRect.Height() - objectVisibleBox.Top() ;
      if (iProposedHeight > _iExpectedHeight)
        _iExpectedHeight = iProposedHeight ;
    }

    // Draw text
    //
    if (false == _bJustIcons)
    {
      //
      //
      string sObjectLabel = (*objectsIterator)->getObject()->_pObjet->getTitre() ;

      NS_CLASSLIB::TSize TailleChaine = dc.GetTextExtent(sObjectLabel.c_str(),
                                               strlen(sObjectLabel.c_str())) ;

      NS_CLASSLIB::TRect txtBox ;
      if (bTextAtRightOfIcon)
      {
        objectVisibleBox.right += TailleChaine.X() + 2 ;
        (*objectsIterator)->setBox(objectVisibleBox) ;

        txtBox = objectVisibleBox ;
        txtBox.left += iTheoricalIconWidth + 2 ;

        // Too large to fit; ask for more
        if (objectVisibleBox.Right() > clientRect.Right())
        {
          int iWidthExcess = objectVisibleBox.Right() - clientRect.Right() ;
          int iNeededWidth = _iRightSideWidth + iWidthExcess ;
          if (iNeededWidth > _iExpectedRightSideWidth)
            _iExpectedRightSideWidth = iNeededWidth ;
        }
      }
      else
      {
        objectVisibleBox.left -= TailleChaine.X() + 2 ;
        (*objectsIterator)->setBox(objectVisibleBox) ;

        txtBox = objectVisibleBox ;
        txtBox.right -= iTheoricalIconWidth + 2 ;

        // Too large to fit; ask for more
        if (objectVisibleBox.Left() < 0)
        {
          int iNeededWidth = _iLeftSideWidth - objectVisibleBox.Left() ;
          if (iNeededWidth > _iExpectedLeftSideWidth)
            _iExpectedLeftSideWidth = iNeededWidth ;
        }
      }

      if (_bSizeIsSet)
        dc.DrawText(sObjectLabel.c_str(), -1, txtBox,
                                         DT_CENTER|DT_SINGLELINE|DT_VCENTER) ;
    }

    if (false == _bJustPileUp)
      iX += iIconWidth + iInBetweenInterval ;
  }

  if (false == _bJustIcons)
    dc.RestoreFont() ;

  // if ((0 != _iExpectedLeftSideWidth) || (0 != _iExpectedRightSideWidth))
  //  resize() ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewIconsShowWindow::Paint.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvViewIconsShowWindow::resize()
{
  if ((_iLeftSideWidth  == _iExpectedLeftSideWidth) &&
      (_iRightSideWidth == _iExpectedRightSideWidth) &&
      (0 == _iExpectedHeight))
    return ;

  NS_CLASSLIB::TRect clientRect = GetWindowRect() ;
  NS_CLASSLIB::TRect parentRect = _pParent->GetWindowRect() ;

  int iLocalX = clientRect.X() - parentRect.X() ;
  int iLocalY = clientRect.Y() - parentRect.Y() ;

  // How far do we have to move window's left side ?
  //
  // int iDeltaX  = _iLeftSideWidth  - _iExpectedLeftSideWidth ;
  int iDeltaX = - _iExpectedLeftSideWidth ;

  _iLeftSideWidth  = _iExpectedLeftSideWidth ;
  _iRightSideWidth = _iExpectedRightSideWidth ;

  _iExpectedLeftSideWidth  = 0 ;
  _iExpectedRightSideWidth = 0 ;

  int iNeededHeight = clientRect.Height() ;
  if (_iExpectedHeight > 0)
  {
    // Move window up
    //
    int iDeltaHeight = _iExpectedHeight - iNeededHeight ;
    iLocalY -= iDeltaHeight ;

    // Increase height
    //
    iNeededHeight = _iExpectedHeight ;
  }

  SetWindowPos(0, iLocalX + iDeltaX, iLocalY, _iLeftSideWidth + _iRightSideWidth, iNeededHeight, SWP_NOZORDER) ;

  _bSizeIsSet = true ;

  // Invalidate() ;
}

//
// Sets on a timer and updates the state variable
//
void
NSLdvViewIconsShowWindow::EnableTimer()
{
  // Set on the timer if not already set
  //
  if (false == _bTimerEnabled)
  {
    SetTimer(ID_MTIPS_TIMER, TIMER_VALUE) ;
    _bTimerEnabled = true ;
  }
}

//
// Kills the timer if it was enabled
//
void
NSLdvViewIconsShowWindow::DisableTimer()
{
  if (_bTimerEnabled)
  {
    KillTimer(ID_MTIPS_TIMER) ;
    _bTimerEnabled = false ;
  }
}

//
// WM_TIMER Handler: Display a tip if the appropriate timeout has
//                   occurred for a tool.
//
void
NSLdvViewIconsShowWindow::EvTimer(uint timerId)
{
  // Check for out timerID - just in case
  //
  if (ID_MTIPS_TIMER != timerId)
    return ;

  // Retrieve current location of cursor
  //
  NS_CLASSLIB::TPoint curLoc ;
  GetCursorPos(curLoc) ;

  // If mouse cursor is no longer in this windows, ask parent to close it
  //
  if (false == containsPoint(curLoc))
  {
    _iNbTimerTicks = 0 ;

    bool bWaitingForClosing = false ;

    if (_ActiveObject)
    {
      string ps = string("NSLdvViewIconsShowWindow::EvTimer when cursor is out. Status is ") + getHitToonStatusLabel(_ActiveObjectStatus) ;
      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

      if      (statusOpening == _ActiveObjectStatus)
        setPosForActiveObject() ;
      else if (statusOpened  == _ActiveObjectStatus)
        closeOpenObject() ;
    }
    // If we are there, it means that cursor is not there and there is no
    // active object... but this window still exists... means we are closing
    //
    else
      bWaitingForClosing = true ;

    // Don't close if active object still exists since it means that its
    // windows is opening and that we must wait for it to open before closing
    //
    if ((NsObjectIcon*) NULL == _ActiveObject)
    {
      string ps = string("Closing multitips window because cursor is out.")  ;
      if (bWaitingForClosing)
        ps = string("Multitips window waiting to be closed.")  ;
      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
      _pParent->mustCloseMultiTips() ;
    }

    return ;
  }

  // If cursor is inside opened popup menu, then nothing to do
  //
  if (isPointInPopup(curLoc))
    return ;

  // Get pointed object, if any
  //
  ScreenToClient(curLoc) ;
  NsObjectIcon* pPointedObject = getObjectFromPoint(curLoc) ;

  // If no pointed object, close displayed information
  //
  if ((NsObjectIcon*) NULL == pPointedObject)
  {
    if (NULL == _ActiveObject)
      return ;

    if (_pToolTip)
      _pToolTip->Hide() ;

    if ((statusOpened == _ActiveObjectStatus) || (statusOpening == _ActiveObjectStatus))
      closeOpenObject() ;

    _ActiveObject       = (NsObjectIcon*) 0 ;
    _ActiveObjectStatus = statusNone ;

    return ;
  }

  // Newly pointed object, just show its label
  //
  if ((NULL == _ActiveObject) || (pPointedObject != _ActiveObject))
  {
    if (_ActiveObject)
      closeOpenObject() ;

    _ActiveObject       = pPointedObject ;
    _ActiveObjectStatus = statusName ;

    _iNbTimerTicks = 1 ;
    _previousCursorLocation = curLoc ;

/*
    string sObjectLabel = _ActiveObject->getObject()->_pObjet->getTitre() ;

    if (_pToolTip)
    {
      NS_CLASSLIB::TRect rectDoc = _ActiveObject->getBox() ;
      rectDoc.right += ICONWIDTH ;
      _pToolTip->Show(rectDoc, &sObjectLabel, 0, -1, NULL, NULL, 10) ;
    }
    else
      openActiveObject() ;
*/

    return ;
  }

  if (curLoc != _previousCursorLocation)
  {
    _previousCursorLocation = curLoc ;
    _iNbTimerTicks = 0 ;
    return ;
  }

  _iNbTimerTicks++ ;

  if (_iNbTimerTicks < 3)
    return ;

  _ActiveObject = pPointedObject ;

  // Pointed object already active, open it or do nothing depending on status
  //
  if ((statusPutFront == _ActiveObjectStatus) ||
      (statusOpened   == _ActiveObjectStatus))
    return ;

  // After name, just a pause before opening document
  //
  if (statusName == _ActiveObjectStatus)
  {
    _ActiveObjectStatus = statusPause ;
    return ;
  }

  // Pointed object still opening, check its position
  //
  if (statusOpening == _ActiveObjectStatus)
  {
    setPosForActiveObject() ;
    return ;
  }

  openActiveObject() ;
}

void
NSLdvViewIconsShowWindow::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
try
{
  // Make certain that the focus doesn't get stuck in sub-area
  //
  THandle focusHdle = GetFocus() ;
  if (GetHandle() != focusHdle)
    SetFocus() ;

  // Get pointed object, if any
  //
  // ScreenToClient(point) ;

  NsObjectIcon* pPointedObject = getObjectFromPoint(point) ;
  if (NULL == pPointedObject)
    return ;

  NSLdvObjetView* pToon = pPointedObject->getObject() ;
  if (NULL == pToon)
    return ;

  _pParent->mustCloseMultiTips() ;

  _pParent->startDragging((NSLdvToon *) pToon, point, 1) ;
}
catch (...)
{
	erreur("Exception NSLdvViewIconsShowWindow EvLButtonDown.", standardError, 0) ;
}
}

void
NSLdvViewIconsShowWindow::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  NsObjectIcon* pPointedObject = getObjectFromPoint(point) ;
  if (NULL == pPointedObject)
    return ;

  _ActiveObject       = pPointedObject ;
  _ActiveObjectStatus = statusName ;

  NSLdvObjetView* pToon = pPointedObject->getObject() ;
  if (NULL == pToon)
    return ;

  NSLdvObjet* pLdvObject = pToon->getObject() ;
  if (NULL == pLdvObject)
    return ;

  string sDocumentId = pLdvObject->getReference() ;

  NSHISTODocument* pDocHis = pContexte->getPatient()->_pDocHis ;
  if (NULL == pDocHis)
    return ;

  DocumentIter docIter = pDocHis->TrouveDocHisto(sDocumentId) ;
  if (NULL == docIter)
    return ;

  NSDocumentHisto* pDocument = *docIter ;
  if (NULL == pDocument)
    return ;

  _pPopupMenu = new OWL::TPopupMenu() ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;
	string sOpen = pSuper->getText("documentManagement", "open") ;
  sOpen[0] = pseumaj(sOpen[0]) ;
  string sOpeA = pSuper->getText("documentManagement", "openEveryDocument") ;
  sOpeA[0] = pseumaj(sOpeA[0]) ;
  string sOpeE = pSuper->getText("documentManagement", "openExternalDocument") ;
  sOpeE[0] = pseumaj(sOpeE[0]) ;
	string sEdit = pSuper->getText("documentManagement", "edit") ;
  sEdit[0] = pseumaj(sEdit[0]) ;
	string sPara = pSuper->getText("documentManagement", "parameters") ;
  sPara[0] = pseumaj(sPara[0]) ;
	string sProp = pSuper->getText("documentManagement", "properties") ;
  sProp[0] = pseumaj(sProp[0]) ;
	string sRemo = pSuper->getText("documentManagement", "removeFromHistory") ;
  sRemo[0] = pseumaj(sRemo[0]) ;
	string sDele = pSuper->getText("documentManagement", "destroy") ;
  sDele[0] = pseumaj(sDele[0]) ;
  string sExpo = pSuper->getText("documentManagement", "exportAsXmlFile") ;
  sExpo[0] = pseumaj(sExpo[0]) ;
  string sReas = pSuper->getText("documentManagement", "reassignToAnotherPatient") ;
  sReas[0] = pseumaj(sReas[0]) ;
  string sSign = pSuper->getText("documentManagement", "forceAttributionOfThisDocument") ;
  sSign[0] = pseumaj(sSign[0]) ;
	//
	// This is not the synthesis
	//
	if (pDocument->getSemCont() != string("ZSYNT"))
	{
    // External document : the only menu is to open externally
    //
    if ((pDocument->getTypeSem() == string("ZDINT")) ||
        (pDocument->getTypeSem() == string("ZURL0")))
      _pPopupMenu->AppendMenu(MF_STRING, IDC_OPEN_EXT, sOpeE.c_str()) ;

    // Regular document
    //
    else
    {
  	  _pPopupMenu->AppendMenu(MF_STRING, IDC_OUVRIR, sOpen.c_str()) ;
      //
		  // Does the author have privileges to modify this document
      //
		  if ((pContexte->typeDocument(pDocument->getTypeSem(), NSSuper::isTree)) ||
			  	(pContexte->typeDocument(pDocument->getTypeSem(), NSSuper::isText)) ||
            (string("ZDHTM") == pDocument->getTypeSem()))
		  {
			  if (pContexte->userHasPrivilege(NSContexte::modifyDocument, -1, -1, pDocument->getAuthor(), string(""), NULL, NULL))
			  	_pPopupMenu->AppendMenu(MF_STRING, CM_EDITER, sEdit.c_str()) ;
			  else
				  _pPopupMenu->AppendMenu(MF_GRAYED, 0, sEdit.c_str()) ;
      }
      // Is this document connected to an external document
      //
      string sMetaDocId = pDocument->getCodeDocMeta() ;
      if (pContexte->getPatient()->getGraphPerson()->getExternalDocId(sMetaDocId) != string(""))
        _pPopupMenu->AppendMenu(MF_STRING, IDC_OPEN_EXT, sOpeE.c_str()) ;
    }

		// _pPopupMenu->AppendMenu(MF_STRING, IDC_PARAMETRES, sPara.c_str()) ;
		// _pPopupMenu->AppendMenu(MF_STRING, IDC_PROPRIETE,  sProp.c_str()) ;
    _pPopupMenu->AppendMenu(MF_STRING, IDC_XML_EXPORT, sExpo.c_str()) ;
		// _pPopupMenu->AppendMenu(MF_SEPARATOR, 0, 0) ;
		// _pPopupMenu->AppendMenu(MF_STRING, IDC_OTER,       sRemo.c_str()) ;
		// _pPopupMenu->AppendMenu(MF_SEPARATOR, 0, 0) ;
		// Does the author have privileges to suppres this document
		//
/*
		if (pContexte->userHasPrivilege(NSContexte::suppresDocument, -1, -1, pDocument->getAuthor(), string(""), NULL, NULL))
			_pPopupMenu->AppendMenu(MF_STRING, IDC_DETRUIRE,	sDele.c_str()) ;
    else
    	_pPopupMenu->AppendMenu(MF_GRAYED, 0,           	sDele.c_str()) ;
    if (NULL != pDocument->getData())
    {
      _pPopupMenu->AppendMenu(MF_SEPARATOR, 0, 0) ;
		  _pPopupMenu->AppendMenu(MF_STRING, IDC_TRANSFERT, sReas.c_str()) ;
      _pPopupMenu->AppendMenu(MF_STRING, IDC_FORCESIGN,  sSign.c_str()) ;
    }
*/
	}

  NS_CLASSLIB::TPoint lp = point ;
	ClientToScreen(lp) ;
	_pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;

	delete _pPopupMenu ;
  _pPopupMenu = (OWL::TPopupMenu*) 0 ;
}

NsObjectIcon*
NSLdvViewIconsShowWindow::getObjectFromPoint(NS_CLASSLIB::TPoint curLoc)
{
  if (_aObjects.empty())
    return (NsObjectIcon*) 0 ;

  vector<NsObjectIcon*>::iterator objectsIterator = _aObjects.begin() ;
  for ( ; _aObjects.end() != objectsIterator ; objectsIterator++)
  {
    NS_CLASSLIB::TRect objectBox = (*objectsIterator)->getBox() ;
    if (objectBox.Contains(curLoc))
      return *objectsIterator ;
  }

  return (NsObjectIcon*) 0 ;
}

bool
NSLdvViewIconsShowWindow::containsPoint(NS_CLASSLIB::TPoint curLoc)
{
  NS_CLASSLIB::TRect winRect = GetWindowRect() ;

  // Is curLoc inside this windows?
  //
  if (winRect.Contains(curLoc))
    return true ;

  // Is curLoc inside the popup menu opened by this windows?
  //
  return isPointInPopup(curLoc) ;
}

// Is curLoc inside the popup menu opened by this windows?
//
bool
NSLdvViewIconsShowWindow::isPointInPopup(NS_CLASSLIB::TPoint curLoc)
{
  if (NULL == _pPopupMenu)
    return false ;

  NSMDIFrameBase* pFrame = pContexte->getSuperviseur()->getApplication()->getFrame() ;
  HWND hPopup = pFrame->GetPopupMenuWindowHandle() ;
  if ((HWND) NULL == hPopup)
    return false ;

  TWindow popuMenuWnd(hPopup) ;
  if (false == popuMenuWnd.IsWindow())
    return false ;

  NS_CLASSLIB::TRect popupRect = popuMenuWnd.GetWindowRect() ;
  if (popupRect.Contains(curLoc))
    return true ;

  return false ;
}

void
NSLdvViewIconsShowWindow::openActiveObject()
{
  // If no object or object that was already opened, nothing to do
  //
  if ((NULL == _ActiveObject) || (statusPutFront == _ActiveObjectStatus) || (statusOpened == _ActiveObjectStatus))
    return ;

  NSLdvObjetView* pLdvObjectView = _ActiveObject->getObject() ;
  if (NULL == pLdvObjectView)
    return ;

  _ActiveObjectStatus = _pParent->openObject(pLdvObjectView) ;
}

void
NSLdvViewIconsShowWindow::setPosForActiveObject()
{
  // If no object or object that was already opened, nothing to do
  //
  if ((NULL == _ActiveObject) || (statusPutFront == _ActiveObjectStatus) || (statusOpened == _ActiveObjectStatus))
    return ;

  string ps = string("NSLdvViewIconsShowWindow::setPosForActiveObject(), entering.")  ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  // Get Documents manager
  //
  NSHISTODocument* pDocHis = _pParent->getDocumentsManager() ;
  if (NULL == pDocHis)
  {
    ps = string("NSLdvViewIconsShowWindow::setPosForActiveObject(), can't find documents manager, leaving.")  ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // Get open object in documents manager
  //
  NSDocumentInfo* pDocInfo = getDocumentInfo(_ActiveObject) ;
  if (NULL == pDocInfo)
  {
    ps = string("NSLdvViewIconsShowWindow::setPosForActiveObject(), can't find NSDocumentInfo for opened document, leaving.")  ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // Check if document is already opened
  //
  NSNoyauDocument* pDocNoy = pDocHis->EstUnDocumentOuvert(pDocInfo) ;

  TWindow* pOpenedWindows = _pParent->getDocViewWindow(pDocNoy) ;
  if ((TWindow*) NULL == pOpenedWindows)
  {
    ps = string("NSLdvViewIconsShowWindow::setPosForActiveObject(), can't find open document's window, leaving.")  ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    return ;
  }

  while (pOpenedWindows)
  {
    TMDIChild *pMdiChild = TYPESAFE_DOWNCAST(pOpenedWindows, TMDIChild) ;
		if (pMdiChild)
    {
      setPosForActiveObjectWindow(pMdiChild) ;

      ps = string("NSLdvViewIconsShowWindow::setPosForActiveObject(), done.")  ;
      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

      return ;
    }

    pOpenedWindows = pOpenedWindows->Parent ;
  }

  ps = string("NSLdvViewIconsShowWindow::setPosForActiveObject(), no proper window found, leaving.")  ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
}

void
NSLdvViewIconsShowWindow::setPosForActiveObjectWindow(TWindow* pMdiWindow)
{
  if (NULL == pMdiWindow)
    return ;

  // Make certain that opened status is set (so better do it early on)
  //
  _ActiveObjectStatus = statusOpened ;

  // Get Windows positions in order to optimize their placements
  //
  NS_CLASSLIB::TRect iconsWinRect = GetWindowRect() ;
  NS_CLASSLIB::TRect viewWinRect  = pMdiWindow->GetWindowRect() ;
  if (false == viewWinRect.Touches(iconsWinRect))
    return ;

  // First check where there is more room
  //
  TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if (NULL == pMyApp)
    return ;
  NSMDIClient *pMDIClient = pMyApp->prendClient() ;
  if (NULL == pMDIClient)
    return ;

  NS_CLASSLIB::TRect mdiClientRect = pMDIClient->GetWindowRect() ;

  int iLeftSpace  = iconsWinRect.X() - mdiClientRect.X() ;
  int iRightSpace = mdiClientRect.Width() - iconsWinRect.Width() - iLeftSpace ;

  int iUpperSpace = iconsWinRect.Y() - mdiClientRect.Y() ;
  int iLowerSpace = mdiClientRect.Height() - iconsWinRect.Height() - iUpperSpace ;

  int iNewX = viewWinRect.X() ;
  int iNewY = iconsWinRect.Y() - viewWinRect.Height() ;
  int iNewH = viewWinRect.Height() ;  // 400 ;
  int iNewW = viewWinRect.Width() ;   // 600 ;

  // Favor upper right to lower left
  //
  if (iUpperSpace > viewWinRect.Height())
  {
    iNewY = iUpperSpace - viewWinRect.Height() ;

    // There is room to the right
    //
    if (iLeftSpace + viewWinRect.Width() > viewWinRect.Width())
      iNewX = iconsWinRect.X() ;

    iNewX = max(0, mdiClientRect.Width() - viewWinRect.Width()) ;
  }
  // No room above; is there room under icons window ?
  //
  else if (iLowerSpace > viewWinRect.Height())
  {
    iNewY = iUpperSpace + viewWinRect.Height() ;

    // There is room to the right
    //
    if (iLeftSpace + viewWinRect.Width() > viewWinRect.Width())
      iNewX = iconsWinRect.X() ;

    iNewX = max(0, mdiClientRect.Width() - viewWinRect.Width()) ;
  }


  if (iNewY < 0)
  {
    iNewY = 5 ;
    iNewH = iconsWinRect.Y() - iNewY ;
  }

  pMdiWindow->MoveWindow(iNewX, iNewY, iNewW, iNewH, true) ;
}

void
NSLdvViewIconsShowWindow::closeOpenObject()
{
  // If no object or object that was not opened, nothing to do
  //                             statusPutFront
  if (NULL == _ActiveObject)
    return ;

  string ps = string("Entering NSLdvViewIconsShowWindow::closeOpenObject. Status is ") + getHitToonStatusLabel(_ActiveObjectStatus) ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  if ((statusOpened != _ActiveObjectStatus) && (statusPutFront != _ActiveObjectStatus))
  {
    ps = string("Entering NSLdvViewIconsShowWindow::closeOpenObject. Not a proper status, leaving") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    return ;
  }

  NsObjectIcon* pToCloseObject = _ActiveObject ;

  _ActiveObject       = (NsObjectIcon*) 0 ;
  _ActiveObjectStatus = statusNone ;

  // If status is statusPutFront, it means that document was already opened
  // before tooltip activation; reseting variables is enough
  //
  if (statusPutFront == _ActiveObjectStatus)
  {
    ps = string("Entering NSLdvViewIconsShowWindow::closeOpenObject, done.") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // Get Documents manager
  //
  NSHISTODocument* pDocHis = _pParent->getDocumentsManager() ;
  if (NULL == pDocHis)
  {
    ps = string("NSLdvViewIconsShowWindow::closeOpenObject cannot find documents manager, leaving.")  ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // Get open object in documents manager
  //
  NSDocumentInfo* pDocInfo = getDocumentInfo(pToCloseObject) ;
  if (NULL == pDocInfo)
  {
    ps = string("NSLdvViewIconsShowWindow::closeOpenObject cannot find documents NSDocumentInfo for document to close, leaving.")  ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // Get opened document
  //
  NSNoyauDocument* pDocNoy = pDocHis->EstUnDocumentOuvert(pDocInfo) ;
  if (NULL == pDocNoy)
  {
    ps = string("NSLdvViewIconsShowWindow::closeOpenObject cannot find document in open documents manager, leaving.")  ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // Get window to close
  //
  TWindow* pOpenedWindows = _pParent->getDocViewWindow(pDocNoy) ;
  if (NULL == pOpenedWindows)
  {
    ps = string("NSLdvViewIconsShowWindow::closeOpenObject cannot find popup window, leaving.")  ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // Tell Documents manager to consider this document as closed
  //
  pDocHis->FermetureDocument(pDocInfo) ;

  // Close window
  //
  pOpenedWindows->CloseWindow() ;

  ps = string("NSLdvViewIconsShowWindow::closeOpenObject done.")  ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
}

void
NSLdvViewIconsShowWindow::resetObjects()
{
  if (_aObjects.empty())
    return ;

  vector<NsObjectIcon*>::iterator objectsIterator = _aObjects.begin() ;

  for ( ; _aObjects.end() != objectsIterator ; )
  {
    delete *objectsIterator ;
    _aObjects.erase(objectsIterator) ;
  }
}

void
NSLdvViewIconsShowWindow::addObject(NSLdvObjetView* pObject)
{
  _aObjects.push_back(new NsObjectIcon(pObject)) ;
}

void
NSLdvViewIconsShowWindow::XmlExport()
{
  NSDocumentHisto* pActiveDocHisto = getHistoDocumentForActiveObject() ;
  if (NULL == pActiveDocHisto)
    return ;

  pContexte->getPatient()->DocumentXmlExport(pActiveDocHisto) ;
}

void
NSLdvViewIconsShowWindow::Ouvrir()
{
  // Make certain there is an object
  //
  if ((NULL == _ActiveObject) || (NULL == _ActiveObject->getObject()))
    return ;

  NSLdvObjet* pLdvObject = _ActiveObject->getObject()->getObject() ;
  if (NULL == pLdvObject)
    return ;

  // Get access to NSLdvDocument
  //
  if ((NULL == _pParent) || (NULL == _pParent->getView()))
    return ;

  NSLdvDocument *pLdvDoc = _pParent->getView()->getDoc() ;
  if (NULL == pLdvDoc)
    return ;

  pLdvDoc->openLdVObject(pLdvObject) ;
}

void
NSLdvViewIconsShowWindow::Editer()
{
  NSDocumentHisto* pActiveDocHisto = getHistoDocumentForActiveObject() ;
  if (NULL == pActiveDocHisto)
    return ;

  NSHISTODocument* pDocHisto = pContexte->getPatient()->getDocHis() ;
  if (NULL == pDocHisto)
    return ;

  pDocHisto->AutoriserEdition(pActiveDocHisto) ;
}

NSDocumentInfo*
NSLdvViewIconsShowWindow::getDocumentInfo(NsObjectIcon* pObjectIcon)
{
  if (NULL == pObjectIcon)
    return (NSDocumentInfo*) 0 ;

  // Get document ID
  //
  NSLdvObjetView* pLdvObjectView = pObjectIcon->getObject() ;
  if (NULL == pLdvObjectView)
    return (NSDocumentInfo*) 0 ;

  return _pParent->getDocumentInfo(pLdvObjectView) ;
}

NSDocumentHisto*
NSLdvViewIconsShowWindow::getHistoDocumentForActiveObject()
{
  if (NULL == _ActiveObject)
    return (NSDocumentHisto*) 0 ;

  NSDocumentInfo* pDocInfo = getDocumentInfo(_ActiveObject) ;
  if (NULL == pDocInfo)
		return (NSDocumentHisto*) 0 ;

  NSHISTODocument* pDocHis = pContexte->getPatient()->getDocHis() ;
  if (NULL == pDocHis)
		return (NSDocumentHisto*) 0 ;

  DocumentIter iterDoc = pDocHis->TrouveDocHisto(pDocInfo->getID()) ;
  if (NULL == iterDoc)
		return (NSDocumentHisto*) 0 ;

  return *iterDoc ;
}

// -----------------------------------------------------------------------------
// ------------------------- NSLdvViewMagnifier --------------------------
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSLdvViewMagnifier, TWindow)
  EV_WM_MOUSEMOVE,
  EV_WM_PAINT,
  EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

NSLdvViewMagnifier::NSLdvViewMagnifier(NSContexte* pCtx, TWindow* parent, NVLdVPoint& centerPoint, NSLigneView* pLine)
                   :TWindow(parent), NSRoot(pCtx), _centerPoint(centerPoint), _previousPoint(NS_CLASSLIB::TPoint(0, 0))
{
  lObjectCount++ ;

  _pParent = (NSLdvViewAreaWindow*) parent ;
  _pLine   = pLine ;

  _iOriginalWidth  = 40 ;
  _iOriginalHeight = 40 ;

  _iZoomFactor = 10 ;

  _previousPoint = NS_CLASSLIB::TPoint((_iOriginalWidth * _iZoomFactor) / 2 , (_iOriginalHeight * _iZoomFactor) / 2) ;
}

NSLdvViewMagnifier::~NSLdvViewMagnifier()
{
  lObjectCount-- ;
}

void
NSLdvViewMagnifier::SetupWindow()
{
  TWindow::SetupWindow() ;

  initPixels() ;
}

void
NSLdvViewMagnifier::EvPaint()
{
  TPaintDC dc(*this) ;
  Paint(dc, true, GetClientRect()) ;
}

void
NSLdvViewMagnifier::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectToPaint)
{
  TWindow::Paint(dc, erase, RectToPaint) ;

  NS_CLASSLIB::TPoint physicalPoint = _pParent->getAreaPhysicalPoint(_centerPoint) ;

  int iPhysicalLeft = physicalPoint.X() - (_iOriginalWidth / 2) ;
  int iPhysicalTop  = physicalPoint.Y() - (_iOriginalHeight / 2) ;

  NS_CLASSLIB::TRect  physicalRect  = NS_CLASSLIB::TRect(iPhysicalLeft,
                                                         iPhysicalTop,
                                                         iPhysicalLeft + _iOriginalWidth,
                                                         iPhysicalTop + _iOriginalHeight) ;

  TDib* pDib = _origBitmap.getDib() ;

  int iDibHeight = pDib->Height() ;

  // BITMAPINFO *pBitmapInfo = pDib->GetInfo() ;

  NS_CLASSLIB::TRect  dibRect  = NS_CLASSLIB::TRect(iPhysicalLeft,
                                                    pDib->Height() - iPhysicalTop - _iOriginalHeight,
                                                    iPhysicalLeft + _iOriginalWidth,
                                                    pDib->Height() - iPhysicalTop) ;

  NS_CLASSLIB::TRect originalRect = NS_CLASSLIB::TRect(0, 0, _iOriginalWidth * _iZoomFactor, _iOriginalHeight * _iZoomFactor) ;

  bool bResult = dc.StretchDIBits(originalRect, dibRect, *pDib) ;

  PaintDecorations(dc, RectToPaint) ;
}

void
NSLdvViewMagnifier::PaintDecorations(TDC& dc, NS_CLASSLIB::TRect& RectToPaint)
{
  if ((NSLigneView*) NULL == _pLine)
    return ;

  NSConcernView *pConcernView = dynamic_cast<NSConcernView*>(_pLine) ;
  if (pConcernView)
    PaintQoLDecorations(dc, RectToPaint, pConcernView) ;
}

void
NSLdvViewMagnifier::PaintQoLDecorations(TDC& dc, NS_CLASSLIB::TRect& RectToPaint, NSConcernView *pConcernView)
{
  if ((NSConcernView*) NULL == pConcernView)
    return ;

  NSConcern* pConcern = pConcernView->getConcern() ;
  if ((NSConcern*) NULL == pConcern)
    return ;

  NSQualityOfLifeChart* pQolChart = pConcern->getQualityOfLifeChartAtTime(_centerPoint.getX()) ;
  if ((NSQualityOfLifeChart*) NULL == pQolChart)
    return ;

  NSQualityOfLifeInformation* pQolInfo = pQolChart->getQolInfo() ;
  if ((NSQualityOfLifeInformation*) NULL == pQolInfo)
    return ;

  NSCoopLineArray* pLines = pQolInfo->getLines() ;
  if (((NSCoopLineArray*) NULL == pLines) || pLines->empty())
    return ;

  int            iLineIndex   = pConcernView->getIndex() ;
  NSLdvViewArea* pWorkingArea = pConcernView->getWorkingArea() ;

  // Top and bottom of considered concern line
  //
  int iTopLine    = pWorkingArea->preoccupDonneTopLine(iLineIndex) ;
  int iBottomLine = pWorkingArea->preoccupDonneBottomLine(iLineIndex) ;

  // Rectangle QoL decorations are drawn into
  //
  NVLdVPoint QofL_TopLeft(_centerPoint) ;
  QofL_TopLeft.setX(pQolChart->getTpsStart()) ;
  QofL_TopLeft.setY(iTopLine - NSConcernView::getQolTopMargin()) ;

  NVLdVPoint QofL_BottomRight(_centerPoint) ;
  QofL_BottomRight.setX(pQolChart->getTpsInfo()) ;
  QofL_BottomRight.setY(iBottomLine) ;

  // Location of this rectangle inside working area
  //
  NS_CLASSLIB::TPoint ptQoL_TL = pWorkingArea->getScrollablePhysicalPoint(QofL_TopLeft) ;
  NS_CLASSLIB::TPoint ptQoL_BR = pWorkingArea->getScrollablePhysicalPoint(QofL_BottomRight) ;

  // Center point expressed in x,y coordinates
  //
  NS_CLASSLIB::TPoint centerPoint = pWorkingArea->getAreaPhysicalPoint(_centerPoint) ;

  dc.SelectObject(OWL::TFont("MS Sans Serif", -9)) ;

  //
  //
  int iLineNb = 0 ;
  for (CoopLineIter lineIt = pLines->begin() ; pLines->end() != lineIt ; lineIt++, iLineNb++)
  {
    NS_CLASSLIB::TRect lineRect = getRectForQolLine(iLineNb, NS_CLASSLIB::TRect(ptQoL_TL, ptQoL_BR), centerPoint) ;

    int iLeft = lineRect.Left() ;
    if (iLeft < 0)
      iLeft = 0 ;

    int iWindowWidth = GetWindowRect().Width() ;
    int iRight       = lineRect.Right() ;
    if (iRight > iWindowWidth)
      iRight = iWindowWidth ;

    NS_CLASSLIB::TRect InWindowRect = NS_CLASSLIB::TRect(iLeft, lineRect.Top(), iRight, lineRect.Bottom()) ;

    dc.DrawText((*lineIt)->getTitle().c_str(), -1, InWindowRect, DT_CENTER) ;
  }
}

void
NSLdvViewMagnifier::initPixels()
{
  NS_CLASSLIB::TRect physicalRect = _pParent->GetWindowRect() ;

  TWindowDC * pWinDC = new TWindowDC(_pParent->HWindow) ;
  _origBitmap.ScreenCapture(pWinDC, &physicalRect) ;
  delete pWinDC ;

/*
  TClipboard clipboard(_pParent->HWindow) ;
  TDib* pDib = _origBitmap.getDib() ;
  pDib->ToClipboard(clipboard) ;
  clipboard.CloseClipboard() ;
*/
}

int
NSLdvViewMagnifier::getOriginalCoordinate(int iLocalCoordinate)
{
  return (int) floor(double(iLocalCoordinate) / double(_iZoomFactor)) ;
}

NS_CLASSLIB::TRect
NSLdvViewMagnifier::getLocalRectFromLdvRect(const NS_CLASSLIB::TRect& QolRect, const NS_CLASSLIB::TPoint& centerPoint)
{
  int iLdvLeft = centerPoint.X() - (_iOriginalWidth / 2) ;
  int iLdvTop  = centerPoint.Y() - (_iOriginalHeight / 2) ;

  NS_CLASSLIB::TRect ldvRect = QolRect.OffsetBy(-iLdvLeft, -iLdvTop) ;

  return NS_CLASSLIB::TRect(ldvRect.Left()   * _iZoomFactor,
                            ldvRect.Top()    * _iZoomFactor,
                            ldvRect.Right()  * _iZoomFactor,
                            ldvRect.Bottom() * _iZoomFactor) ;
}

NS_CLASSLIB::TRect
NSLdvViewMagnifier::getRectForQolLine(int iLine, const NS_CLASSLIB::TRect& QolRect, const NS_CLASSLIB::TPoint& centerPoint)
{
  int iLineTop = NSConcernView::getYForQolLine(QolRect.Top(), iLine) ;

  NS_CLASSLIB::TRect rectInLdvCoord = NS_CLASSLIB::TRect(QolRect.Left(),
                                                         iLineTop,
                                                         QolRect.Right(),
                                                         iLineTop + NSConcernView::getQolLineHeight()) ;

  return getLocalRectFromLdvRect(rectInLdvCoord, centerPoint) ;
}

void
NSLdvViewMagnifier::EvMouseMove(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
  if (_previousPoint == point)
    return ;

  Destroy() ;

  _pParent->mustCloseMagnifier() ;

  // _previousPoint = NS_CLASSLIB::TPoint(0, 0) ;
}

void
NSLdvViewMagnifier::EvKillFocus(HWND hWndGetFocus)
{
  Destroy() ;

  _pParent->mustCloseMagnifier() ;
}

