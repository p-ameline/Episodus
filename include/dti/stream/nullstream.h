/* nullstream.h -- interface of the nullstream clases
  version 1.0.0, September 11th, 2002
  Author:	Ralf Mattethat

  nullstreambuffer discards the output given to it
*/


#ifndef NULLSTREAM_H_20020911
#define NULLSTREAM_H_20020911

#include <iostream>

namespace dti
{

	namespace stream
	{

		template < typename E, typename T = std::char_traits<E> >
		class basic_nullstreambuf : public std::basic_streambuf<E, T>
		{
		public:
			typedef T								traits_type;
			typedef typename traits_type::char_type	char_type;
			typedef typename traits_type::int_type	int_type;
			typedef typename traits_type::pos_type	pos_type;
			typedef typename traits_type::off_type	off_type;

			basic_nullstreambuf () { }
			int_type overflow ( int_type c = traits_type::eof() )	{ return traits_type::not_eof( c ); }

		private:
			basic_nullstreambuf ( const basic_nullstreambuf& s );
			basic_nullstreambuf& operator= ( const basic_nullstreambuf& s );
		};

		template < typename E, typename T = std::char_traits<E> >
		class basic_onullstream : public std::basic_ostream<E, T >
		{
		public:
			typedef T								traits_type;
			typedef typename traits_type::char_type	char_type;
			typedef typename traits_type::int_type	int_type;
			typedef typename traits_type::pos_type	pos_type;
			typedef typename traits_type::off_type	off_type;

			basic_onullstream () : std::basic_ostream<E, T >( 0 ) { init( &nullbuf ); }
			basic_nullstreambuf<E, T>* rdbuf() const	{ return const_cast<basic_nullstreambuf<E, T>*>( &nullbuf ); }

		private:
			basic_nullstreambuf<E, T> nullbuf;
		};

		typedef basic_nullstreambuf<char>		nullstreambuf;
		typedef basic_onullstream<char>			onullstream;

		typedef basic_nullstreambuf<wchar_t>	wnullstreambuf;
		typedef basic_onullstream<wchar_t>		wonullstream;

	}	// namespace stream

}	// namespace dti

#endif // NULLSTREAM_H_20020911
