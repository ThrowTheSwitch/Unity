###################################################################################
#                                                                                 #
# NAME: meson.build                                                               #
#                                                                                 #
# AUTHOR: Mike Karlesky, Mark VanderVoord, Greg Williams.                         #
# WRITTEN BY: Michael Brockus.                                                    #
#                                                                                 #
# License: MIT                                                                    #
#                                                                                 #
###################################################################################
unity_fixture_dir = include_directories('.')

unity_fixture_lib = static_library('unity_fixture', 
    sources: ['unity_fixture.c'],
    link_with: unity_memory_lib,
    dependencies: unity_dep,
    include_directories: [unity_fixture_dir, unity_memory_dir])
