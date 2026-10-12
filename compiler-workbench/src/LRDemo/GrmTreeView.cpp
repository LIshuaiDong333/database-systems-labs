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

// GrmTreeView.cpp : implementation of the CGrmTreeView class
//

#include "stdafx.h"
#include "Demo.h"

#include "DemoDoc.h"
#include "GrmTreeView.h"

#include "vertex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrmTreeView

IMPLEMENT_DYNCREATE(CGrmTreeView, CScrollView)

BEGIN_MESSAGE_MAP(CGrmTreeView, CScrollView)
	//{{AFX_MSG_MAP(CGrmTreeView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrmTreeView construction/destruction

CGrmTreeView::CGrmTreeView()
{
	// TODO: add construction code here

}

CGrmTreeView::~CGrmTreeView()
{
}

BOOL CGrmTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGrmTreeView drawing

void CGrmTreeView::DrawNode(CDC* pDC, GrmTreeNode* node, int left, int top,
							int& total_right,
							int& total_bottom,
							int& vertex_left,
							int& vertex_right)
{
	if (node->child_list == NULL || node->child_list->GetCount() == 0) {
		SIZE vertex_size;
		VertexSize(pDC, node->str, vertex_size);
		DrawVertex(pDC, left, top, node->str, node->status);
		vertex_left = left;
		vertex_right = left + vertex_size.cx;
		total_right = vertex_right;
		total_bottom = top + vertex_size.cy;
		return;
	}

	int child_left = left;
	int child_top = top + vert_space;
	int child_total_right;
	int child_total_bottom = child_top;
	int child_total_vertex_left;
	int child_total_vertex_right;
	node->child_list->Rewind();
	int child_count = node->child_list->GetCount();
	int* child_center = new int[child_count];
	for (int i = 0; i < child_count; i++) {
		int child_bottom;
		int child_vertex_left;
		int child_vertex_right;
		GrmTreeNode* child = node->child_list->GetNext();
		DrawNode(pDC, child, child_left, child_top,
			child_total_right,
			child_bottom,
			child_vertex_left,
			child_vertex_right);
		if (child_bottom > child_total_bottom)
			child_total_bottom = child_bottom;
		if (i == 0)
			child_total_vertex_left = child_vertex_left;
		if (i == child_count - 1)
			child_total_vertex_right = child_vertex_right;
		child_center[i] = (child_vertex_left + child_vertex_right) / 2;

		child_left = child_total_right + horz_space;
	}

	SIZE vertex_size;
	VertexSize(pDC, node->str, vertex_size);
	vertex_left = (child_total_vertex_left + child_total_vertex_right - vertex_size.cx) / 2;
	if (vertex_left < left)
		vertex_left = left;
	vertex_right = vertex_left + vertex_size.cx;
	total_right = max(vertex_right, child_total_right);
	total_bottom = child_total_bottom;
	DrawVertex(pDC, vertex_left, top, node->str, node->status);

	int slice_width = vertex_size.cx / (child_count + 1);
	for (i = 0; i < child_count; i++) {
		int x1 = vertex_left + (i + 1) * slice_width;
		int y1 = top + vertex_size.cy + 3;
		pDC->MoveTo(x1, y1);
		int x2 = child_center[i];
		int y2 = child_top - 3;
		pDC->LineTo(x2, y2);
	}
	delete[] child_center;
}

void CGrmTreeView::OnDraw(CDC* pDC)
{
	CDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	int left = horz_margin;
	int top = vert_margin;
	int total_right = horz_margin;
	int total_bottom = vert_margin;
	int vertex_left, vertex_right;
	CDemoDoc* doc = (CDemoDoc*)GetDocument();
	doc->tree.Rewind();
	int child_count = doc->tree.GetCount();
	for (int i = 0; i < child_count; i++) {
		int bottom;
		GrmTreeNode* child = doc->tree.GetNext();
		DrawNode(pDC, child, left, top,
			total_right,
			bottom,
			vertex_left,
			vertex_right);
		if (bottom > total_bottom)
			total_bottom = bottom;
		left = total_right + horz_space;
	}
	SIZE total_size;
	total_size.cx = total_right + horz_margin;
	total_size.cy = total_bottom + vert_margin;
	SetScrollSizes(MM_TEXT, total_size);
}

void CGrmTreeView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	horz_margin = 50;
	vert_margin = 50;
	horz_space = 50;
	vert_space = 50;

	sizeTotal.cx = 2 * horz_margin;
	sizeTotal.cy = 2 * vert_margin;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CGrmTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	Invalidate();
	UpdateWindow();
	if (lHint == 0 || lHint == 2)
		return;
	OnScroll(MAKEWORD(SB_RIGHT, SB_TOP), 0);
}

/////////////////////////////////////////////////////////////////////////////
// CGrmTreeView diagnostics

#ifdef _DEBUG
void CGrmTreeView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGrmTreeView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDemoDoc* CGrmTreeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemoDoc)));
	return (CDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGrmTreeView message handlers

