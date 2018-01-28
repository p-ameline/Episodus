/* gzstream.h -- interface of the gzstream clases
  version 1.0.0, May 11th, 2002
  Author:	Ralf Mattethat

  This library uses the libbz2 library version 1.0.2 (www.cacheprof.org)
  
  The classes handel input and output to a bzip2 stream. The interface is almost 
  identical to the fstream classes in the standard library.
*/

/*
TO DO:
* work out a way to make wchar_t streams work
*/


#ifndef BZ2STREAM_H_20020511
#define BZ2STREAM_H_20020511

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

		class bz2streambuf : public detail::streambuf_impl<std::char_traits<char>, detail::shared_buffer<char> >
		{
		public:
			typedef std::char_traits<char>	traits_type;
			typedef traits_type::char_type	char_type;
			typedef traits_type::int_type	int_type;
			typedef traits_type::pos_type	pos_type;
			typedef traits_type::off_type	off_type;

			bz2streambuf();
			bz2streambuf( const char* s, std::ios_base::openmode mode, BZLevel l=bzDefault );
			bool is_open() const;
			bz2streambuf* open( const char* s, std::ios_base::openmode mode, BZLevel l=bzDefault );
			bz2streambuf* close();

		private:
			bz2streambuf ( const bz2streambuf& s );
			bz2streambuf& operator= ( const bz2streambuf& s );
		};

		class ibz2stream : public std::basic_istream<char, std::char_traits<char> >
		{
		public:
			typedef std::char_traits<char>	traits_type;
			typedef traits_type::char_type	char_type;
			typedef traits_type::int_type	int_type;
			typedef traits_type::pos_type	pos_type;
			typedef traits_type::off_type	off_type;

			ibz2stream() : std::basic_istream<char, std::char_traits<char> >( 0 ) { init( &bz2buf ); }
			explicit ibz2stream( const char* s ) 
				: std::basic_istream<char, std::char_traits<char> >( 0 ), bz2buf( s, std::ios_base::in )
			{ init( &bz2buf ); }

			bz2streambuf* rdbuf() const	{ return const_cast<bz2streambuf*>( &bz2buf ); }
			bool is_open() const		{ return bz2buf.is_open(); }

			void open( const char* s )
			{
				if( bz2buf.open( s, std::ios_base::in ) == 0 )
					setstate( failbit );
			}

			void close()
			{
				if( bz2buf.close() == 0 )
					setstate( failbit );
			}

		private:
			bz2streambuf bz2buf;
		};

		class obz2stream : public std::basic_ostream<char, std::char_traits<char> >
		{
		public:
			typedef std::char_traits<char>	traits_type;
			typedef traits_type::char_type	char_type;
			typedef traits_type::int_type	int_type;
			typedef traits_type::pos_type	pos_type;
			typedef traits_type::off_type	off_type;

			obz2stream() : std::basic_ostream<char, std::char_traits<char> >( 0 ) { init( &bz2buf ); }
			explicit obz2stream( const char* s, BZLevel l=bzDefault )
				: std::basic_ostream<char, std::char_traits<char> >( 0 ), bz2buf( s, std::ios_base::out, l )
			{ init( &bz2buf ); }

			bz2streambuf* rdbuf() const	{ return const_cast<bz2streambuf*>( &bz2buf ); }
			bool is_open() const		{ return bz2buf.is_open(); }

			void open( const char* s, BZLevel l=bzDefault )
			{
				if( bz2buf.open( s, std::ios_base::out, l ) == 0 )
					setstate( failbit );
			}

			void close()
			{
				if( bz2buf.close() == 0 )
					setstate( failbit );
			}


		private:
			bz2streambuf bz2buf;
		};

	}	// namespace stream

}	// namespace dti

#endif // BZ2STREAM_H_20020511
