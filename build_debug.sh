#!/bin/bash
set -e

cmake -S . -B build_debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build_debug --parallel
