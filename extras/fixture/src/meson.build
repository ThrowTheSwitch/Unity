unity_inc += include_directories('.')
unity_src += files('unity_fixture.c')

if not meson.is_subproject()
  install_headers(
    'unity_fixture.h',
    'unity_fixture_internals.h',
    subdir: meson.project_name()
  )
endif
