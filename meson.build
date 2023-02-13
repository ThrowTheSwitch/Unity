#
# build script written by : Michael Gene Brockus.
# github repo author: Mike Karlesky, Mark VanderVoord, Greg Williams.
#
# license: MIT
#
project('unity', 'c',
  license: 'MIT',
  # `meson.project_source_root()` introduced in 0.56.0
  meson_version: '>=0.56.0',
  default_options: [
    'werror=true',
    'c_std=c11'
  ]
)

subdir('src')
unity_dep = declare_dependency(link_with: unity_lib, include_directories: unity_dir)

# Create a generator that can be used by consumers of our build system to generate
# test runners.
gen_test_runner = generator(
  find_program(meson.project_source_root() / 'auto' / 'generate_test_runner.rb'),
  output: '@BASENAME@_Runner.c',
  arguments: ['@INPUT@', '@OUTPUT@']
)
