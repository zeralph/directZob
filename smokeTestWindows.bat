@echo off
SET target=%1
IF [%target%]==[] ( 
     SET target=Release
)
echo *** SMOKE TEST TARGET %target% ***
pushd DirectZobEngine
XCOPY .\build\DirectZob\%target%\DirectZobExe.exe .. /Y
XCOPY .\build\DirectZob\%target%\DirectZobExe.pdb .. /Y
XCOPY .\build\DirectZob\%target%\libfbxsdk.dll .. /Y
XCOPY .\build\DirectZob\%target%\gainput.dll .. /Y
XCOPY .\build\DirectZob\%target%\optickCore.dll .. /Y
popd
echo *** GO ***
.\DirectZobExe.exe --scene .\resources\carTest.dzs --bench --test