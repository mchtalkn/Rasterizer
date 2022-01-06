cmake_minimum_required (VERSION 3.8)

project ("the2")

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall ")

File(GLOB GIVEN_FILES "*.h" "*.cpp")

# Add source to this project's executable.
add_executable (the2  ${GIVEN_FILES})
target_link_libraries (the2 ${CMAKE_THREAD_LIBS_INIT})
