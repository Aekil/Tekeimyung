# Tekeimyung


The Tekeimyung project is a game engine developed during our year of study in South Korea, in Keimyung University in the city of Daegu.

This game engine is based on the ECS (entity component system) architecture, and has a script system close to the Unity script system, which allows us to  develop our game logic.

For now, the game engine and the game are on the same repository, but we would like to put the game on another respository.

Notes: The game editor is available when using Debug or RelWithDebug builds.

![Game editor](https://raw.githubusercontent.com/chardoj/Tekeimyung/dev/images/ntd_editor.PNG)
Game editor

Here are some of the game engine features:
- ECS (entity component system) architecture
- OpenGL as graphics backend
- Particle system: cone emission, can emit everything (planes, 3D models and geometries)
- Debug tools: modify entities, materials and animations, view logs, systems monitoring, entity inspector
- Editor: create, run and save levels, camera debug/preview, move camera, lights and entities
- In-game engine animations: color, rotation, scale, translation
- Unity-like script system for the game logic
- UI component to have percentage-size UI and horizontal/vetical alignements
- And a lot more!

<br />

# The NTD project

The NTD project (new tower defense project) is a game using the Tekeimyung game engine.
It's a 3D Tower Defense game in isometric perspective that takes place in a futuristic environment. The player personifies an avatar summoned by an autonomous entity that will dedicate his life to the defense of this one.
The player can build everywhere, but enemies will destroy some walls if they can't find a path to the castle (A* algorithm).

![Game](https://raw.githubusercontent.com/chardoj/Tekeimyung/dev/images/ntd_game.PNG)
Game

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
