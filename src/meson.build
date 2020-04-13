#
# build script written by : Michael Brockus.
# github repo author: Mike Karlesky, Mark VanderVoord, Greg Williams.
#
# license: MIT
#
conf_data = configuration_data()
conf_data.set('PROJECT_VERSION', meson.project_version())
conf_data.set('PROJECT_VERSION_MAJOR',meson.project_version().split('.')[0])
conf_data.set('PROJECT_VERSION_MINOR',meson.project_version().split('.')[1])
conf_data.set('PROJECT_VERSION_PATCH',meson.project_version().split('.')[2])
unity_version_template = join_paths(meson.source_root(), 'cmake/templates/unity_version.h.in')
unity_version_file =configure_file(input : unity_version_template,
                                   output : 'unity_version.h',
                                   configuration : conf_data)
unity_dir = include_directories('.')

unity_lib = static_library(meson.project_name(), 
    sources: ['unity.c'],
    include_directories: unity_dir)
