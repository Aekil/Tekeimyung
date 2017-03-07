# Find FMOD
#
# Below are the output variables:
#  - ECS_INCLUDE_DIR
#  - ECS_LIBRARY
#  - ECS_FOUND

find_path(ECS_INCLUDE_DIR
    NAMES ECS/World.hpp
    PATHS $ENV{ECS_ROOT}/include ${ECS_ROOT}/include
)

find_library(ECS_LIBRARY_RELEASE
    NAMES ecs
    PATHS $ENV{ECS_ROOT}
            ${ECS_ROOT}/lib
            ${ECS_ROOT}
            $ENV{ECS_ROOT}/lib
    PATH_SUFFIXES lib
)

find_library(ECS_LIBRARY_DEBUG
    NAMES ecs-d
    PATHS $ENV{ECS_ROOT}
            ${ECS_ROOT}/lib
            ${ECS_ROOT}
            $ENV{ECS_ROOT}/lib
    PATH_SUFFIXES lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ECS DEFAULT_MSG ECS_LIBRARY_RELEASE ECS_INCLUDE_DIR)

if(ECS_FOUND)
    set(ECS_LIBRARY debug ${ECS_LIBRARY_DEBUG}
                       optimized ${ECS_LIBRARY_RELEASE}
    )
endif()

mark_as_advanced(ECS_INCLUDE_DIR ECS_LIBRARY)
