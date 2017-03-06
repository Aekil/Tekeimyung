# Find FMOD
#
# Below are the output variables:
#  - FMOD_INCLUDE_DIR
#  - FMOD_LIBRARY
#  - FMOD_FOUND

find_path(FMOD_INCLUDE_DIR
    NAMES fmod.h
    PATHS $ENV{FMOD_ROOT}/include ${FMOD_ROOT}/include
    ${FMOD_ROOT}/api/lowlevel/inc
)

set(FMOD_RELEASE_LIBRARIES fmod_vc fmod64_vc libfmod)
set(FMOD_DEBUG_LIBRARIES fmodL_vc fmod64L_vc libfmodL)

find_library(FMOD_LIBRARY_RELEASE
    NAMES ${FMOD_RELEASE_LIBRARIES}
    PATHS $ENV{FMOD_ROOT} ${FMOD_ROOT}
    PATH_SUFFIXES lib
)

find_library(FMOD_LIBRARY_DEBUG
    NAMES ${FMOD_RELEASE_LIBRARIES}
    PATHS $ENV{FMOD_ROOT} ${FMOD_ROOT}
    PATH_SUFFIXES lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FMOD DEFAULT_MSG FMOD_LIBRARY_RELEASE FMOD_INCLUDE_DIR)

if(FMOD_FOUND)
    set(FMOD_LIBRARY debug ${FMOD_LIBRARY_DEBUG}
                       optimized ${FMOD_LIBRARY_RELEASE}
    )
endif()

mark_as_advanced(FMOD_INCLUDE_DIR FMOD_LIBRARY)
