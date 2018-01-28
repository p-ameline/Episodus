//
//  search_binder.h
//  
//  Copyright (c) 2004 Kristian Lippert, Danish Technological Institute, Informatics
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef SEARCH_BINDER_H_KRISTIANLIPPERT_20040729_1154
#define SEARCH_BINDER_H_KRISTIANLIPPERT_20040729_1154

#include <string>

#include <dti/database/common.h>
#include <dti/database/detail/binder.h>

namespace dti{
namespace db{

class database;
template <typename R> class search; // forward declaration of search class

namespace detail{

class search_binder_impl;
} //namespace detail


class search_binder: public binder
{
private: // only the search class can create instances of the search_binder-class and knows its interface
	template <typename R> friend class search;

	search_binder() {}
	search_binder( database & db, const std::string & sql_stmt );

	void get_next( void * record );
	void bind_all_values_to_db();
	void open( const std::string & sqlStmt );
};

//} // namespace detail
} // namespace db
} // namespace dti

 
#endif // SEARCH_BINDER_H_KRISTIANLIPPERT_20040729_1154


