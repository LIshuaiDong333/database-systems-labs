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

#ifndef LR_H
#define LR_H

#include "grammar.h"
#include "first.h"
#include "follow.h"
#include "global.h"
#include "html.h"

class LRDFA
{
protected:
	Grammar* grammar;
	First* first;
	Follow* follow;
	virtual void ComputeAction() = 0;

	CStdioFile* file;
	Html html;
public:
	List<GotoElemList> goto_list;
	List<ActionElemList> action_list;

	void SetGrammar(Grammar* grammar, First* first, Follow* follow);
	virtual void Compute() = 0;
	virtual int GetItemSetCount() = 0;
	bool Check();
	bool GetGoto(int source, Symbol s, int& target);
	void GetAction(int item_set_id, Symbol s, List<Action>& action);

	void SetFile(CStdioFile* file);
	void HtmlWriteSymbol(Symbol s);
	void HtmlWriteArrow();
	void HtmlWriteProduction(int production_id);
	void HtmlWriteActionTableHead();
	void HtmlWriteAction(Action action);
	void HtmlWriteActionCell(int item_set_id, Symbol s, int& index);
	void HtmlWriteGotoCell(int source, Symbol s);
	void HtmlWriteTable();
	virtual void HtmlWriteDFA() = 0;
	void HtmlWriteAll(LPCTSTR title);
	void HtmlWriteIndex(CString table_file_name,
		 CString html_table_name);
	void HtmlWriteFrame(CString title,
		 CString table_file_name,
		 CString html_table_name,
		 CString index_file_name,
		 CString html_index_name);

	void AutoWriteSymbol(Symbol s, CString& str);
	virtual void AutoWriteDFA(CArchive& ar) = 0;
	void AutoWriteAll(CFile* file);

	void DemoWriteSymbol(Symbol s, CArchive& ar);
	void DemoWriteProduction(int production_id, CArchive& ar);
	void DemoWriteGrammar(CArchive& ar);
	void DemoWriteGotoElemList(GotoElemList& goto_elem_list, CArchive& ar);
	void DemoWriteGotoList(CArchive& ar);
	void DemoWriteActionElemList(ActionElemList& action_elem_list, CArchive& ar);
	void DemoWriteActionList(CArchive& ar);
	void DemoWriteLex(List<int>& lex_list, CArchive& ar);
	void DemoWriteAll(List<int>& lex_list, CFile* file);
};

#endif