cmake_minimum_required(VERSION 3.6)

project(Connect4)

## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "D:\\SFML\\SFML-2.5.1-windows-vc15-64-bit\\SFML-2.5.1\\lib\\cmake\\SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(src)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Connect4)
	
