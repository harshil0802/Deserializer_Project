cmake_minimum_required(VERSION 3.5)
project(DeserializeProject)

# Define the sources and headers
set(SOURCES src/deserialize.c)
set(HEADERS include/deserialize.h include/deserializeApi.h)

# Add executable target
add_executable(deserialize ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(deserialize PUBLIC include)

# Compiler flags for debug build
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra")

# Compiler flags for release build
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

