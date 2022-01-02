#!/bin/sh
echo "*** CAR TEST ***"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./dependencies/optick/lib/macos/
./DirectZobExe --scene ./resources/carTest.dzs  --width 960 --height 540
#--with 640 --height 400