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
#include "token.h"

void TokenReader::SetFile(CStdioFile* file)
{
	this->file = file;
	file->ReadString(line);
	line_pointer = line;
	Forward();
}

void TokenReader::Forward()
{
	while (true) {
		Forward2();
		if (token != Eof)
			return;
		if (!file->ReadString(line))
			return;
		line_pointer = line;
	}
}

void TokenReader::Forward2()
{
	token_str.Empty();

	while (*line_pointer != NULL && _istspace(*line_pointer))
		line_pointer++;
	if (*line_pointer == NULL) {
		token = Eof;
		return;
	}

	LPTSTR target;
	if (IsKeyword(line_pointer, token, target)) {
		line_pointer = target;
		return;
	}
	while (*line_pointer != NULL && !_istspace(*line_pointer) &&
		!IsKeyword(line_pointer, token, target, false)) {
		token_str += *line_pointer;
		line_pointer++;
	}
	token = Sym;
}

bool TokenReader::Follow(LPCTSTR source, LPCTSTR pattern, LPTSTR& target)
{
	int i = 0;
	while (source[i] && pattern[i] && source[i] == pattern[i])
		i++;
	target = (LPTSTR)source + i;
	if (pattern[i] == NULL)
		return true;
	else
		return false;
}

bool TokenReader::IsKeyword(LPCTSTR source, Token& token, LPTSTR& target, bool write_token_str)
{
	static const CString string_array[] = {
		_T("[非终结符]"),
		_T("[nonterminal]"),
		_T("[终结符]"),
		_T("[terminal]"),
		_T("[文法起始符]"),
		_T("[start]"),
		_T("[生成式]"),
		_T("[production]"),
		_T("->"),
		_T("=>"),
		_T("|"),
		_T(";"),
		_T("//")};
	static const Token token_array[] = {
		NSeg,
		NSeg,
		TSeg,
		TSeg,
		SSeg,
		SSeg,
		PSeg,
		PSeg,
		Arrow,
		Arrow,
		Or,
		Semi,
		Eof};
	const int token_count = sizeof(token_array) / sizeof(Token);
	
	for (int i = 0; i < token_count; i++) {
		if (Follow(source, string_array[i], target)) {
			token = token_array[i];
			if (write_token_str)
				token_str = string_array[i];
			return true;
		}
	}
	return false;
}