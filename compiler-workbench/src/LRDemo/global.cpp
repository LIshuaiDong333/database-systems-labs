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
#include "global.h"

GotoElem::GotoElem()
{
}

GotoElem::GotoElem(Symbol s, int target)
{
	this->s = s;
	this->target = target;
}

bool GotoElem::operator==(const GotoElem& goto_elem)
{
	if (s == goto_elem.s && target == goto_elem.target)
		return true;
	else
		return false;
}

Action::Action()
{
}

Action::Action(ActionType type, int param)
{
	this->type = type;
	this->param = param;
}

bool Action::operator==(const Action& action)
{
	if (type == action.type && param == action.param)
		return true;
	else
		return false;
}

ActionElem::ActionElem()
{
}

ActionElem::ActionElem(Symbol s, Action action)
{
	this->s = s;
	this->action = action;
}

bool ActionElem::operator==(const ActionElem& action_elem)
{
	if (s == action_elem.s && action == action_elem.action)
		return true;
	else
		return false;
}
