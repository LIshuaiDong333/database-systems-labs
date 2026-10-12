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
#include "follow.h"

void Follow::SetGrammar(Grammar* grammar, First* first)
{
	this->grammar = grammar;
	this->first = first;
}

void Follow::Compute()
{
	map.Clear();

	//为每个非终结符在FOLLOW函数中构造一个映射
	map.Touch(grammar->symbol_home.GetStart());
	int n_count = grammar->symbol_home.GetNCount();
	for (int i = 0; i < n_count; i++)
		map.Touch(Symbol(N, i));

	//对文法开始符号S，置$于FOLLOW(S)中
	Symbol s = grammar->symbol_home.GetStart();
	map[s].Add(grammar->symbol_home.GetEof());

	bool modified;
	do {
		modified = false;

		grammar->production.Rewind();
		int production_count = grammar->production.GetCount();
		for (int i = 0; i < production_count; i++) {
			//对每一个生成式p
			Production& p = grammar->production.GetNext();
			p.right.Rewind();
			int symbol_count = p.right.GetCount();
			for (int j = 0; j < symbol_count; j++) {
				//对生成式p右部的每一个语法符号s
				Symbol s = p.right.GetNext();
				if (s.GetType() == N || s.GetType() == S) {
					//s右部的文法符号组成的串symbol_string
					List<Symbol> symbol_string;
					p.right.GetSubList(symbol_string, j + 1);
					//symbol_string的FIRST集为first_set
					SymbolSet first_set = first->GetFirst(symbol_string);
					symbol_string.ReleaseSubList();
					//如果first_set中含有空串e，则将生成式左部的非终结符的FOLLOW集加到s的FOLLOW集中
					if (first_set.has_e) {
						first_set.has_e = false;
						modified |= (map[s].Add(map[p.left]) > 0);
					}
					//将first_set加入到s的FOLLOW集中
					modified |= (map[s].Add(first_set) > 0);
				}
			}
		}
	} while (modified);
}

SymbolSet& Follow::GetFollow(Symbol s)
{
	return map[s];
}
