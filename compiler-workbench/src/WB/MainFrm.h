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

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__730CA820_0B39_4E93_9FBA_F2F3D2653391__INCLUDED_)
#define AFX_MAINFRM_H__730CA820_0B39_4E93_9FBA_F2F3D2653391__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProjectTree.h"
#include "grammar.h"
#include "first.h"
#include "follow.h"
#include "ll1.h"
#include "lr.h"
#include "lex.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool is_wizard;
	bool is_navigate;
	CFont font;
	CProjectTree project_tree;
	CListBox message_list;
	CSizingControlBarG project_bar;
	CSizingControlBarG message_bar;
	void SaveAll(CMultiDocTemplate* doc_template);
	void DisplayError(List<Error>& error_list);
	bool ParseGrammar(Grammar* grammar);
	void GenerateLL(bool generate_first_follow, bool generate_ll,
		Grammar* grammar, First* first, Follow* follow);
	void GenerateLR(LRDFA* lr_dfa, CString type_str, int html_type, int auto_type,
		Grammar* grammar, First* first, Follow* follow);
	bool ParseLex(Lex* lex, CString file_path, Grammar* grammar);
	void ParseLL(Lex* lex,
		Grammar* grammar, First* first, Follow* follow);
	void ParseLR(LRDFA* lr_dfa, CString type_str, Lex* lex,
		Grammar* grammar, First* first, Follow* follow);

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar    m_wndToolBar;

// Generated message map functions
public:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateAddProject(CCmdUI* pCmdUI);
	afx_msg void OnAddProject();
	afx_msg void OnUpdateSaveAll(CCmdUI* pCmdUI);
	afx_msg void OnSaveAll();
	afx_msg void OnUpdateCloseProject(CCmdUI* pCmdUI);
	afx_msg void OnCloseProject();
	afx_msg void OnClose();
	afx_msg LRESULT OnFileSel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileDel(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateGenerate(CCmdUI* pCmdUI);
	afx_msg void OnGenerate();
	afx_msg void OnUpdateParse(CCmdUI* pCmdUI);
	afx_msg void OnParse();
	afx_msg void OnUpdateProjectTree(CCmdUI* pCmdUI);
	afx_msg void OnProjectTree();
	afx_msg void OnUpdateMessage(CCmdUI* pCmdUI);
	afx_msg void OnMessage();
	afx_msg void OnUpdateNavigator(CCmdUI* pCmdUI);
	afx_msg void OnNavigator();
	afx_msg void OnUpdateWizard(CCmdUI* pCmdUI);
	afx_msg void OnWizard();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__730CA820_0B39_4E93_9FBA_F2F3D2653391__INCLUDED_)
