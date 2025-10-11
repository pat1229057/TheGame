rm -rf build
cmake -S . -B build -DUSE_FETCH=OFF
cmake --build build -j$(nproc)
./bin/SFML_First_Game 