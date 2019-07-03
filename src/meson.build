###################################################################################
#                                                                                 #
# NAME: src/meson.build                                                           #
#                                                                                 #
# AUTHOR: Mike Karlesky, Mark VanderVoord, Greg Williams.                         #
# WRITTEN BY: Michael Brockus.                                                    #
#                                                                                 #
# License: MIT                                                                    #
#                                                                                 #
###################################################################################



unity_src = files('unity.c')

unity_dir = include_directories('.')

unity_lib = library(meson.project_name(), 
    sources: unity_src,
    include_directories: unity_dir)