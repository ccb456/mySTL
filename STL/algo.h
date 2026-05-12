#pragma once

/*
	其它算法：

*/
#include "iterator.h"
#include "type_traits.h"
#include "algobase.h"

namespace My_STL
{
	/*
	adjacent_find: 找出第一组满足条件的相邻元素
	*/
	template<typename ForwardIterator>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last)
	{
		if (first == last) return last;
		ForwardIterator next = first;

		while (++next != last)
		{
			if (*first == *next) return first;
			first = next;
		}

		return last;
	}
	template<typename ForwardIterator, typename BinaryPredicate>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred)
	{
		if (first == last) return last;
		ForwardIterator next = first;

		while (++next != last)
		{
			if (binary_pred(*first, *next)) return first;
			first = next;
		}

		return last;
	}

	// count : 返回与元素值相等的的元素数量
	template<typename InputIterator, typename T>
	typename iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, const T& value)
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first)
		{
			if (*first == value)
				++n;
		}

		return n;
	}

	// count_if
	template<typename InputIterator, typename Predicate>
	typename iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, Predicate pred)
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first)
		{
			if (pred(*first))
				++n;
		}

		return n;
	}

	// find
	template<typename InputIterator, typename T>
	InputIterator find(InputIterator first, InputIterator last, const T& value)
	{
		while (first != last && *first != value) ++first;
		return first;
	}

	// find_if
	template<typename InputIterator, typename Predicate>
	InputIterator find_if(InputIterator first, InputIterator last, Predicate pred)
	{
		while (first != last && !pred(*first)) ++first;

		return first;
	}

	// find_end ： 在[first1, last1] 中查找[first2, last2]最后一次出现的点
	template <typename ForwardIterator1, typename ForwardIterator2>
	inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2)
	{
		typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
		typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;

		return __find_end(first1, last1, first2, last2, category1(), category2());
	}

	template <typename ForwardIterator1, typename ForwardIterator2>
	inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		forward_iterator_tag, forward_iterator_tag)
	{
		if (first2 == last2) return last1;
		else
		{
			ForwardIterator1 result = last1;
			while (true)
			{
				ForwardIterator1 new_result = search(first1, last1, first2, last2);
				if (new_result == last1)
					return result;
				else
				{
					result = new_result;
					first1 = new_result;
					++first1;
				}
			}

		}
	}

	template <typename BidirectionIterator1, typename BidirectionIterator2>
	BidirectionIterator1 __find_end(BidirectionIterator1 first1, BidirectionIterator1 last1,
		BidirectionIterator2 first2, BidirectionIterator2 last2,
		bidirectional_iterator_tag, bidirectional_iterator_tag)
	{
		typedef reverse_iterator<BidirectionIterator1> reviter1;
		typedef reverse_iterator<BidirectionIterator2> reviter2;

		reviter1 rlast1(first1);
		reviter2 rlast2(first2);

		reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);

		if (rresult == rlast1) return last1;
		else
		{
			BidirectionIterator1 result = rresult.base();
			advance(result, -distance(first2, last2));
			return result;
		}

	}

	// 支持自定义比较运算的find_end版本
	template <typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
	inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		BinaryPredicate pred)
	{
		typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
		typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;

		return __find_end(first1, last1, first2, last2, pred, category1(), category2());
	}

	// 前向迭代器版本，使用自定义比较函数
	template <typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
	inline ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		BinaryPredicate pred,
		forward_iterator_tag, forward_iterator_tag)
	{
		if (first2 == last2) return last1;
		else
		{
			ForwardIterator1 result = last1;
			while (true)
			{
				// 使用接受比较函数的search版本
				ForwardIterator1 new_result = search(first1, last1, first2, last2, pred);
				if (new_result == last1)
					return result;
				else
				{
					result = new_result;
					first1 = new_result;
					++first1;
				}
			}
		}
	}

	// 双向迭代器版本，使用自定义比较函数
	template <typename BidirectionIterator1, typename BidirectionIterator2, typename BinaryPredicate>
	BidirectionIterator1 __find_end(BidirectionIterator1 first1, BidirectionIterator1 last1,
		BidirectionIterator2 first2, BidirectionIterator2 last2,
		BinaryPredicate pred,
		bidirectional_iterator_tag, bidirectional_iterator_tag)
	{
		typedef reverse_iterator<BidirectionIterator1> reviter1;
		typedef reverse_iterator<BidirectionIterator2> reviter2;

		reviter1 rlast1(first1);
		reviter2 rlast2(first2);

		// 使用接受比较函数的search版本
		reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2, pred);

		if (rresult == rlast1) return last1;
		else
		{
			BidirectionIterator1 result = rresult.base();
			advance(result, -distance(first2, last2));
			return result;
		}
	}

	// find_first_of : 以[first2, last2]区间内的元素作为查找目标，寻找在[first1,last1]中第一次出现的地点
	template <typename InputIterator, typename ForwardIterator>
	InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2)
	{
		for (; first1 != last1; ++first1)
		{
			for (ForwardIterator iter = first2; iter != last2; ++iter)
			{
				if (*first1 == *iter)
					return first1;
			}
		}

		return last1;
	}

	template <typename InputIterator, typename ForwardIterator, typename BinaryPredicate>
	InputIterator find_first_of(InputIterator first1, InputIterator last1,
		ForwardIterator first2, ForwardIterator last2,
		BinaryPredicate comp)
	{
		for (; first1 != last1; ++first1)
		{
			for (ForwardIterator iter = first2; iter != last2; ++iter)
			{
				if (comp(*first1, *iter))
					return first1;
			}
		}

		return last1;
	}

	// for_each : 将仿函数f施于[first, last]区间内的每一个元素身上，f不可以改变元素内容
	template <typename InputIterator, typename Function>
	Function for_each(InputIterator first, InputIterator last, Function f)
	{
		for (; first != last; ++first)
		{
			f(*first);
		}

		return f;
	}

	// generate : 将仿函数gen的运算结果填写在[first, last]区间内的所有元素身上
	template <typename ForwardIterator, typename Generator>
	void generate(ForwardIterator first, ForwardIterator last, Generator gen)
	{
		for (; first != last; ++first)
		{
			*first = gen();
		}
	}

	template <typename OutputIterator, typename Size, typename Generator>
	OutputIterator generate_n(OutputIterator first, Size n, Generator gen)
	{
		for (; n > 0; --n, ++first)
		{
			*first = gen();
		}

		return first;
	}

	// includes : 对有序集合操作，序列二是否涵盖与序列一
	template <typename InputIterator1, typename InputIterator2>
	bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *first1)
			{
				return false;
			}
			else if (*first1 < *first2)
			{
				++first1;
			}
			else
			{
				++first1;
				++first2;
			}
		}

		return first2 == last2;
	}

	template <typename InputIterator1, typename InputIterator2, typename Compare>
	bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (comp(*first2, *first1))
			{
				return false;
			}
			else if (comp(*first1 < *first2))
			{
				++first1;
			}
			else
			{
				++first1;
				++first2;
			}
		}

		return first2 == last2;
	}

	// max_element : 返回序列中数值最大的元素
	template<typename ForwardIterator>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
	{
		if (first == last) return first;
		ForwardIterator result = first;
		while (++first != last)
		{
			if (*result < *first)
			{
				result = first;
			}
		}

		return result;
	}

	template<typename ForwardIterator, typename Compare>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp)
	{
		if (first == last) return first;
		ForwardIterator result = first;
		while (++first != last)
		{
			if (comp(*result, *first))
			{
				result = first;
			}
		}

		return result;
	}

	// merge : 将两个经过排序的集合S1和S2，合并起来置于另一段空间
	template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *first1)
			{
				*result = *first2;
				++first2;
			}
			else
			{
				*result = *first1;
				++first1;
			}

			++result;
		}

		return copy(first2, last2, copy(first1, last1, result));
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, Compare comp)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (comp(*first2, *first1))
			{
				*result = *first2;
				++first2;
			}
			else
			{
				*result = *first1;
				++first1;
			}

			++result;
		}

		return copy(first2, last2, copy(first1, last1, result));
	}

	// min_element
	template<typename ForwardIterator>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last)
	{
		if (first == last) return first;
		ForwardIterator result = first;
		while (++first != last)
		{
			if (*first < *result)
			{
				result = first;
			}
		}

		return result;
	}

	template<typename ForwardIterator, typename Compare>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp)
	{
		if (first == last) return first;
		ForwardIterator result = first;
		while (++first != last)
		{
			if (comp(*first, *result))
			{
				result = first;
			}
		}

		return result;
	}

	// partition
	template <typename BidirectionalIterator, typename Predicate>
	BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred)
	{
		while (true)
		{
			while (true)
			{
				if (first == last) return first;
				else if (pred(*first))
				{
					++first;
				}
				else
				{
					break;
				}
			}

			--last;
			while (true)
			{
				if (first == last) return first;
				else if (!pred(*last))
				{
					--last;
				}
				else
					break;
			}

			iter_swap(first, last);
			++first;
		}
	}

	//  remove : 移除所有与value相等的元素
	template <typename ForwardIterator, typename T>
	ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value)
	{
		first = find(first, last, value);
		ForwardIterator next = first;
		return first == last ? first : remove_copy(++next, last, first, value);
	}

	// remove_copy
	template<typename InputIterator, typename OutputIterator, typename T>
	OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& value)
	{
		for (; first != last; ++first)
		{
			if (*first != value)
			{
				*result = *first;
				++result;
			}
		}

		return result;
	}

	template<typename ForwardIterator, typename Predicate>
	ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred)
	{
		first = find_if(first, last, pred);
		ForwardIterator next = first;
		return first == last ? first : remove_copy_if(++next, last, first, pred);

	}

	template<typename InputIterator, typename OutputIterator, typename Predicate>
	OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
	{
		for (; first != last; ++first)
		{
			if (!pred(*first))
			{
				*result = *first;
				++result;
			}
		}

		return result;
	}

	// replace
	template <typename ForwardIterator, typename T>
	void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value)
	{
		for (; first != last; ++first)
		{
			if (*first == old_value)
				*first = new_value;
		}
	}

	// replace_copy
	template <typename InputIterator, typename OutputIterator, typename T>
	OutputIterator replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value)
	{
		for (; first != last; ++first, ++result)
		{
			*result = *first == old_value ? new_value : *first;
		}

		return result;
	}

	// replace_if
	template <typename ForwardIterator, typename T, typename Predicate>
	void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred, const T& new_value)
	{
		for (; first != last; ++first)
		{
			if (pred(*first))
				*first = new_value;
		}
	}

	// replace_copy_if
	template <typename ForwardIterator, typename T, typename Predicate, typename OutputIterator>
	OutputIterator replace_copy_if(ForwardIterator first, ForwardIterator last, OutputIterator result, Predicate pred, const T& new_value)
	{
		for (; first != last; ++first, ++result)
		{
			*result = pred(*first) ? new_value : *first;
		}
	}

	// reverse
	template <typename BidirectionalIterator>
	inline void reverse(BidirectionalIterator first, BidirectionalIterator last)
	{
		__reverse(first, last, iterator_category(first));
	}
	template <typename BidirectionalIterator>
	void __reverse(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag)
	{
		while (true)
		{
			if (first == last || first == -last) return;
			else
			{
				iter_swap(first++, last);
			}
		}
	}

	template<typename RandomAccessIterator>
	void __reverse(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
	{
		while (first < last)
		{
			iter_swap(first++, --last);
		}
	}

	// reverse_copy
	template <typename BidirectionalIterator, typename OutputIterator>
	OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result)
	{
		while (first != last)
		{
			--last;
			*result = *last;
			++result;
		}

		return result;
	}

	//rotate
	template <typename ForwardIterator>
	inline void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last)
	{
		if (first == middle || middle == last) return;
		__rotate(first, middle, last, distance_type(first), iterator_category(first));
	}

	template<typename ForwardIterator, typename Distance>
	void __rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, Distance*, forward_iterator_tag)
	{
		for (ForwardIterator i = middle; ;)
		{
			iter_swap(first, i);
			++first;
			++i;

			if (first == middle)
			{
				if (last == i) return;

				middle = i;
			}
			else if (i == last)
			{
				i = middle;
			}
		}
	}

	template<typename BidirectionalIterator, typename Distance>
	void __rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance*, bidirectional_iterator_tag)
	{
		reverse(first, middle);
		reverse(middle, last);
		reverse(first, last);
	}

	template<typename RandomAccessIterator, typename Distance>
	void __rotate(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Distance*, random_access_iterator_tag)
	{
		Distance n = __gcd(last - first, middle - first);
		while (n--)
		{
			__rotate_cycle(first, last, first + n, middle - first, value_type(first));
		}
	}

	// __gcd
	template<typename EuclideanRingElement>
	EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n)
	{
		while (n != 0)
		{
			EuclideanRingElement t = m % n;
			m = n;
			n = t;
		}

		return m;
	}

	// __rptate_cycle
	template<typename RandomAccessIterator, typename Distance, typename T>
	void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator initial, Distance shift, T*)
	{
		T value = *initial;
		RandomAccessIterator ptr1 = initial;
		RandomAccessIterator ptr2 = ptr1 + shift;

		while (ptr2 != initial)
		{
			*ptr1 = *ptr2;
			ptr1 = ptr2;

			if (last - ptr2 > shift)
			{
				ptr2 += shift;
			}
			else
			{
				ptr2 = first + (shift - (last - ptr2));
			}
		}
		*ptr1 = value;
	}

	// rotate_copy
	template <typename ForwardIterator, typename OutputIterator>
	OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result)
	{
		return copy(first, middle, copy(middle, last, result));
	}

	// search
	template <typename ForwardIterator1, typename ForwardIterator2>
	inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2)
	{
		return __search(first1, last1, first2, last2, distance_type(first1), distance_type(first2));
	}
	template <typename ForwardIterator1, typename ForwardIterator2, typename Distance1, typename Distance2>
	inline ForwardIterator1 __search(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2, Distance1*, Distance2*)
	{
		Distance1 d1 = 0;
		distance(first1, last1, d1);
		Distance2 d2 = 0;
		distance(first2, last2, d2);

		if (d1 < d2) return last1;
		ForwardIterator1 current1 = first1;
		ForwardIterator2 current2 = first2;

		while (current2 != last2)
		{
			if (*current1 == *current2)
			{
				++current1;
				++current2;
			}
			else
			{
				if (d1 == d2) return last1;
				else
				{
					current1 = ++first1;
					current2 = first2;
					--d1;
				}
			}
		}

		return first1;
	}

	// search_n
	template <typename ForwardIterator, typename Integer, typename T>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer count, const T& value)
	{
		if (count < 0)	return first;
		else
		{
			first = find(first, last, value);

			while (first != last)
			{
				Integer n = count - 1;
				ForwardIterator i = first;
				++i;

				while (i != last && n != 0 && *i == value)
				{
					++i;
					--n;
				}

				if (n == 0)
				{
					return first;
				}
				else
				{
					first = find(i, last, value);
				}
			}

			return last;
		}
	}

	template<typename ForwardIterator, typename Integer, typename T, typename BinaryPredicate>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Integer count, const T& value, BinaryPredicate binary_pred)
	{
		if (count <= 0) return first;
		else
		{
			while (first != last)
			{
				if (binary_pred(*first, value)) break;
				++first;
			}

			while (first != last)
			{
				Integer n = count - 1;
				ForwardIterator i = first;
				++i;

				while (i != last && n != 0 && binary_pred(*i, value))
				{
					++i;
					++n;
				}

				if (n == 0) return first;
				else
				{
					while (i != last)
					{
						if (binary_pred(*i, value)) break;
						++i;
					}
					first = i;
				}
			}

			return last;
		}
	}

	// swap_range
	template <typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator2 swap_range(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			iter_swap(first1, first2);
		}

		return first2;
	}

	// transform
	template <typename InputIterator, typename OutputIterator, typename UnaryOperation>
	OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation op)
	{
		for (; first != last; ++first, ++result)
		{
			*result = op(*first);
		}

		return result;
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryOperation>
	OutputIterator transform(InputIterator1 first1, InputIterator1 last1, 
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, BinaryOperation binary_op)
	{
		for (; first1 != last1; ++first1, ++first2, ++result)
		{
			*result = binary_op(*first1, *first2);
		}

		return result;
	}

	// unique
	template <typename ForwardIterator>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last)
	{
		first = adjacent_find(first, last);
		return unique_copy(first, last, first);
	}

	template <typename InputIterator, typename OutputIterator>
	inline OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		if (first == result) return result;

		return __unique_copy(first, last, result, iterator_category(result));
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator __unique_copy(InputIterator first, InputIterator last, OutputIterator result, forward_iterator_tag)
	{
		*result = *first;
		while (++first != last)
		{
			if (*result != *first) *++result = *first;
		}

		return ++result;
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator __unique_copy(InputIterator first, InputIterator last, OutputIterator result, output_iterator_tag)
	{

		return __unique_copy(first, last, result, value_type(first));
	}
	template <typename InputIterator, typename OutputIterator, typename T>
	OutputIterator __unique_copy(InputIterator first, InputIterator last, OutputIterator result, T*)
	{
		T value = *first;
		*result = value;

		while (++first != last)
		{
			if (value != *first)
			{
				value = *first;
				*++result = value;
			}
		}

		return ++ + result;	
	}

	// lower_bound(应用于有序区间)
	template <typename ForwardIterator, typename T>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value)
	{
		return __lower_bound(first, last, value, distance_type(first), iterator_category(first));
	}

	template <typename ForwardIterator, typename T, typename Compare>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp)
	{
		return __lower_bound(first, last, value, comp, distance_type(first), iterator_category(first));
	}

	template <typename ForwardIterator, typename T, typename Distance>
	ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag)
	{
		Distance len = 0;
		distance(first, last, len);
		Distance half;
		ForwardIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			advance(middle, half);

			if (*middle < value)
			{
				first = middle;
				++first;
				len = len - half - 1;
			}
			else
			{
				len = half;
			}
		}

		return first;
	}

	template <typename RandomAccessIterator, typename T, typename Distance>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Distance*, random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first + half;

			if (*middle < value)
			{
				first = middle + 1;
				len = len - half - 1;
			}
			else
			{
				len = half;
			}
		}

		return first;
	}

	// 版本二
	template <typename ForwardIterator, typename T, typename Distance, typename Compare>
	ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp, Distance*, forward_iterator_tag)
	{
		Distance len = 0;
		distance(first, last, len);
		Distance half;
		ForwardIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			advance(middle, half);

			if (comp(*middle, value))
			{
				first = middle;
				++first;
				len = len - half - 1;
			}
			else
			{
				len = half;
			}
		}

		return first;
	}

	template <typename RandomAccessIterator, typename T, typename Distance, typename Compare>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Compare comp, Distance*, random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first + half;

			if (comp(*middle, value))
			{
				first = middle + 1;
				len = len - half - 1;
			}
			else
			{
				len = half;
			}
		}

		return first;
	}

	// upper_bound(应用于有序区间)
	template <typename ForwardIterator, typename T>
	inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value)
	{
		return __upper_bound(first, last, value, distance_type(first), iterator_category(first));
	}
	template <typename ForwardIterator, typename T, typename Compare>
	inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp)
	{
		return __upper_bound(first, last, value, comp, distance_type(first), iterator_category(first));
	}

	template <typename ForwardIterator, typename T, typename Distance>
	inline ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag)
	{
		Distance len = 0;
		distance(first, last, len);
		Distance half;
		ForwardIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			advance(middle, half);

			if (value < *middle) len = half;
			else
			{
				first = middle;
				++first;
				len = len - half - 1;
			}
		}

		return first;
	}

	template <typename RandomAccessIterator, typename T, typename Distance>
	RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Distance*, random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first + half;

			if (value < *middle)
			{
				len = half;
			}
			else
			{
				first = middle + 1;
				len = len - half - 1;
			}
		}

		return first;
	}


	template <typename ForwardIterator, typename T, typename Distance, typename Compare>
	inline ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp ,Distance*, forward_iterator_tag)
	{
		Distance len = 0;
		distance(first, last, len);
		Distance half;
		ForwardIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			advance(middle, half);

			if (comp(value , *middle)) len = half;
			else
			{
				first = middle;
				++first;
				len = len - half - 1;
			}
		}

		return first;
	}

	template <typename RandomAccessIterator, typename T, typename Distance, typename Compare>
	RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Compare comp, Distance*, random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first + half;

			if (comp(value ,*middle))
			{
				len = half;
			}
			else
			{
				first = middle + 1;
				len = len - half - 1;
			}
		}

		return first;
	}

	// binary_search(应用于有序区间)
	template <typename ForwardIterator, typename T>
	bool binary_search(ForwardIterator first, ForwardIterator last, const T& value)
	{
		ForwardIterator i = lower_bound(first, last, value);
		return i != last && !(value < *i);
	}

	template <typename ForwardIterator, typename T, typename Compare>
	bool binary_search(ForwardIterator first, ForwardIterator last, const T& value, Compare comp)
	{
		ForwardIterator i = lower_bound(first, last, value, comp);
		return i != last && !comp(value, *i);
	}

	// next_permutation
	template <typename BidirectionalIterator>
	bool next_permutation(BidirectionalIterator first, BidirectionalIterator last)
	{
		if (first == last) return false;
		BidirectionalIterator i = first;
		++i;
		if (i == last) return false;
		i = last;
		--i;

		for (;;)
		{
			BidirectionalIterator ii = i;
			--i;
			if (*i < *ii)
			{
				BidirectionalIterator j = last;
				while (!(*i < *--j));
				iter_swap(i, j);
				reverse(ii, last);
				return true;
			}

			if (i == first)
			{
				reverse(first, last);
				return false;
			}
		}

	}

	//prev_permutation
	template <typename BidirectionalIterator>
	bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last)
	{
		if (first == last) return false;
		BidirectionalIterator i = first;
		++i;
		if (i == last) return false;
		i = last;
		--i;

		for (;;)
		{
			BidirectionalIterator ii = i;
			--i;
			if (*ii < *i)
			{
				BidirectionalIterator j = last;
				while (!((*--j) < *i));
				iter_swap(i, j);
				reverse(ii, last);
				return true;
			}

			if (i == first)
			{
				reverse(first, last);
				return false;
			}
		}
	}

	// random_shuffle
	template <typename RandomAccessIterator>
	inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last)
	{
		__random_shuffle(first, last, distance_type(first));
	}

	// __random_shuffle
//	template <typename RandomAccessIterator, typename Distance>
//	void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last, Distance*)
//	{
//		if (first == last) return;
//		for (RandomAccessIterator i = first + 1; i != last; ++i)
//		{
//#ifdef __STL_NO_DRAND48
//			iter_swap(i, first + Distance(rand() % ((i - first) + 1)));
//#else
//			iter_swap(i, first + Distance(lrand48() % ((i - first) + 1)));
//#endif // __STL_NO_DRAND48
//
//		}
//	}

	template <typename RandomAccessIterator, typename RandomNumberGenerator>
	void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last, RandomNumberGenerator& rand)
	{
		if (first == last) return;
		for (RandomAccessIterator i = first + 1; i != last; ++i)
		{
			iter_swap(i, first + rand((i - first) + 1));
		}
	}

	// partial_sort / paritial_sort/copy
	template <typename RandomAccessIterator>
	inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last)
	{
		__partial_sort(first, middle, last, value_type(first));
	}

	template <typename RandomAccessIterator, typename T>
	void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, T*)
	{
		make_heap(first, middle);
		for (RandomAccessIterator i = middle; i < last; ++i)
		{
			if (*i < *first)
			{
				__pop_heap(first, middle, i, T(*i), distance_type(first));
			}
		}
		sort_heap(first, middle);
	}

	// insertion_sort
	template <typename RandomAccessIterator>
	void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		if (first == last) return;
		for (RandomAccessIterator i = first + 1; i != last; ++i)
		{
			__linear_insert(first, i, value_type(first));
		}
	}

	template <typename RandomAccessIterator, typename T>
	void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last, T*)
	{
		T value = *last;
		if (value < *first)
		{
			copy_backward(first, last, last + 1);
			*first = value;
		}
		else
		{
			__unguarded_linear_insert(last, value);
		}
	}


	// sort
	template < typename RandomAccessIterator>
	inline void sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		if (first != last)
		{
			__introsort_loop(first, last, value_type(first), __lg(last - first) * 2);
			__final_insertion_sort(first, last);
		}
	}

	template <typename Size>
	inline Size __lg(Size n)
	{
		Size k;
		for (k = 0; n > 1; n >>= 1) ++k;

		return k;
	}

	const int __stl_threshold = 16;

	template <typename RandomAccessIterator, typename T, typename Size>
	void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last, T*, Size depth_limit)
	{
		while (last - first > __stl_threshold)
		{
			if (depth_limit == 0)
			{
				partial_sort(first, last, last);
				return;
			}

			--depth_limit;
			RandomAccessIterator cut = __unguarded_partition(first, last, T(__median(*first, *(first + (last - first) / 2), *(last - 1))));
			__introsort_loop(cut, last, value_type(first), depth_limit);
			last = cut;
		}
	}

	template <typename RandomAcessIterator>
	void __final_insertion_sort(RandomAcessIterator first, RandomAcessIterator last)
	{
		if (last - first > __stl_threshold)
		{
			__insertion_sort(first, first + __stl_threshold);
			__unguarded_insertion_sort(first + __stl_threshold, last);
		}
		else
		{
			__insertion_sort(first, last);
		}
	}

	template<typename RandomAccessIterator>
	inline void __unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		__unguarded_insertion_sort_aux(first, last, value_type(first));
	}

	template<typename RandomAccessIterator, typename T>
	inline void __unguarded_insertion_sort_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
	{
		for (RandomAccessIterator i = first; i != last; ++i)
		{
			__unguarded_linear_insert(i, T(*i));
		}
	}

	// -----------------------------------

	template <typename RandomAccessIterator>
	void __inserttion_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		if (first == last) return;
		for (RandomAccessIterator i = first + 1; i != last; ++i)
		{
			__linear_insert(first, i, value_type(first));
		}
	}

	template<typename RandomAccessIterator, typename T>
	inline void __linear_insert(RandomAccessIterator first, RandomAccessIterator last, T*)
	{
		T value = *last;
		if (value < *first)
		{
			copy_backward(first, last, last + 1);
			*first = value;
		}
		else
			__unguarded_linear_insert(last, value);
	}

	template<typename RandomAccessIterator, typename T>
	inline void __unguarded_linear_insert(RandomAccessIterator last,T value)
	{
		RandomAccessIterator next = last;
		--next;

		while (value < *next)
		{
			*last = *next;
			last = next;
			--next;
		}

		*last = value;
	}

	template<typename T>
	inline const T& __median(const T& a, const T& b, const T& c)
	{
		if (a < b)
		{
			if (b < c)
				return b;
			else if (a < c)
				return c;
			else
				return a;
		}
		else if (a < c)
			return c;
		else if (b < c)
			return c;
		else
			return b;

	}

	template <typename RandomAccessIterator, typename T>
	RandomAccessIterator __unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, T pivot)
	{
		while (true)
		{
			while (*first < pivot) ++first;
			--last;

			while (pivot < *last) --last;

			if (!(first < last)) return first;
			iter_swap(first, last);
			++first;
		}
	}
}