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

#ifndef CIRAUTO_H
#define CIRAUTO_H

#include "automaton.h"

class CirAuto : public Automaton
{
	int circle_margin;

	int* level_height;
	int max_height;
	void ComputeHeight();
	int ComputeTop(int level_id);
public:
	CirAuto();
	~CirAuto();

	void SetCircleMargin(int circle_margin);
	void SetDefault();

	void InitVertex(CDC* pDC);
	void InitEdge();

	void DrawVertex(CDC* pDC, int v_id);
	void DrawEdge(CDC* pDC, int edge_id);

	void DrawVertexFrame(CDC* pDC, int v_id, int offset_x, int offset_y);

	void MoveVertex(int v_id, int offset_x, int offset_y);
	void MoveEdge(int edge_id, int offset_x, int offset_y);

	bool HitTest(POINT point, SelectedType& selected_type, int& selected_id);
	CRect GetTotalSize();
};

#endif
