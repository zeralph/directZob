#!/bin/sh
echo "*** SMOKE TEST ***"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./dependencies/optick/lib/linux/
./DirectZobExe --scene ./resources/carTest.dzs --bench --test