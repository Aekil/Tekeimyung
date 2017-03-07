# Find FMOD
#
# Below are the output variables:
#  - SQUIRREL_ENGINE_INCLUDE_DIR
#  - SQUIRREL_ENGINE_LIBRARY
#  - SQUIRREL_ENGINE_FOUND


find_path(SQUIRREL_ENGINE_INCLUDE_DIR
    NAMES Engine/Core/Engine.hpp
    PATHS $ENV{SQUIRREL_ENGINE_ROOT}/include ${SQUIRREL_ENGINE_ROOT}/include
)

find_library(SQUIRREL_ENGINE_LIBRARY_RELEASE
    NAMES squirrel_engine
    PATHS $ENV{SQUIRREL_ENGINE_ROOT}/lib ${SQUIRREL_ENGINE_ROOT}/lib
    PATH_SUFFIXES lib
)

find_library(SQUIRREL_ENGINE_LIBRARY_DEBUG
    NAMES squirrel_engine-d
    PATHS $ENV{SQUIRREL_ENGINE_ROOT}/lib ${SQUIRREL_ENGINE_ROOT}/lib
    PATH_SUFFIXES lib
)


# Use static lib of glew
add_definitions(-DGLEW_STATIC)

# Find GLEW library
set(GLEW_LOCATION "${SQUIRREL_ENGINE_ROOT}/dependencies/lib/glew")
set(GLEW_INCLUDE_DIR "${SQUIRREL_ENGINE_ROOT}/dependencies/include/glew")
find_package(GLEW 3.0.0 REQUIRED)

# Find GLFW library
set(GLFW_LOCATION "${SQUIRREL_ENGINE_ROOT}/dependencies/lib/glfw")
set(GLFW_INCLUDE_DIR "${SQUIRREL_ENGINE_ROOT}/dependencies/include/glfw")
find_package(GLFW 3.0.0 REQUIRED)

# Find ECS library
set(ECS_ROOT "${SQUIRREL_ENGINE_ROOT}/dependencies/lib/ecs")
set(ECS_INCLUDE_DIR "${SQUIRREL_ENGINE_ROOT}/dependencies/include/ecs")
find_package(ECS REQUIRED)

# Find FMOD library
set(FMOD_ROOT "${SQUIRREL_ENGINE_ROOT}/dependencies/lib/fmod")
set(FMOD_INCLUDE_DIR "${SQUIRREL_ENGINE_ROOT}/dependencies/include/fmod")
find_package(FMOD REQUIRED)

# Find ASSIMP library
set(ASSIMP_ROOT "${SQUIRREL_ENGINE_ROOT}/dependencies/lib/assimp")
set(ASSIMP_INCLUDE_DIR "${SQUIRREL_ENGINE_ROOT}/dependencies/include/assimp")
find_package(ASSIMP REQUIRED)

# Find Opengl library
find_package(OpenGL REQUIRED)

# Append include dependencies
file(GLOB dependencies_dirs ${SQUIRREL_ENGINE_ROOT}/dependencies/include/*)
set(SQUIRREL_ENGINE_INCLUDE_DIR ${SQUIRREL_ENGINE_INCLUDE_DIR} ${dependencies_dirs})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SQUIRREL_ENGINE DEFAULT_MSG SQUIRREL_ENGINE_LIBRARY_RELEASE SQUIRREL_ENGINE_INCLUDE_DIR)


# Append release and debug libraries to SQUIRREL_ENGINE_LIBRARY
if (SQUIRREL_ENGINE_LIBRARY_DEBUG OR SQUIRREL_ENGINE_LIBRARY_RELEASE)
    if (SQUIRREL_ENGINE_LIBRARY_DEBUG)
        set(SQUIRREL_ENGINE_LIBRARY ${SQUIRREL_ENGINE_LIBRARY}
            debug ${SQUIRREL_ENGINE_LIBRARY_DEBUG}
        )
    endif()

    if (SQUIRREL_ENGINE_LIBRARY_RELEASE)
        set(SQUIRREL_ENGINE_LIBRARY ${SQUIRREL_ENGINE_LIBRARY}
            debug ${SQUIRREL_ENGINE_LIBRARY_RELEASE}
        )
    endif()
else()
    message(FATAL_ERROR "Can't find Squirrel Engine")
endif()


set(SQUIRREL_ENGINE_LIBRARY ${SQUIRREL_ENGINE_LIBRARY}
                            ${GLFW_LIBRARY}
                            ${GLEW_LIBRARY}
                            ${ECS_LIBRARY}
                            ${FMOD_LIBRARY}
                            ${ASSIMP_LIBRARY}
                            ${OPENGL_gl_LIBRARY}
)

mark_as_advanced(SQUIRREL_ENGINE_INCLUDE_DIR SQUIRREL_ENGINE_LIBRARY)
