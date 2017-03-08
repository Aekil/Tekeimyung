#   @Author   Guillaume Labey
#   @Author   Mathieu Chassara

cmake_minimum_required(VERSION 3.0)

project("Squirrel Engine")


# include macros
include(${PROJECT_SOURCE_DIR}/../cmake/Macros.cmake)

# define the path of CMake modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake/modules/")

# set the output directory for library
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")


# Add parallel build
if(ENGINE_COMPILER_MSVC)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Define RELEASE_WITH_DEBUG for RelWithDebInfo build
set(DEBUG_OPTIONS /D RELEASE_WITH_DEBUG)

add_compile_options(
    "$<$<CONFIG:RelWithDebInfo>:${DEBUG_OPTIONS}>"
)

set(LIBRARY_NAME "squirrel_engine")

# All source files using glob
file(GLOB_RECURSE source_files ${CMAKE_SOURCE_DIR}/src/*)
file(GLOB_RECURSE include_files ${CMAKE_SOURCE_DIR}/include/*)
file(GLOB_RECURSE imgui_source_files ${CMAKE_SOURCE_DIR}/../external/imgui/*.cpp*)
file(GLOB_RECURSE imGuizmo_source_files ${CMAKE_SOURCE_DIR}/../external/imGuizmo/*.cpp*)
file(GLOB_RECURSE jsoncpp_source_files ${CMAKE_SOURCE_DIR}/../external/jsoncpp/*.cpp)

# Group files into the Visual Studio tree structure
source_group_files(${source_files})
source_group_files(${include_files})
source_group_files(${jsoncpp_source_files})
source_group_files(${imgui_source_files})
source_group_files(${imGuizmo_source_files})

# Use static lib of glew
add_definitions(-DGLEW_STATIC)

# Find ECS
find_package(ECS QUIET)
if (NOT ECS_FOUND)
    if (ENGINE_OS_WINDOWS)
        set(ECS_ROOT "${CMAKE_SOURCE_DIR}/../external/ecs/lib/windows/${ARCH_DIR}")
    elseif (ENGINE_OS_LINUX)
        set(ECS_ROOT "${CMAKE_SOURCE_DIR}/../external/ecs/lib/linux")
    endif()

    set(ECS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../external/ecs/include")
    find_package(ECS REQUIRED)
endif()

# Find GLEW library
find_package(GLEW 3.0.0 QUIET)
if (NOT GLEW_FOUND)
    if (ENGINE_OS_WINDOWS)
        set(GLEW_LOCATION "${CMAKE_SOURCE_DIR}/../external/glew/lib/windows/${ARCH_DIR}")
    elseif (ENGINE_OS_LINUX)
        set(GLEW_LOCATION "${CMAKE_SOURCE_DIR}/../external/glew")
    endif()

    set(GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../external/glew/include")
    find_package(GLEW 3.0.0 REQUIRED)
endif()

# Find GLFW library
find_package(GLFW 3.0.0 QUIET)
if (NOT GLFW_FOUND)
    if (ENGINE_OS_WINDOWS)
        set(GLFW_LOCATION "${CMAKE_SOURCE_DIR}/../external/glfw/lib/windows/${ARCH_DIR}")
    elseif (ENGINE_OS_LINUX)
        set(GLFW_LOCATION "${CMAKE_SOURCE_DIR}/../external/glfw")
    endif()

    set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../external/glfw/include")
    find_package(GLFW 3.0.0 REQUIRED)
endif()

# Find Opengl library
find_package(OpenGL REQUIRED)

# Find assimp
find_package(Assimp QUIET)
if (NOT ASSIMP_FOUND)
    if (ENGINE_OS_WINDOWS)
        set(ASSIMP_ROOT "${CMAKE_SOURCE_DIR}/../external/assimp/lib/windows/${ARCH_DIR}")
        file(GLOB_RECURSE ASSIMP_DLLS "${CMAKE_SOURCE_DIR}/../external/assimp/bin/windows/${ARCH_DIR}/*")
    elseif (ENGINE_OS_LINUX)
        set(ASSIMP_ROOT "${CMAKE_SOURCE_DIR}/../external/assimp/lib/linux")
    endif()

    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../external/assimp/include")
    find_package(Assimp REQUIRED)
endif()

# Find fmod
find_package(FMOD QUIET)
if (NOT FMOD_FOUND)
    if (ENGINE_OS_WINDOWS)
        set(FMOD_ROOT "${CMAKE_SOURCE_DIR}/../external/fmod/lib/windows/${ARCH_DIR}")
        file(GLOB_RECURSE FMOD_DLLS "${CMAKE_SOURCE_DIR}/../external/fmod/bin/windows/${ARCH_DIR}/*")
    elseif (ENGINE_OS_LINUX)
        set(FMOD_ROOT "${CMAKE_SOURCE_DIR}/../external/fmod/lib/linux")
    endif()

    set(FMOD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../external/fmod/include")
    find_package(FMOD REQUIRED)
endif()



# Add project and libraries includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${ECS_INCLUDE_DIR})

# Add external includes
include_directories(${CMAKE_SOURCE_DIR}/../external/glm/)
include_directories(${CMAKE_SOURCE_DIR}/../external/stb_image/)
include_directories(${CMAKE_SOURCE_DIR}/../external/imgui/)
include_directories(${CMAKE_SOURCE_DIR}/../external/imGuizmo/)
include_directories(${CMAKE_SOURCE_DIR}/../external/jsoncpp)
include_directories(${FMOD_INCLUDE_DIR})
include_directories(${ASSIMP_INCLUDE_DIR})

# Include dirent only for windows
# Linux have his own header
IF (ENGINE_OS_WINDOWS)
    include_directories(${CMAKE_SOURCE_DIR}/../external/dirent)
ENDIF()

# Create library
add_library(${LIBRARY_NAME} STATIC ${source_files} ${include_files} ${imgui_source_files} ${imGuizmo_source_files} ${jsoncpp_source_files})

# Add debug postfix
set_target_properties(${LIBRARY_NAME} PROPERTIES DEBUG_POSTFIX -d)

#Disable ImGui C4244 warnings on Windows
IF (WIN32)
  set_property(SOURCE ${imgui_source_files} APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4244")
ENDIF()

# Link libraries with engine library
target_link_libraries(${LIBRARY_NAME} ${ECS_LIBRARY} ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY} ${FMOD_LIBRARY} ${ASSIMP_LIBRARY})


# setup the install of the library
install(TARGETS ${LIBRARY_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/Engine
        DESTINATION include)

# Install include dependencies
install(DIRECTORY ${CMAKE_SOURCE_DIR}/../external/glm
        DESTINATION dependencies/include)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/../external/stb_image
        DESTINATION dependencies/include)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/../external/imgui
        DESTINATION dependencies/include)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/../external/imGuizmo
        DESTINATION dependencies/include)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/../external/jsoncpp
        DESTINATION dependencies/include)
install(DIRECTORY ${GLEW_INCLUDE_DIR}/GL
        DESTINATION dependencies/include/glew)
install(DIRECTORY ${GLFW_INCLUDE_DIR}/GLFW
        DESTINATION dependencies/include/glfw)
install(DIRECTORY ${ECS_INCLUDE_DIR}/ECS
        DESTINATION dependencies/include/ecs)
IF (ENGINE_OS_WINDOWS)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/../external/dirent
        DESTINATION dependencies/include)
ENDIF()

# Install libraries dependencies
install(FILES ${GLFW_LIBRARY}
        DESTINATION dependencies/lib/glfw)
install(FILES ${GLEW_LIBRARY}
        DESTINATION dependencies/lib/glew)
install(FILES ${FMOD_LIBRARY_DEBUG}
                ${FMOD_LIBRARY_RELEASE}
        DESTINATION dependencies/lib/fmod)
install(FILES ${ASSIMP_LIBRARY_DEBUG}
                ${ASSIMP_LIBRARY_RELEASE}
        DESTINATION dependencies/lib/assimp)
install(FILES ${ECS_LIBRARY_DEBUG}
                ${ECS_LIBRARY_RELEASE}
        DESTINATION dependencies/lib/ecs)

# Install dll dependencies
install(FILES ${FMOD_DLLS}
        DESTINATION dependencies/bin/fmod)
install(FILES ${ASSIMP_DLLS}
        DESTINATION dependencies/bin/assimp)

# Install resources
install_directories(${CMAKE_SOURCE_DIR}/resources resources)
