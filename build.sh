cmake -DCMAKE_BUILD_TYPE=Debug .  -B build
cd build
make
cd ..
cp ./bin/compiler .
