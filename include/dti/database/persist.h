//
//  persist.h
//  
//  Copyright (c) 2004 Kristian Lippert, Danish Technological Institute, Informatics
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef PERSIST_H_KRISTIANLIPPERT_20041024_1354
#define PERSIST_H_KRISTIANLIPPERT_20041024_1354

#include "detail/database_autolib.h"
#include "detail/persist_binder.h"
#include "detail/binder_macros.h" // contains the BEGIN_DATA_BINDING + more macros

namespace dti{
namespace db{

// Requirement for R:
//  Public create_bindings(search<R> & sb) method
//  Default constructor
//  Copy constructor
//  Assignment operator
// 
//  An example could look like:
//
//	class Person
//	{
//	public:
//	…. // Standard and member variable constructors
//
//	// the variables
//	int 	   id_;
//	std::string name_;
//	int         age_;
//
//	BEGIN_DB_BINDING(Person)  
//		DB_BIND_ATT_COLUMN_NAME( from_db|unique_field_db, id_  , "Id"   )
// 		DB_BIND_ATT_COLUMN_NAME( to_db|from_db          , name_, "Name" )
//		DB_BIND_ATT_COLUMN_NAME( to_db|from_db          , age_ , "Age"  )
//	END_DB_BINDING()
//	};
//
//.....
//	int main()
//	{
//		database db("MyLife", "sa", "");
//		std::string tableName = "Persons";
//			
//		persist<Person> p(db, tableName);
//			
//		Person value("Asger, my only son", 2);
//		p.insert(value); 
//			
//		value.name_= “Asger, my first son out of two”; // setting a non-unique field
//		p.update(value);
//
//		Person value2("Sigurd, my second son", 0);
//		p.erase(value); // delete is a keyword in C++, so “erase” used instead
//	}


class database;

template <typename R>
class persist
{
public:
	persist( database & db, const std::string & table_name)
		   : binder_( table_name ),
			 db_(db)
	{
		typename R::template create_bindings<R>( *this );
	}

	void insert(R & r)
	{
		std::string sql_stmt = binder_.get_insert_sql_stmt(&r);
		db_.excute(sql_stmt);
	}

	void erase(R & r)
	{
		std::string sql_stmt = binder_.get_erase_sql_stmt(&r);
		db_.excute(sql_stmt);
	}

	void update(R & r)
	{
		std::string sql_stmt = binder_.get_update_sql_stmt(&r);
		db_.excute(sql_stmt);
	}
	// idx is by call order
	template <typename T> 
	void bind( R & r, detail::field_attributes fa, T & t )                                                         
	{ binder_.bind( &r, fa, t);}

	// idx is by call order, possible size pre-determined																						   
	template <typename T> 
	void bind( R & r, detail::field_attributes fa, T & t, std::size_t max_size ) 
	{ binder_.bind( &r, fa, t, max_size );}

	template <typename T> 
	void bind( R & r, detail::field_attributes fa, T & t, const std::string & column_name )                        
	{ binder_.bind( &r, fa, t, column_name );	}

	template <typename T> 
	void bind( R & r, detail::field_attributes fa, T & t, const std::string & column_name, std::size_t max_size )  
	{ binder_.bind( &r, fa, t, column_name, max_size );	}

	// static binding																															   
	template <typename T> 
	void bind( R & r, detail::field_attributes fa, T & t, int column_idx )    
	{ binder_.bind( &r, fa, t, column_idx ); }

	template <typename T> 
	void bind( R & r, detail::field_attributes fa, T & t, int column_idx, std::size_t max_size)                    
	{ binder_.bind( &r, fa, t, column_idx, max_size ); }

private:
	persist_binder        binder_;
	database    &         db_;

private:
	// makes it non-copyable
	persist(const persist & rhs);
	persist& operator= (const persist & rhs);
};

} // namespace db
} // namespace dti

#endif // PERSIST_H_KRISTIANLIPPERT_20041024_1354
