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
#include "slr.h"

void SLRDFA::ComputeAction()
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
					//follow_set为p左部的非终结符的FOLLOW集
					SymbolSet& follow_set = follow->GetFollow(p.left);
					follow_set.Rewind();
					int symbol_count = follow_set.GetCount();
					for (int k = 0; k < symbol_count; k++) {
						//对follow_set中的每个终结符s，action[i,s]=reduce
						Symbol s = follow_set.GetNext();
						action_elem_list.Add(ActionElem(s, Action(Reduce, item.production_id)));
					}
				}
			}
		}
	}
}
