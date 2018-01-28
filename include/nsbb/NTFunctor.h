#ifndef _NT_FUNCTOR_
# define _NT_FUNCTOR_

class Functor
{
	public:

		virtual void Call() = 0 ;
    virtual void operator() () = 0 ;
} ;

template <class TypeClass>
class MemFunctor : public Functor
{
	protected:

		TypeClass *_pt ;
    typedef void (TypeClass::*Thunk)(void) ;
    Thunk _thunk ;

	public:

		MemFunctor(TypeClass* pt, Thunk thunk) : _pt(pt)
		{
			_thunk = thunk ;
  	}
		virtual	void operator()()
		{
			(_pt->*_thunk)() ;
		}
		virtual void Call()
		{
    	(_pt->*_thunk)() ;
		}
} ;

/*
** Foncteur d'arite 1
*/

/* Tabun's version

template <class TypeClass, class ParamType>
class MemUnaFunctor  : public Functor
{
	protected:

    TypeClass *_pt ;
    typedef void (TypeClass::*Thunk)(ParamType) ;
    Thunk _thunk ;

	public:

		MemUnaFunctor(TypeClass* pt, Thunk thunk) : _pt(pt)
		{
    	_thunk = thunk ;
		}
		virtual	void operator()(ParamType i)
		{
			(_pt->*_thunk)(i) ;
		}
		virtual void Call(ParamType i)
		{
			(_pt->*_thunk)(i) ;
		}
} ;

template <class ParamType>
class UnaFunctor  : public Functor
{
	public:

		typedef void (Thunk)(ParamType) ;

	protected:

    Thunk* _thunk ;

	public:

		UnaFunctor(Thunk* thunk)
		{
			_thunk = thunk ;
		}
		virtual	void operator()(ParamType i)
		{
			(*_thunk)(i) ;
		}
		virtual void Call(ParamType i)
		{
			(*_thunk)(i) ;
		}
} ;

*/

template <class ParamType>
class UnaFunctor
{
	public:

		virtual void Call(ParamType i) = 0 ;
    virtual void operator() (ParamType i) = 0 ;
} ;

template <class ParamType>
class SimpleUnaFunctor : public UnaFunctor<ParamType>
{
	public:

		typedef void (Thunk)(ParamType) ;

	protected:

    Thunk* _thunk ;

	public:

		SimpleUnaFunctor(Thunk* thunk)
		{
			_thunk = thunk ;
		}
		virtual	void operator()(ParamType i)
		{
			(*_thunk)(i) ;
		}
		virtual void Call(ParamType i)
		{
			(*_thunk)(i) ;
		}
} ;

template <class TypeClass, class ParamType>
class MemUnaFunctor  : public UnaFunctor<ParamType>
{
	protected:

    TypeClass    *_pt ;
    typedef void (TypeClass::*Thunk)(ParamType) ;
    Thunk        _thunk ;

	public:

		MemUnaFunctor(TypeClass* pt, Thunk thunk) : _pt(pt)
		{
    	_thunk = thunk ;
		}
		virtual	void operator()(ParamType i)
		{
			(_pt->*_thunk)(i) ;
		}
		virtual void Call(ParamType i)
		{
			(_pt->*_thunk)(i) ;
		}
} ;

/*
** Sert a emuler des swicth dans un tableau
** On doit passer une fonction permettant la comparaion de cle
*/
template<class T>
struct  FunctorBucket
{
	protected:

		Functor*  _functor ;
		T*        _key ;
		typedef bool (*comp)(T* key, T* key2) ;
		comp      _comp ;

  public:

		FunctorBucket(Functor* func, T* key, comp com) :_functor(func),_key(key), _comp(com)
		{
		}
		~FunctorBucket()
		{
			if (NULL != _functor)
      	(*_functor)() ;
      if (NULL != _key)
      	delete (_key) ;
    }
		/*
    ** Test si la clé est identique
    */
    bool operator == (T* key)
		{
			return (*_com)(_key, key) ;
		}
} ;

#endif  // # define _NT_FUNCTOR_

