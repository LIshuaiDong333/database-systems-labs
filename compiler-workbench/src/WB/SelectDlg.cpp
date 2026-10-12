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

// SelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WB.h"
#include "SelectDlg.h"
#include "project.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg dialog


CSelectDlg::CSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectDlg)
	DDX_Control(pDX, IDC_SOURCE_LIST, source_list);
	DDX_Control(pDX, IDC_GRAMMAR_LIST, grammar_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectDlg)
	ON_LBN_DBLCLK(IDC_GRAMMAR_LIST, OnDblclkGrammarList)
	ON_LBN_DBLCLK(IDC_SOURCE_LIST, OnDblclkSourceList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg message handlers

BOOL CSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	grammar_list.AddString(_T("LL1文法"));
	grammar_list.AddString(_T("LR0文法"));
	grammar_list.AddString(_T("SLR文法"));
	grammar_list.AddString(_T("LR1文法"));
	grammar_list.AddString(_T("LALR文法"));
	grammar_list.SetCurSel(0);

	POSITION pos = project.src_file_info.GetHeadPosition();
	while (pos != 0) {
		InFileInfo* file_info = &project.src_file_info.GetNext(pos);
		CString title = Project::GetFileTitle(file_info->path);
		int index = source_list.InsertString(-1, title);
		source_list.SetItemData(index, (DWORD)file_info);
	}
	source_list.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectDlg::OnOK() 
{
	// TODO: Add extra validation here
	int cur_sel = source_list.GetCurSel();
	if (cur_sel >= 0) {
		grammar_type = grammar_list.GetCurSel();
		InFileInfo* file_info = (InFileInfo*)source_list.GetItemData(cur_sel);
		source_path = file_info->path;
	} else {
		AfxMessageBox(_T("请选择源文件"));
		return;
	}
	CDialog::OnOK();
}

void CSelectDlg::OnDblclkGrammarList() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CSelectDlg::OnDblclkSourceList() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}
