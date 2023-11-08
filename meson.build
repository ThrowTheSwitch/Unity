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
      'auto/extract_version.py',
      'src/unity.h'
    ],
    check: true
  ).stdout().strip(),

  meson_version: '>=0.47.0',
  default_options: [
    'werror=true',
    'c_std=c11'
  ]
)

build_fixture = get_option('extension_fixture')
build_memory = get_option('extension_memory')
support_double = get_option('support_double')

unity_args = []
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

if support_double
  unity_args += '-DUNITY_INCLUDE_DOUBLE'
endif

unity_lib = static_library(meson.project_name(),
  sources: unity_src,
  c_args: unity_args,
  include_directories: unity_inc,
  install: not meson.is_subproject(),
)

unity_dep = declare_dependency(
  link_with: unity_lib,
  include_directories: unity_inc
)

# Generate pkg-config file.
if not meson.is_subproject()
  pkg = import('pkgconfig')
  pkg.generate(
    name: meson.project_name(),
    version: meson.project_version(),
    libraries: [ unity_lib ],
    description: 'C Unit testing framework.'
  )
endif

# Create a generator that can be used by consumers of our build system to generate
# test runners.
gen_test_runner = generator(
  find_program('auto/generate_test_runner.rb'),
  output: '@BASENAME@_Runner.c',
  arguments: ['@INPUT@', '@OUTPUT@']
)
