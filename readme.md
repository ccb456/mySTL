## 一、项目介绍

本项目主要借鉴**SGI STL**实现版本，代码内容参考了

- 书籍 《STL源码剖析》- - 侯捷
- 视频 《STL源码剖析》--侯捷

**环境配置：**

Windows11 + vs2019

## 二、实现功能

### 1.空间配置器

空间配置器包含的文件有：

- allocator.h 	    [标准的空间配置器]
- alloc.h                     [SGI 使用的空间配置器]
- construct.h             [具体的空间配置操作]
- uninitialized.h       [空间配置的有关辅助函数]

### 2.迭代器

迭代器包含的文件有：

- iterator.h                  [迭代器相关操作]
- type_traits.h             [类型萃取的操作]

### 3.容器

容器包含的文件有：

- vector.h
- list.h
- deque.h
- stack.h
- queue.h
- heap.h
- priority_queue.h
- slist.h
- RB-tree.h
- set.h
- map.h
- hashtable.h
- hash_set.h
- hash_map.h

#### 3.1 vector实现的功能

1. vector()
2. vector(size_type n, const T& value)
3. vector(int n, const T& value)
4. vector(long n, const T& value)
5. explicit vector(size_type n)
6. ~vector()
7. begin() 
8. end() 
9. size() 
10. capacity()
11. empty()
12. operator[]
13. front() 
14. back() 
15. push_back(const T& x)
16. pop_back()
17. erase(iterator position)
18. erase(iterator first, iterator last)
19. resize(size_type new_size, const T& x)
20. resize(size_type new_size)
21. clear() 
22. insert(iterator position, size_type n, const T& x);

#### 3.2 list实现的功能

1. list()
2. push_back(const T& x)
3. push_front(const T& x)
4. back()
5. front()
6. size()
7. empty()
8. end()
9. begin()
10. erase(iterator position)
11. pop_front()
12. pop_back()
13. splice
14. clear();
15. remove(const T& x);
16. unique();
17. merge(list<T, Alloc>& x);
18. reverse();

#### 3.3  deque实现的功能

1. deque(int n, const T& value)
2. begin()
3. end()
4. operator[]
5. front()
6. back()
7. size()
8. max_size()
9. empty()
10. push_back(const T& t)
11. push_front(const T& t)
12. pop_back()
13. pop_front()
14. clear()
15. erase
16. insert(iterator position, const value_type& x)

#### 3.4 stack实现的功能

1. empty() 
2. size() 
3. top()
4. push(const value_type& x) 
5. pop()

#### 3.5 queue实现的功能

1. empty()
2. size() 
3. front() 
4. back() 
5. push(const value_type& x)
6. pop()

#### 3.6 heap 实现的功能

1. push_heap(RandomAccessIterator first, RandomAccessIterator last)
2. pop_heap(RandomAccessIterator first, RandomAccessIterator last)
3. sort_heap(RandomAccessIterator first, RandomAccessIterator last)
4. make_heap(RandomAccessIterator first, RandomAccessIterator last)

#### 3.7 priority_queue实现的功能

【因为heap没有实现带comp的函数所以目前这个类还不能使用】

1. priority_queue()
2. priority_queue(InputIterator first, InputIterator last, const Compare& x)
3. priority_queue(InputIterator first, InputIterator last)
4. empty() 
5. size() 
6. top()
7. push(const value_type& x) 
8. pop()

#### 3.8 slist实现的功能

1. empty() 
2. size() 
3. swap(slist& L)
4. begin()
5. end()
6. front()
7. pop_front()
8. slist()

#### 3.9 RB-tree实现的功能

1. key_comp() 
2. begin() 
3. end() 
4. empty() 
5. size() 
6. max_size()
7. insert_unique(const value_type& x)
8. insert_equal(const value_type& x)
9. find(const Key& k)

#### 3.10 set实现的功能



#### 3.11 map实现的功能



#### 3.12 hashtable实现的功能



#### 3.13 hash_set实现的功能



#### 3.14 hash_map实现的功能



### 4.算法

算法包含的文件：

- algo.h               [其它算法]
- algobase.h       [基本算法]
- numeric.h        [数值算法]

#### 4.1 数值算法

1. accumulate
2. adjacent_difference
3. inner_product
4. partial_sum
5. power
6. itoa

#### 4.2 基本算法

1. equal
2. fill
3. fill_n
4. iter_swap
5. lexicographical_compare
6. max
7. min
8. mismatch
9. swap
10. copy      
11. copy_backward   
12. set_union
13. set_intersection
14. set_difference
15. set_symmetric_difference

#### 4.3 其它算法

1. adjacent_find
2. count
3. count_if
4. find
5. find_if
6. find_end
7. find_first_of
8. for_each
9. generate
10. generate_n
11. includes (应用于有序区间)
12. max_element
13. merge (应用于有序区间)
14. min_element
15. partition
16. remove (移除但不删除)
17. remove_copy
18. remove_if
19. remove_copy_if
20. replace
21. replace_copy
22. replace_if
23. replace_copy_if
24. reverse
25. reverse_copy
26. rotate
27. rotate_copy
28. search
29. search_n
30. swap_ranges
31. transform
32. unique
33. unique_copy
34. lower_bound(应用于有序区间)
35. upper_bound(应用于有序区间)
36. binary_search (应用于有序区间)
37. next_permutation
38. prev_permutation
39. random_shuffle
40. partial_sort
41. partial_sort_copy
42. sort
43. equal_range(应用于有序区间)
44. inplace_merge(应用于有序区间)     【未实现】
45. nth_element                                        【未实现】
46. merge_sort                                           【未实现】

### 5.仿函数

仿函数包含的文件：

- function.h

#### 5.1 算术类(Arithmetic)仿函数

1. 加法：plus<T>
2. 减法：minus<T>
3. 乘法：multiplies<T>
4. 除法：divides<T>
5. 模取(modulus)：modulus<T>
6. 否定(negation)：negate<T>

#### 5.2 关系运算类(Relational)仿函数

1. 等于(equality)：equal_to<T>
2. 不等于(inequality)：not_equal_to<T>
3. 大于(greater than)：greater<T>
4. 大于或等于(greater than or equal)：greater_equal<T>
5. 小于(less than)：less<T>
6. 小于或等于(less than orequal)：less_equal<T>

#### 5.3 逻辑运算类(Logical)仿函数

1. And：logical_and<T>
2. Or：logical_or<T>
3. Not：logical_not<T>

#### 5.4 证同(identity)、选择(select)、投射(project)

1. identity
2. select1st
3. select2nd
4. project1st
5. project2nd

### 6.配接器

配接器包含的文件：

- function.h
- iterator.h

#### 6.1 迭代器的配接器

1. insert_iterators
2. reverse_iterators
3. stream_iterators    【未实现】

#### 6.2 函数配接器

1. notl，not2 ：对返回值进行逻辑否定
2. bindlst,bind2nd ：对参数进行绑定
3. compose1,compose2：用于函数合成
4. ptr_fun：用于函数指针
5. mem fun,mem fun_ref：用于成员函数指针
