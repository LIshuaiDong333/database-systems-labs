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

void VertexSize(CDC* pDC, const CString& title_str, SIZE& total_size)
{
	int margin = 3;

	CFont title_font;
	title_font.CreateFont(-16, 0, 0, 0, 400, 1, 0, 0, 0, 3, 2, 1, 49, _T("Courier New"));

	SIZE title_size;
	CFont* old_font = pDC->SelectObject(&title_font);
	TextSize(pDC, title_str, title_size);

	total_size.cx = title_size.cx + margin * 4;
	total_size.cy = title_size.cy + margin * 4;

	pDC->SelectObject(old_font);
}

void BareVertexSize(CDC* pDC, const CString& title_str, SIZE& total_size)
{
	CFont title_font;
	title_font.CreateFont(-16, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 34, _T("Arial"));

	CFont* old_font = pDC->SelectObject(&title_font);
	TextSize(pDC, title_str, total_size);

	pDC->SelectObject(old_font);
}

void FrameVertexSize(CDC* pDC, const CString& title_str, SIZE& total_size)
{
	int margin = 5;

	CFont title_font;
	title_font.CreateFont(-19, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 34, _T("Arial"));

	CFont* old_font = pDC->SelectObject(&title_font);
	TextSize(pDC, title_str, total_size);

	total_size.cx += margin * 2;
	total_size.cy += margin * 2;

	pDC->SelectObject(old_font);
}

void DrawVertex(CDC* pDC, int x, int y, const CString& title_str, int status)
{
	int margin = 3;

	SIZE total_size;
	VertexSize(pDC, title_str, total_size);

	COLORREF old_text_color = pDC->GetTextColor();
	COLORREF old_bk_color = pDC->GetBkColor();
	COLORREF frame_color;
	if (status == 0)
		frame_color = RGB(49, 106, 197);
	else
		frame_color = RGB(228, 91, 49);
	COLORREF title_text_color = RGB(0, 0, 0);
	COLORREF title_bk_color = RGB(255, 255, 255);

	CBrush frame_brush(frame_color);

	CBrush* old_brush = pDC->SelectObject(&frame_brush);
	pDC->Rectangle(x, y, x + total_size.cx, y + total_size.cy);

	CBrush title_brush(title_bk_color);

	CFont title_font;
	title_font.CreateFont(-16, 0, 0, 0, 400, 1, 0, 0, 0, 3, 2, 1, 49, _T("Courier New"));

	pDC->SelectObject(&title_brush);
	pDC->Rectangle(x + margin, y + margin, x + total_size.cx - margin, y + total_size.cy - margin);

	CFont* old_font = pDC->SelectObject(&title_font);
	pDC->SetTextColor(title_text_color);
	pDC->SetBkColor(title_bk_color);
	DrawText(pDC, x + 2 * margin, y + 2 * margin, title_str);

	pDC->SetTextColor(old_text_color);
	pDC->SetBkColor(old_bk_color);
	pDC->SelectObject(old_brush);
	pDC->SelectObject(old_font);
}

void DrawBareVertex(CDC* pDC, int x, int y, const CString& title_str)
{
	CFont title_font;
	title_font.CreateFont(-16, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 34, _T("Arial"));

	CFont* old_font = pDC->SelectObject(&title_font);
	DrawText(pDC, x, y, title_str);
	pDC->SelectObject(old_font);
}

void DrawFrameVertex(CDC* pDC, int x, int y, const CString& title_str, int type)
{
	int margin = 5;

	SIZE total_size;
	FrameVertexSize(pDC, title_str, total_size);

	COLORREF old_text_color = pDC->GetTextColor();
	COLORREF text_color;
	if (type == 0)
		text_color = RGB(0, 0, 0);
	else
		text_color = RGB(0, 0, 255);
	pDC->SetTextColor(text_color);
	CPen pen(PS_SOLID, 1, text_color);
	CPen* old_pen = pDC->SelectObject(&pen);

	pDC->Rectangle(x, y, x + total_size.cx, y + total_size.cy);

	CFont title_font;
	title_font.CreateFont(-19, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 34, _T("Arial"));
	CFont* old_font = pDC->SelectObject(&title_font);
	
	DrawText(pDC, x + margin, y + margin, title_str);
	
	pDC->SelectObject(old_font);
	pDC->SelectObject(old_pen);
	pDC->SetTextColor(old_text_color);
}
