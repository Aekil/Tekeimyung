cmake_minimum_required(VERSION 3.0)
project("Tekeimyung")


# Set cmake modules directory
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(ECS)
add_subdirectory(Engine)