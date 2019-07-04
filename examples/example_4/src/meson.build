###################################################################################
#                                                                                 #
# NAME: examples/example_4/src/meson.build                                        #
#                                                                                 #
# AUTHOR: Mike Karlesky, Mark VanderVoord, Greg Williams.                         #
# WRITTEN BY: Michael Brockus.                                                    #
#                                                                                 #
# License: MIT                                                                    #
#                                                                                 #
###################################################################################



a_lib = library(
    'production-code-1', 
    'ProductionCode.c',
    include_directories: example_dir)

b_lib = library(
    'production-code-2',
    'ProductionCode2.c',
    include_directories: example_dir)


a_dep = declare_dependency(
    link_with: a_lib,
    include_directories: example_dir)

b_dep = declare_dependency(
    link_with: b_lib,
    include_directories: example_dir)