cmake_minimum_required(VERSION 3.0)
project("Tekeimyung")


# Ensure windows export libraries symbols when building
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_subdirectory(ECS)
add_subdirectory(Engine)
