#	@Author   Guillaume Labey
#	@Author   Mathieu Chassara

cmake_minimum_required(VERSION 3.0)
set(EXECUTABLE_NAME "game")
project(${EXECUTABLE_NAME})

# All source files using glob
file(GLOB_RECURSE source_files src/*)
file(GLOB_RECURSE include_files include/*)

# Group source files into the Visual Studio tree structure
message(STATUS "Grouping source files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
source_group_files(${source_files})

# Group header files into the Visual Studio tree structure
message(STATUS "Grouping header files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
source_group_files(${include_files})

# add Engine includes with ENGINE_INCLUDE_DIRS
include_directories(${ENGINE_INCLUDE_DIRS})

# add ECS includes with ECS_INCLUDE_DIRS
include_directories(${ECS_INCLUDE_DIRS})

# Add project includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add external includes
include_directories(${CMAKE_SOURCE_DIR}/external/glm)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/)
include_directories(${CMAKE_SOURCE_DIR}/external/imGuizmo/)

# Include dirent only for windows
# Linux have his own header
IF (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/external/dirent)
ENDIF()

# Use static lib of glew
# Even if the lib is not directly linked,
# the macro has to be set to prevent warnings
add_definitions(-DGLEW_STATIC)

# Create executable
add_executable(${EXECUTABLE_NAME} ${source_files} ${include_files})


# Link libraries with executable
target_link_libraries(${EXECUTABLE_NAME} ENGINE)



# Copy resources in Game directory
file(COPY ../resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
