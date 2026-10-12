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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "htmlhelp.h"
#include "Demo.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_CLOSE, OnUpdateClose)
	ON_COMMAND(ID_CLOSE, OnFileClose)
	ON_UPDATE_COMMAND_UI(ID_TABLE, OnUpdateTable)
	ON_COMMAND(ID_TABLE, OnTable)
	ON_UPDATE_COMMAND_UI(ID_TREE, OnUpdateTree)
	ON_COMMAND(ID_TREE, OnTree)
	ON_UPDATE_COMMAND_UI(ID_SOURCE, OnUpdateSource)
	ON_COMMAND(ID_SOURCE, OnSource)
	ON_UPDATE_COMMAND_UI(ID_STACK, OnUpdateStack)
	ON_COMMAND(ID_STACK, OnStack)
	ON_UPDATE_COMMAND_UI(ID_START, OnUpdateStart)
	ON_COMMAND(ID_START, OnStart)
	ON_UPDATE_COMMAND_UI(ID_NEXT, OnUpdateNext)
	ON_COMMAND(ID_NEXT, OnNext)
	ON_UPDATE_COMMAND_UI(ID_END, OnUpdateEnd)
	ON_COMMAND(ID_END, OnEnd)
	ON_UPDATE_COMMAND_UI(ID_TILE, OnUpdateTile)
	ON_COMMAND(ID_TILE, OnTile)
	ON_COMMAND(ID_HELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	is_open = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0(_T("Failed to create toolbar\n"));
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::Tile()
{
	if (table_frame->IsZoomed())
		table_frame->ShowWindow(SW_RESTORE);
	if (tree_frame->IsZoomed())
		tree_frame->ShowWindow(SW_RESTORE);
	if (src_frame->IsZoomed())
		src_frame->ShowWindow(SW_RESTORE);
	if (stk_frame->IsZoomed())
		stk_frame->ShowWindow(SW_RESTORE);
	CRect rect;
	GetWindow(GW_CHILD)->GetClientRect(rect);
	int width = rect.Width() / 2;
	int height = rect.Height() / 2;
	table_frame->MoveWindow(0, 0, width, height);
	src_frame->MoveWindow(width, 0, width, height);
	stk_frame->MoveWindow(0, height, width, height);
	tree_frame->MoveWindow(width, height, width, height);
}

void CMainFrame::Open()
{
	Tile();
	table_frame->SetWindowText(_T("分析表"));
	tree_frame->SetWindowText(_T("分析树"));
	src_frame->SetWindowText(_T("输入缓冲器"));
	stk_frame->SetWindowText(_T("堆栈"));
	is_open = true;
}

CString CMainFrame::FullPath(CString path)
{
	if (path[path.GetLength() - 1] != '\\')
		path += '\\';
	return path;
}

CString CMainFrame::GetFilePath(CString path_name)
{
	int right_bound = path_name.ReverseFind('\\');
	if (right_bound != -1)
		return path_name.Left(right_bound + 1);
	else {
		CString path;
		GetCurrentDirectory(MAX_PATH, path.GetBuffer(MAX_PATH));
		path.ReleaseBuffer();
		return FullPath(path);
	}
}

CString CMainFrame::GetAppFilePath()
{
	CString app_file_path;
	LPTSTR buffer = app_file_path.GetBuffer(MAX_PATH);
	GetModuleFileName(NULL, buffer, MAX_PATH);
	app_file_path.ReleaseBuffer();
	app_file_path = GetFilePath(app_file_path);
	return app_file_path;
}

CString CMainFrame::GetHelpFilePath()
{
	return GetAppFilePath() + _T("LL帮助.chm");
}

void CMainFrame::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CString newName;
	if (AfxGetApp()->DoPromptFileName(newName, AFX_IDS_OPENFILE,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL)) {
		AfxGetApp()->CloseAllDocuments(FALSE);
		AfxGetApp()->OpenDocumentFile(newName);
	}
}

void CMainFrame::OnUpdateClose(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(is_open);
}

void CMainFrame::OnFileClose() 
{
	// TODO: Add your command handler code here
	AfxGetApp()->CloseAllDocuments(FALSE);
	is_open = false;
}

void CMainFrame::OnUpdateTable(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(is_open);
}

void CMainFrame::OnTable() 
{
	// TODO: Add your command handler code here
	table_frame->ActivateFrame(SW_SHOW);
}

void CMainFrame::OnUpdateTree(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(is_open);
}

void CMainFrame::OnTree() 
{
	// TODO: Add your command handler code here
	tree_frame->ActivateFrame(SW_SHOW);
}

void CMainFrame::OnUpdateSource(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(is_open);
}

void CMainFrame::OnSource() 
{
	// TODO: Add your command handler code here
	src_frame->ActivateFrame(SW_SHOW);
}

void CMainFrame::OnUpdateStack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(is_open);
}

void CMainFrame::OnStack() 
{
	// TODO: Add your command handler code here
	stk_frame->ActivateFrame(SW_SHOW);
}

void CMainFrame::OnUpdateStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(is_open);
}

void CMainFrame::OnStart() 
{
	// TODO: Add your command handler code here
	doc->Start();
}

void CMainFrame::OnUpdateNext(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(is_open);
}

void CMainFrame::OnNext() 
{
	// TODO: Add your command handler code here
	doc->Next();
}

void CMainFrame::OnUpdateEnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(is_open);
}

void CMainFrame::OnEnd() 
{
	// TODO: Add your command handler code here
	doc->End();
}


void CMainFrame::OnUpdateTile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(is_open);
}

void CMainFrame::OnTile() 
{
	// TODO: Add your command handler code here
	Tile();
}

void CMainFrame::OnHelp() 
{
	// TODO: Add your command handler code here
	HtmlHelp(GetSafeHwnd(),	GetHelpFilePath() + _T("::/html/界面.htm"),
		HH_DISPLAY_TOPIC, NULL);
}
