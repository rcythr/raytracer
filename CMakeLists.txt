cmake_minimum_required(VERSION 2.8)
project(raytrace)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Have CMake look for GLM in the include/directory.
set(GLM_ROOT_DIR ${CMAKE_SOURCE_DIR})

list(APPEND CMAKE_CXX_FLAGS "-O3 -std=c++11 -Wall -pedantic") 

# File all .cc files in the source dir and helpers dir
file(GLOB_RECURSE SOURCES "src/*.cc")

# Setup include path for the lib
include_directories(${CMAKE_SOURCE_DIR}/src)

# Include GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIR})

# Include LibPNG
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

# Build the lib using the cc files found above.
add_executable(raytrace ${SOURCES})
target_link_libraries(raytrace pthread)
target_link_libraries(raytrace ${PNG_LIBRARY})
