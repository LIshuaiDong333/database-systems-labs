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

// GenerateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WB.h"
#include "GenerateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenerateDlg dialog


CGenerateDlg::CGenerateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGenerateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenerateDlg)
	ll1 = FALSE;
	lr0 = FALSE;
	slr = FALSE;
	lr1 = FALSE;
	lalr = FALSE;
	first_follow = FALSE;
	all = FALSE;
	//}}AFX_DATA_INIT
}


void CGenerateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenerateDlg)
	DDX_Check(pDX, IDC_CHECK1, ll1);
	DDX_Check(pDX, IDC_CHECK2, lr0);
	DDX_Check(pDX, IDC_CHECK3, slr);
	DDX_Check(pDX, IDC_CHECK4, lr1);
	DDX_Check(pDX, IDC_CHECK5, lalr);
	DDX_Check(pDX, IDC_CHECK6, first_follow);
	DDX_Check(pDX, IDC_CHECK7, all);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenerateDlg, CDialog)
	//{{AFX_MSG_MAP(CGenerateDlg)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenerateDlg message handlers

bool CGenerateDlg::HasLR()
{
	if (lr0 || slr || lr1 || lalr)
		return true;
	else
		return false;
}

void CGenerateDlg::OnCheck7() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (all) {
		ll1 = TRUE;
		lr0 = TRUE;
		slr = TRUE;
		lr1 = TRUE;
		lalr = TRUE;
		first_follow = TRUE;
	} else {
		ll1 = FALSE;
		lr0 = FALSE;
		slr = FALSE;
		lr1 = FALSE;
		lalr = FALSE;
		first_follow = FALSE;
	}
	UpdateData(FALSE);
}
