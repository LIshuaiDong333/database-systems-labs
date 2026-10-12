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
#include "automaton.h"
#include "set.h"

Automaton::Automaton()
{
	start = 0;
	vertex = NULL;
	vertex_count = 0;
	edge = NULL;
	edge_count = 0;
	loc = NULL;
	SetSpace(0, 0, 0, 0);
	align = Top;
	d_init = 0;
}

Automaton::~Automaton()
{
	if (loc != NULL)
		delete[] loc;
}

void Automaton::SetTopo(int start, Vertex* vertex, int vertex_count, Edge* edge, int edge_count)
{
	this->start = start;
	this->vertex = vertex;
	this->vertex_count = vertex_count;
	this->edge = edge;
	this->edge_count = edge_count;

	if (loc != NULL)
		delete[] loc;
	this->loc = new Location[vertex_count];

	ComputeLayout();
}

void Automaton::SetSpace(int horz_margin, int vert_margin, int horz_space, int vert_space)
{
	this->horz_margin = horz_margin;
	this->vert_margin = vert_margin;
	this->horz_space = horz_space;
	this->vert_space = vert_space;
}

void Automaton::SetAlign(Align align)
{
	this->align = align;
}

void Automaton::SetCircle(int d_init)
{
	this->d_init = d_init;
}

void Automaton::SetDefault()
{
	SetSpace(90, 150, 100, 100);
	SetCircle(60);
	SetAlign(Top);
}

void Automaton::ComputeLayout()
{
	layout.Clear();

	Set<int> vertex_set;

	Level* new_level = &layout.Add(Level()); 

	new_level->Add(start);
	vertex_set.Add(start);
	loc[start].level = 0;
	loc[start].ord = 0;

	int level_count = 1;
	do {
		Level* last_level = new_level;
		new_level = &layout.Add(Level());
		level_count++;

		int ord = 0;
		for (int i = 0; i < edge_count; i++) {
			if (last_level->FindFirst(edge[i].v1_id) != -1 && !vertex_set.Has(edge[i].v2_id)) {
				new_level->Add(edge[i].v2_id);
				vertex_set.Add(edge[i].v2_id);
				loc[edge[i].v2_id].level = level_count - 1;
				loc[edge[i].v2_id].ord = ord;
				ord++;
			}
		}
	} while (new_level->GetCount() != 0);
	layout.RemoveTail();
}

void Automaton::DrawAll(CDC* pDC)
{
	for (int i = 0; i < edge_count; i++)
		DrawEdge(pDC, i);

	for (i = 0; i < vertex_count; i++)
		DrawVertex(pDC, i);
}

void Automaton::DrawEdgeFrame(CDC* pDC, int edge_id, int offset_x, int offset_y)
{
	int a = 8;
	int left = edge[edge_id].m.x + offset_x - a;
	int top = edge[edge_id].m.y + offset_y - a;
	int right = edge[edge_id].m.x + offset_x + a;
	int bottom = edge[edge_id].m.y + offset_y + a;
	pDC->MoveTo(left, top);
	pDC->LineTo(right, top);
	pDC->LineTo(right, bottom);
	pDC->LineTo(left, bottom);
	pDC->LineTo(left, top);
}

void Automaton::MoveAll(int offset_x, int offset_y)
{
	for (int i = 0; i < vertex_count; i++) {
		vertex[i].pos.x += offset_x;
		vertex[i].pos.y += offset_y;
	}
	for (i = 0; i < edge_count; i++) {
		edge[i].m.x += offset_x;
		edge[i].m.y += offset_y;
	}
}