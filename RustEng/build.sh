cargo build --release
cp ./target/release/libre.dylib ./
mkdir build
cd build
cmake ..
make
cd ..
cp build/re ./