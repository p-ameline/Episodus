// -*- c++ -*-
/*
Copyright (c) 2005 by the Danish Technological Institute.

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef SOCKSTREAM_H__DTI_
#define SOCKSTREAM_H__DTI_

#include <dti/sockets/sock.h>
#include <dti/stream/streambufImpl.h>
#include <dti/abortable.h>

namespace dti {
	namespace sockets {

		using namespace dti::stream;

		// The basic_tcpstreambuf class is the streambuf that will form the basis
		// of the tcpstream class
		template <typename Trait>
		class basic_tcpstreambuf : public dti::stream::detail::streambuf_impl<Trait, dti::stream::detail::dual_buffer< typename Trait::char_type> > {
		  public:
			// CONSTRUCTORS

			// Construct an uninitialized streambuf
			basic_tcpstreambuf();

			// Construct a streambuf and initialize it with an open tcp_socket
			basic_tcpstreambuf(tcp_socket& s);

			// Abort an I/O operation
			void abort();

			// Abort completed
			void clear_abort();
		};

		// tcpstreambuf is for char I/O
		// wtcpstreambuf is for w_char I/O
		typedef basic_tcpstreambuf< std::char_traits<char> > tcpstreambuf;
		typedef basic_tcpstreambuf< std::char_traits<wchar_t> > wtcpstreambuf;


		// The basic_tcpstream class can be used to perform formatted I/O
		template <typename BS>
		class basic_tcpstream : public BS, public dti::abortable::ios_abortable {
		  public:
			// CONSTRUCTORS

			// Construct an uninitialized streambuf
			basic_tcpstream();

			// Construct a streambuf and initialize it with an open tcp_socket
			basic_tcpstream(tcp_socket& s);

			// Abort an I/O operation
			virtual void abort() { tcpbuf.abort(); }

			// Abort completed
			virtual void clear_abort() { tcpbuf.clear_abort(); }

		  private:
			// The underlying basic_tcpstreambuf that the stream uses
			basic_tcpstreambuf<typename BS::traits_type> tcpbuf;
		};

		// Input stream, output stream, and input/output stream.
		// It is unlikely that itcpstream and otcpstream will ever be used.
		typedef basic_tcpstream< std::istream >  itcpstream;
		typedef basic_tcpstream< std::ostream >  otcpstream;
		typedef basic_tcpstream< std::iostream > tcpstream;

#ifdef _WIN32
		typedef basic_tcpstream< std::wistream >  witcpstream;
		typedef basic_tcpstream< std::wostream >  wotcpstream;
		typedef basic_tcpstream< std::wiostream > wtcpstream;
#endif

	}
}


#endif  /* SOCKSTREAM_H__DTI_ */
