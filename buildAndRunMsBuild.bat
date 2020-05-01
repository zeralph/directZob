@echo off
pushd DirectZobEngine
CALL generate_sln.bat
python.exe .\strip_emptylines.py .\build\DirectZob\DirectZob.sln
msbuild build\DirectZob\DirectZob.sln /p:TargetFrameworkVersion=v4.6.1
popd
start /W DirectZobEngine\build\DirectZob\Debug\DirectZobExe.exe --scene .\resources\fbx.xml
