@echo off

REM Pfad und Maschinentyp anpassen!
call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

set COMPILER=cl
set LINKER=link

%COMPILER% -c coder.cpp /EHsc
%LINKER% -out:coder.exe coder.obj

del *.obj
