@echo off
pushd DirectZobEngine
CALL generate_sln.bat
SET target=%1
IF [%target%]==[] ( 
     SET target=Release
)
echo *** BUILDING TARGET %target% ***
C:\Python27\python.exe .\strip_emptylines.py .\build\DirectZob\DirectZob.sln
msbuild build\DirectZob\DirectZob.sln /p:TargetFrameworkVersion=v4.6.1 /p:Configuration=%target%
XCOPY .\build\DirectZob\%target%\DirectZobExe.exe .. /Y
XCOPY .\build\DirectZob\%target%\DirectZobExe.pdb .. /Y
XCOPY .\build\DirectZob\%target%\libfbxsdk.dll .. /Y
popd