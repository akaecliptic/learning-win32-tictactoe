@echo off

set msvc="c:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvars64.bat"

if not defined devmode ( call %msvc% )

if errorlevel 0 ( set devmode=1 )

cl /DUNICODE *.cpp /Fo"./objects/" /Fe"app" /std:c++17 /EHsc /link /subsystem:windows user32.lib

if errorlevel 1 (
    echo: 
    echo compilation error. please check above.
) else (
    cls
    echo compilation success. 
)

echo Running application: 
echo:
app.exe

echo exiting with - %errorlevel%
