#!/bin/sh
if [ "$1" != "" ]; then
    target="$1"
else
    target=Release
fi
cwd=$(pwd)
echo "building target "$target
if [ x"$1" != x"Clean" ]; then
    cd ./DirectZobEngine
    rm -f ../DirectZobExe
    gyp DirectZob.gyp --depth=. --generator-output=./build/DirectZob/ --format=make
    cd ./build/DirectZob/
    make DirectZobExe BUILDTYPE=$target -j 4
    cd $cwd
    cp DirectZobEngine/build/DirectZob/out/$target/DirectZobExe .
    #./DirectZobExe
else
    cd ./DirectZobEngine
    rm -rf ./build
    rm -f ../DirectZobExe
    echo "Cleaned build dir"
fi
