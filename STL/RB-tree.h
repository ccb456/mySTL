#pragma once

#include "iterator.h"
#include "alloc.h"
#include "construct.h"
namespace My_STL
{
	typedef bool __rb_tree_color_type;
	const __rb_tree_color_type __rb_tree_red = false;
	const __rb_tree_color_type __rb_tree_black = true;

	struct __rb_tree_node_base
	{
		typedef __rb_tree_color_type color_type;
		typedef __rb_tree_node_base* base_ptr;

		color_type color;	// 节点颜色，非黑即白
		base_ptr parent;
		base_ptr left;
		base_ptr right;

		static base_ptr minimum(base_ptr x)
		{
			while (x->left != nullptr) x = x->left;

			return x;
		}

		static base_ptr maximum(base_ptr x)
		{
			while (x->right != nullptr) x = x->right;

			return x;
		}

	};

	template <typename Value>
	struct __rb_tree_node : public __rb_tree_node_base
	{
		typedef __rb_tree_node<Value>* link_type;
		Value value_field;		// 节点值
	};


	// 基层迭代器
	struct __rb_tree_base_iterator
	{
		typedef __rb_tree_node_base::base_ptr base_ptr;
		typedef bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;

		base_ptr node;

		void increment()
		{
			if (node->right != nullptr)
			{
				node = node->right;

				while (node->left != nullptr)
				{
					node = node->left;
				}
			}
			else
			{
				base_ptr y = node->parent;
				while (node == y->right)
				{
					node = y;
					y = y->parent;
				}

				if (node->right != y)
					node = y;
			}
		}

		void decrement()
		{
			if (node->color == __rb_tree_red && node->parent->parent == node)
			{
				node = node->right;
			}
			else if(node->left != nullptr)
			{
				base_ptr y = node->left;
				while (y->right != nullptr)
				{
					y = y->right;
				}
				node = y;
			}
			else
			{
				base_ptr y = node->parent;
				while (node == y->left)
				{
					node = y;
					y = y->parent;
				}

				node = y;
			}
		}
	};

	// RB-tree 的正规迭代器
	template <class Value, class Ref, class Ptr>
	struct __rb_tree_iterator : public __rb_tree_base_iterator
	{
		typedef Value value_type;
		typedef Ref reference;
		typedef Ptr pionter;
		typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
		typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
		typedef __rb_tree_iterator<Value, Ref, Ptr> self;
		typedef __rb_tree_node<Value>* link_type;

		__rb_tree_iterator() {}
		__rb_tree_iterator(link_type x) { node = x }
		__rb_tree_iterator(const iterator& it) { node = it.node}

		reference operator*() const { return link_type(node)->value_field; }
		pionter operator->() const { return &(operator*()); }

		self& operator++() { increment(); return *this; }
		self operator++(int)
		{
			self tmp = *this;
			increment();
			return tmp;
		}

		self& operator--() { decrement(); return *this; }
		self operator--(int)
		{
			self tmp = *this;
			decrement();
			return tmp;
		}
	};


	template <typename Key, typename Value, typename KeyOfVaule, typename Compare, typename Alloc = alloc>
	class rb_tree
	{
	protected:
		typedef void* void_pointer;
		typedef __rb_tree_node_base* base_ptr;
		typedef __rb_tree_node<Value> rb_tree_node;
		typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
		typedef __rb_tree_color_type color_type;

	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef rb_tree_node* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

	protected:
		link_type get_node() { return rb_tree_node_allocator::allocate(); }
		void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

		link_type create_node(const value_type& x)
		{
			link_type tmp = get_node();	//配置空间
			try
			{
				construct(&tmp->value_field, x);
			}
			catch (const std::exception&)
			{
				put_node(tmp);
			}

			return tmp;
			
		}

		link_type clone_node(link_type x)
		{
			link_type tmp = create_node(x->value_field);
			tmp->color = x->color;
			tmp->right = nullptr;
			tmp->left = nullptr;
			return tmp;
		}

		void destroy_node(link_type p)
		{
			destroy(&p->value_field);	//析构内容
			put_node(p);				// 释放内存
		}

	protected:
		size_type node_count;	// 记录树大小
		link_type header;
		Compare key_compare;	// 比较准则

		// 下面三个函数用来方便取得header的成员
		link_type& root() const { return static_cast<link_type&>(header->parent); }
		link_type& leftmost() const { return static_cast<link_type&>(header->left); }
		link_type& rightmost() const { return static_cast<link_type&>(header->right); }

		// 下面6个函数用来方便获取节点x的成员
		static link_type& left(link_type x) { return static_cast<link_type&>(x->left); }
		static link_type& right(link_type x) { return static_cast<link_type&>(x-right); }
		static link_type& parent(link_type x) { return static_cast<link_type&>(x-parent); }
		static reference value(link_type x) { return x->value_field; }
		static const Key& key(link_type x) { return static_cast<KeyOfVaule()>(value(x)); }
		static color_type& color(link_type x) { return static_cast<color_type&>(x->color); }

		// 以下6个函数方便获取节点x的成员
		static link_type& left(base_ptr x) { return static_cast<link_type&>(x->left); }
		static link_type& right(base_ptr x) { return static_cast<link_type&>(x->righr); }
		static link_type& parent(base_ptr x) { return static_cast<link_type&>(x->parent); }
		static reference value(base_ptr x) { return static_cast<link_type>(x)->value_field; }
		static const Key& key(base_ptr x) { return static_cast<KeyOfVaule()>(value(static_cast<link_type>(x))); }
		static color_type& color(base_ptr x) { return static_cast<color_type&>(static_cast<link_type>(x)->color); }

		// 求极大值和极小值
		static link_type minimum(link_type x)
		{
			return static_cast<link_type>(__rb_tree_node_base::minimum(x));
		}
		static link_type maximum(link_type x)
		{
			return static_cast<link_type>(__rb_tree_node_base::maximum(x));
		}

	public:
		typedef __rb_tree_iterator<value_type, reference, pointer> iterator;

	private:
		iterator __insert(base_ptr x, base_ptr y, const value_type& v);
		link_type __copy(link_type x, link_type p);
		void __erase(link_type x);

		void init()
		{
			header = get_node();
			color(header) = __rb_tree_red;

			root() = nullptr;
			leftmost() = header;
			rightmost() = header;
		}

	public:
		rb_tree(const Comapre& comp = Compare()) : node_count(0), key_compare(comp)
		{
			init();
		}

		~rb_tree()
		{
			clear();
			put_node(header);
		}

		rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>&
		operator=(const rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>& x);
	
	public:
		Compare key_comp() const { return key_comp; }
		iterator begin() const { return leftmost(); }		// RB树的起头为最左节点处
		iterator end() const { return header; }		// RB树的终点为header所指处
		bool empty() const { return node_count == 0; }
		size_type size() const { return node_count; }
		size_type max_size() const { return size_type(-1); }

		pair<iterator, bool> insert_unique(const value_type& x);
		iterator insert_equal(const value_type& x);
		iterator find(const Key& k);

	};


	inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root)
	{
		__rb_tree_node_base* y = x->right;
		x->right = y->left;
		if (y->left != nullptr)
		{
			y->left->parent = x;
		}

		y->parent = x->parent;

		if (x == root)
		{
			root = y;
		}
		else if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}

		y->left = x;
		x->parent = y;
	}

	inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root)
	{
		__rb_tree_node_base* y = x->left;
		x->left = y->right;
		if (y->right != nullptr)
		{
			y->right->parent = x;
		}
		y->parent = x->parent;

		if (x == root)
		{
			root = y;
		}
		else if (x == x->parent->right)
		{
			x->parent->right = y;
		}
		else
		{
			x->parent->left = y;
		}

		y->right = x;
		x->parent = y;
	}

	inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root)
	{
		x->color = __rb_tree_red;

		while (x != root && x->parent->color == __rb_tree_red)
		{
			if (x->parent == x->parent->parent->left)
			{
				__rb_tree_node_base* y = x->parent->parent->right;
				
				if (y && y->color == __rb_tree_red)
				{
					x->parent->color = __rb_tree_black;
					y->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					x = x->parent->parent;
				}

				else
				{
					if (x == x->parent->right)
					{
						x = x->parent;
						__rb_tree_rotate_left(x, root);
					}
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_right(x->parent->parent, root);
				}
			}

			else
			{
				__rb_tree_node_base* y = x->parent->parent->left;
				if (y && y->color == __rb_tree_red)
				{
					x->parent->color = __rb_tree_black;
					y->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					x = x->parent->parent;
				}
				else
				{
					if (x == x->parent->left)
					{
						x = x->parent;
						__rb_tree_rotate_right(x, root);
					}

					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_left(x->parent->parent, root);
				}
			}

		}

		root->color = __rb_tree_black;
	}

	
	template<typename Key, typename Value, typename KeyOfVaule, typename Compare, typename Alloc>
	typename rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>::iterator
	rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const value_type& v)
	{
		link_type x = static_cast<link_type> x_;
		link_type y = static_cast<link_type> y_;
		link_type z;

		if (y == header || x != nullptr || key_compare(KeyOfVaule()(v), key(y)))
		{
			z = create_node(v);
			left(y) = z;
			if (y == header)
			{
				root() = z;
				rightmost() = z;
			}
			else if (y == leftmost())
			{
				leftmost() = z;
			}
		}
		else
		{
			z = create_node(v);
			right(y) = z;
			if (y == rightmost())
			{
				rightmost() = z;
			}
		}

		parent(z) = y;
		left(z) = nullptr;
		right(z) = nullptr;

		__rb_tree_rebalance(z, header->parent);
		++node_count;
		return iterator(z);
	}

	template<typename Key, typename Value, typename KeyOfVaule, typename Compare, typename Alloc>
	typename rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>::link_type
	rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>::__copy(link_type x, link_type p)
	{
		return link_type();
	}

	template<typename Key, typename Value, typename KeyOfVaule, typename Compare, typename Alloc>
	void rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>::__erase(link_type x)
	{
	}

	template<typename Key, typename Value, typename KeyOfVaule, typename Compare, typename Alloc>
	rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>& 
	rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>::operator=(const rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>& x)
	{
		// TODO: 在此处插入 return 语句
	}

	template<typename Key, typename Value, typename KeyOfVaule, typename Compare, typename Alloc>
	pair<typename rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>::iterator, bool>
	rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>::insert_unique(const value_type& v)
	{
		link_type y = header;
		link_type x = root();
		bool comp = true;

		while (x != nullptr)
		{
			y = x;
			comp = key_compare(static_cast<KeyOfVaule()>(v), key(x));
			x = comp ? left(x) : right(x);
		}

		iterator j = iterator(y);
		if (comp)
		{
			if(j == begin())
				return pair<iterator, bool>(__insert(x, y, v), true);
			else
			{
				--j;
			}
		}

		if(key_compare(key(j.node), KeyOfVaule()(v)))
			return pair<iterator, bool>(__insert(x, y, v), true);

		return pair<iterator, bool>(j, false);
	}

	template<typename Key, typename Value, typename KeyOfVaule, typename Compare, typename Alloc>
	typename rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>::iterator
	rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>::insert_equal(const value_type& v)
	{
		link_type y = header;
		link_type x = root();

		while (x != nullptr)
		{
			y = x;
			x = key_compare(static_cast<KeyOfVaule()>(v), key(x)) ? left(x) : right(x);
		}
		return __insert(x,y,v);
	}

	template<typename Key, typename Value, typename KeyOfVaule, typename Compare, typename Alloc>
	typename rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>::iterator
	rb_tree<Key, Value, KeyOfVaule, Compare, Alloc>::find(const Key& k)
	{
		link_type y = header;
		link_type x = root();

		while (x != nullptr)
		{
			if (!key_compare(key(x), k))
			{
				y = x;
				x = left(x);
			}
			else
			{
				x = right(x);
			}
		}

		iterator j = iterator(y);

		return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}

}