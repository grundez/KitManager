# bash build script

mkdir -p build && cd build
cmake ..
make

echo "Build complete: ./build/KitManager"