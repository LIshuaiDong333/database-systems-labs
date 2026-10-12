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

#if !defined(AFX_PROJECTTREE_H__0E2EAC4E_8AB6_4BBE_8872_8F0481990760__INCLUDED_)
#define AFX_PROJECTTREE_H__0E2EAC4E_8AB6_4BBE_8872_8F0481990760__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProjectTree window

#include "project.h"

//文件夹说明：
//文件夹0：语法文件夹
//文件夹1：输出文件夹
const int folder_count = 2;

const int TREE_FILE_SEL = WM_USER + 100;
const int TREE_FILE_DEL = WM_USER + 101;

class CProjectTree : public CTreeCtrl
{
// Construction
public:
	CProjectTree();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Open();
	void Close();
	void AddFile(FileInfo* file_info);
	void RemoveFile(FileInfo* file_info);
	CFont font;
	CImageList icons;
	HTREEITEM folder[folder_count];
	int image_map[out_type_count];	//数组下标为输出文件类型type，数组内容为该类型对应的图标的id

	virtual ~CProjectTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CProjectTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelete();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTTREE_H__0E2EAC4E_8AB6_4BBE_8872_8F0481990760__INCLUDED_)
