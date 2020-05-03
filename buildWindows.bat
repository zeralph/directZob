@echo off
pushd DirectZobEngine
CALL generate_sln.bat
SET target=%1
IF [%1]==[] SET target=Release
python.exe .\strip_emptylines.py .\build\DirectZob\DirectZob.sln
msbuild build\DirectZob\DirectZob.sln /p:TargetFrameworkVersion=v4.6.1 -target:%target%
popd