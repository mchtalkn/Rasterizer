# CMakeList.txt : CMake project for the2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("the2")

# Add source to this project's executable.
add_executable (the2 "the2.cpp" "the2.h")
# TODO: Add tests and install targets if needed.

add_subdirectory("code_base")
