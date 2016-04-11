# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

$QUICK_RUBY_VERSION = RUBY_VERSION.split('.').inject(0){|vv,v| vv * 100 + v.to_i }
File.expand_path(File.join(File.dirname(__FILE__),'colour_prompt'))

class UnityTestRunnerGenerator

  def initialize(options = nil)
    @options = UnityTestRunnerGenerator.default_options
    case(options)
      when NilClass then @options
      when String   then @options.merge!(UnityTestRunnerGenerator.grab_config(options))
      when Hash     then @options.merge!(options)
      else          raise "If you specify arguments, it should be a filename or a hash of options"
    end
    require "#{File.expand_path(File.dirname(__FILE__))}/type_sanitizer"
  end

  def self.default_options
    {
      :includes            => [],
      :plugins             => [],
      :framework           => :unity,
      :test_prefix         => "test|spec|should",
      :setup_name          => "setUp",
      :teardown_name       => "tearDown",
      :main_name           => "main",
      :auto_main_name      => false,
      :main_with_params    => false,
      :main_export_decl    => "",
      :isolation_directive => false,
      :use_param_tests     => false
    }
  end

  def self.grab_config(config_file)
    options = self.default_options
    unless (config_file.nil? or config_file.empty?)
      require 'yaml'
      yaml_guts = YAML.load_file(config_file)
      options.merge!(yaml_guts[:unity] || yaml_guts[:cmock])
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
    source = File.read(input_file)
    source = source.force_encoding("ISO-8859-1").encode("utf-8", :replace => nil) if ($QUICK_RUBY_VERSION > 10900)
    tests               = find_tests(source)
    headers             = find_includes(source)
    testfile_includes   = (headers[:local] + headers[:system])
    used_mocks          = find_mocks(testfile_includes)
    testfile_includes   = (testfile_includes - used_mocks)
    testfile_includes.delete_if{|inc| inc =~ /(unity|cmock)/}

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
      create_header(output, used_mocks, input_file, testfile_includes)
      create_externs(output, tests, used_mocks)
      create_mock_management(output, used_mocks)
      create_suite_setup_and_teardown(output)
      create_reset(output, used_mocks)
      create_main(output, input_file, tests, used_mocks)
    end

    if (@options[:header_file] && !@options[:header_file].empty?)
      File.open(@options[:header_file], 'w') do |output|
        create_h_file(output, @options[:header_file], tests, testfile_includes, used_mocks)
      end
    end
  end

  def find_tests(source)
    tests_and_line_numbers = []

    source_scrubbed = source.clone
    source_scrubbed = source_scrubbed.gsub(/"[^"]*"/, '')      # remove things in strings
    source_scrubbed = source_scrubbed.gsub(/\/\/.*$/, '')      # remove line comments
    source_scrubbed = source_scrubbed.gsub(/\/\*.*?\*\//m, '') # remove block comments
    lines = source_scrubbed.split(/(^\s*\#.*$)                 # Treat preprocessor directives as a logical line
                              | (;|\{|\}) /x)                  # Match ;, {, and } as end of lines

    lines.each_with_index do |line, index|
      #find tests
      if line =~ /^((?:\s*TEST_CASE\s*\(.*?\)\s*)*)\s*void\s+((?:#{@options[:test_prefix]}).*)\s*\(\s*(.*)\s*\)/
        arguments = $1
        name = $2
        call = $3
        params = $4
        args = nil
        if (@options[:use_param_tests] and !arguments.empty?)
          args = []
          arguments.scan(/\s*TEST_CASE\s*\((.*)\)\s*$/) {|a| args << a[0]}
        end
        tests_and_line_numbers << { :test => name, :args => args, :call => call, :params => params, :line_number => 0 }
      end
    end
    tests_and_line_numbers.uniq! {|v| v[:test] }

    #determine line numbers and create tests to run
    source_lines = source.split("\n")
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

  def find_includes(source)

    #remove comments (block and line, in three steps to ensure correct precedence)
    source.gsub!(/\/\/(?:.+\/\*|\*(?:$|[^\/])).*$/, '')  # remove line comments that comment out the start of blocks
    source.gsub!(/\/\*.*?\*\//m, '')                     # remove block comments
    source.gsub!(/\/\/.*$/, '')                          # remove line comments (all that remain)

    #parse out includes
    includes = {
      :local => source.scan(/^\s*#include\s+\"\s*(.+)\.[hH]\s*\"/).flatten,
      :system => source.scan(/^\s*#include\s+<\s*(.+)\s*>/).flatten.map { |inc| "<#{inc}>" }
    }
    return includes
  end

  def find_mocks(includes)
    mock_headers = []
    includes.each do |include_path|
      include_file = File.basename(include_path)
      mock_headers << include_path if (include_file =~ /^mock/i)
    end
    return mock_headers
  end

  def create_header(output, mocks, input_file, testfile_includes=[])
    if @options[:isolation_directive]
		   output.puts("#ifdef USE_REAL_#{input_file.gsub('.c','').gsub('_test', '').upcase}")
	  end
    output.puts('/* AUTOGENERATED FILE. DO NOT EDIT. */')
    create_runtest(output, mocks)
    output.puts("\n//=======Automagically Detected Files To Include=====")
    output.puts("#include \"#{@options[:framework].to_s}.h\"")
    output.puts('#include "cmock.h"') unless (mocks.empty?)
    output.puts('#include <setjmp.h>')
    output.puts('#include <stdio.h>')
    output.puts('#include "CException.h"') if @options[:plugins].include?(:cexception)
    if (@options[:header_file] && !@options[:header_file].empty?)
      output.puts("#include \"#{File.basename(@options[:header_file])}\"")
    else
      @options[:includes].flatten.uniq.compact.each do |inc|
        output.puts("#include #{inc.include?('<') ? inc : "\"#{inc.gsub('.h','')}.h\""}")
      end
      testfile_includes.each do |inc|
        output.puts("#include #{inc.include?('<') ? inc : "\"#{inc.gsub('.h','')}.h\""}")
      end
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
    output.puts("extern void #{@options[:setup_name]}(void);")
    output.puts("extern void #{@options[:teardown_name]}(void);")
    tests.each do |test|
      output.puts("extern void #{test[:test]}(#{test[:call] || 'void'});")
    end
    output.puts('')
  end

  def create_mock_management(output, mock_headers)
    unless (mock_headers.empty?)
      output.puts("\n//=======Mock Management=====")
      output.puts("static void CMock_Init(void)")
      output.puts("{")
      if @options[:enforce_strict_ordering]
        output.puts("  GlobalExpectCount = 0;")
        output.puts("  GlobalVerifyOrder = 0;")
        output.puts("  GlobalOrderError = NULL;")
      end
      mocks = mock_headers.map {|mock| File.basename(mock)}
      mocks.each do |mock|
        mock_clean = TypeSanitizer.sanitize_c_identifier(mock)
        output.puts("  #{mock_clean}_Init();")
      end
      output.puts("}\n")

      output.puts("static void CMock_Verify(void)")
      output.puts("{")
      mocks.each do |mock|
        mock_clean = TypeSanitizer.sanitize_c_identifier(mock)
        output.puts("  #{mock_clean}_Verify();")
      end
      output.puts("}\n")

      output.puts("static void CMock_Destroy(void)")
      output.puts("{")
      mocks.each do |mock|
        mock_clean = TypeSanitizer.sanitize_c_identifier(mock)
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
    output.puts("#define RUN_TEST(TestFunc, #{@options[:main_with_params]? "Filter," : ""} TestLineNum#{va_args1}) \\")
    output.puts("{ \\")
    output.puts("  Unity.CurrentTestName = #TestFunc#{va_args2.empty? ? '' : " \"(\" ##{va_args2} \")\""}; \\")
    output.puts("  #{@options[:main_with_params]? "if (MatchFilter(Filter)) \\" : "\\"}")
	  output.puts("  { \\")
    output.puts("  Unity.CurrentTestLineNumber = TestLineNum; \\")
    output.puts("  Unity.NumberOfTests++; \\")
    output.puts("  CMock_Init(); \\") unless (used_mocks.empty?)
    output.puts("  UNITY_CLR_DETAILS(); \\") unless (used_mocks.empty?)
    output.puts("  if (TEST_PROTECT()) \\")
    output.puts("  { \\")
    output.puts("    CEXCEPTION_T e; \\") if cexception
    output.puts("    Try { \\") if cexception
    output.puts("      #{@options[:setup_name]}(); \\")
    output.puts("      TestFunc(#{va_args2}); \\")
    output.puts("    } Catch(e) { TEST_ASSERT_EQUAL_HEX32_MESSAGE(CEXCEPTION_NONE, e, \"Unhandled Exception!\"); } \\") if cexception
    output.puts("  } \\")
    output.puts("  if (TEST_PROTECT() && !TEST_IS_IGNORED) \\")
    output.puts("  { \\")
    output.puts("    #{@options[:teardown_name]}(); \\")
    output.puts("    CMock_Verify(); \\") unless (used_mocks.empty?)
    output.puts("  } \\")
    output.puts("  CMock_Destroy(); \\") unless (used_mocks.empty?)
    output.puts("  UnityConcludeTest(); \\")
    output.puts("  } \\")	
    output.puts("}\n")
  end

  def create_reset(output, used_mocks)
    output.puts("\n//=======Test Reset Option=====")
    output.puts("void resetTest(void);")
    output.puts("void resetTest(void)")
    output.puts("{")
    output.puts("  CMock_Verify();") unless (used_mocks.empty?)
    output.puts("  CMock_Destroy();") unless (used_mocks.empty?)
    output.puts("  #{@options[:teardown_name]}();")
    output.puts("  CMock_Init();") unless (used_mocks.empty?)
    output.puts("  #{@options[:setup_name]}();")
    output.puts("}")
  end

  def create_main(output, filename, tests, used_mocks)
    main_name = @options[:auto_main_name]? "#{@options[:main_name]}_#{filename.gsub('.c','')}" : "#{@options[:main_name]}"
    output.puts("\n\n//=======MAIN=====")
    if (@options[:main_name] != "main")
           output.puts("#{@options[:main_export_decl]} int #{main_name}(#{@options[:main_with_params]? "int argc, char **argv" : "void"});")
    end
    
    output.puts("#{@options[:main_export_decl]} int #{main_name}(#{@options[:main_with_params]? "int argc, char **argv" : "void"})")
    output.puts("{")
    
    if (@options[:main_with_params])
		  output.puts("  if (IsFlagTrue(UNITY_FLAG_LIST_TESTS, argc, argv))")
		  output.puts("  {")
		  output.puts("    UnityPrint(\"#{filename.gsub('.c','')}.\");")
		  output.puts("    UNITY_PRINT_EOL();")
		 
		  if (@options[:use_param_tests])
			  tests.each do |test|
			  if ((test[:args].nil?) or (test[:args].empty?))
				  output.puts("    UnityPrint(\"  #{test[:test]}(RUN_TEST_NO_ARGS)\");\n    UNITY_PRINT_EOL();")				
			  else
				  test[:args].each {|args| output.puts("    UnityPrint(\"  #{test[:test]}(#{args})\");\n    UNITY_PRINT_EOL();")}				
			  end
		  end
		  else
			  tests.each { |test| output.puts("    UnityPrint(\"  #{test[:test]}\");\n    UNITY_PRINT_EOL();")}
		  end		
		  output.puts("    return 0;")
		  output.puts("  }")
		  output.puts("  else")
		  output.puts("  {")
	  end
    
    output.puts("  suite_setup();") unless @options[:suite_setup].nil?
    output.puts("  UnityBegin(\"#{filename.gsub(/\\/,'\\\\')}\");")
    if (@options[:main_with_params])
      output.puts("  char * Filter = \"\";")
		  output.puts("  if (IsFlagTrue(UNITY_FLAG_FILTER_TESTS, argc, argv)) Filter = argv[1]+3;")		 
	  end
    if (@options[:use_param_tests])
      tests.each do |test|
        if ((test[:args].nil?) or (test[:args].empty?))
          output.puts("  RUN_TEST(#{test[:test]}, #{@options[:main_with_params]? "Filter," : ""} #{test[:line_number]}, RUN_TEST_NO_ARGS);")
        else
          test[:args].each {|args| output.puts("  RUN_TEST(#{test[:test]}, #{@options[:main_with_params]? "Filter," : ""} #{test[:line_number]}, #{args});")}
        end
      end
    else
        tests.each { |test| output.puts("  RUN_TEST(#{test[:test]}, #{@options[:main_with_params]? "Filter," : ""} #{test[:line_number]});") }
    end
    output.puts()
    output.puts("  CMock_Guts_MemFreeFinal();") unless used_mocks.empty?
    output.puts("  return #{@options[:suite_teardown].nil? ? "" : "suite_teardown"}(UnityEnd());")
    output.puts("}")
    
    if @options[:isolation_directive]
		  output.puts("#endif")
	  end
  end

  def create_h_file(output, filename, tests, testfile_includes, used_mocks)
    filename = File.basename(filename).gsub(/[-\/\\\.\,\s]/, "_").upcase
    output.puts("/* AUTOGENERATED FILE. DO NOT EDIT. */")
    output.puts("#ifndef _#{filename}")
    output.puts("#define _#{filename}\n\n")
    output.puts("#include \"#{@options[:framework].to_s}.h\"")
    output.puts('#include "cmock.h"') unless (used_mocks.empty?)
    @options[:includes].flatten.uniq.compact.each do |inc|
      output.puts("#include #{inc.include?('<') ? inc : "\"#{inc.gsub('.h','')}.h\""}")
    end
    testfile_includes.each do |inc|
      output.puts("#include #{inc.include?('<') ? inc : "\"#{inc.gsub('.h','')}.h\""}")
    end
    output.puts "\n"
    tests.each do |test|
      if ((test[:params].nil?) or (test[:params].empty?))
        output.puts("void #{test[:test]}(void);")
      else
        output.puts("void #{test[:test]}(#{test[:params]});")
      end
    end
    output.puts("#endif\n\n")
  end
end

if ($0 == __FILE__)
  options = { :includes => [] }
  yaml_file = nil

  #parse out all the options first (these will all be removed as we go)
  ARGV.reject! do |arg|
    case(arg)
      when '-cexception'
        options[:plugins] = [:cexception]; true
      when /\.*\.ya?ml/
        options = UnityTestRunnerGenerator.grab_config(arg); true
      when /--(\w+)=\"?(.*)\"?/
        options[$1.to_sym] = $2; true
      when /\.*\.h/
        options[:includes] << arg; true
      else false
    end
  end

  #make sure there is at least one parameter left (the input file)
  if !ARGV[0]
    puts ["\nusage: ruby #{__FILE__} (files) (options) input_test_file (output)",
           "\n  input_test_file         - this is the C file you want to create a runner for",
           "  output                  - this is the name of the runner file to generate",
           "                            defaults to (input_test_file)_Runner",
           "  files:",
           "    *.yml / *.yaml        - loads configuration from here in :unity or :cmock",
           "    *.h                   - header files are added as #includes in runner",
           "  options:",
           "    -cexception           - include cexception support",
           "    --setup_name=\"\"       - redefine setUp func name to something else",
           "    --teardown_name=\"\"    - redefine tearDown func name to something else",
           "    --main_name=\"\"        - redefine main func name to something else",
           "    --test_prefix=\"\"      - redefine test prefix from default test|spec|should",
           "    --suite_setup=\"\"      - code to execute for setup of entire suite",
           "    --suite_teardown=\"\"   - code to execute for teardown of entire suite",
           "    --use_param_tests=1   - enable parameterized tests (disabled by default)",
           "    --header_file=\"\"      - path/name of test header file to generate too"
          ].join("\n")
    exit 1
  end

  #create the default test runner name if not specified
  ARGV[1] = ARGV[0].gsub(".c","_Runner.c") if (!ARGV[1])

  UnityTestRunnerGenerator.new(options).run(ARGV[0], ARGV[1])
end
