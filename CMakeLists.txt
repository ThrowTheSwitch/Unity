###################################################################################
#                                                                                 #
# NAME: CMakeLists.txt                                                            #
#                                                                                 #
# AUTHOR: Mike Karlesky, Mark VanderVoord, Greg Williams.                         #
# WRITTEN BY: Michael Brockus.                                                    #
#                                                                                 #
# License: MIT                                                                    #
#                                                                                 #
###################################################################################
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)


project(unity LANGUAGES C DESCRIPTION "C Unit testing framework.")

add_subdirectory(src)
target_include_directories(unity
    PUBLIC 
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
        "$<INSTALL_INTERFACE:src>"

    PRIVATE "src"
)

add_library(unity::framework ALIAS unity)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/ DESTINATION src)
install(EXPORT unityConfig DESTINATION share/unityConfig/cmake)

# This makes the project importable from the build directory
export(TARGETS unity FILE unityConfig.cmake)
