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
#include "ll1.h"

void LL1::SetFile(CStdioFile* file)
{
	this->file = file;
	html.SetFile(file);
}

void LL1::SetGrammar(Grammar* grammar, First* first, Follow* follow)
{
	this->grammar = grammar;
	this->first = first;
	this->follow = follow;
}

void LL1::Compute()
{
	table.Clear();
	SymbolType row_type_array[] = {S, N};
	SymbolIterator row_si(grammar->symbol_home, row_type_array, 2);
	int row_count = row_si.GetCount();
	for (int i = 0; i < row_count; i++)
		table.Add(LL1Row());
	grammar->production.Rewind();
	int production_count = grammar->production.GetCount();
	for (i = 0; i < production_count; i++) {
		Production& p = grammar->production.GetNext();
		Symbol ns = p.left;
		int row = row_si.Index(ns);
		SymbolSet first_set = first->GetFirst(p.right);
		first_set.Rewind();
		int symbol_count = first_set.GetCount();
		for (int j = 0; j < symbol_count; j++) {
			Symbol ts = first_set.GetNext();
			table[row][ts].Add(i);
		}
		if (first_set.has_e) {
			SymbolSet& follow_set = follow->GetFollow(ns);
			follow_set.Rewind();
			int symbol_count = follow_set.GetCount();
			for (int j = 0; j < symbol_count; j++) {
				Symbol ts = follow_set.GetNext();
				table[row][ts].Add(i);
			}
		}
	}
}

bool LL1::Check()
{
	table.Rewind();
	int row_count = table.GetCount();
	for (int i = 0; i < row_count; i++) {
		LL1Row& row = table.GetNext();
		row.Rewind();
		int col_count = row.GetCount();
		for (int j = 0; j < col_count; j++) {
			ProductionList& production_list = row.GetNext();
			if (production_list.GetCount() > 1)
				return false;
		}
	}
	return true;
}

void LL1::HtmlWriteSymbol(Symbol s)
{
	file->WriteString(grammar->symbol_home.GetName(s));
}

void LL1::HtmlWriteArrow()
{
	html.Nb();
	file->WriteString(_T("->"));
	html.Nb();
}

void LL1::HtmlWriteProduction(int production_id)
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

void LL1::HtmlWriteSymbolSet(SymbolSet& symbol_set)
{
	symbol_set.Rewind();
	int symbol_count = symbol_set.GetCount();
	for (int j = 0; j < symbol_count; j++) {
		Symbol s = symbol_set.GetNext();
		HtmlWriteSymbol(s);
		if (j < symbol_count - 1)
			html.Nb();
	}
	if (symbol_set.has_e) {
		html.Nb();
		file->WriteString(_T("ε"));
	}
	if (symbol_count == 0 && !symbol_set.has_e)
		html.Nb();
}

void LL1::HtmlWriteSymbolFirstFollow(Symbol s)
{
	html.Tr();
	html.Td();
	HtmlWriteSymbol(s);
	html.Td2();
	html.Td();
	SymbolSet& first_set = first->GetFirst(s);
	HtmlWriteSymbolSet(first_set);
	html.Td2();
	html.Td();
	SymbolSet& follow_set = follow->GetFollow(s);
	HtmlWriteSymbolSet(follow_set);
	html.Td2();
	html.Tr2();
}

void LL1::HtmlWriteFirstFollow(CString title)
{
	html.Head(title);
	html.Table();
	html.Tr();
	html.Td();
	file->WriteString(_T("非终结符"));
	html.Td2();
	html.Td();
	file->WriteString(_T("FIRST"));
	html.Td2();
	html.Td();
	file->WriteString(_T("FOLLOW"));
	html.Td2();
	html.Tr2();

	SymbolType type_array[] = {S, N};
	SymbolIterator si(grammar->symbol_home, type_array, 2);
	int count = si.GetCount();
	for (int i = 0; i < count; i++) {
		Symbol s = si[i];
		HtmlWriteSymbolFirstFollow(s);
	}
	html.Table2();
	html.Tail();
}

void LL1::HtmlWriteProductionList(ProductionList& production_list, int& index)
{
	production_list.Rewind();
	int production_count = production_list.GetCount();
	if (production_count > 1) {
		file->WriteString(_T("<a name=\""));
		CString info;
		info.Format(_T("%d"), index);
		file->WriteString(info);
		file->WriteString(_T("\">"));
		index++;
		html.HighLight();
	}
	for (int i = 0; i < production_count; i++) {
		HtmlWriteProduction(production_list.GetNext());
		if (i < production_count - 1)
			html.Br();
	}
	if (production_count > 1) {
		html.HighLight2();
		file->WriteString(_T("</a>"));
	}
}

void LL1::HtmlWriteTable(CString title)
{
	html.Head(title);
	html.Table();
	html.Tr();
	html.Td();
	html.Nb();
	html.Td2();
	SymbolType col_type_array[] = {T, E};
	SymbolIterator col_si(grammar->symbol_home, col_type_array, 2);
	int col_count = col_si.GetCount();
	for (int i = 0; i < col_count; i++) {
		html.Td();
		Symbol s = col_si[i];
		HtmlWriteSymbol(s);
		html.Td2();
	};
	html.Tr2();

	int index = 0;
	SymbolType row_type_array[] = {S, N};
	SymbolIterator row_si(grammar->symbol_home, row_type_array, 2);
	table.Rewind();
	int row_count = table.GetCount();
	for (i = 0; i < row_count; i++) {
		LL1Row& row = table.GetNext();
		html.Tr();
		html.Td();
		Symbol ns = row_si[i];
		HtmlWriteSymbol(ns);
		html.Td2();

		for (int j = 0; j < col_count; j++) {
			html.Td();
			Symbol ts = col_si[j];
			if (row.Has(ts))
				HtmlWriteProductionList(row[ts], index);
			else
				html.Nb();
			html.Td2();
		}
		html.Tr2();
	}
	html.Table2();
	html.Tail();
}

void LL1::HtmlWriteIndex(CString table_file_name,
						 CString html_table_name)
{
	file->WriteString(_T("<html>\n<head>\n"));
	file->WriteString(_T("<title>重定义表项</title>\n"));
	file->WriteString(_T("<base target=\""));
	file->WriteString(html_table_name);
	file->WriteString(_T("\">\n</head>\n<body>\n"));
	file->WriteString(_T("<p>重定义表项</p>\n"));
	int index = 0;
	SymbolType row_type_array[] = {S, N};
	SymbolIterator row_si(grammar->symbol_home, row_type_array, 2);
	SymbolType col_type_array[] = {T, E};
	SymbolIterator col_si(grammar->symbol_home, col_type_array, 2);
	table.Rewind();
	int row_count = table.GetCount();
	for (int i = 0; i < row_count; i++) {
		LL1Row& row = table.GetNext();
		row.Rewind();
		int col_count = col_si.GetCount();
		for (int j = 0; j < col_count; j++) {
			Symbol s = col_si[j];
			if (row.Has(s)) {
				ProductionList& production_list = row[s];
				if (production_list.GetCount() > 1) {
					file->WriteString(_T("<p><a href=\""));
					file->WriteString(table_file_name);
					file->WriteString(_T("#"));
					CString info;
					info.Format(_T("%d"), index);
					file->WriteString(info);
					file->WriteString(_T("\">["));
					HtmlWriteSymbol(row_si[i]);
					file->WriteString(_T(","));
					HtmlWriteSymbol(s);
					file->WriteString(_T("]</a></p>\n"));
					index++;
				}
			}
		}
	}
	html.Tail();
}

void LL1::HtmlWriteFrame(CString title,
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

void LL1::DemoWriteSymbol(Symbol s, CArchive& ar)
{
	ar << (int)s.GetType();
	ar << s.GetId();
}

void LL1::DemoWriteProduction(int production_id, CArchive& ar)
{
	Production& p = grammar->production[production_id];
	DemoWriteSymbol(p.left, ar);
	p.right.Rewind();
	int symbol_count = p.right.GetCount();
	ar << symbol_count;
	for (int i = 0; i < symbol_count; i++)
		DemoWriteSymbol(p.right.GetNext(), ar);
}

void LL1::DemoWriteGrammar(CArchive& ar)
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

void LL1::DemoWriteTable(CArchive& ar)
{
	table.Rewind();
	int row_count = table.GetCount();
	for (int i = 0; i < row_count; i++) {
		LL1Row& row = table.GetNext();
		row.Rewind();
		int col_count = row.GetCount();
		ar << col_count;
		for (int j = 0; j < col_count; j++) {
			Symbol s;
			ProductionList& production_list = row.GetNext(s);
			DemoWriteSymbol(s, ar);
			production_list.Rewind();
			ar << production_list.GetNext();
		}
	}
}

void LL1::DemoWriteLex(List<int>& lex_list, CArchive& ar)
{
	lex_list.Rewind();
	int lex_count = lex_list.GetCount();
	ar << lex_count;
	for (int i = 0; i < lex_count; i++) {
		int lex = lex_list.GetNext();
		ar << lex;
	}
}

void LL1::DemoWriteAll(List<int>& lex_list, CFile* file)
{
	CArchive ar(file, CArchive::store);
	DemoWriteGrammar(ar);
	DemoWriteTable(ar);
	DemoWriteLex(lex_list, ar);
	ar.Close();
}
