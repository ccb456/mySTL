#pragma once

/*
vector 实现的功能：
	iterator begin() 
	iterator end() 
	size_type size() 
	size_type capacity()
	bool empty()
	reference operator[]
	vector():start(0), finish(0), end_of_storage(0)
	vector(size_type n, const T& value)
	vector(int n, const T& value)
	vector(long n, const T& value)
	explicit vector(size_type n)
	~vector()
	reference front() { return *begin(); }
	reference back() { return *(end() - 1); }
	void push_back(const T& x)
	void pop_back()
	iterator erase(iterator position)
	iterator erase(iterator first, iterator last)
	void resize(size_type new_size, const T& x)
	void resize(size_type new_size)
	clear() { erase(begin(), end()); }
	insert(iterator position, size_type n, const T& x);

*/

#include "alloc.h"
#include <cstddef>
#include <algorithm>
#include "construct.h"
#include "uninitialized.h"
#include "algobase.h"

namespace My_STL
{
	template<typename T, typename Alloc = alloc>
	class vector
	{
	public:

		typedef T					value_type;
		typedef value_type*			pionter;
		typedef value_type*			iterator;
		typedef value_type&			reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;

	protected:
		typedef simple_alloc<value_type, Alloc> data_allocator;


	private:
		iterator start;				// 目前使用空间的头
		iterator finish;			// 表示使用空间的尾
		iterator end_of_storage;	// 表示目前可用空间的尾

		void insert_aux(iterator position, const T& x);
		void deallocate()
		{
			if (start)
			{
				data_allocator::deallocate(start, end_of_storage - start);
			}
		}

		void fill_initialize(size_type n, const T& value)
		{
			start = alloca_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

		iterator alloca_and_fill(size_type n, const T& x)
		{
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n(result, n, x);
			return result;
		}

	public:
		iterator begin() const { return start; }
		iterator end() const { return finish; }
		size_type size() const { return size_type((end() - begin())); }
		size_type capacity() const
		{
			return static_cast<size_type>(end_of_storage - begin());
		}

		bool empty() const
		{
			return begin() == end();
		}

		reference operator[] (size_type n)
		{
			return *(begin() + n);
		}


		vector():start(0), finish(0), end_of_storage(0){}
		vector(size_type n, const T& value)
		{
			fill_initialize(n, value);
		}

		vector(int n, const T& value)
		{
			fill_initialize(n, value);
		}

		vector(long n, const T& value)
		{
			fill_initialize(n, value);
		}

		explicit vector(size_type n)
		{
			fill_initialize(n, T());
		}

		~vector()
		{
			destroy(start, finish);
			deallocate();
		}

		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }
		void push_back(const T& x)
		{
			if (finish != end_of_storage)	// 还有可分配空间
			{
				construct(finish, x);
				++finish;
			}
			else
			{
				insert_aux(end(), x);
			}
		}

		void pop_back()
		{
			--finish;
			destroy(finish);
		}

		iterator erase(iterator position)	// 清除某个位置上的元素
		{
			if (position + 1 != end())	// 删除的位置不是在尾巴处
			{
				copy(position + 1, finish, position);	// 把后面的元素向前覆盖
			}

			--finish;
			destroy(finish);
			return position;
		}

		iterator erase(iterator first, iterator last)
		{
			iterator i = std::copy(last, finish, first);
			destroy(i, finish);
			finish = finish - (last - first);
			return first;
		}

		void resize(size_type new_size, const T& x)
		{
			if (new_size < size())	// 截断
			{
				erase(begin() + new_size, end());
			}
			else
			{
				insert(end(), new_size - size(), x);
			}

		}

		void resize(size_type new_size)
		{
			resize(new_size, T());
		}

		void clear() { erase(begin(), end()); }
		void insert(iterator position, size_type n, const T& x);

	};

	template <typename T, typename Alloc>
	void vector<T, Alloc>::insert_aux(iterator position, const T& x)
	{
		if (finish != end_of_storage)
		{
			// 在备用空间起始处构造一个元素，并以vector最后一个元素值为处值
			construct(finish, *(finish - 1));
			++finish;
			T x_copy = x;
			copy_backward(position, finish - 2, finish - 1);
			*position = x_copy;
		}
		else  // 已经没有备用空间了
		{
			const size_type old_size = size();
			const size_type len = old_size != 0 ? 2 * old_size : 1;

			// 实际分配内存

			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;

			try
			{
				// 把之前的内容拷贝过来
				new_finish = uninitialized_copy(start, position, new_start);

				// 将新元素设置为x
				construct(new_finish, x);
				// 调整水位
				++new_finish;

				// 将安插点后面的也拷贝过来
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch (const std::exception&)
			{
				destroy(new_start, new_finish);

				// 销毁内存
				data_allocator::deallocate(new_start, len);

				throw;
			}

			// 析构并销毁原来的vector
			destroy(begin(), end());
			deallocate();

			// 调用迭代器，指向新的迭代器
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
		}
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::insert(iterator position, size_type n, const T& x)
	{
		if (n != 0)
		{
			if (static_cast<size_type>(end_of_storage - finish) >= n)	// 备用空间够用
			{
				T x_copy = x;

				// 计算插入点之后的现有元素个数
				const size_type elems_after = finish - position;
				iterator old_finish = finish;

				if (elems_after > n)
				{
					// 插入点后的元素个数大于 新增元素个数
					uninitialized_copy(finish - n, finish, finish); // 把插入点后的元素后移n个位置
					finish += n;

					copy_backward(position, old_finish - n, old_finish);
					fill(position, old_finish, x_copy);
				}
				else
				{
					// 插入点后的元素个数 小于等于 新增元素个数
					uninitialized_fill_n(finish, n - elems_after, x_copy);
					finish += n - elems_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, x_copy);
				}
			}
			else  
			{
				// 备用空间小于 新增元素个数

				// 首先决定新长度，旧长度的2倍，或旧长度+新增长度
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);

				// 配置vector的空间
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try
				{
					// 把旧的vector的插入点之前的元素复制到新空间
					new_finish = uninitialized_copy(start, position, new_start);
					// 新元素加入
					new_finish = uninitialized_fill_n(new_finish, n, x);

					// 再将剩下的复制过来
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch (const std::exception&)
				{
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;

				}

				// 清除旧vector，并释放内存
				destroy(start, finish);
				deallocate();

				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		}
	}


}