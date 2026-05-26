#!/bin/bash
set -e

cmake -S . -B build_release -DCMAKE_BUILD_TYPE=Release
cmake --build build_release --parallel
