// WebBrowserPrint.h: interface for the CWebBrowserPrint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBBROWSERPRINT_H__DA19EEF7_7132_11D3_BB8B_00C04FA3471C__INCLUDED_)
#define AFX_WEBBROWSERPRINT_H__DA19EEF7_7132_11D3_BB8B_00C04FA3471C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "stdafx.h"
#include <afx.h>
#include <exdisp.h>
#include "SHDocVw_OCX.h"

class CWebBrowserPrint
{
public:
	enum Orientation { OrientationUndefined, OrientationPortrait, OrientationLandscape };
	enum PrintRange { PrintRangeUndefined, PrintRangeAll, PrintRangePages, PrintRangeSelection };
	enum PrintFrames { PrintFramesUndefined, PrintFramesScreen, PrintFramesSelected, PrintFramesIndividually };

	CWebBrowserPrint();
	virtual ~CWebBrowserPrint();

	void SetWebBrowser(Shdocvw_tlb::IWebBrowser2* pWebBrowser);
	bool Print(int iNbExpl = 1, bool bPromptUser = false);
	bool ReadDlgSettings();
	CString GetPrinterName(ULONG lIndex);
	ULONG GetPrinterCount() { return m_lPrinterCount; }
	CString GetDefaultPrinterName();
    
	// Page Setup dialog settings
	CString m_sPaperSize;
	CString m_sPaperSource;
	CString m_sHeader;
	CString m_sFooter;
	Orientation m_Orientation;
	float m_fLeftMargin;
	float m_fTopMargin;
	float m_fRightMargin;
	float m_fBottomMargin;

	// Print dialog settings
	CString m_sPrinterName;
	bool m_bPrintToFile;
	PrintRange m_PrintRange;
	ULONG m_lPrintRangePagesFrom;
	ULONG m_lPrintRangePagesTo;
	ULONG m_lCopies;
	bool m_bCollate;
	PrintFrames m_PrintFrames;
	bool m_bPrintLinks;
	bool m_bPrintLinkTable;

private:
	void ExchangeEditText(HWND hwnd, CString& sText);
	void ExchangeButtonState(HWND hwnd, bool& bState);
	void ExchangeEditNum(HWND hwnd, float& fValue);
	void ExchangeEditNum(HWND hwnd, ULONG& lValue);
	void DoPageSetupDlgExchange(HWND hwnd);
	void DoPrintDlgExchange(HWND hwnd);
	bool InvokeDlg(Shdocvw_tlb::OLECMDID cmdid, void (CWebBrowserPrint::*pDoDlgExchangeFunc)(HWND hwnd));
	static LRESULT CALLBACK CbtProc(int nCode, WPARAM wParam, LPARAM lParam);

	Shdocvw_tlb::IWebBrowser2* m_pWebBrowser;
	ULONG m_lPrinterCount;
	LPTSTR* m_ppPrinterNames;
	bool m_bPrinting;
    bool m_bPromptUser;
};

#endif // !defined(AFX_WEBBROWSERPRINT_H__DA19EEF7_7132_11D3_BB8B_00C04FA3471C__INCLUDED_)
