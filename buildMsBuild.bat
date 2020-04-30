@echo off
pushd
cd DirectZobEngine
CALL generate_sln.bat
python.exe .\strip_emptylines.py .\build\DirectZob\DirectZob.sln
msbuild -m build\DirectZob\DirectZob.sln /p:TargetFrameworkVersion=v4.6.1
popd