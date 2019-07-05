###################################################################################
#                                                                                 #
# NAME: CMakeLsits.txt                                                            #
#                                                                                 #
# AUTHOR: Mike Karlesky, Mark VanderVoord, Greg Williams.                         #
# WRITTEN BY: Michael Brockus.                                                    #
#                                                                                 #
# License: MIT                                                                    #
#                                                                                 #
###################################################################################
cmake_minimum_required(VERSION 3.13.2.0 FATAL_ERROR)



#
#  CMake: Declare project
#
project(unity LANGUAGES C DESCRIPTION "C Unit testing framework.")



#
#  CMake: Creation of library   
#
add_library("unity" STATIC)



#
#  CMake: Adding source to target
#
target_sources("unity" PRIVATE "src/unity.c")



#
#  CMake: Including directories to target
#
target_include_directories("unity" 
    PUBLIC 
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
        "$<INSTALL_INTERFACE:src>"

    PRIVATE "src"
)



#
#  CMake: Give target an alias 
#
add_library("unity::framework" ALIAS "unity")



#
#  CMake: export project
#
install(TARGETS "unity" EXPORT "unityConfig"
    ARCHIVE  DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_INSTALL_LIBDIR}"
    LIBRARY  DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_INSTALL_LIBDIR}"
    RUNTIME  DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_LIBDIR}")

install(DIRECTORY src/ DESTINATION src)
install(EXPORT unityConfig DESTINATION share/unityConfig/cmake)

# This makes the project importable from the build directory
export(TARGETS unity FILE unityConfig.cmake)
