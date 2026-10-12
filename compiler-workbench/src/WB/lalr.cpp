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
#include "lalr.h"
#include "html.h"

LR1ItemPackSet::LR1ItemPackSet()
{
	present = NULL;
}

LR1ItemPackSet::~LR1ItemPackSet()
{
	if (present != NULL)
		delete[] present;
}

LR1ItemPack* LR1ItemPackSet::FindItem(LR1Item& item, int& pos)
{
	pack_list.Rewind();
	int pack_count = pack_list.GetCount();
	for (int i = 0; i < pack_count; i++) {
		LR1ItemPack& pack = pack_list.GetNext();
		if (pack.core == item.core) {
			pos = i;
			return &pack;
		}
	}
	return NULL;
}

void LR1ItemPackSet::Create(LR1ItemSet& item_set)
{
	if (present != NULL) {
		delete[] present;
		present = NULL;
	}

	item_set.Rewind();
	int item_count = item_set.GetCount();
	for (int i = 0; i < item_count; i++) {
		int pos;
		//对LR1项目集中的每个项目item
		LR1Item& item = item_set.GetNext();
		//在项目包集中查找是否有一个pack，使其的核心与item相同
		LR1ItemPack* pack = FindItem(item, pos);
		//若有
		if (pack != NULL)
			//将item的look_ahead添加到pack的look_ahead列表中
			pack->look_ahead.Add2(item.look_ahead);
		//若没有
		else {
			//构建一个新的项目包，该项目包的核心同item的核心，look_ahead同item的look_ahead
			LR1ItemPack& new_pack = pack_list.Add(LR1ItemPack());
			new_pack.core = item.core;
			new_pack.look_ahead.Add(item.look_ahead);
		}
	}
}

bool LR1ItemPackSet::IsHomocentric(LR1ItemSet& item_set)
{
	//pack_count为项目包的个数
	int pack_count = pack_list.GetCount();
	//present数组保存item_set中项目涉及了本项目包集中的哪些项目包
	if (present == NULL)
		present = new bool[pack_count];
	for (int i = 0; i < pack_count; i++)
		present[i] = false;
	//present_count为item_set中项目涉及本项目包集中的项目包的个数
	int present_count = 0;

	item_set.Rewind();
	int item_count = item_set.GetCount();
	for (i = 0; i < item_count; i++) {
		int pos;
		//对LR1项目集中的每个项目item
		LR1Item& item = item_set.GetNext();
		//在项目包集中查找是否有一个pack，使其的核心与item相同
		LR1ItemPack* pack = FindItem(item, pos);
		if (pack != NULL) {
			if (present[pos] == false) {
				present[pos] = true;
				present_count++;
			}
		//若没有则不同心
		} else {
			return false;
		}
	}
	//若item_set中项目涉及了本项目包集中的所有项目包
	if (present_count == pack_count)
		return true;
	else
		return false;
}

void LR1ItemPackSet::Merge(LR1ItemSet& item_set)
{
	item_set.Rewind();
	int item_count = item_set.GetCount();
	for (int i = 0; i < item_count; i++) {
		int pos;
		//对LR1项目集中的每个项目item
		LR1Item& item = item_set.GetNext();
		//在项目包集中查找是否有一个pack，使其的核心与item相同
		LR1ItemPack* pack = FindItem(item, pos);
		if (pack != NULL)
			pack->look_ahead.Add2(item.look_ahead);
	}
}

int LALRDFA::InstallItemSet(LR1ItemSet& item_set)
{
	set_list.Rewind();
	int set_count = set_list.GetCount();
	for (int i = 0; i < set_count; i++) {
		LR1ItemPackSet& pack_set = set_list.GetNext();
		if (pack_set.IsHomocentric(item_set)) {
			pack_set.Merge(item_set);
			return i;
		}
	}
	LR1ItemPackSet& pack_set = set_list.Add(LR1ItemPackSet());
	pack_set.Create(item_set);
	return set_count;
}

void LALRDFA::Compute()
{
	LR1DFA lr1_dfa;
	lr1_dfa.SetGrammar(grammar, first, follow);
	lr1_dfa.Compute();
	Compute(lr1_dfa);
}

void LALRDFA::Compute(LR1DFA& lr1_dfa)
{
	int item_set_count = lr1_dfa.set_list.GetCount();
	int* goto_map = new int[item_set_count];
	lr1_dfa.set_list.Rewind();
	for (int i = 0; i < item_set_count; i++) {
		LR1ItemSet& item_set = lr1_dfa.set_list.GetNext();
		goto_map[i] = InstallItemSet(item_set);
	}

	int lalr_goto_count = set_list.GetCount();
	for (i = 0; i < lalr_goto_count; i++)
		goto_list.Add(GotoElemList());
	lr1_dfa.goto_list.Rewind();
	int lr1_goto_count = lr1_dfa.goto_list.GetCount();
	for (i = 0; i < lr1_goto_count; i++) {
		GotoElemList& lr1_goto_elem_list = lr1_dfa.goto_list.GetNext();
		GotoElemList& lalr_goto_elem_list = goto_list[goto_map[i]];
		lr1_goto_elem_list.Rewind();
		int lr1_goto_elem_count = lr1_goto_elem_list.GetCount();
		for (int j = 0; j < lr1_goto_elem_count; j++) {
			GotoElem& lr1_goto_elem = lr1_goto_elem_list.GetNext();
			GotoElem lalr_goto_elem;
			lalr_goto_elem.s = lr1_goto_elem.s;
			lalr_goto_elem.target = goto_map[lr1_goto_elem.target];
			lalr_goto_elem_list.Add2(lalr_goto_elem);
		}
	}
	delete[] goto_map;

	ComputeAction();
}

int LALRDFA::GetItemSetCount()
{
	return set_list.GetCount();
}

void LALRDFA::ComputeAction()
{
	set_list.Rewind();
	int pack_set_count = set_list.GetCount();
	for (int i = 0; i < pack_set_count; i++) {
		//对每一个项目包集pack_set
		LR1ItemPackSet& pack_set = set_list.GetNext();
		ActionElemList& action_elem_list = action_list.Add(ActionElemList());
		pack_set.pack_list.Rewind();
		int pack_count = pack_set.pack_list.GetCount();
		for (int j = 0; j < pack_count; j++) {
			//对pack_set中的每一个项目包pack，及项目对应的生成式p
			LR1ItemPack& pack = pack_set.pack_list.GetNext();
			Production& p = grammar->production[pack.core.production_id];
			//如果是归约项目
			if (pack.core.dot == p.right.GetCount()) {
				if (p.left.IsStart())
					action_elem_list.Add(ActionElem(grammar->symbol_home.GetEof(), Accept));
				else {
					pack.look_ahead.Rewind();
					int look_ahead_count = pack.look_ahead.GetCount();
					for (int k = 0; k < look_ahead_count; k++)
						action_elem_list.Add(ActionElem(pack.look_ahead.GetNext(), Action(Reduce, pack.core.production_id)));
				}
			}
		}
	}
}

void LALRDFA::HtmlWritePack(LR1ItemPack& pack)
{
	Production& p = grammar->production[pack.core.production_id];
	HtmlWriteSymbol(p.left);
	HtmlWriteArrow();
	p.right.Rewind();
	int symbol_count = p.right.GetCount();
	for (int i = 0; i < pack.core.dot; i++) {
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
	file->WriteString(_T("["));
	html.Nb();
	pack.look_ahead.Rewind();
	int look_ahead_count = pack.look_ahead.GetCount();
	for (i = 0; i < look_ahead_count; i++) {
		HtmlWriteSymbol(pack.look_ahead.GetNext());
		if (i < look_ahead_count - 1) {
			html.Nb();
			file->WriteString(_T(","));
			html.Nb();
		} else {
			html.Nb();
			file->WriteString(_T("]"));
		}
	}
}

void LALRDFA::HtmlWritePackSet(LR1ItemPackSet& pack_set)
{
	pack_set.pack_list.Rewind();
	int pack_count = pack_set.pack_list.GetCount();
	for (int i = 0; i < pack_count; i++) {
		HtmlWritePack(pack_set.pack_list.GetNext());
		html.Br();
	}
}

void LALRDFA::HtmlWriteDFA()
{
	set_list.Rewind();
	int pack_set_count = set_list.GetCount();
	for (int i = 0; i < pack_set_count; i++) {
		LR1ItemPackSet& pack_set = set_list.GetNext();
		CString info;
		info.Format(_T("I%d:"), i);
		file->WriteString(info);
		html.Br();
		HtmlWritePackSet(pack_set);
		html.Br();
	}
}

void LALRDFA::AutoWriteItemPack(LR1ItemPack& pack, CString& str)
{
	Production& p = grammar->production[pack.core.production_id];
	AutoWriteSymbol(p.left, str);
	str += _T(" -> ");
	p.right.Rewind();
	int symbol_count = p.right.GetCount();
	for (int i = 0; i < pack.core.dot; i++) {
		AutoWriteSymbol(p.right.GetNext(), str);
		str += _T(" ");
	}
	str += _T(".");
	for (; i < symbol_count; i++) {
		str += _T(" ");
		AutoWriteSymbol(p.right.GetNext(), str);
	}
	str += _T(" , [ ");
	pack.look_ahead.Rewind();
	int look_ahead_count = pack.look_ahead.GetCount();
	for (i = 0; i < look_ahead_count; i++) {
		AutoWriteSymbol(pack.look_ahead.GetNext(), str);
		if (i < look_ahead_count - 1)
			str += _T(" , ");
		else
			str += _T(" ]");
	}
}

void LALRDFA::AutoWriteItemPackSet(LR1ItemPackSet& pack_set, CString& str)
{
	pack_set.pack_list.Rewind();
	int pack_count = pack_set.pack_list.GetCount();
	for (int i = 0; i < pack_count; i++) {
		AutoWriteItemPack(pack_set.pack_list.GetNext(), str);
		if (i != pack_count - 1)
			str += _T("\n");
	}
}

void LALRDFA::AutoWriteDFA(CArchive& ar)
{
	//vertex_count
	set_list.Rewind();
	int set_count = set_list.GetCount();
	ar << set_count;
	for (int i = 0; i < set_count; i++) {
		LR1ItemPackSet& pack_set = set_list.GetNext();
		//vertex_title
		CString title;
		title.Format(_T("I%d"), i);
		ar << title;
		//vertex_body
		CString body;
		AutoWriteItemPackSet(pack_set, body);
		ar << body;
	}
}
