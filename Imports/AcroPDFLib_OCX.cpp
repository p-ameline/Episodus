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
// File generated on 28/09/2008 14:46:57 from Type Library described below.

// ************************************************************************ //
// Type Lib: C:\Program Files\Fichiers communs\Adobe\Acrobat\ActiveX\AcroPDF.dll
// IID\LCID: {05BFD3F1-6319-4F30-B752-C7A22889BCC4}\0
// Helpfile: 
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\system32\stdole2.tlb)
//   (2) v4.0 StdVCL, (C:\WINDOWS\system32\STDVCL40.DLL)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#include "AcroPDFLib_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Acropdflib_tlb
{



// *********************************************************************//
// OCX PROXY CLASS IMPLEMENTATION
// (The following variables/methods implement the class TAcroPDF which
// allows "Adobe Acrobat Browser Control" to be hosted in CBuilder IDE/apps).
// *********************************************************************//
int   TAcroPDF::EventDispIDs[2] = {
    0x00000001, 0x00000002};

TControlData TAcroPDF::CControlData =
{
  // GUID of CoClass and Event Interface of Control
  {0xCA8A9780, 0x280D, 0x11CF,{ 0xA2, 0x4D, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00} }, // CoClass
  {0xE790E1D1, 0x9DE8, 0x4853,{ 0x8A, 0xC6, 0x93, 0x3D, 0x4F, 0xD9, 0xC9, 0x27} }, // Events

  // Count of Events and array of their DISPIDs
  2, &EventDispIDs,

  // Pointer to Runtime License string
  NULL,  // HRESULT(0x80004002)

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

GUID     TAcroPDF::DEF_CTL_INTF = {0x3B813CE7, 0x7C10, 0x4F84,{ 0xAD, 0x06, 0x9D, 0xF7, 0x6D, 0x97, 0xA9, 0xAA} };
TNoParam TAcroPDF::OptParam;

static inline void ValidCtrCheck(TAcroPDF *)
{
   delete new TAcroPDF((TComponent*)(0));
};

void __fastcall TAcroPDF::InitControlData(void)
{
  ControlData = &CControlData;
};

void __fastcall TAcroPDF::CreateControl(void)
{
  if (!m_OCXIntf)
  {
    _ASSERTE(DefaultDispatch);
    DefaultDispatch->QueryInterface(DEF_CTL_INTF, (LPVOID*)&m_OCXIntf);
  }
};

TCOMIAcroAXDocShim __fastcall TAcroPDF::GetControlInterface(void)
{
  CreateControl();
  return m_OCXIntf;
};

TOLEBOOL __fastcall TAcroPDF::LoadFile(BSTR fileName/*[in]*/)
{
  return GetControlInterface().LoadFile(fileName/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::setShowToolbar(TOLEBOOL On/*[in]*/)
{
  GetControlInterface().setShowToolbar(On/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::gotoFirstPage(void)
{
  GetControlInterface().gotoFirstPage();
}

void/*HR*/ __fastcall TAcroPDF::gotoLastPage(void)
{
  GetControlInterface().gotoLastPage();
}

void/*HR*/ __fastcall TAcroPDF::gotoNextPage(void)
{
  GetControlInterface().gotoNextPage();
}

void/*HR*/ __fastcall TAcroPDF::gotoPreviousPage(void)
{
  GetControlInterface().gotoPreviousPage();
}

void/*HR*/ __fastcall TAcroPDF::setCurrentPage(long n/*[in]*/)
{
  GetControlInterface().setCurrentPage(n/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::goForwardStack(void)
{
  GetControlInterface().goForwardStack();
}

void/*HR*/ __fastcall TAcroPDF::goBackwardStack(void)
{
  GetControlInterface().goBackwardStack();
}

void/*HR*/ __fastcall TAcroPDF::setPageMode(BSTR pageMode/*[in]*/)
{
  GetControlInterface().setPageMode(pageMode/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::setLayoutMode(BSTR layoutMode/*[in]*/)
{
  GetControlInterface().setLayoutMode(layoutMode/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::setNamedDest(BSTR namedDest/*[in]*/)
{
  GetControlInterface().setNamedDest(namedDest/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::Print(void)
{
  GetControlInterface().Print();
}

void/*HR*/ __fastcall TAcroPDF::printWithDialog(void)
{
  GetControlInterface().printWithDialog();
}

void/*HR*/ __fastcall TAcroPDF::setZoom(float percent/*[in]*/)
{
  GetControlInterface().setZoom(percent/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::setZoomScroll(float percent/*[in]*/, float left/*[in]*/, 
                                              float top/*[in]*/)
{
  GetControlInterface().setZoomScroll(percent/*[in]*/, left/*[in]*/, top/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::setView(BSTR viewMode/*[in]*/)
{
  GetControlInterface().setView(viewMode/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::setViewScroll(BSTR viewMode/*[in]*/, float offset/*[in]*/)
{
  GetControlInterface().setViewScroll(viewMode/*[in]*/, offset/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::setViewRect(float left/*[in]*/, float top/*[in]*/, 
                                            float width/*[in]*/, float height/*[in]*/)
{
  GetControlInterface().setViewRect(left/*[in]*/, top/*[in]*/, width/*[in]*/, height/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::printPages(long from/*[in]*/, long to/*[in]*/)
{
  GetControlInterface().printPages(from/*[in]*/, to/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::printPagesFit(long from/*[in]*/, long to/*[in]*/, 
                                              TOLEBOOL shrinkToFit/*[in]*/)
{
  GetControlInterface().printPagesFit(from/*[in]*/, to/*[in]*/, shrinkToFit/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::printAll(void)
{
  GetControlInterface().printAll();
}

void/*HR*/ __fastcall TAcroPDF::printAllFit(TOLEBOOL shrinkToFit/*[in]*/)
{
  GetControlInterface().printAllFit(shrinkToFit/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::setShowScrollbars(TOLEBOOL On/*[in]*/)
{
  GetControlInterface().setShowScrollbars(On/*[in]*/);
}

TVariant __fastcall TAcroPDF::GetVersions(void)
{
  return GetControlInterface().GetVersions();
}

void/*HR*/ __fastcall TAcroPDF::setCurrentHightlight(long a/*[in]*/, long b/*[in]*/, long c/*[in]*/, 
                                                     long d/*[in]*/)
{
  GetControlInterface().setCurrentHightlight(a/*[in]*/, b/*[in]*/, c/*[in]*/, d/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::setCurrentHighlight(long a/*[in]*/, long b/*[in]*/, long c/*[in]*/, 
                                                    long d/*[in]*/)
{
  GetControlInterface().setCurrentHighlight(a/*[in]*/, b/*[in]*/, c/*[in]*/, d/*[in]*/);
}

void/*HR*/ __fastcall TAcroPDF::postMessage(TVariant strArray/*[in]*/)
{
  GetControlInterface().postMessage(strArray/*[in]*/);
}

TVariant __fastcall TAcroPDF::Get_messageHandler(void)
{
  return GetControlInterface().get_messageHandler();
}

void/*HR*/ __fastcall TAcroPDF::Set_messageHandler(TVariant pVarOut/*[in]*/)
{
  GetControlInterface().set_messageHandler(pVarOut/*[in]*/);
}

};     // namespace Acropdflib_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Acropdflib_ocx
{

void __fastcall PACKAGE Register()
{
  // [1]
  TComponentClass classes[] = {
                              __classid(Acropdflib_tlb::TAcroPDF)
                             };
  RegisterComponents("ActiveX", classes,
                     sizeof(classes)/sizeof(classes[0])-1);
}
};     // namespace Acropdflib_ocx
