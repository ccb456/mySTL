#pragma once

#include "deque.h"

namespace My_STL
{
	template <typename T, typename Sequence = deque<T>>
	class queue
	{
		template <typename T_, typename Sequence_>
		friend bool operator==(const queue<T_, Sequence_>&, const queue<T_, Sequence_>&);

		template <typename T_, typename Sequence_>
		friend bool operator<(const queue<T_, Sequence_>&, const queue<T_, Sequence_>&);
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;

	private:
		Sequence c;
	public:
		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		reference front() { return c.front(); }
		const_reference front() const { return c.front(); }
		reference back() { return c.back(); }
		const_reference back() const { return c.back();}

		void push(const value_type& x) { c.push_back(x); }
		void pop() { c.pop_front(); }

	};

	template<typename T, class Sequence>
	bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y)
	{
		return x.c == y.c;
	}

	template<typename T, class Sequence>
	bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y)
	{
		return x.c < y.c;
	}

};