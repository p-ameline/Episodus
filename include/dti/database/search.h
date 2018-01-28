//
//  search.h
//  
//  Copyright (c) 2004 Kristian Lippert, Danish Technological Institute, Informatics
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef SEARCH_H_KRISTIANLIPPERT_20040729_1414
#define SEARCH_H_KRISTIANLIPPERT_20040729_1414

#include <iterator>

#include "detail/database_autolib.h"
#include "detail/search_binder.h"
#include "detail/binder_macros.h" // contains the BEGIN_DATA_BINDING + more macros

namespace dti{
namespace db{


// Requirement for R:
//  BEGIN_DB_BINDING macros to make database binding
//  Default constructor
//  Copy constructor
//  Assignment operator


class database;
	
template <typename R>
class search: public std::iterator<std::input_iterator_tag, R, std::ptrdiff_t, const R *, const R&>
{
public:
	// To conform to compiler that uses 2-phase lookup because iterator is dependent base
	typedef std::input_iterator_tag		iterator_category;
	typedef R							value_type;
	typedef const R&		            reference;
	typedef const R*			        pointer;
	typedef std::ptrdiff_t	            difference_type;

	search( database & db, const std::string & sqlStmt)
		: binder_( db, sqlStmt )
	{
		typename R::template create_bindings<R>( *this );
		binder_.bind_all_values_to_db();
		binder_.get_next( &record_ );
	}

	search() { }

	search end() { return search(); };

	reference operator*() const { return record_; }
	pointer operator->() const { return &record_; }

	bool operator== ( const search & rhs ) const { 	return binder_ == rhs.binder_;	}
	bool operator!= ( const search & rhs ) const { 	return binder_ != rhs.binder_;	}

	search& operator++() // pre-decrement
	{
		binder_.get_next( &record_ );
		return *this;
	}

	search operator++( int ) // post-decrement
	{
		search s = *this;
		++( *this );
		return s;
	}

	// index is by call order
	template <typename T> 
	void bind( R & r, detail::field_attributes fa, T & t )                                                         
	{ binder_.bind( &r, fa, t);}
	
	template <typename T> 
	void bind( R & r, detail::field_attributes fa, T & t, std::size_t max_size )                                   
	{ binder_.bind( &r, fa, t, max_size );}

    // index is fetched from metadata
	template <typename T> 
	void bind( R & r, detail::field_attributes fa, T & t, const std::string & column_name )                        
	{ binder_.bind( &r, fa, t, column_name );	}

	template <typename T> 
	void bind( R & r, detail::field_attributes fa, T & t, const std::string & column_name, std::size_t max_size )  
	{ binder_.bind( &r, fa, t, column_name, max_size );	}

	// static bind to index																														   
	template <typename T> 
	void bind( R & r, detail::field_attributes fa, T & t, int column_idx )                                         
	{ binder_.bind( &r, fa, t, column_idx ); }
	
	template <typename T> 
	void bind( R & r, detail::field_attributes fa, T & t, int column_idx, std::size_t max_size)                    
	{ binder_.bind( &r, fa, t, column_idx, max_size ); }

private:
	search_binder binder_;
	R record_;
};


} // namespace db
} // namespace dti

#endif
