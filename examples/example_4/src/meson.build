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

inc_dir = include_directories('.')
lib_list = {'a': ['ProductionCode.c' ], 'b': ['ProductionCode2.c']}

foreach lib, src : lib_list
    set_variable(lib + '_lib', 
        static_library(lib + '_lib', sources: src, include_directories: inc_dir))
endforeach

a_dep = declare_dependency(link_with: a_lib, include_directories: inc_dir)
b_dep = declare_dependency(link_with: b_lib, include_directories: inc_dir)
