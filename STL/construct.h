#pragma once

#include <new>
#include "type_traits.h"
#include "iterator.h"

namespace My_STL
{

	// 构造类型并初始化

	template <typename T1, typename T2>
	inline void construct(T1* p, const T2& value)
	{
		// placement new 表达式
		// 在指针p所指向的内存位置上构造一个T1类型的对象，并使用value初始化该对象。
		new(p) T1(value);
	}

	// 析构类型
	template <typename T>
	inline void destroy(T* ptr)
	{
		ptr->~T();

	}
	template <typename ForwardIterator, typename T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
	{
		typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
		__destroy_aux(first, last, trivial_destructor());
	}

	template <typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
	{
		for (; first < last; ++first)
		{
			destroy(&*first);
		}
	}

	template <typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type)
	{}

	template <typename ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		__destroy(first, last, value_type(first));
	}

	inline void destroy(char*, char*){}
	inline void destroy(wchar_t*, wchar_t*){}

}