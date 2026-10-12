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

// srcDoc.cpp : implementation of the CSrcDoc class
//

#include "stdafx.h"

#include "srcDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSrcDoc

IMPLEMENT_DYNCREATE(CSrcDoc, CDocument)

BEGIN_MESSAGE_MAP(CSrcDoc, CDocument)
	//{{AFX_MSG_MAP(CSrcDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSrcDoc construction/destruction

CSrcDoc::CSrcDoc()
{
	// TODO: add one-time construction code here

}

CSrcDoc::~CSrcDoc()
{
}

BOOL CSrcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSrcDoc serialization

void CSrcDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CSrcDoc diagnostics

#ifdef _DEBUG
void CSrcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSrcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSrcDoc commands
