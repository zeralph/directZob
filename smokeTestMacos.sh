#!/bin/sh
echo "*** SMOKE TEST ***"
cp ./dependecies/optick/lib/macos/libOptickCore.dylib /usr/local/lib
./DirectZobExe --scene ./resources/carTest.dzs --bench --test