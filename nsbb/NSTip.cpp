////////////////////////////////////////////////////////////////////////////// Adapted from
// TitleTip.cpp : implementation file
//
// Adapted from code written by Zafir Anjum
//
// Modifed 10 Apr 1999  Now accepts a LOGFONT pointer and
// a tracking rect in Show(...)  (Chris Maunder)
// 18 Apr 1999  Resource leak in Show fixed by Daniel Gehriger
// 7 Jan 2000  Added multiline capabilities, and the ability to
// specify the maximum length of the tip (Mark Findlay)

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

#include "nautilus\nssuper.h"
// #include "nsdn\nsdochis.h"
// #include "nautilus\nsldvvue.h"
#include "nsbb\NSTip.h"

#include <windef.h>

#define TITLETIP_CLASSNAME "ZTitleTip"

#define MARGE_GCHE  3
#define MARGE_DRTE  3
#define MARGE_HAUTE 1
#define MARGE_BAS   1

DEFINE_RESPONSE_TABLE1(NSTitleTip, TWindow)
  	EV_WM_LBUTTONDBLCLK,  //double click    EV_WM_RBUTTONDBLCLK,  //double click    EV_WM_MOUSEMOVE,    EV_WM_MBUTTONDOWN,    EV_WM_RBUTTONDOWN,    EV_WM_TIMER,
  	EV_WM_PAINT,    EV_WM_LBUTTONDOWN,
    EV_WM_LBUTTONUP,
END_RESPONSE_TABLE;

NSTitleTip::NSTitleTip(TWindow* pParent, NSContexte* pCtx)
           :TWindow(pParent), NSRoot(pCtx)
{
	// Enregistre la fenêtre parent
	pParentWnd = pParent ;

	// définit le style de la fenêtre (sans bordure)
	uint32 style = GetStyle() ;
	SetStyle(style |= WS_BORDER | WS_POPUP) ;

	// Définition des styles étendu
	uint32 styleEx = GetExStyle() ;
	SetExStyle(styleEx |= WS_EX_TOPMOST | WS_EX_TOOLWINDOW) ;

	// On cherche si on a des skin à appliquer au toolTip
	if (pContexte->getSkins())		pSkin = pContexte->getSkins()->donneSkin("ldvTips") ;	else		pSkin = 0 ;}

NSTitleTip::~NSTitleTip(){	// Détruit le Timer si il existe	killTipsTimer() ;
}
voidNSTitleTip::SetupWindow(){	TWindow::SetupWindow() ;	ShowWindow( SW_HIDE ) ;	// TODO FIXME Charger les skins}
// Creation de la fenêtreboolNSTitleTip::Create()
{
	return TWindow::Create() ;
}

// Show 		 - Show the titletip if needed// rectTitle	 - The rectangle within which the original//					title is constrained - in client coordinates// lpszTitleText - The text to be displayed// xoffset		 - Number of pixel that the text is offset from//				   left border of the cellvoid
NSTitleTip::EvTimer(uint timerId)
{
  if (ID_HT_TIMER != timerId)
    return ;

  if (-1 == time_to)
    return ;

  if (timercount < time_to)
    timercount++ ;
  else
	{
		killTipsTimer() ;
    Hide() ;
	}
}

voidNSTitleTip::Show(NS_CLASSLIB::TRect rectTitle,											string* psText,                      int xoffset /*= 0*/,
                      int nMaxChars /*= -1*/,
                      NS_CLASSLIB::TRect* pHoverRect /*= NULL*/,
                      LPLOGFONT lpLogFont /*= NULL*/,
                      int time /* = -1 */)
{
	// Declenche l'auto fermeture de la fenetre au bout d'un laps de temps
  //
  timercount = 0 ;
  time_to    = time ;
  if (-1 != time)
    setTipsTimer() ;

	if (rectTitle.IsEmpty())
		return ;

  GetCursorPos(MouseLoc) ;

	// Si le toolTip est visible
	if( IsWindowVisible() )
		return ;

	rectHover = (pHoverRect != NULL)? *pHoverRect : rectTitle ;
	rectHover.right++ ;
  rectHover.bottom++ ;

  NS_CLASSLIB::TPoint ptTL ;
  NS_CLASSLIB::TPoint ptBR ;

	// Ne pas afficher si la fenetre n'a pas le focus
	if( GetFocus() == NULL )
		return ;

	// Define the rectangle outside which the titletip will be hidden.
	// We add a buffer of one pixel around the rectangle
	rectTitle.top	 		= rectTitle.top - 1 ;
	rectTitle.left   = rectTitle.left - xoffset - 1 ;
	rectTitle.right  = rectTitle.right + rectTitle.Width() - xoffset ;
	rectTitle.bottom = rectTitle.top + rectTitle.Height() + 1 ;

	// On détermine la taille du texte
  ptTL = rectTitle.TopLeft() ;
  pParentWnd->ClientToScreen( ptTL ) ;
  ptBR = rectTitle.BottomRight() ;
  pParentWnd->ClientToScreen( ptBR ) ;
  rectTitle = NS_CLASSLIB::TRect(ptTL, ptBR) ;

	TClientDC dc(*this) ;
	strTitle = string(*psText) ;

  TFont *pOldFont = NULL ;

	if (lpLogFont)
	{
  	TFont font(lpLogFont) ;
    dc.SelectObject( font ) ;
	}
  else if (pSkin && pSkin->getLogFont())
  {
  	TFont font(pSkin->getLogFont()) ;
    dc.SelectObject( font ) ;
  }

  TEXTMETRIC tm ;
	dc.GetTextMetrics(tm) ;

  //
  // Mesure de la plus grande chaine
  //
  int iCx = 0 ;
  int iCy = 0 ;

  size_t iCr = strTitle.find('\n') ;
  if (NPOS == iCr)
  {
    NS_CLASSLIB::TSize size = dc.GetTextExtent( strTitle.c_str(), strlen(strTitle.c_str()) ) ;
    iCx = size.cx ;
    iCy = size.cy ;
  }
  else
  {
  	int iDeb = 0;
    while (NPOS != iCr)
    {
    	string sSubString = string(strTitle, iDeb, iCr - iDeb) ;
      NS_CLASSLIB::TSize size = dc.GetTextExtent(sSubString.c_str(), strlen(sSubString.c_str()) ) ;
      if (size.cx > iCx)
      	iCx = size.cx ;
      iCy += size.cy ;

      iDeb = iCr + 1 ;
      iCr = strTitle.find('\n', iDeb) ;
    }
    string sSubString = string(strTitle, iDeb, strlen(strTitle.c_str()) - iDeb) ;
    NS_CLASSLIB::TSize size = dc.GetTextExtent(sSubString.c_str(), strlen(sSubString.c_str()) ) ;
    if (size.cx > iCx)
    	iCx = size.cx ;
  	iCy += size.cy ;
	}

	iCx += tm.tmOverhang ;

	if (pOldFont)
  	dc.SelectObject( *pOldFont ) ;

	rectDisplay = rectTitle ;
	rectDisplay.left += xoffset-1 ;
	rectDisplay.top  += 2 ;
	rectDisplay.right = rectDisplay.left + iCx + xoffset ;
	rectDisplay.bottom = rectDisplay.top + iCy ;

	// We will use avg char width to set max tooltip width
	int nMaxTooltipWidth = -1 ;
	if (nMaxChars > 0)
	{
		int nMaxTooltipWidth = (tm.tmAveCharWidth * nMaxChars) ;
		if (nMaxTooltipWidth < 0)
    	nMaxTooltipWidth *= -1 ;

		// Rect display to be set to max chars
		if (rectDisplay.Width() > nMaxTooltipWidth)
    	rectDisplay.right = rectDisplay.left + nMaxTooltipWidth ;
	}

	//***************************************************************************************
	//Adjust the dimensions of the rect to fit within the client

	// Get the coordinates of the parents client area. (In this case the ListView's client
	// area) and convert coordinates to those of the tooltip.
	NS_CLASSLIB::TRect rectClient ;
	pParentWnd->GetClientRect( rectClient ) ;

	ptTL = rectClient.TopLeft() ;
  pParentWnd->ClientToScreen( ptTL ) ;
  ptBR = rectClient.BottomRight() ;
  pParentWnd->ClientToScreen( ptBR ) ;
  rectClient = NS_CLASSLIB::TRect(ptTL, ptBR) ;

  //pParentWnd->ClientToScreen( rectClient );

	// ------------------------------------------------------------------------------
	// Use the screen's right edge as the right hand border, not the right edge of the client.
	// You can comment this out to use the right client as the border.
	TWindowDC wdc(NULL) ;
	rectClient.right = GetDeviceCaps(wdc, HORZRES) - 8 ;
	rectClient.bottom = GetDeviceCaps(wdc, VERTRES) - 8 ;
	//---------------------------------------------------------------------------------------


	//If the right edge exceeds the right edge of the client:
  //          see how much room there is to move the display to the left and adjust the
  //          rectangle that far to the left. If the rect still exceeds the right edge, clip
  //          the right edge to match the client right edge.
  //
  // Does the right display edge exceed the right client edge?
  if (rectDisplay.right > rectClient.right)
  {
  	// establish what is available left shift wise and what is needed
    int nAvail = 0 ;
    int nNeeded = rectDisplay.right - rectClient.right ;

    if (rectDisplay.left > rectClient.left)
    	nAvail = rectDisplay.left - rectClient.left ;

    // is there room to move left?
    if (nAvail >= nNeeded)
    {
    	rectDisplay.Offset(-nNeeded, 0) ;  // yes, move all that is needed
      // increase the size of the window that will be inspected to see if the
      // cursor has gone outside of the tooltip area by the number of units we
      // offset our display rect.
      rectTitle.right += nNeeded ;
    }
    else
    {
    	rectDisplay.Offset(-nAvail, 0) ; // no, at least move to left edge of client
      // increase the size of the window that will be inspected to see if the
      // cursor has gone outside of the tooltip area by the number of units we
      // offset our display rect.
      rectTitle.right += nAvail ;
    }

    // Did we move enough? If not, clip right edge to match client right edge
    if (rectDisplay.right > rectClient.right)
    	rectDisplay.right = rectClient.right ;
	}

	//If the left edge exceeds the left edge of the client:
  //          see how much room there is to move the display to the right and adjust the
  //          rectangle that far to the right. If the rect still exceeds the left edge, clip
  //          the left edge to match the client left edge.
  //
  // Does the left display edge exceed the left client edge?
  if (rectDisplay.left < rectClient.left)
  {
  	// establish what is available right shift wise and what is needed
    int nAvail = 0 ;
    int nNeeded = rectClient.left - rectDisplay.left ;

    if (rectDisplay.right < rectClient.right)
    	nAvail = rectClient.right - rectDisplay.right ;

    // is there room to move left?
    if (nAvail >= nNeeded)
    {
    	rectDisplay.Offset(+nNeeded, 0) ;  // yes, move all that is needed
      // increase the size of the window that will be inspected to see if the
      // cursor has gone outside of the tooltip area by the number of units we
      // offset our display rect.
      rectTitle.left -= nNeeded ;
    }
    else
    {
    	rectDisplay.Offset(+nAvail, 0) ;  // no, at least move to left edge of client
      // increase the size of the window that will be inspected to see if the
      // cursor has gone outside of the tooltip area by the number of units we
      // offset our display rect.
      rectTitle.left -= nAvail ;
    }

    // Did we move enough? If not, clip left edge to match client left edge
    if (rectDisplay.left < rectClient.left)
    	rectDisplay.left = rectClient.left ;
  }

	// if the calculated width > maxwidth set above then truncate
	if (nMaxTooltipWidth > 0 && rectDisplay.Width() > nMaxTooltipWidth)
  	rectDisplay.right = rectDisplay.left + nMaxTooltipWidth ;

	//***************************************************************************************
  // Use a "work" rect to calculate the bottom. This work rect will be inset
  // slightly from the rect we have just created so the tooltip does not touch
  // the sides.
  NS_CLASSLIB::TRect rectCalc = rectDisplay ;

  rectCalc.top += 1 ;

  int nHeight = dc.DrawText(strTitle.c_str(), -1, rectCalc, 0
        | DT_CALCRECT
        | DT_LEFT
        | DT_NOCLIP
        | DT_EDITCONTROL
        | DT_NOPREFIX
        | DT_WORDBREAK
        ) ;

	// If this is a single line, shorten the display to get rid of any excess blank space
	if (nHeight == tm.tmHeight)
  	rectCalc.right = rectCalc.left + iCx + 3 ;

	rectDisplay.bottom = rectDisplay.top + nHeight ;

	// ensure the tooltip does not exceed the bottom of the screen
	if (rectDisplay.bottom > rectClient.bottom)
  {
		rectDisplay.bottom = rectClient.bottom ;
    rectCalc.bottom = rectClient.bottom ;
  }

  NS_CLASSLIB::TRect rectShow = NS_CLASSLIB::TRect(rectDisplay.left - MARGE_GCHE,
                                        rectDisplay.top - MARGE_HAUTE,
                                        rectDisplay.left + rectDisplay.Width() + MARGE_DRTE,
                                        rectDisplay.top + rectDisplay.Height() + MARGE_BAS) ;

	pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;

	SetWindowPos( /*&wndTop*/ GetTopWindow(), rectShow,
        //rectDisplay.left-3,
        //rectDisplay.top-1,
		//rectDisplay.Width()+3,
        //rectDisplay.Height(),
		SWP_SHOWWINDOW|SWP_NOACTIVATE ) ;

	ShowWindow( SW_SHOW ) ;

	disableTipTimer() ;

	SetCapture() ;
}

voidNSTitleTip::Hide(){   /*  	if (!::IsWindow(GetSafeHwnd()))		return; */
	enableTipTimer() ;
	if (GetCapture() == *this)		ReleaseCapture() ;	ShowWindow( SW_HIDE ) ;}
voidNSTitleTip::EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  NS_CLASSLIB::TPoint curLoc;
  GetCursorPos(curLoc);
  if (curLoc == MouseLoc)
    return ;

	// On récupère le point en coordonnées locales, alors que rectHover est
  // en coordonnées locales du parent
  NS_CLASSLIB::TPoint pointParent = point ;
  ClientToScreen( pointParent ) ;
  pParentWnd->ScreenToClient( pointParent ) ;

  if (!rectHover.Contains(pointParent))
	{
  	Hide() ;

		// Forward the message
		ClientToScreen( point ) ;
    THandle hWindow = WindowFromPoint(point) ;
		TWindow* pWnd ;
		if ( hWindow == GetHandle() )
			pWnd = pParentWnd ;
    else
    	pWnd = new TWindow(hWindow) ;

	 	int hittest = (int)pWnd->SendMessage(WM_NCHITTEST, 0, (MAKELONG((WORD)(point.x),(WORD)(point.y)))) ;
		if (HTCLIENT == hittest)
    {
			pWnd->ScreenToClient( point ) ;
    	pWnd->PostMessage( WM_MOUSEMOVE, hittest, MAKELONG((WORD)(point.x), (WORD)(point.y)) ) ;
		}
    else
			pWnd->PostMessage( WM_NCMOUSEMOVE, hittest, MAKELONG((WORD)(point.x), (WORD)(point.y)) ) ;

    if ( hWindow != GetHandle() )
    	delete pWnd ;
	}

  TWindow::EvMouseMove(modKeys, point) ;
}

voidNSTitleTip::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point){	TWindow *pWnd ;
	int hittest ;
	ClientToScreen( point ) ;
	THandle hWindow = WindowFromPoint( point ) ;
	if ( hWindow == GetHandle() )  	pWnd = pParentWnd ;  else  	pWnd = new TWindow(hWindow) ;
	hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y)) ;
	MSG Msg ;
	if (hittest == HTCLIENT)	{  	pWnd->ScreenToClient( point ) ;    Msg.message = WM_LBUTTONDOWN ;    Msg.wParam = modKeys ;    Msg.lParam = MAKELONG(point.x,point.y) ;  }  else  {  	Msg.message = WM_NCLBUTTONDOWN ;    Msg.wParam = hittest ;    Msg.lParam = MAKELONG(point.x,point.y) ;  }
	// Hide();	pWnd->PostMessage(Msg.message, Msg.wParam, Msg.lParam) ;
	if ( hWindow != GetHandle() )  	delete pWnd ;
  TWindow::EvLButtonDown(modKeys, point) ;

	return ;}

voidNSTitleTip::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TWindow *pWnd ;
	int hittest ;

	ClientToScreen( point ) ;	THandle hWindow = WindowFromPoint( point ) ;

	if ( hWindow == GetHandle() )  	pWnd = pParentWnd ;	else  	pWnd = new TWindow(hWindow) ;
	hittest = (int)(pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y))) ;
	MSG Msg ;
	if (hittest == HTCLIENT)	{  	pWnd->ScreenToClient( point ) ;    Msg.message = WM_RBUTTONDOWN ;    Msg.wParam = modKeys ;    Msg.lParam = MAKELONG(point.x,point.y) ;  }  else  {  	Msg.message = WM_NCRBUTTONDOWN ;    Msg.wParam = hittest ;    Msg.lParam = MAKELONG(point.x,point.y) ;	}
	Hide() ;	pWnd->PostMessage(Msg.message, Msg.wParam, Msg.lParam) ;

	if ( hWindow != GetHandle() )  	delete pWnd ;
	return ;}

void NSTitleTip::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point)
{
   TWindow *pWnd;

	int hittest;

    ClientToScreen( point );
    THandle hWindow = WindowFromPoint( point );
    if( hWindow == GetHandle() )        pWnd = pParentWnd;    else        pWnd = new TWindow(hWindow);
    hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));
    MSG Msg;
    if (hittest == HTCLIENT)    {        pWnd->ScreenToClient( point );        Msg.message = WM_LBUTTONUP;        Msg.wParam = modKeys;        Msg.lParam = MAKELONG(point.x,point.y);    }    else    {        Msg.message = WM_NCLBUTTONDOWN;        Msg.wParam = hittest;        Msg.lParam = MAKELONG(point.x,point.y);    }
    Hide();    pWnd->PostMessage(Msg.message, Msg.wParam, Msg.lParam);
    if( hWindow != GetHandle() )        delete pWnd;
    return;
}

void NSTitleTip::EvMButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
    TWindow *pWnd;
	int hittest;

    ClientToScreen( point );
    THandle hWindow = WindowFromPoint( point );

    if( hWindow == GetHandle() )        pWnd = pParentWnd;    else        pWnd = new TWindow(hWindow);
    hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));
    MSG Msg;
    if (hittest == HTCLIENT)    {        pWnd->ScreenToClient( point );        Msg.message = WM_MBUTTONDOWN;        Msg.wParam = modKeys;        Msg.lParam = MAKELONG(point.x,point.y);    }    else    {        Msg.message = WM_NCMBUTTONDOWN;        Msg.wParam = hittest;        Msg.lParam = MAKELONG(point.x,point.y);    }

    Hide();    pWnd->PostMessage(Msg.message, Msg.wParam, Msg.lParam);

    if( hWindow != GetHandle() )        delete pWnd;

    return;
}

void NSTitleTip::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point){
    TWindow *pWnd;	int hittest;
    ClientToScreen( point );    THandle hWindow = WindowFromPoint( point );
    if( hWindow == GetHandle() )        pWnd = pParentWnd;    else        pWnd = new TWindow(hWindow);
    hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));
    MSG Msg;
    if (hittest == HTCLIENT)    {        pWnd->ScreenToClient( point );        Msg.message = WM_LBUTTONDBLCLK;        Msg.wParam = modKeys;        Msg.lParam = MAKELONG(point.x,point.y);    }    else    {        Msg.message = WM_NCLBUTTONDBLCLK;        Msg.wParam = hittest;        Msg.lParam = MAKELONG(point.x,point.y);    }
    Hide();    pWnd->PostMessage(Msg.message, Msg.wParam, Msg.lParam);

    if( hWindow != GetHandle() )        delete pWnd;
    return;
}

voidNSTitleTip::EvRButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  ClientToScreen(point) ;
  THandle hWindow = WindowFromPoint(point) ;

  TWindow *pWnd ;
  if (hWindow == GetHandle())
    pWnd = pParentWnd ;
  else
    pWnd = new TWindow(hWindow) ;

  int hittest = (int)pWnd->SendMessage(WM_NCHITTEST, 0, MAKELONG(point.x, point.y)) ;

  MSG Msg ;

  if (HTCLIENT == hittest)
  {
    pWnd->ScreenToClient(point) ;
    Msg.message = WM_RBUTTONDBLCLK ;
    Msg.wParam  = modKeys ;
    Msg.lParam  = MAKELONG(WORD(point.x), WORD(point.y)) ;
  }
  else
  {
    Msg.message = WM_NCRBUTTONDBLCLK ;
    Msg.wParam  = hittest ;
    Msg.lParam  = MAKELONG(WORD(point.x), WORD(point.y)) ;
  }

  Hide() ;
  pWnd->PostMessage(Msg.message, Msg.wParam, Msg.lParam) ;

  if (GetHandle() != hWindow)
    delete pWnd ;
}

 /*
BOOL CTitleTip::PreTranslateMessage(MSG* pMsg){	CWnd *pWnd;	int hittest;	switch (pMsg->message)
	{	case WM_LBUTTONDOWN:	case WM_RBUTTONDOWN:	case WM_MBUTTONDOWN:		POINTS pts = MAKEPOINTS( pMsg->lParam );		POINT  point;		point.x = pts.x;		point.y = pts.y;		ClientToScreen( &point );		pWnd = WindowFromPoint( point );
		if( pWnd == this )			pWnd = m_pParentWnd;
		hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));		if (hittest == HTCLIENT)		{			pWnd->ScreenToClient( &point );			pMsg->lParam = MAKELONG(point.x,point.y);		}		else		{			switch (pMsg->message)			{				case WM_LBUTTONDOWN:					pMsg->message = WM_NCLBUTTONDOWN;					break;
				case WM_RBUTTONDOWN:					pMsg->message = WM_NCRBUTTONDOWN;					break;
				case WM_MBUTTONDOWN:					pMsg->message = WM_NCMBUTTONDOWN;					break;			}			pMsg->wParam = hittest;			pMsg->lParam = MAKELONG(point.x,point.y);		}
		Hide();		pWnd->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);		return TRUE;	case WM_KEYDOWN:	case WM_SYSKEYDOWN:		Hide();		m_pParentWnd->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );		return TRUE;	}	if( GetFocus() == NULL )	{		Hide();		return TRUE;	}	return CWnd::PreTranslateMessage(pMsg);}             */
void
NSTitleTip::Paint(TDC& dc, bool /* erase */, NS_CLASSLIB::TRect& /* RectAPeindre */){  dc.SaveDC() ;

  TEXTMETRIC tm ;	dc.GetTextMetrics(tm) ;
  if (pSkin && pSkin->getLogFont())  {    TFont font(pSkin->getLogFont()) ;    dc.SelectObject( font ) ;  }  else  {    HFONT hFont = pParentWnd->GetWindowFont() ;    TFont font(hFont) ; 	// use same font as ctrl    dc.SelectObject( font ) ;  }
	int nHeight = 0 ;	NS_CLASSLIB::TRect rect = rectDisplay ;
  NS_CLASSLIB::TPoint ptTL = rect.TopLeft() ;  ScreenToClient( ptTL ) ;  NS_CLASSLIB::TPoint ptBR = rect.BottomRight() ;  ScreenToClient( ptBR ) ;  rect = NS_CLASSLIB::TRect(ptTL, ptBR) ;
  //  // Fond : Bitmap ou Couleur  //
  if (pSkin && pSkin->getBackColor())    dc.SetBkColor(*(pSkin->getBackColor())) ;
  if (pSkin && pSkin->getBackBmp())  {    NS_CLASSLIB::TRect rectShow = NS_CLASSLIB::TRect(rectDisplay.left - MARGE_GCHE,
                                        rectDisplay.top - MARGE_HAUTE,
                                        rectDisplay.left + rectDisplay.Width() + MARGE_DRTE,                                        rectDisplay.top + rectDisplay.Height() + MARGE_BAS);
    NS_CLASSLIB::TPoint ptTLshow = rectShow.TopLeft() ;    ScreenToClient( ptTLshow ) ;    NS_CLASSLIB::TPoint ptBRshow = rectShow.BottomRight() ;    ScreenToClient( ptBRshow ) ;
    OWL::TDib* pBackgrnd = pSkin->getBackBmp() ;
    NS_CLASSLIB::TPoint dibOrigin ;
    bool bBottomUp ;
    //    // Bottom up dib : origin lies at the lower left corner    if (pBackgrnd->Height() > 0)    {      bBottomUp = true ;      dibOrigin = NS_CLASSLIB::TPoint(0, 0) ;    }    // top down dib : origin lies at the upper left corner    //    else    {      bBottomUp = false ;      dibOrigin = NS_CLASSLIB::TPoint(0, 0) ;    }
    NS_CLASSLIB::TRect winRect(0, 0, Attr.W, Attr.H) ;
    int x = ptTLshow.x ;    int y = ptTLshow.y ;    while (y < ptBRshow.y)    {
      while (x < ptBRshow.x)      {        winRect = NS_CLASSLIB::TRect(x, y, x+pBackgrnd->Width(), y+pBackgrnd->Height()) ;        dc.SetDIBitsToDevice(winRect, dibOrigin, *pBackgrnd) ;        x += pBackgrnd->Width() ;      }
      if (bBottomUp)        y += pBackgrnd->Height() ;      else        y -= pBackgrnd->Height() ;      x = x = ptTLshow.x ;    }    dc.SetBkMode( TRANSPARENT ) ;  }	nHeight = dc.DrawText(strTitle.c_str(), -1, rect, 0		| DT_LEFT		| DT_EDITCONTROL		| DT_NOPREFIX		| DT_WORDBREAK		) ;
	dc.RestoreDC() ;}

void NSTitleTip::EvPaint(){  TWindow::EvPaint() ;}
void NSTitleTip::disableTipTimer(){  if (pParentWnd)    pParentWnd->PostMessage(WM_COMMAND, CM_TIPTIMER_OFF) ;}

void NSTitleTip::enableTipTimer(){    if (pParentWnd)        pParentWnd->PostMessage(WM_COMMAND, CM_TIPTIMER_ON);
}
