# ==========================================
#   CMock Project - Automatic Mock Generation for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

require '../auto/generate_test_runner.rb'

TEST_FILE = 'testdata/testsample.c'
TEST_MOCK = 'testdata/mocksample.c'
OUT_FILE  = 'build/testsample_'
EXP_FILE  = 'expectdata/testsample_'

$generate_test_runner_failures = 0

def verify_output_equal(subtest)
  expected = File.read(EXP_FILE + subtest + '.c').gsub(/\r\n/,"\n")
  actual   = File.read(OUT_FILE + subtest + '.c').gsub(/\r\n/,"\n")
  if (expected != actual)
    report("    #{subtest}:FAIL")
    $generate_test_runner_failures += 1
  else
    report("    #{subtest}:PASS")
  end
end

should "GenerateARunnerByCreatingRunnerWithOptions" do
  sets = { 'def'  => nil,
           'new1' => { :plugins => [:cexception], :includes => ['one.h', 'two.h'], :enforce_strict_ordering => true },
           'new2' => { :plugins => [:ignore], :suite_setup => "a_custom_setup();", :suite_teardown => "a_custom_teardown();" }
  }

  sets.each_pair do |subtest, options|
    UnityTestRunnerGenerator.new(options).run(TEST_FILE, OUT_FILE + subtest + '.c')
    verify_output_equal(subtest)
    UnityTestRunnerGenerator.new(options).run(TEST_MOCK, OUT_FILE + 'mock_' + subtest + '.c')
    verify_output_equal('mock_' + subtest)
  end
end

should "GenerateARunnerAlongWithAHeaderIfSpecified" do
  sets = { 'head1' => { :header_file => "#{OUT_FILE}head1.h" } }
  sets.each_pair do |subtest, options|
    UnityTestRunnerGenerator.new(options).run(TEST_FILE, OUT_FILE + subtest + '.c')
    verify_output_equal(subtest)
  end

  sets = { 'head1' => { :header_file => "#{OUT_FILE}mock_head1.h" } }
  sets.each_pair do |subtest, options|
    UnityTestRunnerGenerator.new(options).run(TEST_MOCK, OUT_FILE + 'mock_' + subtest + '.c')
    verify_output_equal('mock_' + subtest)
  end
end

should "GenerateARunnerByRunningRunnerWithOptions" do
  sets = { 'run1' => { :plugins => [:cexception], :includes => ['one.h', 'two.h'], :enforce_strict_ordering => true },
           'run2' => { :plugins => [:ignore], :suite_setup => "a_custom_setup();", :suite_teardown => "a_custom_teardown();" }
  }

  sets.each_pair do |subtest, options|
    UnityTestRunnerGenerator.new.run(TEST_FILE, OUT_FILE + subtest + '.c', options)
    verify_output_equal(subtest)
    UnityTestRunnerGenerator.new.run(TEST_MOCK, OUT_FILE + 'mock_' + subtest + '.c', options)
    verify_output_equal('mock_' + subtest)
  end
end

should "GenerateARunnerByPullingYamlOptions" do
  subtest = 'yaml'
  cmdstr = "ruby ../auto/generate_test_runner.rb testdata/sample.yml \"#{TEST_FILE}\" \"#{OUT_FILE + subtest + '.c'}\""
  `#{cmdstr}`
  verify_output_equal(subtest)

  cmdstr = "ruby ../auto/generate_test_runner.rb testdata/sample.yml \"#{TEST_MOCK}\" \"#{OUT_FILE + 'mock_' + subtest + '.c'}\""
  `#{cmdstr}`
  verify_output_equal('mock_' + subtest)
end

should "GenerateARunnerByPullingCommandlineOptions" do
  subtest = 'cmd'
  cmdstr = "ruby ../auto/generate_test_runner.rb -cexception \"#{TEST_FILE}\" \"#{OUT_FILE + subtest + '.c'}\""
  `#{cmdstr}`
  verify_output_equal(subtest)

  cmdstr = "ruby ../auto/generate_test_runner.rb -cexception \"#{TEST_MOCK}\" \"#{OUT_FILE + 'mock_' + subtest + '.c'}\""
  `#{cmdstr}`
  verify_output_equal('mock_' + subtest)
end

should "GenerateARunnerThatUsesParameterizedTests" do
  sets = { 'param'  => { :plugins => [:ignore], :use_param_tests => true }
  }

  sets.each_pair do |subtest, options|
    UnityTestRunnerGenerator.new(options).run(TEST_FILE, OUT_FILE + subtest + '.c')
    verify_output_equal(subtest)
    UnityTestRunnerGenerator.new(options).run(TEST_MOCK, OUT_FILE + 'mock_' + subtest + '.c')
    verify_output_equal('mock_' + subtest)
  end
end

raise "There were #{$generate_test_runner_failures.to_s} failures while testing generate_test_runner.rb" if ($generate_test_runner_failures > 0)
