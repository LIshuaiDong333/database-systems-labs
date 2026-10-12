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
#include "WB.h"

#include "MainFrm.h"
#include "NewDlg.h"
#include "NewProjectDlg.h"
#include "GenerateDlg.h"
#include "SelectDlg.h"
#include "grmDoc.h"

#include "lr0.h"
#include "slr.h"
#include "lr1.h"
#include "lalr.h"

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
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_ADD_PROJECT, OnUpdateAddProject)
	ON_COMMAND(ID_ADD_PROJECT, OnAddProject)
	ON_UPDATE_COMMAND_UI(ID_SAVE_ALL, OnUpdateSaveAll)
	ON_COMMAND(ID_SAVE_ALL, OnSaveAll)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_PROJECT, OnUpdateCloseProject)
	ON_COMMAND(ID_CLOSE_PROJECT, OnCloseProject)
	ON_WM_CLOSE()
	ON_MESSAGE(TREE_FILE_SEL, OnFileSel)
	ON_MESSAGE(TREE_FILE_DEL, OnFileDel)
	ON_UPDATE_COMMAND_UI(ID_GENERATE, OnUpdateGenerate)
	ON_COMMAND(ID_GENERATE, OnGenerate)
	ON_UPDATE_COMMAND_UI(ID_PARSE, OnUpdateParse)
	ON_COMMAND(ID_PARSE, OnParse)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_TREE, OnUpdateProjectTree)
	ON_COMMAND(ID_PROJECT_TREE, OnProjectTree)
	ON_UPDATE_COMMAND_UI(ID_MESSAGE, OnUpdateMessage)
	ON_COMMAND(ID_MESSAGE, OnMessage)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATOR, OnUpdateNavigator)
	ON_COMMAND(ID_NAVIGATOR, OnNavigator)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_UPDATE_COMMAND_UI(ID_WIZARD, OnUpdateWizard)
	ON_COMMAND(ID_WIZARD, OnWizard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	is_navigate = true;
}

CMainFrame::~CMainFrame()
{
}

const int IDC_PROJECT_BAR = 101;
const int IDC_MESSAGE_BAR = 102;
const int IDC_PROJECT_TREE = 103;
const int IDC_MESSAGE_LIST = 104;

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
		return -1;

	if (!project_bar.Create(_T("工程"), this, CSize(210, 210), TRUE, IDC_PROJECT_BAR))
		return -1;
	project_bar.SetSCBStyle(project_bar.GetSCBStyle() | SCBS_SHOWEDGES | SCBS_SIZECHILD);
	project_bar.SetBarStyle(project_bar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	if (!message_bar.Create(_T("信息"), this, CSize(131, 131), TRUE, IDC_MESSAGE_BAR))
		return -1;
	message_bar.SetSCBStyle(message_bar.GetSCBStyle() | SCBS_SHOWEDGES | SCBS_SIZECHILD);
	message_bar.SetBarStyle(message_bar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	if (!project_tree.Create(WS_CHILD | WS_VISIBLE |
		TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &project_bar, IDC_PROJECT_TREE))
		return -1;

	if (!message_list.Create(WS_VSCROLL | WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), &message_bar, IDC_MESSAGE_LIST))
		return -1;
	message_list.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	font.CreateFont(-16, 0, 0, 0, 400, 0, 0, 0, 134, 1, 2, 1, 49, _T("Fixedsys"));
	message_list.SetFont(&font);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	project_bar.EnableDocking(CBRS_ALIGN_ANY);
	message_bar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&project_bar, AFX_IDW_DOCKBAR_LEFT);
	DockControlBar(&message_bar, AFX_IDW_DOCKBAR_BOTTOM);

	is_navigate = (GetPrivateProfileInt(_T("WB"), _T("NAVIGATE"), 1, Project::GetINIFilePath()) != 0);
	is_wizard = (GetPrivateProfileInt(_T("WB"), _T("WIZARD"), 1, Project::GetINIFilePath()) != 0);

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


void CMainFrame::OnFileNew()
{
	// TODO: Add your command handler code here
	if (project.is_open) {
		CNewDlg dialog;
		if (dialog.DoModal() == IDOK) {
			AfxGetApp()->OpenDocumentFile(dialog.filename);
			project.AddFile(dialog.file_type, dialog.filename);
		}
	} else {
		CNewProjectDlg dialog;
		if (dialog.DoModal() == IDOK) {
			project.New(dialog.project_file_name);
		}
	}
}

void CMainFrame::OnFileOpen()
{
	// TODO: Add your command handler code here
	if (project.is_open) {
		static LPCTSTR filter = _T("语法文件 (*.grm)|*.grm|源文件 (*.src)|*.src|");
		static LPCTSTR title = _T("打开文件");
		CFileDialog dialog(TRUE, NULL, NULL,
			OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, filter, this);
		dialog.m_ofn.lpstrTitle = title;
		const int buffer_length = MAX_PATH * 10;
		TCHAR buffer[buffer_length];
		buffer[0] = NULL;
		dialog.m_ofn.lpstrFile = buffer;
		dialog.m_ofn.nMaxFile = buffer_length;
		if (dialog.DoModal() == IDOK) {
			POSITION pos = dialog.GetStartPosition();
			while (pos) {
				AfxGetApp()->OpenDocumentFile(dialog.GetNextPathName(pos));
			}
		}
	} else {
		static LPCTSTR filter = _T("工程文件 (*.prj)|*.prj|");
		static LPCTSTR title = _T("打开工程");
		CFileDialog dialog(TRUE, NULL, NULL,
			OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this);
		dialog.m_ofn.lpstrTitle = title;
		if (dialog.DoModal() == IDOK) {
			CString project_file_name = dialog.GetPathName();
			project.Open(project_file_name);
		}
	}
}

void CMainFrame::OnUpdateAddProject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(project.is_open);
}

void CMainFrame::OnAddProject() 
{
	// TODO: Add your command handler code here
	static LPCTSTR filter = _T("语法文件 (*.grm)|*.grm|源文件 (*.src)|*.src|");
	static LPCTSTR title = _T("添加工程");
	CFileDialog dialog(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, filter, this);
	dialog.m_ofn.lpstrTitle = title;
	const int buffer_length = MAX_PATH * 10;
	TCHAR buffer[buffer_length];
	buffer[0] = NULL;
	dialog.m_ofn.lpstrFile = buffer;
	dialog.m_ofn.nMaxFile = buffer_length;
	if (dialog.DoModal() == IDOK) {
		POSITION pos = dialog.GetStartPosition();
		while (pos) {
			CString file_name = dialog.GetNextPathName(pos);
			AfxGetApp()->OpenDocumentFile(file_name);
			CString ext = Project::GetFileExt(file_name);
			if (ext.CompareNoCase(_T("grm")) == 0)
				if (!project.IsDup(0, file_name))
					project.AddFile(0, file_name);
			if (ext.CompareNoCase(_T("src")) == 0)
				if (!project.IsDup(1, file_name))
					project.AddFile(1, file_name);
		}
	}
}

void CMainFrame::OnUpdateSaveAll(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(project.is_open);
}

void CMainFrame::SaveAll(CMultiDocTemplate* doc_template)
{
	POSITION pos = doc_template->GetFirstDocPosition();
	while (pos != NULL) {
		CDocument* pDoc = doc_template->GetNextDoc(pos);
		pDoc->DoFileSave();
	}
}

void CMainFrame::OnSaveAll()
{
	// TODO: Add your command handler code here
	CWBApp* theApp = (CWBApp*)AfxGetApp();
	SaveAll(theApp->grm_template);
	SaveAll(theApp->src_template);
	SaveAll(theApp->auto_template);
	project.Save();
}

void CMainFrame::OnUpdateCloseProject(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(project.is_open);
}

void CMainFrame::OnCloseProject()
{
	// TODO: Add your command handler code here
	if (!AfxGetApp()->SaveAllModified())
		return;
	AfxGetApp()->CloseAllDocuments(FALSE);
	project.Close();
	DisplayError(List<Error>());
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (project.is_open) {
		if (!AfxGetApp()->SaveAllModified())
			return;
		AfxGetApp()->CloseAllDocuments(FALSE);
		project.Close();
	}

	WritePrivateProfileString(_T("WB"), _T("NAVIGATE"), (is_navigate ? _T("1") : _T("0")), Project::GetINIFilePath());
	WritePrivateProfileString(_T("WB"), _T("WIZARD"), (is_wizard ? _T("1") : _T("0")), Project::GetINIFilePath());

	CMDIFrameWnd::OnClose();
}

LRESULT CMainFrame::OnFileSel(WPARAM wParam, LPARAM lParam)
{
	SetCurrentDirectory(project.project_path);
	FileInfo* file_info = (FileInfo*)wParam;
	AfxGetApp()->OpenDocumentFile(file_info->path);
	return 0;
}

LRESULT CMainFrame::OnFileDel(WPARAM wParam, LPARAM lParam)
{
	FileInfo* file_info = (FileInfo*)wParam;
	project.RemoveFile(file_info);
	return 0;
}


void CMainFrame::OnUpdateGenerate(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(project.is_open);
}

void CMainFrame::DisplayError(List<Error>& error_list)
{
	message_list.ResetContent();
	error_list.Rewind();
	int error_count = error_list.GetCount();
	for (int i = 0; i < error_count; i++) {
		Error& error = error_list.GetNext();
		message_list.AddString(error.GetMessage());
	}
}

bool CMainFrame::ParseGrammar(Grammar* grammar)
{
	grammar->BeginParse();
	POSITION pos = project.grm_file_info.GetHeadPosition();
	while (pos != 0) {
		InFileInfo& file_info = project.grm_file_info.GetNext(pos);
		CStdioFile f;
		if (!f.Open(file_info.path, CFile::modeRead)) {
			CString prompt(_T("没有找到 "));
			prompt += f.GetFilePath();
			prompt += _T("。");
			AfxMessageBox(prompt);
			return false;
		}
		grammar->SetFile(&f);
		grammar->ParseSymbol();
		f.Close();
	}
	pos = project.grm_file_info.GetHeadPosition();
	while (pos != 0) {
		InFileInfo& file_info = project.grm_file_info.GetNext(pos);
		CStdioFile f;
		f.Open(file_info.path, CFile::modeRead);
		grammar->SetFile(&f);
		grammar->ParseProduction();
		f.Close();
	}
	grammar->EndParse();
	DisplayError(grammar->error_list);
	if (grammar->error_list.GetCount() > 0) {
		MessageBeep((UINT)-1);
		return false;
	}
	return true;
}

void CMainFrame::GenerateLL(bool generate_first_follow, bool generate_ll,
							Grammar* grammar, First* first, Follow* follow)
{
	first->SetGrammar(grammar);
	first->Compute();

	follow->SetGrammar(grammar, first);
	follow->Compute();

	LL1 ll1;
	ll1.SetGrammar(grammar, first, follow);
	ll1.Compute();

	try {
		if (generate_first_follow) {
			CStdioFile first_follow_file(out_file_path[0],
				CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			ll1.SetFile(&first_follow_file);
			ll1.HtmlWriteFirstFollow(out_file_title[0]);
			first_follow_file.Close();
			project.AddFile(0);
		}

		if (generate_ll) {
			if (is_navigate && !ll1.Check()) {
				static CString table_file_name = _T("分析表_LL1.htm");
				static CString index_file_name = _T("索引_LL1.htm");
				static CString html_table_name = _T("table");
				static CString html_index_name = _T("index");
				CStdioFile frame_file(out_file_path[1],
					CFile::modeCreate | CFile::modeWrite | CFile::typeText);
				ll1.SetFile(&frame_file);
				ll1.HtmlWriteFrame(out_file_title[1],
					table_file_name, html_table_name,
					index_file_name, html_index_name);
				frame_file.Close();
				CStdioFile table_file(_T("输出\\") + table_file_name,
					CFile::modeCreate | CFile::modeWrite | CFile::typeText);
				ll1.SetFile(&table_file);
				ll1.HtmlWriteTable(out_file_title[1]);
				table_file.Close();
				CStdioFile index_file(_T("输出\\") + index_file_name,
					CFile::modeCreate | CFile::modeWrite | CFile::typeText);
				ll1.SetFile(&index_file);
				ll1.HtmlWriteIndex(table_file_name, html_table_name);
				index_file.Close();
				project.AddFile(1);
			} else {
				CStdioFile table_file(out_file_path[1],
					CFile::modeCreate | CFile::modeWrite | CFile::typeText);
				ll1.SetFile(&table_file);
				ll1.HtmlWriteTable(out_file_title[1]);
				table_file.Close();
				project.AddFile(1);
			}
		}
	}
	catch (...) {
		AfxMessageBox(_T("无法创建输出文件"));
	}
}

void CMainFrame::GenerateLR(LRDFA* lr_dfa, CString type_str, int html_type, int auto_type,
							Grammar* grammar, First* first, Follow* follow)
{
	lr_dfa->SetGrammar(grammar, first, follow);
	lr_dfa->Compute();

	try {
		if (is_navigate && !lr_dfa->Check()) {
			CString table_file_name = _T("分析表_");
			table_file_name += type_str;
			table_file_name += _T(".htm");
			CString index_file_name = _T("索引_");
			index_file_name += type_str;
			index_file_name += _T(".htm");
			static CString html_table_name = _T("table");
			static CString html_index_name = _T("index");
			CStdioFile frame_file(out_file_path[html_type],
				CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			lr_dfa->SetFile(&frame_file);
			lr_dfa->HtmlWriteFrame(out_file_title[html_type],
				table_file_name, html_table_name,
				index_file_name, html_index_name);
			frame_file.Close();
			CStdioFile table_file(_T("输出\\") + table_file_name,
				CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			lr_dfa->SetFile(&table_file);
			lr_dfa->HtmlWriteAll(out_file_title[html_type]);
			table_file.Close();
			CStdioFile index_file(_T("输出\\") + index_file_name,
				CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			lr_dfa->SetFile(&index_file);
			lr_dfa->HtmlWriteIndex(table_file_name, html_table_name);
			index_file.Close();
			project.AddFile(html_type);
		} else {
			CStdioFile table_file(out_file_path[html_type],
				CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			lr_dfa->SetFile(&table_file);
			lr_dfa->HtmlWriteAll(out_file_title[html_type]);
			table_file.Close();
			project.AddFile(html_type);
		}
	}
	catch (...) {
		AfxMessageBox(_T("无法创建输出文件"));
	}

	CString file_path = out_file_path[auto_type];
	CFile dfa_file;
	if (!dfa_file.Open(file_path, CFile::modeCreate | CFile::modeWrite)) {
		AfxMessageBox(_T("无法创建输出文件"));
		return;
	}
	lr_dfa->AutoWriteAll(&dfa_file);
	dfa_file.Close();
	project.AddFile(auto_type);
}

void CMainFrame::OnGenerate()
{
	// TODO: Add your command handler code here
	CGenerateDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;
	OnSaveAll();
	SetCurrentDirectory(project.project_path);
	CreateDirectory(_T("输出"), NULL);
	if (!Project::DirExist(_T("输出"))) {
		AfxMessageBox(_T("无法创建输出文件夹"));
		return;
	}

	Grammar grammar;
	if (!ParseGrammar(&grammar))
		return;

	First first;
	Follow follow;
	GenerateLL(dlg.first_follow != 0, dlg.ll1 != 0,
		&grammar, &first, &follow);

	if (!dlg.HasLR())
		return;
	if (!grammar.IsAugmented()) {
		message_list.AddString(_T("不是拓广文法"));
		MessageBeep((UINT)-1);
		return;
	}
	if (dlg.lr0) {
		LR0DFA lr0_dfa;
		GenerateLR(&lr0_dfa, _T("LR0"), 2, 3, &grammar, &first, &follow);
	}
	if (dlg.slr) {
		SLRDFA slr_dfa;
		GenerateLR(&slr_dfa, _T("SLR"), 4, 5, &grammar, &first, &follow);
	}
	if (dlg.lr1) {
		LR1DFA lr1_dfa;
		GenerateLR(&lr1_dfa, _T("LR1"), 6, 7, &grammar, &first, &follow);
	}
	if (dlg.lalr) {
		LALRDFA lalr_dfa;
		GenerateLR(&lalr_dfa, _T("LALR"), 8, 9, &grammar, &first, &follow);
	}
}

void CMainFrame::OnUpdateParse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(project.is_open);
}

bool CMainFrame::ParseLex(Lex* lex, CString file_path, Grammar* grammar)
{
	CStdioFile lex_file;
	if (!lex_file.Open(file_path, CFile::modeRead)) {
		CString prompt(_T("没有找到 "));
		prompt += lex_file.GetFilePath();
		prompt += _T("。");
		AfxMessageBox(prompt);
		return false;
	}
	lex->SetGrammar(grammar);
	lex->Parse(&lex_file);
	lex_file.Close();
	if (lex->error_list.GetCount() > 0) {
		DisplayError(lex->error_list);
		MessageBeep((UINT)-1);
		return false;
	}
	return true;
}

void CMainFrame::ParseLL(Lex* lex,
						 Grammar* grammar, First* first, Follow* follow)
{
	LL1 ll1;
	ll1.SetGrammar(grammar, first, follow);
	ll1.Compute();
	if (!ll1.Check()) {
		message_list.AddString(_T("不是LL1文法"));
		MessageBeep((UINT)-1);
		return;
	}

	CFile demo_file;
	CString file_path = _T("LL1.ll");
	if (!demo_file.Open(file_path, CFile::modeCreate | CFile::modeWrite)) {
		AfxMessageBox(_T("无法创建输出文件"));
		return;
	}
	ll1.DemoWriteAll(lex->lex_list, &demo_file);
	demo_file.Close();

	ShellExecute(NULL, NULL, Project::GetLLParserFilePath(),
		file_path, project.project_path, SW_SHOWNORMAL);
}

void CMainFrame::ParseLR(LRDFA* lr_dfa, CString type_str, Lex* lex,
			 Grammar* grammar, First* first, Follow* follow)
{
	lr_dfa->SetGrammar(grammar, first, follow);
	lr_dfa->Compute();
	if (!lr_dfa->Check()) {
		CString error_message = _T("不是");
		error_message += type_str;
		error_message += _T("文法");
		message_list.AddString(error_message);
		MessageBeep((UINT)-1);
		return;
	}

	CFile demo_file;
	CString file_path = type_str + _T(".lr");
	if (!demo_file.Open(file_path, CFile::modeCreate | CFile::modeWrite)) {
		AfxMessageBox(_T("无法创建输出文件"));
		return;
	}
	lr_dfa->DemoWriteAll(lex->lex_list, &demo_file);
	demo_file.Close();

	ShellExecute(NULL, NULL, Project::GetLRParserFilePath(),
		file_path, project.project_path, SW_SHOWNORMAL);
}

void CMainFrame::OnParse() 
{
	// TODO: Add your command handler code here
	CSelectDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	OnSaveAll();
	SetCurrentDirectory(project.project_path);

	Grammar grammar;
	if (!ParseGrammar(&grammar))
		return;

	Lex lex;
	if (!ParseLex(&lex, dlg.source_path, &grammar))
		return;

	First first;
	first.SetGrammar(&grammar);
	first.Compute();

	Follow follow;
	follow.SetGrammar(&grammar, &first);
	follow.Compute();

	if (dlg.grammar_type == 0) {
		ParseLL(&lex, &grammar, &first, &follow);
		return;
	}

	if (!grammar.IsAugmented()) {
		message_list.AddString(_T("不是拓广文法"));
		MessageBeep((UINT)-1);
		return;
	}

	if (dlg.grammar_type == 1) {
		LR0DFA lr0_dfa;
		ParseLR(&lr0_dfa, _T("LR0"), &lex, &grammar, &first, &follow);
	} else if (dlg.grammar_type == 2) {
		SLRDFA slr_dfa;
		ParseLR(&slr_dfa, _T("SLR"), &lex, &grammar, &first, &follow);
	} else if (dlg.grammar_type == 3) {
		LR1DFA lr1_dfa;
		ParseLR(&lr1_dfa, _T("LR1"), &lex, &grammar, &first, &follow);
	} else if (dlg.grammar_type == 4) {
		LALRDFA lalr_dfa;
		ParseLR(&lalr_dfa, _T("LALR"), &lex, &grammar, &first, &follow);
	}
}

void CMainFrame::OnUpdateProjectTree(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(project_bar.IsVisible());
}

void CMainFrame::OnProjectTree() 
{
	// TODO: Add your command handler code here
	ShowControlBar(&project_bar, !project_bar.IsVisible(), FALSE);
}

void CMainFrame::OnUpdateMessage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(message_bar.IsVisible());
}

void CMainFrame::OnMessage() 
{
	// TODO: Add your command handler code here
	ShowControlBar(&message_bar, !message_bar.IsVisible(), FALSE);
}

void CMainFrame::OnUpdateNavigator(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
	pCmdUI->SetCheck(is_navigate);
}

void CMainFrame::OnNavigator() 
{
	// TODO: Add your command handler code here
	is_navigate = !is_navigate;
}

void CMainFrame::OnUpdateWizard(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
	pCmdUI->SetCheck(is_wizard);
}

void CMainFrame::OnWizard() 
{
	// TODO: Add your command handler code here
	is_wizard = !is_wizard;
}

void CMainFrame::OnHelp() 
{
	// TODO: Add your command handler code here
	HtmlHelp(GetSafeHwnd(),	Project::GetHelpFilePath() + _T("::/html/概述.htm"),
		HH_DISPLAY_TOPIC, NULL);
}

