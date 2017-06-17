# Find Fmod
#
# Below are the output variables:
#  - FMOD_INCLUDE_DIR
#  - FMOD_LIBRARY
#  - FMOD_FOUND

set(FIND_PATHS
    ${FMOD_ROOT}
    $ENV{FMOD_ROOT}
    /usr/local
    /usr
    /opt/local
    /opt
)

find_path(FMOD_INCLUDE_DIR
    NAMES include/fmod.h
    PATHS ${FIND_PATHS}
    CMAKE_FIND_ROOT_PATH_BOTH
)

find_library(FMOD_LIBRARY_RELEASE
    NAMES fmod
    PATHS ${FIND_PATHS}
    PATH_SUFFIXES lib
    CMAKE_FIND_ROOT_PATH_BOTH
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FMOD DEFAULT_MSG FMOD_LIBRARY_RELEASE FMOD_INCLUDE_DIR)

if(FMOD_FOUND)
    set(FMOD_LIBRARY debug ${FMOD_LIBRARY_RELEASE}
                       optimized ${FMOD_LIBRARY_RELEASE}
    )
endif()

mark_as_advanced(FMOD_INCLUDE_DIR FMOD_LIBRARY)
