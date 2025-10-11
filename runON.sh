rm -rf build
cmake -S . -B build -DUSE_FETCH=ON
cmake --build build -j$(nproc)
./bin/SFML_First_Game
