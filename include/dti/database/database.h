//
//  database.h
//  
//  Copyright (c) 2004 Kristian Lippert, Danish Technological Institute, Informatics
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef DATABASE_H_KRISTIANLIPPERT_20040330_2224
#define DATABASE_H_KRISTIANLIPPERT_20040330_2224

#include <string>

#include "detail/database_autolib.h"
#include "common.h"

namespace dti{
namespace db{

/////////////////////////////////////////////////////////////////

namespace detail
{

	class database_impl;
	class transaction_impl;
	class search_binder;

}	// namespace detail

class database
{
public:
	database(); // must be followed by a call to open(...) - looks like c'tor and open pair on fstream  
	database( const std::string & database_identification,
		      const std::string & user,
			  const std::string & password,
			  const std::string & extra_init_str = "" );

	~database();

	database( const database & rhs );
	database& operator= ( const database & rhs );

	void open(const std::string & database_identification,
		      const std::string & user,
			  const std::string & password,
			  const std::string & extra_init_str = "" );
	void close();

	// SQL stmt execution
	void excute( const std::string & sqlStm );

private:
	detail::database_impl *pImpl;
	friend class search_binder; // needs access to pImpl
	friend class transaction; // needs access to pImpl
};

class transaction
{
public:
	transaction(database & db);
	~transaction();

	void set_complete(); // current transaction at this level has succeeded
	void set_abort(); // current transaction at this level has failed
private:
	detail::transaction_impl *pImpl;

	// we make copying impossible
	transaction(const transaction & rhs);
	transaction& operator= (const transaction & rhs);
};

} // namespace db
} // namespace dti

/////////////////////////////////////////////////////////////////

#endif
