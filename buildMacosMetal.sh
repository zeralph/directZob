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
    #cd ./DirectZobEngine
    rm -rf ./dist
    rm -rf ./build
    #rm -f ../DirectZobExe
    echo "Cleaned build dir"
else
    #cd ./DirectZobEngine
    #rm -f ../DirectZobExe
    gyp DirectZob.gyp --depth=. --generator-output=./build/ --format=make -Dminifb_macos_flags="USE_METAL_API"
    cd ./build/
    sed -i -e "1 s/.*/#\!\/usr\/bin\/env python3/" gyp-mac-tool
    sed -i -e "1 s/.*/#\!\/usr\/bin\/env python3/" gyp-mac-tool-e
    #`../../../wxWidgets-3.2.6/build/wx-config --cxxflags`
    make BUILDTYPE=$target -j 4
    #make DirectZobExe BUILDTYPE=$target -j 4
    cd $cwd
    echo $pwd
    mkdir ./dist/$target
    cp ./build/out/$target/DirectZobEditor ./dist/$target
    cp ./build/out/$target/DirectZobExe ./dist/$target
    #./DirectZobExe
fi
