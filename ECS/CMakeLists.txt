#	@Author   Guillaume Labey

cmake_minimum_required(VERSION 3.0)
set(LIBRARY_NAME "ecs")
project(${LIBRARY_NAME})

# include macros
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Macros.cmake)

string(TOUPPER ${LIBRARY_NAME} LIBRARY_NAME_UPPER)

# set the output directory for library
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# All source files using glob
file(GLOB_RECURSE source_files src/*)
file(GLOB_RECURSE include_files include/*)

source_group_files(${source_files})
source_group_files(${include_files})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create library
add_library(${LIBRARY_NAME} STATIC ${source_files} ${include_files})

# Add debug postfix
set_target_properties(${LIBRARY_NAME} PROPERTIES DEBUG_POSTFIX -d)


# Store include dir into variable and share it with other projects through cache
set(${LIBRARY_NAME_UPPER}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
CACHE INTERNAL "${LIBRARY_NAME}: Include Directories" FORCE)

# Store root dir into variable and share it with other projects through cache
set(${LIBRARY_NAME_UPPER}_ROOT ${PROJECT_BINARY_DIR}
CACHE INTERNAL "${LIBRARY_NAME}: Root Directory" FORCE)
