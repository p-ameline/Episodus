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
#ifndef   __MSIELib_TLB_h__
#define   __MSIELib_TLB_h__

#pragma option push -b -w-inl

#include <vcl/utilcls.h>
#if !defined(__UTILCLS_H_VERSION) || (__UTILCLS_H_VERSION < 0x0101)
#error "This file requires an newer version of the header file UTILCLS.H"
#endif

#include <olectl.h>
#include <ocidl.h>
#if defined(USING_ATLVCL) || defined(USING_ATL)
#if !defined(__TLB_NO_EVENT_WRAPPERS)
#include <atl/atlmod.h>
#endif
#endif


// *********************************************************************//
// Forward reference of some VCL types (to avoid including STDVCL.HPP)  //
// *********************************************************************//
namespace Stdvcl {class IStrings; class IStringsDisp;}
using namespace Stdvcl;

namespace Msielib_tlb
{

// *********************************************************************//
// HelpString: Msie ActiveX Control module
// Version:    1.0
// *********************************************************************//


// *********************************************************************//
// GUIDS declared in the TypeLibrary. Following prefixes are used:      //
//   Type Libraries     : LIBID_xxxx                                    //
//   CoClasses          : CLSID_xxxx                                    //
//   DISPInterfaces     : DIID_xxxx                                     //
//   Non-DISP interfaces: IID_xxxx                                      //
// *********************************************************************//
DEFINE_GUID(LIBID_MSIELib, 0x25959BEC, 0xE700, 0x11D2, 0xA7, 0xAF, 0x00, 0xC0, 0x4F, 0x80, 0x62, 0x00);
DEFINE_GUID(DIID__DMsie, 0x25959BED, 0xE700, 0x11D2, 0xA7, 0xAF, 0x00, 0xC0, 0x4F, 0x80, 0x62, 0x00);
DEFINE_GUID(DIID__DMsieEvents, 0x25959BEE, 0xE700, 0x11D2, 0xA7, 0xAF, 0x00, 0xC0, 0x4F, 0x80, 0x62, 0x00);
DEFINE_GUID(CLSID_Msie, 0x25959BEF, 0xE700, 0x11D2, 0xA7, 0xAF, 0x00, 0xC0, 0x4F, 0x80, 0x62, 0x00);

// *********************************************************************//
// Forward declaration of types defined in TypeLibrary                  //
// *********************************************************************//
interface DECLSPEC_UUID("{25959BED-E700-11D2-A7AF-00C04F806200}") _DMsie;
interface DECLSPEC_UUID("{25959BEE-E700-11D2-A7AF-00C04F806200}") _DMsieEvents;

// *********************************************************************//
// Declaration of CoClasses defined in Type Library                     //
// (NOTE: Here we map each CoClass to its Default Interface)            //
//                                                                      //
// The LIBID_OF_ macro(s) map a LIBID_OF_CoClassName to the GUID of this//
// TypeLibrary. It simplifies the updating of macros when CoClass name  //
// change.                                                              //
// *********************************************************************//
typedef _DMsie Msie;

#define LIBID_OF_Msie (&LIBID_MSIELib)
// *********************************************************************//
// Interface: _DMsie
// Flags:     (4112) Hidden Dispatchable
// GUID:      {25959BED-E700-11D2-A7AF-00C04F806200}
// *********************************************************************//
interface _DMsie : public IDispatch
{
// DispInterfaces have no methods. Their methods and properties are
// accessed via IDispatch::Invoke. See the class _DMsieDisp provided
// below for an easy way to access the methods/properties of this
// interface.
};

// *********************************************************************//
// Interface: _DMsieEvents
// Flags:     (4096) Dispatchable
// GUID:      {25959BEE-E700-11D2-A7AF-00C04F806200}
// *********************************************************************//
interface _DMsieEvents : public IDispatch
{
// DispInterfaces have no methods. Their methods and properties are
// accessed via IDispatch::Invoke. See the class _DMsieEventsDisp provided
// below for an easy way to access the methods/properties of this
// interface.
};

#if !defined(__TLB_NO_INTERFACE_WRAPPERS)
// *********************************************************************//
// DispIntf:  _DMsie
// Flags:     (4112) Hidden Dispatchable
// GUID:      {25959BED-E700-11D2-A7AF-00C04F806200}
// *********************************************************************//
template<class T>
class _DMsieDispT : public TAutoDriver<_DMsie>
{
public:
  _DMsieDispT(){}

  _DMsieDispT(_DMsie *pintf)
  {
    TAutoDriver<_DMsie>::Bind(pintf);
  }

  _DMsieDispT& operator=(_DMsie *pintf)
  {
    TAutoDriver<_DMsie>::Bind(pintf);
    return *this;
  }

  HRESULT BindDefault(/*Binds to new instance of CoClass Msie*/)
  {
    return OLECHECK(Bind(CLSID_Msie));
  }

  HRESULT BindRunning(/*Binds to a running instance of CoClass Msie*/)
  {
    return BindToActive(CLSID_Msie);
  }

  void            __fastcall set_MSInfoView(long Value);
  long            __fastcall get_MSInfoView();
  void            __fastcall MSInfoRefresh(TOLEBOOL fForSave, long* pCancel);
  TOLEBOOL        __fastcall MSInfoLoadFile(BSTR szFileName);
  void            __fastcall MSInfoSelectAll();
  void            __fastcall MSInfoCopy();
  void            __fastcall MSInfoUpdateView();
  long            __fastcall MSInfoGetData(long dwMSInfoView, long* pBuffer, long dwLength);
  void            __fastcall AboutBox();

  __property   long            MSInfoView = {read = get_MSInfoView, write = set_MSInfoView};
};
typedef _DMsieDispT<_DMsie> _DMsieDisp;

// *********************************************************************//
// DispIntf:  _DMsieEvents
// Flags:     (4096) Dispatchable
// GUID:      {25959BEE-E700-11D2-A7AF-00C04F806200}
// *********************************************************************//
template <class T>
class _DMsieEventsDispT : public TAutoDriver<_DMsieEvents>
{
public:
  _DMsieEventsDispT(){}

  void Attach(LPUNKNOWN punk)
  { m_Dispatch = static_cast<T*>(punk); }


};
typedef _DMsieEventsDispT<_DMsieEvents> _DMsieEventsDisp;

// *********************************************************************//
// DispIntf:  _DMsie
// Flags:     (4112) Hidden Dispatchable
// GUID:      {25959BED-E700-11D2-A7AF-00C04F806200}
// *********************************************************************//
template <class T> void __fastcall
_DMsieDispT<T>::set_MSInfoView(long /*[VT_I4:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("MSInfoView"), DISPID(1));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> long /*[VT_I4:0]*/ __fastcall
_DMsieDispT<T>::get_MSInfoView()
{
  static _TDispID _dispid(*this, OLETEXT("MSInfoView"), DISPID(1));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DMsieDispT<T>::MSInfoRefresh(TOLEBOOL fForSave, long* pCancel)
{
  static _TDispID _dispid(*this, OLETEXT("MSInfoRefresh"), DISPID(2));
  TAutoArgs<2> _args;
  _args[1] = fForSave /*[VT_BOOL:0]*/;
  _args[2] = pCancel /*[VT_I4:1]*/;
  OleProcedure(_dispid, _args);
}

template <class T> TOLEBOOL __fastcall
_DMsieDispT<T>::MSInfoLoadFile(BSTR szFileName)
{
  static _TDispID _dispid(*this, OLETEXT("MSInfoLoadFile"), DISPID(3));
  TAutoArgs<1> _args;
  _args[1] = szFileName /*[VT_BSTR:0]*/;
  OleFunction(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DMsieDispT<T>::MSInfoSelectAll()
{
  static _TDispID _dispid(*this, OLETEXT("MSInfoSelectAll"), DISPID(4));
  OleProcedure(_dispid);
}

template <class T> void __fastcall
_DMsieDispT<T>::MSInfoCopy()
{
  static _TDispID _dispid(*this, OLETEXT("MSInfoCopy"), DISPID(5));
  OleProcedure(_dispid);
}

template <class T> void __fastcall
_DMsieDispT<T>::MSInfoUpdateView()
{
  static _TDispID _dispid(*this, OLETEXT("MSInfoUpdateView"), DISPID(6));
  OleProcedure(_dispid);
}

template <class T> long __fastcall
_DMsieDispT<T>::MSInfoGetData(long dwMSInfoView, long* pBuffer, long dwLength)
{
  static _TDispID _dispid(*this, OLETEXT("MSInfoGetData"), DISPID(7));
  TAutoArgs<3> _args;
  _args[1] = dwMSInfoView /*[VT_I4:0]*/;
  _args[2] = pBuffer /*[VT_I4:1]*/;
  _args[3] = dwLength /*[VT_I4:0]*/;
  OleFunction(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DMsieDispT<T>::AboutBox()
{
  static _TDispID _dispid(*this, OLETEXT("AboutBox"), DISPID(-552));
  OleProcedure(_dispid);
}

// *********************************************************************//
// DispIntf:  _DMsieEvents
// Flags:     (4096) Dispatchable
// GUID:      {25959BEE-E700-11D2-A7AF-00C04F806200}
// *********************************************************************//
#endif  //   __TLB_NO_INTERFACE_WRAPPERS


#if !defined(__TLB_NO_EVENT_WRAPPERS) && defined(USING_ATLVCL)
// *********************************************************************//
// CONNECTIONPOINT/EVENT PROXY
// CoClass         : Msie
// Event Interface : _DMsieEvents
// *********************************************************************//
template <class T>
class TEvents_Msie : public IConnectionPointImpl<T,
                                                 &DIID__DMsieEvents,
                                                 CComUnkArray<CONNECTIONPOINT_ARRAY_SIZE> >
{
public:
protected:
  _DMsieEventsDisp m_EventIntfObj;
};

#endif  //   __TLB_NO_EVENT_WRAPPERS

};     // namespace Msielib_tlb

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace Msielib_tlb;
#endif

#pragma option pop

#endif // __MSIELib_TLB_h__
