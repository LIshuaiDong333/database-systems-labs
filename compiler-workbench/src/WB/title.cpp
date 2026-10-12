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
#include "vertex.h"

void DrawTitle(CDC* pDC, const CString& title_string,
			   POINT p, double dx, double dy, double margin)
{
	if (dx == 0.0 && dy == 0.0)
		return;
	SIZE title_size;
	BareVertexSize(pDC, title_string, title_size);
	double a = double(title_size.cx) / 2.0 + margin;
	double b = double(title_size.cy) / 2.0 + margin;
	double deno = dx * dx + dy * dy;
	double cx;
	double cy;
	if ((dx > 0.0 && dy > 0.0) || (dx < 0.0 && dy < 0.0)) {
		cx = (a * dy * dy + b * dx * dy) / deno;
		cy = -(b * dx * dx + a * dx * dy) / deno;
	} else {
		cx = -(a * dy * dy - b * dx * dy) / deno;
		cy = -(b * dx * dx - a * dx * dy) / deno;
	}
	int x = p.x + int(cx) - title_size.cx / 2;
	int y = p.y + int(cy) - title_size.cy / 2;
	DrawBareVertex(pDC, x, y, title_string);
}
