# Tekeimyung
<br />


## Building with CMake on Windows

### Downloading Dependencies
* [GLFW] (http://www.glfw.org/download.html)
* [GLEW] (http://glew.sourceforge.net/)
* [FMOD] (http://www.fmod.org/download/ - make sure to download the Low Level Programmer API)

### Libraries environment variables
Variable | Type
---------|-----
GLFW_INCLUDE_DIR | PATH
GLFW_LIBRARY | FILE_PATH
GLEW_LOCATION | PATH
FMOD_INCLUDE_DIR | PATH (something like : FMOD Studio API Windows\api\lowlevel\inc)
FMOD_LIBRARY | FILE_PATH (something like : FMOD Studio API Windows\api\lowlevel\lib\fmod?_vc.lib)


<br />
## Building with CMake on Linux

### Downloading Dependencies
Simply use your package manager to install these libraries:
* GLFW
* GLEW
* FMOD