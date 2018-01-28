//  ordered_map.h -- implementation af a STL complatible map based on an ordered sequence.
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

#ifndef ORDERED_MAP_H_RMA07112003
#define ORDERED_MAP_H_RMA07112003

#include "detail/ordered_sequence.h"
#include <functional>
#include <vector>

/*	Requirements for element type
	* must be copy-constructible
	* must be copy-assignable
	* destructor must not throw exception

	Methods marked with note A only throws an exception if operator== or operator< 
	or the used predicate throws an exception

	Iterators are invalidated by calls to insert() or erase() methods
	same goes for element references

	The weak exception guarantee of insert()/erase() only applies if the copy constructor 
	or assignment operator can throw an exception. Otherwise insert() of single elements 
	have a strong guarantee and erase() has a guarantee of nothrow.


	Implementation:
	The elements are kept in a sequence container with random access iterators. 
	At any time the element are sorted.
*/

namespace dti
{

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ordered_map
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename K, typename T, typename Compare = std::less<K>, typename Allocator = std::allocator<std::pair<const K,T> > >
	class ordered_map			// unique keys
	{
		// function object to compare keys and values
		struct KeyCompare
		{
			KeyCompare( const Compare& comp ) : comp_( comp ) { }

			bool operator()( const K& x , const K& y )							 const	{ return comp_( x, y ); }
			bool operator()( const K& x , const std::pair<K,T>& y )				 const	{ return comp_( x, y.first ); }
			bool operator()( const std::pair<K,T>& x , const K& y )				 const	{ return comp_( x.first, y ); }
			bool operator()( const std::pair<K,T>& x , const std::pair<K,T>& y ) const	{ return comp_( x.first, y.first ); }

			operator Compare () const { return comp_; }

		private:
			Compare comp_;
		};

		typedef typename Allocator::template rebind<std::pair<K,T> >::other node_allocator;

	public:
		// container
		typedef K						key_type;
		typedef std::pair<const K,T>	value_type;

		// map
		typedef T						mapped_type;
		typedef Compare					key_compare;

		// map
		class value_compare : public std::binary_function<value_type, value_type, bool>
		{
			friend class ordered_map<K,T,Compare,Allocator>;

		protected:
			key_compare comp;
			value_compare( key_compare c ) : comp( c ) { }

		public:
			bool operator()( const value_type& x, const value_type& y ) const
			{
				return comp( x.first, y.first );
			}
		};

	private:
		// extract key part of value
		struct KeyPart : public std::unary_function<value_type,key_type>
		{
			const key_type& operator()( const value_type& x ) const { return x.first; }
		};
	    
		typedef detail::ordered_sequence<K, value_type, KeyPart, KeyCompare, std::vector<std::pair<K,T>, node_allocator> > Rep_type;

	public:

		// container
		typedef typename Allocator::reference				reference;
		typedef typename Allocator::const_reference			const_reference;
		typedef typename Allocator::size_type				size_type;
		typedef typename Allocator::difference_type			difference_type;

		typedef Allocator									allocator_type;
		typedef typename Allocator::pointer					pointer;
		typedef typename Allocator::const_pointer			const_pointer;
		
		// container
		class const_iterator;

		class iterator : public std::iterator< typename std::iterator_traits<typename Rep_type::iterator>::iterator_category , std::pair<const K,T>, typename std::iterator_traits<typename Rep_type::iterator>::difference_type >
		{
			friend class ordered_map<K,T,Compare,Allocator>;
			friend class const_iterator;

			iterator( typename Rep_type::iterator iter ) : it_( iter ) { }

		public:
			typedef typename std::iterator_traits<typename Rep_type::const_iterator>::iterator_category	iterator_category;
			typedef std::pair<const K,T>																value_type;
			typedef value_type& 																		reference;
			typedef value_type*																			pointer;
			typedef typename std::iterator_traits<typename Rep_type::const_iterator>::difference_type	difference_type;
			iterator() { }

			reference	operator* () const							{ return *reinterpret_cast<pointer>( &( *it_ ) ); }
			pointer		operator-> () const 						{ return reinterpret_cast<pointer>( &( *it_ ) ); }
			reference	operator[] ( difference_type index ) const	{ return *( it_ + index ); }
			
			iterator& operator++ ()			{ ++it_; return *this; }
			iterator& operator-- ()			{ --it_; return *this; }
			iterator  operator++ ( int )	{ iterator tmp( *this ); ++*this; return tmp; }
			iterator  operator-- ( int )	{ iterator tmp( *this ); --*this; return tmp; }
			
			iterator& operator+= ( difference_type n )		{ it_ += n; return *this; }
			iterator& operator-= ( difference_type n )		{ it_ -= n; return *this; }
			iterator  operator+  ( difference_type n ) const	{ iterator tmp( *this ); return tmp += n; }
			iterator  operator-  ( difference_type n ) const	{ iterator tmp( *this ); return tmp -= n; }

			difference_type operator- ( const iterator& iter ) const	{ return it_ - iter.it_; }

			bool operator== ( const iterator& iter ) const	{ return it_ == iter.it_; }
			bool operator!= ( const iterator& iter ) const	{ return it_ != iter.it_; }
			bool operator<	( const iterator& iter ) const	{ return it_ <  iter.it_; }
			bool operator<= ( const iterator& iter ) const	{ return it_ <= iter.it_; }
			bool operator>	( const iterator& iter ) const	{ return it_ >  iter.it_; }
			bool operator>= ( const iterator& iter ) const	{ return it_ >= iter.it_; }

		private:
			typename Rep_type::iterator it_;
		};

		class const_iterator : public std::iterator< typename std::iterator_traits<typename Rep_type::const_iterator>::iterator_category , const std::pair<const K,T>, typename std::iterator_traits<typename Rep_type::const_iterator>::difference_type >
		{
			friend class ordered_map<K,T,Compare,Allocator>;

			const_iterator( typename Rep_type::const_iterator iter ) : it_( iter ) { }

		public:
			typedef typename std::iterator_traits<typename Rep_type::const_iterator>::iterator_category	iterator_category;
			typedef const std::pair<const K,T>															value_type;
			typedef value_type& 																		reference;
			typedef value_type*																			pointer;
			typedef typename std::iterator_traits<typename Rep_type::const_iterator>::difference_type	difference_type;

			const_iterator() { }

			const_iterator( iterator it ) : it_( it.it_ ) { }

			reference	operator* () const							{ return *reinterpret_cast<pointer>( &( *it_ ) ); }
			pointer		operator-> () const 						{ return reinterpret_cast<pointer>( &( *it_ ) ); }
			reference	operator[] ( difference_type index ) const	{ return *( it_ + index ); }
			
			const_iterator& operator++ ()			{ ++it_; return *this; }
			const_iterator& operator-- ()			{ --it_; return *this; }
			const_iterator  operator++ ( int )	{ const_iterator tmp( *this ); ++*this; return tmp; }
			const_iterator  operator-- ( int )	{ const_iterator tmp( *this ); --*this; return tmp; }
			
			const_iterator& operator+= ( difference_type n )		{ it_ += n; return *this; }
			const_iterator& operator-= ( difference_type n )		{ it_ -= n; return *this; }
			const_iterator  operator+  ( difference_type n ) const	{ const_iterator tmp( *this ); return tmp += n; }
			const_iterator  operator-  ( difference_type n ) const	{ const_iterator tmp( *this ); return tmp -= n; }

			difference_type operator- ( const const_iterator& iter ) const	{ return it_ - iter.it_; }

			bool operator== ( const const_iterator& iter ) const	{ return it_ == iter.it_; }
			bool operator!= ( const const_iterator& iter ) const	{ return it_ != iter.it_; }
			bool operator<	( const const_iterator& iter ) const	{ return it_ <  iter.it_; }
			bool operator<= ( const const_iterator& iter ) const	{ return it_ <= iter.it_; }
			bool operator>	( const const_iterator& iter ) const	{ return it_ >  iter.it_; }
			bool operator>= ( const const_iterator& iter ) const	{ return it_ >= iter.it_; }

			bool operator== ( const iterator& iter ) const	{ return it_ == iter.it_; }
			bool operator!= ( const iterator& iter ) const	{ return it_ != iter.it_; }
			bool operator<	( const iterator& iter ) const	{ return it_ <  iter.it_; }
			bool operator<= ( const iterator& iter ) const	{ return it_ <= iter.it_; }
			bool operator>	( const iterator& iter ) const	{ return it_ >  iter.it_; }
			bool operator>= ( const iterator& iter ) const	{ return it_ >= iter.it_; }

		private:
			typename Rep_type::const_iterator it_;
		};

		// reversible container
		typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef std::reverse_iterator<iterator>			reverse_iterator;

		/////////////////////////////////////////////////////////////////
		// construct/copy/destroy:
			
		// container	| complexity : constant		| exception : 
		explicit ordered_map( const key_compare& comp = Compare(), const allocator_type& a = Allocator() )
			: rep_( KeyCompare( comp ), a ), allocator_( a )
		{ }

		// associative sequence		| complexity : quadratic/linear	| exception : 
		template <typename InputIterator>
		ordered_map( InputIterator first, InputIterator last, const key_compare& comp = Compare(), const allocator_type& a = Allocator() )
			: rep_( KeyCompare( comp ), a ), allocator_( a )
		{ insert( first, last ); }

		// container	| complexity : linear		| exception : 
		ordered_map( const ordered_map& x )
			: rep_( x.rep_ ), allocator_( x.allocator_ )
		{ }

		// container	| complexity : linear		| exception : nothrow
		~ordered_map()	{ }

		// container	| complexity : linear		| exception : strong
		ordered_map& operator= ( const ordered_map& rhs )
		{
			rep_ = rhs.rep_;
			return *this;
		}

		//				| complexity : constant		| exception : nothrow
		allocator_type get_allocator() const { return allocator_; }


		/////////////////////////////////////////////////////////////////
		// iterators:
		
		// container	| complexity : constant		| exception : nothrow
		iterator		begin()			{ return iterator( rep_.begin() ); }
		const_iterator	begin()	const	{ return const_iterator( rep_.begin() ); }
		iterator		end()			{ return iterator( rep_.end() ); }
		const_iterator	end()	const	{ return const_iterator( rep_.end() ); }

		// reversible container	| complexity : constant		| exception : nothrow
		reverse_iterator		rbegin()		{ return reverse_iterator( end() ); }
		const_reverse_iterator	rbegin() const	{ return const_reverse_iterator( end() ); }
		reverse_iterator		rend()			{ return reverse_iterator( begin() ); }
		const_reverse_iterator	rend()	 const	{ return const_reverse_iterator( begin() ); }

		
		/////////////////////////////////////////////////////////////////
		// capacity:
		
		// container	| complexity : constant		| exception : nothrow
		bool empty()		 const	{ return rep_.empty(); }
		size_type size()	 const	{ return rep_.size(); }
		size_type max_size() const	{ return rep_.max_size(); }

		
		/////////////////////////////////////////////////////////////////
		// element access:
		
		//				| complexity : logarithmic/linear	| exception : weak
		mapped_type& operator[] ( const key_type& x )
		{
			return ( *( insert( std::make_pair(x, mapped_type() ) ).first)).second;
		}


		/////////////////////////////////////////////////////////////////
		// modifiers:
		
		// associative sequence		| complexity : linear			| exception : weak
		std::pair<iterator, bool> insert( const value_type& x )
		{
			std::pair<typename Rep_type::iterator, bool> res = rep_.insert_unique( x );
			return std::pair<iterator, bool>( iterator( res.first ), res.second );
		}

		// associative sequence		| complexity : linear			| exception : weak
		iterator insert( iterator position, const value_type& x )	{ return iterator( rep_.insert_unique( position.it_, x ) ); }

		// associative sequence		| complexity : quadratic/linear	| exception : weak
		template <typename InputIterator>
		void insert( InputIterator first, InputIterator last )		{ rep_.insert_unique( first, last ); }
	    
		// associative sequence		| complexity : linear			| exception : weak
		size_type erase( const key_type& x )						{ return rep_.erase( x ); }

		// associative sequence		| complexity : linear			| exception : weak
		void erase( iterator position )								{ rep_.erase( position.it_ ); }
		void erase( iterator first, iterator last )					{ rep_.erase( first.it_, last.it_ ); }

		// associative sequence		| complexity : linear			| exception : nothrow
		void clear()												{ rep_.clear(); }

		// container				| complexity : constant			| exception : nothrow
		void swap( ordered_map& x )									{ rep_.swap( x.rep_ ); }


		/////////////////////////////////////////////////////////////////
		// observers:
		
		// associative sequence		| complexity : constant		| exception : nothrow
		key_compare key_comp()		const	{ return rep_.key_comp(); }
		value_compare value_comp()	const	{ return value_compare( rep_.key_comp() ); }


		/////////////////////////////////////////////////////////////////
		// map operations:
		
 		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		iterator find( const key_type& x )						{ return iterator( rep_.find( x ) ); }
		const_iterator find( const key_type& x ) const			{ return const_iterator( rep_.find( x ) ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		size_type count( const key_type& x ) const				{ return rep_.count( x ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		iterator lower_bound( const key_type& x )				{ return iterator( rep_.lower_bound( x ) ); }
		const_iterator lower_bound( const key_type& x ) const	{ return const_iterator( rep_.lower_bound( x ) ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		iterator upper_bound( const key_type& x )				{ return iterator( rep_.upper_bound( x ) ); }
		const_iterator upper_bound( const key_type& x ) const	{ return const_iterator( rep_.upper_bound( x ) ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		std::pair<iterator, iterator> equal_range( const key_type& x )
		{ 
			std::pair<typename Rep_type::iterator, typename Rep_type::iterator> r = rep_.equal_range( x );
			return std::pair<iterator, iterator>( iterator( r.first ), iterator( r.second ) );
		}

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		std::pair<const_iterator, const_iterator> equal_range( const key_type& x ) const
		{
			std::pair<typename Rep_type::const_iterator, typename Rep_type::const_iterator> r = rep_.equal_range( x );
			return std::pair<const_iterator, const_iterator>( const_iterator( r.first ), const_iterator( r.second ) );
		}

	private:
		Rep_type rep_;
		allocator_type allocator_;
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ordered_multimap
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename K, typename T, typename Compare = std::less<K>, typename Allocator = std::allocator<std::pair<const K,T> > >
	class ordered_multimap			// equivalent keys
	{
		// function object to compare keys and values
		struct KeyCompare
		{
			KeyCompare( const Compare& comp ) : comp_( comp ) { }

			bool operator()( const K& x , const K& y )							 const	{ return comp_( x, y ); }
			bool operator()( const K& x , const std::pair<K,T>& y )				 const	{ return comp_( x, y.first ); }
			bool operator()( const std::pair<K,T>& x , const K& y )				 const	{ return comp_( x.first, y ); }
			bool operator()( const std::pair<K,T>& x , const std::pair<K,T>& y ) const	{ return comp_( x.first, y.first ); }

			operator Compare () const { return comp_; }

		private:
			Compare comp_;
		};

		typedef typename Allocator::template rebind<std::pair<K,T> >::other node_allocator;

	public:
		// container
		typedef K						key_type;
		typedef std::pair<const K,T>	value_type;

		// map
		typedef T						mapped_type;
		typedef Compare					key_compare;

		// map
		class value_compare : public std::binary_function<value_type, value_type, bool>
		{
			friend class ordered_multimap<K,T,Compare,Allocator>;

		protected:
			key_compare comp;
			value_compare( key_compare c ) : comp( c ) { }

		public:
			bool operator()( const value_type& x, const value_type& y ) const
			{
				return comp( x.first, y.first );
			}
		};

	private:
		// extract key part of value
		struct KeyPart : public std::unary_function<value_type,key_type>
		{
			const key_type& operator()( const value_type& x ) const { return x.first; }
		};
	    
		typedef detail::ordered_sequence<K, value_type, KeyPart, KeyCompare, std::vector<std::pair<K,T>, node_allocator> > Rep_type;

	public:

		// container
		typedef typename Allocator::reference				reference;
		typedef typename Allocator::const_reference			const_reference;
		typedef typename Allocator::size_type				size_type;
		typedef typename Allocator::difference_type			difference_type;

		typedef Allocator									allocator_type;
		typedef typename Allocator::pointer					pointer;
		typedef typename Allocator::const_pointer			const_pointer;
		
		// container
		class const_iterator;

		class iterator : public std::iterator< typename std::iterator_traits<typename Rep_type::iterator>::iterator_category , std::pair<const K,T>, typename std::iterator_traits<typename Rep_type::iterator>::difference_type >
		{
			friend class ordered_multimap<K,T,Compare,Allocator>;
			friend class const_iterator;

			iterator( typename Rep_type::iterator iter ) : it_( iter ) { }

		public:
			typedef typename std::iterator_traits<typename Rep_type::const_iterator>::iterator_category	iterator_category;
			typedef std::pair<const K,T>																value_type;
			typedef value_type& 																		reference;
			typedef value_type*																			pointer;
			typedef typename std::iterator_traits<typename Rep_type::const_iterator>::difference_type	difference_type;

			iterator() { }

			reference	operator* () const							{ return *reinterpret_cast<pointer>( &( *it_ ) ); }
			pointer		operator-> () const 						{ return reinterpret_cast<pointer>( &( *it_ ) ); }
			reference	operator[] ( difference_type index ) const	{ return *( it_ + index ); }
			
			iterator& operator++ ()			{ ++it_; return *this; }
			iterator& operator-- ()			{ --it_; return *this; }
			iterator  operator++ ( int )	{ iterator tmp( *this ); ++*this; return tmp; }
			iterator  operator-- ( int )	{ iterator tmp( *this ); --*this; return tmp; }
			
			iterator& operator+= ( difference_type n )		{ it_ += n; return *this; }
			iterator& operator-= ( difference_type n )		{ it_ -= n; return *this; }
			iterator  operator+  ( difference_type n ) const	{ iterator tmp( *this ); return tmp += n; }
			iterator  operator-  ( difference_type n ) const	{ iterator tmp( *this ); return tmp -= n; }

			difference_type operator- ( const iterator& iter ) const	{ return it_ - iter.it_; }

			bool operator== ( const iterator& iter ) const	{ return it_ == iter.it_; }
			bool operator!= ( const iterator& iter ) const	{ return it_ != iter.it_; }
			bool operator<	( const iterator& iter ) const	{ return it_ <  iter.it_; }
			bool operator<= ( const iterator& iter ) const	{ return it_ <= iter.it_; }
			bool operator>	( const iterator& iter ) const	{ return it_ >  iter.it_; }
			bool operator>= ( const iterator& iter ) const	{ return it_ >= iter.it_; }

		private:
			typename Rep_type::iterator it_;
		};

		class const_iterator : public std::iterator< typename std::iterator_traits<typename Rep_type::const_iterator>::iterator_category , const std::pair<const K,T>, typename std::iterator_traits<typename Rep_type::const_iterator>::difference_type >
		{
			friend class ordered_multimap<K,T,Compare,Allocator>;

			const_iterator( typename Rep_type::const_iterator iter ) : it_( iter ) { }

		public:
			typedef typename std::iterator_traits<typename Rep_type::const_iterator>::iterator_category	iterator_category;
			typedef const std::pair<const K,T>															value_type;
			typedef value_type& 																		reference;
			typedef value_type*																			pointer;
			typedef typename std::iterator_traits<typename Rep_type::const_iterator>::difference_type	difference_type;

			const_iterator() { }

			const_iterator( iterator it ) : it_( it.it_ ) { }

			reference	operator* () const							{ return *reinterpret_cast<pointer>( &( *it_ ) ); }
			pointer		operator-> () const 						{ return reinterpret_cast<pointer>( &( *it_ ) ); }
			reference	operator[] ( difference_type index ) const	{ return *( it_ + index ); }
			
			const_iterator& operator++ ()			{ ++it_; return *this; }
			const_iterator& operator-- ()			{ --it_; return *this; }
			const_iterator  operator++ ( int )	{ const_iterator tmp( *this ); ++*this; return tmp; }
			const_iterator  operator-- ( int )	{ const_iterator tmp( *this ); --*this; return tmp; }
			
			const_iterator& operator+= ( difference_type n )		{ it_ += n; return *this; }
			const_iterator& operator-= ( difference_type n )		{ it_ -= n; return *this; }
			const_iterator  operator+  ( difference_type n ) const	{ const_iterator tmp( *this ); return tmp += n; }
			const_iterator  operator-  ( difference_type n ) const	{ const_iterator tmp( *this ); return tmp -= n; }

			difference_type operator- ( const const_iterator& iter ) const	{ return it_ - iter.it_; }

			bool operator== ( const const_iterator& iter ) const	{ return it_ == iter.it_; }
			bool operator!= ( const const_iterator& iter ) const	{ return it_ != iter.it_; }
			bool operator<	( const const_iterator& iter ) const	{ return it_ <  iter.it_; }
			bool operator<= ( const const_iterator& iter ) const	{ return it_ <= iter.it_; }
			bool operator>	( const const_iterator& iter ) const	{ return it_ >  iter.it_; }
			bool operator>= ( const const_iterator& iter ) const	{ return it_ >= iter.it_; }

		private:
			typename Rep_type::const_iterator it_;
		};

		// reversible container
		typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef std::reverse_iterator<iterator>			reverse_iterator;
		
		/////////////////////////////////////////////////////////////////
		// construct/copy/destroy:
			
		// container	| complexity : constant		| exception : 
		explicit ordered_multimap( const key_compare& comp = Compare(), const allocator_type& a = Allocator() )
			: rep_( KeyCompare( comp ), a ), allocator_( a )
		{ }

		// associative sequence		| complexity : quadratic/linear	| exception : 
		template <typename InputIterator>
		ordered_multimap( InputIterator first, InputIterator last, const key_compare& comp = Compare(), const allocator_type& a = Allocator() )
			: rep_( KeyCompare( comp ), a ), allocator_( a )
		{ insert( first, last );  }

		// container	| complexity : linear		| exception : 
		ordered_multimap( const ordered_multimap& x )
			: rep_( x.rep_ ), allocator_( x.allocator_ )
		{ }

		// container	| complexity : linear		| exception : nothrow
		~ordered_multimap()	{ }

		// container	| complexity : linear		| exception : strong
		ordered_multimap& operator= ( const ordered_multimap& rhs )
		{
			rep_ = rhs.rep_;
			return *this;
		}

		//				| complexity : constant		| exception : nothrow
		allocator_type get_allocator() const { return allocator_; }


		/////////////////////////////////////////////////////////////////
		// iterators:
		
		// container	| complexity : constant		| exception : nothrow
		iterator		begin()			{ return iterator( rep_.begin() ); }
		const_iterator	begin()	const	{ return const_iterator( rep_.begin() ); }
		iterator		end()			{ return iterator( rep_.end() ); }
		const_iterator	end()	const	{ return const_iterator( rep_.end() ); }

		// reversible container	| complexity : constant		| exception : nothrow
		reverse_iterator		rbegin()		{ return reverse_iterator( end() ); }
		const_reverse_iterator	rbegin() const	{ return const_reverse_iterator( end() ); }
		reverse_iterator		rend()			{ return reverse_iterator( begin() ); }
		const_reverse_iterator	rend()	 const	{ return const_reverse_iterator( begin() ); }

		
		/////////////////////////////////////////////////////////////////
		// capacity:
		
		// container	| complexity : constant		| exception : nothrow
		bool empty()		 const	{ return rep_.empty(); }
		size_type size()	 const	{ return rep_.size(); }
		size_type max_size() const	{ return rep_.max_size(); }

		
		/////////////////////////////////////////////////////////////////
		// modifiers:
		
		// associative sequence		| complexity : linear			| exception : weak
		iterator insert( const value_type& x )						{ return iterator( rep_.insert_equal( x ) ); }

		// associative sequence		| complexity : linear			| exception : weak
		iterator insert( iterator position, const value_type& x )	{ return iterator( rep_.insert_equal( position.it_, x ) ); }

		// associative sequence		| complexity : quadratic/linear	| exception : weak
		template <typename InputIterator>
		void insert( InputIterator first, InputIterator last )		{ rep_.insert_equal( first, last ); }
	    
		// associative sequence		| complexity : linear			| exception : weak
		size_type erase( const key_type& x )						{ return rep_.erase( x ); }

		// associative sequence		| complexity : linear			| exception : weak
		void erase( iterator position )								{ rep_.erase( position.it_ ); }
		void erase( iterator first, iterator last )					{ rep_.erase( first.it_, last.it_ ); }

		// associative sequence		| complexity : linear			| exception : nothrow
		void clear()												{ rep_.clear(); }

		// container	| complexity : constant						| exception : nothrow
		void swap( ordered_multimap& x )							{ rep_.swap( x.rep_ ); }


		/////////////////////////////////////////////////////////////////
		// observers:
		
		// associative sequence		| complexity : constant			| exception : nothrow
		key_compare key_comp()		const	{ return rep_.key_comp(); }
		value_compare value_comp()  const	{ return value_compare( rep_.key_comp() ); }


		/////////////////////////////////////////////////////////////////
		// map operations:
		
 		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		iterator find( const key_type& x )						{ return iterator( rep_.find( x ) ); }
		const_iterator find( const key_type& x ) const			{ return const_iterator( rep_.find( x ) ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		size_type count( const key_type& x ) const				{ return rep_.count( x ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		iterator lower_bound( const key_type& x )				{ return iterator( rep_.lower_bound( x ) ); }
		const_iterator lower_bound( const key_type& x ) const	{ return const_iterator( rep_.lower_bound( x ) ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		iterator upper_bound( const key_type& x )				{ return iterator( rep_.upper_bound( x ) ); }
		const_iterator upper_bound( const key_type& x ) const	{ return const_iterator( rep_.upper_bound( x ) ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		std::pair<iterator, iterator> equal_range( const key_type& x )
		{ 
			std::pair<typename Rep_type::iterator, typename Rep_type::iterator> r = rep_.equal_range( x );
			return std::pair<iterator, iterator>( iterator( r.first ), iterator( r.second ) );
		}

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		std::pair<const_iterator, const_iterator> equal_range( const key_type& x ) const
		{
			std::pair<typename Rep_type::const_iterator, typename Rep_type::const_iterator> r = rep_.equal_range( x );
			return std::pair<const_iterator, const_iterator>( const_iterator( r.first ), const_iterator( r.second ) );
		}

	private:
		Rep_type rep_;
		allocator_type allocator_;
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// global functions
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	// container	| complexity : linear		| exception : strong, note A
	template <typename K, typename T, typename Compare, typename Allocator>
	inline bool operator == ( const ordered_map<K,T,Compare, Allocator>& lhs, const ordered_map<K,T,Compare, Allocator>& rhs )
	{
		return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin() );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename K, typename T, typename Compare, typename Allocator>
	inline bool operator < ( const ordered_map<K,T,Compare, Allocator>& lhs, const ordered_map<K,T,Compare, Allocator>& rhs )
	{
		return std::lexicographical_compare( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename K, typename T, typename Compare, typename Allocator>
	inline bool operator != ( const ordered_map<K,T,Compare, Allocator>& lhs, const ordered_map<K,T,Compare, Allocator>& rhs )
	{
		return !( lhs == rhs );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename K, typename T, typename Compare, typename Allocator>
	inline bool operator > ( const ordered_map<K,T,Compare, Allocator>& lhs, const ordered_map<K,T,Compare, Allocator>& rhs )
	{
		return rhs < lhs;
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename K, typename T, typename Compare, typename Allocator>
	inline bool operator <= ( const ordered_map<K,T,Compare, Allocator>& lhs, const ordered_map<K,T,Compare, Allocator>& rhs )
	{
		return !( rhs < lhs );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename K, typename T, typename Compare, typename Allocator>
	inline bool operator >= ( const ordered_map<K,T,Compare, Allocator>& lhs, const ordered_map<K,T,Compare, Allocator>& rhs )
	{
		return !( lhs < rhs );
	}

	// container	| complexity : constant		| exception : nothrow
	template <typename K, typename T, typename Compare, typename Allocator>
	inline void swap( ordered_map<K,T,Compare, Allocator>& x, ordered_map<K,T,Compare, Allocator>& y )
	{
		x.swap( y );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename K, typename T, typename Compare, typename Allocator>
	inline bool operator == ( const ordered_multimap<K,T,Compare, Allocator>& lhs, const ordered_multimap<K,T,Compare, Allocator>& rhs )
	{
		return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin() );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename K, typename T, typename Compare, typename Allocator>
	inline bool operator < ( const ordered_multimap<K,T,Compare, Allocator>& lhs, const ordered_multimap<K,T,Compare, Allocator>& rhs )
	{
		return std::lexicographical_compare( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename K, typename T, typename Compare, typename Allocator>
	inline bool operator != ( const ordered_multimap<K,T,Compare, Allocator>& lhs, const ordered_multimap<K,T,Compare, Allocator>& rhs )
	{
		return !( lhs == rhs );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename K, typename T, typename Compare, typename Allocator>
	inline bool operator > ( const ordered_multimap<K,T,Compare, Allocator>& lhs, const ordered_multimap<K,T,Compare, Allocator>& rhs )
	{
		return rhs < lhs;
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename K, typename T, typename Compare, typename Allocator>
	inline bool operator <= ( const ordered_multimap<K,T,Compare, Allocator>& lhs, const ordered_multimap<K,T,Compare, Allocator>& rhs )
	{
		return !( rhs < lhs );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename K, typename T, typename Compare, typename Allocator>
	inline bool operator >= ( const ordered_multimap<K,T,Compare, Allocator>& lhs, const ordered_multimap<K,T,Compare, Allocator>& rhs )
	{
		return !( lhs < rhs );
	}

	// container	| complexity : constant		| exception : nothrow
	template <typename K, typename T, typename Compare, typename Allocator>
	inline void swap( ordered_multimap<K,T,Compare, Allocator>& x, ordered_multimap<K,T,Compare, Allocator>& y )
	{
		x.swap( y );
	}

}	// namespace dti

#endif // ORDERED_MAP_H_RMA07112003
