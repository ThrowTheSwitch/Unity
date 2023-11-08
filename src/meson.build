#
# build script written by : Michael Gene Brockus.
# github repo author: Mike Karlesky, Mark VanderVoord, Greg Williams.
#
# license: MIT
#

unity_inc += include_directories('.')
unity_src += files('unity.c')

if not meson.is_subproject()
  install_headers(
    'unity.h',
    'unity_internals.h',
    subdir: meson.project_name()
  )
endif
