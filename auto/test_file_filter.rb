require'yaml'

module RakefileHelpers
  class TestFileFilter
    def initialize(all_files = false)
      @all_files = all_files
      if not @all_files == true
        if File.exist?('test_file_filter.yml')
          filters = YAML.load_file( 'test_file_filter.yml' )
          @all_files, @only_files, @exclude_files = 
            filters[:all_files], filters[:only_files], filters[:exclude_files] 
        end
      end
    end		
    attr_accessor :all_files, :only_files, :exclude_files
  end
end
