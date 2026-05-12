#pragma once

#include "alloc.h"
#include <cstddef>
#include "iterator.h"
#include "construct.h"

using std::size_t;
using std::ptrdiff_t;


namespace My_STL
{
	template<typename T>
	struct __list_node
	{
		__list_node<T>* prev;
		__list_node<T>* next;
		T data;
	};


	template<typename T>
	struct __list_iterator
	{
		typedef __list_iterator<T> iterator;
		typedef __list_iterator<T> self;

		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef __list_node<T>* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		link_type node;

		__list_iterator(){}
		__list_iterator(link_type x) : node(x) {}
		__list_iterator(const iterator& x): node(x.node){}


		bool operator ==(const self& x) const
		{
			return node == x.node;
		}

		bool operator !=(const self& x) const
		{
			return node != x.node;
		}

		reference operator*() const
		{
			return (*node).data;
		}

		pointer operator->()
		{
			return &(operator*());
		}

		self& operator++()
		{
			node = node->next;
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return tmp;
		}

		self& operator--()
		{
			node = node->prev;
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			--*this;
			return tmp;
		}

	};


	template <typename T, typename Alloc = alloc>
	class list
	{
	public:
		typedef __list_node<T>*		link_type;
		typedef T					value_type;
		typedef value_type*			pionter;
		typedef __list_iterator<T>  iterator;
		typedef value_type&			reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
	protected:
		typedef __list_node<T> list_node;
		typedef simple_alloc<list_node, Alloc> list_node_allocator;
		
		// 配置一个节点并返回
		link_type get_node()
		{
			return list_node_allocator::allocate();
		}
		// 释放一个节点
		void put_node(link_type p)
		{
			list_node_allocator::deallocate(p);
		}

		// 产生一个节点，带有元素值
		link_type create_node(const T& x)
		{
			link_type p = get_node();
			construct(&p->data, x);
			return p;
		}

		// 销毁一个节点
		void destroy_node(link_type p)
		{
			destroy(&p->data);
			put_node(p);
		}


	protected:
		link_type node;

	protected:
		void empty_initialize()
		{
			node = get_node();
			node->next = node;
			node->prev = node;
		}


		// 将[ first, last) 内的所有元素移动到position之前
		void transfer(iterator position, iterator first, iterator last)
		{
			if (position != last)
			{
				(*(link_type((*last.node).prev))).next = position.node;
				(*(link_type((*first.node).prev))).next = last.node;
				(*(link_type((*position.node).prev))).next = first.node;

				link_type tmp = link_type((*position.node).prev);
				(*position.node).prev = (*last.node).prev;
				(*last.node).prev = (*first.node).prev;
				(*first.node).prev = tmp;
			}
		}

	public:

		list()
		{
			empty_initialize();
		}
		iterator begin() const
		{
			return (link_type)(node->next);
		}

		iterator end() const
		{
			return node;
		}

		bool empty() const
		{
			return node->next == node;
		}

		size_type size() const
		{
			size_type result = 0;
			result = distance(begin(), end());
			return result;
		}

		reference front()
		{
			return *begin();
		}

		reference back()
		{
			return *(--end());
		}

		void push_back(const T& x)
		{
			insert(end(), x);
		}
		void push_front(const T& x)
		{
			insert(begin(), x);
		}

		iterator erase(iterator position)
		{
			link_type next_node = link_type(position.node->next);
			link_type prev_node = link_type(position.node->prev);

			prev_node->next = next_node;
			next_node->prev = prev_node;
			destroy_node(position.node);

			return iterator(next_node);
		}

		iterator insert(iterator position, const T& x)
		{
			link_type tmp = create_node(x);
			tmp->next = position.node;
			tmp->prev = position.node->prev;
			(link_type(position.node->prev))->next = tmp;
			position.node->prev = tmp;
			return tmp;

		}

		void pop_front()
		{
			erase(begin());
		}

		void pop_back()
		{
			iterator tmp = end();
			erase(--tmp);
		}

		void splice(iterator position, list& x)
		{
			if (!x.empty())
			{
				transfer(position, x.begin(), x.end());
			}
		}

		void splice(iterator position, iterator i)
		{
			iterator j = i;
			++j;
			if (position == i || position == j) return;
			transfer(position, i, j);
		}

		void splice(iterator position, iterator first, iterator last)
		{
			if (first != last)
			{
				transfer(position, first, last);
			}
		}

		void clear();
		void remove(const T& x);
		void unique();
		void merge(list<T, Alloc>& x);
		void reverse();
		void sort();

	};

	template <typename T, typename Alloc>
	void list<T, Alloc>::clear()
	{
		link_type cur = (link_type)(node->next);

		while (cur != node)
		{
			link_type tmp = cur;
			cur = (link_type)cur->next;
			destroy(tmp);
		}

		node->next = node;
		node->prev = node;
	}

	template <typename T, typename Alloc>
	void list<T, Alloc>::remove(const T& value)
	{
		iterator first = begin();
		iterator last = end();

		while (first != last)
		{
			iterator next = first;
			++next;
			if (*first == value)
			{
				erase(first);
			}

			first = next;
		}
	}

	template <typename T, typename Alloc>
	void list<T, Alloc>::unique()
	{
		iterator first = begin();
		iterator last = end();

		if (first == last)	return;
		iterator next = first;

		while (++next != last)
		{
			if (*first == *next)
			{
				erase(next);
			}
			else
			{
				first = next;
				next = first;
			}
		}
	}

	template <typename T, typename Alloc>
	void list<T, Alloc>::merge(list<T, Alloc>& x)
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();

		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *first1)
			{
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else
			{
				++first1;
			}

		}

		if (first2 != last2)
		{
			transfer(last1, first2, last2);
		}
	}

	template <typename T, typename Alloc>
	void list<T, Alloc>::reverse()
	{
		if (node->next == node || link_type(node->next)->next == node)
			return;

		iterator first = begin();
		++first;

		while (first != end())
		{
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}


	template <typename T, typename Alloc>
	void list<T, Alloc>::sort()
	{
		if (node->next == node || link_type(node->next)->next == node)
			return;

		list<T, Alloc> carry;
		list<T, Alloc> counter[64];
		int fill = 0;

		while (!empty())
		{
			carry.splice(carry.begin(), *this, begin());
			int i = 0;

			while (i < fill && !counter[i].empty())
			{
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}

			carry.swap(counter[i++]);
			if (i == fill)
			{
				++fill;
			}

			for (int i = 1; i < fill; ++i)
			{
				counter[i].merge(counter[i - 1]);

			}

			swap(counter[fill - 1]);
		}
	}

}