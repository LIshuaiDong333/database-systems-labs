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
#include "project.h"
#include "WB.h"
#include "MainFrm.h"

CString out_file_path[] = {
	_T("输出\\FIRST和FOLLOW集.htm"),
	_T("输出\\LL1分析表.htm"),
	_T("输出\\LR0分析表.htm"),
	_T("输出\\LR0状态机.dfa"),
	_T("输出\\SLR分析表.htm"),
	_T("输出\\SLR状态机.dfa"),
	_T("输出\\LR1分析表.htm"),
	_T("输出\\LR1状态机.dfa"),
	_T("输出\\LALR分析表.htm"),
	_T("输出\\LALR状态机.dfa")};

CString out_file_title[] = {
	_T("FIRST/FOLLOW集"),
	_T("LL1分析表"),
	_T("LR0分析表"),
	_T("LR0状态机"),
	_T("SLR分析表"),
	_T("SLR状态机"),
	_T("LR1分析表"),
	_T("LR1状态机"),
	_T("LALR分析表"),
	_T("LALR状态机")};

InFileInfo::InFileInfo()
{
	io_type = false;
}

OutFileInfo::OutFileInfo()
{
	io_type = true;
}

CProjectTree& Project::GetTree()
{
	return ((CMainFrame*)AfxGetMainWnd())->project_tree;
}

CString Project::GetAppFilePath()
{
	CString app_file_path;
	LPTSTR buffer = app_file_path.GetBuffer(MAX_PATH);
	GetModuleFileName(NULL, buffer, MAX_PATH);
	app_file_path.ReleaseBuffer();
	app_file_path = GetFilePath(app_file_path);
	return app_file_path;
}

CString Project::GetINIFilePath()
{
	return GetAppFilePath() + _T("profile.ini");
}

CString Project::GetLLParserFilePath()
{
	return GetAppFilePath() + _T("LLDemo.exe");
}

CString Project::GetLRParserFilePath()
{
	return GetAppFilePath() + _T("LRDemo.exe");
}

CString Project::GetHelpFilePath()
{
	return GetAppFilePath() + _T("WB帮助.chm");
}

CString Project::GetMyDocumentsPath()
{
	CString my_documents_path;
	LPTSTR buffer = my_documents_path.GetBuffer(MAX_PATH);
	SHGetSpecialFolderPath(NULL, buffer, CSIDL_PERSONAL, FALSE);
	my_documents_path.ReleaseBuffer();
	my_documents_path = FullPath(my_documents_path);
	return my_documents_path;
}

CString Project::GetProjectBasePath()
{
	CString project_base_path;
	CString ini_file_name = GetINIFilePath();
	GetPrivateProfileString(_T("PATH"), _T("PROJECTBASEPATH"),
		GetMyDocumentsPath(),
		project_base_path.GetBuffer(MAX_PATH), MAX_PATH,
		ini_file_name);
	project_base_path.ReleaseBuffer();
	return project_base_path;
}

void Project::SetProjectBasePath(CString project_base_path)
{
	CString ini_file_name = GetINIFilePath();
	WritePrivateProfileString(_T("PATH"), _T("PROJECTBASEPATH"),
		project_base_path,
		ini_file_name);
}

bool Project::FileExist(CString file_name)
{
	CFileFind finder;
	return (finder.FindFile(file_name) != 0);
}

bool Project::DirExist(CString path)
{
	CFileFind finder;
	if (finder.FindFile(path)) {
		finder.FindNextFile();
		return (finder.IsDirectory() != 0);
	} else {
		return false;
	}
}

CString Project::FullPath(CString path)
{
	if (path[path.GetLength() - 1] != '\\')
		path += '\\';
	return path;
}

CString Project::GetFilePath(CString path_name)
{
	int right_bound = path_name.ReverseFind('\\');
	if (right_bound != -1)
		return path_name.Left(right_bound + 1);
	else {
		CString path;
		GetCurrentDirectory(MAX_PATH, path.GetBuffer(MAX_PATH));
		path.ReleaseBuffer();
		return FullPath(path);
	}
}

CString Project::GetFileName(CString path_name)
{
	int length = path_name.GetLength();
	int left_bound = path_name.ReverseFind('\\');
	if (left_bound != -1)
		return path_name.Right(length - left_bound - 1);
	else
		return path_name;
}

CString Project::GetFileTitle(CString path_name)
{
	CString file_name = GetFileName(path_name);
	int right_bound = file_name.ReverseFind('.');
	if (right_bound != -1)
		return file_name.Left(right_bound);
	else
		return file_name;
}

CString Project::GetFileExt(CString path_name)
{
	CString file_name = GetFileName(path_name);
	int length = file_name.GetLength();
	int left_bound = file_name.ReverseFind('.');
	if (left_bound != -1)
		return file_name.Right(length - left_bound - 1);
	else
		return CString();
}

bool Project::CompareFilePath(CString path_name1, CString path_name2)
{
	path_name1.MakeLower();
	path_name2.MakeLower();
	if (path_name1 == path_name2)
		return true;
	else
		return false;
}

bool Project::GetRelativePath(CString path_name, CString base_path, CString& relative_path)
{
	path_name.MakeLower();
	path_name.TrimLeft();
	int path_name_length = path_name.GetLength();
	base_path = FullPath(base_path);
	base_path.MakeLower();
	base_path.TrimLeft();
	int base_path_length = base_path.GetLength();
	if (path_name_length >= base_path_length && path_name.Left(base_path_length) == base_path) {
		relative_path = path_name.Right(path_name_length - base_path_length);
		return true;
	} else {
		relative_path = path_name;
		return false;
	}
}

Project::Project()
{
	is_open = false;
	for (int i = 0; i < out_type_count; i++) {
		out_file_info[i].exist = false;
		out_file_info[i].type = i;
		out_file_info[i].path = out_file_path[i];
	}
}

bool Project::IsDup(int type, CString path)
{
	CString relative_path;
	GetRelativePath(path, project_path, relative_path);
	if (type == 0) {
		POSITION pos = grm_file_info.GetHeadPosition();
		while (pos) {
			InFileInfo* file_info = &grm_file_info.GetNext(pos);
			if (CompareFilePath(relative_path, file_info->path))
				return true;
		}
	} else {
		POSITION pos = src_file_info.GetHeadPosition();
		while (pos) {
			InFileInfo* file_info = &src_file_info.GetNext(pos);
			if (CompareFilePath(relative_path, file_info->path))
				return true;
		}
	}
	return false;
}

void Project::AddFile(int type, CString path)
{
	InFileInfo* file_info;
	POSITION pos = 0;
	if (type == 0) {
		CString title = GetFileTitle(path);
		POSITION current_pos = grm_file_info.GetHeadPosition();
		while (current_pos) {
			InFileInfo* current_file_info = &grm_file_info.GetAt(current_pos);
			CString current_title = GetFileTitle(current_file_info->path);
			if (current_title > title) {
				pos = grm_file_info.InsertBefore(current_pos, InFileInfo());
				file_info = &grm_file_info.GetAt(pos);
				break;
			}
			grm_file_info.GetNext(current_pos);
		}
		if (pos == 0) {
			pos = grm_file_info.AddTail(InFileInfo());
			file_info = &grm_file_info.GetAt(pos);
		}
	} else {
		CString title = GetFileTitle(path);
		POSITION current_pos = src_file_info.GetHeadPosition();
		while (current_pos) {
			InFileInfo* current_file_info = &src_file_info.GetAt(current_pos);
			CString current_title = GetFileTitle(current_file_info->path);
			if (current_title > title) {
				pos = src_file_info.InsertBefore(current_pos, InFileInfo());
				file_info = &src_file_info.GetAt(pos);
				break;
			}
			src_file_info.GetNext(current_pos);
		}
		if (pos == 0) {
			pos = src_file_info.AddTail(InFileInfo());
			file_info = &src_file_info.GetAt(pos);
		}
	}
	file_info->type = type;
	GetRelativePath(path, project_path, file_info->path);
	file_info->pos = pos;
	GetTree().AddFile(file_info);
	is_modified = true;
}

void Project::AddFile(int type)
{
	OutFileInfo* file_info = &out_file_info[type];
	if (file_info->exist)
		return;
	file_info->exist = true;
	GetTree().AddFile(file_info);
	is_modified = true;
}

void Project::RemoveFile(FileInfo* file_info)
{
	GetTree().RemoveFile(file_info);
	if (file_info->io_type == false) {
		POSITION pos = ((InFileInfo*)file_info)->pos;
		if (file_info->type == 0)
			grm_file_info.RemoveAt(pos);
		else
			src_file_info.RemoveAt(pos);
	} else {
		out_file_info[file_info->type].exist = false;
	}
	is_modified = true;
}

void Project::New(CString project_file_name)
{
	this->project_file_name = project_file_name;
	project_path = GetFilePath(project_file_name);
	is_open = true;
	is_modified = true;
	Save();
	GetTree().Open();
}

void Project::Open(CString project_file_name)
{
	CFile project_file;
	if (!project_file.Open(project_file_name, CFile::modeRead)) {
		CString prompt(_T("没有找到 "));
		prompt += project_file.GetFilePath();
		prompt += _T("。");
		AfxMessageBox(prompt);
		return;
	} else {
		this->project_file_name = project_file_name;
		project_path = GetFilePath(project_file_name);
		GetTree().Open();
	}
	CArchive ar(&project_file, CArchive::load);

	int grm_file_count;
	ar >> grm_file_count;
	for (int i = 0; i < grm_file_count; i++) {
		CString path;
		ar >> path;
		AddFile(0, path);
	}
	int src_file_count;
	ar >> src_file_count;
	for (i = 0; i < src_file_count; i++) {
		CString path;
		ar >> path;
		AddFile(1, path);
	}
	for (i = 0; i < out_type_count; i++) {
		int exist;
		ar >> exist;
		if (exist)
			AddFile(i);
	}

	ar.Close();
	project_file.Close();
	is_open = true;
	is_modified = false;
}

void Project::Save()
{
	if (is_open && is_modified) {
		CFile project_file;
		project_file.Open(project_file_name, CFile::modeCreate | CFile::modeWrite);
		CArchive ar(&project_file, CArchive::store);

		ar << grm_file_info.GetCount();
		POSITION pos = grm_file_info.GetHeadPosition();
		while (pos != 0) {
			InFileInfo* file_info = &grm_file_info.GetNext(pos);
			ar << file_info->path;
		}
		ar << src_file_info.GetCount();
		pos = src_file_info.GetHeadPosition();
		while (pos != 0) {
			InFileInfo* file_info = &src_file_info.GetNext(pos);
			ar << file_info->path;
		}
		for (int i = 0; i < out_type_count; i++)
			ar << (int)out_file_info[i].exist;

		ar.Close();
		project_file.Close();
		is_modified = false;
	}
}

void Project::Close()
{
	Save();
	grm_file_info.RemoveAll();
	src_file_info.RemoveAll();
	for (int i = 0; i < out_type_count; i++)
		out_file_info[i].exist = false;
	GetTree().Close();
	is_open = false;
}

Project project;