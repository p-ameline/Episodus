//
//  common.h
//  
//  Copyright (c) 2004 Kristian Lippert, Danish Technological Institute, Informatics
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef COMMON_H_KRISTIANLIPPERT_20040923_1242
#define COMMON_H_KRISTIANLIPPERT_20040923_1242

#include <stdexcept>

namespace dti{
namespace db{

enum field_attribute {to_db=1, from_db=2, unique_field_db=4};

class db_error: public std::runtime_error
{
public:
	explicit db_error( const std::string& message )
		: runtime_error( message )
	{ }
};

namespace detail
{
	typedef int field_attributes;
} // detail 

}
} // namespace dti::db


#endif // COMMON_H_KRISTIANLIPPERT_20040923_1242

