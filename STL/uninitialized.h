#pragma once

#include "construct.h"
#include <cstring>
#include "algobase.h"
#include "type_traits.h"

namespace My_STL
{
	// uninitialized_fill_n 一组函数
	template <typename ForwardIterator, typename Size, typename T, typename T1>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*)
	{
		typedef typename __type_traits<T1>::is_POD_type is_POD;

		return __uninitialized_fill_n_aux(first, n, x, is_POD());
	}

	template <typename ForwardIterator, typename Size, typename T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type)
	{


		return fill_n(first, n, x);
	}

	template <typename ForwardIterator, typename Size, typename T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type)
	{

		ForwardIterator cur = first;

		for (; n > 0; --n, ++cur)
		{
			construct(&*cur, x);
		}

		return cur;
	}
	template <typename ForwardIterator, typename Size, typename T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
	{
		return __uninitialized_fill_n(first, n, x, value_type(first));
	}


	// uninitialized_copy 一组函数

	template <typename InputIterator, typename ForwardIterator, typename T>
	inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
	{
		typedef typename __type_traits<T>::is_POD_type is_POD;
		return __uninitialized_copy_aux(first, last, result, is_POD());
	}

	template <typename InputIterator, typename ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
	{
		return copy(first, last, result);
	}

	template <typename InputIterator, typename ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
	{
		ForwardIterator cur = result;

		for (; first != last; ++first, ++cur)
		{
			construct(&*cur, *first);
		}

		return cur;
	}

	template <typename InputIterator, typename ForwardIterator>
	inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
	{
		return __uninitialized_copy(first, last, result, value_type(result));
	}

	// 对char* 和wchar_t* 做特化版本
	inline char* uninitialized_copy(const char* first, const char* last, char* result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
	{
		memmove(result, first, (last - first) * sizeof(wchar_t));
		return result + (last - first);
	}

	// uninitialized_fill 的一组函数

	template <typename ForwardIterator, typename T, typename T1>
	inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*)
	{
		typedef typename __type_traits<T1>::is_POD_type is_POD;

		__uninitialized_fill_aux(first, last, x, is_POD());
	}

	template <typename ForwardIterator, typename T>
	inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type)
	{

		fill(first, last, x);
	}

	template <typename ForwardIterator, typename T>
	inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type)
	{

		ForwardIterator cur = first;

		for (; cur != last; ++cur)
		{
			construct(&*cur, x);
		}

	}

	template <typename ForwardIterator, typename T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
	{
		__uninitialized_fill(first, last, x, value_type(first));
	}








}

