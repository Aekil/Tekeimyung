#	@Author   Guillaume Labey
#	@Author   Mathieu Chassara

cmake_minimum_required(VERSION 3.0)
set(EXECUTABLE_NAME "ENGINE")
project(${EXECUTABLE_NAME})

# All source files using glob
file(GLOB_RECURSE source_files src/*)
file(GLOB_RECURSE include_files include/*)
file(GLOB_RECURSE imgui_source_files ${CMAKE_SOURCE_DIR}/external/imgui/*.cpp*)
file(GLOB_RECURSE imGuizmo_source_files ${CMAKE_SOURCE_DIR}/external/imGuizmo/*.cpp*)
file(GLOB_RECURSE jsoncpp_source_files ${CMAKE_SOURCE_DIR}/external/jsoncpp/*.cpp)


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

# add ECS includes with ECS_INCLUDE_DIRS
include_directories(${ECS_INCLUDE_DIRS})

# Use static lib of glew
add_definitions(-DGLEW_STATIC)


# Find GLFW library
find_package(GLFW 3.0.0 REQUIRED)

# Find GLEW library
find_package(GLEW REQUIRED)

# Find Opengl library
find_package(OpenGL REQUIRED)

# Find FMOD library

#Set Assimp directory and library on Windows
IF (WIN32)
  set(ASSIMP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/Assimp/include)
  IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
    find_library(ASSIMP_LIBRARY assimp PATHS ${CMAKE_SOURCE_DIR}/external/Assimp/lib/x64)
  ELSE()
    find_library(ASSIMP_LIBRARY assimp PATHS ${CMAKE_SOURCE_DIR}/external/Assimp/lib/x86)
  ENDIF()
  set(ASSIMP_FOUND true)
ELSE()
  find_package(Assimp REQUIRED)
ENDIF()

IF (NOT ASSIMP_FOUND)
  message(FATAL_ERROR "Can't find assimp library")
ENDIF()

IF (WIN32)
#Set Freetype directory and library on Windows
  set(FREETYPE_INCLUDE_DIR_ft2build ${CMAKE_SOURCE_DIR}/external/freetype/include/freetype2)
  set(FREETYPE_LIBRARIES ${CMAKE_SOURCE_DIR}/external/freetype/lib/freetype.lib)
  set(FREETYPE_FOUND true)
ELSE()
    find_package(Freetype REQUIRED)
ENDIF()

IF (NOT FREETYPE_FOUND)
  message(FATAL_ERROR "Can't find freetype library")
ENDIF()

IF (WIN32)
#Set FMOD directory and library on Windows
  set(FMOD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/fmod/inc)
    find_library(FMOD_LIBRARY fmodL64_vc PATHS ${CMAKE_SOURCE_DIR}/external/fmod/lib)
ELSE()
    find_package(FMOD REQUIRED)
ENDIF()


# Add project and libraries includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${FMOD_INCLUDE_DIR})

# Add external includes
include_directories(${CMAKE_SOURCE_DIR}/external/glm/)
include_directories(${CMAKE_SOURCE_DIR}/external/stb_image/)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/)
include_directories(${CMAKE_SOURCE_DIR}/external/imGuizmo/)
include_directories(${CMAKE_SOURCE_DIR}/external/jsoncpp)
include_directories(${FREETYPE_INCLUDE_DIR_ft2build})
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
target_link_libraries(${EXECUTABLE_NAME} ECS ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY} ${FMOD_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES})


# Store include dir into variable and share it with other projects through cache
set(${EXECUTABLE_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} ${FMOD_INCLUDE_DIR}
CACHE INTERNAL "${EXECUTABLE_NAME}: Include Directories" FORCE)
