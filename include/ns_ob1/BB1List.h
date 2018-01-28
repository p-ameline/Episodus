/*
**
** BB1List.h
**
*/

#ifndef _BB1List_h
# define _BB1List_h 1

# ifdef __GNUC__
#  pragma interface
# endif

# ifndef _OWLDLL
#   ifndef _ENTERPRISE_DLL
#     include <stl.h>
#   else
#     include <vector>
#   endif
# else
#  include <vector>
# endif

# include <assert.h>
# include <iostream> // for operator<<(...)

/**
* \brief Oject List
*/
/**
* Non-intrusive list of T: does not require application to know slink
* Apparently, all of the methods must be inline for this to link.
* \todo Verifier qu'elle n'est pas utiliser et la retirer
* car ce n'est qu'une redéfinition de vector sans ajout
*/
template<class T>
class BB1List : public std::vector<T> // private slist_base
{
public:
	BB1List() { }                                   // null constructor

	BB1List(const BB1List& l)
	{
		if (l.empty())
			return ;
		for (BB1List<T>::const_iterator iter = l.begin() ; iter != l.end() ; iter++)
			push_back(*iter) ;
	}

	~BB1List() { clear() ; } ;

	void			append(const T a) { push_back(a) ; } ;

	T					pop_front()
	{
		BB1List<T>::iterator  a = begin() ;
		T  result = *a ;
		erase(a) ;
		return result ;
	}

	void			push_front(const T a)
  {
    BB1List<T>::iterator  iter = begin() ;
    insert(iter, T(a)) ;
  }

  BB1List&	operator=(const BB1List& src)
	{
		vider() ;
		
		if (*this == src)
			return (*this) ;

		if (!src.empty())
			for (BB1List<T>::const_iterator iter = src.begin() ; iter != src.end() ; iter++)
				push_back(*iter) ;
		return (*this) ;
	}

	void			vider()
	{
		if (empty())
			return ;
		for (BB1List<T>::iterator iter = begin() ; iter != end() ; )
			erase(iter) ;
		clear() ;
	}
} ;

template<class T> std::ostream&	operator<<(std::ostream& os, const BB1List<T>& s) ;


#endif // _BB1List_h

