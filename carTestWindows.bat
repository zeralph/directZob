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
XCOPY .\build\DirectZob\%target%\OptickCore.dll .. /Y
popd
.\DirectZobExe.exe --scene .\resources\_menu.dzs --width 1280 --height 720
rem .\DirectZobExe.exe --scene .\resources\_menu.dzs --width 1280 --height 768
rem .\DirectZobExe.exe --scene .\resources\menu.dzs --width 320 --height 180