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

#include <vcl.h>
#pragma hdrstop

#include "nsbb\webTips.h"

#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <windef.h>

#include "nssavoir\nsBdmDriver.h"
#include "nautilus\nssuper.h"
#include "nsdn\nsdochis.h"
#include "nautilus\nsldvvue.h"
#include "partage\ns_timer.h"
#include "nsbb\NSTipBrowser.h"

typedef enum BrowserNavConstants {
   navOpenInNewWindow   = 1,
   navNoHistory         = 2,
   navNoReadFromCache   = 4,
   navNoWriteToCache    = 8
} BrowserNavConstants ;

DEFINE_RESPONSE_TABLE1(NSWebTipWindow, TWindow)
  	EV_WM_LBUTTONDBLCLK,  //double click    EV_WM_RBUTTONDBLCLK,  //double click    EV_WM_MOUSEMOVE,    EV_WM_MBUTTONDOWN,    EV_WM_RBUTTONDOWN,    EV_WM_TIMER,
  	EV_WM_PAINT,    EV_WM_LBUTTONDOWN,
    EV_WM_LBUTTONUP,
END_RESPONSE_TABLE;

NSWebTipWindow::NSWebTipWindow(TWindow* pParent, NSContexte* pCtx)
               :TWindow(pParent), NSRoot(pCtx)
{
	// Enregistre la fenêtre parent
	pParentWnd = pParent ;

	// définit le style de la fenêtre (sans bordure)
	uint32 style = GetStyle() ;
	// SetStyle(style |= WS_BORDER | WS_POPUP) ;
  SetStyle(style ^= WS_BORDER | WS_POPUP) ;

	// Définition des styles étendu
	uint32 styleEx = GetExStyle() ;
	// SetExStyle(styleEx |= WS_EX_TOPMOST | WS_EX_TOOLWINDOW) ;

	// On cherche si on a des skin à appliquer au toolTip
	if (pContexte->getSkins())		pSkin = pContexte->getSkins()->donneSkin("ldvTips") ;	else		pSkin = 0 ;}

NSWebTipWindow::~NSWebTipWindow(){	// Détruit le Timer si il existe	killTipsTimer() ;

  // Delete de la Form
	delete Form ;
	// CoUninitialize();
	// OleUninitialize() ;
}
//
// Override a TWindow virtual to create the HWND directly.
// NSImportWindow and the VCL TForm1 class both end up
// wrapping the same HWND.
//
void
NSWebTipWindow::PerformCreate(int /*menuOrId*/)
{
  // OleInitialize(NULL) ;    // Use OleInitialize to get clipboard functionality
	Form = new TWebTip(NULL == Parent ? 0 : Parent->GetHandle(), this) ;
	Form->Visible = false ;
	Form->ParentWindow = ( NULL == Parent ? 0 : Parent->HWindow ) ;
	SetHandle(Form->Handle) ;
	::SetParent(Forms::Application->Handle, pContexte->GetMainWindow()->HWindow) ;

	// on navigue vers le fichier html à importer
  if (string("") != sURl)
		Navigate() ;
}

voidNSWebTipWindow::SetupWindow(){	TWindow::SetupWindow() ;	ShowWindow( SW_HIDE ) ;	// TODO FIXME Charger les skins}
// Creation de la fenêtreboolNSWebTipWindow::Create()
{
	return TWindow::Create() ;
}

// Show 		 - Show the titletip if needed// rectTitle	 - The rectangle within which the original//					title is constrained - in client coordinates// lpszTitleText - The text to be displayed// xoffset		 - Number of pixel that the text is offset from//				   left border of the cellvoid
NSWebTipWindow::EvTimer(uint timerId)
{
	if (ID_WEBTIPSTIMER != timerId)
		return ;

	if (timercount < time_to)
		timercount++ ;
  else
	{
		killTipsTimer() ;
    Hide() ;
	}
}

voidNSWebTipWindow::Show(NS_CLASSLIB::TRect rectTitle, string* psURL,                      NS_CLASSLIB::TRect* pHoverRect /*= NULL*/,
                      int time /* = -1 */)
{
	if ((string*) NULL != psURL)
	{
		sURl = *psURL ;
		Navigate() ;
	}

	// Declenche l'auto fermeture de la fenetre au bout d'un laps de temps
  if (time != -1)
	{
  	timercount = 0 ;
    time_to = time ;
    setTipsTimer() ;
	}

	if (rectTitle.IsEmpty())
		return ;

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

	//***************************************************************************************
	//Adjust the dimensions of the rect to fit within the client

	// Get the coordinates of the parents client area. (In this case the ListView's client
	// area) and convert coordinates to those of the tooltip.
	NS_CLASSLIB::TRect rectClient ;
  if (NULL == pParentWnd)
  	rectClient = rectTitle ;
	else
	{
		pParentWnd->GetClientRect( rectClient ) ;

		ptTL = rectClient.TopLeft() ;
  	pParentWnd->ClientToScreen( ptTL ) ;
  	ptBR = rectClient.BottomRight() ;
  	pParentWnd->ClientToScreen( ptBR ) ;
  	rectClient = NS_CLASSLIB::TRect(ptTL, ptBR) ;
  }

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
	// if (nMaxTooltipWidth > 0 && rectDisplay.Width() > nMaxTooltipWidth)
  //	rectDisplay.right = rectDisplay.left + nMaxTooltipWidth ;

	Form->Show() ;
	// MakeVisible() ;
	ShowWindow( SW_SHOW ) ;

	disableTipTimer() ;

	SetCapture() ;
}

voidNSWebTipWindow::Hide(){   /*  	if (!::IsWindow(GetSafeHwnd()))		return; */
	enableTipTimer() ;
	if (GetCapture() == *this)		ReleaseCapture() ;	ShowWindow( SW_HIDE ) ;}
void
NSWebTipWindow::Navigate()
{
try
{
	wchar_t buff[1024] ;
	Variant  Flags(navNoReadFromCache) ;
	TVariant VFlags = Flags.operator TVariant() ;

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sURl.c_str(), -1, buff, sizeof(buff)) ;
	Form->Control->Navigate(buff, &VFlags) ;
}
catch (Exception &ex)
{
  string ps = string("Exception NSWebTipWindow::Navigate (") + string(ex.Message.c_str()) + string(")") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
}
catch (...)
{
	erreur("Exception NSWebTipWindow::Navigate.", standardError, 0) ;
}
}

voidNSWebTipWindow::EvMouseMove(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	// On récupère le point en coordonnées locales, alors que rectHover est
  // en coordonnées locales du parent
  NS_CLASSLIB::TPoint pointParent = point ;
  ClientToScreen( pointParent ) ;
  if (NULL != pParentWnd)
		pParentWnd->ScreenToClient( pointParent ) ;

  if ((NULL == pParentWnd) || (false == rectHover.Contains(pointParent)))
	{
		// Forward the message
		ClientToScreen( point ) ;
    THandle hWindow = WindowFromPoint( point ) ;
		TWindow* pWnd ;
		if ((GetHandle() == hWindow) && (NULL != pParentWnd))
			pWnd = pParentWnd ;
    else
    	pWnd = new TWindow(hWindow) ;

	 	int hittest = (int)pWnd->SendMessage(WM_NCHITTEST, 0, (MAKELONG((WORD)(point.x),(WORD)(point.y)))) ;

		if (hittest == HTCLIENT)
    {
			pWnd->ScreenToClient( point ) ;
    	pWnd->PostMessage( WM_MOUSEMOVE, hittest, MAKELONG((WORD)(point.x), (WORD)(point.y)) ) ;
		}
    else
			pWnd->PostMessage( WM_NCMOUSEMOVE, hittest, MAKELONG((WORD)(point.x), (WORD)(point.y)) ) ;

    if ( hWindow != GetHandle() )
    	delete pWnd ;
	}
}

voidNSWebTipWindow::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point){	TWindow *pWnd ;
	int hittest ;
	ClientToScreen( point ) ;
	THandle hWindow = WindowFromPoint( point ) ;
	if ((GetHandle() == hWindow) && (NULL != pParentWnd))  	pWnd = pParentWnd ;  else  	pWnd = new TWindow(hWindow) ;
	hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y)) ;
	MSG Msg ;
	if (hittest == HTCLIENT)	{  	pWnd->ScreenToClient( point ) ;    Msg.message = WM_LBUTTONDOWN ;    Msg.wParam = modKeys ;    Msg.lParam = MAKELONG(point.x,point.y) ;  }  else  {  	Msg.message = WM_NCLBUTTONDOWN ;    Msg.wParam = hittest ;    Msg.lParam = MAKELONG(point.x,point.y) ;  }
	// Hide();	pWnd->PostMessage(Msg.message, Msg.wParam, Msg.lParam) ;
	if ( hWindow != GetHandle() )  	delete pWnd ;
	return ;}

voidNSWebTipWindow::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TWindow *pWnd ;
	int hittest ;

	ClientToScreen( point ) ;	THandle hWindow = WindowFromPoint( point ) ;

	if ((hWindow == GetHandle()) && (NULL != pParentWnd))  	pWnd = pParentWnd ;	else  	pWnd = new TWindow(hWindow) ;
	hittest = (int)(pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y))) ;
	MSG Msg ;
	if (hittest == HTCLIENT)	{  	pWnd->ScreenToClient( point ) ;    Msg.message = WM_RBUTTONDOWN ;    Msg.wParam = modKeys ;    Msg.lParam = MAKELONG(point.x,point.y) ;  }  else  {  	Msg.message = WM_NCRBUTTONDOWN ;    Msg.wParam = hittest ;    Msg.lParam = MAKELONG(point.x,point.y) ;	}
	Hide() ;	pWnd->PostMessage(Msg.message, Msg.wParam, Msg.lParam) ;

	if ( hWindow != GetHandle() )  	delete pWnd ;
	return ;}

void NSWebTipWindow::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point)
{
   TWindow *pWnd;

	int hittest;

    ClientToScreen( point );
    THandle hWindow = WindowFromPoint( point );
    if ((hWindow == GetHandle()) && (NULL != pParentWnd))        pWnd = pParentWnd;    else        pWnd = new TWindow(hWindow);
    hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));
    MSG Msg;
    if (hittest == HTCLIENT)    {        pWnd->ScreenToClient( point );        Msg.message = WM_LBUTTONUP;        Msg.wParam = modKeys;        Msg.lParam = MAKELONG(point.x,point.y);    }    else    {        Msg.message = WM_NCLBUTTONDOWN;        Msg.wParam = hittest;        Msg.lParam = MAKELONG(point.x,point.y);    }
    Hide();    pWnd->PostMessage(Msg.message, Msg.wParam, Msg.lParam);
    if( hWindow != GetHandle() )        delete pWnd;
    return;
}
void NSWebTipWindow::EvMButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TWindow *pWnd;
	int hittest;

	ClientToScreen( point );	THandle hWindow = WindowFromPoint( point );

	if ((hWindow == GetHandle()) && (NULL != pParentWnd))		pWnd = pParentWnd;	else		pWnd = new TWindow(hWindow);
	hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));
	MSG Msg;
	if (hittest == HTCLIENT)	{		pWnd->ScreenToClient( point );    Msg.message = WM_MBUTTONDOWN;    Msg.wParam = modKeys;    Msg.lParam = MAKELONG(point.x,point.y);	}	else	{  	Msg.message = WM_NCMBUTTONDOWN;    Msg.wParam = hittest;    Msg.lParam = MAKELONG(point.x,point.y);	}
	Hide();	pWnd->PostMessage(Msg.message, Msg.wParam, Msg.lParam);
	if( hWindow != GetHandle() )		delete pWnd ;
	return ;}

void NSWebTipWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point){
    TWindow *pWnd;	int hittest;
    ClientToScreen( point );    THandle hWindow = WindowFromPoint( point );
    if ((hWindow == GetHandle()) && (NULL != pParentWnd))        pWnd = pParentWnd;    else        pWnd = new TWindow(hWindow);
    hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));
    MSG Msg;
    if (hittest == HTCLIENT)    {        pWnd->ScreenToClient( point );        Msg.message = WM_LBUTTONDBLCLK;        Msg.wParam = modKeys;        Msg.lParam = MAKELONG(point.x,point.y);    }    else    {        Msg.message = WM_NCLBUTTONDBLCLK;        Msg.wParam = hittest;        Msg.lParam = MAKELONG(point.x,point.y);    }
    Hide();    pWnd->PostMessage(Msg.message, Msg.wParam, Msg.lParam);

    if( hWindow != GetHandle() )        delete pWnd;
    return;
}
voidNSWebTipWindow::EvRButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
    TWindow *pWnd;
	int hittest;

    ClientToScreen( point );
    THandle hWindow = WindowFromPoint( point );

    if ((hWindow == GetHandle()) && (NULL != pParentWnd))
        pWnd = pParentWnd;
    else
        pWnd = new TWindow(hWindow);

    hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));

    MSG Msg;

    if (hittest == HTCLIENT)
    {
        pWnd->ScreenToClient( point );
        Msg.message = WM_RBUTTONDBLCLK;
        Msg.wParam = modKeys;
        Msg.lParam = MAKELONG(WORD(point.x), WORD(point.y));
    }
    else
    {
        Msg.message = WM_NCRBUTTONDBLCLK;
        Msg.wParam = hittest;
        Msg.lParam = MAKELONG(WORD(point.x), WORD(point.y));
    }

    Hide();
    pWnd->PostMessage(Msg.message, Msg.wParam, Msg.lParam);

    if ( hWindow != GetHandle() )
        delete pWnd;

    return;
}

void
NSWebTipWindow::CmFermer()
{
	Hide() ;
}

voidNSWebTipWindow::disableTipTimer(){	if (pParentWnd)		pParentWnd->PostMessage(WM_COMMAND, CM_TIPTIMER_OFF) ;}
voidNSWebTipWindow::enableTipTimer(){	if (pParentWnd)		pParentWnd->PostMessage(WM_COMMAND, CM_TIPTIMER_ON) ;}

