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

// ProjectTree.cpp : implementation file
//

#include "stdafx.h"
#include "WB.h"
#include "ProjectTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectTree

CProjectTree::CProjectTree()
{
}

CProjectTree::~CProjectTree()
{
}


BEGIN_MESSAGE_MAP(CProjectTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CProjectTree)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectTree message handlers

int CProjectTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	ModifyStyleEx(0, WS_EX_DLGMODALFRAME);
	font.CreateFont(-16, 0, 0, 0, 400, 0, 0, 0, 134, 1, 2, 1, 49, _T("Fixedsys"));
	SetFont(&font);

	icons.Create(16, 16, ILC_COLOR, 0, 4);
	icons.Add(AfxGetApp()->LoadIcon(IDI_FOLDER_CLOSE));
	icons.Add(AfxGetApp()->LoadIcon(IDI_FOLDER_OPEN));
	icons.Add(AfxGetApp()->LoadIcon(IDI_TYPE0));
	icons.Add(AfxGetApp()->LoadIcon(IDI_TYPE1));
	icons.Add(AfxGetApp()->LoadIcon(IDI_TYPE2));
	icons.Add(AfxGetApp()->LoadIcon(IDI_TYPE3));
	SetImageList(&icons, TVSIL_NORMAL);

	Close();

	image_map[0] = 4;
	image_map[1] = 4;
	image_map[2] = 4;
	image_map[3] = 5;
	image_map[4] = 4;
	image_map[5] = 5;
	image_map[6] = 4;
	image_map[7] = 5;
	image_map[8] = 4;
	image_map[9] = 5;

	return 0;
}

void CProjectTree::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM item = pNMTreeView->itemNew.hItem;
	if (pNMTreeView->action == TVE_EXPAND)
		SetItemImage(item, 1, 1);
	else
		SetItemImage(item, 0, 0);

	*pResult = 0;
}

void CProjectTree::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM item = GetSelectedItem();
	if (GetParentItem(item) != NULL) {
		FileInfo* file_info = (FileInfo*)GetItemData(item);
		AfxGetMainWnd()->SendMessage(TREE_FILE_SEL, (WPARAM)file_info, 0);
	}

	*pResult = 0;
}

void CProjectTree::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	CPoint screen_point, client_point;
	GetCursorPos(&screen_point);
	client_point = screen_point;
	ScreenToClient(&client_point);
	UINT flag;
	HTREEITEM item = HitTest(client_point, &flag);
	if ((item != NULL) && (TVHT_ONITEM & flag)) {
		Select(item, TVGN_CARET);
		if (GetParentItem(item) != NULL) {
			CMenu pop_menu;
			pop_menu.LoadMenu(IDR_PROJECT_MENU);
			CMenu* sub_menu = pop_menu.GetSubMenu(0);
			sub_menu->TrackPopupMenu(TPM_LEFTALIGN, screen_point.x, screen_point.y, this);
		}
	}

	*pResult = 0;
}

void CProjectTree::OnDelete() 
{
	// TODO: Add your command handler code here
	HTREEITEM item = GetSelectedItem();
	FileInfo* file_info = (FileInfo*)GetItemData(item);
	AfxGetMainWnd()->SendMessage(TREE_FILE_DEL, (WPARAM)file_info, 0);
}

void CProjectTree::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	HTREEITEM item = GetSelectedItem();
	if (nChar == VK_DELETE && GetParentItem(item) != NULL)
		OnDelete();

	CTreeCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CProjectTree::AddFile(FileInfo* file_info)
{
	HTREEITEM folder_item;
	int image_id;
	CString title;
	int type = file_info->type;
	if (file_info->io_type == false) {
		folder_item = folder[0];
		if (type == 0)
			image_id = 2;
		else
			image_id = 3;
		title = Project::GetFileTitle(file_info->path);
	} else {
		folder_item = folder[1];
		image_id = image_map[type];
		title = out_file_title[type];
	}
	HTREEITEM current_item = GetChildItem(folder_item);
	HTREEITEM prev_item = TVI_FIRST;
	while (current_item) {
		FileInfo* current_file_info = (FileInfo*)GetItemData(current_item);
		int current_type = current_file_info->type;
		CString current_title = GetItemText(current_item);
		if (current_type > type || (current_type == type && current_title > title))
			break;
		prev_item = current_item;
		current_item = GetNextSiblingItem(current_item);
	}
	HTREEITEM item = InsertItem(title, image_id, image_id, folder_item, prev_item);
	file_info->item = item;
	SetItemData(item, (DWORD)file_info);
	Expand(folder_item, TVE_EXPAND);
}

void CProjectTree::RemoveFile(FileInfo* file_info)
{
	HTREEITEM item = file_info->item;
	HTREEITEM folder_item = GetParentItem(item);
	DeleteItem(item);
	if (!ItemHasChildren(folder_item))
		SetItemImage(folder_item, 0, 0);
}

void CProjectTree::Open()
{
	ModifyStyle(WS_DISABLED, 0);
	ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED);

	DeleteAllItems();
	folder[0] = InsertItem(_T("语法"));
	folder[1] = InsertItem(_T("输出"));
}

void CProjectTree::Close()
{
	ModifyStyle(0, WS_DISABLED);
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);

	DeleteAllItems();
}
