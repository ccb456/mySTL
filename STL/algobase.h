#pragma once
/*
包含基本算法
*/

#include "iterator.h"
#include "type_traits.h"
#include <utility>  // pai 
#include <cstring> // memmove

namespace My_STL
{
	// equal : 必须保证两个容器拥有相同的大小
	template <typename InputIterator1, typename InputIterator2>
	inline bool equal(InputIterator1 first1, InputIterator1 last, InputIterator2 first2)
	{
		for (; first1 != last; ++first1, ++first2)
		{
			if (*first1 != *first2)
				return false;

		}
		return true;
	}

	template <typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
	inline bool equal(InputIterator1 first1, InputIterator1 last, InputIterator2 first2, BinaryPredicate binary_pred)
	{
		for (; first1 != last; ++first1, ++first2)
		{
			if (!binary_pred(*first1, *first2))
				return false;

		}
		return true;
	}

	// fill
	template<typename ForwardIterator, typename T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		for (; first != last; ++first)
		{
			*first = value;
		}
	}

	// fill_n: 将前n个元素改填新值
	template<typename OutputIterator, typename Size, typename T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		for (; n > 0; --n, ++first)
		{
			*first = value;
		}

		return first;
	}

	// iter_swap
	template<typename ForwardIterator1, typename ForwardIterator2>
	inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
	{
		__iter_swap(a, b, value_type(a));
	}

	template<typename ForwardIterator1, typename ForwardIterator2, typename T>
	inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*)
	{
		T tmp = *a;
		*a = *b;
		*b = tmp;
	}

	// lexicographical_compare
	template<typename InputIterator1, typename InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (*first1 < *first2)
			{
				return true;
			}

			if (*first2 < *first1)
			{
				return false;
			}
		}

		return first1 == last1 && first2 != last2;
	}

	template<typename InputIterator1, typename InputIterator2, typename Compare>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (comp(*first1, *first2))
			{
				return true;
			}

			if (comp(*first2, *first1))
			{
				return false;
			}
		}

		return first1 == last1 && first2 != last2;
	}

	// max
	template<typename T>
	inline const T& max(const T& a, const T& b)
	{
		return a < b ? b : a;
	}
	template<typename T, typename Compare>
	inline const T& max(const T& a, const T& b, Compare comp)
	{
		return comp(a,b) ? b : a;
	}

	// min
	template<typename T>
	inline const T& min(const T& a, const T& b)
	{
		return b < a ? b : a;
	}
	template<typename T, typename Compare>
	inline const T& min(const T& a, const T& b, Compare comp)
	{
		return comp(b, a) ? b : a;
	}

	// mismatch: 用来平行比较两个序列，用来指出第一个不匹配点
	template<typename InputIterator1, typename InputIterator2>
	std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		while (first1 != last1 && *first1 == *first2)
		{
			++first1;
			++first2;
		}

		return std::pair< InputIterator1, InputIterator2>(first1, first2);

	}

	template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
	std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, BinaryPredicate binary_pred)
	{
		while (first1 != last1 && binary_pred(*first1, *first2))
		{
			++first1;
			++first2;
		}

		return std::pair< InputIterator1, InputIterator2>(first1, first2);

	}

	// swap
	template<typename T>
	inline void swap(T& a, T& b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}

	// copy

	template<typename InputIterator, typename OutputIterator>
	struct __copy_dispatch
	{
		OutputIterator operator() (InputIterator first, InputIterator last, OutputIterator result)
		{
			return __copy(first, last, result);
		}
	};

	template<typename T>
	struct __copy_dispatch<T*, T*>
	{
		T* operator()(T* first, T* last, T* result)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};

	template<typename T>
	struct __copy_dispatch<const T*, T*>
	{
		T* operator()(const T* first, const T* last, T* result)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};

	template<typename InputIterator, typename OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		return __copy_dispatch<InputIterator, OutputIterator>()
			(first, last, result);
	}

	inline char* copy(const char* first, const char* last, char* result)
	{
		std::memmove(result, first, last - first);
		return result + (last - first);
	}
	inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
	{
		std::memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}



	template<typename InputIterator, typename OutputIterator>
	inline OutputIterator __copy(InputIterator first, InputIterator last, 
			OutputIterator result, input_iterator_tag)
	{
		for (; first != last; ++result, ++first)
		{
			*result = *first;
		}
		return result;
	}

	template<typename RandomAccessIterator, typename OutputIterator>
	inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, random_access_iterator_tag)
	{
		return __copy_d(first, last, result, distance_type(first));
	}

	template<typename RandomAccessIterator, typename OutputIterator, typename Distance>
	inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, Distance*)
	{
		for (Distance n = last - first; n > 0; --n, ++result, ++first)
		{
			*result = *first;
		}

		return result;
	}

	template<typename T>
	inline T* __copy_t(const T* first, const T* last, T* result, __true_type)
	{
		memmove(result, first, sizeof(T) * (last - first));

		return result + (last - first);
	}

	template<typename T>
	inline T* __copy_t(const T* first, const T* last, T* result, __false_type)
	{
	
		return __copy_d(first, last, result, (ptrdiff_t*)0);
	}

	// copy_backward

	// 辅助函数：针对一般迭代器的copy_backward实现（从后往前依次复制）
	template <typename BidirectionalIterator1, typename BidirectionalIterator2>
	BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result,
											bidirectional_iterator_tag) 
	{
		// 从源范围的末尾开始向前复制，直到源范围的起始位置
		while (first != last) {
			*(--result) = *(--last);
		}
		return result;
	}

	// 辅助函数：针对随机访问迭代器的优化实现（利用指针算术提升效率）
	template <typename RandomAccessIterator1, typename RandomAccessIterator2>
	RandomAccessIterator2 __copy_backward(RandomAccessIterator1 first,
										RandomAccessIterator1 last,
										RandomAccessIterator2 result,
										random_access_iterator_tag) 
	{
		// 计算元素数量
		typename std::iterator_traits<RandomAccessIterator1>::difference_type n = last - first;
		// 直接定位到目标起始位置，从后往前批量复制
		while (n > 0) {
			*(--result) = *(--last);
			--n;
		}
		return result - n; // n已减为0，等价于result
	}

	// 对外接口：自动根据迭代器类型选择最优实现
	template <typename BidirectionalIterator1, typename BidirectionalIterator2>
	BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
										BidirectionalIterator1 last,
										BidirectionalIterator2 result) 
	{
		// 通过迭代器特性获取迭代器类别，分发到对应实现
		typedef typename iterator_traits<BidirectionalIterator1>::iterator_category category;
		return __copy_backward(first, last, result, category());
	}

	/*
	=========== set 集合操作 ========================== =
	 1. 操作的集合必须是有序的
	*/ 

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
							InputIterator2 first2, InputIterator2 last2,
							OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				*result = *first1;
				++first1;
			}
			else if (*first2 < *first1)
			{
				*result = *first2;
				++first2;
			}
			else
			{
				*result = *first1;
				++first1;
				++first2;
			}

			++result;
		}

		return copy(first2, last2, copy(first1, last1, result));
	}
	template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
									InputIterator2 first2, InputIterator2 last2,
									OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				++first1;
			}
			else if (*first2 < *first1)
			{
				++first2;
			}
			else
			{
				*result = *first1;
				++first1;
				++first2;
				++result;
			}

		}

		return result;

	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
								InputIterator2 first2, InputIterator2 last2,
								OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				*result = *first1;
				++first1;
				++result;
			}
			else if (*first2 < *first1)
			{
				++first2;
			}
			else
			{
				++first1;
				++first2;
			}

		}

		return copy(first1, last1, result);

	}

	// (S1 - S2) U (S2 - S1)
	template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
											InputIterator2 first2, InputIterator2 last2,
											OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				*result = *first1;
				++first1;
				++result;
			}
			else if (*first2 < *first1)
			{
				*result = *first2;
				++first2;
				++result;
			}
			else
			{
				++first1;
				++first2;
			}

		}

		return copy(first2, last2, copy(first1, last1, result));

	}

	/*
	======================== set 集合操作 end ======================
	*/



}