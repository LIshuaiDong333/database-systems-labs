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
#include "cmath"

void DrawArrow(CDC* pDC, POINT p, double dx, double dy, double l)
{
	if (dx == 0.0 && dy == 0.0)
		return;

	double deno;

	deno = sqrt(dx * dx + dy * dy);
	double x0 = l * dx / deno;
	double y0 = l * dy / deno;

	double dx1 = dx - 1.732 * dy;
	double dy1 = dy + 1.732 * dx;
	deno = sqrt(dx1 * dx1 + dy1 * dy1);
	double x1 = -l * dx1 / deno;
	double y1 = -l * dy1 / deno;

	double dx2 = dx + 1.732 * dy;
	double dy2 = dy - 1.732 * dx;
	deno = sqrt(dx2 * dx2 + dy2 * dy2);
	double x2 = -l * dx2 / deno;
	double y2 = -l * dy2 / deno;

	POINT triangle_points[3];
	triangle_points[0].x = p.x + int(x0);
	triangle_points[0].y = p.y + int(y0);
	triangle_points[1].x = p.x + int(x1);
	triangle_points[1].y = p.y + int(y1);
	triangle_points[2].x = p.x + int(x2);
	triangle_points[2].y = p.y + int(y2);

	COLORREF arrow_color = RGB(0, 0, 0);
	CBrush arrow_brush(arrow_color);
	CBrush* old_brush = pDC->SelectObject(&arrow_brush);
	pDC->Polygon(triangle_points, 3);
	pDC->SelectObject(old_brush);
}
