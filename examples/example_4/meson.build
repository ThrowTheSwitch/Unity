###################################################################################
#                                                                                 #
# NAME: examples/example_4/meson.build                                            #
#                                                                                 #
# AUTHOR: Mike Karlesky, Mark VanderVoord, Greg Williams.                         #
# WRITTEN BY: Michael Brockus.                                                    #
#                                                                                 #
# License: MIT                                                                    #
#                                                                                 #
###################################################################################


project('example-4', 'c')

unity_dep = dependency('unity', fallback : ['unity', 'unity_dep'])

example_dir = include_directories('.', join_paths('.', 'src'))

subdir('src')
subdir('test')