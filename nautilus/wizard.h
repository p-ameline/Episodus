//---------------------------------------------------------------------------#ifndef wizardH
#define wizardH

#ifndef _PWCLASS
#define _PWCLASS
#endif

#ifndef __cplusplus
#include <windows.h>
#include <olectl.h>
#else
#include <owl/pch.h>
#include <owl/oledialg.h>
#endif

#include <pshpack8.h>

#include "partage/ole_utils.h"

// Get a TAutoString from a TAutoVal hosting a BSTR
//
// Fonction pour récupérer sans bug une TAutoString
// à partir d'une TAutoVal qui contient une BSTR
TAutoString autoString(const TAutoVal& val)
{
	return TAutoString((val.StrVal()).operator wchar_t*());
}

#ifdef __cplusplus
#if !defined(__push_arg)
#define __push_arg

namespace PWCLASS {

class _PWCLASS SafeArrayBase
{
public:
   virtual VARTYPE type() = 0;
   virtual void copy(SAFEARRAY** pparray) = 0;
   virtual SAFEARRAY** ref() = 0;
};

template <class T>
class _PWCLASS SafeArray;

template <class T>
struct _PWCLASS VarType
{
};

template<>
struct _PWCLASS VarType<short>
{
   static VARTYPE Val(){return VT_I2;}
};

template<>
struct _PWCLASS VarType<long>
{
   static VARTYPE Val(){return VT_I4;}
};

template<>
struct _PWCLASS VarType<float>
{
   static VARTYPE Val(){return VT_R4;}
};

template<>
struct _PWCLASS VarType<double>
{
   static VARTYPE Val(){return VT_R8;}
};

template<>
struct _PWCLASS VarType<CY>
{
   static VARTYPE Val(){return VT_CY;}
};

template<>
struct _PWCLASS VarType<TAutoDate>
{
   static VARTYPE Val(){return VT_DATE;}
};

template<>
struct _PWCLASS VarType<BSTR>
{
   static VARTYPE Val(){return VT_BSTR;}
};

template<>
struct _PWCLASS VarType<TAutoString>
{
   static VARTYPE Val(){return VT_BSTR;}
};

template<>
struct _PWCLASS VarType<IDispatch*>
{
   static VARTYPE Val(){return VT_DISPATCH;}
};

template<>
struct _PWCLASS VarType<TBool>
{
   static VARTYPE Val(){return VT_BOOL;}
};

template<>
struct _PWCLASS VarType<TAutoBool>
{
   static VARTYPE Val(){return VT_BOOL;}
};

template<>
struct _PWCLASS VarType<IUnknown*>
{
   static VARTYPE Val(){return VT_UNKNOWN;}
};

template<>
struct _PWCLASS VarType<signed char>
{
   static VARTYPE Val(){return VT_I1;}
};

template<>
struct _PWCLASS VarType<unsigned char>
{
   static VARTYPE Val(){return VT_UI1;}
};

template<>
struct _PWCLASS VarType<unsigned short>
{
   static VARTYPE Val(){return VT_UI2;}
};

template<>
struct _PWCLASS VarType<unsigned long>
{
   static VARTYPE Val(){return VT_UI4;}
};

template<>
struct _PWCLASS VarType<int>
{
   static VARTYPE Val(){return VT_INT;}
};

template<>
struct _PWCLASS VarType<unsigned int>
{
   static VARTYPE Val(){return VT_UINT;}
};

template<>
struct _PWCLASS VarType<TAutoVal>
{
   static VARTYPE Val(){return VT_VARIANT;}
};

template <class T>
inline void push_arg(TAutoVal& val, T arg)
{
   val = arg;
}

inline void push_arg(TAutoVal& val, const TAutoString& arg)
{
   val = const_cast<TAutoString&>(arg);
}

inline void push_arg(TAutoVal& val, SafeArrayBase& arg)
{
   ((VARIANT*)(&val))->vt = arg.type();
   arg.copy(&((VARIANT*)(&val))->parray);
}

inline void push_arg(TAutoVal& val, SafeArrayBase* arg)
{
   ((VARIANT*)(&val))->vt = arg->type() | (VARTYPE)VT_BYREF;
   ((VARIANT*)(&val))->pparray = arg->ref();
}

inline void push_arg(TAutoVal& val, BSTR* arg)
{
   ((VARIANT*)(&val))->vt = VT_BSTR | VT_BYREF;
   ((VARIANT*)(&val))->pbstrVal = arg;
}

inline void push_arg(TAutoVal& val, void* arg)
{
   ((VARIANT*)(&val))->vt = VT_BYREF;
   ((VARIANT*)(&val))->byref = arg;
}

inline void push_arg(TAutoVal& val, IUnknown* arg)
{
   ((VARIANT*)(&val))->vt = VT_UNKNOWN;
   ((VARIANT*)(&val))->punkVal = arg;
}

inline void push_arg(TAutoVal& val, IDispatch* arg)
{
   ((VARIANT*)(&val))->vt = VT_DISPATCH;
   ((VARIANT*)(&val))->pdispVal = arg;
}

inline void push_arg(TAutoVal& val, IUnknown** arg)
{
   ((VARIANT*)(&val))->vt = VT_UNKNOWN | VT_BYREF;
   ((VARIANT*)(&val))->ppunkVal = arg;
}

inline void push_arg(TAutoVal& val, IDispatch** arg)
{
   ((VARIANT*)(&val))->vt = VT_DISPATCH | VT_BYREF;
   ((VARIANT*)(&val))->ppdispVal = arg;
}

inline void push_arg(TAutoVal& val, TAutoVal* arg)
{
   if(arg)
   {
      ((VARIANT*)(&val))->vt = VT_VARIANT | VT_BYREF;
      ((VARIANT*)(&val))->pvarVal = (VARIANT*)arg;
   }
   else
      val = TNoArg();
}

inline void push_arg(TAutoVal& val, TAutoVal** arg)
{
   TXOle::Check(E_INVALIDARG);
}

template <class T>
class _PWCLASS SafeArrayElementRef
{
private:
   long index;
   SafeArray<T>* ptr;
   SafeArrayElementRef(long i, SafeArray<T>* p)
   {
      index = i;
      ptr = p;
   }
public:
   SafeArrayElementRef(const SafeArrayElementRef& e)
   {
      index = e.index;
      ptr = e.ptr;
   }
   SafeArrayElementRef& operator=(const SafeArrayElementRef& e)
   {
      index = e.index;
      ptr = e.ptr;
      return *this;
   }   operator TAutoVal();

   SafeArrayElementRef& operator=(const TAutoVal& val);

   SafeArrayElementRef& operator=(const T& val)
   {
      TAutoVal v;
      push_arg(v, val);
      return (*this = v);
   }
friend SafeArray<T>;
};


template<>
class _PWCLASS SafeArrayElementRef<TAutoVal>
{
private:
   long index;
   SafeArray<TAutoVal>* ptr;
   SafeArrayElementRef(long i, SafeArray<TAutoVal>* p)
   {
      index = i;
      ptr = p;
   }
public:
   SafeArrayElementRef(const SafeArrayElementRef& e)
   {
      index = e.index;
      ptr = e.ptr;
   }
   SafeArrayElementRef& operator=(const SafeArrayElementRef& e)
   {
      index = e.index;
      ptr = e.ptr;
      return *this;
   }
   operator TAutoVal();

   SafeArrayElementRef& operator=(const TAutoVal& val);

friend SafeArray<TAutoVal>;
};

template <class T>
class _PWCLASS SafeArray: public SafeArrayBase
{
private:
   SAFEARRAY* ptr;

public:
   SafeArray()
   {
      ptr = 0;
   }

   SafeArray(unsigned long len)
   {
      SAFEARRAYBOUND bound;
      bound.cElements = len;
      bound.lLbound = 0;
      ptr = SafeArrayCreate(_PWCLASS::VarType<T>::Val(), 1, &bound);
      if(ptr == 0)
         throw TXOle("Unable to create SAFEARRAY structure", S_OK);
   }

   SafeArray(unsigned long len, T* values);

   SafeArray(unsigned int cDims, SAFEARRAYBOUND* bound)
   {
      ptr = SafeArrayCreate(_PWCLASS::VarType<T>::Val(), cDims, bound);
      if(ptr == 0)
         throw TXOle("Unable to create SAFEARRAY structure", S_OK);
   }

   SafeArray(const SafeArray<T>& s)
   {
      if(s.ptr)
         TXOle::Check(SafeArrayCopy(s.ptr, &ptr));
      else
         ptr = 0;
   }


   SafeArray(const SAFEARRAY* p)
   {
      TXOle::Check(SafeArrayCopy(const_cast<SAFEARRAY*>(p), &ptr));
   }

   SafeArray(const TAutoVal& Val)
   {
      if((((VARIANT*)&Val)->vt & VT_ARRAY) == 0)
         throw TXOle("Invalid VariantArg passed to SafeArray", E_INVALIDARG);
      TXOle::Check(SafeArrayCopy(((VARIANT*)&Val)->parray, &ptr));
   }

   ~SafeArray()
   {
      if(ptr)
         TXOle::Check(SafeArrayDestroy(ptr));
   }

   SafeArray<T>& operator=(const SafeArray<T>& s)
   {
      this->~SafeArray<T>();
      new (this) SafeArray<T>(s);
      return *this;
   }

   SafeArray<T>& operator=(const SAFEARRAY* p)
   {
      this->~SafeArray<T>();
      new (this) SafeArray<T>(p);
      return *this;
   }

   SafeArray<T>& operator=(const TAutoVal& Val)
   {
      this->~SafeArray<T>();
      new (this) SafeArray<T>(Val);
      return *this;
   }

   void MustBeBound()
   {
      if(ptr == 0)
         throw TXOle("Attempt to access uninitialised array", S_OK);
   }

   unsigned short Dimentions()
   {
      return ptr->cDims;
   }

   SAFEARRAYBOUND* Bound(unsigned short i)
   {
      if(i >= ptr->cDims)
         throw TXOle("Attempt to access array out of range", S_OK);
      return ptr->rgsabound + i;
   }

   TAutoVal GetValue(long FAR* indices)
   {
      MustBeBound();
      TAutoVal v;
      void* data;
      if(ptr->fFeatures & (FADF_DISPATCH | FADF_UNKNOWN | FADF_BSTR))
      {
         // we have a 4 byte value:
         TXOle::Check(SafeArrayGetElement(ptr, indices, &(((VARIANT*)&v)->punkVal)));
         ((VARIANT*)&v)->vt = (VARTYPE)((ptr->fFeatures & FADF_UNKNOWN) ? VT_UNKNOWN :
                                (ptr->fFeatures & FADF_BSTR) ? VT_BSTR :
                                (ptr->fFeatures & FADF_DISPATCH) ? VT_DISPATCH :
                                VT_VARIANT);
      }
      else if(ptr->fFeatures & FADF_VARIANT)
         TXOle::Check(SafeArrayGetElement(ptr, indices, &v));
      else
      {
         TXOle::Check(SafeArrayGetElement(ptr, indices, &(((VARIANT*)&v)->punkVal)));
         ((VARIANT*)&v)->vt = _PWCLASS::VarType<T>::Val();
      }
      return v;
   }

   void SetValue(long FAR* indices, TAutoVal v)
   {
      MustBeBound();
      // must coerce to the type we expect:
      if(ptr->fFeatures & FADF_VARIANT)
         TXOle::Check(SafeArrayPutElement(ptr, indices, (void*)&v));
      else if(ptr->fFeatures & (FADF_DISPATCH | FADF_UNKNOWN | FADF_BSTR))
      {
         // put a pointer directly:
         VARTYPE vt = ((VARIANT*)&v)->vt;
         if((vt != VT_DISPATCH) && (vt != VT_UNKNOWN) && (vt != VT_BSTR) )
            TXOle::Check(E_INVALIDARG);
         TXOle::Check(SafeArrayPutElement(ptr, indices, (void*)((VARIANT*)&v)->punkVal) );
      }
      else
      {
         TXOle::Check(VariantChangeType((VARIANT*)&v, (VARIANT*)&v, 0, _PWCLASS::VarType<T>::Val()));
         TXOle::Check(SafeArrayPutElement(ptr, indices, (void*)&((VARIANT*)&v)->punkVal) );
      }
   }

   SafeArrayElementRef<T> operator[](long i)
   {
      MustBeBound();
      return SafeArrayElementRef<T>(i, this);
   }

   operator SAFEARRAY* ()
   {
      return ptr;
   }

   operator SAFEARRAY** ()
   {
      //need to set ptr to null first:
      if(ptr)
      {
         this->~SafeArray<T>();
         new (this) SafeArray<T>();
      }
      return &ptr;
   }

   virtual VARTYPE type()
   {
      return _PWCLASS::VarType<T>::Val() | (VARTYPE)VT_ARRAY;
   }

   virtual void copy(SAFEARRAY** pparray)
   {
      MustBeBound();
      SafeArrayCopy(ptr, pparray);
   }

   virtual SAFEARRAY** ref()
   {
      return &ptr;
   }

   friend SafeArray<T>;
   friend void push_arg(TAutoVal& val, SafeArrayBase& arg);
};

template <class T>
SafeArray<T>::SafeArray(unsigned long len, T* values)
{
   SAFEARRAYBOUND bound;
   bound.cElements = len;
   bound.lLbound = 0;
   ptr = SafeArrayCreate(_PWCLASS::VarType<T>::Val(), 1, &bound);
   if(ptr == 0)
      throw TXOle("Unable to create SAFEARRAY structure", S_OK);
   for(unsigned long i = 0; i < len; ++i)
   {
      TAutoVal v;
      push_arg(v, *(values + i));
      SetValue((long*)&i, v);
   }
}

template <class T>
SafeArrayElementRef<T>::operator TAutoVal ()
{
   TAutoVal val;
   val = ptr->GetValue((long*)&index);
   return val;
}

template <class T>
SafeArrayElementRef<T>& SafeArrayElementRef<T>::operator=(const TAutoVal& val)
{
   ptr->SetValue((long*)&index, val);
   return *this;
}

inline SafeArrayElementRef<TAutoVal>::operator TAutoVal ()
{
   TAutoVal val;
   val = ptr->GetValue((long*)&index);
   return val;
}

inline SafeArrayElementRef<TAutoVal>& SafeArrayElementRef<TAutoVal>::operator=(const TAutoVal& val)
{
   ptr->SetValue((long*)&index, val);
   return *this;
}

} // namespace PWCLASS

#endif
#endif

#endif
// fin de wizard.h
////////////////////////////////////////////////////////////////////////////////

