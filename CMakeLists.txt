cmake_minimum_required(VERSION 2.6)
project(unity)

include_directories(extras/fixture/src src/)
add_library(unity src/unity.c extras/fixture/src/unity_fixture.c)
install(TARGETS unity DESTINATION lib)
install(FILES   src/unity.h 
                src/unity_internals.h 
                extras/fixture/src/unity_fixture.h
                extras/fixture/src/unity_fixture_internals.h
                extras/fixture/src/unity_fixture_malloc_overrides.h
        DESTINATION "include/unity"
                )


