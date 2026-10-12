# Microsoft Developer Studio Project File - Name="WB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WB - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WB.mak" CFG="WB - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WB - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WB - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WB - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 htmlhelp.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "WB - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 htmlhelp.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "WB - Win32 Release"
# Name "WB - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\arrow.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\automaton.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoView.cpp
# End Source File
# Begin Source File

SOURCE=.\bezier.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowseForFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\cirauto.cpp
# End Source File
# Begin Source File

SOURCE=.\circle.cpp
# End Source File
# Begin Source File

SOURCE=.\first.cpp
# End Source File
# Begin Source File

SOURCE=.\follow.cpp
# End Source File
# Begin Source File

SOURCE=.\GenerateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\global.cpp
# End Source File
# Begin Source File

SOURCE=.\grammar.cpp
# End Source File
# Begin Source File

SOURCE=.\graph_ds.cpp
# End Source File
# Begin Source File

SOURCE=.\grmDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\grmView.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\html.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmView.cpp
# End Source File
# Begin Source File

SOURCE=.\lalr.cpp
# End Source File
# Begin Source File

SOURCE=.\lex.cpp
# End Source File
# Begin Source File

SOURCE=.\ll1.cpp
# End Source File
# Begin Source File

SOURCE=.\lr.cpp
# End Source File
# Begin Source File

SOURCE=.\lr0.cpp
# End Source File
# Begin Source File

SOURCE=.\lr1.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\mymath.cpp
# End Source File
# Begin Source File

SOURCE=.\NewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\production.cpp
# End Source File
# Begin Source File

SOURCE=.\project.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectTree.cpp
# End Source File
# Begin Source File

SOURCE=.\rectauto.cpp
# End Source File
# Begin Source File

SOURCE=.\rotate.cpp
# End Source File
# Begin Source File

SOURCE=.\scbarg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\sizecbar.cpp
# End Source File
# Begin Source File

SOURCE=.\slr.cpp
# End Source File
# Begin Source File

SOURCE=.\srcDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\srcView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\symbol.cpp
# End Source File
# Begin Source File

SOURCE=.\symbolset.cpp
# End Source File
# Begin Source File

SOURCE=.\title.cpp
# End Source File
# Begin Source File

SOURCE=.\token.cpp
# End Source File
# Begin Source File

SOURCE=.\vertex.cpp
# End Source File
# Begin Source File

SOURCE=.\WB.cpp
# End Source File
# Begin Source File

SOURCE=.\WB.rc
# End Source File
# Begin Source File

SOURCE=.\WhiteStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\arrow.h
# End Source File
# Begin Source File

SOURCE=.\AutoDoc.h
# End Source File
# Begin Source File

SOURCE=.\automaton.h
# End Source File
# Begin Source File

SOURCE=.\AutoView.h
# End Source File
# Begin Source File

SOURCE=.\bezier.h
# End Source File
# Begin Source File

SOURCE=.\BrowseForFolder.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\cirauto.h
# End Source File
# Begin Source File

SOURCE=.\circle.h
# End Source File
# Begin Source File

SOURCE=.\first.h
# End Source File
# Begin Source File

SOURCE=.\follow.h
# End Source File
# Begin Source File

SOURCE=.\GenerateDlg.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\grammar.h
# End Source File
# Begin Source File

SOURCE=.\graph.h
# End Source File
# Begin Source File

SOURCE=.\graph_ds.h
# End Source File
# Begin Source File

SOURCE=.\grmDoc.h
# End Source File
# Begin Source File

SOURCE=.\grmView.h
# End Source File
# Begin Source File

SOURCE=.\HtmDoc.h
# End Source File
# Begin Source File

SOURCE=.\html.h
# End Source File
# Begin Source File

SOURCE=.\HtmView.h
# End Source File
# Begin Source File

SOURCE=.\lalr.h
# End Source File
# Begin Source File

SOURCE=.\lex.h
# End Source File
# Begin Source File

SOURCE=.\list.h
# End Source File
# Begin Source File

SOURCE=.\ll1.h
# End Source File
# Begin Source File

SOURCE=.\lr.h
# End Source File
# Begin Source File

SOURCE=.\lr0.h
# End Source File
# Begin Source File

SOURCE=.\lr1.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\map.h
# End Source File
# Begin Source File

SOURCE=.\mymath.h
# End Source File
# Begin Source File

SOURCE=.\NewDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewProjectDlg.h
# End Source File
# Begin Source File

SOURCE=.\production.h
# End Source File
# Begin Source File

SOURCE=.\project.h
# End Source File
# Begin Source File

SOURCE=.\ProjectTree.h
# End Source File
# Begin Source File

SOURCE=.\rectauto.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\rotate.h
# End Source File
# Begin Source File

SOURCE=.\scbarg.h
# End Source File
# Begin Source File

SOURCE=.\SelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\set.h
# End Source File
# Begin Source File

SOURCE=.\sizecbar.h
# End Source File
# Begin Source File

SOURCE=.\slr.h
# End Source File
# Begin Source File

SOURCE=.\srcDoc.h
# End Source File
# Begin Source File

SOURCE=.\srcView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\symbol.h
# End Source File
# Begin Source File

SOURCE=.\symbolset.h
# End Source File
# Begin Source File

SOURCE=.\title.h
# End Source File
# Begin Source File

SOURCE=.\token.h
# End Source File
# Begin Source File

SOURCE=.\vertex.h
# End Source File
# Begin Source File

SOURCE=.\WB.h
# End Source File
# Begin Source File

SOURCE=.\WhiteStatic.h
# End Source File
# Begin Source File

SOURCE=.\WizardDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\grmDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WB.ico
# End Source File
# Begin Source File

SOURCE=.\res\WB.rc2
# End Source File
# End Group
# End Target
# End Project
