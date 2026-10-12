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

#ifndef PROJECT_H
#define PROJECT_H

//输入文件类型：
//文件类型0：语法文件
//文件类型1：源文件
//输出文件类型：
//文件类型0：FIRST/FOLLOW文件
//文件类型1：LL1分析表文件
//文件类型2：LR0分析表文件
//文件类型3：LR0状态机文件
//文件类型4：SLR分析表文件
//文件类型5：SLR状态机文件
//文件类型6：LR1分析表文件
//文件类型7：LR1状态机文件
//文件类型8：LALR分析表文件
//文件类型9：LALR状态机文件
const int out_type_count = 10;
extern CString out_file_path[];
extern CString out_file_title[];

struct FileInfo
{
	bool io_type;	//false代表输入文件类型，true代表输出文件类型
	int type;
	CString path;
	HTREEITEM item;
};

struct InFileInfo : FileInfo
{
	POSITION pos;
	InFileInfo();
};

struct OutFileInfo : FileInfo
{
	bool exist;
	OutFileInfo();
};

class CProjectTree;

class Project
{
public:
	static CProjectTree& GetTree();

	static CString GetAppFilePath();
	static CString GetINIFilePath();
	static CString GetLLParserFilePath();
	static CString GetLRParserFilePath();
	static CString GetHelpFilePath();
	static CString GetMyDocumentsPath();
	static CString GetProjectBasePath();
	static void SetProjectBasePath(CString project_base_path);

	static bool FileExist(CString file_name);
	static bool DirExist(CString path);
	static CString FullPath(CString path);
	static CString GetFilePath(CString path_name);
	static CString GetFileName(CString path_name);
	static CString GetFileTitle(CString path_name);
	static CString GetFileExt(CString path_name);
	static bool CompareFilePath(CString path_name1, CString path_name2);
	static bool GetRelativePath(CString path_name, CString base_path, CString& relative_path);

	Project();

	CList<InFileInfo, InFileInfo> grm_file_info;
	CList<InFileInfo, InFileInfo> src_file_info;
	OutFileInfo out_file_info[out_type_count];
	bool IsDup(int type, CString path);	//判断文件是否已在工程中存在
	void AddFile(int type, CString path);	//用于将输入文件加入工程
	void AddFile(int type);	//用于将输出文件加入工程
	void RemoveFile(FileInfo* file_info);

	bool is_open;
	bool is_modified;
	CString project_file_name;
	CString project_path;
	void New(CString project_file_name);
	void Open(CString project_file_name);
	void Save();
	void Close();
};

extern Project project;

#endif