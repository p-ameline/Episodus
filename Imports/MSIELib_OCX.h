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
// File generated on 16/06/2006 14:37:25 from Type Library described below.

// ************************************************************************ //
// Type Lib: C:\Program Files\Fichiers communs\Microsoft Shared\MSInfo\ieinfo5.ocx
// IID\LCID: {25959BEC-E700-11D2-A7AF-00C04F806200}\0
// Helpfile: C:\Program Files\Fichiers communs\Microsoft Shared\MSInfo\Msie.hlp
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\System32\stdole2.tlb )
//   (2) v4.0 StdVCL, (C:\WINDOWS\System32\STDVCL40.DLL)
// ************************************************************************ //
#ifndef   __MSIELib_OCX_h__
#define   __MSIELib_OCX_h__

#pragma option push -b -w-inl

#include <vcl/utilcls.h>
#if !defined(__UTILCLS_H_VERSION) || (__UTILCLS_H_VERSION < 0x0101)
#error "This file requires an newer version of the header file UTILCLS.H"
#endif

#include <olectl.h>
#include <ocidl.h>
#if !defined(_NO_VCL)   // For Console applications that don't use VCL
#include <vcl/stdvcl.hpp>
#endif  //   _NO_VCL
#include <vcl/ocxproxy.h>

#include "MSIELib_TLB.h"
namespace Msielib_tlb
{

// *********************************************************************//
// HelpString: Msie ActiveX Control module
// Version:    1.0
// *********************************************************************//


// *********************************************************************//
// OCX PROXY CLASS DECLARATION
// Control Name     : TMsie
// Help String      : Msie Control
// Default Interface: _DMsie
// Def. Intf. Object: _DMsieDisp
// Def. Intf. DISP? : Yes
// Event   Interface: _DMsieEvents
// TypeFlags        : (34) CanCreate Control
// *********************************************************************//

// *********************************************************************//
// Definition of closures to allow VCL handlers to catch OCX events.    //
// *********************************************************************//
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// Proxy class to host Msie Control in CBuilder IDE/Applications.
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class PACKAGE TMsie : public TOleControl
{
  OVERLOADED_PROP_METHODS;
  static TNoParam  OptParam;
  static GUID      DEF_CTL_INTF;

  // Instance of Closures to expose OCX Events as VCL ones
  //

  // Default Interace of OCX
  //
  _DMsieDisp m_OCXIntf;

  // VCL Property Getters/Setters which delegate to OCX
  //

  // Static variables used by all instances of OCX proxy
  //
  static TControlData CControlData;
  static GUID         CTL_DEF_INTF;

  // Method providing access to interface as __property 
  //
  _DMsieDisp __fastcall GetControlInterface(void);

protected:
  void     __fastcall CreateControl  (void);
  void     __fastcall InitControlData(void);

public:
  virtual  __fastcall TMsie(TComponent* AOwner) : TOleControl(AOwner)
  {};
  virtual  __fastcall TMsie(HWND        Parent) : TOleControl(Parent)
  {};

  // OCX methods
  //
  void            __fastcall MSInfoRefresh(TOLEBOOL fForSave, long* pCancel);
  TOLEBOOL        __fastcall MSInfoLoadFile(BSTR szFileName);
  void            __fastcall MSInfoSelectAll(void);
  void            __fastcall MSInfoCopy(void);
  void            __fastcall MSInfoUpdateView(void);
  long            __fastcall MSInfoGetData(long dwMSInfoView, long* pBuffer, long dwLength);
  void            __fastcall AboutBox(void);

  // OCX properties
  //
  __property _DMsieDisp ControlInterface={ read=GetControlInterface };

  // Published properties
  //
__published:

  // Standard/Extended properties
  //

  // OCX properties
  //
  __property int MSInfoView={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=1 };

  // OCX Events
  //
};
typedef TMsie  TMsieProxy;

};     // namespace Msielib_tlb

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace Msielib_tlb;
#endif

#pragma option pop

#endif // __MSIELib_OCX_h__
