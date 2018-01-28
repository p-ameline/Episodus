//  ordered_set.h -- implementation af a STL complatible set/multiset based on an ordered sequence.
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

#ifndef ORDERED_SET_H_RMA07112003
#define ORDERED_SET_H_RMA07112003

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
	// ordered_set
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename T, typename Compare = std::less<T>, typename Allocator = std::allocator<T> >
	class ordered_set			// unique keys
	{
		// extract key part of value
		struct KeyPart : public std::unary_function<T,T>
		{
			const T& operator()( const T& x ) const { return x; }
		};
	    
		typedef detail::ordered_sequence<T, T, KeyPart, Compare, std::vector<T, Allocator> > Rep_type;

	public:
		// container
		typedef typename Rep_type::key_type					key_type;
		typedef typename Rep_type::value_type				value_type;
		typedef Compare										key_compare;
		typedef Compare										value_compare;
		typedef typename Rep_type::reference				reference;
		typedef typename Rep_type::const_reference			const_reference;
		typedef typename Rep_type::size_type				size_type;
		typedef typename Rep_type::difference_type			difference_type;

		typedef typename Rep_type::allocator_type			allocator_type;
		typedef typename Rep_type::pointer					pointer;
		typedef typename Rep_type::const_pointer			const_pointer;

		// container
		typedef typename Rep_type::const_iterator			const_iterator;
		typedef typename Rep_type::iterator					iterator;

		// reversible container
		typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef std::reverse_iterator<iterator>			reverse_iterator;

		/////////////////////////////////////////////////////////////////
		// construct/copy/destroy:
			
		// container	| complexity : constant		| exception : 
		explicit ordered_set( const key_compare& comp = Compare(), const allocator_type& a = Allocator() )
			: rep_( comp, a )
		{ }

		// associative sequence		| complexity : quadratic/linear	| exception : 
		template <typename InputIterator>
		ordered_set( InputIterator first, InputIterator last, const key_compare& comp = Compare(), const allocator_type& a = Allocator()  )
			: rep_( comp, a )
		{ insert( first, last ); }

		// container	| complexity : linear		| exception : 
		ordered_set( const ordered_set& x )
			: rep_( x.rep_ )
		{ }

		// container	| complexity : linear		| exception : nothrow
		~ordered_set()	{ }

		// container	| complexity : linear		| exception : strong
		ordered_set& operator= ( const ordered_set& rhs )
		{
			rep_ = rhs.rep_;
			return *this;
		}

		//				| complexity : constant		| exception : nothrow
		allocator_type get_allocator() const { return rep_.get_allocator(); }


		/////////////////////////////////////////////////////////////////
		// iterators:
		
		// container	| complexity : constant		| exception : nothrow
		iterator		begin()			{ return rep_.begin(); }
		const_iterator	begin()	const	{ return rep_.begin(); }
		iterator		end()			{ return rep_.end(); }
		const_iterator	end()	const	{ return rep_.end(); }

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
		std::pair<iterator, bool> insert( const value_type& x )		{ return rep_.insert_unique( x ); }

		// associative sequence		| complexity : linear			| exception : weak
		iterator insert( iterator position, const value_type& x )	{ return rep_.insert_unique( position, x ); }

		// associative sequence		| complexity : quadratic/linear	| exception : weak
		template <typename InputIterator>
		void insert( InputIterator first, InputIterator last )		{ rep_.insert_unique( first, last ); }
	    
		// associative sequence		| complexity : linear			| exception : weak
		size_type erase( const key_type& x )						{ return rep_.erase( x ); }

		// associative sequence		| complexity : linear			| exception : weak
		void erase( iterator position )								{ rep_.erase( position ); }
		void erase( iterator first, iterator last )					{ rep_.erase( first, last ); }

		// associative sequence		| complexity : linear			| exception : nothrow
		void clear()												{ rep_.clear(); }

		// container				| complexity : constant			| exception : nothrow
		void swap( ordered_set& x )									{ rep_.swap( x.rep_ ); }


		/////////////////////////////////////////////////////////////////
		// observers:
		
		// associative sequence		| complexity : constant		| exception : nothrow
		key_compare key_comp()		const	{ return rep_.key_comp(); }
		value_compare value_comp()	const	{ return rep_.key_comp(); }


		/////////////////////////////////////////////////////////////////
		// set operations:
		
 		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		iterator find( const key_type& x )						{ return rep_.find( x ); }
		const_iterator find( const key_type& x ) const			{ return rep_.find( x ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		size_type count( const key_type& x ) const				{ return rep_.count( x ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		iterator lower_bound( const key_type& x )				{ return rep_.lower_bound( x ); }
		const_iterator lower_bound( const key_type& x ) const	{ return rep_.lower_bound( x ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		iterator upper_bound( const key_type& x )				{ return rep_.upper_bound( x ); }
		const_iterator upper_bound( const key_type& x ) const	{ return rep_.upper_bound( x ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		std::pair<iterator, iterator> equal_range( const key_type& x )
		{
			return rep_.equal_range( x );
		}

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		std::pair<const_iterator, const_iterator> equal_range( const key_type& x ) const
		{
			return rep_.equal_range( x );
		}

	private:
		Rep_type rep_;
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ordered_multiset
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename T, typename Compare = std::less<T>, typename Allocator = std::allocator<T> >
	class ordered_multiset			// equivalent keys
	{
		// extract key part of value
		struct KeyPart : public std::unary_function<T,T>
		{
			const T& operator()( const T& x ) const { return x; }
		};
	    
		typedef detail::ordered_sequence<T, T, KeyPart, Compare, std::vector<T, Allocator> > Rep_type;

	public:
		// container
		typedef typename Rep_type::key_type					key_type;
		typedef typename Rep_type::value_type				value_type;
		typedef Compare										key_compare;
		typedef Compare										value_compare;
		typedef typename Rep_type::reference				reference;
		typedef typename Rep_type::const_reference			const_reference;
		typedef typename Rep_type::size_type				size_type;
		typedef typename Rep_type::difference_type			difference_type;

		typedef typename Rep_type::allocator_type			allocator_type;
		typedef typename Rep_type::pointer					pointer;
		typedef typename Rep_type::const_pointer			const_pointer;

		// container
		typedef typename Rep_type::const_iterator			const_iterator;
		typedef typename Rep_type::iterator					iterator;

		// reversible container
		typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef std::reverse_iterator<iterator>			reverse_iterator;

		/////////////////////////////////////////////////////////////////
		// construct/copy/destroy:
			
		// container	| complexity : constant		| exception : 
		explicit ordered_multiset( const Compare& comp = Compare(), const allocator_type& a = Allocator() )
			: rep_( comp, a )
		{ }

		// associative sequence		| complexity : quadratic/linear	| exception : 
		template <typename InputIterator>
		ordered_multiset( InputIterator first, InputIterator last, const key_compare& comp = Compare(), const allocator_type& a = Allocator() )
			: rep_( comp, a )
		{ insert( first, last ); }

		// container	| complexity : linear		| exception : 
		ordered_multiset( const ordered_multiset& x )
			: rep_( x.rep_ )
		{ }

		// container	| complexity : linear		| exception : nothrow
		~ordered_multiset()	{ }

		// container	| complexity : linear		| exception : strong
		ordered_multiset& operator= ( const ordered_multiset& rhs )
		{
			rep_ = rhs.rep_;
			return *this;
		}

		//				| complexity : constant		| exception : nothrow
		allocator_type get_allocator() const { return rep_.get_allocator(); }


		/////////////////////////////////////////////////////////////////
		// iterators:
		
		// container	| complexity : constant		| exception : nothrow
		iterator		begin()			{ return rep_.begin(); }
		const_iterator	begin()	const	{ return rep_.begin(); }
		iterator		end()			{ return rep_.end(); }
		const_iterator	end()	const	{ return rep_.end(); }

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
		iterator insert( const value_type& x )						{ return rep_.insert_equal( x ); }

		// associative sequence		| complexity : linear			| exception : weak
		iterator insert( iterator position, const value_type& x )	{ return rep_.insert_equal( position, x ); }

		// associative sequence		| complexity : quadratic/linear	| exception : weak
		template <typename InputIterator>
		void insert( InputIterator first, InputIterator last )		{ rep_.insert_equal( first, last ); }
	    
		// associative sequence		| complexity : linear			| exception : weak
		size_type erase( const key_type& x )						{ return rep_.erase( x ); }

		// associative sequence		| complexity : linear			| exception : weak
		void erase( iterator position )								{ rep_.erase( position ); }
		void erase( iterator first, iterator last )					{ rep_.erase( first, last ); }

		// associative sequence		| complexity : linear			| exception : nothrow
		void clear()												{ rep_.clear(); }

		// container				| complexity : constant			| exception : nothrow
		void swap( ordered_multiset& x )							{ rep_.swap( x.rep_ ); }


		/////////////////////////////////////////////////////////////////
		// observers:
		
		// associative sequence		| complexity : constant			| exception : nothrow
		key_compare key_comp()		const	{ return rep_.key_comp(); }
		value_compare value_comp()	const	{ return rep_.key_comp(); }


		/////////////////////////////////////////////////////////////////
		// set operations:
		
 		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		iterator find( const key_type& x )						{ return rep_.find( x ); }
		const_iterator find( const key_type& x ) const			{ return rep_.find( x ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		size_type count( const key_type& x ) const				{ return rep_.count( x ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		iterator lower_bound( const key_type& x )				{ return rep_.lower_bound( x ); }
		const_iterator lower_bound( const key_type& x ) const	{ return rep_.lower_bound( x ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		iterator upper_bound( const key_type& x )				{ return rep_.upper_bound( x ); }
		const_iterator upper_bound( const key_type& x ) const	{ return rep_.upper_bound( x ); }

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		std::pair<iterator, iterator> equal_range( const key_type& x )
		{
			return rep_.equal_range( x );
		}

		// associative sequence		| complexity : logarithmic		| exception : strong, note A
		std::pair<const_iterator, const_iterator> equal_range( const key_type& x ) const
		{
			return rep_.equal_range( x );
		}

	private:
		Rep_type rep_;
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// global functions
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Compare, typename Allocator>
	inline bool operator == ( const ordered_set<T, Compare, Allocator>& lhs, const ordered_set<T, Compare, Allocator>& rhs )
	{
		return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin() );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Compare, typename Allocator>
	inline bool operator < ( const ordered_set<T, Compare, Allocator>& lhs, const ordered_set<T, Compare, Allocator>& rhs )
	{
		return std::lexicographical_compare( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Compare, typename Allocator>
	inline bool operator != ( const ordered_set<T, Compare, Allocator>& lhs, const ordered_set<T, Compare, Allocator>& rhs )
	{
		return !( lhs == rhs );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Compare, typename Allocator>
	inline bool operator > ( const ordered_set<T, Compare, Allocator>& lhs, const ordered_set<T, Compare, Allocator>& rhs )
	{
		return rhs < lhs;
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Compare, typename Allocator>
	inline bool operator <= ( const ordered_set<T, Compare, Allocator>& lhs, const ordered_set<T, Compare, Allocator>& rhs )
	{
		return !( rhs < lhs );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Compare, typename Allocator>
	inline bool operator >= ( const ordered_set<T, Compare, Allocator>& lhs, const ordered_set<T, Compare, Allocator>& rhs )
	{
		return !( lhs < rhs );
	}

	// container	| complexity : constant		| exception : nothrow
	template <typename T, typename Compare, typename Allocator>
	inline void swap( ordered_set<T, Compare, Allocator>& x, ordered_set<T, Compare, Allocator>& y )
	{
		x.swap( y );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Compare, typename Allocator>
	inline bool operator == ( const ordered_multiset<T, Compare, Allocator>& lhs, const ordered_multiset<T, Compare, Allocator>& rhs )
	{
		return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin() );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Compare, typename Allocator>
	inline bool operator < ( const ordered_multiset<T, Compare, Allocator>& lhs, const ordered_multiset<T, Compare, Allocator>& rhs )
	{
		return std::lexicographical_compare( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Compare, typename Allocator>
	inline bool operator != ( const ordered_multiset<T, Compare, Allocator>& lhs, const ordered_multiset<T, Compare, Allocator>& rhs )
	{
		return !( lhs == rhs );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Compare, typename Allocator>
	inline bool operator > ( const ordered_multiset<T, Compare, Allocator>& lhs, const ordered_multiset<T, Compare, Allocator>& rhs )
	{
		return rhs < lhs;
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Compare, typename Allocator>
	inline bool operator <= ( const ordered_multiset<T, Compare, Allocator>& lhs, const ordered_multiset<T, Compare, Allocator>& rhs )
	{
		return !( rhs < lhs );
	}

	// container	| complexity : linear		| exception : strong, note A
	template <typename T, typename Compare, typename Allocator>
	inline bool operator >= ( const ordered_multiset<T, Compare, Allocator>& lhs, const ordered_multiset<T, Compare, Allocator>& rhs )
	{
		return !( lhs < rhs );
	}

	// container	| complexity : constant		| exception : nothrow
	template <typename T, typename Compare, typename Allocator>
	inline void swap( ordered_multiset<T, Compare, Allocator>& x, ordered_multiset<T, Compare, Allocator>& y )
	{
		x.swap( y );
	}

}	// namespace dti

#endif // ORDERED_SET_H_RMA07112003
