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

// TableView.cpp : implementation of the CTableView class
//

#include "stdafx.h"
#include "Demo.h"

#include "DemoDoc.h"
#include "TableView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableView

IMPLEMENT_DYNCREATE(CTableView, CView)

BEGIN_MESSAGE_MAP(CTableView, CView)
	//{{AFX_MSG_MAP(CTableView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableView construction/destruction

CTableView::CTableView()
{
	// TODO: add construction code here
	grid = NULL;
}

CTableView::~CTableView()
{
	if (grid != NULL)
		delete grid;
}

BOOL CTableView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTableView drawing

void CTableView::OnDraw(CDC* pDC)
{
	CDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CTableView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (lHint != 1)
		return;
	CDemoDoc* doc = (CDemoDoc*)GetDocument();
	Symbol ns = doc->symbol_stack.GetTop();
	if (ns.GetType() != S && ns.GetType() != N) {
		grid->SetSelectedRange(-1, -1, -1, -1);
		return;
	}
	SymbolType row_type_array[] = {S, N};
	SymbolIterator row_si(doc->grammar.symbol_home,
		row_type_array, 2);
	int row = row_si.Index(ns) + 1;
	Symbol ts;
	if (doc->lex_ptr < doc->lex_list.GetCount()) {
		int lex = doc->lex_list[doc->lex_ptr];
		ts = Symbol(T, lex);
	} else
		ts = doc->grammar.symbol_home.GetEof();
	SymbolType col_type_array[] = {T, E};
	SymbolIterator col_si(doc->grammar.symbol_home,
		col_type_array, 2);
	int col = col_si.Index(ts) + 1;
	Focus(row, col);
}

/////////////////////////////////////////////////////////////////////////////
// CTableView diagnostics

#ifdef _DEBUG
void CTableView::AssertValid() const
{
	CView::AssertValid();
}

void CTableView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDemoDoc* CTableView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemoDoc)));
	return (CDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTableView message handlers

BOOL CTableView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (grid && IsWindow(grid->m_hWnd))
		if (grid->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CTableView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (grid->GetSafeHwnd()) {
		CRect rect;
		GetClientRect(rect);
		grid->MoveWindow(rect);
	}
}

void CTableView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	grid = new CGridCtrl;
	if (!grid)
		return;

	CRect rect;
	GetClientRect(rect);
	grid->Create(rect, this, IDC_TABLE);
	grid->SetEditable(FALSE);

	CDemoDoc* doc = GetDocument();
	SymbolType row_type_array[] = {S, N};
	SymbolIterator row_si(doc->grammar.symbol_home,
		row_type_array, 2);
	int row_count = row_si.GetCount() + 1;
	grid->SetRowCount(row_count);
	grid->SetFixedRowCount(1);

	SymbolType col_symbol_type[] = {T, E};
	SymbolIterator col_si(doc->grammar.symbol_home,
		col_symbol_type, 2);
	int col_count = col_si.GetCount() + 1;
	grid->SetColumnCount(col_count);
	grid->SetFixedColumnCount(1);

	int row, col;
	for (col = 1; col < col_count; col++) {
		GV_ITEM item;
		item.mask = GVIF_TEXT;
		item.row = 0;
		item.col = col;
		WriteSymbol(col_si[col - 1], item.strText);
		grid->SetItem(&item);
	}
	for (row = 1; row < row_count; row++) {
		GV_ITEM item;
		item.mask = GVIF_TEXT;
		item.row = row;
		item.col = 0;
		WriteSymbol(row_si[row - 1], item.strText);
		grid->SetItem(&item);
	}

	doc->table.Rewind();
	for (row = 1; row < row_count; row++) {
		LL1Row& ll1_row = doc->table.GetNext();
		for (col = 1; col < col_count; col++) {
			GV_ITEM item;
			item.mask = GVIF_TEXT;
			item.row = row;
			item.col = col;
			Symbol s = col_si[col - 1];
			if (ll1_row.Has(s)) {
				WriteProduction(ll1_row[s], item.strText);
				grid->SetItem(&item);
			}
		}
	}
	grid->AutoSize();
	OnUpdate(NULL, 1, NULL);

	CDemoApp* theApp = (CDemoApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->doc = GetDocument();
	pMainFrame->table_frame = (CChildFrame*)GetParentFrame();
	pMainFrame->tree_frame = (CChildFrame*)theApp->tree_template->CreateNewFrame(doc, pMainFrame);
	theApp->tree_template->InitialUpdateFrame(pMainFrame->tree_frame, doc);
	pMainFrame->src_frame = (CChildFrame*)theApp->src_template->CreateNewFrame(doc, pMainFrame);
	theApp->src_template->InitialUpdateFrame(pMainFrame->src_frame, doc);
	pMainFrame->stk_frame = (CChildFrame*)theApp->stk_template->CreateNewFrame(doc, pMainFrame);
	theApp->stk_template->InitialUpdateFrame(pMainFrame->stk_frame, doc);
}

void CTableView::WriteSymbol(Symbol s, CString& str)
{
	CDemoDoc* doc = GetDocument();
	str += doc->grammar.symbol_home.GetName(s);
}

void CTableView::WriteProduction(int production_id, CString& str)
{
	CDemoDoc* doc = GetDocument();
	Production& p = doc->grammar.production[production_id];
	WriteSymbol(p.left, str);
	str += _T(" -> ");
	p.right.Rewind();
	int symbol_count = p.right.GetCount();
	for (int i = 0; i < symbol_count; i++) {
		WriteSymbol(p.right.GetNext(), str);
		if (i < symbol_count - 1)
			str += ' ';
	}
	if (symbol_count == 0)
		str += _T("ε");
}

void CTableView::Focus(int row, int col)
{
	grid->EnsureVisible(row, col);
	grid->SetSelectedRange(row, col, row, col, TRUE);
}

