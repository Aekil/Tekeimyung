cmake_minimum_required(VERSION 3.0)
set(EXECUTABLE_NAME "game")
project(${EXECUTABLE_NAME})


# All source files using glob
file(GLOB_RECURSE source_files src/*)
file(GLOB_RECURSE include_files include/*)

# add Engine includes with ENGINE_INCLUDE_DIRS
include_directories(${ENGINE_INCLUDE_DIRS})

# add ECS includes with ECS_INCLUDE_DIRS
include_directories(${ECS_INCLUDE_DIRS})

# Add project includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add external includes
include_directories(${CMAKE_SOURCE_DIR}/external/include)


# Create executable
add_executable(${EXECUTABLE_NAME} ${source_files} ${include_files})


# Link libraries with executable
target_link_libraries(${EXECUTABLE_NAME} ENGINE)



# Copy ressources in Game directory
file(COPY ../ressources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})