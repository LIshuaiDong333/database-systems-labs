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
#include "rectauto.h"

RectAuto::RectAuto()
{
	level_height = NULL;
	max_height = 0;
}

RectAuto::~RectAuto()
{
	if (level_height != NULL)
		delete[] level_height;
}

void RectAuto::ComputeHeight()
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
			current_height += vertex[v_id].size.cy;
		}

		current_height += this->vert_space * (vertex_count - 1);

		level_height[i] = current_height;

		if (current_height > max_height)
			max_height = current_height;
	}
}

int RectAuto::ComputeTop(int level_id)
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

void RectAuto::InitVertex(CDC* pDC)
{
	for (int i = 0; i < vertex_count; i++) {
		SIZE size;
		RECT rect;
		VertexSize(pDC, vertex[i].title, vertex[i].body, size, rect);
		vertex[i].size = size;
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
			vertex[v_id].pos.x = left + vertex[v_id].size.cx / 2;
			vertex[v_id].pos.y = top + vertex[v_id].size.cy / 2;

			if (vertex[v_id].size.cx > level_width)
				level_width = vertex[v_id].size.cx;

			top += vertex[v_id].size.cy + vert_space;
		}

		left += level_width + horz_space;
	}
}

void RectAuto::InitEdge()
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
				edge[i].m.y = vertex[edge[i].v1_id].pos.y - vertex[edge[i].v1_id].size.cy / 2 - d_init;
			}
		}
		edge[i].old_v1 = vertex[edge[i].v1_id].pos;
		edge[i].old_v2 = vertex[edge[i].v2_id].pos;
		edge[i].old_m = edge[i].m;
	}
}

void RectAuto::DrawVertex(CDC* pDC, int v_id)
{
	int x = vertex[v_id].pos.x - vertex[v_id].size.cx / 2;
	int y = vertex[v_id].pos.y - vertex[v_id].size.cy / 2;
	::DrawVertex(pDC, x, y, vertex[v_id].title, vertex[v_id].body);
}

void RectAuto::DrawEdge(CDC* pDC, int edge_id)
{
	if (edge[edge_id].v1_id != edge[edge_id].v2_id) {
		Bezier(pDC, edge[edge_id].title,
			vertex[edge[edge_id].v1_id].pos, edge[edge_id].m,
			vertex[edge[edge_id].v2_id].pos);
	} else {
		RECT rect;
		MkRect(vertex[edge[edge_id].v1_id].pos,
			vertex[edge[edge_id].v1_id].size.cx,
			vertex[edge[edge_id].v1_id].size.cy, rect);
		Circle(pDC, edge[edge_id].title, edge[edge_id].m, rect);
	}
}

void RectAuto::DrawVertexFrame(CDC* pDC, int v_id, int offset_x, int offset_y)
{
	int left = vertex[v_id].pos.x + offset_x - vertex[v_id].size.cx / 2;
	int top = vertex[v_id].pos.y + offset_y - vertex[v_id].size.cy / 2;
	int right = vertex[v_id].pos.x + offset_x + vertex[v_id].size.cx / 2;
	int bottom = vertex[v_id].pos.y + offset_y + vertex[v_id].size.cy / 2;
	pDC->MoveTo(left, top);
	pDC->LineTo(right, top);
	pDC->LineTo(right, bottom);
	pDC->LineTo(left, bottom);
	pDC->LineTo(left, top);
}

void RectAuto::MoveVertex(int v_id, int offset_x, int offset_y)
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

void RectAuto::MoveEdge(int edge_id, int offset_x, int offset_y)
{
	edge[edge_id].m.x += offset_x;
	edge[edge_id].m.y += offset_y;

	edge[edge_id].old_m = edge[edge_id].m;
	edge[edge_id].old_v1 = vertex[edge[edge_id].v1_id].pos;
	edge[edge_id].old_v2 = vertex[edge[edge_id].v2_id].pos;
}

bool RectAuto::HitTest(POINT point, SelectedType& selected_type, int& selected_id)
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
		CRect rect;
		MkRect(vertex[i].pos, vertex[i].size.cx, vertex[i].size.cy, rect);
		if (rect.PtInRect(point)) {
			selected_id = i;
			selected_type = VERTEX;
			return true;
		}
	}
	return false;
}

CRect RectAuto::GetTotalSize()
{
	CRect rect;
	for (int i = 0; i < vertex_count; i++) {
		int left = vertex[i].pos.x - vertex[i].size.cx / 2;
		if (i == 0 || left < rect.left)
			rect.left = left;
		int top = vertex[i].pos.y - vertex[i].size.cy / 2;
		if (i == 0 || top < rect.top)
			rect.top = top;
		int right = vertex[i].pos.x + vertex[i].size.cx / 2;
		if (i == 0 || right > rect.right)
			rect.right = right;
		int bottom = vertex[i].pos.y + vertex[i].size.cy / 2;
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