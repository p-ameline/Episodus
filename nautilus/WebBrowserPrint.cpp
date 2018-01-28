//--------------------------------------------------------------------------
// (C) Copyright 1999 Microsoft Corporation.  All Rights Reserved.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS-IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// --- THIS CODE IS NOT SUPPORTED BY MICROSOFT ---
//
// This is unsupported code and Microsoft makes no guarantee that the
// techniques used in this sample will function properly in future
// versions of the Internet Explorer product.  This sample takes advantage
// of the fact that the resource identifiers in the "Page Setup" and
// "Print" dialogs are consistent between IE4 and IE5.  Therefore, it is
// possible that this technique will not work properly in future versions
// of Internet Explorer due to changes in the user interface or other
// modifications made to the product at a later date.  The product is not
// designed or tested to accommodate this technique, and the fact that it
// works is fortuitous but not by design.  Please use caution when adding
// this functionality to an application, and bear in mind this warning
// when testing your application with future versions of Internet Explorer.
//
// Author: Dave Templin
//         Support Engineer - Internet Client SDK
//         Microsoft Developer Support
//--------------------------------------------------------------------------
// WebBrowserPrint.cpp: implementation of the CWebBrowserPrint class.
//
//////////////////////////////////////////////////////////////////////

// #include <vcl.h>
// #pragma hdrstop
// #include <dstring.h>
// #include "stdafx.h"
#include "WebBrowserPrint.h"
#include <winuser.h>

const size_t BUFSIZE = 255;
static TCHAR g_szBuf[BUFSIZE];
static HHOOK g_hHook = NULL;
static HWND g_hWnd = NULL;
static bool g_bSuccess = false;
static CWebBrowserPrint* g_pWebBrowserPrint = NULL;
static void (CWebBrowserPrint::*g_pDoDlgExchangeFunc)(HWND hwnd) = NULL;

ULONG GetComboList(HWND hwnd, LPTSTR*& ppTextItems)
{
	if (!IsWindow(hwnd))
	{
		g_bSuccess = false;
		return 0;
	}

	//On Win9x, the list of printers is filled completely only when the combo box is dropped down
	//So send F4 keypresses to drop down and retract the combo box
	SendMessage(hwnd, WM_KEYDOWN, 115, 1);
	SendMessage(hwnd, WM_KEYUP, 115, 1);
	SendMessage(hwnd, WM_KEYDOWN, 115, 1);
	SendMessage(hwnd, WM_KEYUP, 115, 1);

	ULONG count = SendMessage(hwnd, CB_GETCOUNT, 0, 0);
	ppTextItems = new LPTSTR[count];  // allocate storage for count TSTR's

	for (ULONG i = 0; i < count; ++i)
	{
		ULONG textlen = SendMessage(hwnd, CB_GETLBTEXTLEN, i, 0) + 1;
		ppTextItems[i] = new TCHAR[textlen];
		SendMessage(hwnd, CB_GETLBTEXT, i, (WPARAM)ppTextItems[i]);
	}

	return count;
}

void DeleteStrings(ULONG& lCount, LPTSTR*& m_ppTextItems)
{
	if (lCount == 0) return;
	for (ULONG i = 0; i < lCount; ++i)
		delete [] m_ppTextItems[i];
	delete [] m_ppTextItems;
	lCount = 0;
}
 
CString GetEditText(HWND hwnd)
{
	CString sText;
	size_t textlen = GetWindowTextLength(hwnd) + 1;
	LPTSTR szText = new TCHAR[textlen];
	GetWindowText(hwnd, szText, textlen);
	sText = szText;
	delete [] szText;
	return sText;
}

CString GetRegValue(LPCTSTR szValueName)
{
	CString sValue;
	HKEY hKey;
	ULONG r;
	DWORD dwType;
	DWORD dwData = BUFSIZE;
	g_szBuf[0] = TCHAR('\0');
	r = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\PageSetup"), 0, KEY_READ, &hKey);
	r = RegQueryValueEx(hKey, szValueName, 0, &dwType, (LPBYTE)g_szBuf, &dwData);
	r = RegCloseKey(hKey);
	sValue = g_szBuf;
	return sValue;
}
void RestoreRegValue(LPCTSTR szValueName, LPCTSTR szValue)
{
	HKEY hKey;
	ULONG r;
	DWORD dwData = _tcslen(szValue) + 1;
	r = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\PageSetup"), 0, KEY_WRITE, &hKey);
	r = RegSetValueEx(hKey, szValueName, 0, REG_SZ, (LPBYTE)szValue, dwData);
	r = RegCloseKey(hKey);
}

CWebBrowserPrint::CWebBrowserPrint()
: m_pWebBrowser(NULL), m_lPrinterCount(0)
{
}

CWebBrowserPrint::~CWebBrowserPrint()
{
	if (m_pWebBrowser)
		m_pWebBrowser->Release() ;
	m_pWebBrowser = NULL ;

	DeleteStrings(m_lPrinterCount, m_ppPrinterNames);
}

void CWebBrowserPrint::SetWebBrowser(Shdocvw_tlb::IWebBrowser2* pWebBrowser)
{
	if (m_pWebBrowser)
	{
		m_pWebBrowser->Release();
		m_pWebBrowser = NULL;
	}

	if (pWebBrowser)
	{
		m_pWebBrowser = pWebBrowser;
		m_pWebBrowser->AddRef();
		m_bPrinting = false;
	}
}

void CWebBrowserPrint::ExchangeEditText(HWND hwnd, CString& sText)
{
	if (!IsWindow(hwnd))
	{
		g_bSuccess = false;
		return;
	}

	if (m_bPrinting)
		SetWindowText(hwnd, sText);
	else
	{
		size_t textlen = GetWindowTextLength(hwnd) + 1;
		LPTSTR szText = new TCHAR[textlen];
		GetWindowText(hwnd, szText, textlen);
		sText = szText;
		delete [] szText;
	}
}

void CWebBrowserPrint::ExchangeButtonState(HWND hwnd, bool& bState)
{
	if (!IsWindow(hwnd))
	{
		g_bSuccess = false;
		return;
	}

	if (m_bPrinting)
	{
		bool bCurrentState = SendMessage(hwnd, BM_GETCHECK, 0, 0) ? true : false;
		if (bState != bCurrentState)
			SendMessage(hwnd, BM_CLICK, 0, 0);
	}
	else
		bState = SendMessage(hwnd, BM_GETCHECK, 0, 0) != FALSE ? true : false;
}

void CWebBrowserPrint::ExchangeEditNum(HWND hwnd, ULONG& iValue)
{
	if (!IsWindow(hwnd))
	{
		g_bSuccess = false;
		return;
	}

	if (m_bPrinting)
	{
		wsprintf(g_szBuf, _T("%d"), iValue);
		SetWindowText(hwnd, g_szBuf);
	}
	else
	{
		CString sText = GetEditText(hwnd);
		LPTSTR szStop = NULL;
		iValue = _tcstol(sText, &szStop, 10);
	}
}

void CWebBrowserPrint::ExchangeEditNum(HWND hwnd, float& fValue)
{
	if (!IsWindow(hwnd))
	{
		g_bSuccess = false;
		return;
	}

	if (m_bPrinting)
	{
		ULONG wp = (ULONG)fValue;
		ULONG fp = (ULONG)(1000 * (fValue - wp));
		wsprintf(g_szBuf, _T("%d.%03d"), wp, fp); 
		SetWindowText(hwnd, g_szBuf);
	}
	else
	{
		CString sText = GetEditText(hwnd);
		LPTSTR szStop = NULL;
		fValue = (float)_tcstod(sText, &szStop);
	}
}

LRESULT CALLBACK CWebBrowserPrint::CbtProc(int nCode, WPARAM wParam, LPARAM lParam) 
{  
	switch (nCode)
	{
		case HCBT_CREATEWND:
		{
			HWND hWnd = (HWND)wParam;
			LPCBT_CREATEWND pcbt = (LPCBT_CREATEWND)lParam;
			LPCREATESTRUCT pcs = pcbt->lpcs;
			if ((DWORD)pcs->lpszClass == 0x00008002)
			{
				g_hWnd = hWnd;          // Get hwnd of Page Setup dialog
				pcs->x = -2 * pcs->cx;  // Move dialog off screen
			}
			break;
		}	
		case HCBT_ACTIVATE:
		{
			HWND hwnd = (HWND)wParam;
			if (hwnd == g_hWnd)
			{
				g_hWnd = NULL;
				(g_pWebBrowserPrint->*g_pDoDlgExchangeFunc)(hwnd);  // Call thru func ptr
				g_bSuccess = true;
			}
			break;
		}
	}
    return CallNextHookEx(g_hHook, nCode, wParam, lParam); 
}

void CWebBrowserPrint::DoPageSetupDlgExchange(HWND hwnd)
{
	bool bPortrait = (m_Orientation == OrientationPortrait);
	bool bLandscape = (m_Orientation == OrientationLandscape);

	ExchangeEditText(GetDlgItem(hwnd, 0x0471), m_sPaperSize);
	ExchangeEditText(GetDlgItem(hwnd, 0x0472), m_sPaperSource);
	ExchangeEditText(GetDlgItem(hwnd, 0x1fd3), m_sHeader);
	ExchangeEditText(GetDlgItem(hwnd, 0x1fd5), m_sFooter);
	ExchangeButtonState(GetDlgItem(hwnd, 0x0420), bPortrait);
	ExchangeButtonState(GetDlgItem(hwnd, 0x0421), bLandscape);
	ExchangeEditNum(GetDlgItem(hwnd, 0x0483), m_fLeftMargin);
	ExchangeEditNum(GetDlgItem(hwnd, 0x0485), m_fRightMargin);
	ExchangeEditNum(GetDlgItem(hwnd, 0x0484), m_fTopMargin);
	ExchangeEditNum(GetDlgItem(hwnd, 0x0486), m_fBottomMargin);

	if (m_bPrinting)
		SendMessage(GetDlgItem(hwnd, 0x0001), BM_CLICK, 0, 0);  // Invoke OK button
	else
		SendMessage(GetDlgItem(hwnd, 0x0002), BM_CLICK, 0, 0);  // Invoke Cancel button

	if (bPortrait)
		m_Orientation = OrientationPortrait;
	else if (bLandscape)
		m_Orientation = OrientationLandscape;
	else
		m_Orientation = OrientationUndefined;
}

void CWebBrowserPrint::DoPrintDlgExchange(HWND hwnd)
{
	bool bPrintRangeAll = (m_PrintRange == PrintRangeAll);
	bool bPrintRangePages = (m_PrintRange == PrintRangePages);
	bool bPrintRangeSelection = (m_PrintRange == PrintRangeSelection);

	bool bPrintFramesScreen = (m_PrintFrames == PrintFramesScreen);
	bool bPrintFramesSelected = (m_PrintFrames == PrintFramesSelected);
	bool bPrintFramesIndividually = (m_PrintFrames == PrintFramesIndividually);

	ExchangeEditText(GetDlgItem(hwnd, 0x0473), m_sPrinterName);
	ExchangeButtonState(GetDlgItem(hwnd, 0x0410), m_bPrintToFile);
	ExchangeButtonState(GetDlgItem(hwnd, 0x0420), bPrintRangeAll);
	ExchangeButtonState(GetDlgItem(hwnd, 0x0422), bPrintRangePages);
	ExchangeButtonState(GetDlgItem(hwnd, 0x0421), bPrintRangeSelection);
	ExchangeEditNum(GetDlgItem(hwnd, 0x0480), m_lPrintRangePagesFrom);
	ExchangeEditNum(GetDlgItem(hwnd, 0x0481), m_lPrintRangePagesTo);
	ExchangeEditNum(GetDlgItem(hwnd, 0x0482), m_lCopies);
	ExchangeButtonState(GetDlgItem(hwnd, 0x0411), m_bCollate);
	ExchangeButtonState(GetDlgItem(hwnd, 0x0423), bPrintFramesScreen);
	ExchangeButtonState(GetDlgItem(hwnd, 0x0424), bPrintFramesSelected);
	ExchangeButtonState(GetDlgItem(hwnd, 0x0425), bPrintFramesIndividually);
	ExchangeButtonState(GetDlgItem(hwnd, 0x1fcc), m_bPrintLinks);
	ExchangeButtonState(GetDlgItem(hwnd, 0x1fce), m_bPrintLinkTable);

	DeleteStrings(m_lPrinterCount, m_ppPrinterNames);
	m_lPrinterCount = GetComboList(GetDlgItem(hwnd, 0x0473), m_ppPrinterNames);

	if (m_bPrinting)
		SendMessage(GetDlgItem(hwnd, 0x0001), BM_CLICK, 0, 0);  // Invoke OK button
	else
		SendMessage(GetDlgItem(hwnd, 0x0002), BM_CLICK, 0, 0);  // Invoke Cancel button

	if (bPrintRangeAll)
		m_PrintRange = PrintRangeAll;
	else if (bPrintRangePages)
		m_PrintRange = PrintRangePages;
	else if (bPrintRangeSelection)
		m_PrintRange = PrintRangeSelection;
	else
		m_PrintRange = PrintRangeUndefined;

	if (bPrintFramesScreen)
		m_PrintFrames = PrintFramesScreen;
	else if (bPrintFramesSelected)
		m_PrintFrames = PrintFramesSelected;
	else if (bPrintFramesIndividually)
		m_PrintFrames = PrintFramesIndividually;
	else
		m_PrintFrames = PrintFramesUndefined;
}

bool
CWebBrowserPrint::InvokeDlg(Shdocvw_tlb::OLECMDID cmdid, void (CWebBrowserPrint::*pDoDlgExchangeFunc)(HWND hwnd))
{
	if (!m_pWebBrowser)
		return false ;

	if (g_hHook)
		return false ;

	g_hHook = SetWindowsHookEx(WH_CBT, CbtProc, NULL, GetCurrentThreadId()) ;
	if (!g_hHook)
		return false ;

	g_bSuccess = false ;
	g_pWebBrowserPrint = this ;
	g_pDoDlgExchangeFunc = pDoDlgExchangeFunc ;

	// ancien code
	// HRESULT hr = m_pWebBrowser->ExecWB(cmdid, Shdocvw_tlb::OLECMDEXECOPT_DODEFAULT, NULL, NULL);
	// ------------------------------------------------------------------------------------------
	// modif RS du 08/04/04 :

  Shdocvw_tlb::OLECMDEXECOPT cmdexecopt = Shdocvw_tlb::OLECMDEXECOPT_DONTPROMPTUSER ;

	HRESULT hr ;
	if (cmdid == Shdocvw_tlb::OLECMDID_PRINT)
	{
  	if (m_bPromptUser)
  		cmdexecopt = Shdocvw_tlb::OLECMDEXECOPT_DODEFAULT ;

  	for (size_t i = 0; i < m_lCopies; i++)
    	hr = m_pWebBrowser->ExecWB(cmdid, cmdexecopt, NULL, NULL) ;
  }
  else
  	hr = m_pWebBrowser->ExecWB(cmdid, cmdexecopt, NULL, NULL) ;

	if (hr == S_OK)
  	g_bSuccess = true ;

	UnhookWindowsHookEx(g_hHook) ;
	g_pWebBrowserPrint = NULL ;
	g_hHook = NULL ;
	return g_bSuccess ;
}

bool CWebBrowserPrint::ReadDlgSettings()
{

    void (CWebBrowserPrint::*pDoPageSetupDlgExchange)(HWND hwnd) ;
    pDoPageSetupDlgExchange = &CWebBrowserPrint::DoPageSetupDlgExchange ;

    void (CWebBrowserPrint::*pDoPrintDlgExchange)(HWND hwnd) ;
    pDoPrintDlgExchange = &CWebBrowserPrint::DoPrintDlgExchange ;

	DeleteStrings(m_lPrinterCount, m_ppPrinterNames);

    // Code original
    //
	// if ( !InvokeDlg(OLECMDID_PAGESETUP, DoPageSetupDlgExchange) )
	//  	return false;
	// if ( !InvokeDlg(OLECMDID_PRINT, DoPrintDlgExchange) )
	//  	return false;

    if ( !InvokeDlg(Shdocvw_tlb::OLECMDID_PAGESETUP, pDoPageSetupDlgExchange) )
	  	return false;
    if ( !InvokeDlg(Shdocvw_tlb::OLECMDID_PRINT, pDoPrintDlgExchange) )
	  	return false;

	return true;
}

bool CWebBrowserPrint::Print(int iNbExpl, bool bPromptUser)
{
try
{
	CString sHeader = GetRegValue(_T("header")) ;
	CString sFooter = GetRegValue(_T("footer")) ;
	CString sLeftMargin = GetRegValue(_T("margin_left")) ;
	CString sRightMargin = GetRegValue(_T("margin_right")) ;
	CString sTopMargin = GetRegValue(_T("margin_top")) ;
	CString sBottomMargin = GetRegValue(_T("margin_bottom")) ;

	m_bPromptUser = bPromptUser ;
	m_lCopies = (ULONG) iNbExpl ;

	bool bSuccess = false ;
	m_bPrinting   = true ;

	// Code initial
	//
	// if ( InvokeDlg(OLECMDID_PAGESETUP, DoPageSetupDlgExchange) )
	//  if ( InvokeDlg(OLECMDID_PRINT, DoPrintDlgExchange) )

	void (CWebBrowserPrint::*pDoPageSetupDlgExchange)(HWND hwnd) ;
	pDoPageSetupDlgExchange = &CWebBrowserPrint::DoPageSetupDlgExchange ;

	void (CWebBrowserPrint::*pDoPrintDlgExchange)(HWND hwnd) ;
	pDoPrintDlgExchange = &CWebBrowserPrint::DoPrintDlgExchange ;

	if ( InvokeDlg(Shdocvw_tlb::OLECMDID_PAGESETUP, pDoPageSetupDlgExchange) )
		if ( InvokeDlg(Shdocvw_tlb::OLECMDID_PRINT, pDoPrintDlgExchange) )
			bSuccess = true ;

	m_bPrinting = false ;

	RestoreRegValue(_T("header"), sHeader) ;
	RestoreRegValue(_T("footer"), sFooter) ;
	RestoreRegValue(_T("margin_left"), sLeftMargin) ;
	RestoreRegValue(_T("margin_right"), sRightMargin) ;
	RestoreRegValue(_T("margin_top"), sTopMargin) ;
	RestoreRegValue(_T("margin_bottom"), sBottomMargin) ;

	return bSuccess ;
}
catch (...)
{
  return false ;
}
}

CString CWebBrowserPrint::GetPrinterName(ULONG lIndex)
{
	CString sPrinterName;
	if (lIndex < m_lPrinterCount)
		sPrinterName = m_ppPrinterNames[lIndex];
	return sPrinterName;
}

CString CWebBrowserPrint::GetDefaultPrinterName()
{
	CString sPrinterName;

	GetProfileString(_T("windows"), _T("device"), _T(",,,"), g_szBuf, BUFSIZE);
	LPTSTR pch = _tcsstr(g_szBuf, _T(","));
	if (pch)
	{
		*pch = _T('\0');
		sPrinterName = g_szBuf;
	}

	return sPrinterName;
}