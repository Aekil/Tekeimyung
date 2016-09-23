cmake_minimum_required(VERSION 3.0)
set(EXECUTABLE_NAME "engine")
project(${EXECUTABLE_NAME})


# All source files using glob
file(GLOB_RECURSE source_files src/*)

# add ECS includes with ECS_INCLUDE_DIRS
include_directories(${ECS_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


# Create executable
add_executable(${EXECUTABLE_NAME} ${source_files})

# Link libraries with executable
target_link_libraries(${EXECUTABLE_NAME} ECS)