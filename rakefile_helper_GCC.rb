#This rakefile sets you up to use GCC as your compiler for tests
module RakefileConstants
  
  C_EXTENSION = '.c'
  OBJ_EXTENSION = '.obj'
  BIN_EXTENSION = '.exe'
  
  UNIT_TEST_PATH = 'test'
  UNITY_PATH = 'src'
  SOURCE_PATH = 'src'
  BUILD_PATH = 'build'
  
  UNITY_SRC = UNITY_PATH + '\unity.c'
  UNITY_HDR = UNITY_PATH + '\unity.h'
  BIN_PATH = 'build'
  UNITY_TEST_SRC = UNIT_TEST_PATH + '\testunity.c'
  UNITY_TEST_RUNNER_SRC = UNIT_TEST_PATH + '\testunity_Runner.c'
  UNITY_OBJ = BIN_PATH + '\unity' + OBJ_EXTENSION
  UNITY_TEST_OBJ = BIN_PATH + '\testunity' + OBJ_EXTENSION
  UNITY_TEST_RUNNER_OBJ = BIN_PATH + '\testunity_Runner' + OBJ_EXTENSION
  UNITY_TEST_EXEC = UNITY_TEST_OBJ.ext BIN_EXTENSION
  TEST_RESULTS = UNITY_TEST_OBJ.ext '.testpass'
  
  COMPILER = 'gcc.exe'
  LINKER   = 'gcc.exe'
  
end

module RakefileHelpers
  include RakefileConstants

  def flush_output
    $stderr.flush
    $stdout.flush
  end
  
  def report message
    puts message
    flush_output
  end

  def compile src, obj
    execute "#{COMPILER} -c -I#{SOURCE_PATH} -I#{UNITY_PATH} -DTEST #{src} -o#{obj}"
  end

  def link prerequisites, executable
    execute "#{LINKER} -DTEST #{prerequisites.join(' ')} -o#{executable}"
  end

  def run_test executable      
    execute "\"#{executable}\""
  end
  
  def write_result_file filename, results
    if (results.include?("OK\n"))
      output_file = filename.gsub(BIN_EXTENSION, '.testpass')
    else
      output_file = filename.gsub(BIN_EXTENSION, '.testfail')
    end
    File.open(output_file, 'w') do |f|
      f.print results
    end
  end
  
private #####################

  def execute command_string
    report command_string
    output = `#{command_string}`
    report output
    output
  end

end
