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

#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "graph.h"
#include "list.h"

struct Vertex
{
	CString title;
	CString body;

	POINT pos;
	SIZE size;	//对于RectAuto
	int r;	//对于CirAuto
};

struct Edge
{
	CString title;

	int v1_id;
	int v2_id;
	POINT m;

	POINT old_v1;
	POINT old_v2;
	POINT old_m;
};

struct Location
{
	int level;
	int ord;
};

enum Align {Top, Center, Bottom};

enum SelectedType {VERTEX, EDGE};

class Automaton
{
protected:
	int start;
	Vertex* vertex;
	int vertex_count;
	Edge* edge;
	int edge_count;

	typedef List<int> Level;
	typedef List<Level> Layout;
	Layout layout;
	Location* loc;
	void ComputeLayout();

	int horz_margin, vert_margin;
	int horz_space, vert_space;

	Align align;

	int d_init;
public:
	Automaton();
	virtual ~Automaton();

	void SetTopo(int start, Vertex* vertex, int vertex_count, Edge* edge, int edge_count);
	void SetSpace(int horz_margin, int vert_margin, int horz_space, int vert_space);
	void SetAlign(Align align);
	void SetCircle(int d_init);

	virtual void SetDefault();

	virtual void InitVertex(CDC* pDC) = 0;
	virtual void InitEdge() = 0;

	virtual void DrawVertex(CDC* pDC, int v_id) = 0;
	virtual void DrawEdge(CDC* pDC, int edge_id) = 0;
	void DrawAll(CDC* pDC);

	virtual void DrawVertexFrame(CDC* pDC, int v_id, int offset_x, int offset_y) = 0;
	void DrawEdgeFrame(CDC* pDC, int edge_id, int offset_x, int offset_y);

	virtual void MoveVertex(int v_id, int offset_x, int offset_y) = 0;
	virtual void MoveEdge(int edge_id, int offset_x, int offset_y) = 0;
	void MoveAll(int offset_x, int offset_y);

	virtual bool HitTest(POINT point, SelectedType& selected_type, int& selected_id) = 0;

	virtual CRect GetTotalSize() = 0;
};

#endif