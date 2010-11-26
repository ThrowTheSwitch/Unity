# ==========================================
#   CMock Project - Automatic Mock Generation for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

ruby_version = RUBY_VERSION.split('.')
if (ruby_version[1].to_i == 9) and (ruby_version[2].to_i > 1)
  require 'rubygems'
  gem 'test-unit'
end
require 'test/unit'
require './auto/generate_test_runner.rb'

TEST_FILE = 'test/testdata/testsample.c'
TEST_MOCK = 'test/testdata/mocksample.c'
OUT_FILE  = 'build/testsample_'
EXP_FILE  = 'test/expectdata/testsample_'

class TestGenerateTestRunner < Test::Unit::TestCase
  def setup
  end

  def teardown
  end
  
  def verify_output_equal(subtest)
    expected = File.read(EXP_FILE + subtest + '.c').gsub(/\r\n/,"\n")
    actual   = File.read(OUT_FILE + subtest + '.c').gsub(/\r\n/,"\n")
    assert_equal(expected, actual, "Generated File Sub-Test '#{subtest}' Failed")
  end
  
  def test_ShouldGenerateARunnerByCreatingRunnerWithOptions
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
  
  def test_ShouldGenerateARunnerByRunningRunnerWithOptions
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
  
  def test_ShouldGenerateARunnerByPullingYamlOptions
    subtest = 'yaml'
    cmdstr = "ruby auto/generate_test_runner.rb test/testdata/sample.yml \"#{TEST_FILE}\" \"#{OUT_FILE + subtest + '.c'}\""
    `#{cmdstr}`
    verify_output_equal(subtest)
    
    cmdstr = "ruby auto/generate_test_runner.rb test/testdata/sample.yml \"#{TEST_MOCK}\" \"#{OUT_FILE + 'mock_' + subtest + '.c'}\""
    `#{cmdstr}`
    verify_output_equal('mock_' + subtest)
  end
  
  def test_ShouldGenerateARunnerByPullingCommandlineOptions
    subtest = 'cmd'
    cmdstr = "ruby auto/generate_test_runner.rb -cexception \"#{TEST_FILE}\" \"#{OUT_FILE + subtest + '.c'}\""
    `#{cmdstr}`
    verify_output_equal(subtest)
    
    cmdstr = "ruby auto/generate_test_runner.rb -cexception \"#{TEST_MOCK}\" \"#{OUT_FILE + 'mock_' + subtest + '.c'}\""
    `#{cmdstr}`
    verify_output_equal('mock_' + subtest)
  end
  
  def test_ShouldGenerateARunnerThatUsesParameterizedTests
    sets = { 'param'  => { :plugins => [:ignore], :use_param_tests => true }   
    }
    
    sets.each_pair do |subtest, options|
      UnityTestRunnerGenerator.new(options).run(TEST_FILE, OUT_FILE + subtest + '.c')
      verify_output_equal(subtest)
      UnityTestRunnerGenerator.new(options).run(TEST_MOCK, OUT_FILE + 'mock_' + subtest + '.c')
      verify_output_equal('mock_' + subtest)
    end
  end
  
end
