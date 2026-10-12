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

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "production.h"
#include "token.h"

class Error
{
public:
	int type;
	Token token;
	CString token_str;
	CString GetMessage();
	bool operator==(const Error& error);
};

class Grammar
{
	TokenReader reader;

	int GetLevel(Token token);
	void Sync(int level);
	void ParseSeg(SymbolType type);
	void ParsePSeg();
public:
	SymbolHome symbol_home;
	List<Production> production;

	List<Error> error_list;
	void SetFile(CStdioFile* file);
	void BeginParse();
	void ParseSymbol();
	void ParseProduction();
	void EndParse();
	bool IsAugmented();	//判断是否为拓广文法
};

#endif