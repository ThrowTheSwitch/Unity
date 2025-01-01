# =========================================================================
#   Unity - A Test Framework for C
#   ThrowTheSwitch.org
#   Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
#   SPDX-License-Identifier: MIT
# =========================================================================

#============================================================
#  Author: John Theofanopoulos
#  A simple parser. Takes the output files generated during the
#  build process and extracts information relating to the tests.
#
#  Notes:
#    To capture an output file under VS builds use the following:
#      devenv [build instructions] > Output.txt & type Output.txt
#
#    To capture an output file under Linux builds use the following:
#      make | tee Output.txt
#
#    This script can handle the following output formats:
#    - normal output (raw unity)
#    - fixture output (unity_fixture.h/.c)
#    - fixture output with verbose flag set ("-v")
#    - time output flag set (UNITY_INCLUDE_EXEC_TIME define enabled with milliseconds output)
#
#    To use this parser use the following command
#    ruby parseOutput.rb [options] [file]
#        options: -xml  : produce a JUnit compatible XML file
#                 -suiteRequiredSuiteName
#                       : replace default test suite name to
#                           "RequiredSuiteName" (can be any name)
#           file: file to scan for results
#============================================================

# Parser class for handling the input file
class ParseOutput
  def initialize
    # internal data
    @class_name_idx = 0
    @result_usual_idx = 3
    @path_delim = nil

    # xml output related
    @xml_out = false
    @array_list = false

    # current suite name and statistics
    ## testsuite name
    @real_test_suite_name = 'Unity'
    ## classname for testcase
    @test_suite = nil
    @total_tests = 0
    @test_passed = 0
    @test_failed = 0
    @test_ignored = 0
  end

  # Set the flag to indicate if there will be an XML output file or not
  def set_xml_output
    @xml_out = true
  end

  # Set the flag to indicate if there will be an XML output file or not
  def test_suite_name=(cli_arg)
    @real_test_suite_name = cli_arg
    puts "Real test suite name will be '#{@real_test_suite_name}'"
  end

  def xml_encode_s(str)
    str.encode(:xml => :attr)
  end

  # If write our output to XML
  def write_xml_output
    output = File.open('report.xml', 'w')
    output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    @array_list.each do |item|
      output << item << "\n"
    end
  end

  # Pushes the suite info as xml to the array list, which will be written later
  def push_xml_output_suite_info
    # Insert opening tag at front
    heading = "<testsuite name=#{xml_encode_s(@real_test_suite_name)} tests=\"#{@total_tests}\" failures=\"#{@test_failed}\" skips=\"#{@test_ignored}\">"
    @array_list.insert(0, heading)
    # Push back the closing tag
    @array_list.push '</testsuite>'
  end

  # Pushes xml output data to the array list, which will be written later
  def push_xml_output_passed(test_name, execution_time = 0)
    @array_list.push "    <testcase classname=#{xml_encode_s(@test_suite)} name=#{xml_encode_s(test_name)} time=#{xml_encode_s((execution_time / 1000.0).to_s)} />"
  end

  # Pushes xml output data to the array list, which will be written later
  def push_xml_output_failed(test_name, reason, execution_time = 0)
    @array_list.push "    <testcase classname=#{xml_encode_s(@test_suite)} name=#{xml_encode_s(test_name)} time=#{xml_encode_s((execution_time / 1000.0).to_s)} >"
    @array_list.push "        <failure type=\"ASSERT FAILED\">#{reason}</failure>"
    @array_list.push '    </testcase>'
  end

  # Pushes xml output data to the array list, which will be written later
  def push_xml_output_ignored(test_name, reason, execution_time = 0)
    @array_list.push "    <testcase classname=#{xml_encode_s(@test_suite)} name=#{xml_encode_s(test_name)} time=#{xml_encode_s((execution_time / 1000.0).to_s)} >"
    @array_list.push "        <skipped type=\"TEST IGNORED\">#{reason}</skipped>"
    @array_list.push '    </testcase>'
  end

  # This function will try and determine when the suite is changed. This is
  # is the name that gets added to the classname parameter.
  def test_suite_verify(test_suite_name)
    # Split the path name
    test_name = test_suite_name.split(@path_delim)

    # Remove the extension and extract the base_name
    base_name = test_name[test_name.size - 1].split('.')[0]

    # Return if the test suite hasn't changed
    return unless base_name.to_s != @test_suite.to_s

    @test_suite = base_name
    printf "New Test: %s\n", @test_suite
  end

  # Prepares the line for verbose fixture output ("-v")
  def prepare_fixture_line(line)
    line = line.sub('IGNORE_TEST(', '')
    line = line.sub('TEST(', '')
    line = line.sub(')', ',')
    line = line.chomp
    array = line.split(',')
    array.map { |x| x.to_s.lstrip.chomp }
  end

  # Test was flagged as having passed so format the output.
  # This is using the Unity fixture output and not the original Unity output.
  def test_passed_unity_fixture(array)
    class_name = array[0]
    test_name  = array[1]
    test_suite_verify(class_name)
    printf "%-40s PASS\n", test_name

    push_xml_output_passed(test_name) if @xml_out
  end

  # Test was flagged as having failed so format the output.
  # This is using the Unity fixture output and not the original Unity output.
  def test_failed_unity_fixture(array)
    class_name = array[0]
    test_name  = array[1]
    test_suite_verify(class_name)
    reason_array = array[2].split(':')
    reason = "#{reason_array[-1].lstrip.chomp} at line: #{reason_array[-4]}"

    printf "%-40s FAILED\n", test_name

    push_xml_output_failed(test_name, reason) if @xml_out
  end

  # Test was flagged as being ignored so format the output.
  # This is using the Unity fixture output and not the original Unity output.
  def test_ignored_unity_fixture(array)
    class_name = array[0]
    test_name  = array[1]
    reason = 'No reason given'
    if array.size > 2
      reason_array = array[2].split(':')
      tmp_reason = reason_array[-1].lstrip.chomp
      reason = tmp_reason == 'IGNORE' ? 'No reason given' : tmp_reason
    end
    test_suite_verify(class_name)
    printf "%-40s IGNORED\n", test_name

    push_xml_output_ignored(test_name, reason) if @xml_out
  end

  # Test was flagged as having passed so format the output
  def test_passed(array)
    # ':' symbol will be valid in function args now
    real_method_name = array[@result_usual_idx - 1..-2].join(':')
    array = array[0..@result_usual_idx - 2] + [real_method_name] + [array[-1]]

    last_item = array.length - 1
    test_time = get_test_time(array[last_item])
    test_name = array[last_item - 1]
    test_suite_verify(array[@class_name_idx])
    printf "%-40s PASS %10d ms\n", test_name, test_time

    return unless @xml_out

    push_xml_output_passed(test_name, test_time) if @xml_out
  end

  # Test was flagged as having failed so format the line
  def test_failed(array)
    # ':' symbol will be valid in function args now
    real_method_name = array[@result_usual_idx - 1..-3].join(':')
    array = array[0..@result_usual_idx - 3] + [real_method_name] + array[-2..]

    last_item = array.length - 1
    test_time = get_test_time(array[last_item])
    test_name = array[last_item - 2]
    reason = "#{array[last_item].chomp.lstrip} at line: #{array[last_item - 3]}"
    class_name = array[@class_name_idx]

    if test_name.start_with? 'TEST('
      array2 = test_name.split(' ')

      test_suite = array2[0].sub('TEST(', '')
      test_suite = test_suite.sub(',', '')
      class_name = test_suite

      test_name = array2[1].sub(')', '')
    end

    test_suite_verify(class_name)
    printf "%-40s FAILED %10d ms\n", test_name, test_time

    push_xml_output_failed(test_name, reason, test_time) if @xml_out
  end

  # Test was flagged as being ignored so format the output
  def test_ignored(array)
    # ':' symbol will be valid in function args now
    real_method_name = array[@result_usual_idx - 1..-3].join(':')
    array = array[0..@result_usual_idx - 3] + [real_method_name] + array[-2..]

    last_item = array.length - 1
    test_time = get_test_time(array[last_item])
    test_name = array[last_item - 2]
    reason = array[last_item].chomp.lstrip
    class_name = array[@class_name_idx]

    if test_name.start_with? 'TEST('
      array2 = test_name.split(' ')

      test_suite = array2[0].sub('TEST(', '')
      test_suite = test_suite.sub(',', '')
      class_name = test_suite

      test_name = array2[1].sub(')', '')
    end

    test_suite_verify(class_name)
    printf "%-40s IGNORED %10d ms\n", test_name, test_time

    push_xml_output_ignored(test_name, reason, test_time) if @xml_out
  end

  # Test time will be in ms
  def get_test_time(value_with_time)
    test_time_array = value_with_time.scan(/\((-?\d+.?\d*) ms\)\s*$/).flatten.map do |arg_value_str|
      arg_value_str.include?('.') ? arg_value_str.to_f : arg_value_str.to_i
    end

    test_time_array.any? ? test_time_array[0] : 0
  end

  # Adjusts the os specific members according to the current path style
  # (Windows or Unix based)
  def detect_os_specifics(line)
    if line.include? '\\'
      # Windows X:\Y\Z
      @class_name_idx = 1
      @path_delim = '\\'
    else
      # Unix Based /X/Y/Z
      @class_name_idx = 0
      @path_delim = '/'
    end
  end

  # Main function used to parse the file that was captured.
  def process(file_name)
    @array_list = []

    puts "Parsing file: #{file_name}"

    @test_passed = 0
    @test_failed = 0
    @test_ignored = 0
    puts ''
    puts '=================== RESULTS ====================='
    puts ''
    # Apply binary encoding. Bad symbols will be unchanged
    File.open(file_name, 'rb').each do |line|
      # Typical test lines look like these:
      # ----------------------------------------------------
      # 1. normal output:
      # <path>/<test_file>.c:36:test_tc1000_opsys:FAIL: Expected 1 Was 0
      # <path>/<test_file>.c:112:test_tc5004_initCanChannel:IGNORE: Not Yet Implemented
      # <path>/<test_file>.c:115:test_tc5100_initCanVoidPtrs:PASS
      #
      # 2. fixture output
      # <path>/<test_file>.c:63:TEST(<test_group>, <test_function>):FAIL: Expected 0x00001234 Was 0x00005A5A
      # <path>/<test_file>.c:36:TEST(<test_group>, <test_function>):IGNORE
      # Note: "PASS" information won't be generated in this mode
      #
      # 3. fixture output with verbose information ("-v")
      # TEST(<test_group, <test_file>)<path>/<test_file>:168::FAIL: Expected 0x8D Was 0x8C
      # TEST(<test_group>, <test_file>)<path>/<test_file>:22::IGNORE: This Test Was Ignored On Purpose
      # IGNORE_TEST(<test_group, <test_file>)
      # TEST(<test_group, <test_file>) PASS
      #
      # Note: Where path is different on Unix vs Windows devices (Windows leads with a drive letter)!
      detect_os_specifics(line)
      line_array = line.split(':')

      # If we were able to split the line then we can look to see if any of our target words
      # were found. Case is important.
      next unless (line_array.size >= 4) || (line.start_with? 'TEST(') || (line.start_with? 'IGNORE_TEST(')

      # check if the output is fixture output (with verbose flag "-v")
      if (line.start_with? 'TEST(') || (line.start_with? 'IGNORE_TEST(')
        line_array = prepare_fixture_line(line)
        if line.include? ' PASS'
          test_passed_unity_fixture(line_array)
          @test_passed += 1
        elsif line.include? 'FAIL'
          test_failed_unity_fixture(line_array)
          @test_failed += 1
        elsif line.include? 'IGNORE'
          test_ignored_unity_fixture(line_array)
          @test_ignored += 1
        end
      # normal output / fixture output (without verbose "-v")
      elsif line.include? ':PASS'
        test_passed(line_array)
        @test_passed += 1
      elsif line.include? ':FAIL'
        test_failed(line_array)
        @test_failed += 1
      elsif line.include? ':IGNORE:'
        test_ignored(line_array)
        @test_ignored += 1
      elsif line.include? ':IGNORE'
        line_array.push('No reason given')
        test_ignored(line_array)
        @test_ignored += 1
      elsif line_array.size >= 4
        # We will check output from color compilation
        if line_array[@result_usual_idx..].any? { |l| l.include? 'PASS' }
          test_passed(line_array)
          @test_passed += 1
        elsif line_array[@result_usual_idx..].any? { |l| l.include? 'FAIL' }
          test_failed(line_array)
          @test_failed += 1
        elsif line_array[@result_usual_idx..-2].any? { |l| l.include? 'IGNORE' }
          test_ignored(line_array)
          @test_ignored += 1
        elsif line_array[@result_usual_idx..].any? { |l| l.include? 'IGNORE' }
          line_array.push("No reason given (#{get_test_time(line_array[@result_usual_idx..])} ms)")
          test_ignored(line_array)
          @test_ignored += 1
        end
      end
      @total_tests = @test_passed + @test_failed + @test_ignored
    end
    puts ''
    puts '=================== SUMMARY ====================='
    puts ''
    puts "Tests Passed  : #{@test_passed}"
    puts "Tests Failed  : #{@test_failed}"
    puts "Tests Ignored : #{@test_ignored}"

    return unless @xml_out

    # push information about the suite
    push_xml_output_suite_info
    # write xml output file
    write_xml_output
  end
end

# If the command line has no values in, used a default value of Output.txt
parse_my_file = ParseOutput.new

if ARGV.size >= 1
  ARGV.each do |arg|
    if arg == '-xml'
      parse_my_file.set_xml_output
    elsif arg.start_with?('-suite')
      parse_my_file.test_suite_name = arg.delete_prefix('-suite')
    else
      parse_my_file.process(arg)
      break
    end
  end
end
