bruterc ./src/populosus.c
bruterc ./src/std.c

rm -rf ./lib
mkdir ./lib

mv src/*.so ./lib/