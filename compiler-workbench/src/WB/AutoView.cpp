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

// AutoView.cpp : implementation of the CAutoView class
//

#include "stdafx.h"

#include "resource.h"
#include "AutoDoc.h"
#include "AutoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoView

IMPLEMENT_DYNCREATE(CAutoView, CScrollView)

BEGIN_MESSAGE_MAP(CAutoView, CScrollView)
	//{{AFX_MSG_MAP(CAutoView)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_AUTOTYPE0, OnAutotype0)
	ON_COMMAND(ID_AUTOTYPE1, OnAutotype1)
	ON_COMMAND(ID_TOP, OnTop)
	ON_COMMAND(ID_CENTER, OnCenter)
	ON_COMMAND(ID_BOTTOM, OnBottom)
	ON_COMMAND(ID_COPY_IMAGE, OnCopyImage)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoView construction/destruction

CAutoView::CAutoView()
{
	// TODO: add construction code here
	automaton = NULL;
}

CAutoView::~CAutoView()
{
}

BOOL CAutoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAutoView drawing

void CAutoView::OnDraw(CDC* pDC)
{
	CAutoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	automaton->DrawAll(pDC);
}

void CAutoView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	margin = 50;
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	if (GetDocument()->auto_type == 0)
		automaton = new RectAuto;
	else
		automaton = new CirAuto;
	automaton->SetTopo(GetDocument()->start,
		GetDocument()->vertex,
		GetDocument()->vertex_count,
		GetDocument()->edge,
		GetDocument()->edge_count);
	automaton->SetDefault();
	if (!GetDocument()->initiated) {
		CDC* pDC = GetDC();
		automaton->InitVertex(pDC);
		ReleaseDC(pDC);
		automaton->InitEdge();
		GetDocument()->initiated = true;
	}
	Adjust();

	selected = false;
}

/////////////////////////////////////////////////////////////////////////////
// CAutoView diagnostics

#ifdef _DEBUG
void CAutoView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAutoView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CAutoDoc* CAutoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAutoDoc)));
	return (CAutoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAutoView message handlers

void CAutoView::OnDestroy() 
{
	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here
	if (automaton != NULL)
		delete automaton;
}

void CAutoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint position = GetScrollPosition();
	selected = automaton->HitTest(point + position, selected_type, selected_id);
	if (selected) {
		SetCapture();
		original_x = point.x + position.x;
		original_y = point.y + position.y;
		oldx = point.x;
		oldy = point.y;
		CDC* pDC = GetDC();
		pDC->SetROP2(R2_NOT);
		if (selected_type == VERTEX)
			automaton->DrawVertexFrame(pDC, selected_id, -position.x, -position.y);
		else
			automaton->DrawEdgeFrame(pDC, selected_id, -position.x, -position.y);
		ReleaseDC(pDC);
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CAutoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	point += GetScrollPosition();
	if (selected) {
		if (selected_type == VERTEX)
			automaton->MoveVertex(selected_id, point.x - original_x, point.y - original_y);
		else
			automaton->MoveEdge(selected_id, point.x - original_x, point.y - original_y);
		Adjust();
		Invalidate();
		GetDocument()->SetModifiedFlag();
	}
	ReleaseCapture();
	selected = false;

	CScrollView::OnLButtonUp(nFlags, point);
}

void CAutoView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (selected) {
		CDC* pDC = GetDC();
		pDC->SetROP2(R2_NOT);
		if (selected_type == VERTEX) {
			automaton->DrawVertexFrame(pDC, selected_id, oldx - original_x, oldy - original_y);
			automaton->DrawVertexFrame(pDC, selected_id, point.x - original_x, point.y - original_y);

			oldx = point.x;
			oldy = point.y;
		} else {
			automaton->DrawEdgeFrame(pDC, selected_id, oldx - original_x, oldy - original_y);
			automaton->DrawEdgeFrame(pDC, selected_id, point.x - original_x, point.y - original_y);

			oldx = point.x;
			oldy = point.y;
		}
		ReleaseDC(pDC);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CAutoView::Adjust()
{
	CRect rect = automaton->GetTotalSize();
	CSize scroll_size;
	int offset_x, offset_y;
	scroll_size.cx = rect.Width() + 2 * margin;
	scroll_size.cy = rect.Height() + 2 * margin;
	offset_x = margin - rect.left;
	offset_y = margin - rect.top;
	SetScrollSizes(MM_TEXT, scroll_size);
	if (offset_x != 0 || offset_y != 0)
		automaton->MoveAll(offset_x, offset_y);
}

void CAutoView::ChangeType(int type)
{
	if (type == GetDocument()->auto_type)
		return;

	if (automaton !=	NULL)
		delete automaton;

	if (type == 0)
		automaton = new RectAuto;
	else
		automaton = new CirAuto;
	automaton->SetTopo(GetDocument()->start,
		GetDocument()->vertex,
		GetDocument()->vertex_count,
		GetDocument()->edge,
		GetDocument()->edge_count);
	automaton->SetDefault();

	CDC* pDC = GetDC();
	automaton->InitVertex(pDC);
	ReleaseDC(pDC);
	automaton->InitEdge();
	GetDocument()->initiated = true;
	GetDocument()->auto_type = type;
	GetDocument()->SetModifiedFlag();

	Adjust();
	Invalidate();
}

void CAutoView::ChangeAlign(Align align)
{
	automaton->SetAlign(align);
	CDC* pDC = GetDC();
	automaton->InitVertex(pDC);
	ReleaseDC(pDC);
	automaton->InitEdge();
	GetDocument()->initiated = true;
	GetDocument()->SetModifiedFlag();

	Adjust();
	Invalidate();
}
void CAutoView::OnAutotype0() 
{
	// TODO: Add your command handler code here
	ChangeType(0);
}

void CAutoView::OnAutotype1() 
{
	// TODO: Add your command handler code here
	ChangeType(1);
}

void CAutoView::OnTop() 
{
	// TODO: Add your command handler code here
	ChangeAlign(Top);
}

void CAutoView::OnCenter() 
{
	// TODO: Add your command handler code here
	ChangeAlign(Center);
}

void CAutoView::OnBottom() 
{
	// TODO: Add your command handler code here
	ChangeAlign(Bottom);
}

void CAutoView::OnCopyImage() 
{
	// TODO: Add your command handler code here
	CDC* pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CSize size = automaton->GetTotalSize().Size();
	size.cx += 2 * margin;
	size.cy += 2 * margin;

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, size.cx, size.cy);
	CBitmap* old_bitmap = memDC.SelectObject(&bitmap);

	CRect rect(CPoint(0, 0), size);
	CBrush brush(RGB(255, 255, 255));
	memDC.FillRect(rect, &brush);
	automaton->DrawAll(&memDC);
	memDC.SelectObject(old_bitmap);
	ReleaseDC(pDC);

	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bitmap.GetSafeHandle());
	CloseClipboard();

	bitmap.Detach();
}

void CAutoView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu pop_menu;
	pop_menu.LoadMenu(IDR_AUTO_MENU);
	CMenu* sub_menu = pop_menu.GetSubMenu(0);
	sub_menu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}
