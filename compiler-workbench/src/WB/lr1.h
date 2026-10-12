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

#ifndef LR1_H
#define LR1_H

#include "lr0.h"

class LR1Item
{
public:
	LR0Item core;
	Symbol look_ahead;
	LR1Item();
	LR1Item(int production_id, int dot, Symbol look_ahead);
	bool operator==(const LR1Item& item);
};

typedef Set<LR1Item> LR1ItemSet;

class LR1DFA : public LRDFA
{
	void InitItemSet(LR1ItemSet& item_set);	//将初始项目加入item_set
	void Closure(LR1ItemSet& item_set);	//求item_set的闭包，新项目直接加入到item_set
	bool Go(LR1ItemSet& source, Symbol s, LR1ItemSet& target);
	void ComputeAction();
public:
	List<LR1ItemSet> set_list;	//set_list为项目集规范族
	void Compute();
	int GetItemSetCount();

	void HtmlWriteItem(LR1Item& item);
	void HtmlWriteItemSet(LR1ItemSet& item_set);
	void HtmlWriteDFA();

	void AutoWriteItem(LR1Item& item, CString& str);
	void AutoWriteItemSet(LR1ItemSet& item_set, CString& str);
	void AutoWriteDFA(CArchive& ar);
};

#endif