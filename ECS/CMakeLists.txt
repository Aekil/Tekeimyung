cmake_minimum_required(VERSION 3.0)
set(EXECUTABLE_NAME "ECS")
project(${EXECUTABLE_NAME})

# All source files using glob
file(GLOB_RECURSE source_files src/*)
file(GLOB_RECURSE include_files include/*)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create library
add_library(${EXECUTABLE_NAME} STATIC ${source_files} ${include_files})


# Store include dir into variable and share it with other projects through cache
set(${EXECUTABLE_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
CACHE INTERNAL "${EXECUTABLE_NAME}: Include Directories" FORCE)