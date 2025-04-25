cmake_minimum_required(VERSION 3.10)
project(cpp-memory-management VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable target
add_executable(main src/main.cpp)

# Include directories
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)