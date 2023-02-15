project('Unity example', 'c',
  license: 'MIT',
  default_options: [
    'c_std=c99',
    'warning_level=3',
  ],
  meson_version: '>= 0.49.0'
)

unity_subproject = subproject('unity')
unity_dependency = unity_subproject.get_variable('unity_dep')
unity_gen_runner = unity_subproject.get_variable('gen_test_runner')

src1 = files([
  'src' / 'ProductionCode.c',
  'test' / 'TestProductionCode.c',
])

src2 = files([
  'src' / 'ProductionCode2.c',
  'test' / 'TestProductionCode2.c',
])

inc = include_directories('src')

test1 = executable('test1',
  sources: [
    src1,
    unity_gen_runner.process('test' / 'TestProductionCode.c')
  ],
  include_directories: [ inc ],
  dependencies: [ unity_dependency ],
)

test('test1', test1,
  should_fail: true)

test2 = executable('test2',
  sources: [
    src2,
    unity_gen_runner.process('test' / 'TestProductionCode2.c')
  ],
  include_directories: [ inc ],
  dependencies: [ unity_dependency ],
)

test('test2', test2)

