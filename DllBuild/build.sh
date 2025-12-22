#!/bin/bash
rm -r ./bin
rm -r ./build
if [ -d "/build/" ]; then
    cd build
else
    mkdir build
    cd build
fi
cmake ..
make
cd ..
