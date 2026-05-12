#pragma once

#include <cstddef>
using std::ptrdiff_t;

namespace My_STL
{

	// 5钟迭代器类型
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	// 为了防止写代码时，写迭代器忘记了某种类型，写个基类，让自定义迭代器继承它
	template <typename Category, typename T, typename Distance = ptrdiff_t,
				typename Pointer = T*, typename Reference = T&>
	struct iterator
	{
		typedef Category		iterator_category;
		typedef T				value_type;
		typedef Distance		difference_type;
		typedef Pointer			pointer;
		typedef Reference		reference;
	};

	// 萃取机
	template <typename Iterator>
	struct iterator_traits
	{
		typedef  typename Iterator::iterator_category		iterator_category;
		typedef  typename Iterator::value_type				value_type;
		typedef  typename Iterator::difference_type			difference_type;
		typedef  typename Iterator::pointer					pointer;
		typedef  typename Iterator::reference				reference;
	};

	// 对原生指针设计的偏特化萃取机
	template <typename T>
	struct iterator_traits<T*>
	{
		typedef  random_access_iterator_tag		iterator_category;
		typedef  T								value_type;
		typedef  ptrdiff_t						difference_type;
		typedef  T*								pointer;
		typedef  T&								reference;
	};

	template <typename T>
	struct iterator_traits<const T*>
	{
		typedef  random_access_iterator_tag		iterator_category;
		typedef  T								value_type;
		typedef  ptrdiff_t						difference_type;
		typedef  const T*						pointer;
		typedef  const T&						reference;
	};

	// 更方便的获取某个迭代器的类型
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator&)
	{
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	// 更方便的获取某个迭代器的 distance type
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	distance_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	// 更方便的获取某个迭代器的 value_type
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}


	// 以下是整组的distance 函数
	template <typename InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	__distance(InputIterator first, InputIterator last, input_iterator_tag)
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last)
		{
			++first;
			++n;
		}

		return n;
	}

	template <typename RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
	__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
	{
		
		return last - first;
	}

	template <typename InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	distance(InputIterator first, InputIterator last)
	{
		typedef typename iterator_traits<InputIterator>::iterator_category  category;

		return __distance(first, last, category());
	}

	// 下列是整组advance函数
	template <typename InputIterator, typename Distance>
	inline void __advance(InputIterator& first, Distance n, input_iterator_tag)
	{
		while (n--) ++first;
	}

	template <typename BidirectionalIterator, typename Distance>
	inline void __advance(BidirectionalIterator& first, Distance n, bidirectional_iterator_tag)
	{
		if (n >= 0)
		{
			while (n--) ++first;
		}
		else
		{
			while (n++) --first;
		}
	}

	template <typename RandomAccessIterator, typename Distance>
	inline void __advance(RandomAccessIterator& first, Distance n, random_access_iterator_tag)
	{

		first + n;
	}

	template <typename InputIterator, typename Distance>
	inline void advance(InputIterator& first, Distance n)
	{
		__distance(first, n, iterator_category(first));
	}

	template <typename Container>
	class back_insert_iterator
	{
	protected:
		Container* container;

	public:
		typedef output_iterator_tag		iterator_category;
		typedef void	value_type;
		typedef void	difference_type;
		typedef void	pointer;
		typedef void	reference;

		explicit back_insert_iterator(Container& x) : container(&x){}
		back_insert_iterator<Container>&
			operator =(const typename Container::value_type& value)
		{
			container->push_back(value);
			return *this;
		}

		back_insert_iterator<Container>& operator*() { return *this; }
		back_insert_iterator<Container>& operator++() { return *this; }
		back_insert_iterator<Container>& operator++(int) { return *this; }
	};

	template<typename Container>
	inline back_insert_iterator<Container> back_inserter(Container& x)
	{
		return back_insert_iterator<Container>(x);
	}

	template <typename Container>
	class front_insert_iterator
	{
	protected:
		Container* container;

	public:
		typedef output_iterator_tag		iterator_category;
		typedef void	value_type;
		typedef void	difference_type;
		typedef void	pointer;
		typedef void	reference;

		explicit front_insert_iterator(Container& x) : container(&x) {}
		front_insert_iterator<Container>&
			operator =(const typename Container::value_type& value)
		{
			container->push_front(value);
			return *this;
		}

		front_insert_iterator<Container>& operator*() { return *this; }
		front_insert_iterator<Container>& operator++() { return *this; }
		front_insert_iterator<Container>& operator++(int) { return *this; }
	};

	template<typename Container>
	inline front_insert_iterator<Container> back_inserter(Container& x)
	{
		return front_insert_iterator<Container>(x);
	}

	template <typename Container>
	class insert_iterator
	{
	protected:
		Container* container;
		typename Container::iterator iter;

	public:
		typedef output_iterator_tag		iterator_category;
		typedef void	value_type;
		typedef void	difference_type;
		typedef void	pointer;
		typedef void	reference;

		explicit insert_iterator(Container& x, typename Container::iterator i) : container(&x), iter(i) {}
		insert_iterator<Container>&
			operator =(const typename Container::value_type& value)
		{
			container->insert(iter, value);
			++iter;
			return *this;
		}

		insert_iterator<Container>& operator*() { return *this; }
		insert_iterator<Container>& operator++() { return *this; }
		insert_iterator<Container>& operator++(int) { return *this; }
	};

	template<typename Container, typename Iterator>
	inline insert_iterator<Container> inserter(Container& x, Iterator i)
	{
		typedef typename Container::iterator iter;
		return insert_iterator<Container>(x, iter(i));
	}

	template <typename Iterator>
	class reverse_iterator
	{
	protected:
		Iterator current;

	public:
		typedef typename iterator_traits<Iterator>::value_type value_type;
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::difference_type difference_type;
		typedef typename iterator_traits<Iterator>::pointer pointer;
		typedef typename iterator_traits<Iterator>::reference reference;

		typedef Iterator iterator_type;		// 代表正向迭代器
		typedef reverse_iterator<Iterator> self;	// 代表逆向迭代器

		reverse_iterator(){}
		explicit reverse_iterator(iterator_type x) : current(x){}
		reverse_iterator(const self& x) : current(x.current) {}
		iterator_type base() const { return current; }
		reference operator*() const 
		{
			Iterator tmp = current;
			return *--tmp;
		
		}

		pointer operator->() const { return &(operator*()); }

		self& operator++()
		{
			--current;
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			--current;
			return tmp;
		}

		self& operator--()
		{
			++current;
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			++current;
			return tmp;
		}

		self operator+(difference_type n) const
		{
			return self(current - n);
		}

		self& operator+=(difference_type n)
		{
			current -= n;
			return *this;
		}

		self operator-(difference_type n) const
		{
			return self(current + n);
		}

		self& operator-=(difference_type n)
		{
			current += n;
			return *this;
		}

		reference operator[](difference_type n) const { return *(*this + n); }
	};

}


