#!/bin/sh
cwd=$(pwd)
gyp ./DirectZobEngine/DirectZob.gyp --depth=./DirectZobEngine --generator-output=./build/DirectZob/
cd ./DirectZobEngine/build/DirectZob/
make DirectZobExe BUILDTYPE=Release
cd $cwd
rm ./DirectZobExe
cp DirectZobEngine/build/DirectZob/out/Release/DirectZobExe .
./DirectZobExe
