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

// srcView.cpp : implementation of the CSrcView class
//

#include "stdafx.h"

#include "srcDoc.h"
#include "srcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSrcView

IMPLEMENT_DYNCREATE(CSrcView, CEditView)

BEGIN_MESSAGE_MAP(CSrcView, CEditView)
	//{{AFX_MSG_MAP(CSrcView)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSrcView construction/destruction

CSrcView::CSrcView()
{
	// TODO: add construction code here

}

CSrcView::~CSrcView()
{
}

BOOL CSrcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CSrcView drawing

void CSrcView::OnDraw(CDC* pDC)
{
	CSrcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CSrcView diagnostics

#ifdef _DEBUG
void CSrcView::AssertValid() const
{
	CEditView::AssertValid();
}

void CSrcView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CSrcDoc* CSrcView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSrcDoc)));
	return (CSrcDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSrcView message handlers

void CSrcView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	font.CreateFont(-16, 0, 0, 0, 400, 0, 0, 0, 134, 1, 2, 1, 49, _T("Fixedsys"));
	SetFont(&font);
	SetTabStops(16);
}

void CSrcView::OnSetFocus(CWnd* pOldWnd) 
{
	CEditView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	CreateSolidCaret(2, 16);
	ShowCaret();
}
