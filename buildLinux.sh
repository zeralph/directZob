#!/bin/sh
if [ "$1" != "" ]; then
    target="$1"
else
    target=Release
fi
cwd=$(pwd)
echo "building target "$target
cd ./DirectZobEngine
gyp DirectZob.gyp --depth=. --generator-output=./build/DirectZob/ --format=make
cd ./build/DirectZob/
make DirectZobExe BUILDTYPE=$target
cd $cwd
rm ./DirectZobExe
cp DirectZobEngine/build/DirectZob/out/$target/DirectZobExe .
#./DirectZobExe
