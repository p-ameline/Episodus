/*
**
** BB1Set.h
**
*/

#ifndef _BB1Set_h
#define _BB1Set_h 1

#ifdef __GNUC__
#pragma interface
#endif

#define MININT -314159

#include <iostream.h> // for operator<<(...)
#include "BB1List.h"

typedef	BB1List<class T>::const_iterator	BB1ListCIter ;

template<class T> class BB1Set : public BB1List<T>
{
 public:

  BB1Set() : BB1List<T>() {}
  BB1Set(const BB1Set<T>& s) : BB1List<T>()
  {
  	if (false == s.empty())
    	for (BB1Set<T>::const_iterator p = s.begin() ; s.end() != p ; p++)
      	push_back(new T(*p)) ;
  }

  const T	*seek(const T& e) const
  {
  	if (empty())
    	return 0 ;

    for (BB1Set<T>::const_iterator p = begin() ; p != end() ; p++)
      if (e == (*p))
				return p ;

    return 0 ;
	}

	void			append(const T a)
  {
  	push_back(new T(*a)) ;
  }

	T					pop_front()
	{
  	if (empty())
    	return 0 ;

		BB1Set<T>::iterator  a = begin() ;
		T  result = *a ;
		erase(a) ;

		return result ;
	}

	void			push_front(const T a)
	{
		BB1Set<T>::iterator iter = begin() ;
		insert(iter, new T(*a)) ;
	}

	BB1Set&	operator=(const BB1Set&	src)
	{
		if (this == &src)
			return (*this) ;

    vider() ;

		if (false == src.empty())
			for (BB1Set<T>::const_iterator iter = src.begin() ; iter != src.end() ; iter++)
				push_back(new T(**iter)) ;

		return (*this) ;
	}

	void			vider()
	{
		if (empty())
			return ;

	 	for (BB1Set<T>::iterator iter = begin() ; end() != iter ; )
		{
      // There is a risk that objects are erased twice (for example, when
      // BB1Objects are deleted, they make certain that they are removed from
      // their knowledge base's ObjectSet ; if this ObjectSet is emptied, the
      // delete will hence issue an erase if erase is not done first
      //
      T pObject = *iter ;

      erase(iter) ;

      if (pObject)
			  delete pObject ;
		}
	}

/*
	BB1Set&	operator-=(BB1Set& s)
	{
		BB1Set<T> t(*this) ;

		vider() ;

		for (BB1Set<T>::iterator p = t.begin() ; p != t.end() ; p++)
		{
			T e = (*p) ;
			if (!s.seek(e))
				push_back(new T(*e)) ;
		}
		return (*this) ;
	}


	BB1Set&	operator|=(BB1Set<T>& s) // add takes care of duplicates
	{
		if ((&s && !s.empty()) && (&s != this))
		{
			for (BB1Set<T>::iterator p = s.begin() ; p != s.end() ; p++)
				push_back(new T(**p)) ;
		}
		return (*this) ;
	}
*/

} ;

template <class T> std::ostream&	operator<<(std::ostream& os, const BB1Set<T>& s) ;

#endif // _BB1Set_h

