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

#ifndef SYMBOL_H
#define SYMBOL_H

#include "list.h"

enum SymbolType {
	T,	//终结符
	N,	//非终结符
	S,	//文法起始符
	E	//文件结束符
};

class Symbol
{
	SymbolType type;
	int id;
public:
	Symbol();	//默认构造函数
	Symbol(const Symbol& s);	//拷贝构造函数
	Symbol(SymbolType type, int id);	//已知类型和id的文法符号的构造函数

	Symbol& operator=(const Symbol& s);

	bool operator==(const Symbol& s) const;

	int GetId() const;
	SymbolType GetType() const;

	bool IsStart() const;
	bool IsEof() const;
};

class SymbolHome
{
	List<CString> t_name;
	List<CString> n_name;
	CString s_name;
public:
	int AddSymbol(const CString& name, SymbolType type);
	Symbol FindSymbol(const CString& name);
	const CString& GetName(const Symbol& s);

	Symbol GetStart();	//获得文法起始符
	Symbol GetEof();	//获得文件结束符

	int GetTCount();
	int GetNCount();

	void Clear();
};

class SymbolIterator
{
	int t_count;
	int n_count;

	SymbolType* type_array;
	int type_count;

	int GetSymbolCount(SymbolType type);
public:
	SymbolIterator(SymbolHome& symbol_home,
		SymbolType* type_array, int type_count);

	int GetCount();
	Symbol operator[](int index);
	int Index(Symbol s);
};

#endif