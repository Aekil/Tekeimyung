cmake_minimum_required(VERSION 3.0)
project("Tekeimyung")

# This function ensures to generate a "Visual Studio like" tree structure for source and header files
# - This function takes a list as an unique argument
# - It then loops through each item (meaning a file) of the list and checks for either a relative or an absolute path
# - Once the relative path has been set, it replaces the basic slash (/) with Visual Studio compliant backslash (\)
# - Finally, the file with its relative path is added to the tree structure through the source_group call
function(source_group_files)
    foreach(filepath IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${filepath}")
            file(RELATIVE_PATH filepath_relative "${CMAKE_CURRENT_SOURCE_DIR}" "${filepath}")
        else()
            set(filepath_relative "${filepath}")
        endif()
		
        get_filename_component(file_path "${filepath_relative}" PATH)
        string(REPLACE "/" "\\" file_path_msvc "${file_path}")
        source_group("${file_path_msvc}" FILES "${filepath}")
    endforeach()
endfunction(source_group_files)

# Set cmake modules directory
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(ECS)
add_subdirectory(Engine)
add_subdirectory(Game)