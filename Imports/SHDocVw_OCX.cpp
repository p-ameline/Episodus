// ************************************************************************ //
// WARNING                                                                    
// -------                                                                    
// The types declared in this file were generated from data read from a       
// Type Library. If this type library is explicitly or indirectly (via        
// another type library referring to this type library) re-imported, or the   
// 'Refresh' command of the Type Library Editor activated while editing the   
// Type Library, the contents of this file will be regenerated and all        
// manual modifications will be lost.                                         
// ************************************************************************ //

// C++ TLBWRTR : $Revision:   1.96.1.40  $
// File generated on 26/09/2008 22:53:58 from Type Library described below.

// ************************************************************************ //
// Type Lib: C:\WINDOWS\system32\ieframe.dll
// IID\LCID: {EAB22AC0-30C1-11CF-A7EB-0000C05BAE0B}\0
// Helpfile: 
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\system32\stdole2.tlb)
//   (2) v4.0 StdVCL, (C:\WINDOWS\system32\STDVCL40.DLL)
// Errors:
//   Hint: IWebBrowser.Application changed to Application_
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#include "SHDocVw_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Shdocvw_tlb
{



// *********************************************************************//
// OCX PROXY CLASS IMPLEMENTATION
// (The following variables/methods implement the class TWebBrowser_V1 which
// allows "WebBrowser Control" to be hosted in CBuilder IDE/apps).
// *********************************************************************//
int   TWebBrowser_V1::EventDispIDs[17] = {
    0x00000064, 0x00000065, 0x00000066, 0x0000006C, 0x00000068, 0x00000069,
    0x0000006A, 0x0000006B, 0x00000071, 0x000000C8, 0x000000C9, 0x000000CC,
    0x00000067, 0x0000006D, 0x0000006E, 0x0000006F, 0x00000070};

TControlData TWebBrowser_V1::CControlData =
{
  // GUID of CoClass and Event Interface of Control
  {0xEAB22AC3, 0x30C1, 0x11CF,{ 0xA7, 0xEB, 0x00, 0x00, 0xC0, 0x5B, 0xAE, 0x0B} }, // CoClass
  {0xEAB22AC2, 0x30C1, 0x11CF,{ 0xA7, 0xEB, 0x00, 0x00, 0xC0, 0x5B, 0xAE, 0x0B} }, // Events

  // Count of Events and array of their DISPIDs
  17, &EventDispIDs,

  // Pointer to Runtime License string
  NULL,  // HRESULT(0x80040111)

  // Flags for OnChanged PropertyNotification
  0x00000000,
  300,// (IDE Version)

  // Count of Font Prop and array of their DISPIDs
  0, NULL,

  // Count of Pict Prop and array of their DISPIDs
  0, NULL,
  0, // Reserved
  0, // Instance count (used internally)
  0, // List of Enum descriptions (internal)
};

GUID     TWebBrowser_V1::DEF_CTL_INTF = {0xEAB22AC1, 0x30C1, 0x11CF,{ 0xA7, 0xEB, 0x00, 0x00, 0xC0, 0x5B, 0xAE, 0x0B} };
TNoParam TWebBrowser_V1::OptParam;

static inline void ValidCtrCheck(TWebBrowser_V1 *)
{
   delete new TWebBrowser_V1((TComponent*)(0));
};

void __fastcall TWebBrowser_V1::InitControlData(void)
{
  ControlData = &CControlData;
};

void __fastcall TWebBrowser_V1::CreateControl(void)
{
  if (!m_OCXIntf)
  {
    _ASSERTE(DefaultDispatch);
    DefaultDispatch->QueryInterface(DEF_CTL_INTF, (LPVOID*)&m_OCXIntf);
  }
};

TCOMIWebBrowser __fastcall TWebBrowser_V1::GetControlInterface(void)
{
  CreateControl();
  return m_OCXIntf;
};

void/*HR*/ __fastcall TWebBrowser_V1::GoBack(void)
{
  GetControlInterface().GoBack();
}

void/*HR*/ __fastcall TWebBrowser_V1::GoForward(void)
{
  GetControlInterface().GoForward();
}

void/*HR*/ __fastcall TWebBrowser_V1::GoHome(void)
{
  GetControlInterface().GoHome();
}

void/*HR*/ __fastcall TWebBrowser_V1::GoSearch(void)
{
  GetControlInterface().GoSearch();
}

void/*HR*/ __fastcall TWebBrowser_V1::Navigate(BSTR URL/*[in]*/, TVariant* Flags/*[in,opt]*/, 
                                               TVariant* TargetFrameName/*[in,opt]*/, 
                                               TVariant* PostData/*[in,opt]*/, 
                                               TVariant* Headers/*[in,opt]*/)
{
  GetControlInterface().Navigate(URL/*[in]*/, Flags/*[in,opt]*/, TargetFrameName/*[in,opt]*/, 
                                 PostData/*[in,opt]*/, Headers/*[in,opt]*/);
}

void/*HR*/ __fastcall TWebBrowser_V1::Refresh(void)
{
  GetControlInterface().Refresh();
}

void/*HR*/ __fastcall TWebBrowser_V1::Refresh2(TVariant* Level/*[in,opt]*/)
{
  GetControlInterface().Refresh2(Level/*[in,opt]*/);
}

void/*HR*/ __fastcall TWebBrowser_V1::Stop(void)
{
  GetControlInterface().Stop();
}

LPDISPATCH __fastcall TWebBrowser_V1::Get_Application_(void)
{
  return GetControlInterface().get_Application_();
}

LPDISPATCH __fastcall TWebBrowser_V1::Get_Parent(void)
{
  return GetControlInterface().get_Parent();
}

LPDISPATCH __fastcall TWebBrowser_V1::Get_Container(void)
{
  return GetControlInterface().get_Container();
}

LPDISPATCH __fastcall TWebBrowser_V1::Get_Document(void)
{
  return GetControlInterface().get_Document();
}



// *********************************************************************//
// OCX PROXY CLASS IMPLEMENTATION
// (The following variables/methods implement the class TWebBrowser which
// allows "WebBrowser Control" to be hosted in CBuilder IDE/apps).
// *********************************************************************//
int   TWebBrowser::EventDispIDs[35] = {
    0x00000066, 0x0000006C, 0x00000069, 0x0000006A, 0x00000068, 0x00000071,
    0x00000070, 0x000000FA, 0x000000FB, 0x000000FC, 0x00000103, 0x000000FD,
    0x000000FE, 0x000000FF, 0x00000100, 0x00000101, 0x00000102, 0x00000104,
    0x00000106, 0x00000108, 0x00000109, 0x0000010A, 0x0000010B, 0x00000107,
    0x0000010C, 0x0000010D, 0x0000010E, 0x0000010F, 0x000000E1, 0x000000E2,
    0x000000E3, 0x00000110, 0x00000111, 0x0000011A, 0x0000011B};

TControlData TWebBrowser::CControlData =
{
  // GUID of CoClass and Event Interface of Control
  {0x8856F961, 0x340A, 0x11D0,{ 0xA9, 0x6B, 0x00, 0xC0, 0x4F, 0xD7, 0x05, 0xA2} }, // CoClass
  {0x34A715A0, 0x6587, 0x11D0,{ 0x92, 0x4A, 0x00, 0x20, 0xAF, 0xC7, 0xAC, 0x4D} }, // Events

  // Count of Events and array of their DISPIDs
  35, &EventDispIDs,

  // Pointer to Runtime License string
  NULL,  // HRESULT(0x80040111)

  // Flags for OnChanged PropertyNotification
  0x00000000,
  300,// (IDE Version)

  // Count of Font Prop and array of their DISPIDs
  0, NULL,

  // Count of Pict Prop and array of their DISPIDs
  0, NULL,
  0, // Reserved
  0, // Instance count (used internally)
  0, // List of Enum descriptions (internal)
};

GUID     TWebBrowser::DEF_CTL_INTF = {0xD30C1661, 0xCDAF, 0x11D0,{ 0x8A, 0x3E, 0x00, 0xC0, 0x4F, 0xC9, 0xE2, 0x6E} };
TNoParam TWebBrowser::OptParam;

static inline void ValidCtrCheck(TWebBrowser *)
{
   delete new TWebBrowser((TComponent*)(0));
};

void __fastcall TWebBrowser::InitControlData(void)
{
  ControlData = &CControlData;
};

void __fastcall TWebBrowser::CreateControl(void)
{
  if (!m_OCXIntf)
  {
    _ASSERTE(DefaultDispatch);
    DefaultDispatch->QueryInterface(DEF_CTL_INTF, (LPVOID*)&m_OCXIntf);
  }
};

IWebBrowser2Disp __fastcall TWebBrowser::GetControlInterface(void)
{
  CreateControl();
  return m_OCXIntf;
};

void/*HR*/ __fastcall TWebBrowser::GoBack(void)
{
  GetControlInterface().GoBack();
}

void/*HR*/ __fastcall TWebBrowser::GoForward(void)
{
  GetControlInterface().GoForward();
}

void/*HR*/ __fastcall TWebBrowser::GoHome(void)
{
  GetControlInterface().GoHome();
}

void/*HR*/ __fastcall TWebBrowser::GoSearch(void)
{
  GetControlInterface().GoSearch();
}

void/*HR*/ __fastcall TWebBrowser::Navigate(BSTR URL/*[in]*/, TVariant* Flags/*[in,opt]*/, 
                                            TVariant* TargetFrameName/*[in,opt]*/, 
                                            TVariant* PostData/*[in,opt]*/, 
                                            TVariant* Headers/*[in,opt]*/)
{
  GetControlInterface().Navigate(URL/*[in]*/, Flags/*[in,opt]*/, TargetFrameName/*[in,opt]*/, 
                                 PostData/*[in,opt]*/, Headers/*[in,opt]*/);
}

void/*HR*/ __fastcall TWebBrowser::Refresh(void)
{
  GetControlInterface().Refresh();
}

void/*HR*/ __fastcall TWebBrowser::Refresh2(TVariant* Level/*[in,opt]*/)
{
  GetControlInterface().Refresh2(Level/*[in,opt]*/);
}

void/*HR*/ __fastcall TWebBrowser::Stop(void)
{
  GetControlInterface().Stop();
}

void/*HR*/ __fastcall TWebBrowser::Quit(void)
{
  GetControlInterface().Quit();
}

void/*HR*/ __fastcall TWebBrowser::ClientToWindow(int* pcx/*[in,out]*/, int* pcy/*[in,out]*/)
{
  GetControlInterface().ClientToWindow(pcx/*[in,out]*/, pcy/*[in,out]*/);
}

void/*HR*/ __fastcall TWebBrowser::PutProperty(BSTR Property/*[in]*/, TVariant vtValue/*[in]*/)
{
  GetControlInterface().PutProperty(Property/*[in]*/, vtValue/*[in]*/);
}

TVariant __fastcall TWebBrowser::GetProperty(BSTR Property/*[in]*/)
{
  return GetControlInterface().GetProperty(Property/*[in]*/);
}

void/*HR*/ __fastcall TWebBrowser::Navigate2(TVariant* URL/*[in]*/, TVariant* Flags/*[in,opt]*/, 
                                             TVariant* TargetFrameName/*[in,opt]*/, 
                                             TVariant* PostData/*[in,opt]*/, 
                                             TVariant* Headers/*[in,opt]*/)
{
  GetControlInterface().Navigate2(URL/*[in]*/, Flags/*[in,opt]*/, TargetFrameName/*[in,opt]*/, 
                                  PostData/*[in,opt]*/, Headers/*[in,opt]*/);
}

Shdocvw_tlb::OLECMDF __fastcall TWebBrowser::QueryStatusWB(Shdocvw_tlb::OLECMDID cmdID/*[in]*/)
{
  return GetControlInterface().QueryStatusWB(cmdID/*[in]*/);
}

void/*HR*/ __fastcall TWebBrowser::ExecWB(Shdocvw_tlb::OLECMDID cmdID/*[in]*/, 
                                          Shdocvw_tlb::OLECMDEXECOPT cmdexecopt/*[in]*/, 
                                          TVariant* pvaIn/*[in,opt]*/, 
                                          TVariant* pvaOut/*[in,out,opt]*/)
{
  GetControlInterface().ExecWB(cmdID/*[in]*/, cmdexecopt/*[in]*/, pvaIn/*[in,opt]*/, 
                               pvaOut/*[in,out,opt]*/);
}

void/*HR*/ __fastcall TWebBrowser::ShowBrowserBar(TVariant* pvaClsid/*[in]*/, 
                                                  TVariant* pvarShow/*[in,opt]*/, 
                                                  TVariant* pvarSize/*[in,opt]*/)
{
  GetControlInterface().ShowBrowserBar(pvaClsid/*[in]*/, pvarShow/*[in,opt]*/, pvarSize/*[in,opt]*/);
}

LPDISPATCH __fastcall TWebBrowser::Get_Application_(void)
{
  return GetControlInterface().get_Application_();
}

LPDISPATCH __fastcall TWebBrowser::Get_Parent(void)
{
  return GetControlInterface().get_Parent();
}

LPDISPATCH __fastcall TWebBrowser::Get_Container(void)
{
  return GetControlInterface().get_Container();
}

LPDISPATCH __fastcall TWebBrowser::Get_Document(void)
{
  return GetControlInterface().get_Document();
}

Shdocvw_tlb::tagREADYSTATE __fastcall TWebBrowser::Get_ReadyState(void)
{
  return GetControlInterface().get_ReadyState();
}

};     // namespace Shdocvw_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Shdocvw_ocx
{

void __fastcall PACKAGE Register()
{
  // [2]
  TComponentClass classes[] = {
                              __classid(Shdocvw_tlb::TWebBrowser_V1), 
                              __classid(Shdocvw_tlb::TWebBrowser)
                             };
  RegisterComponents("ActiveX", classes,
                     sizeof(classes)/sizeof(classes[0])-1);
}
};     // namespace Shdocvw_ocx
