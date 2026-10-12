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

#ifndef LL1_H
#define LL1_H

#include "grammar.h"
#include "first.h"
#include "follow.h"
#include "html.h"

typedef Set<int> ProductionList;
typedef Map<Symbol, ProductionList> LL1Row;
typedef List<LL1Row> LL1Table;

class LL1
{
	Grammar* grammar;
	First* first;
	Follow* follow;

	CStdioFile* file;
	Html html;
public:
	LL1Table table;
	void SetFile(CStdioFile* file);
	void SetGrammar(Grammar* grammar, First* first, Follow* follow);
	void Compute();
	bool Check();

	void HtmlWriteSymbol(Symbol s);
	void HtmlWriteArrow();
	void HtmlWriteProduction(int production_id);
	void HtmlWriteSymbolSet(SymbolSet& symbol_set);
	void HtmlWriteSymbolFirstFollow(Symbol s);
	void HtmlWriteFirstFollow(CString title);
	void HtmlWriteProductionList(ProductionList& production_list,
		 int& index);
	void HtmlWriteTable(CString title);
	void HtmlWriteIndex(CString table_file_name,
		 CString html_table_name);
	void HtmlWriteFrame(CString title,
		 CString table_file_name,
		 CString html_table_name,
		 CString index_file_name,
		 CString html_index_name);

	void DemoWriteSymbol(Symbol s, CArchive& ar);
	void DemoWriteProduction(int production_id, CArchive& ar);
	void DemoWriteGrammar(CArchive& ar);
	void DemoWriteTable(CArchive& ar);
	void DemoWriteLex(List<int>& lex_list, CArchive& ar);
	void DemoWriteAll(List<int>& lex_list, CFile* file);
};

#endif