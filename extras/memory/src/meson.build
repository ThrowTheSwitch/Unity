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
unity_memory_dir = include_directories('.')

unity_memory_lib = static_library('unity_memory', 
    sources: ['unity_memory.c'],
    dependencies: unity_dep,
    include_directories: unity_memory_dir)
