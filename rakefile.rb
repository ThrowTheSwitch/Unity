$here = File.expand_path( File.dirname( __FILE__ ) )

require 'rake/clean'
require 'rake/loaders/makefile'
require 'fileutils'
require 'set'
require 'auto/unity_test_summary'

#USE THIS ONE IF YOU WANT TO TRY THIS WITH GCC
#require 'rakefile_helper_GCC'

#USE THIS ONE IF YOU WANT TO TRY THIS WITH IAR
require 'rakefile_helper_IAR'

include RakefileHelpers

CLEAN.include('build/*')

desc "Build Unity and run tests."
task :default => [:clobber, :all]

desc "Build Unity object file."
task :build_unity => [:clobber, UNITY_OBJ]

task :run_test => [TEST_RESULTS, :summary].flatten

task :all => [UNITY_TEST_EXEC, :run_test]

task :summary do
  flush_output
  summary = UnityTestSummary.new
  summary.set_root_path($here)
  summary.set_targets(Dir["build/*.test*"])
  summary.run
end

file UNITY_OBJ => [UNITY_SRC, UNITY_HDR] do |obj|
  compile UNITY_SRC, obj.name
end

file UNITY_TEST_OBJ => [UNITY_TEST_SRC, UNITY_SRC, UNITY_HDR] do |obj|
  compile UNITY_TEST_SRC, obj.name
end

file UNITY_TEST_RUNNER_OBJ => [UNITY_TEST_RUNNER_SRC, UNITY_TEST_SRC, UNITY_SRC, UNITY_HDR] do |obj|
  compile UNITY_TEST_RUNNER_SRC, obj.name
end

file UNITY_TEST_EXEC => [UNITY_OBJ, UNITY_TEST_OBJ, UNITY_TEST_RUNNER_OBJ] do |bin|
  link bin.prerequisites, bin.name
end

rule /.*\.test/ => [BIN_EXTENSION] do |file|
  bin = file.name.ext BIN_EXTENSION
  test_results = 'build\sim.txt'
  fail_file = file.name.ext 'testfail'
  if File.exist?(fail_file)
    rm_f fail_file
  end
  rm_f test_results
  rm_f file.name
  output = run_test bin
  if !File.file?(test_results)
    File.open(test_results, 'w') do |f|
      f.print output
    end
  end
  open test_results, 'r' do |f| 
    testoutput = f.read
    if testoutput.index 'OK'
      cp test_results, file.name
    else
      cp test_results, fail_file
    end
  end
end

