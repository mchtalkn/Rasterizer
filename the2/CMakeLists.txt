# CMakeList.txt : CMake project for the2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("the2")

# Add source to this project's executable.

# TODO: Add tests and install targets if needed.


File(GLOB GIVEN_FILES "*.h" "*.cpp") 

add_executable(rasterizer ${GIVEN_FILES} "CameraHandler.h" "CameraHandler.cpp" "GeneratedMesh.h" "GeneratedMesh.cpp"  "Transformation.h" "Transformation.cpp")