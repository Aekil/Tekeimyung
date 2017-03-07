# Tekeimyung
<br />


## 1) Building the Engine

No requirements needed, just run cmake.

After you compile the engine, run the "INSTALL" project.
By default, it will install the engine on your C: drive on Windows, so you'll need to have admins rights. To change that, modify the CMAKE_INSTALL_PREFIX variable in your cmake to the path you want the engine to be installed.


## 2) Building the Game

Add SQUIRREL_ENGINE_ROOT variable in your cmake to the path where the engine is installed.
Then, just run cmake, all the needed dlls are also copied.

Note that the engine has different libraries for debug and release, so you'll have to build the engine in both debug and release if you want to compile the Game in debug and release.
