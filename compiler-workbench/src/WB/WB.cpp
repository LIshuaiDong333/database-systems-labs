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

// WB.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WB.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "grmDoc.h"
#include "grmView.h"
#include "srcDoc.h"
#include "srcView.h"
#include "htmDoc.h"
#include "htmView.h"
#include "AutoDoc.h"
#include "AutoView.h"
#include "WizardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWBApp

BEGIN_MESSAGE_MAP(CWBApp, CWinApp)
	//{{AFX_MSG_MAP(CWBApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWBApp construction

CWBApp::CWBApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWBApp object

CWBApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWBApp initialization

BOOL CWBApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("编译工作台"));

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	grm_template = new CMultiDocTemplate(
		IDR_GRMTYPE,
		RUNTIME_CLASS(CGrmDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CGrmView));
	AddDocTemplate(grm_template);

	src_template = new CMultiDocTemplate(
		IDR_SRCTYPE,
		RUNTIME_CLASS(CSrcDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSrcView));
	AddDocTemplate(src_template);

	htm_template = new CMultiDocTemplate(
		IDR_HTMTYPE,
		RUNTIME_CLASS(CHtmDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CHtmView));
	AddDocTemplate(htm_template);

	auto_template = new CMultiDocTemplate(
		IDR_AUTOTYPE,
		RUNTIME_CLASS(CAutoDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CAutoView));
	AddDocTemplate(auto_template);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	m_nCmdShow = SW_SHOWMAXIMIZED;

	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileDDE) {
		m_pCmdInfo = (CCommandLineInfo*)m_nCmdShow;
		m_nCmdShow = SW_HIDE;
	}

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	if (pMainFrame->is_wizard && cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew) {
		CWizardDlg dlg;
		if (dlg.DoModal() == IDOK) {
			pMainFrame->is_wizard = (dlg.display != FALSE);
			if (dlg.select == 0) {
				static LPCTSTR filter = _T("工程文件 (*.prj)|*.prj|");
				static LPCTSTR title = _T("打开工程");
				CFileDialog dialog(TRUE, NULL, NULL,
					OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, pMainFrame);
				dialog.m_ofn.lpstrTitle = title;
				CString sample_path = Project::GetAppFilePath() + "samples";
				dialog.m_ofn.lpstrInitialDir = (LPCTSTR)sample_path;
				if (dialog.DoModal() == IDOK) {
					CString project_file_name = dialog.GetPathName();
					project.Open(project_file_name);
				}
			}
			if (dlg.select == 1) {
				pMainFrame->OnFileNew();
			}
			if (dlg.select == 2) {
				pMainFrame->OnFileOpen();
			}
		}
	}

	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
		OpenDocumentFile(cmdInfo.m_strFileName);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CFont font;
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CWBApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CWBApp message handlers


CDocument* CWBApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (Project::GetFileExt(lpszFileName).CompareNoCase(_T("prj")) == 0) {
		if (project.is_open) {
			if (!SaveAllModified())
				return NULL;
			project.Close();
		}
		CloseAllDocuments(FALSE);
		project.Open(lpszFileName);
		return NULL;
	}

	return CWinApp::OpenDocumentFile(lpszFileName);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	font.CreateFont(-35, 0, 0, 0, 700, 0, 0, 0, 134, 3, 2, 1, 49, _T("隶书"));
	GetDlgItem(IDC_STATIC1)->SetFont(&font);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
