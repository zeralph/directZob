@echo off
pushd .
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
popd
pushd DirectZobEngine
CALL generate_sln.bat
python.exe .\strip_emptylines.py .\build\DirectZob\DirectZob.sln
msbuild build\DirectZob\DirectZob.sln /m /p:Configuration=Release -v:diag
popd
start /W DirectZobEngine\build\DirectZob\Release\DirectZobExe.exe --scene .\resources\fbx.xml --bench
