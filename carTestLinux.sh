#!/bin/sh
echo "*** CAR TEST ***"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./dependencies/optick/lib/linux/
./DirectZobExe --scene ./resources/carTest.dzs --width 1024 --height 768
