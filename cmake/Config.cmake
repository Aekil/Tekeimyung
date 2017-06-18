# detect os
IF (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(OS_WINDOWS 1)
ELSEIF (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(OS_LINUX 1)
ELSE()
    message(FATAL_ERROR "Unsupported operating system or environment")
    return()
ENDIF()

# define the architecture
IF (CMAKE_CL_64)
  set(ARCH_DIR "x64")
ELSE()
  set(ARCH_DIR "x86")
ENDIF()
