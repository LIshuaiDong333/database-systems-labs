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
#include "vertex.h"
#include "arrow.h"
#include "circle.h"

bool Intersect(RECT rect, POINT p, POINT& start, POINT& end, double ratio)
{
	//左上
	if (p.x < rect.left && p.y < rect.top) {
		start.x = min(rect.left + int(ratio * (rect.top - p.y)), rect.right);
		start.y = rect.top;
		end.x = rect.left;
		end.y = min(rect.top + int(ratio * (rect.left - p.x)), rect.bottom);
		return true;
	}
	//左
	if (p.x <= rect.left && p.y >= rect.top && p.y <= rect.bottom) {
		start.x = rect.left;
		start.y = max(p.y - int(ratio * (rect.left - p.x)), rect.top);
		end.x = rect.left;
		end.y = min(p.y + int(ratio * (rect.left - p.x)), rect.bottom);
		return true;
	}
	//左下
	if (p.x < rect.left && p.y > rect.bottom) {
		start.x = rect.left;
		start.y = max(rect.bottom - int(ratio * (rect.left - p.x)), rect.top);
		end.x = min(rect.left + int(ratio * (p.y - rect.bottom)), rect.right);
		end.y = rect.bottom;
		return true;
	}
	//上
	if (p.x >= rect.left && p.x <= rect.right && p.y <= rect.top) {
		start.x = min(p.x + int(ratio * (rect.top - p.y)), rect.right);
		start.y = rect.top;
		end.x = max(p.x - int(ratio * (rect.top - p.y)), rect.left);
		end.y = rect.top;
		return true;
	}
	//下
	if (p.x >= rect.left && p.x <= rect.right && p.y >= rect.bottom) {
		start.x = max(p.x - int(ratio * (p.y - rect.bottom)), rect.left);
		start.y = rect.bottom;
		end.x = min(p.x + int(ratio * (p.y - rect.bottom)), rect.right);
		end.y = rect.bottom;
		return true;
	}
	//右上
	if (p.x > rect.right && p.y < rect.top) {
		start.x = rect.right;
		start.y = min(rect.top + int(ratio * (p.x - rect.right)), rect.bottom);
		end.x = max(rect.right - int(ratio * (rect.top - p.y)), rect.left);
		end.y = rect.top;
		return true;
	}
	//右
	if (p.x >= rect.right && p.y >= rect.top && p.y <= rect.bottom) {
		start.x = rect.right;
		start.y = min(p.y + int(ratio * (p.x - rect.right)), rect.bottom);
		end.x = rect.right;
		end.y = max(p.y - int(ratio * (p.x - rect.right)), rect.top);
		return true;
	}
	//右下
	if (p.x > rect.right && p.y > rect.bottom) {
		start.x = max(rect.right - int(ratio * (p.y - rect.bottom)), rect.left);
		start.y = rect.bottom;
		end.x = rect.right;
		end.y = max(rect.bottom - int(ratio * (p.x - rect.right)), rect.top);
		return true;
	}

	return false;
}

//已知圆上三点p0，p1，p2，求圆心p
bool CircleCenter(POINT p0, POINT p1, POINT p2, POINT& p)
{
	int a0 = 2 * (p1.x - p0.x);
	int b0 = 2 * (p1.y - p0.y);
	int c0 = p1.x * p1.x - p0.x * p0.x + p1.y * p1.y - p0.y * p0.y;
	int a1 = 2 * (p2.x - p0.x);
	int b1 = 2 * (p2.y - p0.y);
	int c1 = p2.x * p2.x - p0.x * p0.x + p2.y * p2.y - p0.y * p0.y;

	int deno0 = a0 * b1 - a1 * b0;
	if (deno0 == 0)
		return false;
	p.x = (c0 * b1 - c1 * b0) / deno0;

	int deno1 = b0 * a1 - b1 * a0;
	if (deno1 == 0)
		return false;
	p.y = (c0 * a1 - c1 * a0) / deno1;

	return true;
}

void Circle(CDC* pDC, const CString& title_string, POINT p, RECT rect, double ratio)
{
	//求起点和终点
	POINT start, end;
	if (!Intersect(rect, p, start, end, ratio))
		return;

	//求圆心
	POINT circle_center;
	if (!CircleCenter(p, start, end, circle_center))
		return;

	Circle(pDC, title_string, circle_center, p);
}

double Dist(double a, double b, double r, double x, double y)
{
	if (x == 0.0 && y == 0.0)
		return 0.0;
	x = fabs(x);
	y = fabs(y);
	double a2 = a + r;
	double b2 = b + r;
	if (a * y > b2 * x) {
		double x2 = x * b2 / y;
		return sqrt(x2 * x2 + b2 * b2);
	} else if (a2 * y < b * x) {
		double y2 = y * a2 / x;
		return sqrt(a2 * a2 + y2 * y2);
	} else {
		double tana = (y * a - x * b) / (x * a + y * b);
		double cosa = 1.0 / sqrt(1.0 + tana * tana);
		double c = sqrt(a * a + b * b);
		double A = 1.0;
		double B = -2.0 * cosa * c;
		double C = c * c - r * r;
		double delta = B * B - 4 * A * C;
		if (delta < 0)
			return 0.0;
		return (-B + sqrt(delta)) / 2.0 * A;
	}
}

void Circle(CDC *pDC, const CString& title_string, POINT center, POINT p, double margin)
{
	double x = p.x - center.x;
	double y = p.y - center.y;
	if (x == 0.0 && y == 0.0)
		return;
	double r = sqrt(x * x + y * y);
	SIZE title_size;
	BareVertexSize(pDC, title_string, title_size);
	double a = double(title_size.cx) / 2.0;
	double b = double(title_size.cy) / 2.0;
	double l = Dist(a + margin, b + margin, r, x, y);
	double deno = sqrt(x * x + y * y);
	double x2 = l * x / deno;
	double y2 = l * y / deno;
	int ir = int(r);
	int ia = int(a);
	int ib = int(b);
	int ix2 = int(x2);
	int iy2 = int(y2);
	pDC->Arc(center.x - ir, center.y - ir, center.x + ir, center.y + ir, 0, 0, 0, 0);
	double dx = center.y - p.y;
	double dy = p.x - center.x;
	DrawArrow(pDC, p, dx, dy);
	DrawBareVertex(pDC, center.x + ix2 - ia, center.y + iy2 - ib, title_string);
}