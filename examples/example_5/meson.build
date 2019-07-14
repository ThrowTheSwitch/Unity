###################################################################################
#                                                                                 #
# NAME: examples/example_5/meson.build                                            #
#                                                                                 #
# AUTHOR: Mike Karlesky, Mark VanderVoord, Greg Williams.                         #
# WRITTEN BY: Michael Brockus.                                                    #
#                                                                                 #
# License: MIT                                                                    #
#                                                                                 #
###################################################################################


project('example-5', 'cpp', 
        default_options: [
            'default_library=static',
            'cpp_std=c++17'
        ])

unity_dep = dependency('unity', fallback : ['unity', 'unity_dep'])

example_dir = include_directories('.', join_paths('.', 'src'))

subdir('src')
subdir('test')