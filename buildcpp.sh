./rebuild.sh
cd DllBuild
./build.sh
cd ..
cd ons
cp ../DllBuild/build/libsh.dylib ./
./build.sh
cd ..
cd ef\ latter
cp ../AST ./
cp ../ons/build/libons.dylib ./
./AST ./test2.lua