#
# build script written by : Michael Gene Brockus.
# github repo author: Mike Karlesky, Mark VanderVoord, Greg Williams.
#
# license: MIT
#
project('unity', 'c',
    license: 'MIT',
    meson_version: '>=0.37.0',
    default_options: ['werror=true', 'c_std=c11'])

subdir('src')
unity_dep = declare_dependency(link_with: unity_lib, include_directories: unity_dir)


# Get the generate_test_runner script relative to itself or the parent project if it is being used as a subproject
# NOTE: This could be (and probably is) a complete hack - but I haven't yet been able to find a better way....
if meson.is_subproject()
gen_test_runner_path = find_program(meson.source_root() / 'subprojects/unity/auto/generate_test_runner.rb')
else
gen_test_runner_path = find_program('subprojects/unity/auto/generate_test_runner.rb')
endif

# Create a generator that we can access from the parent project
gen_test_runner = generator(gen_test_runner_path, output: '@BASENAME@_Runner.c', arguments: ['@INPUT@', '@OUTPUT@'] )
