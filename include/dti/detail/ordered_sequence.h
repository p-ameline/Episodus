//  ordered_sequence.h -- implementation af a STL complatible ordered_sequence.
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

#ifndef ORDERED_SEQUENCE_H_RMA07112003
#define ORDERED_SEQUENCE_H_RMA07112003

#include <iterator>
#include <utility>
#include <memory>
#include <functional>
#include <algorithm>

/*	Requirements for element type
	* must be copy-constructible
	* must be copy-assignable
	* destructor must not throw exception

	Methods marked with note A only throws an exception if operator== or operator< 
	or the used predicate throws an exception

	Iterators are invalidated by calls to insert or erase methods
	same goes for element references

	The weak exception guarantee of insert()/erase() only applies if the copy constructor 
	or assignment operator can throw an exception. Otherwise insert() of single elements 
	have a strong guarantee and erase() has a guarantee of nothrow.


	Implementation:
	The elements are kept in a sequence container with random access iterators. 
	At any time the element are sorted. The standard library algorithms, 
	lower_bound(), upper_bound() and equal_range() are used for lookups.

	Alternative implementations:
	1.	Sort the elements lazily
		Consequence: Insertion of multiple elements will be faster if duplicate 
		keys are allowed. Lookup speed will not always be logarithmic
*/

namespace dti
{
	namespace detail
	{

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ordered_sequence
		//////////////////////////////////////////////////////////////////////////////////////////////////////////

		// K :			key type
		// T :			value type
		// Key :		function object to extract key part of value
		// Compare :	function object to compare keys
		// Container :	sequence with random access iterator

		template <typename K, typename T, typename Key, typename Compare, typename Container>
		class ordered_sequence
		{
		public:
			// container
			typedef K											key_type;
			typedef T											value_type;
			typedef Compare										key_compare;
			typedef typename Container::reference				reference;
			typedef typename Container::const_reference			const_reference;
			typedef typename Container::size_type				size_type;
			typedef typename Container::difference_type			difference_type;

			typedef typename Container::allocator_type			allocator_type;
			typedef typename Container::pointer					pointer;
			typedef typename Container::const_pointer			const_pointer;

			// container
			typedef typename Container::iterator				iterator;
			typedef typename Container::const_iterator			const_iterator;

			/////////////////////////////////////////////////////////////////
			// construct/copy/destroy:
				
			// container	| complexity : constant		| exception : 
			ordered_sequence( const key_compare& comp, const allocator_type& a )
				: container_( a ), comp_( comp )
			{ }

			//				| complexity : constant		| exception : nothrow
			allocator_type get_allocator() const { return container_.get_allocator(); }

			/////////////////////////////////////////////////////////////////
			// iterators:
			
			// container	| complexity : constant		| exception : nothrow
			iterator		begin()			{ return container_.begin(); }
			const_iterator	begin()	const	{ return container_.begin(); }
			iterator		end()			{ return container_.end(); }
			const_iterator	end()	const	{ return container_.end(); }


			/////////////////////////////////////////////////////////////////
			// capacity:
			
			// container	| complexity : constant		| exception : nothrow
			bool empty()		 const	{ return container_.empty(); }
			size_type size()	 const	{ return container_.size(); }
			size_type max_size() const	{ return container_.max_size(); }


			/////////////////////////////////////////////////////////////////
			// modifiers:
			
			// associative sequence		| complexity : linear		| exception : weak
			std::pair<iterator, bool> insert_unique( const value_type& x )
			{
				bool inserted = false;
				iterator it = lower_bound( Key()( x ) );

				if( it == end() || comp_( x,  *it ) )
				{
					it = container_.insert( it, x );
					inserted = true;
				}

				return std::pair<iterator, bool>( it, inserted );
			}

			// associative sequence		| complexity : linear		| exception : weak
			iterator insert_unique( iterator position, const value_type& x )
			{
				if( position != end() && comp_( *position, x ) )
				{	// 'position' is before x
					if( (position + 1) == end() || comp_( x, position[ 1 ] ) )
					{
						return container_.insert( position + 1, x );
					}
					else if( !comp_( position[ 1 ], x ) )
					{	// x already inserted
						return position + 1;
					}
				}

				// 'position' didn´t point the right place
				return insert_unique( x ).first;
			}
		    
			// associative sequence		| complexity : quadratic/linear	| exception : weak
			template <typename InputIterator>
			void insert_unique( InputIterator first, InputIterator last )
			{
				iterator pos = end();
				for( InputIterator it = first; it != last; ++it )
				{
					pos = insert_unique( pos, *it );
				}
			}

			// associative sequence		| complexity : linear		| exception : weak
			iterator insert_equal( const value_type& x )
			{
				iterator it = upper_bound( Key()( x ) );
				return container_.insert( it, x );
			}

			// associative sequence		| complexity : linear		| exception : weak
			iterator insert_equal( iterator position, const value_type& x )
			{
				if( position != end() && !comp_( x, *position ) )
				{	// 'position' isn't after x
					if( position + 1 == end() || !comp_( position[ 1 ], x ) )
					{
						return container_.insert( position + 1, x );
					}
				}

				// 'position' didn´t point the right place
				return insert_equal( x );
			}
 
			// associative sequence		| complexity : quadratic/linear	| exception : weak
			template <typename InputIterator>
			void insert_equal( InputIterator first, InputIterator last )
			{
				iterator pos = end();
				for( InputIterator it = first; it != last; ++it )
				{
					pos = insert_equal( pos, *it );
				}
			}

			// associative sequence		| complexity : linear		| exception : weak
			size_type erase( const key_type& x )
			{
				std::pair<iterator,iterator> p = equal_range( x );
				size_type n = std::distance( p.first, p.second );
				erase( p.first, p.second );
				return n;
			}

			// associative sequence		| complexity : linear		| exception : weak
			void erase( iterator position )
			{
				container_.erase( position );
			}

			// associative sequence		| complexity : linear		| exception : weak
			void erase( iterator first, iterator last )
			{
				container_.erase( first, last );
			}

			// associative sequence		| complexity : linear		| exception : nothrow
			void clear()
			{
				container_.clear();
			}

			// container				| complexity : constant		| exception : nothrow
			void swap( ordered_sequence& x )
			{
				std::swap( comp_, x.comp_ );		
				container_.swap( x.container_ );		
			}


			/////////////////////////////////////////////////////////////////
			// observers:
			
			// associative sequence		| complexity : constant		| exception : nothrow
			key_compare key_comp() const { return comp_; }


			/////////////////////////////////////////////////////////////////
			// search operations:
			
 			// associative sequence		| complexity : logarithmic		| exception : strong, note A
			iterator find( const key_type& x )
			{
				iterator it = lower_bound( x );
				return it != end() && !comp_( x, *it ) ? it : end();
			}
		 
			// associative sequence		| complexity : logarithmic		| exception : strong, note A
			const_iterator find( const key_type& x ) const
			{
				const_iterator it = lower_bound( x );
				return it != end() && !comp_( x, *it ) ? it : end();
 			}

			// associative sequence		| complexity : logarithmic		| exception : strong, note A
			size_type count( const key_type& x ) const
			{
				std::pair<const_iterator, const_iterator> p = equal_range( x );
				return std::distance( p.first, p.second );
			}

			// associative sequence		| complexity : logarithmic		| exception : strong, note A
			iterator lower_bound( const key_type& x )
			{
				return std::lower_bound( begin(), end(), x, comp_ );
			}

			// associative sequence		| complexity : logarithmic		| exception : strong, note A
			const_iterator lower_bound( const key_type& x ) const
			{
				return std::lower_bound( begin(), end(), x, comp_ );
			}

			// associative sequence		| complexity : logarithmic		| exception : strong, note A
			iterator upper_bound( const key_type& x )
			{
				return std::upper_bound( begin(), end(), x, comp_ );
			}

			// associative sequence		| complexity : logarithmic		| exception : strong, note A
			const_iterator upper_bound( const key_type& x ) const
			{
				return std::upper_bound( begin(), end(), x, comp_ );
			}

			// associative sequence		| complexity : logarithmic		| exception : strong, note A
			std::pair<iterator, iterator> equal_range( const key_type& x )
			{
				return std::equal_range( begin(), end(), x, comp_ );
			}

			// associative sequence		| complexity : logarithmic		| exception : strong, note A
			std::pair<const_iterator, const_iterator> equal_range( const key_type& x ) const
			{
				return std::equal_range( begin(), end(), x, comp_ );
			}

		private:
			Container container_;

			key_compare comp_;
		};

	}	// namespace detail
}	// namespace dti

#endif // ORDERED_SEQUENCE_H_RMA30062003
