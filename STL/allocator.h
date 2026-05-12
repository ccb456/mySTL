#pragma once
/*
	STL定义的标准分配器，只是对operator new 和 operator delete 的简单包装，
	但是实际上不能与STL内的容器搭配使用，因为目前这个代码还缺少一些东西
*/

#include <new>				// for placement new
#include <cstddef>			// for ptrdiff_t, size_t
#include <cstdlib>			// for exit()
#include <climits>			// for UINT_MAX
#include <iostream>			// for cerr

using namespace std;

namespace My_STL
{

	// 实现分配内存
	template<typename T>
	inline T* _allocate(ptrdiff_t size, T*)
	{
		set_new_handler(0);

		T* tmp = static_cast<T*>(::operator new(static_cast<size_t>(size * sizeof(T))));

		if (tmp == nullptr)
		{
			cerr << "out of memory" << endl;
			exit(1);
		}

		return tmp;

	}

	// 释放分配的内存
	template<typename T>
	inline void _deallocate(T* buffer)
	{
		::operator delete(buffer);
	}

	// 构造类型并初始化

	template <typename T1, typename T2>
	inline void _construct(T1* p, const T2& value)
	{
		// placement new 表达式
		// 在指针p所指向的内存位置上构造一个T1类型的对象，并使用value初始化该对象。
		new(p) T1(value);	
	}

	// 析构类型
	template <typename T>
	inline void _destroy(T* ptr)
	{
		ptr->~T();
	}


	template <typename T>
	class allocator
	{
	public:
		typedef T				value_type;
		typedef T*				pointer;
		typedef const T*		const_pointer;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;

		template<typename U>
		struct rebind
		{
			typedef allocator<U> other;

		};

		pointer allocate(size_type n, const void* hint = 0)
		{
			return _allocate(static_cast<difference_type>(n), static_cast<pointer>(0));
		}

		void deallocate(pointer p, size_type n)
		{
			_deallocate(p);
		}

		void construct(pointer p, const T& value)
		{
			_construct(p, value);
		}

		void destroy(pointer p)
		{
			_destroy(p);
		}

		pointer address(reference x)
		{
			return static_cast<pointer>(&x);
		}

		const_pointer const_address(const_reference x)
		{
			return static_cast<const_pointer>(&x);
		}

		size_type max_size() const
		{
			return static_cast<size_type>(UINT_MAX / sizeof(T));
		}
	};

}