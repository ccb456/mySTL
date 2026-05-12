#pragma once

#include "RB-tree.h"
#include "function.h"
#include "alloc.h"
#include "iterator.h"

namespace My_STL
{
	template <typename Key, typename Compare = less<Key>, typename Alloc = alloc>
	class set
	{
	public:
		typedef Key key_type;
		typedef Key value_type;
		typedef Compare key_compare;
		typedef Compare value_compare;

	private:
		typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;

		rep_type t;		// ºìºÚÊ÷

	public:
		typedef typename rep_type::const_pointer pointer;
		typedef typename rep_type::const_pointer const_pointer;
		typedef typename rep_type::const_reference reference;
		typedef typename rep_type::const_reference const_reference;
		typedef typename rep_type::const_iterator iterator;
		typedef typename rep_type::const_iterator const_iterator;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::difference_type difference_type;

		set(): t(Compare()) {}
		explicit set(const Compare& cmp) : t(cmp) {}

		template <typename InputIterator>
		set(InputIterator first, InputIterator last, const Compare& comp) : t(comp)
		{
			t.insert_unique(first, last);
		}

		template <typename InputIterator>
		set(InputIterator first, InputIterator last) : t(Compare())
		{
			t.insert_unique(first, last);
		}

		set(const set<Key, Compare, Alloc>& x) : t(x.t){}

		set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x)
		{
			t = x.t;
			return *this;
		}


	};
};