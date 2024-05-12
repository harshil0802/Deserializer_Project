#!/bin/bash

# Ensure the correct number of arguments
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <build_type>"
    exit 1
fi

# Validate build type
if [ "$1" != "Debug" ] && [ "$1" != "Release" ]; then
    echo "Invalid build type. Use 'Debug' or 'Release'."
    exit 1
fi

# Clean previous build directory
rm -rf build
mkdir build
cd build || exit 1

# Run CMake to generate build files
cmake .. -DCMAKE_BUILD_TYPE="$1" || exit 1

# Build the project
make -j || exit 1

# Return to the parent directory
cd ..
