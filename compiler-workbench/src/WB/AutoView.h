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

// AutoView.h : interface of the CAutoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOVIEW_H__F7508469_C1D9_48CA_A82B_FBABA7668798__INCLUDED_)
#define AFX_AUTOVIEW_H__F7508469_C1D9_48CA_A82B_FBABA7668798__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAutoView : public CScrollView
{
protected: // create from serialization only
	CAutoView();
	DECLARE_DYNCREATE(CAutoView)

// Attributes
public:
	CAutoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	void Adjust();
	void ChangeType(int type);
	void ChangeAlign(Align align);
	int margin;
	Automaton* automaton;

	bool selected;
	int selected_id;
	SelectedType selected_type;
	int original_x, original_y;
	int oldx, oldy;

	virtual ~CAutoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAutoView)
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnAutotype0();
	afx_msg void OnAutotype1();
	afx_msg void OnTop();
	afx_msg void OnCenter();
	afx_msg void OnBottom();
	afx_msg void OnCopyImage();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AutoView.cpp
inline CAutoDoc* CAutoView::GetDocument()
   { return (CAutoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOVIEW_H__F7508469_C1D9_48CA_A82B_FBABA7668798__INCLUDED_)
