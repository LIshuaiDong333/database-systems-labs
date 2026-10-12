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
#include "lex.h"

void Lex::SetGrammar(Grammar* grammar)
{
	this->grammar = grammar;
}

void Lex::Parse(CStdioFile* file)
{
	lex_list.Clear();
	error_list.Clear();

	file->ReadString(line);
	line_pointer = line;
	while (true) {
		Parse2();
		if (!file->ReadString(line))
			return;
		line_pointer = line;
	}
}

void Lex::Parse2()
{
	CString lex_str;
	ptr = 0;
	while (true) {
		while (line_pointer[ptr] != NULL && _istspace(line_pointer[ptr]))
			ptr++;
		if (line_pointer[ptr] == NULL)
			break;
		if (IsRemark())
			return;
		while (line_pointer[ptr] != NULL && !_istspace(line_pointer[ptr]) && !IsRemark()) {
			lex_str += line_pointer[ptr];
			ptr++;
		}
		Symbol s = grammar->symbol_home.FindSymbol(lex_str);
		if (s.IsEof()) {
			Error error;
			error.type = 2;
			error.token_str = lex_str;
			error_list.Add2(error);
		} else if (s.GetType() != T) {
			Error error;
			error.type = 5;
			error.token_str = lex_str;
			error_list.Add2(error);
		} else
			lex_list.Add(s.GetId());
		lex_str.Empty();
	}
}

bool Lex::IsRemark()
{
	if (line_pointer[ptr] != NULL && line_pointer[ptr] == '/')
		if (line_pointer[ptr + 1] != NULL && line_pointer[ptr + 1] == '/')
			return true;
	return false;
}