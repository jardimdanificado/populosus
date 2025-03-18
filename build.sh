bruterc "./src/populosus.c --debug"
bruterc "./src/std.c --debug"

rm -rf ./lib
mkdir ./lib

mv src/*.so ./lib/