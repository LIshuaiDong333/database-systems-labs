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

// DemoDoc.h : interface of the CDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMODOC_H__7977C6A3_D3CF_4D38_8C1E_1004CBF1B69F__INCLUDED_)
#define AFX_DEMODOC_H__7977C6A3_D3CF_4D38_8C1E_1004CBF1B69F__INCLUDED_

#include "symbol.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "grammar.h"
#include "global.h"
#include "grmtree_ds.h"
#include "stack.h"

class CDemoDoc : public CDocument
{
protected: // create from serialization only
	CDemoDoc();
	DECLARE_DYNCREATE(CDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	Grammar grammar;
	int state_count;
	List<GotoElemList> goto_list;
	List<ActionElemList> action_list;
	List<int> lex_list;

	Symbol ReadSymbol(CArchive& ar);
	void ReadProduction(Production& p, CArchive& ar);
	void ReadGrammar(CArchive& ar);
	void ReadGotoElemList(GotoElemList& goto_elem_list, CArchive& ar);
	void ReadGotoList(CArchive& ar);
	void ReadActionElemList(ActionElemList& action_elem_list, CArchive& ar);
	void ReadActionList(CArchive& ar);
	void ReadLex(CArchive& ar);

	bool GetGoto(int source, Symbol s, int& target);
	bool GetAction(int item_set_id, Symbol s, Action& action);

	int reduce_status;
	int reduce_count;
	Symbol reduce_symbol;
	void Shift(Symbol s);
	void Reduce(int count, Symbol s);
	void Reduce1(int count, Symbol s);
	void Reduce2();

	void Start();
	void Next();
	void End();

	List<GrmTreeNode*> tree;
	Stack<int> state_stack;
	Stack<Symbol> symbol_stack;
	int lex_ptr;

	bool is_finished;

	virtual ~CDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMODOC_H__7977C6A3_D3CF_4D38_8C1E_1004CBF1B69F__INCLUDED_)
