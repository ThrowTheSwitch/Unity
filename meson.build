#
# build script written by : Michael Gene Brockus.
# github repo author: Mike Karlesky, Mark VanderVoord, Greg Williams.
#
# license: MIT
#
project('unity', 'c',
  license: 'MIT',

  # Set project version to value extracted from unity.h header
  version: run_command(
    [
      find_program('python', 'python3'),
      'auto' / 'extract_version.py',
      'src' / 'unity.h'
    ],
    check: true
  ).stdout().strip(),

  # `meson.project_source_root()` introduced in 0.56.0
  meson_version: '>=0.56.0',
  default_options: [
    'werror=true',
    'c_std=c11'
  ]
)

build_fixture = get_option('extension_fixture').enabled()
build_memory = get_option('extension_memory').enabled()

unity_src = []
unity_inc = []

subdir('src')

if build_fixture
  # Building the fixture extension implies building the memory
  # extension.
  build_memory = true
  subdir('extras/fixture/src')
endif

if build_memory
  subdir('extras/memory/src')
endif

unity_lib = static_library(meson.project_name(),
  sources: unity_src,
  include_directories: unity_inc,
  install: true
)

unity_dep = declare_dependency(
  link_with: unity_lib,
  include_directories: unity_inc
)

# Generate pkg-config file.
pkg = import('pkgconfig')
pkg.generate(unity_lib,
  description: 'C Unit testing framework.'
)

# Create a generator that can be used by consumers of our build system to generate
# test runners.
gen_test_runner = generator(
  find_program(meson.project_source_root() / 'auto' / 'generate_test_runner.rb'),
  output: '@BASENAME@_Runner.c',
  arguments: ['@INPUT@', '@OUTPUT@']
)

# Summarize.
summary(
  {
    'Fixture extension': build_fixture,
    'Memory extension': build_memory,
  },
  section: 'Extensions',
  bool_yn: true
)
