#
# build script written by : Michael Brockus.
# github repo author: Mike Karlesky, Mark VanderVoord, Greg Williams.
#
# license: MIT
#
project('unity', 'c',
    license: 'MIT',
    meson_version: '>=0.53.0',
    default_options: ['werror=true', 'c_std=c11']
)

subdir('src')
unity_dep = declare_dependency(link_with: unity_lib, include_directories: unity_dir)
