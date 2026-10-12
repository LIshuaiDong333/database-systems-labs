////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  本文件由 尹涛<taco@163.com> 编写。
//
//  这个软件不提供任何保证。作者不对任何由于使用这个软件而引起的损害事件负责。
//
//  如果您同意下列条件，允许任何人、出于任何目的使用本文件所包含的源代码，包括商业软件，以及改进和重新发布：
//
//    1. 所有源代码文件重新发布必须在适当位置保留当前的版权信息和条件说明。
//
//    2. 由您修改的部分要有清楚的说明。
//
//    3. 所有以二进制格式重新发布必须在适当位置保留当前的版权信息。
//
//    4. 这个软件的来源必须如实说明，你不能声称这个原始软件是你编写的。
//
//    5. 已修改的源代码或二进制形式版本在重新发布时要包含这个许可协议的一个副本。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYLIST_H
#define MYLIST_H

template <class T> class Set;

template <class T> class List
{
protected:
	struct Node	//列表节点
	{
		T obj;
		Node* next;
	};

	Node head;
	Node* tail;
	Node* pointer;

	int count;

	friend class Set<T>;

public:
	List();	//默认构造函数
	List(List<T>& l);	//拷贝构造函数
	virtual ~List();

	List<T>& operator=(List<T>& l);

	bool operator==(List<T>& l);

	void Clear();	//清空列表

	int FindFirst(T obj);	//查找对象，找到返回对象位置，未找到返回-1

	T& Add(T obj);	//添加至列表尾
	T& Add2(T obj);	//若列表中没有该项则添加，否则不添加
	T& Add2(T obj, int& pos);
	T& Insert(T obj, int pos);
	void Merge(List<T>& list);	//将另一个列表合并到本列表尾
	void Merge2(List<T>& list);	//添加另一个列表有，而本列表没有的对象
	void RemoveAt(int i);
	void RemoveHead();
	void RemoveTail();

	int GetCount();	//获得列表长度
	void Rewind();	//使当前指针指向列表头
	T& GetNext();	//返回当前指针所指对象，并向前移动指针

	void GetSubList(List<T>& sub_list, int start, int count);
	void GetSubList(List<T>& sub_list, int start);
	void ReleaseSubList();

	List<T>* Divide(int start);

	T& operator [] (int i);	//获得列表第i个对象
};

template <class T> List<T>::List()
{
	head.next = 0;
	tail = &head;
	count = 0;
}

template <class T> List<T>::List(List<T>& l)
{
	head.next = 0;
	tail = &head;
	count = 0;
	operator=(l);
}

template <class T> List<T>::~List()
{
	Clear();
}

template <class T> List<T>& List<T>::operator=(List<T>& l)
{
	if (&l == this)
		return *this;
	Clear();
	l.Rewind();
	int count = l.GetCount();
	for (int i = 0; i < count; i++)
		Add(l.GetNext());
	return *this;
}

template <class T> bool List<T>::operator==(List<T>& l)
{
	if (count != l.count)
		return false;

	Node* ptr1 = head.next;
	Node* ptr2 = l.head.next;

	for (int i = 0; i < count; i++) {
		if (!(ptr1->obj == ptr2->obj))
			return false;
		ptr1 = ptr1->next;
		ptr2 = ptr2->next;
	}
	return true;
}

template <class T> void List<T>::Clear()
{
	Node* temp1 = head.next;
	Node* temp2;
	for (int i = 0; i < count; i++) {
		temp2 = temp1->next;
		delete temp1;
		temp1 = temp2;
	}
	head.next = 0;
	tail = &head;
	count = 0;
}

template <class T> int List<T>::FindFirst(T obj)
{
	Node* ptr = head.next;
	int i = 0;
	while (ptr) {
		if (ptr->obj == obj)
			return i;
		ptr = ptr->next;
		i++;
	}
	return -1;
}

template <class T> T& List<T>::Add(T obj)
{
	tail->next = new Node;
	tail = tail->next;
	tail->obj = obj;
	tail->next = 0;
	count++;
	return tail->obj;
}

template <class T> T& List<T>::Add2(T obj)
{
	Node* ptr = head.next;
	while (ptr) {
		if (ptr->obj == obj)
			return ptr->obj;
		ptr = ptr->next;
	}
	return Add(obj);
}

template <class T> T& List<T>::Add2(T obj, int& pos)
{
	Node* ptr = head.next;
	pos = 0;
	while (ptr) {
		if (ptr->obj == obj)
			return ptr->obj;
		ptr = ptr->next;
		pos++;
	}
	return Add(obj);
}

template <class T> T& List<T>::Insert(T obj, int pos)
{
	Node* pre = &head;
	for (int i = 0; i < pos; i++)
		pre = pre->next;
	Node* new_node = new Node;
	new_node->obj = obj;
	new_node->next = pre->next;
	pre->next = new_node;
	if (pos == count)
		tail = new_node;
	count++;
	return new_node->obj;
}

template <class T> void List<T>::Merge(List<T>& l)
{
	l.Rewind();
	int count = l.GetCount();
	for (int i = 0; i < count; i++)
		Add(l.GetNext());
}

template <class T> void List<T>::Merge2(List<T>& l)
{
	l.Rewind();
	int count = l.GetCount();
	for (int i = 0; i < count; i++)
		Add2(l.GetNext());
}

template <class T> void List<T>::RemoveAt(int i)
{
	if (i < 0 || i >= count)
		return;

	Node* prev = &head;
	for (int j = 0; j < i; j++)
		prev = prev->next;

	Node* temp = prev->next;
	prev->next = temp->next;
	delete temp;

	if (i == count - 1)
		tail = prev;

	count--;
}

template <class T> void List<T>::RemoveHead()
{
	RemoveAt(0);
}

template <class T> void List<T>::RemoveTail()
{
	RemoveAt(count - 1);
}

template <class T> int List<T>::GetCount()
{
	return count;
}

template <class T> void List<T>::Rewind()
{
	pointer = &head;
}

template <class T> T& List<T>::GetNext()
{
	pointer = pointer->next;
	return pointer->obj;
}

template <class T> T& List<T>::operator [] (int i)
{
	Node* ptr = head.next;
	for (int j = 0; j < i; j++)
		ptr = ptr->next;
	return ptr->obj;
}

template <class T> void List<T>::GetSubList(List<T>& sub_list, int start, int count)
{
	Node* ptr = head.next;
	for (int i = 0; i < start; i++)
		ptr = ptr->next;
	sub_list.head.next = ptr;
	sub_list.count = count;
}

template <class T> void List<T>::GetSubList(List<T>& sub_list, int start)
{
	GetSubList(sub_list, start, count - start);
}

template <class T> void List<T>::ReleaseSubList()
{
	count = 0;
}

template <class T> List<T>* List<T>::Divide(int start)
{
	if (start == count)
		return new List<T>;
	Node* pre = &head;
	for (int i = 0; i < start; i++)
		pre = pre->next;
	List<T>* new_list = new List<T>;
	new_list->head.next = pre->next;
	new_list->count = count - start;
	new_list->tail = tail;
	count = start;
	tail = pre;
	tail->next = 0;
	return new_list;
}

#endif