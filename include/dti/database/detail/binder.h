// 
//  binder.h 
// 
//  Copyright (c) 2004 Kristian Lippert, Danish Technological Institute, Informatics
// 
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided 'as is' without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
// 
#ifndef BINDER_H_KRISTIAN_LIPPERT_20041209091127
#define BINDER_H_KRISTIAN_LIPPERT_20041209091127

#include <string>

#include <dti/database/common.h>

namespace dti{
namespace db{


namespace detail{

	class binder_impl;

} //namespace detail

class binder
{
protected: // only the search class can create instances of the search_binder-class and knows its interface

	binder() : pImpl_( 0 ) {}
	~binder();

	binder( const binder & rhs );
	binder& operator= ( const binder & rhs );

	bool operator== ( const binder & rhs ) const;
	bool operator!= ( const binder & rhs ) const;

	template <typename T> void bind( void* record, detail::field_attributes fa, T & t );
	template <typename T> void bind( void* record, detail::field_attributes fa, T & t, std::size_t maxSize );
	template <typename T> void bind( void* record, detail::field_attributes fa, T & t, const std::string & column_name );
	template <typename T> void bind( void* record, detail::field_attributes fa, T & t, const std::string & column_name, std::size_t max_size );
	template <typename T> void bind( void* record, detail::field_attributes fa, T & t, int column_idx );
	template <typename T> void bind( void* record, detail::field_attributes fa, T & t, int column_idx, std::size_t max_size);

protected:
	detail::binder_impl * pImpl_;
};

//} // namespace detail
} // namespace db
} // namespace dti

#endif // BINDER_H_KRISTIAN_LIPPERT_20041209091127

