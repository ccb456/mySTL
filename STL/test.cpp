#include <iostream>
#include "algo.h"
#include "vector.h"
#include "list.h"
#include "deque.h"


using namespace My_STL;
using std::cout;
using std::endl;


void vector_test()
{
	My_STL::vector<int> iv(2,9);
	cout << "size=" << iv.size() << endl;
	cout << "capacity=" << iv.capacity() << endl;

	iv.push_back(1);
	cout << "size=" << iv.size() << endl;
	cout << "capacity=" << iv.capacity() << endl;

	iv.push_back(2);
	cout << "size=" << iv.size() << endl;
	cout << "capacity=" << iv.capacity() << endl;

	iv.push_back(3);
	cout << "size=" << iv.size() << endl;
	cout << "capacity=" << iv.capacity() << endl;

	iv.push_back(4);
	cout << "size=" << iv.size() << endl;
	cout << "capacity=" << iv.capacity() << endl;

	for (int i = 0; i < iv.size(); ++i)
	{
		cout << iv[i] << " ";
	}

	cout << endl;

	iv.push_back(5);
	
	cout << "size=" << iv.size() << endl;
	cout << "capacity=" << iv.capacity() << endl;
	for (int i = 0; i < iv.size(); ++i)
	{
		cout << iv[i] << " ";
	}

	cout << endl;

	iv.pop_back();
	iv.pop_back();
	cout << "size=" << iv.size() << endl;
	cout << "capacity=" << iv.capacity() << endl;
	
	iv.pop_back();
	cout << "size=" << iv.size() << endl;
	cout << "capacity=" << iv.capacity() << endl;

	auto ivite = My_STL::find(iv.begin(), iv.end(), 1);

	if (ivite != iv.end()) iv.erase(ivite);
	cout << "size=" << iv.size() << endl;
	cout << "capacity=" << iv.capacity() << endl;
	for (int i = 0; i < iv.size(); ++i)
	{
		cout << iv[i] << " ";
	}
	cout << endl;

	ivite = find(iv.begin(), iv.end(), 2);
	if (ivite != iv.end()) iv.insert(ivite, 3, 7);

	cout << "size=" << iv.size() << endl;
	cout << "capacity=" << iv.capacity() << endl;
	for (int i = 0; i < iv.size(); ++i)
	{
		cout << iv[i] << " ";
	}
	cout << endl;

	iv.clear();
	cout << "size=" << iv.size() << endl;
	cout << "capacity=" << iv.capacity() << endl;
}

void list_test()
{
	list<int> m_list;
	cout << "size=" << m_list.size() << endl;

	m_list.push_back(0);
	m_list.push_back(1);
	m_list.push_back(2);
	m_list.push_back(3);
	m_list.push_back(4);
	cout << "size=" << m_list.size() << endl;

	for (auto i = m_list.begin(); i != m_list.end(); ++i)
	{
		cout << *i << " ";
	}

	cout << endl;

	list<int>::iterator iter;

	iter = find(m_list.begin(), m_list.end(), 3);
	if (iter != 0)
	{
		m_list.insert(iter, 99);
	}

	cout << "size=" << m_list.size() << endl;
	cout << *iter << endl;

	for (auto i = m_list.begin(); i != m_list.end(); ++i)
	{
		cout << *i << " ";
	}

	cout << endl;

	iter = find(m_list.begin(), m_list.end(), 1);
	if (iter != 0)
	{
		cout << *(m_list.erase(iter)) << endl;
	}

	for (auto i = m_list.begin(); i != m_list.end(); ++i)
	{
		cout << *i << " ";
	}
	cout << endl;

	list<int> i_list;
	i_list.push_front(6);
	i_list.push_front(7);
	i_list.push_front(8);
	i_list.push_front(9);

	for (auto i = i_list.begin(); i != i_list.end(); ++i)
	{
		cout << *i << " ";
	}
	cout << endl;

	m_list.remove(99);

	m_list.reverse();

	for (auto i = m_list.begin(); i != m_list.end(); ++i)
	{
		cout << *i << " ";
	}
	cout << endl;

	i_list.merge(m_list);

	for (auto i = i_list.begin(); i != i_list.end(); ++i)
	{
		cout << *i << " ";
	}
	cout << endl;

}

void deque_test()
{
	deque<int, alloc, 8> ideq(20, 9);
	cout << "size=" << ideq.size() << endl;

	for (int i = 0; i < ideq.size(); ++i)
	{
		ideq[i] = i;
	}

	for (int i = 0; i < ideq.size(); ++i)
	{
		cout << ideq[i] << " ";
	}

	cout << endl;

	for (int i = 0; i < 3; ++i)
		ideq.push_back(i);

	for (int i = 0; i < ideq.size(); ++i)
	{
		cout << ideq[i] << " ";
	}

	cout << endl;
	cout << "size=" << ideq.size() << endl;

	ideq.push_back(3);

	for (int i = 0; i < ideq.size(); ++i)
	{
		cout << ideq[i] << " ";
	}

	cout << endl;
	cout << "size=" << ideq.size() << endl;

	ideq.push_front(99);

	for (int i = 0; i < ideq.size(); ++i)
	{
		cout << ideq[i] << " ";
	}

	cout << endl;
	cout << "size=" << ideq.size() << endl;

	ideq.push_front(98);
	ideq.push_front(97);

	for (int i = 0; i < ideq.size(); ++i)
	{
		cout << ideq[i] << " ";
	}

	cout << endl;
	cout << "size=" << ideq.size() << endl;

	auto i = find(ideq.begin(), ideq.end(), 99);
	cout << *i << endl;
	cout << *(i.cur) << endl;
}


// 向下调整
void adjustHeap(vector<int>& heap, int parent, int len)
{
	int value = heap[parent];
	int rightChild = parent * 2 + 1;
	if (rightChild >= len) return;

	int leftChild = parent * 2;

	int idx = heap[leftChild] <= heap[rightChild] ? rightChild : leftChild;

	if (value < heap[idx])
	{
		heap[parent] = heap[idx];
		heap[idx] = value;
		adjustHeap(heap, idx, len);
	}

}

void makeHeap(vector<int>& heap, int len)
{
	for (int i = len / 2; i > 0; --i)
	{
		adjustHeap(heap, i, len);
	}
}


int main()
{
	//vector_test();
	//list_test();
	deque_test();
}