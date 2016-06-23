# ==========================================
#   CMock Project - Automatic Mock Generation for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

require '../auto/generate_test_runner.rb'

$generate_test_runner_tests = 0
$generate_test_runner_failures = 0

OUT_FILE  = 'build/testsample_'

RUNNER_TESTS = [
  { :name => 'DefaultsThroughOptions',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :options => nil, #defaults
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'should_RunTestsStartingWithShouldByDefault',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'DefaultsThroughCommandLine',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :cmdline => "", #defaults
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'should_RunTestsStartingWithShouldByDefault',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'DefaultsThroughYAMLFile',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :cmdline => "", #defaults
    :yaml => {}, #defaults
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'should_RunTestsStartingWithShouldByDefault',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'ShorterFilterOfJustTest',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :options => {
      :test_prefix => "test",
    },
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'ShorterFilterOfJustShould',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :options => {
      :test_prefix => "should",
    },
    :expected => {
      :to_pass => [ 'should_RunTestsStartingWithShouldByDefault' ],
      :to_fail => [  ],
      :to_ignore => [  ],
    }
  },

  { :name => 'ShorterFilterOfJustSpec',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :options => {
      :test_prefix => "spec",
    },
    :expected => {
      :to_pass => [ 'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                  ],
      :to_fail => [  ],
      :to_ignore => [  ],
    }
  },

  { :name => 'InjectIncludes',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :options => {
      :includes => ['Defs.h'],
    },
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'should_RunTestsStartingWithShouldByDefault',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'ParameterizedThroughOptions',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :options => {
      :test_prefix => "paratest",
      :use_param_tests => true,
    },
    :expected => {
      :to_pass => [ 'paratest_ShouldHandleParameterizedTests\(25\)',
                    'paratest_ShouldHandleParameterizedTests\(125\)',
                    'paratest_ShouldHandleParameterizedTests\(5\)',
                    'paratest_ShouldHandleParameterizedTests2\(7\)',
                    'paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid\(RUN_TEST_NO_ARGS\)',
                  ],
      :to_fail => [ 'paratest_ShouldHandleParameterizedTestsThatFail\(17\)' ],
      :to_ignore => [ ],
    }
  },

  { :name => 'ParameterizedThroughCommandLine',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :cmdline => " --test_prefix=\"paratest\" --use_param_tests=1",
    :expected => {
      :to_pass => [ 'paratest_ShouldHandleParameterizedTests\(25\)',
                    'paratest_ShouldHandleParameterizedTests\(125\)',
                    'paratest_ShouldHandleParameterizedTests\(5\)',
                    'paratest_ShouldHandleParameterizedTests2\(7\)',
                    'paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid\(RUN_TEST_NO_ARGS\)',
                  ],
      :to_fail => [ 'paratest_ShouldHandleParameterizedTestsThatFail\(17\)' ],
      :to_ignore => [ ],
    }
  },

  { :name => 'ParameterizedThroughCommandLineAndYaml',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :cmdline => "--use_param_tests=1",
    :yaml => {
      :test_prefix => "paratest"
    },
    :expected => {
      :to_pass => [ 'paratest_ShouldHandleParameterizedTests\(25\)',
                    'paratest_ShouldHandleParameterizedTests\(125\)',
                    'paratest_ShouldHandleParameterizedTests\(5\)',
                    'paratest_ShouldHandleParameterizedTests2\(7\)',
                    'paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid\(RUN_TEST_NO_ARGS\)',
                  ],
      :to_fail => [ 'paratest_ShouldHandleParameterizedTestsThatFail\(17\)' ],
      :to_ignore => [ ],
    }
  },

  { :name => 'SupportCExceptionWhenRequested',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST', 'USE_CEXCEPTION'],
    :options => {
      :test_prefix => "extest",
      :cexception => 1,
    },
    :expected => {
      :to_pass => [ 'extest_ShouldHandleCExceptionInTest' ],
      :to_fail => [ ],
      :to_ignore => [ ],
    }
  },

  { :name => 'CustomSetupAndTeardownThroughOptions',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :options => {
      :test_prefix => "custtest|test",
      :setup_name => "custom_setup",
      :teardown_name => "custom_teardown",
    },
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'custtest_ThisTestPassesWhenCustomSetupRan',
                    'custtest_ThisTestPassesWhenCustomTeardownRan',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'CustomSetupAndTeardownThroughCommandLine',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :cmdline => " --test_prefix=\"custtest|test\" --setup_name=\"custom_setup\" --teardown_name=\"custom_teardown\"",
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'custtest_ThisTestPassesWhenCustomSetupRan',
                    'custtest_ThisTestPassesWhenCustomTeardownRan',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'CustomSetupAndTeardownThroughYaml',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :cmdline => " --test_prefix=\"custtest|test\"",
    :yaml => {
      :setup_name => "custom_setup",
      :teardown_name => "custom_teardown",
    },
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'custtest_ThisTestPassesWhenCustomSetupRan',
                    'custtest_ThisTestPassesWhenCustomTeardownRan',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'UseACustomMainFunction',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST', "USE_ANOTHER_MAIN"],
    :options => {
      :main_name => "custom_main",
    },
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'should_RunTestsStartingWithShouldByDefault',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'SupportCustomSuiteSetupAndTeardown',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :includes => ['Defs.h'],
    :options => {
      :test_prefix    => "suitetest|test",
      :suite_setup    => "  CounterSuiteSetup = 1;",
      :suite_teardown => "  return num_failures;",
    },
    :expected =>  {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'suitetest_ThisTestPassesWhenCustomSuiteSetupAndTeardownRan',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'SupportMainExternDeclaration',
    :testfile => 'testdata/testRunnerGenerator.c',
    :testdefines => ['TEST'],
    :includes => ['Defs.h'],
    :options => {
      :main_export_decl => "EXTERN_DECL",
    },
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'should_RunTestsStartingWithShouldByDefault',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },


  #### WITH MOCKS ##########################################

  { :name => 'DefaultsThroughOptions',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :options => nil, #defaults
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'should_RunTestsStartingWithShouldByDefault',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                    'test_ShouldCallMockInitAndVerifyFunctionsForEachTest',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'DefaultsThroughCommandLine',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :cmdline => "", #defaults
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'should_RunTestsStartingWithShouldByDefault',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                    'test_ShouldCallMockInitAndVerifyFunctionsForEachTest',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'DefaultsThroughYAMLFile',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :cmdline => "", #defaults
    :yaml => {}, #defaults
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'should_RunTestsStartingWithShouldByDefault',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                    'test_ShouldCallMockInitAndVerifyFunctionsForEachTest',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'ShorterFilterOfJustTest',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :options => {
      :test_prefix => "test",
    },
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'test_ShouldCallMockInitAndVerifyFunctionsForEachTest',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'ShorterFilterOfJustShould',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :options => {
      :test_prefix => "should",
    },
    :expected => {
      :to_pass => [ 'should_RunTestsStartingWithShouldByDefault' ],
      :to_fail => [  ],
      :to_ignore => [  ],
    }
  },

  { :name => 'ShorterFilterOfJustSpec',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :options => {
      :test_prefix => "spec",
    },
    :expected => {
      :to_pass => [ 'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                  ],
      :to_fail => [  ],
      :to_ignore => [  ],
    }
  },

  { :name => 'InjectIncludes',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :options => {
      :includes => ['Defs.h'],
    },
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'should_RunTestsStartingWithShouldByDefault',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                    'test_ShouldCallMockInitAndVerifyFunctionsForEachTest',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'ParameterizedThroughOptions',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :options => {
      :test_prefix => "paratest",
      :use_param_tests => true,
    },
    :expected => {
      :to_pass => [ 'paratest_ShouldHandleParameterizedTests\(25\)',
                    'paratest_ShouldHandleParameterizedTests\(125\)',
                    'paratest_ShouldHandleParameterizedTests\(5\)',
                    'paratest_ShouldHandleParameterizedTests2\(7\)',
                    'paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid\(RUN_TEST_NO_ARGS\)',
                  ],
      :to_fail => [ 'paratest_ShouldHandleParameterizedTestsThatFail\(17\)' ],
      :to_ignore => [ ],
    }
  },

  { :name => 'ParameterizedThroughCommandLine',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :cmdline => " --test_prefix=\"paratest\" --use_param_tests=1",
    :expected => {
      :to_pass => [ 'paratest_ShouldHandleParameterizedTests\(25\)',
                    'paratest_ShouldHandleParameterizedTests\(125\)',
                    'paratest_ShouldHandleParameterizedTests\(5\)',
                    'paratest_ShouldHandleParameterizedTests2\(7\)',
                    'paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid\(RUN_TEST_NO_ARGS\)',
                  ],
      :to_fail => [ 'paratest_ShouldHandleParameterizedTestsThatFail\(17\)' ],
      :to_ignore => [ ],
    }
  },

  { :name => 'ParameterizedThroughCommandLineAndYaml',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :cmdline => "--use_param_tests=1",
    :yaml => {
      :test_prefix => "paratest"
    },
    :expected => {
      :to_pass => [ 'paratest_ShouldHandleParameterizedTests\(25\)',
                    'paratest_ShouldHandleParameterizedTests\(125\)',
                    'paratest_ShouldHandleParameterizedTests\(5\)',
                    'paratest_ShouldHandleParameterizedTests2\(7\)',
                    'paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid\(RUN_TEST_NO_ARGS\)',
                  ],
      :to_fail => [ 'paratest_ShouldHandleParameterizedTestsThatFail\(17\)' ],
      :to_ignore => [ ],
    }
  },

  { :name => 'SupportCExceptionWhenRequested',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST', 'USE_CEXCEPTION'],
    :options => {
      :test_prefix => "extest",
      :cexception => 1,
    },
    :expected => {
      :to_pass => [ 'extest_ShouldHandleCExceptionInTest' ],
      :to_fail => [ ],
      :to_ignore => [ ],
    }
  },

  { :name => 'CustomSetupAndTeardownThroughOptions',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :options => {
      :test_prefix => "custtest|test",
      :setup_name => "custom_setup",
      :teardown_name => "custom_teardown",
    },
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'custtest_ThisTestPassesWhenCustomSetupRan',
                    'custtest_ThisTestPassesWhenCustomTeardownRan',
                    'test_ShouldCallMockInitAndVerifyFunctionsForEachTest',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'CustomSetupAndTeardownThroughCommandLine',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :cmdline => " --test_prefix=\"custtest|test\" --setup_name=\"custom_setup\" --teardown_name=\"custom_teardown\"",
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'custtest_ThisTestPassesWhenCustomSetupRan',
                    'custtest_ThisTestPassesWhenCustomTeardownRan',
                    'test_ShouldCallMockInitAndVerifyFunctionsForEachTest',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'CustomSetupAndTeardownThroughYaml',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :cmdline => " --test_prefix=\"custtest|test\"",
    :yaml => {
      :setup_name => "custom_setup",
      :teardown_name => "custom_teardown",
    },
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'custtest_ThisTestPassesWhenCustomSetupRan',
                    'custtest_ThisTestPassesWhenCustomTeardownRan',
                    'test_ShouldCallMockInitAndVerifyFunctionsForEachTest',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'UseACustomMainFunction',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST', "USE_ANOTHER_MAIN"],
    :options => {
      :main_name => "custom_main",
    },
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'should_RunTestsStartingWithShouldByDefault',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                    'test_ShouldCallMockInitAndVerifyFunctionsForEachTest',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'SupportCustomSuiteSetupAndTeardown',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :includes => ['Defs.h'],
    :options => {
      :test_prefix    => "suitetest|test",
      :suite_setup    => "  CounterSuiteSetup = 1;",
      :suite_teardown => "  return num_failures;",
    },
    :expected =>  {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'suitetest_ThisTestPassesWhenCustomSuiteSetupAndTeardownRan',
                    'test_ShouldCallMockInitAndVerifyFunctionsForEachTest',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },

  { :name => 'SupportMainExternDeclaration',
    :testfile => 'testdata/testRunnerGeneratorWithMocks.c',
    :testdefines => ['TEST'],
    :includes => ['Defs.h'],
    :options => {
      :main_export_decl => "EXTERN_DECL",
    },
    :expected => {
      :to_pass => [ 'test_ThisTestAlwaysPasses',
                    'spec_ThisTestPassesWhenNormalSetupRan',
                    'spec_ThisTestPassesWhenNormalTeardownRan',
                    'test_NotBeConfusedByLongComplicatedStrings',
                    'test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings',
                    'test_StillNotBeConfusedByLongComplicatedStrings',
                    'should_RunTestsStartingWithShouldByDefault',
                    'spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan',
                    'test_ShouldCallMockInitAndVerifyFunctionsForEachTest',
                  ],
      :to_fail => [ 'test_ThisTestAlwaysFails' ],
      :to_ignore => [ 'test_ThisTestAlwaysIgnored' ],
    }
  },
]

def runner_test(test, runner, expected, test_defines)
  # Tack on TEST define for compiling unit tests
  load_configuration($cfg_file)

  #compile objects
  obj_list = [
    compile(runner, test_defines),
    compile(test, test_defines),
    compile('../src/unity.c', test_defines),
  ]

  # Link the test executable
  test_base = File.basename(test, C_EXTENSION)
  link_it(test_base, obj_list)

  # Execute unit test and generate results file
  simulator = build_simulator_fields
  executable = $cfg['linker']['bin_files']['destination'] + test_base + $cfg['linker']['bin_files']['extension']
  if simulator.nil?
    cmd_str = executable
  else
    cmd_str = "#{simulator[:command]} #{simulator[:pre_support]} #{executable} #{simulator[:post_support]}"
  end
  output = execute(cmd_str, true)

  #compare to the expected pass/fail
  allgood = expected[:to_pass].inject(true)      {|s,v| s && (/#{v}:PASS/ =~ output) }
  allgood = expected[:to_fail].inject(allgood)   {|s,v| s && (/#{v}:FAIL/ =~ output) }
  allgood = expected[:to_ignore].inject(allgood) {|s,v| s && (/#{v}:IGNORE/ =~ output) }
  report output if (!allgood && !$verbose) #report failures if not already reporting everything
  return allgood
end

RUNNER_TESTS.each do |testset|
  testset_name = "Runner_#{testset[:testfile]}_#{testset[:name]}"
  should testset_name do
    runner_name = OUT_FILE + testset[:name] + '_runner.c'

    #create a yaml file first if required
    yaml_option = ""
    if (testset[:yaml])
      File.open("build/runner_options.yml",'w') {|f| f << { :unity => testset[:yaml] }.to_yaml }
      yaml_option = "build/runner_options.yml"
    end

    #run script via command line or through hash function call, as requested
    if (testset[:cmdline])
      cmdstr = "ruby ../auto/generate_test_runner.rb #{yaml_option} #{testset[:cmdline]} \"#{testset[:testfile]}\" \"#{runner_name}\""
      `#{cmdstr}`
    else
      UnityTestRunnerGenerator.new(testset[:options]).run(testset[:testfile], runner_name)
    end

    #test the script against the specified test file and check results
    if (runner_test(testset[:testfile], runner_name, testset[:expected], testset[:testdefines]))
      report "#{testset_name}:PASS"
    else
      report "#{testset_name}:FAIL"
      $generate_test_runner_failures += 1
    end
    $generate_test_runner_tests += 1
  end
end

raise "There were #{$generate_test_runner_failures.to_s} failures while testing generate_test_runner.rb" if ($generate_test_runner_failures > 0)
