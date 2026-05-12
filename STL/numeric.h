#pragma once
/*
主要包含数值算法
	1. accumulate
	2. adjacent_difference
	3. inner_product
	4. partial_sum
	5. power
	6. itoa
*/

#include "iterator.h"

namespace My_STL
{
	// accumulate
	template <typename InputIterator, typename T>
	T accumulate(InputIterator first, InputIterator last, T init)
	{
		for (; first != last; ++first)
		{
			init = init + *first;
		}
		return init;
	}

	template <typename InputIterator, typename T, typename BinaryOperation>
	T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op)
	{
		for (; first != last; ++first)
		{
			init = binary_op(init, *first);
		}

		return init;
	}

	//adjacent_difference
	template<typename InputIterator, typename OutputIterator>
	OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result)
	{
		if (first == last) return result;
		*result = *first;
		iterator_traits<InputIterator>::value_type value = *first;
		while (++first != last)
		{
			iterator_traits<InputIterator>::value_type tmp = *first;
			*++result = tmp - value;
			value = tmp;

		}

		return ++result;
	}

	template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
	OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op)
	{
		if (first == last) return result;
		*result = *first;
		iterator_traits<InputIterator>::value_type value = *first;
		while (++first != last)
		{
			iterator_traits<InputIterator>::value_type tmp = *first;
			*++result = binary_op(tmp, value);
			value = tmp;

		}

		return ++result;
	}

	// inner_product
	template <typename InputIterator1, typename InputIterator2, typename T>
	T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			init = init + (*first1 * *first2);
		}

		return init;
	}
	template <typename InputIterator1, typename InputIterator2, 
			typename T, typename BinaryOpration1, typename BinaryOperation2>
	T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, 
					T init, BinaryOpration1 binary_op1, BinaryOperation2 binary_op2)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			init = binary_op1(init, binary_op2(*first1, *first2));
		}

		return init;
	}

	// partial_sum
	template<typename InputIterator, typename OutputIterator>
	OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result)
	{
		if (first == last) return result;
		*result = *first;

		return __partial_sum(first, last, result, value_type(first));

	}

	template<typename InputIterator, typename OutputIterator, typename T>
	OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*)
	{
		T value = *first;
		while (++first != last)
		{
			value = value + *first;
			*++result = value;
		}

		return ++result;

	}

	template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
	OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op)
	{
		if (first == last) return result;
		*result = *first;

		return __partial_sum(first, last, result, binary_op, value_type(first));

	}

	template<typename InputIterator, typename OutputIterator, typename BinaryOperation, typename T>
	OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op, T*)
	{
		T value = *first;
		while (++first != last)
		{
			value = binary_op(value, *first);
			*++result = value;
		}

		return ++result;

	}

	// power
	template<typename T, typename Integer>
	inline T power(T x, Integer n)
	{
		return power(x, n, multiplies<T>());
	}
	template<typename T, typename Integer, typename MonoidOperation>
	inline T power(T x, Integer n, MonoidOperation op)
	{
		if (n == 0)
		{
			return identity_element(op);
		}
		else
		{
			while ((n & 1) == 0)
			{
				n >>= 1;
				x = op(x, x);
			}

			T result = x;
			n >>= 1;

			while (n != 0)
			{
				x = op(x, x);
				if ((n & 1) != 0)
				{
					result = op(result, x);
				}

				n >>= 1;
			}

			return result;
		}
	}

	// itoa
	template<typename ForwardIterator, typename T>
	void itoa(ForwardIterator first, ForwardIterator last, T value)
	{
		while (first != last)
		{
			*first++ = value++;
			 
		}
	}



}