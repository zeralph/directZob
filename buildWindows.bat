@echo off
pushd DirectZobEngine
CALL generate_sln.bat
SET target=Release
IF %1.!=.
SET target=%1
python.exe .\strip_emptylines.py .\build\DirectZob\DirectZob.sln
msbuild build\DirectZob\DirectZob.sln /p:TargetFrameworkVersion=v4.6.1 -target:%target%
popd