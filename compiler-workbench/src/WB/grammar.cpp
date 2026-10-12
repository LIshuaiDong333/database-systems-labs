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
#include "grammar.h"

CString Error::GetMessage() {
	CString message;
	switch (type) {
	case 0:
		message = _T("预期 ");
		switch (token) {
		case NSeg:
			message += _T("段标识符");
			break;
		case Sym:
			message += _T("文法符号");
			break;
		case Arrow:
			message += _T("“=>”");
			break;
		case Semi:
			message += _T("“;”");
		}
		if (!token_str.IsEmpty()) {
			message += _T(" ，发现 “");
			message += token_str;
			message += _T("”");
		} else
			message += _T(" ，发现文件结束");
		break;
	case 1:
		message = _T("没有文法起始符");
		break;
	case 2:
		message = token_str;
		message += _T(" ： 未定义的文法符号");
		break;
	case 3:
		message = token_str;
		message += _T(" ： 重定义的文法符号");
		break;
	case 4:
		message = token_str;
		message += _T(" ： 终结符作为生成式左部");
		break;
	case 5:
		message = token_str;
		message += _T(" ： 源文件中包含非终结符");
		break;
	}
	return message;
}

bool Error::operator==(const Error& error) {
	if (type != error.type)
		return false;

	if ((type == 2 || type == 3 || type == 5) &&
		token_str == error.token_str)
		return true;
	else
		return false;
}

void Grammar::SetFile(CStdioFile* file)
{
	reader.SetFile(file);
}

int Grammar::GetLevel(Token token)
{
	if (token == Eof)
		return 0;
	if (token == NSeg || token == TSeg || token == SSeg || token == PSeg)
		return 1;
	if (token == Semi)
		return 2;
	if (token == Or)
		return 3;
	return 4;
}

void Grammar::Sync(int level)
{
	while (GetLevel(reader.token) > level)
		reader.Forward();
}

void Grammar::ParseSymbol()
{
	if (GetLevel(reader.token) > 1) {
		Error error;
		error.type = 0;
		error.token = NSeg;
		error.token_str = reader.token_str;
		error_list.Add2(error);
	}
	while (reader.token != Eof) {
		Sync(1);
		switch (reader.token) {
		case NSeg:
			reader.Forward();
			ParseSeg(N);
			break;
		case TSeg:
			reader.Forward();
			ParseSeg(T);
			break;
		case SSeg:
			reader.Forward();
			ParseSeg(S);
			break;
		case PSeg:
			reader.Forward();
		}
	}
}

void Grammar::ParseSeg(SymbolType type)
{
	while (GetLevel(reader.token) > 1) {
		if (reader.token == Sym) {
			if (!symbol_home.AddSymbol(reader.token_str, type)) {
				Error error;
				error.type = 3;
				error.token_str = reader.token_str;
				error_list.Add2(error);
			}
		} else {
			Error error;
			error.type = 0;
			error.token = Sym;
			error.token_str = reader.token_str;
			error_list.Add2(error);
		}
		reader.Forward();
	}
}

void Grammar::ParseProduction()
{
	while (reader.token != Eof) {
		Sync(1);
		if (reader.token == PSeg) {
			reader.Forward();
			ParsePSeg();
		} else
			reader.Forward();
	}
}

void Grammar::ParsePSeg()
{
	while (GetLevel(reader.token) > 1) {
		Production p;
		Symbol s;
		if (reader.token == Sym) {
			s = symbol_home.FindSymbol(reader.token_str);
			if (s.IsEof()) {
				Error error;
				error.type = 2;
				error.token_str = reader.token_str;
				error_list.Add2(error);
			}
			if (s.GetType() == T) {
				Error error;
				error.type = 4;
				error.token_str = reader.token_str;
				error_list.Add2(error);
			}
			p.left = s;
		} else {
			Error error;
			error.type = 0;
			error.token = Sym;
			error.token_str = reader.token_str;
			error_list.Add2(error);

			Sync(2);
			if (reader.token == Semi)
				reader.Forward();
			continue;
		}

		reader.Forward();
		if (reader.token != Arrow) {
			Error error;
			error.type = 0;
			error.token = Arrow;
			error.token_str = reader.token_str;
			error_list.Add2(error);

			Sync(2);
			if (reader.token == Semi)
				reader.Forward();
			continue;
		}

		reader.Forward();
		while (GetLevel(reader.token) > 2) {
			while (GetLevel(reader.token) > 3) {
				if (reader.token == Sym) {
					Symbol s = symbol_home.FindSymbol(reader.token_str);
					if (s.IsEof()) {
						Error error;
						error.type = 2;
						error.token_str = reader.token_str;
						error_list.Add2(error);
					}
					p.right.Add(s);
				} else {
					Error error;
					error.type = 0;
					error.token = Sym;
					error.token_str = reader.token_str;
					error_list.Add2(error);
				}
				reader.Forward();
			}
			production.Add2(p);
			if (reader.token == Or) {
				p.right.Clear();
				reader.Forward();
			}
		}
		if (p.right.GetCount() == 0)
			production.Add2(p);
		else
			p.right.Clear();

		if (reader.token == Semi) {
			reader.Forward();
		} else {
			Error error;
			error.type = 0;
			error.token = Semi;
			error.token_str = reader.token_str;
			error_list.Add2(error);
		}
	}
}

void Grammar::BeginParse()
{
	error_list.Clear();
}

void Grammar::EndParse()
{
	if (symbol_home.GetName(symbol_home.GetStart()).IsEmpty()) {
		Error error;
		error.type = 1;
		error_list.Add(error);
	}
}

bool Grammar::IsAugmented()
{
	bool flag = false;

	production.Rewind();
	int production_count = production.GetCount();
	for (int i = 0; i < production_count; i++) {
		//对文法中的每个生成式p
		Production& p = production.GetNext();
		//若p左部的非终结符是文法起始符
		if (p.left.IsStart()) {
			//如果是第一次发现这样的生成式
			if (flag == false) {
				//如果生成式长度为1，且右部的第一个文法符号是非终结符
				if (p.right.GetCount() == 1 && p.right[0].GetType() == N) {
					//记录已经发现这样的生成式
					flag = true;
				} else
					return false;
			} else
				return false;
		}
		p.right.Rewind();
		int symbol_count = p.right.GetCount();
		for (int j = 0; j < symbol_count; j++) {
			//若p右部的文法符号是文法起始符，则该文法不是拓广文法
			if (p.right.GetNext().IsStart())
				return false;
		}
	}

	return flag;
}
