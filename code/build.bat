@echo off

if not defined VSINSTALLDIR (
    call "c:\program files\microsoft visual studio\2022\community\vc\auxiliary\build\vcvarsall.bat" x64
)

set CommonCompilerFlags= -MTd -nologo -fp:fast -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -DGARLIC_INTERNAL=1 -DGARLIC_SLOW=1 -DGARLIC_MARC=0 -DGARLIC_ROOT=1 -DGARLIC_WIN32=1 -FC -Z7
set CommonLinkerFlags= user32.lib gdi32.lib winmm.lib

IF NOT EXIST .\build mkdir .\build
pushd .\build

cl %CommonCompilerFlags% ..\code\garlic.cpp %CommonLinkerFlags%
popd
