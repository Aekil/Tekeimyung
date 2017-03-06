#    @Author   Guillaume Labey
#    @Author   Mathieu Chassara

message(STATUS "Current CMake version is: ${CMAKE_VERSION}")

cmake_minimum_required(VERSION 3.0)
project("Tekeimyung")

# Set cmake modules directory
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Add parallel build
if(MSVC)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Define RELEASE_WITH_DEBUG for RelWithDebInfo build
set(DEBUG_OPTIONS /D RELEASE_WITH_DEBUG)

add_compile_options(
    "$<$<CONFIG:RelWithDebInfo>:${DEBUG_OPTIONS}>"
)

add_subdirectory(ECS)
add_subdirectory(Engine)
#add_subdirectory(Game)
