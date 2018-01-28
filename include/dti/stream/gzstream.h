/* gzstream.h -- interface of the gzstream clases
  version 1.0.0, May 11th, 2002
  Author:	Ralf Mattethat

  This library uses the zlib library version 1.1.4 (www.zlib.org)
  
  The classes handel input and output to a gzip stream. The interface is almost 
  identical to the fstream classes in the standard library.
*/

/*
TO DO:
* work out a way to make wchar_t streams work
*/


#ifndef GZSTREAM_H_20020511
#define GZSTREAM_H_20020511

#include <iostream>
#include "streambufImpl.h"
#include "stream_autolib.h"

namespace dti
{

	namespace stream
	{

#ifndef ZCOMPRESS
#define ZCOMPRESS

		enum Level { None=0, SuperFast=1, Fast=2, Normal=6, Maximum=9, Default=-1 };

#endif

		class gzstreambuf : public detail::streambuf_impl<std::char_traits<char>, detail::shared_buffer<char> >
		{
		public:
			typedef std::char_traits<char>	traits_type;
			typedef traits_type::char_type	char_type;
			typedef traits_type::int_type	int_type;
			typedef traits_type::pos_type	pos_type;
			typedef traits_type::off_type	off_type;

			gzstreambuf();
			gzstreambuf( const char* s, std::ios_base::openmode mode, Level l=Default );
			bool is_open() const;
			gzstreambuf* open( const char* s, std::ios_base::openmode mode, Level l=Default );
			gzstreambuf* close();

		private:
			gzstreambuf ( const gzstreambuf& s );
			gzstreambuf& operator= ( const gzstreambuf& s );
		};

		class igzstream : public std::basic_istream<char, std::char_traits<char> >
		{
		public:
			typedef std::char_traits<char>	traits_type;
			typedef traits_type::char_type	char_type;
			typedef traits_type::int_type	int_type;
			typedef traits_type::pos_type	pos_type;
			typedef traits_type::off_type	off_type;

			igzstream() : std::basic_istream<char, std::char_traits<char> >( 0 ) { init( &gzbuf ); }
			explicit igzstream ( const char* s )
				: std::basic_istream<char, std::char_traits<char> >( 0 ), gzbuf( s, std::ios_base::in )
			{ init( &gzbuf ); }

			gzstreambuf* rdbuf() const	{ return const_cast<gzstreambuf*>( &gzbuf ); }
			bool is_open() const		{ return gzbuf.is_open(); }

			void open( const char* s )
			{
				if( gzbuf.open( s, std::ios_base::in ) == 0 )
					setstate( failbit );
			}

			void close()
			{
				if( gzbuf.close() == 0 )
					setstate( failbit );
			}

		private:
			gzstreambuf gzbuf;
		};

		class ogzstream : public std::basic_ostream<char, std::char_traits<char> >
		{
		public:
			typedef std::char_traits<char>	traits_type;
			typedef traits_type::char_type	char_type;
			typedef traits_type::int_type	int_type;
			typedef traits_type::pos_type	pos_type;
			typedef traits_type::off_type	off_type;

			ogzstream() : std::basic_ostream<char, std::char_traits<char> >( 0 ) { init( &gzbuf ); }
			explicit ogzstream ( const char* s, Level c=Default )
				: std::basic_ostream<char, std::char_traits<char> >( 0 ), gzbuf( s, std::ios_base::out )
			{ init( &gzbuf ); }

			gzstreambuf* rdbuf() const	{ return const_cast<gzstreambuf*>( &gzbuf ); }
			bool is_open() const		{ return gzbuf.is_open(); }

			void open( const char* s, Level l=Default )
			{
				if( gzbuf.open( s, std::ios_base::out, l ) == 0 )
					setstate( failbit );
			}

			void close()
			{
				if( gzbuf.close() == 0 )
					setstate( failbit );
			}

		private:
			gzstreambuf gzbuf;
		};

	}	// namespace stream

}	// namespace dti

#endif // GZSTREAM_H_20020511
