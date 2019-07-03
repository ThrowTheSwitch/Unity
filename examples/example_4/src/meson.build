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



example_src = files('ProductionCode.c', 'ProductionCode2.c')

example_lib = library(meson.project_name(), 
    sources: example_src,
    include_directories: example_dir)

example_dep = declare_dependency(
    version: meson.project_version(),
    link_with: example_lib,
    include_directories: example_dir)