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
#include "html.h"

void Html::SetFile(CStdioFile* file)
{
	this->file = file;
}

void Html::Nb()
{
	file->WriteString(_T("&nbsp;"));
}

void Html::Tab()
{
	file->WriteString(_T("&nbsp;&nbsp;&nbsp;&nbsp;"));
}

void Html::Br()
{
	file->WriteString(_T("<br>\n"));
}

void Html::Table()
{
	file->WriteString(_T("<table border=\"1\">\n"));
}

void Html::Table2()
{
	file->WriteString(_T("</table>\n"));
}

void Html::Tr()
{
	file->WriteString(_T("<tr>\n"));
}

void Html::Tr2()
{
	file->WriteString(_T("</tr>\n"));
}

void Html::Td()
{
	file->WriteString(_T("<td nowrap>"));
}

void Html::Td2()
{
	file->WriteString(_T("</td>\n"));
}

void Html::HighLight()
{
	file->WriteString(_T("<font color=\"#FF0000\">"));
}

void Html::HighLight2()
{
	file->WriteString(_T("</font>"));
}

void Html::Head(LPCTSTR title)
{
	file->WriteString(_T("<html>\n"));
	file->WriteString(_T("<head>\n"));
	file->WriteString(_T("<title>"));
	file->WriteString(title);
	file->WriteString(_T("</title>\n"));
	file->WriteString(_T("</head>\n"));
	file->WriteString(_T("<body>\n"));
	file->WriteString(_T("<p align=\"center\">"));
	file->WriteString(title);
	file->WriteString(_T("</p>\n"));
}

void Html::Tail()
{
	file->WriteString(_T("</body>\n</html>"));
}
