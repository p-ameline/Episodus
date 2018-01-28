//
//  persist_binder.h
//  
//  Copyright (c) 2004 Kristian Lippert, Danish Technological Institute, Informatics
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef PERSIST_BINDER_H_KRISTIANLIPPERT_20040729_1154
#define PERSIST_BINDER_H_KRISTIANLIPPERT_20040729_1154

#include <string>

#include <dti/database/common.h>
#include <dti/database/detail/binder.h>

namespace dti{
namespace db{

template <typename R> class persist; // forward declaration of persist class

namespace detail{

class persist_binder_impl;
} //namespace detail




///////////////////////////////////////////////////////////////////////////////////////////////
// Persistence
///////////////////////////////////////////////////////////////////////////////////////////////
class persist_binder: public binder
{
private: // only the search class can create instances of the search_binder-class and knows its interface
	template <typename R> friend class persist;

	persist_binder() {} //: pImpl_( 0 ) {}
	persist_binder( const std::string & table_name ); // for persist

	// persistence
	std::string get_insert_sql_stmt( void* record );
	std::string get_erase_sql_stmt ( void* record );
	std::string get_update_sql_stmt( void* record );
};

//} // namespace detail
} // namespace db
} // namespace dti

 #endif  // PERSIST_BINDER_H_KRISTIANLIPPERT_20040729_1154


