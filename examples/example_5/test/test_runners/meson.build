###################################################################################
#                                                                                 #
# NAME: examples/example_5/test/test_runners/meson.build                          #
#                                                                                 #
# AUTHOR: Mike Karlesky, Mark VanderVoord, Greg Williams.                         #
# WRITTEN BY: Michael Brockus.                                                    #
#                                                                                 #
# License: MIT                                                                    #
#                                                                                 #
###################################################################################



test_src_1 = [ 
    'TestProductionCode_Runner.cc',
    join_paths('..' ,'TestProductionCode.cc')
    ]
test_src_2 = [ 
    'TestProductionCode2_Runner.cc',
    join_paths('..' ,'TestProductionCode2.cc') 
    ]

test('Test production code one', executable('test-1', test_src_1, dependencies: [ a_dep, unity_dep ]))
test('Test production code two', executable('test-2', test_src_2, dependencies: [ b_dep, unity_dep ]))