#	@Author   Guillaume Labey
#	@Author   Mathieu Chassara

cmake_minimum_required(VERSION 3.0)

project("Game")

# include macros
include(${PROJECT_SOURCE_DIR}/../cmake/Macros.cmake)

# define the path of CMake modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake/modules/")

# Add parallel build
if(ENGINE_COMPILER_MSVC)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

set(EXECUTABLE_NAME "game")

# All source files using glob
file(GLOB_RECURSE source_files ${CMAKE_SOURCE_DIR}/src/*)
file(GLOB_RECURSE include_files ${CMAKE_SOURCE_DIR}/include/*)

# Group files into the Visual Studio tree structure
source_group_files(${source_files})
source_group_files(${include_files})

include_directories(${CMAKE_SOURCE_DIR}/include/)

# Find Squirrel Engine
find_package(Squirrel_Engine REQUIRED)

# add Engine includes with SQUIRREL_ENGINE_INCLUDE_DIR
include_directories(${SQUIRREL_ENGINE_INCLUDE_DIR})

# Create executable
add_executable(${EXECUTABLE_NAME} ${source_files} ${include_files})


# Link libraries with executable
target_link_libraries(${EXECUTABLE_NAME} ${SQUIRREL_ENGINE_LIBRARY})



# Copy resources in Game directory
file(COPY ../resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
