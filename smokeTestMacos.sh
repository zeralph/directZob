#!/bin/sh
echo "*** SMOKE TEST ***"
cp ./dependencies/optick/lib/macos/libOptickCore.dylib /usr/local/lib/
./DirectZobExe --scene ./resources/_carTest.dzs --bench --test