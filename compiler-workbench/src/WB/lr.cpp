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
#include "lr.h"

void LRDFA::SetGrammar(Grammar* grammar, First* first, Follow* follow)
{
	this->grammar = grammar;
	this->first = first;
	this->follow = follow;
}

bool LRDFA::Check()
{
	goto_list.Rewind();
	action_list.Rewind();
	int item_set_count = GetItemSetCount();
	for (int i = 0; i < item_set_count; i++) {
		Set<Symbol> symbol_set;
		GotoElemList& goto_elem_list = goto_list.GetNext();
		goto_elem_list.Rewind();
		int goto_elem_count = goto_elem_list.GetCount();
		for (int j = 0; j < goto_elem_count; j++) {
			GotoElem& goto_elem = goto_elem_list.GetNext();
			symbol_set.Add(goto_elem.s);
		}
		ActionElemList& action_elem_list = action_list.GetNext();
		action_elem_list.Rewind();
		int action_elem_count = action_elem_list.GetCount();
		for (j = 0; j < action_elem_count; j++) {
			ActionElem& action_elem = action_elem_list.GetNext();
			if (symbol_set.Has(action_elem.s))
				return false;
			else
				symbol_set.Add(action_elem.s);
		}
	}
	return true;
}

bool LRDFA::GetGoto(int source, Symbol s, int& target)
{
	GotoElemList& goto_elem_list = goto_list[source];
	goto_elem_list.Rewind();
	int goto_elem_count = goto_elem_list.GetCount();
	for (int i = 0; i < goto_elem_count; i++) {
		GotoElem& goto_elem = goto_elem_list.GetNext();
		if (goto_elem.s == s) {
			target = goto_elem.target;
			return true;
		}
	}
	return false;
}

void LRDFA::GetAction(int item_set_id, Symbol s, List<Action>& action)
{
	action.Clear();

	//若go(item_set_id,s)=target，则置action[item_set_id,s]=shift target
	int target;
	if (GetGoto(item_set_id, s, target))
		action.Add(Action(Shift, target));

	ActionElemList& action_elem_list = action_list[item_set_id];
	action_elem_list.Rewind();
	int action_elem_count = action_elem_list.GetCount();
	for (int i = 0; i < action_elem_count; i++) {
		ActionElem& action_elem = action_elem_list.GetNext();
		if (action_elem.s == s)
			action.Add(action_elem.action);
	}
}

void LRDFA::SetFile(CStdioFile* file)
{
	this->file = file;
	html.SetFile(file);
}

void LRDFA::HtmlWriteSymbol(Symbol s)
{
	file->WriteString(grammar->symbol_home.GetName(s));
}

void LRDFA::HtmlWriteArrow()
{
	html.Nb();
	file->WriteString(_T("->"));
	html.Nb();
}

void LRDFA::HtmlWriteProduction(int production_id)
{
	Production& p = grammar->production[production_id];
	HtmlWriteSymbol(p.left);
	HtmlWriteArrow();
	p.right.Rewind();
	int symbol_count = p.right.GetCount();
	for (int i = 0; i < symbol_count; i++) {
		HtmlWriteSymbol(p.right.GetNext());
		if (i < symbol_count - 1)
			html.Nb();
	}
	if (symbol_count == 0)
		file->WriteString(_T("ε"));
}

void LRDFA::HtmlWriteActionTableHead()
{
	int t_count = grammar->symbol_home.GetTCount();
	int n_count = grammar->symbol_home.GetNCount();
	file->WriteString(_T("<tr>\n"));
	file->WriteString(_T("<td rowspan=\"2\" nowrap>"));
	file->WriteString(_T("<p align=\"center\">状态</p></td>\n"));
	CString info;
	info.Format(_T("<td colspan=\"%d\" nowrap>"), t_count + 1);
	file->WriteString(info);
	file->WriteString(_T("<p align=\"center\">ACTION</p></td>\n"));
	info.Format(_T("<td colspan=\"%d\" nowrap>"), n_count);
	file->WriteString(info);
	file->WriteString(_T("<p align=\"center\">GOTO</p></td>\n"));
	file->WriteString(_T("</tr>\n"));
}

void LRDFA::HtmlWriteAction(Action action)
{
	if (action.type == Shift) {
		file->WriteString(_T("shift"));
		html.Nb();
		CString info;
		info.Format(_T("%d"), action.param);
		file->WriteString(info);
	}
	if (action.type == Reduce) {
		file->WriteString(_T("reduce"));
		html.Nb();
		HtmlWriteProduction(action.param);
	}
	if (action.type == Accept) {
		file->WriteString(_T("accept"));
	}
}

void LRDFA::HtmlWriteActionCell(int item_set_id, Symbol s, int& index)
{
	html.Td();
	List<Action> action;
	GetAction(item_set_id, s, action);
	int action_count = action.GetCount();
	if (action_count > 1) {
		file->WriteString(_T("<a name=\""));
		CString info;
		info.Format(_T("%d"), index);
		file->WriteString(info);
		file->WriteString(_T("\">"));
		index++;
		html.HighLight();
	}
	action.Rewind();
	for (int i = 0; i < action_count; i++) {
		HtmlWriteAction(action.GetNext());
		if (i < action_count - 1)
			html.Br();
	}
	if (action_count > 1) {
		html.HighLight2();
		file->WriteString(_T("</a>"));
	}
	if (action_count == 0)
		html.Nb();
	html.Td2();
}

void LRDFA::HtmlWriteGotoCell(int source, Symbol s)
{
	html.Td();
	int target;
	if (GetGoto(source, s, target)) {
		CString info;
		info.Format(_T("%d"), target);
		file->WriteString(info);
	} else
		html.Nb();
	html.Td2();
}

void LRDFA::HtmlWriteTable()
{
	html.Table();
	HtmlWriteActionTableHead();
	html.Tr();
	SymbolType type_array[] = {T, E, N};
	SymbolIterator si(grammar->symbol_home, type_array, 3);
	int count = si.GetCount();
	for (int i = 0; i < count; i++) {
		html.Td();
		Symbol s = si[i];
		HtmlWriteSymbol(s);
		html.Td2();
	}
	html.Tr2();

	int index = 0;
	int item_set_count = GetItemSetCount();
	for (i = 0; i < item_set_count; i++) {
		html.Tr();
		html.Td();
		CString info;
		info.Format(_T("%d"), i);
		file->WriteString(info);
		html.Td2();

		for (int j = 0; j < count; j++) {
			Symbol s = si[j];
			if (s.GetType() != N)
				HtmlWriteActionCell(i, s, index);
			else
				HtmlWriteGotoCell(i, s);
		}
		html.Tr2();
	}
	html.Table2();
}

void LRDFA::HtmlWriteAll(LPCTSTR title)
{
	html.Head(title);
	HtmlWriteTable();
	html.Br();
	html.Br();
	file->WriteString(_T("项目集规范族："));
	html.Br();
	html.Br();
	HtmlWriteDFA();
	html.Tail();
}

void LRDFA::HtmlWriteIndex(CString table_file_name,
						 CString html_table_name)
{
	file->WriteString(_T("<html>\n<head>\n"));
	file->WriteString(_T("<title>重定义表项</title>\n"));
	file->WriteString(_T("<base target=\""));
	file->WriteString(html_table_name);
	file->WriteString(_T("\">\n</head>\n<body>\n"));
	file->WriteString(_T("<p>重定义表项</p>\n"));
	int index = 0;
	SymbolType type_array[] = {T, E};
	SymbolIterator si(grammar->symbol_home, type_array, 2);
	int count = si.GetCount();
	int item_set_count = GetItemSetCount();
	for (int i = 0; i < item_set_count; i++) {
		for (int j = 0; j < count; j++) {
			Symbol s = si[j];
			List<Action> action;
			GetAction(i, s, action);
			int action_count = action.GetCount();
			if (action_count > 1) {
				file->WriteString(_T("<p><a href=\""));
				file->WriteString(table_file_name);
				file->WriteString(_T("#"));
				CString info;
				info.Format(_T("%d"), index);
				file->WriteString(info);
				file->WriteString(_T("\">["));
				info.Format(_T("%d"), i);
				file->WriteString(info);
				file->WriteString(_T(","));
				HtmlWriteSymbol(s);
				file->WriteString(_T("]</a></p>\n"));
				index++;
			}
		}
	}
	html.Tail();
}

void LRDFA::HtmlWriteFrame(CString title,
					CString table_file_name,
					CString html_table_name,
					CString index_file_name,
					CString html_index_name)
{
	file->WriteString(_T("<html>\n<head>\n"));
	file->WriteString(_T("<title>"));
	file->WriteString(title);
	file->WriteString(_T("</title>\n"));
	file->WriteString(_T("</head>\n"));
	file->WriteString(_T("<frameset cols=\"150,*\">\n"));
	file->WriteString(_T("<frame name=\""));
	file->WriteString(html_index_name);
	file->WriteString(_T("\" target=\""));
	file->WriteString(html_table_name);
	file->WriteString(_T("\" src=\""));
	file->WriteString(index_file_name);
	file->WriteString(_T("\">\n"));
	file->WriteString(_T("<frame name=\""));
	file->WriteString(html_table_name);
	file->WriteString(_T("\" src=\""));
	file->WriteString(table_file_name);
	file->WriteString(_T("\">\n"));
	file->WriteString(_T("</frameset>\n</html>"));
}

void LRDFA::AutoWriteSymbol(Symbol s, CString& str)
{
	str += grammar->symbol_home.GetName(s);
}

void LRDFA::AutoWriteAll(CFile* file)
{
	CArchive ar(file, CArchive::store);
	//start
	ar << 0;
	//vertex
	AutoWriteDFA(ar);
	//edge_count
	int total_goto_elem_count = 0;
	goto_list.Rewind();
	int goto_count = goto_list.GetCount();
	for (int i = 0; i < goto_count; i++) {
		GotoElemList& goto_elem_list = goto_list.GetNext();
		total_goto_elem_count += goto_elem_list.GetCount();
	}
	ar << total_goto_elem_count;
	goto_list.Rewind();
	for (i = 0; i < goto_count; i++) {
		GotoElemList& goto_elem_list = goto_list.GetNext();
		goto_elem_list.Rewind();
		int goto_elem_count = goto_elem_list.GetCount();
		for (int j = 0; j < goto_elem_count; j++) {
			GotoElem& goto_elem = goto_elem_list.GetNext();
			CString title;
			AutoWriteSymbol(goto_elem.s, title);
			ar << i;
			ar << goto_elem.target;
			ar << title;
		}
	}
	//auto_type
	ar << 0;
	//initiated
	ar << false;
	ar.Close();
}

void LRDFA::DemoWriteSymbol(Symbol s, CArchive& ar)
{
	ar << (int)s.GetType();
	ar << s.GetId();
}

void LRDFA::DemoWriteProduction(int production_id, CArchive& ar)
{
	Production& p = grammar->production[production_id];
	DemoWriteSymbol(p.left, ar);
	p.right.Rewind();
	int symbol_count = p.right.GetCount();
	ar << symbol_count;
	for (int i = 0; i < symbol_count; i++)
		DemoWriteSymbol(p.right.GetNext(), ar);
}

void LRDFA::DemoWriteGrammar(CArchive& ar)
{
	ar << grammar->symbol_home.GetTCount();
	ar << grammar->symbol_home.GetNCount();
	SymbolType type_array[] = {T, N, S};
	SymbolIterator si(grammar->symbol_home, type_array, 3);
	int count = si.GetCount();
	for (int i = 0; i < count; i++) {
		Symbol s = si[i];
		ar << grammar->symbol_home.GetName(s);
	}

	grammar->production.Rewind();
	int production_count = grammar->production.GetCount();
	ar << production_count;
	for (i = 0; i < production_count; i++)
		DemoWriteProduction(i, ar);
}

void LRDFA::DemoWriteGotoElemList(GotoElemList& goto_elem_list, CArchive& ar)
{
	goto_elem_list.Rewind();
	int goto_elem_count = goto_elem_list.GetCount();
	ar << goto_elem_count;
	for (int i = 0; i < goto_elem_count; i++) {
		GotoElem& goto_elem = goto_elem_list.GetNext();
		DemoWriteSymbol(goto_elem.s, ar);
		ar << goto_elem.target;
	}
}

void LRDFA::DemoWriteGotoList(CArchive& ar)
{
	goto_list.Rewind();
	int goto_count = goto_list.GetCount();
	for (int i = 0; i < goto_count; i++) {
		GotoElemList& goto_elem_list = goto_list.GetNext();
		DemoWriteGotoElemList(goto_elem_list, ar);
	}
}

void LRDFA::DemoWriteActionElemList(ActionElemList& action_elem_list, CArchive& ar)
{
	action_elem_list.Rewind();
	int action_elem_count = action_elem_list.GetCount();
	ar << action_elem_count;
	for (int i = 0; i < action_elem_count; i++) {
		ActionElem& action_elem = action_elem_list.GetNext();
		DemoWriteSymbol(action_elem.s, ar);
		ar << (int)action_elem.action.type;
		ar << action_elem.action.param;
	}
}

void LRDFA::DemoWriteActionList(CArchive& ar)
{
	action_list.Rewind();
	int action_count = action_list.GetCount();
	for (int i = 0; i < action_count; i++) {
		ActionElemList& action_elem_list = action_list.GetNext();
		DemoWriteActionElemList(action_elem_list, ar);
	}
}

void LRDFA::DemoWriteLex(List<int>& lex_list, CArchive& ar)
{
	lex_list.Rewind();
	int lex_count = lex_list.GetCount();
	ar << lex_count;
	for (int i = 0; i < lex_count; i++) {
		int lex = lex_list.GetNext();
		ar << lex;
	}
}

void LRDFA::DemoWriteAll(List<int>& lex_list, CFile* file)
{
	CArchive ar(file, CArchive::store);
	DemoWriteGrammar(ar);
	ar << GetItemSetCount();
	DemoWriteGotoList(ar);
	DemoWriteActionList(ar);
	DemoWriteLex(lex_list, ar);
	ar.Close();
}
