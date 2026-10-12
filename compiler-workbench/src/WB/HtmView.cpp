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

// HtmView.cpp : implementation of the CHtmView class
//

#include "stdafx.h"

#include "HtmDoc.h"
#include "HtmView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmView

IMPLEMENT_DYNCREATE(CHtmView, CHtmlView)

BEGIN_MESSAGE_MAP(CHtmView, CHtmlView)
	//{{AFX_MSG_MAP(CHtmView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmView construction/destruction

CHtmView::CHtmView()
{
	// TODO: add construction code here

}

CHtmView::~CHtmView()
{
}

BOOL CHtmView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CHtmView drawing

void CHtmView::OnDraw(CDC* pDC)
{
	CHtmDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CHtmView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	// TODO: This code navigates to a popular spot on the web.
	//  change the code to go where you'd like.
	if (!GetDocument()->GetPathName().IsEmpty())
		Navigate2(GetDocument()->GetPathName(), NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CHtmView diagnostics

#ifdef _DEBUG
void CHtmView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHtmView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CHtmDoc* CHtmView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHtmDoc)));
	return (CHtmDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHtmView message handlers
