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

#ifndef GLOBAL_H
#define GLOBAL_H

#include "grammar.h"

class GotoElem
{
public:
	Symbol s;
	int target;
	GotoElem();
	GotoElem(Symbol s, int target);
	bool operator==(const GotoElem& goto_elem);
};

typedef List<GotoElem> GotoElemList;

enum ActionType {Shift, Reduce, Accept};

class Action
{
public:
	ActionType type;
	int param;
	Action();
	Action(ActionType type, int param = 0);
	bool operator==(const Action& action);
};

class ActionElem
{
public:
	Symbol s;
	Action action;
	ActionElem();
	ActionElem(Symbol s, Action action);
	bool operator==(const ActionElem& action_elem);
};

typedef List<ActionElem> ActionElemList;

#endif