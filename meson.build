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
project('unity', 'c',
    license: 'MIT',
    meson_version: '>=0.50.0',
    default_options: ['layout=flat', 'warning_level=3', 'werror=true', 'c_std=c11']
)

#
# Sub directory to project source code
subdir('src')
unity_dep = declare_dependency(link_with: unity_lib, include_directories: unity_dir)

#
# Exposing extra add-ons from the extras directory
subdir('extras')
unity_fixture_dep = declare_dependency(link_with: unity_fixture_lib, include_directories: unity_fixture_dir)
unity_memory_dep  = declare_dependency(link_with: unity_memory_lib,  include_directories: unity_memory_dir)
