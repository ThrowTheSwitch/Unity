# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

File.expand_path(File.join(File.dirname(__FILE__),'colour_prompt'))

class UnityTestRunnerGenerator

  def initialize(options = nil)
    @options = { :includes => [], :plugins => [], :framework => :unity }
    case(options)
      when NilClass then @options
      when String   then @options.merge!(UnityTestRunnerGenerator.grab_config(options))
      when Hash     then @options.merge!(options)
      else          raise "If you specify arguments, it should be a filename or a hash of options"
    end
  end
  
  def self.grab_config(config_file)
    options = { :includes => [], :plugins => [], :framework => :unity }
    unless (config_file.nil? or config_file.empty?)
      require 'yaml'
      yaml_guts = YAML.load_file(config_file)
      options.merge!(yaml_guts[:unity] ? yaml_guts[:unity] : yaml_guts[:cmock])
      raise "No :unity or :cmock section found in #{config_file}" unless options
    end
    return(options)
  end

  def run(input_file, output_file, options=nil)
    tests = []
    testfile_includes = []
    used_mocks = []
    
    @options.merge!(options) unless options.nil?
    module_name = File.basename(input_file)
    
    #pull required data from source file
    File.open(input_file, 'r') do |input|
      tests               = find_tests(input)
      testfile_includes   = find_includes(input)
      used_mocks          = find_mocks(testfile_includes)
    end

    #build runner file
    generate(input_file, output_file, tests, used_mocks, testfile_includes)
    
    #determine which files were used to return them
    all_files_used = [input_file, output_file]
    all_files_used += testfile_includes.map {|filename| filename + '.c'} unless testfile_includes.empty?
    all_files_used += @options[:includes] unless @options[:includes].empty?
    return all_files_used.uniq
  end
  
  def generate(input_file, output_file, tests, used_mocks, testfile_includes)
    File.open(output_file, 'w') do |output|
      create_header(output, used_mocks, testfile_includes)
      create_externs(output, tests, used_mocks)
      create_mock_management(output, used_mocks)
      create_suite_setup_and_teardown(output)
      create_reset(output, used_mocks)
      create_main(output, input_file, tests)
    end
  end
  
  def find_tests(input_file)
    tests_raw = []
    tests_args = []
    tests_and_line_numbers = []
    
    input_file.rewind
    source_raw = input_file.read
    source_scrubbed = source_raw.gsub(/\/\/.*$/, '')           # remove line comments
    source_scrubbed = source_scrubbed.gsub(/\/\*.*?\*\//m, '') # remove block comments
    lines = source_scrubbed.split(/(^\s*\#.*$)                 # Treat preprocessor directives as a logical line
                              | (;|\{|\}) /x)                  # Match ;, {, and } as end of lines

    lines.each_with_index do |line, index|
      #find tests
      if line =~ /^((?:\s*TEST_CASE\s*\(.*?\)\s*)*)\s*void\s+(test.*?)\s*\(\s*(.*)\s*\)/
        arguments = $1
        name = $2
        call = $3
        args = nil
        if (@options[:use_param_tests] and !arguments.empty?)
          args = []
          arguments.scan(/\s*TEST_CASE\s*\((.*)\)\s*$/) {|a| args << a[0]}
        end
        tests_and_line_numbers << { :test => name, :args => args, :call => call, :line_number => 0 }
        tests_args = []
      end
    end

    #determine line numbers and create tests to run
    source_lines = source_raw.split("\n")
    source_index = 0;
    tests_and_line_numbers.size.times do |i|
      source_lines[source_index..-1].each_with_index do |line, index|
        if (line =~ /#{tests_and_line_numbers[i][:test]}/)
          source_index += index
          tests_and_line_numbers[i][:line_number] = source_index + 1
          break
        end
      end
    end
    
    return tests_and_line_numbers
  end

  def find_includes(input_file)
    input_file.rewind
    
    #read in file
    source = input_file.read
    
    #remove comments (block and line, in three steps to ensure correct precedence)
    source.gsub!(/\/\/(?:.+\/\*|\*(?:$|[^\/])).*$/, '')  # remove line comments that comment out the start of blocks
    source.gsub!(/\/\*.*?\*\//m, '')                     # remove block comments 
    source.gsub!(/\/\/.*$/, '')                          # remove line comments (all that remain)
    
    #parse out includes
    includes = source.scan(/^\s*#include\s+\"\s*(.+)\.[hH]\s*\"/).flatten  
	brackets_includes = source.scan(/^\s*#include\s+<\s*(.+)\s*>/).flatten
	brackets_includes.each { |inc| includes << '<' + inc +'>' }		
	return includes
  end
  
  def find_mocks(includes)
    mock_headers = []
    includes.each do |include_file|
      mock_headers << File.basename(include_file) if (include_file =~ /^mock/i)
    end
    return mock_headers  
  end
  
  def create_header(output, mocks, testfile_includes)
    output.puts('/* AUTOGENERATED FILE. DO NOT EDIT. */')
    create_runtest(output, mocks)
    output.puts("\n//=======Automagically Detected Files To Include=====")
    output.puts("#include \"#{@options[:framework].to_s}.h\"")
    output.puts('#include "cmock.h"') unless (mocks.empty?)
    @options[:includes].flatten.uniq.compact.each do |inc|
      output.puts("#include #{inc.include?('<') ? inc : "\"#{inc.gsub('.h','')}.h\""}")
    end
    output.puts('#include <setjmp.h>')
    output.puts('#include <stdio.h>')
    output.puts('#include "CException.h"') if @options[:plugins].include?(:cexception)
	testfile_includes.delete("unity").delete("cmock")
	testrunner_includes = testfile_includes - mocks	
	testrunner_includes.each do |inc|	  
	  output.puts("#include #{inc.include?('<') ? inc : "\"#{inc.gsub('.h','')}.h\""}")	  
	end
    mocks.each do |mock|
      output.puts("#include \"#{mock.gsub('.h','')}.h\"")
    end
    if @options[:enforce_strict_ordering]
      output.puts('')    
      output.puts('int GlobalExpectCount;') 
      output.puts('int GlobalVerifyOrder;') 
      output.puts('char* GlobalOrderError;') 
    end
  end
  
  def create_externs(output, tests, mocks)
    output.puts("\n//=======External Functions This Runner Calls=====")
    output.puts("extern void setUp(void);")
    output.puts("extern void tearDown(void);")
    tests.each do |test|
      output.puts("extern void #{test[:test]}(#{test[:call] || 'void'});")
    end
    output.puts('')
  end
  
  def create_mock_management(output, mocks)
    unless (mocks.empty?)
      output.puts("\n//=======Mock Management=====")
      output.puts("static void CMock_Init(void)")
      output.puts("{")
      if @options[:enforce_strict_ordering]
        output.puts("  GlobalExpectCount = 0;")
        output.puts("  GlobalVerifyOrder = 0;") 
        output.puts("  GlobalOrderError = NULL;") 
      end
      mocks.each do |mock|
        mock_clean = mock.gsub(/(?:-|\s+)/, "_")
        output.puts("  #{mock_clean}_Init();")
      end
      output.puts("}\n")

      output.puts("static void CMock_Verify(void)")
      output.puts("{")
      mocks.each do |mock|
        mock_clean = mock.gsub(/(?:-|\s+)/, "_")
        output.puts("  #{mock_clean}_Verify();")
      end
      output.puts("}\n")

      output.puts("static void CMock_Destroy(void)")
      output.puts("{")
      mocks.each do |mock|
        mock_clean = mock.gsub(/(?:-|\s+)/, "_")
        output.puts("  #{mock_clean}_Destroy();")
      end
      output.puts("}\n")
    end
  end
  
  def create_suite_setup_and_teardown(output)
    unless (@options[:suite_setup].nil?)
      output.puts("\n//=======Suite Setup=====")
      output.puts("static int suite_setup(void)")
      output.puts("{")
      output.puts(@options[:suite_setup])
      output.puts("}")
    end
    unless (@options[:suite_teardown].nil?)
      output.puts("\n//=======Suite Teardown=====")
      output.puts("static int suite_teardown(int num_failures)")
      output.puts("{")
      output.puts(@options[:suite_teardown])
      output.puts("}")
    end
  end
  
  def create_runtest(output, used_mocks)
    cexception = @options[:plugins].include? :cexception
    va_args1   = @options[:use_param_tests] ? ', ...' : ''
    va_args2   = @options[:use_param_tests] ? '__VA_ARGS__' : ''
    output.puts("\n//=======Test Runner Used To Run Each Test Below=====")
    output.puts("#define RUN_TEST_NO_ARGS") if @options[:use_param_tests] 
    output.puts("#define RUN_TEST(TestFunc, TestLineNum#{va_args1}) \\")
    output.puts("{ \\")
    output.puts("  Unity.CurrentTestName = #TestFunc#{va_args2.empty? ? '' : " \"(\" ##{va_args2} \")\""}; \\")
    output.puts("  Unity.CurrentTestLineNumber = TestLineNum; \\")
    output.puts("  Unity.NumberOfTests++; \\")
    output.puts("  if (TEST_PROTECT()) \\")
    output.puts("  { \\")
    output.puts("    CEXCEPTION_T e; \\") if cexception
    output.puts("    Try { \\") if cexception
    output.puts("      CMock_Init(); \\") unless (used_mocks.empty?) 
    output.puts("      setUp(); \\")
    output.puts("      TestFunc(#{va_args2}); \\")
    output.puts("      CMock_Verify(); \\") unless (used_mocks.empty?)
    output.puts("    } Catch(e) { TEST_ASSERT_EQUAL_HEX32_MESSAGE(CEXCEPTION_NONE, e, \"Unhandled Exception!\"); } \\") if cexception
    output.puts("  } \\")
    output.puts("  CMock_Destroy(); \\") unless (used_mocks.empty?)
    output.puts("  if (TEST_PROTECT() && !TEST_IS_IGNORED) \\")
    output.puts("  { \\")
    output.puts("    tearDown(); \\")
    output.puts("  } \\")
    output.puts("  UnityConcludeTest(); \\")
    output.puts("}\n")
  end
  
  def create_reset(output, used_mocks)
    output.puts("\n//=======Test Reset Option=====")
    output.puts("void resetTest()")
    output.puts("{")
    output.puts("  CMock_Verify();") unless (used_mocks.empty?)
    output.puts("  CMock_Destroy();") unless (used_mocks.empty?)
    output.puts("  tearDown();")
    output.puts("  CMock_Init();") unless (used_mocks.empty?) 
    output.puts("  setUp();")
    output.puts("}")
  end
  
  def create_main(output, filename, tests)
    output.puts("\n\n//=======MAIN=====")
    output.puts("int main(void)")
    output.puts("{")
    output.puts("  suite_setup();") unless @options[:suite_setup].nil?
    output.puts("  Unity.TestFile = \"#{filename}\";")
    output.puts("  UnityBegin();")
    if (@options[:use_param_tests])
      tests.each do |test|
        if ((test[:args].nil?) or (test[:args].empty?))
          output.puts("  RUN_TEST(#{test[:test]}, #{test[:line_number]}, RUN_TEST_NO_ARGS);")
        else
          test[:args].each {|args| output.puts("  RUN_TEST(#{test[:test]}, #{test[:line_number]}, #{args});")}
        end
      end
    else
        tests.each { |test| output.puts("  RUN_TEST(#{test[:test]}, #{test[:line_number]});") }
    end
    output.puts()
    output.puts("  return #{@options[:suite_teardown].nil? ? "" : "suite_teardown"}(UnityEnd());")
    output.puts("}")
  end
end


if ($0 == __FILE__)
  options = { :includes => [] }
  yaml_file = nil
  
  #parse out all the options first
  ARGV.reject! do |arg| 
    case(arg)
      when '-cexception' 
        options[:plugins] = [:cexception]; true
      when /\.*\.yml/
        options = UnityTestRunnerGenerator.grab_config(arg); true
      else false
    end
  end     
           
  #make sure there is at least one parameter left (the input file)
  if !ARGV[0]
    puts ["usage: ruby #{__FILE__} (yaml) (options) input_test_file output_test_runner (includes)",
           "  blah.yml    - will use config options in the yml file (see docs)",
           "  -cexception - include cexception support"].join("\n")
    exit 1
  end
  
  #create the default test runner name if not specified
  ARGV[1] = ARGV[0].gsub(".c","_Runner.c") if (!ARGV[1])
  
  #everything else is an include file
  options[:includes] ||= (ARGV.slice(2..-1).flatten.compact) if (ARGV.size > 2)
  
  UnityTestRunnerGenerator.new(options).run(ARGV[0], ARGV[1])
end
