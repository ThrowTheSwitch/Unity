#!/usr/bin/ruby
#
# unity_test_summary.rb
#
require 'fileutils'
require 'set'

class UnityTestSummary
  include FileUtils::Verbose

  attr_reader :report, :total_tests, :failures, :ignored
  
  def initialize
    @report = ''
    @total_tests = 0
    @failures = 0
    @ignored = 0
  end
  
  def run
    # Clean up result file names
    results = @targets.map {|target| target.gsub(/\\/,'/')}

    # Dig through each result file, looking for details on pass/fail:   
    failure_output = ""
    ignore_output = ""
    
    results.each do |result_file|
      lines = File.readlines(result_file).map { |line| line.chomp }
      if lines.length == 0
        raise "Empty test result file: #{result_file}"
      else
        summary_line = -2
        output = get_details(result_file, lines)
        failure_output += output[:failures] if !output[:failures].empty?
        ignore_output += output[:ignores] if !output[:ignores].empty?
        tests,failures,ignored = parse_test_summary(lines[summary_line])
        @total_tests += tests
        @failures += failures
        @ignored += ignored
      end
    end
    
    if @ignored > 0
      @report += "\n"
      @report += "--------------------------\n"
      @report += "UNITY IGNORED TEST SUMMARY\n"
      @report += "--------------------------\n"
      @report += ignore_output
    end
    
    if @failures > 0
      @report += "\n"
      @report += "--------------------------\n"
      @report += "UNITY FAILED TEST SUMMARY\n"
      @report += "--------------------------\n"
      @report += failure_output
    end
  
    @report += "\n"
    @report += "--------------------------\n"
    @report += "OVERALL UNITY TEST SUMMARY\n"
    @report += "--------------------------\n"
    @report += "TOTAL TESTS: #{@total_tests} TOTAL FAILURES: #{@failures} IGNORED: #{@ignored}\n"
    @report += "\n"
  end
  
  def set_targets(target_array)
    @targets = target_array
  end
  
  def set_root_path(path)
    @root = path
  end

  def usage(err_msg=nil)
    puts err_msg if err_msg
    puts "Usage: unity_test_summary.rb"
    exit 1
  end
  
  protected
  
  @@targets=nil
  @@path=nil
  @@root=nil

  def get_details(result_file, lines)
    fail_lines = [] # indices of lines with failures
    ignore_lines = [] # indices of lines with ignores
    lines.each_with_index do |line,i|
      if (i < (lines.length - 2) && !(line =~ /PASS$/))
        if line =~ /(^.*\.c):(\d+)/
          if line =~ /IGNORED$/
            ignore_lines << i
          else
            fail_lines << i
          end
        elsif line =~ /IGNORED$/
          ignore_lines << i        
        end
      end
    end
    
    failures = []
    fail_lines.each do |fail_line|
      if lines[fail_line] =~ /\w:/
        src_file,src_line,test_name,msg = lines[fail_line].split(/:/)
        src_file = "#{@root}#{src_file}" unless @root == nil || @root.length == 0
        detail = "#{src_file}:#{src_line}:#{test_name}:: #{msg}"
        failures << detail.gsub(/\//, "\\")
      end
    end
    if failures.length == 0
      failure_results = ""
    else
      failure_results = failures.join("\n") + "\n"
    end
    
    ignores = []
    ignore_lines.each do |ignore_line|
      if lines[ignore_line] =~ /\w:/
        src_file,src_line,test_name,msg = lines[ignore_line].split(/:/)
        src_file = "#{@root}#{src_file}" unless @root == nil || @root.length == 0
        detail = "#{src_file}:#{src_line}:#{test_name}:: #{msg}"
        ignores << detail.gsub(/\//, "\\")
      end
    end
    if ignores.length == 0
      ignore_results = ""
    else
      ignore_results = ignores.join("\n") + "\n"
    end
    
    results = {:failures => failure_results, :ignores => ignore_results}
  end
  
  def parse_test_summary(summary)
    if summary =~ /(\d+) Tests (\d+) Failures (\d+) Ignored/
      [$1.to_i,$2.to_i,$3.to_i]
    else
      raise "Couldn't parse test results: #{summary}"
    end
  end

  def here; File.expand_path(File.dirname(__FILE__)); end
  
end

if $0 == __FILE__
  script = UnityTestSummary.new
  begin
    script.run
  rescue Exception => e
    script.usage e.message
  end
end
