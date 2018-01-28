//  slist.h -- implementation af a STL complatible single chained list.
//  
//  Copyright (c) 2004 Ralf Mattethat, Danish Technological Institute, Informatics
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  Please send questions, comments, complaints, performance data, etc to  
//  ralf.mattethat@teknologisk.dk

#ifndef SLIST_H_RMA28012003
#define SLIST_H_RMA28012003

#include <cstddef>
#include <iterator>
#include <utility>
#include <functional>
#include <memory>

/*	Requirements for element type
	* must be copy-constructible
	* destructor must not throw exception

	Methods marked with note A only throws an exception if operator== or operator< 
	or the used predicate throws an exception

	iterators are only invalidated, if element pointed to by iterator is deleted
	same goes for element references


	Implementation:
	The interface is designed where possible to be equal to std::list. If an 
	operation can't be implemented effective it is omitted.

	Elements are kept in a single linked list of nodes. Each node containing one 
	element and a 'next' pointer. Manipulations on the nodes are normally through 
	pointers to the 'next' pointer of the previous node. This allows for fast 
	updating of 'next' pointers, and the pointer to the first element is not a 
	special case.


	Alternative implementations:
	1.	Eliminate size_ member
		Consequence: internal_splice() will execute in constant time, but size() 
		in linear time

	2.	Make iterators point to previous element in chain
		Consequence: previous_node() will execute in constant time, but iterators 
		will be invalidated by manipulations on the container
*/


namespace dti
{

	template<typename T, typename Allocator = std::allocator<T> >
	class slist
	{

		struct node;

		typedef typename Allocator::template rebind<node>::other node_allocator_type;
		typedef typename node_allocator_type::pointer	node_pointer;


		struct node
		{
			node_pointer	next;
			T				data;

			node( const T& e, node_pointer n=0 )
				: next( n ), data( e ) { }
		};

	public:
		// container
		// container
		typedef typename Allocator::reference		reference;
		typedef typename Allocator::const_reference	const_reference;
		typedef typename Allocator::size_type		size_type;
		typedef typename Allocator::difference_type	difference_type;
		typedef T									value_type;

		typedef Allocator							allocator_type;
		typedef typename Allocator::pointer			pointer;
		typedef typename Allocator::const_pointer	const_pointer;


		// container
		class iterator_base
		{
		protected:
			explicit iterator_base( node_pointer p ) : p_( p ) { }
			iterator_base() : p_( 0 ) { }

		public:
			bool operator== ( const iterator_base& iter ) const	{ return p_ == iter.p_; }
			bool operator!= ( const iterator_base& iter ) const	{ return p_ != iter.p_; }

		protected:
			node_pointer p_;
		};

		class const_iterator;

		class iterator : public std::iterator<std::forward_iterator_tag, value_type, difference_type, pointer, reference>,
			             public iterator_base
		{
			friend class slist<T,Allocator>;
			friend class const_iterator;

			explicit iterator( node_pointer p ) : iterator_base( p ) { }

		public:
			typedef std::forward_iterator_tag			iterator_category;
			typedef T									value_type;
			typedef typename Allocator::reference		reference;
			typedef typename Allocator::pointer			pointer;
			typedef typename Allocator::difference_type	difference_type;
			
			iterator() { }

			reference	operator* () const		{ return this->p_->data; }
			pointer		operator-> () const		{ return &( **this ); }
		
			iterator&	operator++ ()		{ this->p_ = this->p_->next; return *this; }
			iterator	operator++ ( int )	{ iterator tmp = *this; this->p_ = this->p_->next; return tmp; }
		};

		class const_iterator : public std::iterator<std::forward_iterator_tag, value_type, difference_type, const_pointer, const_reference>,
							   public iterator_base
		{
			friend class slist<T,Allocator>;

			explicit const_iterator( node_pointer p ) : iterator_base( p ) { }

		public:
			typedef std::forward_iterator_tag			iterator_category;
			typedef T									value_type;
			typedef typename Allocator::const_reference	reference;
			typedef typename Allocator::const_pointer	pointer;
			typedef typename Allocator::difference_type	difference_type;

			const_iterator() { }
			const_iterator( iterator it ) : iterator_base( it.p_ ) { }

			reference	operator* () const		{ return this->p_->data; }
			pointer		operator-> () const		{ return &( **this ); }
		
			const_iterator&	operator++ ()		{ this->p_ = this->p_->next; return *this; }
			const_iterator	operator++ ( int )	{ const_iterator tmp = *this; this->p_ = this->p_->next; return tmp; }
		};


		/////////////////////////////////////////////////////////////////
		// construct/copy/destroy:
			
		// container	| complexity : constant		| exception : nothrow
		explicit slist( const allocator_type& alloc = allocator_type() )
			: head_( 0 ), size_( 0 ), allocator_( alloc ), node_allocator_( alloc )
		{ }

		// sequence		| complexity : linear		| exception : 
		explicit slist( size_type n, const value_type& value = value_type(), const allocator_type& alloc = allocator_type() )
			: head_( 0 ), size_( 0 ), allocator_( alloc ), node_allocator_( alloc )
		{
			try
			{
				insert( begin(), n, value );
			}
			catch( ... )
			{
				clear();
				throw;
			}
		}

		// sequence		| complexity : linear		| exception : 
		template <typename InputIterator>
		slist( InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type() ) 
			: head_( 0 ), size_( 0 ), allocator_( alloc ), node_allocator_( alloc )
		{
			try
			{
				insert( begin(), first, last );
			}
			catch( ... )
			{
				clear();
				throw;
			}
		}

		// container	| complexity : linear		| exception : 
		slist( const slist& x )
			: head_( 0 ), size_( 0 ), allocator_( x.allocator_ ), node_allocator_( x.node_allocator_ )
		{
			try
			{
				insert( begin(), x.begin(), x.end() );
			}
			catch( ... )
			{
				clear();
				throw;
			}
		}

		// container	| complexity : linear		| exception : nothrow
		~slist()	{ clear(); }

		// container	| complexity : linear		| exception : strong
		slist& operator= ( const slist& rhs )
		{
			slist<T> temp( rhs );
			swap( temp );
			return *this;
		}

		//				| complexity : linear		| exception : strong
		template <typename InputIterator>
		void assign( InputIterator first, InputIterator last )
		{
			slist temp( first, last );
			swap( temp );
		}

		//				| complexity : linear		| exception : strong
		void assign( size_type n, const value_type& t )
		{
			slist temp( n, t );
			swap( temp );
		}

		//						| complexity : constant		| exception : nothrow
		allocator_type get_allocator() const { return allocator_; }


		/////////////////////////////////////////////////////////////////
		// iterators:

		// container	| complexity : constant		| exception : nothrow
		iterator		begin()			{ return iterator( head_ ); }
		const_iterator	begin()	const	{ return const_iterator( head_ ); }
		iterator		end()			{ return iterator( 0 ); }
		const_iterator	end()	const	{ return const_iterator( 0 ); }


		/////////////////////////////////////////////////////////////////
		// capacity:
		
		// container	| complexity : constant		| exception : nothrow
		bool empty() const			{ return head_ == 0; }

		// container	| complexity : constant		| exception : nothrow
		size_type size() const		{ return size_; }
		
		// container	| complexity : constant		| exception : nothrow
		size_type max_size() const	{ return allocator_.max_size(); }
		
		//				| complexity : linear		| exception : weak
		void resize( size_type n, const value_type& c = value_type() )
		{
			size_type s = size();
			if( n > s )
				insert( end(), n-s, c );
			else if( n < s )
			{
				iterator it = begin();
				std::advance( it, n );
				erase( it, end() );
			}
		}


		/////////////////////////////////////////////////////////////////
		// element access:

		// optional sequence operation		| complexity : constant		| exception : nothrow
		reference		front()			{ return head_->data; }
		const_reference front()	const	{ return head_->data; }


		/////////////////////////////////////////////////////////////////
		// modifiers:

		// optional sequence operation		| complexity : constant		| exception : strong
		void push_front( const value_type& x )	{ internal_insert( &head_, x ); }

		// optional sequence operation		| complexity : constant		| exception : nothrow
		void pop_front()						{ internal_erase( &head_ ); }

		// sequence		| complexity : linear		| exception : strong
		iterator insert( iterator position, const value_type& x )
		{
			node_pointer* prev = previous_node( position );
			internal_insert( prev, x );
			return iterator( *prev );
		}

		// sequence		| complexity : linear		| exception : weak
		void insert( iterator position, size_type n, const value_type& x )
		{
			node_pointer* prev = previous_node( position );
			for( ; n > 0; --n )
			{
				internal_insert( prev, x );
				prev = &( (*prev)->next );
			}
		}

		// sequence		| complexity : linear		| exception : weak
		template <typename InputIterator>
		void insert( iterator position, InputIterator first, InputIterator last )
		{
			node_pointer* prev = previous_node( position );
			for( InputIterator it = first; it != last; ++it )
			{
				internal_insert( prev, *it );
				prev = &( (*prev)->next );
			}
		}

		// sequence		| complexity : linear		| exception : nothrow
		iterator erase( iterator position )
		{
			node_pointer* prev = previous_node( position );
			internal_erase( prev );
			return iterator( *prev );
		}

		// sequence		| complexity : linear		| exception : nothrow
		iterator erase( iterator first, iterator last )
		{
			node_pointer* prev = previous_node( first );
			for( node* p = first.p_; p != last.p_; p = *prev )
			{
				internal_erase( prev );
			}
			return last;
		}

		// container	| complexity : constant		| exception : nothrow
		void swap( slist<T,Allocator>& x )
		{
			std::swap( head_, x.head_ );
			std::swap( size_, x.size_ );
		}

		// sequence		| complexity : linear		| exception : nothrow
		void clear()	{ erase( begin(), end() ); }


		/////////////////////////////////////////////////////////////////
		// slist operations:
		
		//				| complexity : linear		| exception : nothrow
		void splice( iterator position, slist<T, Allocator>& x )
		{
			if( x.head_ != 0 )
			{
				node_pointer* prev = previous_node( position );

				node_pointer* last = &x.head_;
				while( *last != 0 )
					last = &( (*last)->next );

				*prev = x.head_;
				*last = position.p_;

				size_ += x.size_;

				x.head_ = 0;
				x.size_ = 0;
			}
		}

		//				| complexity : linear		| exception : nothrow
		void splice( iterator position, slist<T, Allocator>& x, iterator i )
		{
			node_pointer* prev  = previous_node( position );
			node_pointer* first = x.previous_node( i );

			internal_splice( prev, x, first );
		}

		//				| complexity : linear		| exception : nothrow
		void splice( iterator position, slist<T, Allocator>& x, iterator first, iterator last )
		{
			node_pointer* prev = previous_node( position );
			node_pointer* f    = x.previous_node( first );

			if( *prev != *f )
			{
				size_type n = 0;
				node_pointer* l = f;
				while( *l != last.p_ )
				{
					l = &( (*l)->next );
					++n;
				}

				size_ += n;
				x.size_ -= n;

				node* q = *prev;

				*prev = *f;
				*f = *l;
				*l = q;
			}
		}
		
		//				| complexity : linear		| exception : weak, note A
		void remove( const value_type& value )
		{
			remove_if( std::bind2nd( std::equal_to<T>(), value ) );
		}
		
		//				| complexity : linear		| exception : weak, note A
		template<typename Predicate>
		void remove_if( Predicate pred )
		{
			node_pointer* prev = &head_;
			while( *prev != 0 )
			{
				if( pred( (*prev)->data ) )
					internal_erase( prev );
				else
					prev = &( (*prev)->next );
			}
		}
		
		//				| complexity : linear		| exception : weak, note A
		void unique()
		{
			unique( std::equal_to<T>() );
		}
		
		//				| complexity : linear		| exception : weak, note A
		template<typename BinaryPredicate>
		void unique( BinaryPredicate binary_pred )
		{
			if( head_ != 0 )
			{
				node_pointer p = head_;
				while( p->next != 0 )
				{
					if( binary_pred( p->data, p->next->data  ) )
						internal_erase( &(p->next) );
					else
						p = p->next;
				}
			}
		}
		
		//				| complexity : linear		| exception : weak, note A
		void merge( slist<T>& x )
		{
			merge( x, std::less<T>() );
		}
		
		//				| complexity : linear		| exception : weak, note A
		template<typename Compare>
		void merge( slist<T>& x, Compare comp )
		{
			node_pointer* p = &head_;
			node_pointer* q = &x.head_;

			while( *p != 0 && *q != 0 )
			{
				if( comp( (*q)->data, (*p)->data ) )
					internal_splice( p, x, q );

				p = &( (*p)->next );
			}
			if( *q != 0 )
			{
				*p = *q;
				size_ += x.size_;
				x.head_ = 0;
				x.size_ = 0;
			}
		}
		
		//				| complexity : NlogN		| exception : weak, note A
		void sort()
		{
			sort( std::less<T>() );
		}
		
		//				| complexity : NlogN		| exception : weak, note A
		template<typename Compare>
		void sort( Compare comp )
		{
			if( head_ && head_->next )
			{
				const int max_elements = 32;
				slist carry;
				slist counter[ max_elements + 1 ];	// at most 2^max_elements-1 elements supported efficiently
				int fill = 0;

				try
				{
					while( !empty() )
					{
						carry.internal_splice( &carry.head_, *this, &head_ );

						int i = 0;
						while( i < fill && !counter[ i ].empty() )
						{
							counter[ i ].merge( carry, comp );
							carry.swap( counter[ i ] );
							++i;
						}
						if( i != max_elements )
						{
							carry.swap( counter[ i ] );
							if( i == fill )
								++fill;
						}
						else
						{
							counter[ i-1 ].merge( carry, comp ) ;
						}
					}
					
					for( int i = 1; i < fill; ++i )
						counter[ i ].merge( counter[ i-1 ], comp ) ;
					swap( counter[ fill-1 ] );
				}
				catch( ... )
				{
					// move all element back in container 
					splice( begin(), carry );
					for( int i = 0; i < fill; ++i )
						splice( begin(), counter[ i ] );

					throw;
				}
			}
		}
		
		//				| complexity : linear		| exception : nothrow
		void reverse()
		{
			node_pointer prev = 0;
			for( node_pointer p = head_; p != 0 ; )
			{
				node_pointer n = p->next;
				p->next = prev;
				prev = p;
				p = n;
			}
			head_ = prev;
		}

	private:
		//				| complexity : constant		| exception : strong
		// return a newly allocate node holding the value x
		node_pointer construct( const value_type& x, node_pointer next )
		{
			node_pointer n = node_allocator_.allocate( 1, 0 );
			try
			{
				new ( n ) node( x, next );
			}
			catch( ... )
			{
				node_allocator_.deallocate( n, 1 );
				throw;
			}
			return n;
		}

		//				| complexity : constant		| exception : nothrow
		// deallocate node pointed to by n
		void destroy( node* n )
		{
			node_allocator_.destroy( n );
			node_allocator_.deallocate( n, 1 );
		}

		//				| complexity : linear		| exception : nothrow
		// return reference to pointer pointing to node
		node_pointer* previous_node( iterator it )
		{
			node_pointer* prev = &head_;
			while( *prev != it.p_ )
			{
				prev = &( (*prev)->next );
			}

			return prev;
		}

		//				| complexity : constant		| exception : strong
		void internal_insert( node_pointer* prev, const value_type& t  )
		{
			*prev = construct( t, *prev );
			++size_;
		}

		//				| complexity : constant		| exception : nothrow
		void internal_erase( node_pointer* prev )
		{
			node_pointer p = *prev;
			*prev = p->next;
			destroy( p );
			--size_;
		}

		//				| complexity : constant		| exception : nothrow
		void internal_splice( node_pointer* prev, slist<T>& x, node_pointer* first )
		{
			if( &( (*first)->next ) != prev )
			{
				node_pointer q = *first;

				*first = q->next;
				q->next = *prev;
				*prev = q;

				++size_;
				--x.size_;
			}
		}

	private:
		node_pointer		head_;
		size_type			size_;

		allocator_type		allocator_;
		node_allocator_type	node_allocator_;
	};

	// container	| complexity : linear		| exception : strong, note A
	template <typename T>
	inline bool operator == ( const slist<T>& lhs, const slist<T>& rhs )
	{
		// return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin() );

		typedef typename slist<T>::const_iterator const_iterator;

		const_iterator end1 = lhs.end();
		const_iterator end2 = rhs.end();

		const_iterator i1 = lhs.begin();
		const_iterator i2 = rhs.begin();
		while( i1 !=  end1 && i2 != end2 && *i1 == *i2 )
		{
			++i1;
			++i2;
		}
		return i1 == end1 && i2 == end2;
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Allocator>
	inline bool operator < ( const slist<T, Allocator>& lhs, const slist<T, Allocator>& rhs )
	{
		return std::lexicographical_compare( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Allocator>
	inline bool operator != ( const slist<T, Allocator>& lhs, const slist<T, Allocator>& rhs )
	{
		return !( lhs == rhs );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Allocator>
	inline bool operator > ( const slist<T, Allocator>& lhs, const slist<T, Allocator>& rhs )
	{
		return rhs < lhs;
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Allocator>
	inline bool operator <= ( const slist<T, Allocator>& lhs, const slist<T, Allocator>& rhs )
	{
		return !( rhs < lhs );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Allocator>
	inline bool operator >= ( const slist<T, Allocator>& lhs, const slist<T, Allocator>& rhs )
	{
		return !( lhs < rhs );
	}

	// container	| complexity : constant		| exception : nothrow
	template <typename T, typename Allocator>
	inline void swap( slist<T, Allocator>& x, slist<T, Allocator>& y )
	{
		x.swap( y );
	}

}	// namespace dti

#endif // SLIST_H_RMA28012003
