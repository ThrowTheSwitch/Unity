# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

require 'yaml'
require 'fileutils'
require_relative '../auto/unity_test_summary'
require_relative '../auto/generate_test_runner'
require_relative '../auto/colour_reporter'

module RakefileHelpers
  C_EXTENSION = '.c'.freeze
  def load_configuration(config_file)
    return if $configured

    $cfg_file = "targets/#{config_file}" unless config_file =~ /[\\|\/]/
    $cfg = YAML.load(File.read($cfg_file))
    $colour_output = false unless $cfg['colour']
    $configured = true if config_file != DEFAULT_CONFIG_FILE
  end

  def configure_clean
    CLEAN.include('build/*.*')
  end

  def configure_toolchain(config_file = DEFAULT_CONFIG_FILE)
    config_file += '.yml' unless config_file =~ /\.yml$/
    config_file = config_file unless config_file =~ /[\\|\/]/
    load_configuration(config_file)
    configure_clean
  end

  def unit_test_files
    path = 'tests/test*' + C_EXTENSION
    path.tr!('\\', '/')
    FileList.new(path)
  end

  def local_include_dirs
    include_dirs = $cfg[:paths][:includes] || []
    include_dirs += $cfg[:paths][:source] || []
    include_dirs += $cfg[:paths][:test] || []
    include_dirs += $cfg[:paths][:support] || []
    include_dirs.delete_if { |dir| dir.is_a?(Array) }
    include_dirs
  end

  def extract_headers(filename)
    includes = []
    lines = File.readlines(filename)
    lines.each do |line|
      m = line.match(/^\s*#include\s+\"\s*(.+\.[hH])\s*\"/)
      includes << m[1] unless m.nil?
    end
    includes
  end

  def find_source_file(header, paths)
    paths.each do |dir|
      src_file = dir + header.ext(C_EXTENSION)
      return src_file if File.exist?(src_file)
    end
    nil
  end

  def tackit(strings)
    result = if strings.is_a?(Array)
               "\"#{strings.join}\""
             else
               strings
             end
    result
  end

  def squash(prefix, items)
    result = ''
    items.each { |item| result += " #{prefix}#{tackit(item)}" }
    result
  end

  def should(behave, &block)
    if block
      puts 'Should ' + behave
      yield block
    else
      puts "UNIMPLEMENTED CASE: Should #{behave}"
    end
  end

  def build_command_string(hash, values, defines = nil)

    # Replace named and numbered slots
    args = []
    hash[:arguments].each do |arg|
      if arg.include? '$'
        if arg.include? ': COLLECTION_PATHS_TEST_TOOLCHAIN_INCLUDE' 
          pattern = arg.gsub(': COLLECTION_PATHS_TEST_TOOLCHAIN_INCLUDE','')
          [ File.join('..','src') ].each do |f|
            args << pattern.gsub(/\$/,f)
          end

        elsif arg.include? ': COLLECTION_PATHS_TEST_SUPPORT_SOURCE_INCLUDE_VENDOR' 
          pattern = arg.gsub(': COLLECTION_PATHS_TEST_SUPPORT_SOURCE_INCLUDE_VENDOR','')
          [ $extra_paths, 'src', File.join('tests'), File.join('testdata'), $cfg[:paths][:support] ].flatten.uniq.compact.each do |f|
            args << pattern.gsub(/\$/,f)
          end

        elsif arg.include? ': COLLECTION_DEFINES_TEST_AND_VENDOR'
          pattern = arg.gsub(': COLLECTION_DEFINES_TEST_AND_VENDOR','')
          [ $cfg[:defines][:test], defines ].flatten.uniq.compact.each do |f|
            args << pattern.gsub(/\$/,f)
          end
          
        elsif arg =~ /\$\{(\d+)\}/
          i = $1.to_i - 1
          if (values[i].is_a?(Array))
            values[i].each {|v| args << arg.gsub(/\$\{\d+\}/, v)}
          else
            args << arg.gsub(/\$\{(\d)+\}/, values[i] || '')
          end

        else
          args << arg

        end
      else
        args << arg
      end
    end

    # Build Command
    return tackit(hash[:executable]) + squash('', args)
  end

  def compile(file, defines = [])
    out_file = File.join('build', File.basename(file, C_EXTENSION)) + $cfg[:extension][:object]
    cmd_str = build_command_string( $cfg[:tools][:test_compiler], [ file, out_file ], defines )
    execute(cmd_str)
    out_file
  end

  def link_it(exe_name, obj_list)
    exe_name = File.join('build', File.basename(exe_name))
    cmd_str = build_command_string( $cfg[:tools][:test_linker], [ obj_list, exe_name ] )
    execute(cmd_str)
  end

  def runtest(bin_name, ok_to_fail = false, extra_args = nil)
    bin_name = File.join('build', File.basename(bin_name))
    extra_args = extra_args.nil? ? "" : " " + extra_args
    if $cfg[:tools][:test_fixture]
      cmd_str = build_command_string( $cfg[:tools][:test_fixture], [ bin_name, extra_args ] )
    else
      cmd_str = bin_name + extra_args
    end
    execute(cmd_str, ok_to_fail)
  end

  def run_astyle(style_what)
    report "Styling C Code..."
    command = "AStyle " \
              "--style=allman --indent=spaces=4 --indent-switches --indent-preproc-define --indent-preproc-block " \
              "--pad-oper --pad-comma --unpad-paren --pad-header " \
              "--align-pointer=type --align-reference=name " \
              "--add-brackets --mode=c --suffix=none " \
              "#{style_what}"
    execute(command, false)
    report "Styling C:PASS"
  end

  def execute(command_string, ok_to_fail = false)
    report command_string if $verbose
    output = `#{command_string}`.chomp
    report(output) if $verbose && !output.nil? && !output.empty?
    raise "Command failed. (Returned #{$?.exitstatus})" if !$?.nil? && !$?.exitstatus.zero? && !ok_to_fail
    output
  end

  def report_summary
    summary = UnityTestSummary.new
    summary.root = __dir__
    results_glob = File.join('build','*.test*')
    results_glob.tr!('\\', '/')
    results = Dir[results_glob]
    summary.targets = results
    report summary.run
  end

  def save_test_results(test_base, output)
    test_results = File.join('build',test_base)
    if output.match(/OK$/m).nil?
      test_results += '.testfail'
    else
      report output unless $verbose # Verbose already prints this line, as does a failure
      test_results += '.testpass'
    end
    File.open(test_results, 'w') { |f| f.print output }
  end

  def test_fixtures()
    report "\nRunning Fixture Addon"

    # Get a list of all source files needed
    src_files  = Dir[File.join('..','extras','fixture','src','*.c')]
    src_files += Dir[File.join('..','extras','fixture','test','*.c')]
    src_files += Dir[File.join('..','extras','fixture','test','main','*.c')]
    src_files += Dir[File.join('..','extras','memory','src','*.c')]
    src_files << File.join('..','src','unity.c')

    # Build object files
    $extra_paths = [File.join('..','extras','fixture','src'), File.join('..','extras','memory','src')]
    obj_list = src_files.map { |f| compile(f, ['UNITY_SKIP_DEFAULT_RUNNER', 'UNITY_FIXTURE_NO_EXTRAS']) }

    # Link the test executable
    test_base = File.basename('framework_test', C_EXTENSION)
    link_it(test_base, obj_list)

    # Run and collect output
    output = runtest(test_base + " -v -r")
    save_test_results(test_base, output)
  end

  def test_memory()
    { 'w_malloc' => [], 
      'wo_malloc' => ['UNITY_EXCLUDE_STDLIB_MALLOC'] 
    }.each_pair do |name, defs|
      report "\nRunning Memory Addon #{name}"

      # Get a list of all source files needed
      src_files  = Dir[File.join('..','extras','memory','src','*.c')]
      src_files += Dir[File.join('..','extras','memory','test','*.c')]
      src_files += Dir[File.join('..','extras','memory','test','main','*.c')]
      src_files << File.join('..','src','unity.c')

      # Build object files
      $extra_paths = [File.join('..','extras','memory','src')]
      obj_list = src_files.map { |f| compile(f, defs) }

      # Link the test executable
      test_base = File.basename("memory_test_#{name}", C_EXTENSION)
      link_it(test_base, obj_list)

      # Run and collect output
      output = runtest(test_base)
      save_test_results(test_base, output)
    end
  end

  def run_tests(test_files)
    report "\nRunning Unity system tests"

    include_dirs = local_include_dirs

    # Build and execute each unit test
    test_files.each do |test|

      # Drop Out if we're skipping this type of test
      if $cfg[:skip_tests]
        if $cfg[:skip_tests].include?(:parameterized) && test.match(/parameterized/)
          report("Skipping Parameterized Tests for this Target:IGNORE")
          next
        end
      end

      obj_list = []
      test_defines = []

      # Detect dependencies and build required modules
      extract_headers(test).each do |header|
        # Compile corresponding source file if it exists
        src_file = find_source_file(header, include_dirs)

        obj_list << compile(src_file, test_defines) unless src_file.nil?
      end

      # Build the test runner (generate if configured to do so)
      test_base = File.basename(test, C_EXTENSION)
      runner_name = test_base + '_Runner.c'
      runner_path = File.join('build',runner_name)

      options = $cfg[:unity]
      options[:use_param_tests] = test =~ /parameterized/ ? true : false
      UnityTestRunnerGenerator.new(options).run(test, runner_path)
      obj_list << compile(runner_path, test_defines)

      # Build the test module
      obj_list << compile(test, test_defines)

      # Link the test executable
      link_it(test_base, obj_list)

      # Execute unit test and generate results file
      output = runtest(test_base)
      save_test_results(test_base, output)
    end
  end

  def run_make_tests() 
    [ "make -s",                               # test with all defaults
      "make -s DEBUG=-m32",                    # test 32-bit architecture with 64-bit support
      "make -s DEBUG=-m32 UNITY_SUPPORT_64=",  # test 32-bit build without 64-bit types
      "make -s UNITY_INCLUDE_DOUBLE= ",        # test without double
      "cd #{File.join("..","extras","fixture",'test')} && make -s default noStdlibMalloc",
      "cd #{File.join("..","extras","fixture",'test')} && make -s C89",
      "cd #{File.join("..","extras","memory",'test')} && make -s default noStdlibMalloc",
      "cd #{File.join("..","extras","memory",'test')} && make -s C89",
    ].each do |cmd|
      report "Testing '#{cmd}'"
      execute(cmd, false)
    end
  end
end
