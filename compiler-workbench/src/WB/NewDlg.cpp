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

// NewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "NewDlg.h"
#include "BrowseForFolder.h"
#include "project.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewDlg dialog

CNewDlg::CNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewDlg)
	DDX_Control(pDX, IDC_LIST, list);
	DDX_Control(pDX, IDC_INFO, info);
	DDX_Control(pDX, IDC_DIRECTORY, directory);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewDlg, CDialog)
	//{{AFX_MSG_MAP(CNewDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
	ON_BN_CLICKED(IDC_DIRECTORY_BUTTON, OnDirectory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewDlg message handlers

BOOL CNewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	icons.Create(32, 32, ILC_COLOR, 0, 4);
	icons.Add(AfxGetApp()->LoadIcon(IDI_TYPE0));
	icons.Add(AfxGetApp()->LoadIcon(IDI_TYPE1));
	list.SetImageList(&icons, TVSIL_NORMAL);
	list.InsertItem(0, _T("语法"), 0);
	list.InsertItem(1, _T("源文件"), 1);
	list.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

	file_path = project.project_path;
	directory.SetWindowText(file_path);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNewDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNewDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNewDlg::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pNMListView->iItem == 0)
		info.SetWindowText(_T("语法文件"));
	else
		info.SetWindowText(_T("源文件代表词法分析后形成的记号流"));
	
	*pResult = 0;
}

void CNewDlg::OnOK() 
{
	// TODO: Add extra validation here
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		AfxMessageBox(_T("请选择文件类型"));
		return;
	}
	file_type = list.GetNextSelectedItem(pos);
	GetDlgItem(IDC_EDIT)->GetWindowText(filename);
	CString old_ext = Project::GetFileExt(filename);
	static CString ext[2] = {_T(".grm"), _T(".src")};
	if (!old_ext.CompareNoCase(_T("grm")))
		file_type = 0;
	else if (!old_ext.CompareNoCase(_T("src")))
		file_type = 1;
	else
		filename += ext[file_type];

	SetCurrentDirectory(file_path);
	if (!Project::FileExist(filename)) {
		CStdioFile file;
		if (file.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeText)) {
			if (file_type == 0) {
				static LPCTSTR file_template =
					_T("[非终结符]\n\n[终结符]\n\n[文法起始符]\n\n[生成式]\n");
				file.WriteString(file_template);
			}
			filename = file.GetFilePath();
			file.Close();
			CDialog::OnOK();
		} else {
			AfxMessageBox(_T("文件名不正确"));
		}
	} else {
		AfxMessageBox(_T("文件已经存在"));
	}
}

void CNewDlg::OnDirectory()
{
	CString path;
	if (BrowseForFolder(path, this)) {
		file_path = Project::FullPath(path);
		directory.SetWindowText(file_path);
	}
}