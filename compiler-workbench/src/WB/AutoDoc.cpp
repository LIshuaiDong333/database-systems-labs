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

// AutoDoc.cpp : implementation of the CAutoDoc class
//

#include "stdafx.h"

#include "AutoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoDoc

IMPLEMENT_DYNCREATE(CAutoDoc, CDocument)

BEGIN_MESSAGE_MAP(CAutoDoc, CDocument)
	//{{AFX_MSG_MAP(CAutoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoDoc construction/destruction

CAutoDoc::CAutoDoc()
{
	// TODO: add one-time construction code here
	vertex = NULL;
	edge = NULL;
}

CAutoDoc::~CAutoDoc()
{
}

BOOL CAutoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	start = 0;

	vertex_count = 6;

	if (vertex != NULL)
		delete[] vertex;
	vertex = new Vertex[vertex_count];

	for (int i = 0; i < vertex_count; i++) {
		vertex[i].title.Format(_T("Title%d"), i);
		vertex[i].body = _T("Taco\nhello world");
	}

	edge_count = 9;

	if (edge != NULL)
		delete[] edge;
	edge = new Edge[edge_count];

	for (i = 0; i < edge_count; i++)
		edge[i].title.Format(_T("E%d"), i);

	edge[0].v1_id = 0;
	edge[0].v2_id = 1;
	edge[1].v1_id = 1;
	edge[1].v2_id = 2;
	edge[2].v1_id = 1;
	edge[2].v2_id = 3;
	edge[3].v1_id = 1;
	edge[3].v2_id = 4;
	edge[4].v1_id = 2;
	edge[4].v2_id = 4;
	edge[5].v1_id = 4;
	edge[5].v2_id = 2;
	edge[6].v1_id = 4;
	edge[6].v2_id = 3;
	edge[7].v1_id = 3;
	edge[7].v2_id = 5;
	edge[8].v1_id = 0;
	edge[8].v2_id = 0;

	auto_type = 0;
	initiated = false;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAutoDoc serialization

void CAutoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << start;

		ar << vertex_count;

		for (int i = 0; i < vertex_count; i++) {
			ar << vertex[i].title;
			ar << vertex[i].body;
		}

		ar << edge_count;

		for (i = 0; i < edge_count; i++) {
			ar << edge[i].v1_id;
			ar << edge[i].v2_id;
			ar << edge[i].title;
		}

		ar << auto_type;
		ar << initiated;

		if (initiated) {
			for (int i = 0; i < vertex_count; i++) {
				ar << vertex[i].pos;
				ar << vertex[i].size;
				ar << vertex[i].r;
			}
			for (i = 0; i < edge_count; i++) {
				ar << edge[i].m;
				ar << edge[i].old_v1;
				ar << edge[i].old_v2;
				ar << edge[i].old_m;
			}
		}
	}
	else
	{
		// TODO: add loading code here
		ar >> start;

		ar >> vertex_count;

		if (vertex != NULL)
			delete[] vertex;
		vertex = new Vertex[vertex_count];

		for (int i = 0; i < vertex_count; i++) {
			ar >> vertex[i].title;
			ar >> vertex[i].body;
		}

		ar >> edge_count;

		if (edge != NULL)
			delete[] edge;
		edge = new Edge[edge_count];

		for (i = 0; i < edge_count; i++) {
			ar >> edge[i].v1_id;
			ar >> edge[i].v2_id;
			ar >> edge[i].title;
		}

		ar >> auto_type;
		ar >> initiated;

		if (initiated) {
			for (int i = 0; i < vertex_count; i++) {
				ar >> vertex[i].pos;
				ar >> vertex[i].size;
				ar >> vertex[i].r;
			}
			for (i = 0; i < edge_count; i++) {
				ar >> edge[i].m;
				ar >> edge[i].old_v1;
				ar >> edge[i].old_v2;
				ar >> edge[i].old_m;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAutoDoc diagnostics

#ifdef _DEBUG
void CAutoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAutoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAutoDoc commands

void CAutoDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (vertex != NULL)
		delete[] vertex;
	if (edge != NULL)
		delete[] edge;

	CDocument::OnCloseDocument();
}
