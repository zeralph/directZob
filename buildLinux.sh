#!/bin/sh
if [ "$1" != "" ]; then
    target="$1"
else
    target=Release
fi
cwd=$(pwd)
echo "building target "$target
if [ "$1" == "Clean" ]; then
    cd ./DirectZobEngine
    rm -rf ./build
    rm -f ../DirectZobExe
    echo "Cleaned build dir"
else
    cd ./DirectZobEngine
    rm -f ../DirectZobExe
    gyp DirectZob.gyp --depth=. --generator-output=./build/DirectZob/ --format=make
    cd ./build/DirectZob/
    make DirectZobExe BUILDTYPE=$target
    cd $cwd
    rm ./DirectZobExe
    cp DirectZobEngine/build/DirectZob/out/$target/DirectZobExe .
    #./DirectZobExe
fi
