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
.\DirectZobExe.exe --scene .\resources\carTest.dzs --width 960 --height 540
rem .\DirectZobExe.exe --scene .\resources\carTest.dzs --width 1320 --height 768
rem .\DirectZobExe.exe --scene .\resources\carTest.dzs --width 320 --height 240
rem .\DirectZobExe.exe --scene .\resources\carTest.dzs --width 1920 --height 1080
rem .\DirectZobExe.exe --scene .\resources\testGround.dzs --width 1320 --height 768