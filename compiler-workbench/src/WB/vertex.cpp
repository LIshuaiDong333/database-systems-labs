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

#include "stdafx.h"

void TextSize(CDC* pDC, LPCTSTR str, SIZE& size)
{
	CRect rect;
	pDC->DrawText(str, rect, DT_NOCLIP | DT_CALCRECT);
	size = rect.Size();
}

void DrawText(CDC* pDC, int x, int y, LPCTSTR str)
{
	pDC->DrawText(str, CRect(x, y, x, y), DT_NOCLIP);
}

void VertexSize(CDC* pDC, const CString& title_str, const CString& body_str, SIZE& total_size, RECT& body_rect)
{
	int margin = 3;

	CFont title_font;
	CFont body_font;
	title_font.CreateFont(-16, 0, 0, 0, 400, 1, 0, 0, 0, 3, 2, 1, 49, _T("Courier New"));
	body_font.CreateFont(-13, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 49, _T("Courier New"));

	SIZE title_size;
	SIZE body_size;
	CFont* old_font = pDC->SelectObject(&title_font);
	TextSize(pDC, title_str, title_size);
	pDC->SelectObject(&body_font);
	TextSize(pDC, body_str, body_size);

	total_size.cx = max(title_size.cx + margin * 2, body_size.cx + margin * 4);
	total_size.cy = title_size.cy + body_size.cy + margin * 4;

	body_rect.left = margin;
	body_rect.top = title_size.cy + margin;
	body_rect.right = body_size.cx + 3 * margin;
	body_rect.bottom = title_size.cy + body_size.cy + 3 * margin;

	pDC->SelectObject(old_font);
}

void BareVertexSize(CDC* pDC, const CString& title_str, SIZE& size)
{
	CFont title_font;
	title_font.CreateFont(-16, 0, 0, 0, 400, 1, 0, 0, 0, 3, 2, 1, 49, _T("Courier New"));

	CFont* old_font = pDC->SelectObject(&title_font);
	TextSize(pDC, title_str, size);
	pDC->SelectObject(old_font);
}

void DrawVertex(CDC* pDC, int x, int y, const CString& title_str, const CString& body_str)
{
	int margin = 3;

	SIZE total_size;
	RECT body_rect;
	VertexSize(pDC, title_str, body_str, total_size, body_rect);

	COLORREF old_text_color = pDC->GetTextColor();
	COLORREF old_bk_color = pDC->GetBkColor();
	COLORREF title_text_color = RGB(255, 255, 255);
	COLORREF title_bk_color = RGB(49, 106, 197);
	COLORREF body_text_color = RGB(0, 0, 0);
	COLORREF body_bk_color = RGB(255, 255, 255);

	CBrush title_brush(title_bk_color);
	CBrush body_brush(body_bk_color);

	CFont title_font;
	CFont body_font;
	title_font.CreateFont(-16, 0, 0, 0, 400, 1, 0, 0, 0, 3, 2, 1, 49, _T("Courier New"));
	body_font.CreateFont(-13, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 49, _T("Courier New"));

	CBrush* old_brush = pDC->SelectObject(&title_brush);
	pDC->Rectangle(x, y, x + total_size.cx, y + total_size.cy);

	CFont* old_font = pDC->SelectObject(&title_font);
	pDC->SetTextColor(title_text_color);
	pDC->SetBkColor(title_bk_color);
	DrawText(pDC, x + margin, y + margin, title_str);

	pDC->SelectObject(&body_brush);
	pDC->Rectangle(x + body_rect.left, y + body_rect.top, x + body_rect.right, y + body_rect.bottom);
	
	pDC->SelectObject(&body_font);
	pDC->SetTextColor(body_text_color);
	pDC->SetBkColor(body_bk_color);
	DrawText(pDC, x + body_rect.left + margin, y + body_rect.top + margin, body_str);

	pDC->SetTextColor(old_text_color);
	pDC->SetBkColor(old_bk_color);
	pDC->SelectObject(old_brush);
	pDC->SelectObject(old_font);
}

void DrawBareVertex(CDC* pDC, int x, int y, const CString& title_str)
{
	SIZE size;
	BareVertexSize(pDC, title_str, size);

	CFont title_font;
	title_font.CreateFont(-16, 0, 0, 0, 400, 1, 0, 0, 0, 3, 2, 1, 49, _T("Courier New"));

	CFont* old_font = pDC->SelectObject(&title_font);
	DrawText(pDC, x, y, title_str);

	pDC->SelectObject(old_font);
}
