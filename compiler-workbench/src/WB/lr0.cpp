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
#include "lr0.h"
#include "html.h"

LR0Item::LR0Item()
{
	production_id = 0;
	dot = 0;
}

LR0Item::LR0Item(int production_id, int dot)
{
	this->production_id = production_id;
	this->dot = dot;
}

bool LR0Item::operator==(const LR0Item& item)
{
	if (production_id == item.production_id && dot == item.dot)
		return true;
	else
		return false;
}

void LR0DFA::InitItemSet(LR0ItemSet& item_set)
{
	grammar->production.Rewind();
	int production_count = grammar->production.GetCount();
	for (int i = 0; i < production_count; i++) {
		//对文法中的每个生成式p
		Production& p = grammar->production.GetNext();
		//若p左部的非终结符是文法起始符
		if (p.left.IsStart()) {
			LR0Item item(i, 0);
			item_set.Add(item);
			Closure(item_set);
			return;
		}
	}
}

void LR0DFA::Closure(LR0ItemSet& item_set)
{
	item_set.Rewind();
	for (int i = 0; i < item_set.GetCount(); i++) {
		//对于item_set中的每一个项目item，及项目对应的生成式p
		LR0Item& item = item_set.GetNext();
		Production& p = grammar->production[item.production_id];
		//若是归约项目，则跳过该项目
		if (item.dot == p.right.GetCount())
			continue;
		//s为点后的文法符号
		Symbol s = p.right[item.dot];
		if (s.GetType() == N) {
			grammar->production.Rewind();
			int production_count = grammar->production.GetCount();
			for (int j = 0; j < production_count; j++) {
				//对文法中的每一个左部非终结符是s的生成式p2
				Production& p2 = grammar->production.GetNext();
				if (p2.left == s)
					item_set.Add(LR0Item(j, 0));
			}
		}
	}
}

bool LR0DFA::Go(LR0ItemSet& source, Symbol s, LR0ItemSet& target)
{
	bool flag = false;
	target.Clear();
	source.Rewind();
	int item_count = source.GetCount();
	for (int i = 0; i < item_count; i++) {
		//对原项目集中的每一个项目item
		LR0Item& item = source.GetNext();
		Production& p = grammar->production[item.production_id];
		//若是归约项目，则跳过该项目
		if (item.dot == p.right.GetCount())
			continue;
		if (p.right[item.dot] == s) {
			target.Add(LR0Item(item.production_id, item.dot + 1));
			flag = true;
		}
	}
	Closure(target);
	return flag;
}

void LR0DFA::Compute()
{
	//求初始项目集
	LR0ItemSet& initial_item_set = set_list.Add(LR0ItemSet());
	InitItemSet(initial_item_set);

	set_list.Rewind();
	for (int i = 0; i < set_list.GetCount(); i++) {
		//对项目集规范族中还未求过GO函数的项目集source;
		LR0ItemSet& source = set_list.GetNext();
		LR0ItemSet target;
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

int LR0DFA::GetItemSetCount()
{
	return set_list.GetCount();
}

void LR0DFA::ComputeAction()
{
	set_list.Rewind();
	int item_set_count = set_list.GetCount();
	for (int i = 0; i < item_set_count; i++) {
		//对每一个项目集item_set
		LR0ItemSet& item_set = set_list.GetNext();
		ActionElemList& action_elem_list = action_list.Add(ActionElemList());
		item_set.Rewind();
		int item_count = item_set.GetCount();
		for (int j = 0; j < item_count; j++) {
			//对item_set中的每一个项目item，及项目对应的生成式p
			LR0Item& item = item_set.GetNext();
			Production& p = grammar->production[item.production_id];
			//如果是归约项目
			if (item.dot == p.right.GetCount()) {
				//若p左部的非终结符是文法起始符，则action[i,$]=accept
				if (p.left.IsStart()) {
					action_elem_list.Add(ActionElem(grammar->symbol_home.GetEof(), Accept));
				} else {
					SymbolType type_array[] = {T, E};
					SymbolIterator si(grammar->symbol_home, type_array, 2);
					int count = si.GetCount();
					for (int k = 0; k < count; k++) {
						//对每个终结符s，action[i,s]=reduce
						Symbol s = si[k];
						action_elem_list.Add(ActionElem(s, Action(Reduce, item.production_id)));
					}
				}
			}
		}
	}
}

void LR0DFA::HtmlWriteItem(LR0Item& item)
{
	Production& p = grammar->production[item.production_id];
	HtmlWriteSymbol(p.left);
	HtmlWriteArrow();
	p.right.Rewind();
	int symbol_count = p.right.GetCount();
	for (int i = 0; i < item.dot; i++) {
		HtmlWriteSymbol(p.right.GetNext());
		html.Nb();
	}
	file->WriteString(_T("."));
	for (; i < symbol_count; i++) {
		html.Nb();
		HtmlWriteSymbol(p.right.GetNext());
	}
}

void LR0DFA::HtmlWriteItemSet(LR0ItemSet& item_set)
{
	item_set.Rewind();
	int item_count = item_set.GetCount();
	for (int i = 0; i < item_count; i++) {
		html.Tab();
		HtmlWriteItem(item_set.GetNext());
		html.Br();
	}
}

void LR0DFA::HtmlWriteDFA()
{
	set_list.Rewind();
	int item_set_count = set_list.GetCount();
	for (int i = 0; i < item_set_count; i++) {
		LR0ItemSet& item_set = set_list.GetNext();
		CString info;
		info.Format(_T("I%d:"), i);
		file->WriteString(info);
		html.Br();
		HtmlWriteItemSet(item_set);
		html.Br();
	}
}

void LR0DFA::AutoWriteItem(LR0Item& item, CString& str)
{
	Production& p = grammar->production[item.production_id];
	AutoWriteSymbol(p.left, str);
	str += _T(" -> ");
	p.right.Rewind();
	int symbol_count = p.right.GetCount();
	for (int i = 0; i < item.dot; i++) {
		AutoWriteSymbol(p.right.GetNext(), str);
		str += _T(" ");
	}
	str += _T(".");
	for (; i < symbol_count; i++) {
		str += _T(" ");
		AutoWriteSymbol(p.right.GetNext(), str);
	}
}

void LR0DFA::AutoWriteItemSet(LR0ItemSet& item_set, CString& str)
{
	item_set.Rewind();
	int item_count = item_set.GetCount();
	for (int i = 0; i < item_count; i++) {
		AutoWriteItem(item_set.GetNext(), str);
		if (i != item_count - 1)
			str += _T("\n");
	}
}

void LR0DFA::AutoWriteDFA(CArchive& ar)
{
	//vertex_count
	set_list.Rewind();
	int set_count = set_list.GetCount();
	ar << set_count;
	for (int i = 0; i < set_count; i++) {
		LR0ItemSet& item_set = set_list.GetNext();
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