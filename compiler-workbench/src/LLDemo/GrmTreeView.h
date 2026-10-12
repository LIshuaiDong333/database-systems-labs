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

// GrmTreeView.h : interface of the CGrmTreeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRMTREEVIEW_H__AB939949_F59B_4D5B_8E2D_D1841E2FFF53__INCLUDED_)
#define AFX_GRMTREEVIEW_H__AB939949_F59B_4D5B_8E2D_D1841E2FFF53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "grmtree_ds.h"

class CGrmTreeView : public CScrollView
{
protected: // create from serialization only
	CGrmTreeView();
	DECLARE_DYNCREATE(CGrmTreeView)

// Attributes
public:
	CDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrmTreeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	int horz_margin;
	int vert_margin;
	int horz_space;
	int vert_space;

	void DrawNode(CDC* pDC, GrmTreeNode* node, int left, int top,
		int& total_right,
		int& total_bottom,
		int& vertex_left,
		int& vertex_right);

	virtual ~CGrmTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGrmTreeView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GrmTreeView.cpp
inline CDemoDoc* CGrmTreeView::GetDocument()
   { return (CDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRMTREEVIEW_H__AB939949_F59B_4D5B_8E2D_D1841E2FFF53__INCLUDED_)
