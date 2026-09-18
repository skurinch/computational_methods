# A minimal CMakeLists. This insures cross-platform builds.
cmake_minimum_required(VERSION 3.28)
project(springs LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 20)

find_package( Eigen3 REQUIRED)

add_executable(springs springs.cpp)
target_link_libraries(springs PRIVATE Eigen3::Eigen)

