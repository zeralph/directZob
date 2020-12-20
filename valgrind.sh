valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./DirectZobEngine/build/DirectZob/out/Debug/DirectZobExe --scene ./resources/wow.dzs --bench --test.sh
