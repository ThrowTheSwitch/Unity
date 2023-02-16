# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

require_relative 'yaml_helper'

module RakefileHelpers
  class TestFileFilter
    def initialize(all_files = false)
      @all_files = all_files

      return unless @all_files

      file = 'test_file_filter.yml'
      return unless File.exist?(file)

      filters = YamlHelper.load_file(file)
      @all_files = filters[:all_files]
      @only_files = filters[:only_files]
      @exclude_files = filters[:exclude_files]
    end

    attr_accessor :all_files, :only_files, :exclude_files
  end
end
