unity_inc += include_directories('.')
unity_src += files('unity_memory.c')

if not meson.is_subproject()
  install_headers(
    'unity_memory.h',
    subdir: meson.project_name()
  )
endif
