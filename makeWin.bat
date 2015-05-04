@echo off

REM Pfad und Maschinentyp anpassen!
call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

set COMPILER=cl
set LINKER=link

%COMPILER% -c ctr.cpp /EHsc
%LINKER% -out:ctr.exe ctr.obj

del *.obj
