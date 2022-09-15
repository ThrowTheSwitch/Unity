#
# build script written by : Michael Brockus.
# github repo author: Mike Karlesky, Mark VanderVoord, Greg Williams.
#
# license: MIT
#
cases = [
          ['TestProductionCode_Runner.c',  join_paths('..' ,'TestProductionCode.c' )],
          ['TestProductionCode2_Runner.c', join_paths('..' ,'TestProductionCode2.c')]
        ]

test('01-test-case', executable('01-test-case', cases[0], dependencies: [ a_dep, unity_dep ]))
test('02-test-case', executable('02-test-case', cases[1], dependencies: [ b_dep, unity_dep ]))
