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

#ifndef LALR_H
#define LALR_H

#include "lr1.h"

class LR1ItemPack
{
public:
	LR0Item core;
	List<Symbol> look_ahead;
};

class LR1ItemPackSet
{
	bool* present;
	LR1ItemPack* FindItem(LR1Item& item, int& pos);
public:
	List<LR1ItemPack> pack_list;

	LR1ItemPackSet();
	~LR1ItemPackSet();

	void Create(LR1ItemSet& item_set);	//由一个LR1项目集创建一个项目包集
	bool IsHomocentric(LR1ItemSet& item_set);	//判断本项目包集是否与一个LR1项目集同心
	void Merge(LR1ItemSet& item_set);	//若本项目包集与一个LR1项目集同心，则本函数将该LR1项目集归并到本项目包集
};

class LALRDFA : public LRDFA
{
	int InstallItemSet(LR1ItemSet& item_set);
	void ComputeAction();
public:
	List<LR1ItemPackSet> set_list;

	void Compute();
	void Compute(LR1DFA& lr1_dfa);
	int GetItemSetCount();

	void HtmlWritePack(LR1ItemPack& pack);
	void HtmlWritePackSet(LR1ItemPackSet& pack_set);
	void HtmlWriteDFA();

	void AutoWriteItemPack(LR1ItemPack& pack, CString& str);
	void AutoWriteItemPackSet(LR1ItemPackSet& pack_set, CString& str);
	void AutoWriteDFA(CArchive& ar);
};

#endif