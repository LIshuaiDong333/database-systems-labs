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

#ifndef STACK_H
#define STACK_H

template <class T> class Stack
{
protected:
	struct Node	//栈节点
	{
		T obj;
		Node* prev;
		Node* next;
	};

	Node head;
	Node* tail;
	Node* pointer;

	int count;
public:
	Stack();
	virtual ~Stack();
	void Clear();

	void Push(T obj);
	T Pop();
	T& GetTop();

	int GetCount();
	void Rewind();
	T& GetNext();
};

template <class T> Stack<T>::Stack()
{
	head.next = 0;
	tail = &head;
	count = 0;
}

template <class T> Stack<T>::~Stack()
{
	Clear();
}

template <class T> void Stack<T>::Clear()
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

template <class T> void Stack<T>::Push(T obj)
{
	tail->next = new Node;
	tail->next->prev = tail;
	tail = tail->next;
	tail->obj = obj;
	count++;
}

template <class T> T Stack<T>::Pop()
{
	T temp = tail->obj;
	tail = tail->prev;
	delete tail->next;
	count--;
	return temp;
}

template <class T> T& Stack<T>::GetTop()
{
	return tail->obj;
}

template <class T> int Stack<T>::GetCount()
{
	return count;
}

template <class T> void Stack<T>::Rewind()
{
	pointer = &head;
}

template <class T> T& Stack<T>::GetNext()
{
	pointer = pointer->next;
	return pointer->obj;
}

#endif