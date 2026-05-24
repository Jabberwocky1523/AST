#!/bin/bash
rm -r ./bin
rm -r ./build
cd "./go"
go build -o libcomgo.a -buildmode=c-archive
cp ./libcomgo.a ../lib
cp ./libcomgo.h ../lib
cd ..
if [ -d "/build/" ]; then
    cd build
else
    mkdir build
    cd build
fi
cmake ..
make
cd ..
mkdir bin
cd ./bin
cp ../build/AST ./
cp ../go/libcomgo.a ./
cp ./AST ../