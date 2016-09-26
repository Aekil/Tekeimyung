cmake_minimum_required(VERSION 3.0)
set(EXECUTABLE_NAME "engine")
project(${EXECUTABLE_NAME})


# All source files using glob
file(GLOB_RECURSE source_files src/*)
file(GLOB_RECURSE include_files include/*)

# add ECS includes with ECS_INCLUDE_DIRS
include_directories(${ECS_INCLUDE_DIRS})


# Find GLFW library
find_package(GLFW 3.0.0 REQUIRED)

# Find GLEW library
find_package(GLEW 3.0.0 REQUIRED)

# Find Opengl library
find_package(OpenGL REQUIRED)

# Add project and libraries includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})


# Create executable
add_executable(${EXECUTABLE_NAME} ${source_files} ${include_files})


# Link libraries with executable
target_link_libraries(${EXECUTABLE_NAME} ECS ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY})