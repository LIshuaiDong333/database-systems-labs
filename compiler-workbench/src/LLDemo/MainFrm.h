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

#if !defined(AFX_MAINFRM_H__26ABB14B_7EFC_431F_B5AF_A247E7CE3AFC__INCLUDED_)
#define AFX_MAINFRM_H__26ABB14B_7EFC_431F_B5AF_A247E7CE3AFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildFrm.h"
#include "DemoDoc.h"

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
	bool is_open;
	CDemoDoc* doc;
	CChildFrame* table_frame;
	CChildFrame* tree_frame;
	CChildFrame* src_frame;
	CChildFrame* stk_frame;
	void Tile();
	void Open();
	CString FullPath(CString path);
	CString GetFilePath(CString path_name);
	CString GetAppFilePath();
	CString GetHelpFilePath();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateClose(CCmdUI* pCmdUI);
	afx_msg void OnFileClose();
	afx_msg void OnUpdateTable(CCmdUI* pCmdUI);
	afx_msg void OnTable();
	afx_msg void OnUpdateTree(CCmdUI* pCmdUI);
	afx_msg void OnTree();
	afx_msg void OnUpdateSource(CCmdUI* pCmdUI);
	afx_msg void OnSource();
	afx_msg void OnUpdateStack(CCmdUI* pCmdUI);
	afx_msg void OnStack();
	afx_msg void OnUpdateStart(CCmdUI* pCmdUI);
	afx_msg void OnStart();
	afx_msg void OnUpdateNext(CCmdUI* pCmdUI);
	afx_msg void OnNext();
	afx_msg void OnUpdateEnd(CCmdUI* pCmdUI);
	afx_msg void OnEnd();
	afx_msg void OnUpdateTile(CCmdUI* pCmdUI);
	afx_msg void OnTile();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__26ABB14B_7EFC_431F_B5AF_A247E7CE3AFC__INCLUDED_)
