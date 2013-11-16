# Microsoft Developer Studio Project File - Name="Fourmis_simul" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Fourmis_simul - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Fourmis_simul.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Fourmis_simul.mak" CFG="Fourmis_simul - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Fourmis_simul - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Fourmis_simul - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Fourmis_simul - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sdl.lib sdlmain.lib sdl_mixer.lib opengl32.lib glu32.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /incremental:yes /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=upx -9 Release/Fourmis_simul.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Fourmis_simul - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /vd0 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__AI_DEBUG" /Fr /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sdl.lib sdlmain.lib sdl_mixer.lib opengl32.lib glu32.lib /nologo /subsystem:console /map /debug /debugtype:both /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "Fourmis_simul - Win32 Release"
# Name "Fourmis_simul - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib\3dsloader.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\C3DObject.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\C3DObjEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\CBubbleText.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\CCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\CSDLChrono.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\CSoundManager.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\customassert.c
# End Source File
# Begin Source File

SOURCE=.\lib\font.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\particles.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\tga.c
# End Source File
# Begin Source File

SOURCE=.\lib\TVector.cpp
# End Source File
# End Group
# Begin Group "AI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AI\AI_ants.cpp
# End Source File
# Begin Source File

SOURCE=.\AI\AI_enemies.cpp
# End Source File
# Begin Source File

SOURCE=.\AI\AI_environment.cpp
# End Source File
# Begin Source File

SOURCE=.\AI\AI_graphic_objects.cpp
# End Source File
# Begin Source File

SOURCE=.\AI\AI_nest.cpp
# End Source File
# Begin Source File

SOURCE=.\AI\AI_objects.cpp
# End Source File
# Begin Source File

SOURCE=.\AI\AI_renders.cpp
# End Source File
# Begin Source File

SOURCE=.\AI\AI_renders_in_menu.cpp
# End Source File
# Begin Source File

SOURCE=.\AI\AI_ring.cpp
# End Source File
# Begin Source File

SOURCE=.\AI\AI_shared.cpp
# End Source File
# Begin Source File

SOURCE=.\AI\AI_technical.cpp
# End Source File
# Begin Source File

SOURCE=.\AI\AI_world.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CMap.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Minimap.cpp
# End Source File
# Begin Source File

SOURCE=.\objects.cpp
# End Source File
# Begin Source File

SOURCE=.\res.rc
# End Source File
# Begin Source File

SOURCE=.\scene.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneio.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "lib No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib\3dsloader.h
# End Source File
# Begin Source File

SOURCE=.\lib\C3DObject.h
# End Source File
# Begin Source File

SOURCE=.\lib\C3DObjEditor.h
# End Source File
# Begin Source File

SOURCE=.\lib\CBoundingBox.h
# End Source File
# Begin Source File

SOURCE=.\lib\CBubbleText.h
# End Source File
# Begin Source File

SOURCE=.\lib\CCamera.h
# End Source File
# Begin Source File

SOURCE=.\lib\CColor.h
# End Source File
# Begin Source File

SOURCE=.\lib\CSDLChrono.h
# End Source File
# Begin Source File

SOURCE=.\lib\CSoundManager.h
# End Source File
# Begin Source File

SOURCE=.\lib\customassert.h
# End Source File
# Begin Source File

SOURCE=.\lib\material.h
# End Source File
# Begin Source File

SOURCE=.\lib\particles.h
# End Source File
# Begin Source File

SOURCE=.\lib\tga.h
# End Source File
# Begin Source File

SOURCE=.\lib\TSingleton.h
# End Source File
# Begin Source File

SOURCE=.\lib\TVector.h
# End Source File
# Begin Source File

SOURCE=.\lib\TVector_impl.h
# End Source File
# End Group
# Begin Group "AI No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AI\AI_ants.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_ants_impl.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_enemies.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_enemies_impl.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_environment.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_environment_impl.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_graphic_objects.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_graphic_objects_impl.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_indicators.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_indicators_impl.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_myrmecology.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_nest.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_nest_impl.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_objects.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_objects_impl.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_renders.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_renders_impl.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_ring.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_ring_impl.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_shared.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_shared_impl.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_technical.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_technical_impl.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_tokens.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_world.h
# End Source File
# Begin Source File

SOURCE=.\AI\AI_world_impl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AI\AI_renders_in_menu.h
# End Source File
# Begin Source File

SOURCE=.\Buttons.h
# End Source File
# Begin Source File

SOURCE=.\CMap.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\Default_settings.h
# End Source File
# Begin Source File

SOURCE=.\lib\font.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\Menu.h
# End Source File
# Begin Source File

SOURCE=.\Menu_type.h
# End Source File
# Begin Source File

SOURCE=.\minimap.h
# End Source File
# Begin Source File

SOURCE=.\objects.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\scene.h
# End Source File
# Begin Source File

SOURCE=.\sceneio.h
# End Source File
# Begin Source File

SOURCE=.\Settings.h
# End Source File
# Begin Source File

SOURCE=.\sounds.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# End Group
# End Target
# End Project
