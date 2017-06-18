# Tekeimyung
<br />


## Building with CMake on Windows

### Downloading Dependencies
All the libraries are included in the respository and cmake will automatically link them, except for FMOD.

Download fmod: http://www.fmod.org/download/ - make sure to download the Low Level Programmer API

### FMOD environment variables
You need to add fmod environment variables :

Variable | Type (if you are using cmake-gui) | Example
---------|-----|-----
FMOD_INCLUDE_DIR | PATH | FMOD Studio API Windows/api/lowlevel/inc
FMOD_LIBRARY | FILE_PATH | FMOD Studio API Windows/api/lowlevel/lib/fmod?_vc.lib


## Building with CMake on Linux

### Downloading Dependencies

Download fmod: http://www.fmod.org/download/ - make sure to download the Low Level Programmer API

For other libraries, simply use your package manager:
* glfw
* glew
* freetype
* assimp
* glm

Example packages on Ubuntu:
* libglfw-dev
* libglew-dev
* libfreetype6-dev
* libassimp-dev
* libglm-dev
