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
#include "mymath.h"
#include "cirauto.h"

CirAuto::CirAuto()
{
	level_height = NULL;
	max_height = 0;
}

CirAuto::~CirAuto()
{
	if (level_height != NULL)
		delete[] level_height;
}

void CirAuto::SetCircleMargin(int circle_margin)
{
	this->circle_margin = circle_margin;
}

void CirAuto::SetDefault()
{
	Automaton::SetDefault();
	SetCircleMargin(6);
}

void CirAuto::ComputeHeight()
{
	if (level_height != NULL)
		delete[] level_height;
	level_height = new int[layout.GetCount()];

	max_height = 0;

	layout.Rewind();
	int level_count = layout.GetCount();
	for (int i = 0; i < level_count; i++) {
		int current_height = 0;

		Level& level = layout.GetNext();
		level.Rewind();
		int vertex_count = level.GetCount();
		for (int j = 0; j < vertex_count; j++) {
			int v_id = level.GetNext();
			current_height += 2 * vertex[v_id].r;
		}

		current_height += this->vert_space * (vertex_count - 1);

		level_height[i] = current_height;

		if (current_height > max_height)
			max_height = current_height;
	}
}

int CirAuto::ComputeTop(int level_id)
{
	int current_height = level_height[level_id];
	switch (align) {
	case Top:
		return vert_margin;
	case Center:
		return vert_margin + (max_height - current_height) / 2;
	default:
		return vert_margin + max_height - current_height;
	}
}

void CirAuto::InitVertex(CDC* pDC)
{
	for (int i = 0; i < vertex_count; i++) {
		BareVertexSize(pDC, vertex[i].title, vertex[i].size);
		vertex[i].size.cx += circle_margin;
		vertex[i].size.cy += circle_margin;
		int a = vertex[i].size.cx / 2;
		int b = vertex[i].size.cy / 2;
		vertex[i].r = sqrt(a * a + b * b);
	}

	ComputeHeight();

	int left = horz_margin;

	layout.Rewind();
	int level_count = layout.GetCount();
	for (i = 0; i < level_count; i++) {
		int top = ComputeTop(i);

		int level_width = 0;

		Level& level = layout.GetNext();
		level.Rewind();
		int vertex_count = level.GetCount();
		for (int j = 0; j < vertex_count; j++) {
			int v_id = level.GetNext();
			vertex[v_id].pos.x = left + vertex[v_id].r;
			vertex[v_id].pos.y = top + vertex[v_id].r;

			if (vertex[v_id].r * 2 > level_width)
				level_width = vertex[v_id].r * 2;

			top += vertex[v_id].r * 2 + vert_space;
		}

		left += level_width + horz_space;
	}
}

void CirAuto::InitEdge()
{
	for (int i = 0; i < edge_count; i++) {
		if (loc[edge[i].v1_id].level != loc[edge[i].v2_id].level) {
			Mid(vertex[edge[i].v1_id].pos, vertex[edge[i].v2_id].pos, edge[i].m);
		} else {
			if (loc[edge[i].v1_id].ord != loc[edge[i].v2_id].ord) {
				POINT center;
				Mid(vertex[edge[i].v1_id].pos, vertex[edge[i].v2_id].pos, center);
				int cx = vertex[edge[i].v2_id].pos.x - vertex[edge[i].v1_id].pos.x;
				int cy = vertex[edge[i].v2_id].pos.y - vertex[edge[i].v1_id].pos.y;
				int l = sqrt(cx * cx + cy * cy);
				if (loc[edge[i].v1_id].ord < loc[edge[i].v2_id].ord) {
					edge[i].m.x = center.x + l / 5;
					edge[i].m.y = center.y;
				} else {
					edge[i].m.x = center.x - l / 5;
					edge[i].m.y = center.y;
				}
			} else {
				edge[i].m.x = vertex[edge[i].v1_id].pos.x;
				edge[i].m.y = vertex[edge[i].v1_id].pos.y - vertex[edge[i].v1_id].r - d_init;
			}
		}
		edge[i].old_v1 = vertex[edge[i].v1_id].pos;
		edge[i].old_v2 = vertex[edge[i].v2_id].pos;
		edge[i].old_m = edge[i].m;
	}
}

void CirAuto::DrawVertex(CDC* pDC, int v_id)
{
	COLORREF old_text_color = pDC->GetTextColor();
	COLORREF old_bk_color = pDC->GetBkColor();
	COLORREF title_text_color = RGB(255, 255, 255);
	COLORREF title_bk_color = RGB(49, 106, 197);
	pDC->SetTextColor(title_text_color);
	pDC->SetBkColor(title_bk_color);

	CBrush title_brush(title_bk_color);
	CBrush* old_brush = pDC->SelectObject(&title_brush);

	int x1 = vertex[v_id].pos.x - vertex[v_id].r;
	int y1 = vertex[v_id].pos.y - vertex[v_id].r;
	int x2 = vertex[v_id].pos.x + vertex[v_id].r;
	int y2 = vertex[v_id].pos.y + vertex[v_id].r;
	pDC->Ellipse(x1, y1, x2, y2);
	DrawBareVertex(pDC, vertex[v_id].pos.x - vertex[v_id].size.cx / 2 + circle_margin / 2,
		vertex[v_id].pos.y - vertex[v_id].size.cy / 2 + circle_margin / 2,
		vertex[v_id].title);

	pDC->SelectObject(old_brush);

	pDC->SetTextColor(old_text_color);
	pDC->SetBkColor(old_bk_color);
}

void CirAuto::DrawEdge(CDC* pDC, int edge_id)
{
	if (edge[edge_id].v1_id != edge[edge_id].v2_id) {
		Bezier(pDC, edge[edge_id].title,
			vertex[edge[edge_id].v1_id].pos, edge[edge_id].m,
			vertex[edge[edge_id].v2_id].pos);
	} else {
		POINT center;
		Mid(edge[edge_id].m, vertex[edge[edge_id].v1_id].pos, center);
		Circle(pDC, edge[edge_id].title, center, edge[edge_id].m);
	}

}

void CirAuto::DrawVertexFrame(CDC* pDC, int v_id, int offset_x, int offset_y)
{
	int left = vertex[v_id].pos.x + offset_x - vertex[v_id].r;
	int top = vertex[v_id].pos.y + offset_y - vertex[v_id].r;
	int right = vertex[v_id].pos.x + offset_x + vertex[v_id].r;
	int bottom = vertex[v_id].pos.y + offset_y + vertex[v_id].r;
	pDC->Arc(left, top, right, bottom, 0, 0, 0, 0);
}

void CirAuto::MoveVertex(int v_id, int offset_x, int offset_y)
{
	vertex[v_id].pos.x += offset_x;
	vertex[v_id].pos.y += offset_y;
	for (int i = 0; i < edge_count; i++) {
		if (edge[i].v1_id == v_id && edge[i].v2_id == v_id) {
			edge[i].m.x += offset_x;
			edge[i].m.y += offset_y;
		} else if (edge[i].v1_id == v_id) {
			POINT p0 = edge[i].old_v1 - edge[i].old_v2;
			POINT p1 = edge[i].old_m - edge[i].old_v2;
			POINT p2 = vertex[edge[i].v1_id].pos - vertex[edge[i].v2_id].pos;
			POINT p3;
			Rotate(p0, p1, p2, p3);
			edge[i].m.x = p3.x + vertex[edge[i].v2_id].pos.x;
			edge[i].m.y = p3.y + vertex[edge[i].v2_id].pos.y;
		} else if (edge[i].v2_id == v_id) {
			POINT p0 = edge[i].old_v2 - edge[i].old_v1;
			POINT p1 = edge[i].old_m - edge[i].old_v1;
			POINT p2 = vertex[edge[i].v2_id].pos - vertex[edge[i].v1_id].pos;
			POINT p3;
			Rotate(p0, p1, p2, p3);
			edge[i].m.x = p3.x + vertex[edge[i].v1_id].pos.x;
			edge[i].m.y = p3.y + vertex[edge[i].v1_id].pos.y;
		}
	}
}

void CirAuto::MoveEdge(int edge_id, int offset_x, int offset_y)
{
	edge[edge_id].m.x += offset_x;
	edge[edge_id].m.y += offset_y;

	edge[edge_id].old_m = edge[edge_id].m;
	edge[edge_id].old_v1 = vertex[edge[edge_id].v1_id].pos;
	edge[edge_id].old_v2 = vertex[edge[edge_id].v2_id].pos;
}

bool CirAuto::HitTest(POINT point, SelectedType& selected_type, int& selected_id)
{
	for (int i = 0; i < edge_count; i++) {
		int a = 8;
		CRect rect;
		rect.left = edge[i].m.x - a;
		rect.top = edge[i].m.y - a;
		rect.right = edge[i].m.x + a;
		rect.bottom = edge[i].m.y + a;
		if (rect.PtInRect(point)) {
			selected_id = i;
			selected_type = EDGE;
			return true;
		}
	}
	for (i = 0; i < vertex_count; i++) {
		POINT p = vertex[i].pos - point;
		if (p.x * p.x + p.y * p.y <= vertex[i].r * vertex[i].r) {
			selected_id = i;
			selected_type = VERTEX;
			return true;
		}
	}
	return false;
}

CRect CirAuto::GetTotalSize()
{
	CRect rect;
	for (int i = 0; i < vertex_count; i++) {
		int left = vertex[i].pos.x - vertex[i].r;
		if (i == 0 || left < rect.left)
			rect.left = left;
		int top = vertex[i].pos.y - vertex[i].r;
		if (i == 0 || top < rect.top)
			rect.top = top;
		int right = vertex[i].pos.x + vertex[i].r;
		if (i == 0 || right > rect.right)
			rect.right = right;
		int bottom = vertex[i].pos.y + vertex[i].r;
		if (i == 0 || bottom > rect.bottom)
			rect.bottom = bottom;
	}
	for (i = 0; i < edge_count; i++) {
		int left = edge[i].m.x;
		if (left < rect.left)
			rect.left = left;
		int top = edge[i].m.y;
		if (top < rect.top)
			rect.top = top;
		int right = edge[i].m.x;
		if (right > rect.right)
			rect.right = right;
		int bottom = edge[i].m.y;
		if (bottom > rect.bottom)
			rect.bottom = bottom;
	}
	return rect;
}