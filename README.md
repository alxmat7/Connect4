# Connect Four AI

Connect Four is a two-player abstract game with simple rules (https://en.wikipedia.org/wiki/Connect_Four). The game has a low branching factor, making it an ideal zero-sum strategy game for testing game AIs.  
This is an implementation of Connect Four with two AIs - one using Monte Carlo tree search and the other using Minmax. 

![connect4 on linux](https://user-images.githubusercontent.com/8562118/185773849-c287cb2c-6d57-4444-acfe-34e1219d5fef.png)

## Prerequisites ##

SFML - https://www.sfml-dev.org/  
CMake - https://cmake.org/

## Build Steps ##

In top level CMakeLists.txt, set SFML_DIR to your SFML installation, if running on Windows. 

### *Linux* ###

Run 
```
cmake -B <build_dir>
```
to generate the makefile.

```
cd <build_dir>
make
```
### *Windows - Visual Studio* ###

Navigate to the top-level directory and run

```
cmake -G "Visual Studio 16 2019" -A x64 -B <build_directory>
```
This writes the build files to '*build_directory*'. Note that this command is for VS2019. For older versions, this is slightly different.
To generate the executable, either build with the generated visual studio solution, or build with cmake as in the second step in ‘Windows - MinGW’.

### *Windows - MinGW* ###

```
cmake -G "MinGW Makefiles"  -B <build_directory>
cmake --build <build_directory> --config Release
```

