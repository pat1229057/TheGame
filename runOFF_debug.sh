set -e
echo "cleaning and buidling in debug mode..."
rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DUSE_FETCH=OFF
cmake --build build -j$(nproc)

echo "Startbg debugger..."
gdb ./bin/SFML_First_Game 
