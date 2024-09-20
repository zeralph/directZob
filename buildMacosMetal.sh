#!/bin/sh
export SDKROOT="/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk"
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
    gyp DirectZob.gyp --depth=. --generator-output=./build/DirectZob/ --format=make -Dminifb_macos_flags="USE_METAL_API"
    cd ./build/DirectZob/
    sed -i -e "1 s/.*/#\!\/usr\/bin\/env python3/" gyp-mac-tool
    make DirectZobExe BUILDTYPE=$target -j 4
    cd $cwd
    cp DirectZobEngine/build/DirectZob/out/$target/DirectZobExe .
    #./DirectZobExe
fi
