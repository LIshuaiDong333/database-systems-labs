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

// SrcView.cpp : implementation of the CSrcView class
//

#include "stdafx.h"
#include "Demo.h"

#include "DemoDoc.h"
#include "SrcView.h"

#include "vertex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSrcView

IMPLEMENT_DYNCREATE(CSrcView, CScrollView)

BEGIN_MESSAGE_MAP(CSrcView, CScrollView)
	//{{AFX_MSG_MAP(CSrcView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
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

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSrcView drawing

void CSrcView::DrawArrow(CDC* pDC, int x, int y)
{
	POINT points[7];
	points[0].x = x;
	points[0].y = y;
	points[1].x = x + 10;
	points[1].y = y + 10;
	points[2].x = x + 5;
	points[2].y = y + 10;
	points[3].x = x + 5;
	points[3].y = y + 15;
	points[4].x = x - 5;
	points[4].y = y + 15;
	points[5].x = x - 5;
	points[5].y = y + 10;
	points[6].x = x - 10;
	points[6].y = y + 10;
	CBrush brush(RGB(255, 255, 0));
	CBrush* old_brush = pDC->SelectObject(&brush);
	pDC->Polygon(points, 7);
	pDC->SelectObject(old_brush);
}

void CSrcView::OnDraw(CDC* pDC)
{
	CDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	int left = horz_margin;
	int top = vert_margin;
	int right;
	int bottom;
	SIZE total_size = {left, top};
	CRect rect;
	GetClientRect(rect);
	CDemoDoc* doc = GetDocument();
	bool first = true;
	bool arrow_line = false;
	int lex_count = doc->lex_list.GetCount();
	for (int i = 0; i <= lex_count; i++) {
		Symbol s;
		if (i < lex_count) {
			int lex = doc->lex_list[i];
			s = Symbol(T, lex);
		} else
			s = doc->grammar.symbol_home.GetEof();
		CString symbol_str = doc->grammar.symbol_home.GetName(s);
		SIZE vertex_size;
		BareVertexSize(pDC, symbol_str, vertex_size);
		if (first || left + vertex_size.cx < rect.right - horz_margin) {
			DrawBareVertex(pDC, left, top, symbol_str);
			right = left + vertex_size.cx;
			if (right > total_size.cx)
				total_size.cx = right;
			bottom = top + vertex_size.cy;
			if (bottom > total_size.cy)
				total_size.cy = bottom;
			if (i == doc->lex_ptr) {
				DrawArrow(pDC, left + vertex_size.cx / 2, top + arrow_space);
				arrow_line = true;
			}
			left = right + horz_space;
			first = false;
		} else {
			left = horz_margin;
			if (arrow_line) {
				top += arrow_vert_space;
				arrow_line = false;
			} else
				top += vert_space;
			DrawBareVertex(pDC, left, top, symbol_str);
			right = left + vertex_size.cx;
			if (right > total_size.cx)
				total_size.cx = right;
			bottom = top + vertex_size.cy;
			if (bottom > total_size.cy)
				total_size.cy = bottom;
			if (i == doc->lex_ptr) {
				DrawArrow(pDC, left + vertex_size.cx / 2, top + arrow_space);
				arrow_line = true;
			}
			left = right + horz_space;
		}
	}
	total_size.cx += horz_margin;
	total_size.cy += vert_margin;
	SetScrollSizes(MM_TEXT, total_size);
}

void CSrcView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	horz_margin = 10;
	vert_margin = 10;
	horz_space = 10;
	vert_space = 25;
	arrow_vert_space = 40;
	arrow_space = 20;

	sizeTotal.cx = 2 * horz_margin;
	sizeTotal.cy = 2 * vert_margin;

	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CSrcView diagnostics

#ifdef _DEBUG
void CSrcView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSrcView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDemoDoc* CSrcView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemoDoc)));
	return (CDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSrcView message handlers
