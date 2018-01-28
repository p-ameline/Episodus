/* bzstream.h -- interface of the bzstream clases
  version 1.0.0, May 11th, 2002
  Author:	Ralf Mattethat

  This library uses the libbz2 library version 1.0.2 (www.cacheprof.org)
  
  The classes adapts a stream into an other stream compressing/uncompressing the data.
*/

/*
TO DO:
* work out a way to make wchar_t streams work
*/

#ifndef BZSTREAM_H_20020511
#define BZSTREAM_H_20020511

#include <iostream>
#include "streambufImpl.h"
#include "stream_autolib.h"

namespace dti
{

	namespace stream
	{

#ifndef BZCOMPRESS
#define BZCOMPRESS

		enum BZLevel { bzSuperFast=1, bzFast=2, bzNormal=6, bzMaximum=9, bzDefault=-1 };

#endif

		class bzstreambuf : public detail::streambuf_impl<std::char_traits<char>, detail::shared_buffer<char> >
		{
		public:
			typedef std::char_traits<char>	traits_type;
			typedef traits_type::char_type	char_type;
			typedef traits_type::int_type	int_type;
			typedef traits_type::pos_type	pos_type;
			typedef traits_type::off_type	off_type;

			explicit bzstreambuf( std::basic_streambuf<char, std::char_traits<char> >& s , BZLevel l=bzDefault );

		private:
			bzstreambuf ( const bzstreambuf& s );
			bzstreambuf& operator= ( const bzstreambuf& s );
		};
			
		class ibzstream : public std::basic_istream<char, std::char_traits<char> >
		{
		public:
			typedef std::char_traits<char>	traits_type;
			typedef traits_type::char_type	char_type;
			typedef traits_type::int_type	int_type;
			typedef traits_type::pos_type	pos_type;
			typedef traits_type::off_type	off_type;

			explicit ibzstream( std::istream& is ) : std::basic_istream<char, std::char_traits<char> >( 0 ), bzbuf( *is.rdbuf() )
			{
				try
				{
					init( &bzbuf );
					clear( is.rdstate() );
					copyfmt( is );
				}
				catch( ... )
				{
					setstate( badbit );
				}
			}

			bzstreambuf* rdbuf() const	{ return const_cast<bzstreambuf*>( &bzbuf ); }

		private:
			bzstreambuf bzbuf;
		};

		class obzstream : public std::basic_ostream<char, std::char_traits<char> >
		{
		public:
			typedef std::char_traits<char>	traits_type;
			typedef traits_type::char_type	char_type;
			typedef traits_type::int_type	int_type;
			typedef traits_type::pos_type	pos_type;
			typedef traits_type::off_type	off_type;

			explicit obzstream( std::ostream& os, BZLevel l=bzDefault ) : std::basic_ostream<char, std::char_traits<char> >( 0 ), bzbuf( *os.rdbuf(), l ) 
			{
				try
				{
					init( &bzbuf );
					clear( os.rdstate() );
					copyfmt( os );
				}
				catch( ... )
				{
					setstate( badbit );
				}
			}

			bzstreambuf* rdbuf() const	{ return const_cast<bzstreambuf*>( &bzbuf ); }

		private:
			bzstreambuf bzbuf;
		};

	}	// namespace stream

}	// namespace dti

#endif // BZSTREAM_H_20020511
