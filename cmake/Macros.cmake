# detect os
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(ENGINE_OS_WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(ENGINE_OS_LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(ENGINE_OS_ANDROID 1)
else()
    message(FATAL_ERROR "Unsupported operating system or environment")
    return()
endif()

# detect compiler
if(CMAKE_CXX_COMPILER MATCHES ".*clang[+][+]" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang++")
    set(ENGINE_COMPILER_CLANG 1)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(ENGINE_COMPILER_GCC 1)
elseif(MSVC)
    set(ENGINE_COMPILER_MSVC 1)
else()
    message(FATAL_ERROR "Unsupported compiler")
    return()
endif()

# define the install directory for miscellaneous files
if(ENGINE_OS_WINDOWS)
    set(INSTALL_MISC_DIR .)
elseif(ENGINE_OS_LINUX)
    set(INSTALL_MISC_DIR share/lug)
endif()

if (CMAKE_CL_64)
  set(ARCH_DIR "x64")
else()
  set(ARCH_DIR "x86")
endif()


# This function ensures to generate a "Visual Studio like" tree structure for source and header files
# - This function takes a list as an unique argument
# - It then loops through each item (meaning a file) of the list and checks for either a relative or an absolute path
# - Once the relative path has been set, it replaces the basic slash (/) with Visual Studio compliant backslash (\)
# - Finally, the file with its relative path is added to the tree structure through the source_group call
function(source_group_files)
    message(STATUS "Grouping source files in the tree structure... (from ${CMAKE_CURRENT_SOURCE_DIR})")
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

function(install_directories dirs_path install_path)
    file(GLOB DEPLOY_FILES_AND_DIRS ${dirs_path}/*)
    foreach(ITEM ${DEPLOY_FILES_AND_DIRS})
        if (IS_DIRECTORY "${ITEM}")
            install(DIRECTORY ${ITEM}
                    DESTINATION ${install_path})
        endif()
    endforeach()
endfunction()
