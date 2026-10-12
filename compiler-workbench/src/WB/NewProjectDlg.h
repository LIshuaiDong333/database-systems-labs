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

// NewProjectDlg.h : header file
//

#if !defined(AFX_NEWPROJECTDLG_H__EAA6FC40_CB81_4CC1_B688_40247F7BB356__INCLUDED_)
#define AFX_NEWPROJECTDLG_H__EAA6FC40_CB81_4CC1_B688_40247F7BB356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNewProjectDlg dialog

#include "WhiteStatic.h"

class CNewProjectDlg : public CDialog
{
// Construction
public:
	CString project_file_name;
	CString project_path;
	CString project_name;
	CNewProjectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNewProjectDlg)
	enum { IDD = IDD_NEWPROJECT_DIALOG };
	CWhiteStatic	info;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString project_base_path;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNewProjectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDirectory();
	virtual void OnOK();
	afx_msg void OnChangeEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJECTDLG_H__EAA6FC40_CB81_4CC1_B688_40247F7BB356__INCLUDED_)
