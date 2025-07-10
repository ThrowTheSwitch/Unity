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
fixture_help_message = get_option('fixture_help_message')

unity_args = []
unity_src = []
unity_inc = []

subdir('src')

if build_fixture
  subdir('extras/fixture/src')
  if fixture_help_message != ''
    unity_args += '-DUNITY_CUSTOM_HELP_MSG=' + fixture_help_message
  endif
endif

if build_memory.enabled() or (build_memory.auto() and build_fixture)
  subdir('extras/memory/src')
else
  unity_args += '-DUNITY_FIXTURE_NO_EXTRAS'
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
    unity_lib,
    version: meson.project_version(),
    subdirs: 'unity',
    extra_cflags: unity_args,
  )
endif

# Create a generator that can be used by consumers of our build system to generate
# test runners.
gen_test_runner = generator(
  find_program('auto/generate_test_runner.rb'),
  output: '@BASENAME@_Runner.c',
  arguments: ['@INPUT@', '@OUTPUT@']
)
