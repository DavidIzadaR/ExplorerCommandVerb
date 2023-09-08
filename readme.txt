Explorer Command Verb Sample
================================
Demonstrates how implement a shell verb using IExplorerCommand. 
This project is an extension of the Microsoft sample with the same name, showing that when we invoke a command
from a submenu under "Show more options" (on Windows 11), the program will not come to the foreground.

Sample Language Implementations
===============================
C++

Files:
=============================================
dll.cpp
dll.def
dll.h
ExplorerCommandVerb.cpp
ExplorerCommandVerb.sln
ExplorerCommandVerb.vcproj
RegisterExtension.cpp
RegisterExtension.h
ShellHelpers.h

To build the sample using the command prompt:
=============================================
     1. Open the Command Prompt window and navigate to the ExplorerCommandVerb directory.
     2. Type msbuild ExplorerCommandVerb.sln.


To build the sample using Visual Studio (preferred method):
===========================================================
     1. Open Windows Explorer and navigate to the ExplorerCommandVerb directory.
     2. Double-click the icon for the ExplorerCommandVerb.sln file to open the file in Visual Studio.
     3. In the Build menu, select Build Solution. The application will be built in the default \Debug or \Release directory.


To run the sample:
=================
     1. Navigate to the directory that contains ExplorerCommandVerb.dll using the command prompt. Make sure you use 64-bit dll on 64-bit Windows.
     2. Create a self-signed certificate.
     3. On Windows 11, execute the script _MakeSamplePackage.ps1 (it assumes you have the self-signed certificate).
     4. On previous Windows versions, type regsvr32 ExplorerCommandVerb.dll.
     5. A new verb "Custom context menu" will be shown on the context menu when you right-click any file.
     6. Two subcommands will allow to open the file with Notepad or Write.

Issue to report to Microsoft:
=============================
     1. In the Windows 11 top-level menu, invoking both commands will execute Notepad.exe or Write.exe and they will appear "above" the Explorer (on the foreground).
     2. In the Windows 11 "Show more options" submenu, the same actions will leave both programs "behind" the Explorer.
