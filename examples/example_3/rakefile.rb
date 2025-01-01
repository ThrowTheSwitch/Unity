# =========================================================================
#   Unity - A Test Framework for C
#   ThrowTheSwitch.org
#   Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
#   SPDX-License-Identifier: MIT
# =========================================================================

require 'rake'
require 'rake/clean'
require_relative 'rakefile_helper'

TEMP_DIRS = [
  File.join(__dir__, 'build')
].freeze

TEMP_DIRS.each do |dir|
  directory(dir)
  CLOBBER.include(dir)
end

task prepare_for_tests: TEMP_DIRS

# Load default configuration, for now
DEFAULT_CONFIG_FILE = 'target_gcc_32.yml'.freeze
configure_toolchain(DEFAULT_CONFIG_FILE)

task unit: [:prepare_for_tests] do
  run_tests unit_test_files
end

desc 'Generate test summary'
task :summary do
  report_summary
end

desc 'Build and test Unity'
task all: %i[clean unit summary]
task default: %i[clobber all]
task ci: [:default]
task cruise: [:default]

desc 'Load configuration'
task :config, :config_file do |_t, args|
  configure_toolchain(args[:config_file])
end
