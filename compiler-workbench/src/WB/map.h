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

#ifndef MAP_H
#define MAP_H

#include "list.h"

template <class T1, class T2> class Map
{
protected:
	struct MapElem
	{
		T1 key;
		T2 obj;
	};
	List<MapElem> map_list;

public:
	void Clear();	//清空所有映射

	bool Has(T1 key);
	T2& Touch(T1 key);
	T2& operator[](T1 key);	//查找key所对应的对象，若映射中还没有key的映射，则创建一个key的映射

	int GetCount();	//获得映射的个数
	void Rewind();	//指针指向第一个映射
	T2& GetNext();
	T2& GetNext(T1& key);
};

template <class T1, class T2> void Map<T1, T2>::Clear()
{
	map_list.Clear();
}

template <class T1, class T2> bool Map<T1, T2>::Has(T1 key)
{
	map_list.Rewind();
	int count = map_list.GetCount();
	for (int i = 0; i < count; i++) {
		MapElem& elem = map_list.GetNext();
		if (elem.key == key)
			return true;
	}
	return false;
}

template <class T1, class T2> T2& Map<T1, T2>::Touch(T1 key)
{
	map_list.Rewind();
	int count = map_list.GetCount();
	for (int i = 0; i < count; i++) {
		MapElem& elem = map_list.GetNext();
		if (elem.key == key)
			return elem.obj;
	}
	MapElem elem;
	elem.key = key;
	map_list.Add(elem);
	return map_list.GetNext().obj;
}

template <class T1, class T2> T2& Map<T1, T2>::operator[](T1 key)
{
	return Touch(key);
}

template <class T1, class T2> int Map<T1, T2>::GetCount()
{
	return map_list.GetCount();
}

template <class T1, class T2> void Map<T1, T2>::Rewind()
{
	map_list.Rewind();
}

template <class T1, class T2> T2& Map<T1, T2>::GetNext()
{
	return map_list.GetNext().obj;
}

template <class T1, class T2> T2& Map<T1, T2>::GetNext(T1& key)
{
	MapElem& elem = map_list.GetNext();
	key = elem.key;
	return elem.obj;
}

#endif