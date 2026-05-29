# =========================================================================
#   Unity - A Test Framework for C
#   ThrowTheSwitch.org
#   Copyright (c) 2007-26 Mike Karlesky, Mark VanderVoord, & Greg Williams
#   SPDX-License-Identifier: MIT
# =========================================================================

require 'fileutils'
require_relative '../../auto/unity_test_summary'
require_relative '../../auto/generate_test_runner'
require_relative '../../auto/colour_reporter'
require_relative '../../auto/yaml_helper'

C_EXTENSION  = '.c'.freeze
TARGETS_PATH = File.join(__dir__, '..', '..', 'test', 'targets').freeze
PROJECT_FILE = File.join(__dir__, 'project.yml').freeze

def load_configuration(config_file)
  $unity_example_config_file = config_file =~ /[\\\/]/ ? config_file : File.join(TARGETS_PATH, config_file)
  project = YamlHelper.load_file(PROJECT_FILE)
  target  = YamlHelper.load_file($unity_example_config_file)

  # Toolchain settings (tools, extensions) come from the target YML.
  # Path and project settings come from project.yml and take precedence.
  $cfg = target.dup
  $cfg[:project] = project[:project]
  $cfg[:paths]   = project[:paths]
  $cfg[:unity]   = project[:unity]  if project[:unity]
  $cfg[:colour]  = project[:colour] if project.key?(:colour)

  # Merge defines: combine target defines with any project-specific defines
  project_defines = project.dig(:defines, :test) || []
  unless project_defines.empty?
    $cfg[:defines] ||= {}
    $cfg[:defines][:test] = (($cfg.dig(:defines, :test) || []) + project_defines).uniq
  end

  $colour_output = $cfg[:colour] ? true : false
end

def configure_clean
  CLEAN.include(File.join($cfg[:project][:build_root], '*.*'))
end

def configure_toolchain(config_file = DEFAULT_CONFIG_FILE)
  config_file += '.yml' unless config_file =~ /\.yml$/
  load_configuration(config_file)
  configure_clean
end

def unit_test_files
  files = FileList.new
  ($cfg[:paths][:test] || []).each do |path|
    files.include(File.join(path, "Test*#{C_EXTENSION}"))
  end
  files
end

def local_include_dirs
  include_dirs = ($cfg[:paths][:source] || []) + ($cfg[:paths][:test] || []) + ($cfg[:paths][:support] || [])
  include_dirs.delete_if { |dir| dir.is_a?(Array) }
  include_dirs
end

def extract_headers(filename)
  includes = []
  File.readlines(filename).each do |line|
    m = line.match(/^\s*#include\s+"\s*(.+\.[hH])\s*"/)
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
  strings.is_a?(Array) ? "\"#{strings.join}\"" : strings
end

def squash(prefix, items)
  items.reduce('') { |result, item| result + " #{prefix}#{tackit(item)}" }
end

def build_command_string(tool_hash, values, defines = nil)
  args = []
  tool_hash[:arguments].each do |arg|
    if arg.include?('$')
      if arg.include?('${5}')
        all_paths = (($cfg[:paths][:support] || []) + ($cfg[:paths][:source] || []) + ($cfg[:paths][:test] || [])).uniq
        all_paths.each { |f| args << arg.gsub('${5}', f) }

      elsif arg.include?('${6}')
        (($cfg.dig(:defines, :test) || []) + Array(defines)).uniq.compact.each { |f| args << arg.gsub('${6}', f) }

      elsif arg.include?(': COLLECTION_PATHS_TEST_TOOLCHAIN_INCLUDE')
        pattern = arg.gsub(': COLLECTION_PATHS_TEST_TOOLCHAIN_INCLUDE', '')
        ($cfg[:paths][:support] || []).each { |f| args << pattern.gsub(/\$/, f) }

      elsif arg.include?(': COLLECTION_PATHS_TEST_SUPPORT_SOURCE_INCLUDE_VENDOR')
        pattern = arg.gsub(': COLLECTION_PATHS_TEST_SUPPORT_SOURCE_INCLUDE_VENDOR', '')
        (($cfg[:paths][:source] || []) + ($cfg[:paths][:test] || [])).uniq.each { |f| args << pattern.gsub(/\$/, f) }

      elsif arg.include?(': COLLECTION_DEFINES_TEST_AND_VENDOR')
        pattern = arg.gsub(': COLLECTION_DEFINES_TEST_AND_VENDOR', '')
        (($cfg.dig(:defines, :test) || []) + Array(defines)).uniq.compact.each { |f| args << pattern.gsub(/\$/, f) }

      elsif arg =~ /\$\{(\d+)\}/
        i = Regexp.last_match(1).to_i - 1
        if values[i].is_a?(Array)
          values[i].each { |v| args << arg.gsub(/\$\{\d+\}/, v) }
        else
          args << arg.gsub(/\$\{\d+\}/, values[i] || '')
        end

      else
        args << arg
      end
    else
      args << arg
    end
  end
  tackit(tool_hash[:executable]) + squash('', args)
end

def compile(file, defines = [])
  build_path = $cfg[:project][:build_root]
  out_file   = File.join(build_path, File.basename(file, C_EXTENSION)) + ($cfg[:extension][:object] || '.o')
  if $cfg[:tools][:test_compiler][:executable] =~ /gcc/
    $cfg[:tools][:test_compiler][:arguments] |= ['-Wno-misleading-indentation']
  end
  cmd_str    = build_command_string($cfg[:tools][:test_compiler], [file, out_file], defines)
  execute(cmd_str)
  out_file
end

def link_it(exe_name, obj_list)
  build_path = $cfg[:project][:build_root]
  exe_file   = File.join(build_path, File.basename(exe_name, '.*')) + ($cfg[:extension][:executable] || '')
  cmd_str    = build_command_string($cfg[:tools][:test_linker], [obj_list, exe_file])
  execute(cmd_str)
  exe_file
end

def execute(command_string, verbose = true, raise_on_fail = true)
  report command_string
  output = `#{command_string}`.chomp
  report(output) if verbose && !output.nil? && !output.empty?
  raise "Command failed. (Returned #{$?.exitstatus})" if !$?.nil? && !$?.exitstatus.zero? && raise_on_fail

  output
end

def report_summary
  summary = UnityTestSummary.new
  summary.root = __dir__
  results_glob = File.join($cfg[:project][:build_root], '*.test*').tr('\\', '/')
  summary.targets = Dir[results_glob]
  summary.run
  fail_out 'FAIL: There were failures' if summary.failures > 0
end

def run_tests(test_files)
  report 'Running system tests...'

  include_dirs = local_include_dirs
  build_path   = $cfg[:project][:build_root]

  test_files.each do |test|
    obj_list = []

    extract_headers(test).each do |header|
      src_file = find_source_file(header, include_dirs)
      obj_list << compile(src_file) unless src_file.nil?
    end

    test_base   = File.basename(test, C_EXTENSION)
    runner_path = File.join(build_path, "#{test_base}_Runner.c")
    UnityTestRunnerGenerator.new($cfg[:unity] || {}).run(test, runner_path)
    obj_list << compile(runner_path)

    obj_list << compile(test)

    exe_file = link_it(test_base, obj_list)

    cmd_str = if $cfg[:tools] && $cfg[:tools][:test_fixture]
                build_command_string($cfg[:tools][:test_fixture], [exe_file, ''])
              else
                exe_file
              end
    output = execute(cmd_str, true, false)

    test_results  = File.join(build_path, test_base)
    test_results += output.match(/OK$/m).nil? ? '.testfail' : '.testpass'
    File.open(test_results, 'w') { |f| f.print output }
  end
end

def fail_out(msg)
  puts msg
  puts 'Not returning exit code so continuous integration can pass'
  #    exit(-1) # Only removed to pass example_3, which has failing tests on purpose.
  #               Still fail if the build fails for any other reason.
end
