# CMakeList.txt : CMake project for the2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("the2")

# Add source to this project's executable.

# TODO: Add tests and install targets if needed.

set (CMAKE_CXX_STANDARD 17)
File(GLOB GIVEN_FILES "*.h" "*.cpp") 

# Add source to this project's executable.
add_executable (the2  ${GIVEN_FILES})
target_link_libraries (the2 ${CMAKE_THREAD_LIBS_INIT})
