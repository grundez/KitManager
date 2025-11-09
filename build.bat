if not exist build mkdir build
cd build
cmake ..
cmake --build . --config Release
echo Build complete: build\Release\KitManager.exe