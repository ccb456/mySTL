#pragma once

#include <cstddef>
#include "iterator.h"
#include "construct.h"
#include "algo.h"
#include "alloc.h"

using std::size_t;
using std::ptrdiff_t;

namespace My_STL
{
	inline size_t __deque_buf_size(size_t n, size_t sz)
	{
		return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
	}

	template <typename T, typename Ref, typename Ptr, size_t BufSize>
	struct __deque_iterator
	{
		typedef __deque_iterator<T, T&, T*, BufSize>	iterator;
		typedef __deque_iterator<T, const T&, const T*, BufSize>	const_iterator;

		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T** map_pointer;

		typedef __deque_iterator self;


		static size_t buffer_size() { return __deque_buf_size(BufSize, sizeof(T)); }


		T* cur;			// 此迭代器所指缓冲区中的现行元素
		T* first;		// 此迭代器所指指缓冲区的头
		T* last;		// 此迭代器所指缓冲区的尾
		map_pointer node;	// 指向管控中心

		void set_node(map_pointer new_node)
		{
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		reference operator*() const { return *cur; }
		pointer operator->() const { return &(operator*()); }
		difference_type operator-(const self& x) const
		{
			return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
		}

		self& operator++()
		{
			++cur;
			if (cur == last)
			{
				set_node(node + 1);
				cur = first;
			}

			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			++* this;
			return tmp;
		}

		self& operator--()
		{
			if (cur == first)
			{
				set_node(node - 1);
				cur = last;
			}

			--cur;
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			--* this;
			return tmp;
		}

		self& operator+=(difference_type n)
		{
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size()))
			{
				cur += n;
			}
			else
			{
				difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size())
					: -difference_type((-offset - 1) / buffer_size()) - 1;

				set_node(node + node_offset);

				cur = first + (offset - node_offset * difference_type(buffer_size()));
			}

			return *this;
		}

		self operator+(difference_type n) const
		{
			self tmp = *this;
			return tmp += n;
		}

		self& operator-=(difference_type n)
		{
			return *this += -n;
		}

		self operator-(difference_type n) const
		{
			self tmp = *this;
			return tmp -= n;
		}

		reference operator[] (difference_type n) const
		{
			return *(*this + n);
		}

		bool operator== (const self& x) const
		{
			return cur == x.cur;
		}

		bool operator!= (const self& x) const
		{
			return !(*this == x);
		}

		bool operator<(const self& x) const
		{
			return (node == x.node) ? (cur < x.cur) : (node < x.node);
		}

	};


	template<typename T, typename Alloc = alloc, size_t BufSize = 0>
	class deque
	{
	public:
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef size_t				size_type;
		typedef value_type&			reference;
		typedef ptrdiff_t			difference_type;
		typedef __deque_iterator<T, T&, T*, BufSize> iterator;

	protected:
		typedef simple_alloc<T, Alloc> data_allocator;			// 配置一个元素大小
		typedef simple_alloc<pointer, Alloc> map_allocator;		//每次分配一个指针大小

		static size_t buffer_size() { return __deque_buf_size(BufSize, sizeof(T)); }

	public:
		static size_type initial_map_size() { return 8; }  // 通常默认初始化为8

		deque(int n, const T& value) :start(), finish(), map(nullptr), map_size(0)
		{
			fill_initialize(n, value);
		}
		iterator begin() const { return start; }
		iterator end() const { return finish; }
		reference operator[] (size_type n)
		{
			return start[difference_type(n)];
		}

		reference front() { return *start; }
		reference back()
		{
			iterator tmp = finish;
			--tmp;
			return *tmp;
		}

		size_type size() const
		{
			return finish - start;
		}

		size_type max_size() const
		{
			return size_type(-1);
		}

		bool empty() const
		{
			return finish == start;
		}

		void push_back(const T& t)
		{
			if (finish.cur != finish.last - 1)
			{
				construct(finish.cur, t);
				++finish.cur;
			}
			else
				push_back_aux(t);
		}

		void push_front(const T& t)
		{
			if (start.cur != start.first)
			{
				construct(start.cur - 1, t);
				--start.cur;
			}
			else
				push_front_aux(t);
		}

		void pop_back()
		{
			if (finish.cur != finish.first)
			{
				--finish.cur;
				destroy(finish.cur);
			}
			else
				pop_back_aux();
		}

		void pop_front()
		{
			if (start.cur != start.last - 1)
			{
				destroy(start.cur);
				++start.cur;
			}
			else
			{
				pop_front_aux();
			}
		}

		void clear();
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);
		iterator insert(iterator position, const value_type& x);

	private:
		// 新增：分配一个缓冲区节点（可容纳buffer_size()个T类型元素）
		T* allocate_node();
		// 新增：释放一个缓冲区节点
		void deallocate_node(T* p);
		void fill_initialize(size_type n, const T& value);
		void create_map_and_node(size_type num_elements);

		void push_back_aux(const T& t);
		void push_front_aux(const T& t);
		void reserve_map_at_back(size_type nodes_to_add = 1)
		{
			if (nodes_to_add + 1 > map_size - (finish.node - map))
			{
				reallocate_map(nodes_to_add, false);
			}
		}
		void reserve_map_at_front(size_type nodes_to_add = 1)
		{
			if (nodes_to_add > start.node - map)
			{
				reallocate_map(nodes_to_add, true);
			}
		}

		void reallocate_map(size_type nodes_to_add, bool add_at_front);
		void pop_back_aux();
		void pop_front_aux();
		iterator insert_aux(iterator pos, const value_type& x);

	private:
		typedef pointer* map_pointer;

		iterator start;			// 指向第一个节点
		iterator finish;		// 指向最后一个节点的下一个位置
		map_pointer map;		// 控制中枢
		size_type map_size;		// 记录map内有多少指针
	};

	// 分配一个缓冲区节点，大小为buffer_size()个T元素
	template<typename T, typename Alloc, size_t BufSize>
	T* deque<T, Alloc, BufSize>::allocate_node()
	{
		// 利用data_allocator分配内存，每个节点可容纳buffer_size()个元素
		return data_allocator::allocate(buffer_size());
	}

	// 释放一个缓冲区节点
	template<typename T, typename Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::deallocate_node(T* p)
	{
		if (p != nullptr)
		{
			// 利用data_allocator释放内存，释放大小为buffer_size()个元素
			data_allocator::deallocate(p, buffer_size());
		}
	}

	template<typename T, typename Alloc, size_t BufSize>
	inline void deque<T, Alloc, BufSize>::clear()
	{
		for (map_pointer node = start.node + 1; node < finish.node; ++node)
		{
			destroy(*node, *node + buffer_size());
			data_allocator::deallocate(*node, buffer_size());
		}

		if (start.node != finish.node)
		{
			destroy(start.cur, start.last);
			destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first, buffer_size());
		}

		finish = start;

	}

	template<typename T, typename Alloc, size_t BufSize>
	typename deque<T, Alloc, BufSize>::iterator
	deque<T, Alloc, BufSize>::erase(iterator pos)
	{
		iterator next = pos;
		++next;
		difference_type index = pos - start;
		if (index < (size() >> 1))
		{
			copy_backward(start, pos, next);
			pop_front();
		}
		else
		{
			copy(next, finish, pos);
			pop_back();
		}

		return start + index;
	}

	template<typename T, typename Alloc, size_t BufSize>
	typename deque<T, Alloc, BufSize>::iterator
	inline deque<T, Alloc, BufSize>::erase(iterator first, iterator last)
	{
		if (first == start && last == finish)
		{
			clear();
			return finish;
		}
		else
		{
			difference_type n = last - first;
			difference_type elems_before = first - start;
			if (elems_before < (size() - n) / 2)
			{
				copy_backward(start, first, last);
				iterator new_start = start + n;
				destroy(start, new_start);

				for (map_pointer cur = start.node; cur < new_start.node; ++cur)
				{
					data_allocator::deallocate(*cur, buffer_size());
				}

				start = new_start;
			}
			else
			{
				copy(last, finish, first);
				iterator new_finish = finish - n;
				destroy(new_finish, finish);

				for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
				{
					data_allocator::deallocate(*cur, buffer_size());
				}
				finish = new_finish;
			}

			return start + elems_before;
		}
	}

	template<typename T, typename Alloc, size_t BufSize>
	typename deque<T, Alloc, BufSize>::iterator
	inline deque<T, Alloc, BufSize>::insert(iterator position, const value_type& x)
	{
		if (position.cur == start.cur)
		{
			push_front(x);
			return start;
		}
		else if (position.cur == finish.cur)
		{
			push_back(x);
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else
		{
			return insert_aux(position, x);
		}
	}

	template <typename T, typename Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::fill_initialize(size_type n, const T& value)
	{
		create_map_and_node(n);
		map_pointer cur;

		try
		{
			for (cur = start.node; cur < finish.node; ++cur)
			{
				uninitialized_fill(*cur, *cur + buffer_size(), value);

				uninitialized_fill(finish.first, finish.cur, value);
			}
		}
		catch (const std::exception&)
		{
			throw;
		}
	}

	template <typename T, typename Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::create_map_and_node(size_type num_elements)
	{
		// 需要节点数 = (元素个数 / 每个缓冲区可容纳的元素个数) + 1 
		size_type num_nodes = num_elements / buffer_size() + 1;

		map_size = max(initial_map_size(), num_nodes + 2);
		map = map_allocator::allocate(map_size);			// 分配内存

		// 灵nstart 和 nfinish 指向map所拥有的全部节点的最中央位置
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;

		map_pointer cur = nullptr;

		try
		{
			for (cur = nstart; cur <= nfinish; ++cur)
			{
				*cur = allocate_node();  // 分配节点的内存
			}
		}
		catch (const std::exception&)
		{
			throw;
		}

		start.set_node(nstart);
		finish.set_node(nfinish);

		start.cur = start.first;
		finish.cur = finish.first + num_elements % buffer_size();

	}

	template<typename T, typename Alloc, size_t BufSize>
	inline void deque<T, Alloc, BufSize>::push_back_aux(const T& t)
	{
		value_type t_copy = t;
		reserve_map_at_back();		// 判断四否需要扩充map

		*(finish.node + 1) = allocate_node();
		try
		{
			construct(finish.cur, t_copy);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		catch (const std::exception&)
		{
			deallocate_node(*(finish.node + 1));
			throw;
		}
	}

	template<typename T, typename Alloc, size_t BufSize>
	inline void deque<T, Alloc, BufSize>::push_front_aux(const T& t)
	{
		T t_copy = t;
		reserve_map_at_front();
		*(start.node - 1) = allocate_node();
		try
		{
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			construct(start.cur, t_copy);
		}
		catch (const std::exception&)
		{
			start.set_node(start.node + 1);
			start.cur = start.first;
			deallocate_node(*(start.node - 1));
			throw;
		}
	}

	template<typename T, typename Alloc, size_t BufSize>
	inline void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front)
	{
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;

		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes)
		{
			new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);

			if (new_nstart < start.node)
			{
				copy(start.node, finish.node + 1, new_nstart);
			}
			else
			{
				copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
			}
		}
		else
		{
			size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;

			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);

			copy(start.node, finish.node + 1, new_nstart);
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}

		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}

	template<typename T, typename Alloc, size_t BufSize>
	inline void deque<T, Alloc, BufSize>::pop_back_aux()
	{
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}

	template<typename T, typename Alloc, size_t BufSize>
	inline void deque<T, Alloc, BufSize>::pop_front_aux()
	{
		destroy(start.cur);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}

	template<typename T, typename Alloc, size_t BufSize>
	typename deque<T, Alloc, BufSize>::iterator
	inline deque<T, Alloc, BufSize>::insert_aux(iterator pos, const value_type& x)
	{
		difference_type index = pos - start;
		value_type x_copy = x;
		if (index < size() / 2)
		{
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;

			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			copy(front2, pos1, front1);
		}
		else
		{
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;
			copy_backward(pos, back2, back1);
		}

		*pos = x_copy;
		return pos;
	}


}