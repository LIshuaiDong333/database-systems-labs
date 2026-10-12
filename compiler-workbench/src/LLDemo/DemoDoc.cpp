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

// DemoDoc.cpp : implementation of the CDemoDoc class
//

#include "stdafx.h"
#include "Demo.h"

#include "DemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc

IMPLEMENT_DYNCREATE(CDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc construction/destruction

CDemoDoc::CDemoDoc()
{
	// TODO: add one-time construction code here

}

CDemoDoc::~CDemoDoc()
{
}

BOOL CDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDemoDoc serialization

Symbol CDemoDoc::ReadSymbol(CArchive& ar)
{
	int type;
	int id;
	ar >> type;
	ar >> id;
	return Symbol((SymbolType)type, id);
}

void CDemoDoc::ReadProduction(Production& p, CArchive& ar)
{
	p.left = ReadSymbol(ar);
	int symbol_count;
	ar >> symbol_count;
	for (int i = 0; i < symbol_count; i++)
		p.right.Add(ReadSymbol(ar));
}

void CDemoDoc::ReadGrammar(CArchive& ar)
{
	int t_count;
	ar >> t_count;
	int n_count;
	ar >> n_count;
	for (int i = 0; i < t_count; i++) {
		CString symbol_str;
		ar >> symbol_str;
		grammar.symbol_home.AddSymbol(symbol_str, T);
	}
	for (i = 0; i < n_count; i++) {
		CString symbol_str;
		ar >> symbol_str;
		grammar.symbol_home.AddSymbol(symbol_str, N);
	}
	CString symbol_str;
	ar >> symbol_str;
	grammar.symbol_home.AddSymbol(symbol_str, S);

	int production_count;
	ar >> production_count;
	for (i = 0; i < production_count; i++)
		ReadProduction(grammar.production.Add(Production()), ar);
}

void CDemoDoc::ReadTable(CArchive& ar)
{
	SymbolType row_type_array[] = {S, N};
	SymbolIterator row_si(grammar.symbol_home, row_type_array, 2);
	int row_count = row_si.GetCount();
	for (int i = 0; i < row_count; i++) {
		LL1Row& row = table.Add(LL1Row());
		int col_count;
		ar >> col_count;
		for (int j = 0; j < col_count; j++) {
			Symbol s = ReadSymbol(ar);
			int production_id;
			ar >> production_id;
			row[s] = production_id;
		}
	}
}

void CDemoDoc::ReadLex(CArchive& ar)
{
	int lex_count;
	ar >> lex_count;
	for (int i = 0; i < lex_count; i++) {
		int lex;
		ar >> lex;
		lex_list.Add(lex);
	}
}

void CDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		ReadGrammar(ar);
		ReadTable(ar);
		ReadLex(ar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc diagnostics

#ifdef _DEBUG
void CDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc commands

void CDemoDoc::Start() 
{
	// TODO: Add your command handler code here
	symbol_stack.Clear();
	symbol_stack.Push(grammar.symbol_home.GetEof());
	symbol_stack.Push(grammar.symbol_home.GetStart());
	lex_ptr = 0;

	is_finished = false;

	tree.Rewind();
	int child_count = tree.GetCount();
	for (int i = 0; i < child_count; i++) {
		GrmTreeNode* child = tree.GetNext();
		delete child;
	}
	tree.Clear();
	node_stack.Clear();
	GrmTreeNode* node = new GrmTreeNode;
	node->str = grammar.symbol_home.GetName(
		grammar.symbol_home.GetStart());
	tree.Add(node);
	node_stack.Push(node);
	UpdateAllViews(NULL, 1, NULL);
}

void CDemoDoc::Next() 
{
	// TODO: Add your command handler code here
	if (is_finished)
		return;

	Symbol ns = symbol_stack.GetTop();
	Symbol ts;
	if (lex_ptr < lex_list.GetCount()) {
		int lex = lex_list[lex_ptr];
		ts = Symbol(T, lex);
	} else
		ts = grammar.symbol_home.GetEof();

	if (ns.IsEof() && ts.IsEof()) {
		is_finished = true;
	} else if (ns == ts) {
		symbol_stack.Pop();
		lex_ptr++;
	} else if (ns.GetType() == T || ns.GetType() == E) {
		is_finished = true;
	} else {
		SymbolType row_type_array[] = {S, N};
		SymbolIterator row_si(grammar.symbol_home,
			row_type_array, 2);
		int row = row_si.Index(ns);
		LL1Row& ll1_row = table[row];
		if (ll1_row.Has(ts)) {
			int production_id = ll1_row[ts];
			Production& p = grammar.production[production_id];
			symbol_stack.Pop();
			GrmTreeNode* node = node_stack.Pop();
			node->child_list = new List<GrmTreeNode*>;
			int symbol_count = p.right.GetCount();
			if (symbol_count > 0) {
				for (int i = symbol_count - 1; i >= 0; i--) {
					Symbol s = p.right[i];
					symbol_stack.Push(s);
					GrmTreeNode* child = new GrmTreeNode;
					child->str = grammar.symbol_home.GetName(s);
					node->child_list->Insert(child, 0);
					if (s.GetType() == S || s.GetType() == N)
						node_stack.Push(child);
				}
			} else {
				GrmTreeNode* child = new GrmTreeNode;
				child->str = _T("ε");
				node->child_list->Insert(child, 0);
			}
		} else {
			is_finished = true;
		}
	}

	UpdateAllViews(NULL, 1, NULL);
}

void CDemoDoc::End() 
{
	// TODO: Add your command handler code here
	if (is_finished)
		return;

	while (true) {
		Symbol ns = symbol_stack.GetTop();
		Symbol ts;
		if (lex_ptr < lex_list.GetCount()) {
			int lex = lex_list[lex_ptr];
			ts = Symbol(T, lex);
		} else
			ts = grammar.symbol_home.GetEof();

		if (ns.IsEof() && ts.IsEof()) {
			is_finished = true;
			UpdateAllViews(NULL, 1, NULL);
			return;
		} else if (ns == ts) {
			symbol_stack.Pop();
			lex_ptr++;
		} else if (ns.GetType() == T || ns.GetType() == E) {
			is_finished = true;
			UpdateAllViews(NULL, 1, NULL);
			return;
		} else {
			SymbolType row_type_array[] = {S, N};
			SymbolIterator row_si(grammar.symbol_home,
				row_type_array, 2);
			int row = row_si.Index(ns);
			LL1Row& ll1_row = table[row];
			if (ll1_row.Has(ts)) {
				int production_id = ll1_row[ts];
				Production& p = grammar.production[production_id];
				symbol_stack.Pop();
				GrmTreeNode* node = node_stack.Pop();
				node->child_list = new List<GrmTreeNode*>;
				int symbol_count = p.right.GetCount();
				if (symbol_count > 0) {
					for (int i = symbol_count - 1; i >= 0; i--) {
						Symbol s = p.right[i];
						symbol_stack.Push(s);
						GrmTreeNode* child = new GrmTreeNode;
						child->str = grammar.symbol_home.GetName(s);
						node->child_list->Insert(child, 0);
						if (s.GetType() == S || s.GetType() == N)
							node_stack.Push(child);
					}
				} else {
					GrmTreeNode* child = new GrmTreeNode;
					child->str = _T("ε");
					node->child_list->Insert(child, 0);
				}
			} else {
				is_finished = true;
				UpdateAllViews(NULL, 1, NULL);
				return;
			}
		}
	}
}

BOOL CDemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	symbol_stack.Push(grammar.symbol_home.GetEof());
	symbol_stack.Push(grammar.symbol_home.GetStart());
	GrmTreeNode* node = new GrmTreeNode;
	node->str = grammar.symbol_home.GetName(
		grammar.symbol_home.GetStart());
	tree.Add(node);
	node_stack.Push(node);
	lex_ptr = 0;

	is_finished = false;

	return TRUE;
}

void CDemoDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	tree.Rewind();
	int child_count = tree.GetCount();
	for (int i = 0; i < child_count; i++) {
		GrmTreeNode* child = tree.GetNext();
		delete child;
	}

	CDocument::OnCloseDocument();
}
