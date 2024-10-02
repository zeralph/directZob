#!/bin/sh

if [ "$1" != "" ]; then
    target="$1"
else
    target=Release
fi
cwd=$(pwd)
echo "building target "$target
if [ "$1" == "Clean" ]; then
    rm -rf ./dist
    rm -rf ./build
    echo "Cleaned build dir"
else
	pip3 install --upgrade six>=1.13.0
    gyp DirectZob.gyp --depth=. --generator-output=./build/ --format=make
    cd ./build/


    make BUILDTYPE=$target -j 4
    cd $cwd
    cp DirectZobEngine/build/DirectZob/out/$target/DirectZobExe .
    #./DirectZobExe
fi
