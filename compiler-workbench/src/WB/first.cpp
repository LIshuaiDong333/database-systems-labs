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
#include "first.h"

void First::SetGrammar(Grammar* grammar)
{
	this->grammar = grammar;
}

void First::Compute()
{
	map.Clear();

	//为每个非终结符在FIRST函数中构造一个映射
	map.Touch(grammar->symbol_home.GetStart());
	int n_count = grammar->symbol_home.GetNCount();
	for (int i = 0; i < n_count; i++)
		map.Touch(Symbol(N, i));

	bool modified;
	do {
		modified = false;
		grammar->production.Rewind();
		int count = grammar->production.GetCount();
		for (int i = 0; i < count; i++) {
			//对每一个生成式p
			Production& p = grammar->production.GetNext();
			//将生成式右部文法符号串的FIRST集加入到生成式左部非终结符的FIRST集中
			modified |= (map[p.left].Add(GetFirst(p.right)) > 0);
		}
	} while (modified);
}

SymbolSet& First::GetFirst(Symbol s)
{
	return map[s];
}

SymbolSet First::GetFirst(List<Symbol>& symbol_string)
{
	SymbolSet first_set;

	//如果符号串的长度为0，则其FIRST集只包含空串e
	symbol_string.Rewind();
	int count = symbol_string.GetCount();
	if (count == 0) {
		first_set.has_e = true;
		return first_set;
	}

	for (int i = 0; i < count; i++) {
		//对符号串中的一个文法符号s
		Symbol s = symbol_string.GetNext();
		if (s.GetType() == T || s.GetType() == E) {
			//若s是终结符，则将其加入到结果FIRST集中，且不再测试下一个语法符号
			first_set.Add(s);
			first_set.has_e = false;
			break;
		} else {
			//若s是非终结符，则将该非终结符的FIRST集加入到结果FIRST集中，
			//如果该非终结符的FIRST集中还包含空串，则还要继续测试下一个语法符号
			first_set.Add(map[s]);
			if (!map[s].has_e) {
				first_set.has_e = false;
				break;
			}
		}
	}

	return first_set;
}