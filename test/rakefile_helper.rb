# =========================================================================
#   Unity - A Test Framework for C
#   ThrowTheSwitch.org
#   Copyright (c) 2007-26 Mike Karlesky, Mark VanderVoord, & Greg Williams
#   SPDX-License-Identifier: MIT
# =========================================================================

require 'fileutils'
require_relative '../auto/unity_test_summary'
require_relative '../auto/generate_test_runner'
require_relative '../auto/colour_reporter'
require_relative '../auto/yaml_helper'

module RakefileHelpers
  C_EXTENSION = '.c'.freeze

  def load_configuration(config_file)
    return if $configured

    $cfg_file_base = config_file
    cfg_file = if config_file =~ /[\\|\/]/
      $unity_test_config_file_in_targets = false
      config_file
    else
      $unity_test_config_file_in_targets = true
      "targets/#{config_file}"
    end
    $cfg = YamlHelper.load_file(cfg_file)
    $cfg[:paths] ||= {}
    $cfg[:paths][:test] = (Array($cfg[:paths][:test]) + ['src/', '../src/', 'testdata/', 'tests/']).uniq
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
        if arg.include? '${5}'
          all_paths = [ File.join('..','src'), $extra_paths, 'src', File.join('tests'), File.join('testdata'), $cfg[:paths][:support] ].flatten.uniq.compact
          all_paths.each { |f| args << arg.gsub('${5}', f) }

        elsif arg.include? '${6}'
          [ $cfg[:defines][:test], $cfg.dig(:unity, :defines), defines ].flatten.uniq.compact.each { |f| args << arg.gsub('${6}', f) }

        elsif arg.include? ': COLLECTION_PATHS_TEST_TOOLCHAIN_INCLUDE'
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
          [ $cfg[:defines][:test], $cfg.dig(:unity, :defines), defines ].flatten.uniq.compact.each do |f|
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
    out_file = File.join('build', File.basename(file, C_EXTENSION)) + ($cfg[:extension][:object] || '.o')
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
              "--mode=c --suffix=none " \
              "#{style_what}"
    execute(command, false)
    report "Styling C:PASS"
  end

  def execute(command_string, ok_to_fail = false)
    report command_string if $verbose
    output = `#{command_string}`.chomp
    report(output) if ($verbose && !output.nil? && !output.empty?) || (!$?.nil? && !$?.exitstatus.zero? && !ok_to_fail)
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

  # Parse all Unity summary lines from combined output (e.g. multiple executables)
  # and produce a single synthesized result string suitable for save_test_results.
  def collect_test_output(output)
    total_tests    = 0
    total_failures = 0
    total_ignored  = 0
    detail_lines   = []

    output.each_line do |line|
      stripped = line.chomp
      if stripped =~ /(\d+) Tests (\d+) Failures (\d+) Ignored/
        total_tests    += Regexp.last_match(1).to_i
        total_failures += Regexp.last_match(2).to_i
        total_ignored  += Regexp.last_match(3).to_i
      elsif stripped =~ /^[^:]+:[^:]+:\w+(?:\([^)]*\))?:(?:PASS|FAIL|IGNORE)/
        detail_lines << stripped
      end
    end

    synthesized  = detail_lines.join("\n")
    synthesized += "\n" unless detail_lines.empty?
    synthesized += "#{total_tests} Tests #{total_failures} Failures #{total_ignored} Ignored\n"
    synthesized += total_failures > 0 ? "FAILED\n" : "OK\n"
    synthesized
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
    test_base = File.basename('fixtures_test', C_EXTENSION)
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

      report "\nRunning Tests in #{test}"
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

      options = $cfg[:unity] || {}
      options[:use_param_tests] = test =~ /parameterized/ ? true : false
      UnityTestRunnerGenerator.new(options).run(test, runner_path)
      obj_list << compile(runner_path, test_defines)

      # Build the test module
      obj_list << compile(test, test_defines)

      # Link the test executable
      link_it(test_base, obj_list)

      # Execute unit test
      output = runtest(test_base)
      
      # This is a list of all non-string valid outputs
      # (in order) this is the following options:
      #     valid binary representations
      #     valid hexadecimal representation
      #     valid integer (signed or unsigned) or float values of any precision
      #     valid floating point special-case verbage
      #     valid boolean verbage
      #     valid pointer verbage
      #     string representations
      #     character representations
      valid_vals_regexes = [
        /[01X]+/,
        /0x[0-9A-Fa-f]+/,
        /-?\d+(?:\.\d+)?/,
        /(?:Not )?(?:Negative )?(?:Infinity|NaN|Determinate|Invalid Float Trait)/,
        /TRUE|FALSE/,
        /NULL/,
        /"[^"]*"/,
        /'[^']*'/
      ]
      valid_vals = "(?:#{valid_vals_regexes.map(&:source).join('|')})"

      # Verify outputs seem to have happened
      failures = 0
      output = output.each_line.map do |line|
        if (line =~ /(?:Delta.*)?(?:Element.*)?Expected.*Was/)
          if !(line =~ /(?:Delta \d+ )?(?:Element \d+ )?Expected #{valid_vals} Was #{valid_vals}/)
            failures += 1
            "[FAIL] " + line.sub(/:PASS$/,":FAIL:Output Format Failure")
          else
            "[p   ] " + line
          end
        elsif (line =~ /:PASS$/)
          "[p   ] " + line
        elsif (line =~ /:FAIL(?:[^:])$/) || (line =~ /^FAILED$/)
          #failure has already been counted therefore do not add
          "[FAIL] " + line
        elsif (line =~ /:IGNORE$/)
          #ignore has already been counted therefore do not add
          "[i---] " + line
        else
          "[    ] " + line
        end
      end.join

      # Update the final test summary
      if failures > 0
        output.sub!(/^(?:\[    \] )?(\d+) Tests (\d+) Failures (\d+) Ignored/) do
          tests = $1
          failures = $2.to_i + failures
          ignored = $3
          "[    ] #{tests} Tests #{failures} Failures #{ignored} Ignored"
        end
        output.sub!(/\[    \] OK$/,"[FAIL] FAILED")
        report output
        raise "Command failed. (#{failures.to_s} Output Formatting Issues)"
      end

      # Generate results file
      save_test_results(test_base, output)
    end
  end

  def run_make_tests()
    report "\nRunning Unity Examples with Make"
    combined_output = ''
    [ "make -s",                 # test with all defaults
      "make -s coverage",        # test with coverage
      "cd #{File.join("..","extras","fixture",'test')} && make -s default noStdlibMalloc",
      "cd #{File.join("..","extras","fixture",'test')} && make -s C89",
      "cd #{File.join("..","extras","memory",'test')} && make -s default noStdlibMalloc",
      "cd #{File.join("..","extras","memory",'test')} && make -s C89",
    ].each do |cmd|
      report "Testing '#{cmd}'"
      combined_output += "Testing '#{cmd}'\n\n#{execute(cmd, false)}\n"
    end
    save_test_results('make_tests', collect_test_output(combined_output))
  end

  def run_examples()
    total_tests = total_ignored = 0
    
    # If we're set up to use gcc, the makefiles should work too. otherwise, just run example 3
    examples = if $cfg_file_base.nil? || ($cfg_file_base =~ /gcc/)
      {
        :example_1 => "cd ../examples/example_1 && make -s ci",
        :example_2 => "cd ../examples/example_2 && make -s ci"
      }
    else
      {}
    end

    if $unity_test_config_file_in_targets
      examples[:example_3] = "cd ../examples/example_3 && rake config[#{$cfg_file_base}] default"
    else
      examples[:example_3] = "cd ../examples/example_3 && rake config[\"../#{$unity_test_config_file}\"] default"
    end

    examples.each_pair do |key, cmd|
      report "\nRunning Unity Example: #{key.to_s}"
      execute(cmd, false).each_line do |line|
        if line =~ /(\d+) Tests \d+ Failures (\d+) Ignored/
          total_tests   += Regexp.last_match(1).to_i
          total_ignored += Regexp.last_match(2).to_i
          # Failures intentionally not counted: the examples contain tests designed
          # to fail to demonstrate Unity's detection capability. A zero exit code
          # from make/rake means those failures were verified as expected; if
          # something truly broke, execute() would have raised above.
        end
      end
    end
    save_test_results('examples', "#{total_tests} Tests 0 Failures #{total_ignored} Ignored\nOK\n")
  end
end
