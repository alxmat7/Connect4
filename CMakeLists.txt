cmake_minimum_required(VERSION 3.6)

project(Connect4)

#On linux, if SFML is installed in the default location, this works out of the box.
IF (WIN32)
    # If you want to link SFML statically
    set(SFML_STATIC_LIBRARIES TRUE)
    #set SFML_DIR to your SFML installation 
    #eg:- set(SFML_DIR "D:\\SFML\\SFML-2.5.1-windows-vc15-64-bit\\SFML-2.5.1\\lib\\cmake\\SFML")
    #set(SFML_DIR <sfml_dir>)
    find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
    #set startup project to Connect4 if using Visual Studio.
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Connect4)
ENDIF()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(src)
