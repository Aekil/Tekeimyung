cmake_minimum_required(VERSION 3.0)
set(EXECUTABLE_NAME "ENGINE")
project(${EXECUTABLE_NAME})

# All source files using glob
file(GLOB_RECURSE source_files src/*)
file(GLOB_RECURSE include_files include/*)
file(GLOB_RECURSE imgui_source_files ${CMAKE_SOURCE_DIR}/external/imgui/*.cpp*)
#file(GLOB_RECURSE fmt_source_files ${CMAKE_SOURCE_DIR}/external/fmt/*.cc*)

# Group source files into the Visual Studio tree structure
message(STATUS "Grouping source files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
source_group_files(${source_files})

# Group header files into the Visual Studio tree structure
message(STATUS "Grouping header files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
source_group_files(${include_files})

# Group ImGui source files into the Visual Studio tree structure
message(STATUS "Grouping ImGui source files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
source_group_files(${imgui_source_files})

# Group fmt source files into the Visual Studio tree structure
#message(STATUS "Grouping fmt source files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
#source_group_files(${fmt_source_files})

# add ECS includes with ECS_INCLUDE_DIRS
include_directories(${ECS_INCLUDE_DIRS})

# Use static lib of glew
add_definitions(-DGLEW_STATIC)


# Find GLFW library
find_package(GLFW 3.0.0 REQUIRED)

# Find GLEW library
find_package(GLEW 3.0.0 REQUIRED)

# Find Opengl library
find_package(OpenGL REQUIRED)

# Add project and libraries includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

# Add external includes
include_directories(${CMAKE_SOURCE_DIR}/external/glm/)
include_directories(${CMAKE_SOURCE_DIR}/external/stb_image/)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/)
#include_directories(${CMAKE_SOURCE_DIR}/external/fmt/)

# Create executable
add_library(${EXECUTABLE_NAME} STATIC ${source_files} ${imgui_source_files} ${include_files}) #${fmt_source_files}

#Disable ImGui C4244 warnings on Windows
IF (WIN32)
  set_property(SOURCE ${imgui_source_files} APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4244")
ENDIF()

# Link libraries with executable
target_link_libraries(${EXECUTABLE_NAME} ECS ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY})


# Store include dir into variable and share it with other projects through cache
set(${EXECUTABLE_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR}
CACHE INTERNAL "${EXECUTABLE_NAME}: Include Directories" FORCE)
