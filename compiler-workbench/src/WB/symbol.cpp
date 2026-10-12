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

#include "stdafx.h"
#include "symbol.h"

Symbol::Symbol()
{
	type = E;
	id = 0;
}

Symbol::Symbol(const Symbol& s)
{
	type = s.type;
	id = s.id;
}

Symbol::Symbol(SymbolType type, int id)
{
	this->type = type;
	this->id = id;
}

Symbol& Symbol::operator=(const Symbol& s)
{
	type = s.type;
	id = s.id;
	return *this;
}

bool Symbol::operator==(const Symbol& s) const
{
	return id == s.id && type == s.type;
}

int Symbol::GetId() const
{
	return id;
}

SymbolType Symbol::GetType() const
{
	return type;
}

bool Symbol::IsStart() const
{
	if (type == S)
		return true;
	else
		return false;
}

bool Symbol::IsEof() const
{
	if (type == E)
		return true;
	else
		return false;
}

int SymbolHome::AddSymbol(const CString& name, SymbolType type)
{
	Symbol s = FindSymbol(name);
	//若符号表中已有名为name的文法符号存在，设为s
	if (!s.IsEof()) {
		if (type == N && s.GetType() == S)
			return true;
		if (type == S && s.GetType() == N) {
			if (s_name.IsEmpty() || s_name == name) {
				n_name.RemoveAt(s.GetId());
				s_name = name;
				return true;
			} else
				return false;
		}
		//若符号表中的文法符号s与要添加的符号同类型
		if (type == s.GetType())
			//说明同一符号书写了两次
			return true;
		else
			//否则为重定义
			return false;
	}

	switch (type) {
	case T:
		t_name.Add(name);
		break;
	case N:
		n_name.Add(name);
		break;
	default:
		if (s_name.IsEmpty() || s_name == name) {
			s_name = name;
			return true;
		} else
			return false;
	}
	return true;
}

Symbol SymbolHome::FindSymbol(const CString& name)
{
	t_name.Rewind();
	int t_count = t_name.GetCount();
	for (int i = 0; i < t_count; i++)
		if (t_name.GetNext() == name)
			return Symbol(T, i);

	n_name.Rewind();
	int n_count = n_name.GetCount();
	for (i = 0; i < n_count; i++)
		if (n_name.GetNext() == name)
			return Symbol(N, i);

	if (s_name == name)
		return Symbol(S, 0);

	return Symbol();
}

const CString& SymbolHome::GetName(const Symbol& s)
{
	static const CString eof_str = _T("$");

	switch (s.GetType()) {
	case T:
		return t_name[s.GetId()];
	case N:
		return n_name[s.GetId()];
	case S:
		return s_name;
	default:
		return eof_str;
	}
}

Symbol SymbolHome::GetStart()
{
	return Symbol(S, 0);
}

Symbol SymbolHome::GetEof()
{
	return Symbol();
}

int SymbolHome::GetTCount()
{
	return t_name.GetCount();
}

int SymbolHome::GetNCount()
{
	return n_name.GetCount();
}

void SymbolHome::Clear()
{
	n_name.Clear();
	t_name.Clear();
	s_name.Empty();
}

SymbolIterator::SymbolIterator(SymbolHome& symbol_home,
							   SymbolType* type_array,
							   int type_count) {
	t_count = symbol_home.GetTCount();
	n_count = symbol_home.GetNCount();
	this->type_array = type_array;
	this->type_count = type_count;
}

int SymbolIterator::GetSymbolCount(SymbolType type)
{
	switch (type) {
	case T:
		return t_count;
	case N:
		return n_count;
	default:
		return 1;
	}
}

int SymbolIterator::GetCount()
{
	int count = 0;
	for (int i = 0; i < type_count; i++) {
		count += GetSymbolCount(type_array[i]);
	}
	return count;
}

Symbol SymbolIterator::operator[](int index)
{
	for (int i = 0; i < type_count; i++) {
		int symbol_count = GetSymbolCount(type_array[i]);
		if (index < symbol_count)
			return Symbol(type_array[i], index);
		index -= symbol_count;
	}
	return Symbol();
}

int SymbolIterator::Index(Symbol s)
{
	int index = 0;
	for (int i = 0; i < type_count; i++) {
		if (s.GetType() == type_array[i])
			return index + s.GetId();
		index += GetSymbolCount(type_array[i]);
	}
	return 0;
}