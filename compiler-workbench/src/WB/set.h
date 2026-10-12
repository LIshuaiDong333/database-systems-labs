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

#ifndef SET_H
#define SET_H

#include "list.h"

template <class T> class Set
{
protected:
	List<T> set_list;
public:
	Set();
	Set(Set<T>& s);

	Set<T>& operator=(Set<T>& s);
	bool operator==(Set<T>& s);

	void Clear();

	T& Add(T obj);
	void Add(Set<T> s);
	void Remove(T obj);
	bool Has(T obj);

	Set<T> Intersect(Set<T>& s);
	Set<T> Union(Set<T>& s);
	Set<T> Subtract(Set<T>& s);

	int GetCount();
	void Rewind();
	T& GetNext();
};

template <class T> Set<T>::Set()
{
}

template <class T> Set<T>::Set(Set<T>& s)
{
	set_list = s.set_list;
}

template <class T> Set<T>& Set<T>::operator=(Set<T>& s)
{
	if (&s == this)
		return *this;
	set_list = s.set_list;
	return *this;
}

template <class T> bool Set<T>::operator==(Set<T>& s)
{
	if (set_list.count != s.set_list.count)
		return false;
	set_list.Rewind();
	for (int i = 0; i < set_list.count; i++)
		if (!s.Has(set_list.GetNext()))
			return false;
	return true;
}

template <class T> void Set<T>::Clear()
{
	set_list.Clear();
}

template <class T> T& Set<T>::Add(T obj)
{
	return set_list.Add2(obj);
}

template <class T> void Set<T>::Add(Set<T> s)
{
	set_list.Merge2(s.set_list);
}

template <class T> void Set<T>::Remove(T obj)
{
	List<T>::Node* ptr = set_list.head.next;
	List<T>::Node* pre = &(set_list.head);
	while (ptr) {
		if (ptr->obj == obj) {
			pre->next = ptr->next;
			delete ptr;
			ptr = pre->next;
			set_list.count--;
		}
		ptr = ptr->next;
		pre = pre->next;
	}
}

template <class T> bool Set<T>::Has(T obj)
{
	return set_list.FindFirst(obj) != -1;
}

template <class T> Set<T> Set<T>::Intersect(Set<T>& s)
{
	Set<T> newSet;
	set_list.Rewind();
	for (int i = 0; i < set_list.count; i++) {
		T& obj = set_list.GetNext();
		if (s.Has(obj))
			newSet.Add(obj);
	}
	return newSet;
}

template <class T> Set<T> Set<T>::Union(Set<T>& s)
{
	Set<T> newSet;
	set_list.Rewind();
	for (int i = 0; i < set_list.count; i++)
		newSet.Add(set_list.GetNext());
	s.set_list.Rewind();
	for (i = 0; i < s.set_list.count; i++)
		newSet.Add(s.set_list.GetNext());
	return newSet;
}

template <class T> Set<T> Set<T>::Subtract(Set<T>& s)
{
	Set<T> newSet;
	set_list.Rewind();
	for (int i = 0; i < set_list.count; i++)
		newSet.Add(set_list.GetNext());
	s.set_list.Rewind();
	for (i = 0; i < s.set_list.count; i++)
		newSet.Remove(s.set_list.GetNext());
	return newSet;
}

template <class T> int Set<T>::GetCount()
{
	return set_list.count;
}

template <class T> void Set<T>::Rewind()
{
	set_list.Rewind();
}

template <class T> T& Set<T>::GetNext()
{
	return set_list.GetNext();
}

#endif