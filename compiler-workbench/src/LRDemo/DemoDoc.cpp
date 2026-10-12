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
#include "MainFrm.h"

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

void CDemoDoc::ReadGotoElemList(GotoElemList& goto_elem_list, CArchive& ar)
{
	int goto_elem_count;
	ar >> goto_elem_count;
	for (int i = 0; i < goto_elem_count; i++) {
		GotoElem& goto_elem = goto_elem_list.Add(GotoElem());
		goto_elem.s = ReadSymbol(ar);
		ar >> goto_elem.target;
	}
}

void CDemoDoc::ReadGotoList(CArchive& ar)
{
	for (int i = 0; i < state_count; i++)
		ReadGotoElemList(goto_list.Add(GotoElemList()), ar);
}

void CDemoDoc::ReadActionElemList(ActionElemList& action_elem_list, CArchive& ar)
{
	int action_elem_count;
	ar >> action_elem_count;
	for (int i = 0; i < action_elem_count; i++) {
		ActionElem& action_elem = action_elem_list.Add(ActionElem());
		action_elem.s = ReadSymbol(ar);
		int type;
		ar >> type;
		action_elem.action.type = (ActionType)type;
		ar >> action_elem.action.param;
	}
}

void CDemoDoc::ReadActionList(CArchive& ar)
{
	for (int i = 0; i < state_count; i++)
		ReadActionElemList(action_list.Add(ActionElemList()), ar);
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
		ar >> state_count;
		ReadGotoList(ar);
		ReadActionList(ar);
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

bool CDemoDoc::GetGoto(int source, Symbol s, int& target)
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

bool CDemoDoc::GetAction(int item_set_id, Symbol s, Action& action)
{
	//若go(item_set_id,s)=target，则置action[item_set_id,s]=shift target
	int target;
	if (GetGoto(item_set_id, s, target)) {
		action = Action(::Shift, target);
		return true;
	}

	ActionElemList& action_elem_list = action_list[item_set_id];
	action_elem_list.Rewind();
	int action_elem_count = action_elem_list.GetCount();
	for (int i = 0; i < action_elem_count; i++) {
		ActionElem& action_elem = action_elem_list.GetNext();
		if (action_elem.s == s) {
			action = action_elem.action;
			return true;
		}
	}

	action = Action(Error);
	return false;
}

void CDemoDoc::Shift(Symbol s)
{
	GrmTreeNode* new_node = new GrmTreeNode;
	new_node->str = grammar.symbol_home.GetName(s);
	tree.Add(new_node);
	int next_state;
	GetGoto(state_stack.GetTop(), s, next_state);
	state_stack.Push(next_state);
	symbol_stack.Push(s);
	lex_ptr++;
}

void CDemoDoc::Reduce(int count, Symbol s)
{
	tree.Rewind();
	int child_count = tree.GetCount();
	int start = child_count - count;
	GrmTreeNode* new_node = new GrmTreeNode;
	new_node->str = grammar.symbol_home.GetName(s);
	new_node->child_list = tree.Divide(start);
	if (count == 0) {
		GrmTreeNode* e_node = new GrmTreeNode;
		e_node->str = _T("ε");
		new_node->child_list->Add(e_node);
	}
	tree.Add(new_node);
	for (int i = 0; i < count; i++) {
		state_stack.Pop();
		symbol_stack.Pop();
	}
	int next_state;
	GetGoto(state_stack.GetTop(), s, next_state);
	state_stack.Push(next_state);
	symbol_stack.Push(s);
}

void CDemoDoc::Reduce1(int count, Symbol s)
{
	tree.Rewind();
	int child_count = tree.GetCount();
	int start = child_count - count;
	for (int i = 0; i < child_count; i++) {
		GrmTreeNode* child = tree.GetNext();
		if (i >= start)
			child->status = 1;
	}
	reduce_count = count;
	reduce_symbol = s;
	reduce_status = 1;
	for (i = 0; i < count; i++) {
		state_stack.Pop();
		symbol_stack.Pop();
	}
}

void CDemoDoc::Reduce2()
{
	tree.Rewind();
	int child_count = tree.GetCount();
	int start = child_count - reduce_count;
	for (int i = 0; i < child_count; i++) {
		GrmTreeNode* child = tree.GetNext();
		if (i >= start)
			child->status = 0;
	}
	GrmTreeNode* new_node = new GrmTreeNode;
	new_node->str = grammar.symbol_home.GetName(reduce_symbol);
	new_node->child_list = tree.Divide(start);
	if (reduce_count == 0) {
		GrmTreeNode* e_node = new GrmTreeNode;
		e_node->str = _T("ε");
		new_node->child_list->Add(e_node);
	}
	tree.Add(new_node);
	reduce_status = 0;
	int next_state;
	GetGoto(state_stack.GetTop(), reduce_symbol, next_state);
	state_stack.Push(next_state);
	symbol_stack.Push(reduce_symbol);
}

void CDemoDoc::Start() 
{
	// TODO: Add your command handler code here
	state_stack.Clear();
	state_stack.Push(0);
	symbol_stack.Clear();
	lex_ptr = 0;

	is_finished = false;
	reduce_status = 0;

	tree.Rewind();
	int child_count = tree.GetCount();
	for (int i = 0; i < child_count; i++) {
		GrmTreeNode* child = tree.GetNext();
		delete child;
	}
	tree.Clear();
	UpdateAllViews(NULL, 2, NULL);
}

void CDemoDoc::Next() 
{
	// TODO: Add your command handler code here
	if (is_finished)
		return;

	if (reduce_status == 1) {
		Reduce2();
		UpdateAllViews(NULL, 2, NULL);
		return;
	}

	Symbol s;
	if (lex_ptr < lex_list.GetCount()) {
		int lex = lex_list[lex_ptr];
		s = Symbol(T, lex);
	} else
		s = grammar.symbol_home.GetEof();
	Action action;
	if (GetAction(state_stack.GetTop(), s, action)) {
		if (action.type == ::Shift) {
			Shift(s);
		} else if (action.type == ::Reduce) {
			Production& p = grammar.production[action.param];
			int symbol_count = p.right.GetCount();
			Reduce1(symbol_count, p.left);
		} else if (action.type == Accept) {
			is_finished = true;
		}
	} else
		is_finished = true;
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame->is_pin && action.type == ::Shift)
		UpdateAllViews(NULL, 1, NULL);
	else
		UpdateAllViews(NULL, 2, NULL);
}

void CDemoDoc::End() 
{
	// TODO: Add your command handler code here
	if (is_finished)
		return;

	if (reduce_status == 1) {
		Reduce2();
	}

	while (true) {
		Symbol s;
		if (lex_ptr < lex_list.GetCount()) {
			int lex = lex_list[lex_ptr];
			s = Symbol(T, lex);
		} else
			s = grammar.symbol_home.GetEof();
		Action action;
		if (GetAction(state_stack.GetTop(), s, action)) {
			if (action.type == ::Shift) {
				Shift(s);
			} else if (action.type == ::Reduce) {
				Production& p = grammar.production[action.param];
				int symbol_count = p.right.GetCount();
				Reduce(symbol_count, p.left);
			} else if (action.type == Accept) {
				is_finished = true;
				UpdateAllViews(NULL, 2, NULL);
				return;
			}
		} else {
			is_finished = true;
			UpdateAllViews(NULL, 2, NULL);
			return;
		}
	}
}

BOOL CDemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	state_stack.Push(0);
	lex_ptr = 0;

	is_finished = false;
	reduce_status = 0;

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
