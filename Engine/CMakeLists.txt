#	@Author   Guillaume Labey
#	@Author   Mathieu Chassara

cmake_minimum_required(VERSION 3.0)
set(EXECUTABLE_NAME "ENGINE")
project(${EXECUTABLE_NAME})

# include the config file
include(${CMAKE_SOURCE_DIR}/cmake/Config.cmake)

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

#Set glfw directory and library on Windows
IF (OS_WINDOWS)
  set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glfw/include)
  find_library(GLFW_LIBRARY glfw3 PATHS ${CMAKE_SOURCE_DIR}/external/glfw/lib/${ARCH_DIR})
  set(GLFW_FOUND true)
# Find GLFW on linux
ELSE()
  find_package(GLFW 3.0.0 REQUIRED)
ENDIF()

IF (NOT GLFW_FOUND)
  message(FATAL_ERROR "Can't find glfw library")
ENDIF()

# Use static lib of glew
add_definitions(-DGLEW_STATIC)

#Set glew directory and library on Windows
IF (OS_WINDOWS)
  set(GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glew/include)
  find_library(GLEW_LIBRARY glew32s PATHS ${CMAKE_SOURCE_DIR}/external/glew/lib/${ARCH_DIR})
  set(GLEW_FOUND true)
# Find GLEW on linux
ELSE()
  find_package(GLEW REQUIRED)
ENDIF()

IF (NOT GLEW_FOUND)
  message(FATAL_ERROR "Can't find glew library")
ENDIF()

# Find Opengl library
find_package(OpenGL REQUIRED)

#Set Assimp directory and library on Windows
IF (OS_WINDOWS)
  set(ASSIMP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/Assimp/include)
  find_library(ASSIMP_LIBRARY assimp PATHS ${CMAKE_SOURCE_DIR}/external/Assimp/lib/${ARCH_DIR})
  set(ASSIMP_FOUND true)
#Find assimp on linux
ELSE()
  find_package(Assimp REQUIRED)
ENDIF()

IF (NOT ASSIMP_FOUND)
  message(FATAL_ERROR "Can't find assimp library")
ENDIF()

#Set Freetype directory and library on Windows
IF (OS_WINDOWS)
  set(FREETYPE_INCLUDE_DIR_ft2build ${CMAKE_SOURCE_DIR}/external/freetype/include/freetype2)
  set(FREETYPE_LIBRARIES ${CMAKE_SOURCE_DIR}/external/freetype/lib/freetype.lib)
  set(FREETYPE_FOUND true)
#Find freetype on linux
ELSE()
    find_package(Freetype REQUIRED)
ENDIF()

IF (NOT FREETYPE_FOUND)
  message(FATAL_ERROR "Can't find freetype library")
ENDIF()

# Find FMOD library
find_package(FMOD REQUIRED)

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
IF (OS_WINDOWS)
    include_directories(${CMAKE_SOURCE_DIR}/external/dirent)
ENDIF()

# Create executable
add_library(${EXECUTABLE_NAME} STATIC ${source_files} ${imgui_source_files} ${include_files} ${imGuizmo_source_files} ${jsoncpp_source_files})

#Disable ImGui C4244 warnings on Windows
IF (OS_WINDOWS)
  set_property(SOURCE ${imgui_source_files} APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4244")
ENDIF()

# Link libraries with executable
target_link_libraries(${EXECUTABLE_NAME} ECS ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY} ${FMOD_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES})


# Store include dir into variable and share it with other projects through cache
set(${EXECUTABLE_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} ${FMOD_INCLUDE_DIR}
CACHE INTERNAL "${EXECUTABLE_NAME}: Include Directories" FORCE)
