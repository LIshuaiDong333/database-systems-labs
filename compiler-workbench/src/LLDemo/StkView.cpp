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

// StkView.cpp : implementation file
//

#include "stdafx.h"
#include "Demo.h"
#include "StkView.h"

#include "vertex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStkView

IMPLEMENT_DYNCREATE(CStkView, CScrollView)

CStkView::CStkView()
{
}

CStkView::~CStkView()
{
}


BEGIN_MESSAGE_MAP(CStkView, CScrollView)
	//{{AFX_MSG_MAP(CStkView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStkView drawing

void CStkView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	horz_margin = 10;
	vert_margin = 10;
	horz_space = 10;
	vert_space = 45;

	sizeTotal.cx = 2 * horz_margin;
	sizeTotal.cy = 2 * vert_margin;

	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CStkView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
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
	doc->symbol_stack.Rewind();
	int symbol_count = doc->symbol_stack.GetCount();
	for (int i = 0; i < symbol_count; i++) {
		CString symbol_str;
		Symbol s = doc->symbol_stack.GetNext();
		symbol_str = doc->grammar.symbol_home.GetName(s);
		SIZE vertex_size;
		FrameVertexSize(pDC, symbol_str, vertex_size);
		if (first || left + vertex_size.cx < rect.right - horz_margin) {
			DrawFrameVertex(pDC, left, top, symbol_str);
			right = left + vertex_size.cx;
			if (right > total_size.cx)
				total_size.cx = right;
			bottom = top + vertex_size.cy;
			if (bottom > total_size.cy)
				total_size.cy = bottom;
			left = right + horz_space;
			first = false;
		} else {
			left = horz_margin;
			top += vert_space;
			DrawFrameVertex(pDC, left, top, symbol_str);
			right = left + vertex_size.cx;
			if (right > total_size.cx)
				total_size.cx = right;
			bottom = top + vertex_size.cy;
			if (bottom > total_size.cy)
				total_size.cy = bottom;
			left = right + horz_space;
		}
	}
	total_size.cx += horz_margin;
	total_size.cy += vert_margin;
	SetScrollSizes(MM_TEXT, total_size);
}

/////////////////////////////////////////////////////////////////////////////
// CStkView diagnostics

#ifdef _DEBUG
void CStkView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CStkView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDemoDoc* CStkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemoDoc)));
	return (CDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStkView message handlers
