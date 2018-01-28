// ************************************************************************ //
// WARNING                                                                  //
// -------                                                                  //
// The types declared in this file were generated from data read from a     //
// Type Library. If this type library is explicitly or indirectly (via      //
// another type library referring to this type library) re-imported, or the //
// 'Refresh' command of the Type Library Editor activated while editing the //
// Type Library, the contents of this file will be regenerated and all      //
// manual modifications will be lost.                                       //
// ************************************************************************ //

// C++ TLBWRTR : $Revision:   1.96.1.27  $
// File generated on 16/06/2006 14:37:26 from Type Library described below.

// ************************************************************************ //
// Type Lib: C:\Program Files\Fichiers communs\Microsoft Shared\MSInfo\ieinfo5.ocx
// IID\LCID: {25959BEC-E700-11D2-A7AF-00C04F806200}\0
// Helpfile: C:\Program Files\Fichiers communs\Microsoft Shared\MSInfo\Msie.hlp
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\System32\stdole2.tlb )
//   (2) v4.0 StdVCL, (C:\WINDOWS\System32\STDVCL40.DLL)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#include "MSIELib_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Msielib_tlb
{



// *********************************************************************//
// OCX PROXY CLASS IMPLEMENTATION
// (The following variables/methods implement the class TMsie which
// allows "Msie Control" to be hosted in CBuilder IDE/apps).
// *********************************************************************//
TControlData TMsie::CControlData =
{
  // GUID of CoClass and Event Interface of Control
  {0x25959BEF, 0xE700, 0x11D2,{ 0xA7, 0xAF, 0x00, 0xC0, 0x4F, 0x80, 0x62, 0x00} }, // CoClass
  {0x25959BEE, 0xE700, 0x11D2,{ 0xA7, 0xAF, 0x00, 0xC0, 0x4F, 0x80, 0x62, 0x00} }, // Events

  // Count of Events and array of their DISPIDs
  0, NULL,

  // Pointer to Runtime License string
  NULL,  // HRESULT(0x80004005)

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

GUID     TMsie::DEF_CTL_INTF = {0x25959BED, 0xE700, 0x11D2,{ 0xA7, 0xAF, 0x00, 0xC0, 0x4F, 0x80, 0x62, 0x00} };
TNoParam TMsie::OptParam;

static inline void ValidCtrCheck(TMsie *)
{
   delete new TMsie((TComponent*)(0));
};

void __fastcall TMsie::InitControlData(void)
{
  ControlData = &CControlData;
};

void __fastcall TMsie::CreateControl(void)
{
  if (!m_OCXIntf)
  {
    _ASSERTE(DefaultDispatch);
    DefaultDispatch->QueryInterface(DEF_CTL_INTF, (LPVOID*)&m_OCXIntf);
  }
};

_DMsieDisp __fastcall TMsie::GetControlInterface(void)
{
  CreateControl();
  return m_OCXIntf;
};

void __fastcall TMsie::MSInfoRefresh(TOLEBOOL fForSave, long* pCancel)
{
  GetControlInterface().MSInfoRefresh(fForSave, pCancel);
}

TOLEBOOL __fastcall TMsie::MSInfoLoadFile(BSTR szFileName)
{
  return GetControlInterface().MSInfoLoadFile(szFileName);
}

void __fastcall TMsie::MSInfoSelectAll(void)
{
  GetControlInterface().MSInfoSelectAll();
}

void __fastcall TMsie::MSInfoCopy(void)
{
  GetControlInterface().MSInfoCopy();
}

void __fastcall TMsie::MSInfoUpdateView(void)
{
  GetControlInterface().MSInfoUpdateView();
}

long __fastcall TMsie::MSInfoGetData(long dwMSInfoView, long* pBuffer, long dwLength)
{
  return GetControlInterface().MSInfoGetData(dwMSInfoView, pBuffer, dwLength);
}

void __fastcall TMsie::AboutBox(void)
{
  GetControlInterface().AboutBox();
}

};     // namespace Msielib_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Msielib_ocx
{

void __fastcall PACKAGE Register()
{
  // [1]
  TComponentClass classes[] = {
                              __classid(Msielib_tlb::TMsie)
                             };
  RegisterComponents("ActiveX", classes,
                     sizeof(classes)/sizeof(classes[0])-1);
}
};     // namespace Msielib_ocx
