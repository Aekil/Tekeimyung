#	@Author   Guillaume Labey
#	@Author   Mathieu Chassara

# include macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# define the path of CMake modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules/")

cmake_minimum_required(VERSION 3.0)
set(EXECUTABLE_NAME "ENGINE")
project(${EXECUTABLE_NAME})

# All source files using glob
file(GLOB_RECURSE source_files src/*)
file(GLOB_RECURSE include_files include/*)
file(GLOB_RECURSE imgui_source_files ${CMAKE_SOURCE_DIR}/external/imgui/*.cpp*)
file(GLOB_RECURSE imGuizmo_source_files ${CMAKE_SOURCE_DIR}/external/imGuizmo/*.cpp*)
file(GLOB_RECURSE jsoncpp_source_files ${CMAKE_SOURCE_DIR}/external/jsoncpp/*.cpp)

#file(GLOB_RECURSE fmod_source_files ${CMAKE_SOURCE_DIR}/external/fmod/lib/*.cs*)
#file(GLOB_RECURSE fmt_source_files ${CMAKE_SOURCE_DIR}/external/fmt/*.cc*)

# Group source files into the Visual Studio tree structure
message(STATUS "Grouping source files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
source_group_files(${source_files})

# Group header files into the Visual Studio tree structure
message(STATUS "Grouping header files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
source_group_files(${include_files})

# Group JsonCpp source files into the Visual Studio tree structure
message(STATUS "Grouping JsonCpp source files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
source_group_files(${jsoncpp_source_files})

# Group ImGui source files into the Visual Studio tree structure
message(STATUS "Grouping ImGui source files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
source_group_files(${imgui_source_files})

# Group ImGuizmo source files into the Visual Studio tree structure
message(STATUS "Grouping ImGuizmo source files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
source_group_files(${imGuizmo_source_files})

# Group FMOD source files into the Visual Studio tree structure
#message(STATUS "Grouping FMOD source files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
#source_group_files(${fmod_source_files})

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

# Find FMOD library

# Find assimp
find_package(Assimp)
if (NOT ASSIMP_FOUND)
    if (ENGINE_OS_WINDOWS)
        set(ASSIMP_ROOT "${CMAKE_SOURCE_DIR}/external/assimp/lib/windows/${ARCH_DIR}")
    elseif (ENGINE_OS_LINUX)
        set(ASSIMP_ROOT "${CMAKE_SOURCE_DIR}/external/assimp/lib/linux")
    endif()

    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/assimp/include")
    find_package(Assimp REQUIRED)
endif()

# Find fmod
find_package(FMOD)
if (NOT FMOD_FOUND)
    if (ENGINE_OS_WINDOWS)
        set(FMOD_ROOT "${CMAKE_SOURCE_DIR}/external/fmod/lib/windows/${ARCH_DIR}")
    elseif (ENGINE_OS_LINUX)
        set(FMOD_ROOT "${CMAKE_SOURCE_DIR}/external/fmod/lib/linux")
    endif()

    set(FMOD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/fmod/include")
    find_package(FMOD REQUIRED)
endif()



# Add project and libraries includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

# Add external includes
include_directories(${CMAKE_SOURCE_DIR}/external/glm/)
include_directories(${CMAKE_SOURCE_DIR}/external/stb_image/)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/)
include_directories(${CMAKE_SOURCE_DIR}/external/imGuizmo/)
include_directories(${CMAKE_SOURCE_DIR}/external/jsoncpp)
include_directories(${FMOD_INCLUDE_DIR})
include_directories(${ASSIMP_INCLUDE_DIR})

# Include dirent only for windows
# Linux have his own header
IF (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/external/dirent)
ENDIF()

# Create executable
add_library(${EXECUTABLE_NAME} STATIC ${source_files} ${imgui_source_files} ${include_files} ${imGuizmo_source_files} ${jsoncpp_source_files})

#Disable ImGui C4244 warnings on Windows
IF (WIN32)
  set_property(SOURCE ${imgui_source_files} APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4244")
ENDIF()

# Link libraries with executable
target_link_libraries(${EXECUTABLE_NAME} ECS ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY} ${FMOD_LIBRARY} ${ASSIMP_LIBRARY})


# Store include dir into variable and share it with other projects through cache
set(${EXECUTABLE_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR}
CACHE INTERNAL "${EXECUTABLE_NAME}: Include Directories" FORCE)
