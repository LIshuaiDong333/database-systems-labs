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
#include "lr1.h"
#include "html.h"

LR1Item::LR1Item()
{
}

LR1Item::LR1Item(int production_id, int dot, Symbol look_ahead) : core(production_id, dot), look_ahead(look_ahead)
{
}

bool LR1Item::operator==(const LR1Item& item)
{
	if (core == item.core && look_ahead == item.look_ahead)
		return true;
	else
		return false;
}

void LR1DFA::InitItemSet(LR1ItemSet& item_set)
{
	grammar->production.Rewind();
	int production_count = grammar->production.GetCount();
	for (int i = 0; i < production_count; i++) {
		//对文法中的每个生成式p
		Production& p = grammar->production.GetNext();
		//若p左部的非终结符是文法起始符
		if (p.left.IsStart()) {
			LR1Item item(i, 0, grammar->symbol_home.GetEof());
			item_set.Add(item);
			Closure(item_set);
			return;
		}
	}
}

void LR1DFA::Closure(LR1ItemSet& item_set)
{
	item_set.Rewind();
	for (int i = 0; i < item_set.GetCount(); i++) {
		//对于item_set中的每一个项目item，及项目对应的生成式p
		LR1Item& item = item_set.GetNext();
		Production& p = grammar->production[item.core.production_id];
		//若是归约项目，则跳过该项目
		if (item.core.dot == p.right.GetCount())
			continue;
		//s为点后的文法符号
		Symbol s = p.right[item.core.dot];
		if (s.GetType() == N) {
			grammar->production.Rewind();
			int production_count = grammar->production.GetCount();
			for (int j = 0; j < production_count; j++) {
				//对文法中的每一个左部非终结符是s的生成式p2
				Production& p2 = grammar->production.GetNext();
				if (p2.left == s) {
					p.right.Add(item.look_ahead);
					List<Symbol> symbol_string;
					p.right.GetSubList(symbol_string, item.core.dot + 1);
					//first_set是点后字符串并联接look_ahead的FIRST集
					SymbolSet first_set = first->GetFirst(symbol_string);
					symbol_string.ReleaseSubList();
					p.right.RemoveTail();
					first_set.Rewind();
					int symbol_count = first_set.GetCount();
					for (int k = 0; k < symbol_count; k++) {
						//对first_set中的每个终结符s
						Symbol s = first_set.GetNext();
						item_set.Add(LR1Item(j, 0, s));
					}
				}
			}
		}
	}
}

bool LR1DFA::Go(LR1ItemSet& source, Symbol s, LR1ItemSet& target)
{
	bool flag = false;
	target.Clear();
	source.Rewind();
	int item_count = source.GetCount();
	for (int i = 0; i < item_count; i++) {
		//对原项目集中的每一个项目item
		LR1Item& item = source.GetNext();
		Production& p = grammar->production[item.core.production_id];
		//若是归约项目，则跳过该项目
		if (item.core.dot == p.right.GetCount())
			continue;
		if (p.right[item.core.dot] == s) {
			target.Add(LR1Item(item.core.production_id, item.core.dot + 1, item.look_ahead));
			flag = true;
		}
	}
	Closure(target);
	return flag;
}

void LR1DFA::Compute()
{
	//求初始项目集
	LR1ItemSet& initial_item_set = set_list.Add(LR1ItemSet());
	InitItemSet(initial_item_set);

	set_list.Rewind();
	for (int i = 0; i < set_list.GetCount(); i++) {
		//对项目集规范族中还未求过GO函数的项目集source;
		LR1ItemSet& source = set_list.GetNext();
		LR1ItemSet target;
		GotoElemList& goto_elem_list = goto_list.Add(GotoElemList());
		SymbolType type_array[] = {N, T};
		SymbolIterator si(grammar->symbol_home, type_array, 2);
		int count = si.GetCount();
		//对每个非终结符和终结符求GO函数
		for (int j = 0; j < count; j++) {
			Symbol s = si[j];
			if (Go(source, s, target)) {
				int target_id;
				set_list.Add2(target, target_id);
				goto_elem_list.Add(GotoElem(s, target_id));
			}
		}
	}
	ComputeAction();
}

int LR1DFA::GetItemSetCount()
{
	return set_list.GetCount();
}

void LR1DFA::ComputeAction()
{
	set_list.Rewind();
	int item_set_count = set_list.GetCount();
	for (int i = 0; i < item_set_count; i++) {
		//对每一个项目集item_set
		LR1ItemSet& item_set = set_list.GetNext();
		ActionElemList& action_elem_list = action_list.Add(ActionElemList());
		item_set.Rewind();
		int item_count = item_set.GetCount();
		for (int j = 0; j < item_count; j++) {
			//对item_set中的每一个项目item，及项目对应的生成式p
			LR1Item& item = item_set.GetNext();
			Production& p = grammar->production[item.core.production_id];
			//如果是归约项目
			if (item.core.dot == p.right.GetCount()) {
				if (p.left.IsStart())
					action_elem_list.Add(ActionElem(grammar->symbol_home.GetEof(), Accept));
				else
					action_elem_list.Add(ActionElem(item.look_ahead, Action(Reduce, item.core.production_id)));
			}
		}
	}
}

void LR1DFA::HtmlWriteItem(LR1Item& item)
{
	Production& p = grammar->production[item.core.production_id];
	HtmlWriteSymbol(p.left);
	HtmlWriteArrow();
	p.right.Rewind();
	int symbol_count = p.right.GetCount();
	for (int i = 0; i < item.core.dot; i++) {
		HtmlWriteSymbol(p.right.GetNext());
		html.Nb();
	}
	file->WriteString(_T("."));
	for (; i < symbol_count; i++) {
		html.Nb();
		HtmlWriteSymbol(p.right.GetNext());
	}
	html.Nb();
	file->WriteString(_T(","));
	html.Nb();
	HtmlWriteSymbol(item.look_ahead);
}

void LR1DFA::HtmlWriteItemSet(LR1ItemSet& item_set)
{
	item_set.Rewind();
	int item_count = item_set.GetCount();
	for (int i = 0; i < item_count; i++) {
		HtmlWriteItem(item_set.GetNext());
		html.Br();
	}
}

void LR1DFA::HtmlWriteDFA()
{
	set_list.Rewind();
	int item_set_count = set_list.GetCount();
	for (int i = 0; i < item_set_count; i++) {
		LR1ItemSet& item_set = set_list.GetNext();
		CString info;
		info.Format(_T("I%d:"), i);
		file->WriteString(info);
		html.Br();
		HtmlWriteItemSet(item_set);
		html.Br();
	}
}

void LR1DFA::AutoWriteItem(LR1Item& item, CString& str)
{
	Production& p = grammar->production[item.core.production_id];
	AutoWriteSymbol(p.left, str);
	str += _T(" -> ");
	p.right.Rewind();
	int symbol_count = p.right.GetCount();
	for (int i = 0; i < item.core.dot; i++) {
		AutoWriteSymbol(p.right.GetNext(), str);
		str += _T(" ");
	}
	str += _T(".");
	for (; i < symbol_count; i++) {
		str += _T(" ");
		AutoWriteSymbol(p.right.GetNext(), str);
	}
	str += _T(" , ");
	AutoWriteSymbol(item.look_ahead, str);
}

void LR1DFA::AutoWriteItemSet(LR1ItemSet& item_set, CString& str)
{
	item_set.Rewind();
	int item_count = item_set.GetCount();
	for (int i = 0; i < item_count; i++) {
		AutoWriteItem(item_set.GetNext(), str);
		if (i != item_count - 1)
			str += _T("\n");
	}
}

void LR1DFA::AutoWriteDFA(CArchive& ar)
{
	//vertex_count
	set_list.Rewind();
	int set_count = set_list.GetCount();
	ar << set_count;
	for (int i = 0; i < set_count; i++) {
		LR1ItemSet& item_set = set_list.GetNext();
		//vertex_title
		CString title;
		title.Format(_T("I%d"), i);
		ar << title;
		//vertex_body
		CString body;
		AutoWriteItemSet(item_set, body);
		ar << body;
	}
}