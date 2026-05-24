./rebuild.sh
cd DllBuild
./build.sh
cd ..
cd RustEng
cp ../DllBuild/build/libsh.dylib ./
cargo build --release
cp ./target/release/libre.dylib ./
./build.sh
cp ./build/librt.dylib ./
cp ../AST ./
./AST ./test2.lua