project('Unity Example', 'c',
  license: 'MIT',
  default_options: [
    'c_std=c89',
    'warning_level=3',
  ])

unity_root = 'subprojects' / 'unity'

unity = subproject('unity')
unity_dep = unity.get_variable('unity_dep')

src1 = files('src/ProductionCode.c', 'test/TestProductionCode.c')
src2 = files('src/ProductionCode2.c', 'test/TestProductionCode2.c')

inc = include_directories('src')

ruby = find_program('ruby')
runner_gen = generator(ruby,
  arguments: [
    '@SOURCE_DIR@' / unity_root / 'auto' / 'generate_test_runner.rb',
    '@INPUT@', '@OUTPUT@',
  ],
  output: '@BASENAME@_Runner.c')


test1 = executable('test1', [
    src1,
    runner_gen.process('test/TestProductionCode.c'),
  ],
  include_directories: inc,
  dependencies: unity_dep)
test('test1', test1,
  should_fail: true)

test2 = executable('test2', [
    src2,
    runner_gen.process('test/TestProductionCode2.c'),
  ],
  include_directories: inc,
  dependencies: unity_dep)
test('test2', test2)
