project('unity', 'c',
  version: run_command(
    find_program('python'), 'auto' / 'extract_version.py', 'src/unity.h',
    check: true).stdout(),
  license: 'MIT',
  meson_version: '>=0.53.0',
  default_options: ['werror=true', 'c_std=c11']
)

build_fixture = get_option('extension_fixture').enabled()
build_memory = get_option('extension_memory').enabled()

unity_inc = []
unity_src = []

subdir('src')

if build_fixture
  build_memory = true
  subdir('extras/fixture/src')
endif

if build_memory
  subdir('extras/memory/src')
endif

unity_lib = static_library(meson.project_name(), unity_src,
  include_directories: unity_inc,
  install: true)

unity_dep = declare_dependency(
  link_with: unity_lib,
  include_directories: unity_inc)

pkg = import('pkgconfig')
pkg.generate(unity_lib,
  description: 'C Unit testing framework.')

summary({
    'Fixture extension': build_fixture,
    'Memory extension': build_memory,
  },
  section: 'Extensions',
  bool_yn: true)
